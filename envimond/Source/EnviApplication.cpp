 /*
  ==============================================================================

    EnviApplication.cpp
    Created: 2 Dec 2013 1:54:30pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviApplication.h"
#include "EnviDSCommand.h"
#include "EnviDSBMP085.h"
#include "EnviDSDHT11.h"

EnviApplication::EnviApplication(int argc, char* argv[])
	: enviCLI(argc, argv), valid(true)
{
	EnviLog::getInstance()->setOwner(this);
	if (enviCLI.isSet("log-console"))
	{
		EnviLog::getInstance()->setLogToConsole(true);
	}

	_DBG(enviCLI.getAllArguments().getDescription());

	/*
	 * Register all data sources here
	 */
	dataSources.add (new EnviDSDHT11(*this));
	dataSources.add (new EnviDSCommand(*this));
	dataSources.add (new EnviDSBMP085(*this));

	if (enviCLI.isSet("help"))
	{
		enviCLI.printHelp();
		valid = false;
		return;
	}

	if (enviCLI.isSet("list-sources"))
	{
		std::cout << "Available data sources:\n";
		for (int i=0; i<dataSources.size(); i++)
		{
			std::cout << "\t" << dataSources[i]->getType().toString() << std::endl;
		}
		valid = false;
		return;
	}

	if (enviCLI.isSet("log-file"))
	{
		Result res = EnviLog::getInstance()->setLogToFile (enviCLI.getParameter("log-file"));

		if (!res.wasOk())
		{
			_ERR("Can't write to specified log file: ["+res.getErrorMessage()+"]");
			valid = false;
			return;
		}
	}

	if (enviCLI.isSet("enable-sources"))
	{
		allowedSources = StringArray::fromTokens(enviCLI.getParameter("enable-sources"), ",", "'\"");
	}

	PropertiesFile::Options options;
	options.applicationName		= "envi";
	options.filenameSuffix		= "xml";

#ifdef JUCE_LINUX
	options.folderName			= ".envi";
#else
	options.folderName			= "envi";
#endif

	options.storageFormat 		= PropertiesFile::storeAsXML;
	applicationProperties.setStorageParameters (options);

	/*
	 *	Envi application classes
	 */
	enviHTTP	= new EnviHTTP(*this);
	enviDB		= new EnviDB(*this);

#ifdef JUCE_LINUX
	enviWiringPi	= new EnviWiringPi(*this);
#endif
}

EnviApplication::~EnviApplication()
{
}

EnviCLI &EnviApplication::getCLI()
{
	return (enviCLI);
}

ApplicationProperties &EnviApplication::getApplicationProperties()
{
	return (applicationProperties);
}

const Result EnviApplication::runDispatchLoop()
{
	_DBG("EnviApplication::runDispatchLoop");

	const Result sourcesResult = findDataSourcesOnDisk();

	if (sourcesResult.wasOk())
	{
		if (enviDB->init())
		{
			MessageManager::getInstance()->runDispatchLoop();
			return (Result::ok());
		}
		else
		{
			return (Result::fail("Can't initialize EnivDB"));
		}
	}
	else
	{
		return (sourcesResult);
	}
}

void EnviApplication::cleanupAndExit()
{
}

void EnviApplication::removeDataSource(EnviDataSource *sourceToRemove)
{
	ScopedLock sl(dataSources.getLock());
	dataSources.removeObject (sourceToRemove);
}

const int EnviApplication::getNumDataSources()
{
	return (dataSources.size());
}

EnviDataSource *EnviApplication::getDataSource(const int index)
{
	return (dataSources[index]);
}

const File EnviApplication::getPropertiesFolder()
{
	if (applicationProperties.getUserSettings())
	{
		return (applicationProperties.getUserSettings()->getFile().getParentDirectory());
	}
	return (File::getSpecialLocation(File::userApplicationDataDirectory));
}

PropertySet *EnviApplication::getProperties()
{
	if (applicationProperties.getUserSettings())
	{
		return (applicationProperties.getUserSettings());
	}

	return (&defaultPropertyStorage);
}

void EnviApplication::timerCallback(int timerId)
{
	ScopedLock sl(dataSources.getLock());
	EnviDataSource *ds = dataSources[timerId - ENVI_TIMER_OFFSET];
	if (ds)
	{
		if (ds->isDisabled())
		{
			_WRN("Timer triggered for data source \""+ds->getName()+"\". Source is disabled");
			return;
		}

		_LOG(LOG_INFO, "Timer triggered for data source \""+ds->getName()+"\"");

		if (!ds->startSource())
		{
			_LOG(LOG_WARN, "Timer trigger for data source \""+ds->getName()+"\", execute failed");
		}
	}
}

void EnviApplication::addDataSource(EnviDataSource *sourceToAdd)
{
	ScopedLock sl(dataSources.getLock());
	registerDataSource (dataSources.add (sourceToAdd));
}

EnviDataSource *EnviApplication::checkForValidInstance(const ValueTree dataSourceInstance)
{
	const String type = dataSourceInstance.getProperty(Ids::type);

	if (allowedSources.indexOf (type) >= 0)
	{
		_WRN("Data source type: ["+type+"] disabled on command line");
		return (nullptr);
	}
	else
	{
		for (int i=0; i<dataSources.size(); i++)
		{
			if (dataSources[i]->getType() == type)
			{
			}
		}
	}
}

EnviDataSource *EnviApplication::createInstance(const File &sourceState)
{
	ScopedPointer <XmlElement> xml(XmlDocument::parse (sourceState));

	if (xml)
	{
		ValueTree instanceState = ValueTree::fromXml (*xml);

		if (instanceState.isValid())
		{
			return (checkForValidInstance (instanceState));
		}
		else
		{
			_WRN("Invalid data source configuration file: "+sourceState.getFullPathName());
			return (nullptr);
		}
	}

	return (nullptr);
}

EnviDataSource *EnviApplication::registerDataSource(EnviDataSource *dataSource)
{
	if (dataSource != nullptr)
	{
		ScopedLock sl(dataSources.getLock());
		startTimer (ENVI_TIMER_OFFSET + dataSources.indexOf(dataSource), dataSource->getInterval());
	}

	return (dataSource);
}

const Result EnviApplication::findDataSourcesOnDisk()
{
	if (applicationProperties.getUserSettings() == nullptr)
	{
		return (Result::fail("Not ready yet"));
	}

	File dataSourcesFolder (enviCLI.isSet("sources-dir") ? enviCLI.getParameter("sources-dir") : applicationProperties.getUserSettings()->getFile().getParentDirectory());

	if (dataSourcesFolder.isDirectory())
	{
		_INF("Data sources directory set to: "+dataSourcesFolder.getFullPathName());

		Array <File> sourceXmls;

		const int ret = dataSourcesFolder.findChildFiles (sourceXmls, File::findFiles, false, "*.xml");

		if (ret > 0)
		{
			for (int i=0; i<sourceXmls.size(); i++)
			{
				EnviDataSource *ds = createInstance (sourceXmls[i]);

				if (ds)
				{
					addDataSource (ds);
				}
			}

			return (Result::ok());
		}
		else
		{
			return (Result::fail ("Found 0 sources in sources directory: "+dataSourcesFolder.getFullPathName()));
		}
	}
	else
	{
		return (Result::fail (dataSourcesFolder.getFullPathName() + " is not a directory, can't initialize any sources"));
	}
}

void EnviApplication::sourceWrite(EnviDataSource *dataSource, const Result &failureReason)
{
	if (failureReason.wasOk())
	{
		enviDB->writeResult (dataSource);
	}
	else
	{
		_WRN("Data source: ["+dataSource->getName()+"] instance: "+_STR(dataSource->getInstanceNumber())+"] has failed: ["+failureReason.getErrorMessage()+"]");
	}
}

const var EnviApplication::getOption(const Identifier &optionId)
{
	return (var::null);
}

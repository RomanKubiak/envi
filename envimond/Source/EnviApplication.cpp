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

const bool EnviApplication::isValid()
{
	ScopedLock sl(dataSources.getLock());
	return (valid);
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
	ScopedLock sl(dataSources.getLock());
	return (dataSources.size());
}

EnviDataSource *EnviApplication::getDataSource(const int index)
{
	ScopedLock sl(dataSources.getLock());
	return (dataSources[index]);
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

		_INF("Timer triggered for data source type: ["+ds->getType().toString()+"] name: ["+ds->getName()+"] instance: ["+_STR(ds->getInstanceNumber())+"]");

		if (!ds->startSource())
		{
			_LOG(LOG_WARN, "Timer trigger for data source \""+ds->getName()+"\", execute failed");
		}
	}
}

EnviDataSource *EnviApplication::createInstance(const ValueTree dataSourceInstance)
{
	const String type = dataSourceInstance.getProperty(Ids::type);

	if (allowedSources.indexOf (type) >= 0)
	{
		_WRN("Data source type: ["+type+"] disabled on command line");
		return (nullptr);
	}
	else
	{
		EnviDataSource *ds = getInstanceFromType(dataSourceInstance.getProperty(Ids::type).toString());

		if (ds != nullptr)
		{
			ScopedLock sl(dataSources.getLock());
			dataSources.add (ds);

			ds->initialize (dataSourceInstance);
			ds->setInstanceNumber (getNumInstances(type));

			startTimer (ENVI_TIMER_OFFSET + dataSources.indexOf(ds), ds->getInterval());
		}
		else
		{
			_WRN("Unknown data source type in XML: ["+type+"]");
			return (nullptr);
		}
		return (ds);
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
			_INF(sourceState.getFileName()+" looks like a valid data source defintion.");
			return (createInstance (instanceState));
		}
		else
		{
			_WRN(sourceState.getFileName()+" is an invalid data source definition (not XML).");
			return (nullptr);
		}
	}
	else
	{
		_WRN(sourceState.getFileName()+" can't be parsed as a XML file.");
	}

	return (nullptr);
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
				if (createInstance (sourceXmls[i]) == nullptr)
				{
					return (Result::fail("Can't create all data sources"));
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

EnviDataSource *EnviApplication::getInstanceFromType(const Identifier dsType)
{
	if (dsType == Ids::dht11)
	{
		return (new EnviDSDHT11(*this));
	}
	else if (dsType == Ids::cmd)
	{
		return (new EnviDSCommand(*this));
	}
	else if (dsType == Ids::bmp085)
	{
		return (new EnviDSBMP085(*this));
	}

	return (nullptr);
}

const int EnviApplication::getNumInstances(const Identifier dsType)
{
	ScopedLock sl(dataSources.getLock());

	int instances = 0;

	for (int i=0; i<dataSources.size(); i++)
	{
		if (dataSources[i]->getType() == dsType)
			instances++;
	}

	return (instances);
}

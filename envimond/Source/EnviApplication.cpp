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
	_DBG(enviCLI.getAllArguments().getDescription());

	if (enviCLI.isSet("help"))
	{
		enviCLI.printHelp();
		valid = false;
		return;
	}

	if (enviCLI.isSet("list-sources"))
	{
		std::cout << "List sources:\n\n";
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
	EnviLog::getInstance()->setOwner(this);

	enviHTTP	= new EnviHTTP(*this);
	enviDB		= new EnviDB(*this);

#ifdef JUCE_LINUX
	enviWiringPi	= new EnviWiringPi(*this);
#endif
}

EnviApplication::~EnviApplication()
{
}

const Result EnviApplication::runDispatchLoop()
{
	_DBG("EnviApplication::runDispatchLoop");

	if (registerDataSources())
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
		return (Result::fail("Can't register data sources"));
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

EnviDataSource *EnviApplication::createInstance(const File &sourceState)
{
	ScopedPointer <XmlElement> xml(XmlDocument::parse (sourceState));

	if (xml)
	{
		ValueTree instanceState = ValueTree::fromXml (*xml);

		if (instanceState.isValid())
		{
			return (createInstance (instanceState));
		}
	}

	return (nullptr);
}

EnviDataSource *EnviApplication::createInstance(const ValueTree dataSourceInstance)
{
	if (dataSourceInstance.getProperty (Ids::type) == "command")
	{
        return (new EnviDSCommand (*this, dataSourceInstance));
	}

	if (dataSourceInstance.getProperty (Ids::type) == "bmp085")
	{
        return (new EnviDSBMP085 (*this, dataSourceInstance));
	}

	if (dataSourceInstance.getProperty (Ids::type) == "dht11")
	{
        return (new EnviDSDHT11 (*this, dataSourceInstance));
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

bool EnviApplication::registerDataSources()
{
	File dataSourcesFolder = getPropertiesFolder().getChildFile("sources");
	if (dataSourcesFolder.isDirectory())
	{
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
				else
				{
				}
			}

			return (true);
		}
		else
		{
			return (false);
		}
	}
	else
	{
		_LOG(LOG_ERROR, "\t"+dataSourcesFolder.getFullPathName() + " is not a directory, can't initialize any sources");
		return (false);
	}
}

void EnviApplication::sourceFailed(EnviDataSource *dataSource)
{
}

void EnviApplication::sourceWrite(EnviDataSource *dataSource)
{
	enviDB->writeResult (dataSource);
}

const var EnviApplication::getOption(const Identifier &optionId)
{
	return (var::null);
}

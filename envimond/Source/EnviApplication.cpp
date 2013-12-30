 /*
  ==============================================================================

    EnviApplication.cpp
    Created: 2 Dec 2013 1:54:30pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviApplication.h"
#include "EnviDSCommand.h"
#include "EnviDSDHT22.h"
#include "EnviDSDHT11.h"
#include "EnviDSPCF8591.h"
#include "EnviDSFile.h"
#include "EnviDSTrigger.h"
#include "EnviDSLua.h"

EnviApplication::EnviApplication(int argc, char* argv[])
	: enviCLI(nullptr), valid(true)
{
	enviCLI = new EnviCLI (argc, argv);

	EnviLog::getInstance()->setOwner(this);
	if (enviCLI->isSet("log-console"))
	{
		EnviLog::getInstance()->setLogToConsole(true);
	}

	_DBG(enviCLI->getAllArguments().getDescription());

	if (enviCLI->isSet("help"))
	{
		enviCLI->printHelp();
		valid = false;
		return;
	}

	if (enviCLI->isSet("log-file"))
	{
		Result res = EnviLog::getInstance()->setLogToFile (getEnviLogFile());

		if (!res.wasOk())
		{
			_ERR("Can't write to specified log file: ["+res.getErrorMessage()+"]");
			valid = false;
			return;
		}
	}

	if (enviCLI->isSet("log-level"))
	{
		EnviLog::getInstance()->setLogLevel (enviCLI->getParameter("log-level").getIntValue());
	}

	if (enviCLI->isSet("disabled-sources"))
	{
		disabledSources = StringArray::fromTokens(enviCLI->getParameter("disabled-sources"), ",", "'\"");
	}

	Result res = findDataSourcesOnDisk();

	if (!res.wasOk())
	{
		_ERR("Can't initialize sources: ["+res.getErrorMessage()+"]");
		valid = false;
		return;
	}

	/*
	 *	Envi application classes
	 */
	enviHTTP	    = new EnviHTTP(*this);
	enviDB		    = new EnviDB(*this);

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
	return (*enviCLI);
}

const Result EnviApplication::runDispatchLoop()
{
	_DBG("EnviApplication::runDispatchLoop");
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

		_DBG("Timer triggered for data source type: ["+ds->getType()+"] name: ["+ds->getName()+"] instance: ["+_STR(ds->getInstanceNumber())+"]");

		Result res = ds->startSource();
		if (!res.wasOk())
		{
			_WRN("Timer trigger for data source \""+ds->getName()+"\", execute failed ["+res.getErrorMessage()+"]");
		}
		else
		{
			_DBG(ds->getType()+"] name: ["+ds->getName()+"] started");
		}
	}
}

EnviDataSource *EnviApplication::createInstance(const ValueTree dataSourceInstance)
{
	const String type = dataSourceInstance.getProperty(Ids::type);

	if (disabledSources.indexOf (type) >= 0 && enviCLI->isSet("disabled-sources"))
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
	File dataSourcesFolder (getEnviSourcesDir());

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
					continue;
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
	else if (dsType == Ids::dht22)
	{
		return (new EnviDSDHT22(*this));
	}
	else if (dsType == Ids::pcf8591)
	{
		return (new EnviDSPCF8591(*this));
	}
	else if (dsType == Ids::file)
	{
		return (new EnviDSFile(*this));
	}
	else if (dsType == Ids::trigger)
	{
		return (new EnviDSTrigger(*this));
	}
	else if (dsType == Ids::lua)
	{
		return (new EnviDSLua(*this));
	}
	return (nullptr);
}

const int EnviApplication::getNumInstances(const Identifier dsType)
{
	ScopedLock sl(dataSources.getLock());

	int instances = 0;

	for (int i=0; i<dataSources.size(); i++)
	{
		if (dataSources[i]->getType() == dsType.toString())
			instances++;
	}

	return (instances);
}

const File EnviApplication::getEnviSourcesDir()
{
	if (enviCLI->isSet("sources-dir"))
	{
		return (File(enviCLI->getParameter("sources-dir")));
	}
	else
	{
		return (File::getSpecialLocation(File::userHomeDirectory).getChildFile(".envi/sources"));
	}
}

const File EnviApplication::getEnviScriptsDir()
{
	if (enviCLI->isSet("scripts-dir"))
	{
		return (File(enviCLI->getParameter("scripts-dir")));
	}
	else
	{
		return (File::getSpecialLocation(File::userHomeDirectory).getChildFile(".envi/scripts"));
	}
}

const File EnviApplication::getEnviStoreFile()
{
	if (enviCLI->isSet("store-file"))
	{
		return (File(enviCLI->getParameter("store-file")));
	}
	else
	{
		return (File::getSpecialLocation(File::userHomeDirectory).getChildFile(".envi/data.db"));
	}
}

const File EnviApplication::getEnviLogFile()
{
	if (enviCLI->isSet("log-file"))
	{
		return (File(enviCLI->getParameter("log-file")));
	}
	else
	{
		return (File::getSpecialLocation(File::userHomeDirectory).getChildFile(".envi/envi.log"));
	}
}

const int EnviApplication::getNumSources()
{
	ScopedLock sl(dataSources.getLock());
	return (dataSources.size());
}

EnviDataSource *EnviApplication::getSource(const int sourceIndex)
{
	ScopedLock sl(dataSources.getLock());
	return (dataSources[sourceIndex]);
}

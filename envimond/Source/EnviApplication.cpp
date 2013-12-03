/*
  ==============================================================================

    EnviApplication.cpp
    Created: 2 Dec 2013 1:54:30pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviApplication.h"
#include "DataSources/EnviDSCommand.h"

EnviApplication::EnviApplication(int argc, char* argv[])
	: enviCLI(argc, argv, "dn:c")
{
	_DBG("EnviApplication::ctor");

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
	enviDB	= new EnviDB(*this);
}

const int EnviApplication::runDispatchLoop()
{
	_DBG("EnviApplication::runDispatchLoop");
	if (registerDataSources() && enviDB->init())
	{
		_DBG("EnviApplication::runDispatchLoop running");
		MessageManager::getInstance()->runDispatchLoop();
		return (0);
	}
	else
	{
		_ERR("EnviApplication::runDispatchLoop error initializing sources, exit");
		return (-1);
	}
}

void EnviApplication::cleanupAndExit()
{
}

void EnviApplication::addDataSource(EnviDataSource *sourceToAdd)
{
	ScopedLock sl(dataSources.getLock());
	registerDataSource (dataSources.add (sourceToAdd));
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
	EnviDataSource *ds = dataSources[ENVI_TIMER_OFFSET - timerId];
	if (ds)
	{
		if (ds->isDisabled())
		{
			_WRN("Timer triggered for data source \""+ds->getName()+"\". Source is disabled");
			return;
		}

		_LOG(LOG_INFO, "Timer triggered for data source \""+ds->getName()+"\"");

		if (ds->startSource())
		{
			_LOG(LOG_INFO, "\tsuccess");
		}
		else
		{
			_LOG(LOG_WARN, "Timer trigger for data source \""+ds->getName()+"\", execute failed");
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

	return (nullptr);
}

bool EnviApplication::registerDataSources()
{
	_DBG("EnviApplication::registerDataSources");

	File dataSourcesFolder = getPropertiesFolder().getChildFile("sources");
	if (dataSourcesFolder.isDirectory())
	{
		Array <File> sourceXmls;

		const int ret = dataSourcesFolder.findChildFiles (sourceXmls, File::findFiles, false, "*.xml");

		if (ret > 0)
		{
			for (int i=0; i<sourceXmls.size(); i++)
			{
				_DBG("\tfoundSource: "+sourceXmls[i].getFullPathName());
				EnviDataSource *ds = createInstance (sourceXmls[i]);

				if (ds)
				{
					addDataSource (ds);
				}
				else
				{
                    _DBG("\t\tfailed to create data source");
				}
			}

			return (true);
		}
		else
		{
			_DBG("\tno data sources found");
			return (false);
		}
	}
	else
	{
		_DBG("\t"+dataSourcesFolder.getFullPathName() + " is not a directory, can't initialize any sources");
		return (false);
	}
}

void EnviApplication::sourceFailed(EnviDataSource *dataSource)
{
	_DBG("EnviApplication::dsFailed ["+dataSource->getName()+"]");
}

void EnviApplication::sourceWrite(EnviDataSource *dataSource)
{
	_DBG("EnviApplication::dsWrite ["+dataSource->getName()+"]");
	_DBG("\tresult: ["+dataSource->getResult().toString()+"]");

	enviDB->writeResult (dataSource);
}
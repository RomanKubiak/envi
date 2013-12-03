/*
  ==============================================================================

    EnviApplication.cpp
    Created: 2 Dec 2013 1:54:30pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviApplication.h"
#include "DataSources/EnviDSCommand.h"

juce_ImplementSingleton (SharedMessageThread)

EnviApplication::EnviApplication(int argc, char* argv[])
	: enviCLI(argc, argv, "dn:c")
{
	std::cout << "EnviApplication::ctor" << std::endl;

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
	enviLog			= new EnviLog (*this);
	enviScheduler 	= new EnviScheduler (*this);
}

bool EnviApplication::messageLoop()
{
	if (registerDataSources())
	{
		while (1)
		{
			SharedMessageThread::getInstance()->waitForThreadToExit(250);
			_DBG("loop()");
		}
		return (true);
	}
	else
	{
		return (false);
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

void EnviApplication::timerCallback(const int timerId)
{
	ScopedLock sl(dataSources.getLock());
	_DBG("EnviApplication::timerCallback timerId="+String(timerId));

	if (dataSources[ENVI_TIMER_OFFSET - timerId])
	{
		dataSources[ENVI_TIMER_OFFSET - timerId]->execute();
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
        return (new EnviDSCommand (dataSourceInstance));
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

/*
  ==============================================================================

    EnviDSFile.cpp
    Created: 23 Dec 2013 1:33:38pm
    Author:  rk09631

  ==============================================================================
*/

#include "EnviDSFile.h"
#include "EnviApplication.h"
#include "Externals/slre/slre.h"

EnviDSFile::EnviDSFile(EnviApplication &owner)
	: Thread("EnviDSFile"), EnviDataSource(owner, "file")
{
}

EnviDSFile::~EnviDSFile()
{
	if (isThreadRunning())
	{
		waitForThreadToExit (500);
	}
}

const Result EnviDSFile::initialize(const ValueTree _instanceConfig)
{
	EnviDataSource::initialize (_instanceConfig);

	if (instanceConfig.isValid())
	{
		filePath 				= File (instanceConfig.hasProperty (Ids::path)				? File(getProperty(Ids::path).toString()) 			: File::nonexistent);

		for (int i=0; i<instanceConfig.getNumChildren(); i++)
		{
			ValueTree v = instanceConfig.getChild(i);
			if (v.hasType (Ids::dataValue))
			{
				regexStrings.add (v.hasProperty (Ids::regex)		? v.getProperty(Ids::regex).toString() 	: String::empty);
				regexMatches.add (v.hasProperty (Ids::regexMatch)	? (int)v.getProperty(Ids::regexMatch) 	: 0);
				addValue (v.getProperty(Ids::name), stringToUnit(v.getProperty(Ids::unit)));
			}
		}

		if (instanceConfig.getNumChildren() > 0)
			return (setAllExpressions());
	}

	return (Result::ok());
}

const Result EnviDSFile::execute()
{
	if (!isDisabled())
	{
		if (isThreadRunning())
		{
			notify();
			return (Result::ok());
		}
		else
		{
			startThread();
			return (Result::ok());
		}
	}
	else
	{
		return (Result::fail(getType()+" ["+getName()+"] is disabled"));
	}
}

void EnviDSFile::handleAsyncUpdate()
{
	_DSDBG("EnviDSFile::handleAsyncUpdate");
	collectFinished (Result::ok());
}

const Result EnviDSFile::processExpressions(const String &stringResult)
{
	Array <double> results;

	for (int i=0; i<regexStrings.size(); i++)
	{
		if (regexStrings[i].isEmpty())
		{
			_DSDBG ("EnviDSFile::processExpressions no regex defined");
			StringArray lines = StringArray::fromLines (stringResult.trim());
			for (int i=0; i<lines.size(); i++)
			{
				results.add (lines[i].getDoubleValue());
				setValue (i, lines[i].getDoubleValue());
			}
		}
		else
		{
			const int regexMatchIndex = regexMatches[i];

			struct slre_cap *caps = (struct slre_cap *)malloc (sizeof (struct slre_cap) * regexMatchIndex + 1);

			if (slre_match(regexStrings[i].toUTF8(), stringResult.toUTF8(), strlen(stringResult.toUTF8()), caps, regexMatchIndex+1) >0 )
			{
				if (caps[regexMatchIndex].len > 0)
				{
					String result (caps[regexMatches[i]].ptr, caps[regexMatches[i]].len);
					results.add (result.getDoubleValue());
					setValue (i, result.getDoubleValue());

					_DSDBG("Match resulted in value ["+result+"]");
				}
			}
			else
			{
				_DSDBG("slre_match returned nothing");
			}
		}
	}

	return (evaluateAllExpressions (results));
}

void EnviDSFile::run()
{
	while (1)
	{
		do
		{
			SHOULD_WE_EXIT();

			_DSDBG("EnviDSFile::run try to read: ["+filePath.getFullPathName()+"]");

			ScopedPointer <FileReader> fileReader(new FileReader(*this, filePath));

            while (fileReader->isThreadRunning())
			{
				if (threadShouldExit())
				{
					fileReader->signalThreadShouldExit();
					if (!fileReader->waitForThreadToExit(getTimeout()))
					{
						_ERR(getType()+" ["+getName()+"] can't gracefuly shut down reader thread, problems might occur");
					}
					return;
				}
			}

			if (!fileReader->getData().isEmpty())
			{
				ScopedLock sl(safeResultLock);

				processExpressions(fileReader->getData());
				triggerAsyncUpdate();
			}
			else
			{
				_WRN(getType()+" ["+getName()+"] empty output from file read: ["+filePath.getFullPathName()+"}");
			}

		} while (wait (-1));
	}
}

/**
 *
 */
EnviDSFile::FileReader::FileReader(EnviDSFile &_owner, const File &_fileToRead)
	: Thread ("EnviDSFile::FileReader"), owner(_owner), fileToRead(_fileToRead)
{
#ifdef JUCE_LINUX
	pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, nullptr);
#endif
	startThread();
}

EnviDSFile::FileReader::~FileReader()
{
}

void EnviDSFile::FileReader::run()
{
	if (fileToRead.existsAsFile())
	{
		_DBG("EnviDSFile::FileReader::run reading: ["+fileToRead.getFullPathName()+"]");
		readData = fileToRead.loadFileAsString();
	}
	else
	{
		_INF(owner.getType()+" ["+owner.getName()+"] ::FileReader file does not exist: ["+fileToRead.getFullPathName()+"]");
	}
}

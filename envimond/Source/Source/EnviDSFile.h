/*
  ==============================================================================

    EnviDSFile.h
    Created: 23 Dec 2013 1:33:38pm
    Author:  rk09631

  ==============================================================================
*/

#ifndef ENVIDSFILE_H_INCLUDED
#define ENVIDSFILE_H_INCLUDED

#include "EnviIncludes.h"
#include "EnviDataSource.h"
class EnviApplication;

class EnviDSFile : public EnviDataSource, public Thread, public AsyncUpdater
{
	public:
		EnviDSFile(EnviApplication &_owner);
		~EnviDSFile();

		class FileReader : public Thread, public ChangeBroadcaster
		{
			public:
				FileReader(EnviDSFile &_owner, const File &_fileToRead);
				~FileReader();
				void run();
				String getData() { return (readData); }
				JUCE_LEAK_DETECTOR(FileReader);

			private:
				EnviDSFile &owner;
				File fileToRead;
				String readData;
		};

		const Result initialize(const ValueTree _instanceConfig);
		const Result execute();
		void run();
		void handleAsyncUpdate();
		const Result processExpressions(const String &stringResult);

		JUCE_LEAK_DETECTOR(EnviDSFile);

	private:
		CriticalSection safeResultLock;
		EnviData safeResult;
		String matchRegex;
		File filePath;
		String data;
};

#endif  // ENVIDSFILE_H_INCLUDED

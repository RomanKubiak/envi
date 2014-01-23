#ifndef JUCE_FILEFILTER_H_INCLUDED
#define JUCE_FILEFILTER_H_INCLUDED

#include "EnviIncludes.h"
class  FileFilter
{
	public:
		FileFilter (const String& filterDescription);
		virtual ~FileFilter();
		const String& getDescription() const noexcept;
		virtual bool isFileSuitable (const File& file) const = 0;
		virtual bool isDirectorySuitable (const File& file) const = 0;
	protected:
		String description;
};


#endif   // JUCE_FILEFILTER_H_INCLUDED

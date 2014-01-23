#ifndef JUCE_WILDCARDFILEFILTER_H_INCLUDED
#define JUCE_WILDCARDFILEFILTER_H_INCLUDED
#include "juce_FileFilter.h"
class WildcardFileFilter  : public FileFilter
{
	public:
		WildcardFileFilter (const String& fileWildcardPatterns,const String& directoryWildcardPatterns,const String& description);
		~WildcardFileFilter();
		bool isFileSuitable (const File& file) const;
		bool isDirectorySuitable (const File& file) const;
	private:
		StringArray fileWildcards, directoryWildcards;
		static void parse (const String& pattern, StringArray& result);
		static bool match (const File& file, const StringArray& wildcards);
		JUCE_LEAK_DETECTOR (WildcardFileFilter)
};
#endif   // JUCE_WILDCARDFILEFILTER_H_INCLUDED

/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_90455326_INCLUDED
#define BINARYDATA_H_90455326_INCLUDED

namespace BinaryData
{
    extern const char*   DejaVuSans_ttf;
    const int            DejaVuSans_ttfSize = 741536;

    extern const char*   DejaVuSansCondensed_ttf;
    const int            DejaVuSansCondensed_ttfSize = 664972;

    extern const char*   DejaVuSansMono_ttf;
    const int            DejaVuSansMono_ttfSize = 335068;

    extern const char*   EnviTranslations_txt;
    const int            EnviTranslations_txtSize = 86;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    extern const int namedResourceListSize;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif

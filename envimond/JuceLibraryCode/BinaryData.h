/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_123712956_INCLUDED
#define BINARYDATA_H_123712956_INCLUDED

namespace BinaryData
{
    extern const char*   mime_types;
    const int            mime_typesSize = 29198;

    extern const char*   LICENSE;
    const int            LICENSESize = 752;

    extern const char*   README_md;
    const int            README_mdSize = 4759;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 3;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif

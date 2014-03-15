/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_22182922_INCLUDED
#define BINARYDATA_H_22182922_INCLUDED

namespace BinaryData
{
    extern const char*   about_png;
    const int            about_pngSize = 23143;

    extern const char*   builtin_pgm_zip;
    const int            builtin_pgm_zipSize = 88890;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 2;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif

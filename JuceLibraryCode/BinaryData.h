/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   LittlePhatty_png;
    const int            LittlePhatty_pngSize = 1641562;

    extern const char*   LittlePhatty_OFF_png;
    const int            LittlePhatty_OFF_pngSize = 1432094;

    extern const char*   Toggle_png;
    const int            Toggle_pngSize = 47714;

    extern const char*   Toggle1_png;
    const int            Toggle1_pngSize = 47457;

    extern const char*   HotPotatoBG_png;
    const int            HotPotatoBG_pngSize = 271430;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 5;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}

# Introduction

This software was written after hours of searching for a robust Microsoft C and C++ implementation of getopt, which led to devoid results.

This software is a modification of the Free Software Foundation, Inc. getopt library for parsing command line arguments and its purpose is to provide a Microsoft Visual C friendly derivative. The source code provides functionality for both Unicode and Multibyte builds and supports getopt, getopt_long, and getopt_long_only. Additionally the code supports the POSIXLY_CORRECT environment flag. The library uses the _UNICODE preprocessor directive when defining the getopt, getopt_long, and getopt_long_only functions. These functions are mapped back to getopt_a\getopt_w, getopt_long_a\getopt_long_w, and getopt_long_only_a\getopt_long_only_w respectively. This improvement was made to allow a single DLL to be used in both multibyte and Unicode projects.

The original GNU code used several header and implementation files containing numerous preprocessor directives specific to Linux environments which have been removed. After removing unneeded dependencies, it was condensed into a single header and implementation file which can be compiled into a DLL, LIB, or directly included into any Visual C, C++, or MFC project. The getopt library can be used by proprietary software, however; certain measures need to be taken to ensure proprietary code adheres to the Lesser GNU Public License for non-derivative works. Please refer to the licensing section of this article for more details on how this software is licensed.

For the sake of brevity, this article doesn't discuss how to use the getopt functions. Anyone new to using the getopt functions should refer to the GNU tutorial for using getopt.

# Licensing
Since getopt is licensed under LGPL, it is free to use in proprietary software under some restrictions. When using this library as part of a proprietary software solution, it is important that the library is used as a dynamically linked library (DLL) and is not statically linked or directly compiled into proprietary source code. Static linkage requires that your software be released GPL. Therefore, by keeping the library separately referenced via Dynamic Link Library (DLL), allows the DLL to be modified and updated without changing the proprietary software which utilizes the library; under this condition proprietary software is said to "use" the library. Thus, it is not considered to be a derivative work and can be distributed freely under any license.

# Preprocessor Definitions
Compiling getopt as a Dynamic Link Library (DLL) requires the preprocessor definition of EXPORTS_GETOPT. The definition of EXPORTS_GETOPT sets the internal preprocessor definition _GETOPT_API to the value __declspec(dllexport). Compiling getopt as a Static Library (LIB) or directly including the source and header file within a project requires the preprocessor definition of STATIC_GETOPT. The definition of STATIC_GETOPT clears the value of the internal preprocessor definition of _GETOPT_API. Compiling software to use getopt.dll requires that no library specific preprocessor definitions be used. When no library specific preprocessor definitions are used the value assigned to the internal preprocessor definition _GETOPT_API is __declspec(dllimport).

The code segment below demonstrates the logic outlined above:

```c++
#if defined(EXPORTS_GETOPT) && defined(STATIC_GETOPT)
    #error "The preprocessor definitions of EXPORTS_GETOPT and STATIC_GETOPT can only be used individually"
#elif defined(STATIC_GETOPT)
#pragma message("Warning static builds of getopt violate the Lesser GNU Public License")
    #define _GETOPT_API
#elif defined(EXPORTS_GETOPT)
    #pragma message("Exporting getopt library")
    #define _GETOPT_API __declspec(dllexport)    
#else
    #pragma message("Importing getopt library")
    #define _GETOPT_API __declspec(dllimport)
#endif
```

The following code segment located in getopt.h is responsible for mapping the correct version of the getopt, getopt_long, and getopt_long_only functions. The getopt functions appended with _a to denote ANSI characters using the char type and Unicode functions are appended with _w to denote wide characters using the wchar_t type.

```c++
#ifdef _UNICODE
    #define getopt getopt_w
    #define getopt_long getopt_long_w
    #define getopt_long_only getopt_long_only_w
    #define option option_w
    #define optarg optarg_w
#else
    #define getopt getopt_a
    #define getopt_long getopt_long_a
    #define getopt_long_only getopt_long_only_a
    #define option option_a
    #define optarg optarg_a
#endif
```
# Sample Code Provided

To help with understanding how to use the code, many versions have been provided for download. The following downloads are provided:

* Visual Studio .NET 2010 ANSI Project (v1.0)
* Visual Studio .NET 2008 ANSI Project (v1.0)
* Visual Studio .NET 2008 MFC Project (v1.0)
* Visual Studio .NET 2005 ANSI Project (v1.0)
* Visual Studio .NET 2005 MFC Project (v1.0)
* Visual Studio 6 ANSI Project (v1.0)
* Visual Studio 6 MFC Project (v1.0)

# Using the Code

The code is used identical to GNU getopt.

```C++
#include <stdio.h>
#include <stdlib.h>
#include "tchar.h"
#include "getopt.h"

int _tmain(int argc, TCHAR** argv)
{
    static int verbose_flag;
    int c;

    while (1)
    {        
        static struct option long_options[] =
        {
            {_T("verbose"), ARG_NONE, &verbose_flag, 1},
            {_T("brief"),   ARG_NONE, &verbose_flag, 0},
            {_T("add"),     ARG_NONE, 0, _T('a')},
            {_T("append"),  ARG_NONE, 0, _T('b')},
            {_T("delete"),  ARG_REQ,  0, _T('d')},
            {_T("create"),  ARG_REQ,  0, _T('c')},
            {_T("file"),    ARG_REQ, 0 , _T('f')},
            { ARG_NULL , ARG_NULL , ARG_NULL , ARG_NULL }
        };

        int option_index = 0;
        c = getopt_long(argc, argv, _T("abc:d:f:"), long_options, &option_index);

        // Check for end of operation or error
        if (c == -1)
            break;

        // Handle options
        switch (c)
        {
        case 0:
            /* If this option set a flag, do nothing else now. */
            if (long_options[option_index].flag != 0)
                break;
            _tprintf (_T("option %s"), long_options[option_index].name);
            if (optarg)
                _tprintf (_T(" with arg %s"), optarg);
            _tprintf (_T("\n"));
            break;

        case _T('a'):
            _tprintf(_T("option -a\n"));
            break;

        case _T('b'):
            _tprintf(_T("option -b\n"));
            break;

        case _T('c'):
            _tprintf (_T("option -c with value `%s'\n"), optarg);
            break;

        case _T('d'):
            _tprintf (_T("option -d with value `%s'\n"), optarg);
            break;

        case _T('f'):
            _tprintf (_T("option -f with value `%s'\n"), optarg);
            break;

        case _T('?'):
            /* getopt_long already printed an error message. */
            break;

        default:
            abort();
        }
    }

    if (verbose_flag)
        _tprintf (_T("verbose flag is set\n"));


    if (optind < argc)
    {
        _tprintf (_T("non-option ARGV-elements: "));
        while (optind < argc) _tprintf (_T("%s "), argv[optind++]);
        _tprintf (_T("\n"));
    }
    return 0;
}     
```

# Using this Code with C++ Precompiled Headers
When using this code statically within a C++ project with precompiled headers, it is necessary to rename getopt.c to getopt.cpp in order to circumvent the following compiler error:

```
"C1853 - Precompiled header file is from a previous version of the compiler, 
or the precompiled header is C++ and you are using it from C (or vice versa)."
```

Additionally precompiled header file must be added as the first include of the getopt.c or getopt.cpp file. For example, if you are using "stdafx.h" as the precompiled header, the following would be expected:

```C++
// File comments removed
#include "stdafx.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include "getopt.h"
```

# History

02/03/2011 - Initial release  
02/20/2011 - Fixed L4 compiler warnings  
07/05/2011 - Added no_argument, required_argument, optional_argument def  
08/05/2011 - Fixed non-argument runtime bug which caused runtime exception  
08/09/2011 - Added code to export functions for DLL and LIB  
02/15/2012 - Fixed _GETOPT_THROW definition missing in implementation file  
08/03/2012 - Created separate functions for char and wchar_t characters so single DLL can do both Unicode and ANSI  
10/15/2012 - Modified to match latest GNU features  
06/19/2015 - Fixed maximum option limitation caused by option_a (255) and option_w (65535) structure val variable  

#License
Licensed under The GNU Lesser General Public License (LGPLv3)

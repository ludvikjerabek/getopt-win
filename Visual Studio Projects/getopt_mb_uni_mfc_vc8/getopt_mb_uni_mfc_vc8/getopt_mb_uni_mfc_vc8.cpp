/*
Date: 02/03/2011 - Ludvik Jerabek - Initial Release
Version: 1.0
Comment: Sample Application using Getopt for Microsoft C\C++ 2005 MFC
License: LGPL

Revisions:

Purpose: MFC Demonstration

02/03/2011 - Ludvik Jerabek - Initial Release

**DISCLAIMER**
THIS MATERIAL IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING, BUT Not LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE, OR NON-INFRINGEMENT. SOME JURISDICTIONS DO NOT ALLOW THE
EXCLUSION OF IMPLIED WARRANTIES, SO THE ABOVE EXCLUSION MAY NOT
APPLY TO YOU. IN NO EVENT WILL I BE LIABLE TO ANY PARTY FOR ANY
DIRECT, INDIRECT, SPECIAL OR OTHER CONSEQUENTIAL DAMAGES FOR ANY
USE OF THIS MATERIAL INCLUDING, WITHOUT LIMITATION, ANY LOST
PROFITS, BUSINESS INTERRUPTION, LOSS OF PROGRAMS OR OTHER DATA ON
YOUR INFORMATION HANDLING SYSTEM OR OTHERWISE, EVEN If WE ARE
EXPRESSLY ADVISED OF THE POSSIBILITY OF SUCH DAMAGES. 
*/
#include "stdafx.h"
#include "getopt_mb_uni_mfc_vc8.h"
#include "getopt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	envp;

	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
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
			c = getopt_long( __argc, __targv , _T("abc:d:f:"), long_options, &option_index);
			
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
				
			case '?':
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

	return nRetCode;
}

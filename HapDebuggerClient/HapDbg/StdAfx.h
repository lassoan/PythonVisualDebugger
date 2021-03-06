//////////////////////////////////////////////////////////////////////////////
//  File:    StdAfx.h
//
//  Purpose: include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2001 by Humongous Entertainment
//  
//  This library is free software; you can redistribute it and/or modify it 
//  under the terms of the GNU Lesser General Public License as published by 
//  the Free Software Foundation; either version 2.1 of the License, or  any 
//  later version.
//  
//  This library is distributed in the hope that it will be useful, but 
//  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
//  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public 
//  License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public License 
//  along with this library (see the file license.txt); if not, write to the 
//  Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//////////////////////////////////////////////////////////////////////////////	


#if !defined(AFX_STDAFX_H__F7EB9042_7381_4C28_8599_7E016F99A232__INCLUDED_)
#define AFX_STDAFX_H__F7EB9042_7381_4C28_8599_7E016F99A232__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#pragma warning(disable:4786)


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>




//this is a little switch file that allows me to easily exclude some
//useful Humongous owned code when I'm compiling an external release
#include <ossinclude/ossinclude.h>
#ifdef OSS_RELEASE
	using namespace HumongousOpenSource;

	//include all the python stuff we'll need
	//#include <python.h>
  
/*
   Use the "documented" trick involving checking for _DEBUG
   and undefined that symbol while we include Python headers.
   Update: this method does not fool Microsoft Visual C++ 8 anymore; two
   of its header files (crtdefs.h and use_ansi.h) check if _DEBUG was set
   or not, and set flags accordingly (_CRT_MANIFEST_RETAIL,
   _CRT_MANIFEST_DEBUG, _CRT_MANIFEST_INCONSISTENT). The next time the
   check is performed in the same compilation unit, and the flags are found,
   and error is triggered. Let's prevent that by setting _CRT_NOFORCE_MANIFEST.
*/

#ifdef _DEBUG
# undef _DEBUG
# if defined(_MSC_VER) && _MSC_VER >= 1400
#   define _CRT_NOFORCE_MANIFEST 1
# endif
# include <Python.h>
# define _DEBUG
#else
# include <Python.h>
#endif  
  
  
  
  
  
	#include <compile.h>
	#include <eval.h>
	#include <frameobject.h>
	#include <structmember.h>

#else
	#include <hecore/hecore.h>
	#include <hecore/threadcore.h>
	using namespace HumongousEntertainment;

	//include all the python stuff we'll need
	//#include <include/python.h>
  
  /*
   Use the "documented" trick involving checking for _DEBUG
   and undefined that symbol while we include Python headers.
   Update: this method does not fool Microsoft Visual C++ 8 anymore; two
   of its header files (crtdefs.h and use_ansi.h) check if _DEBUG was set
   or not, and set flags accordingly (_CRT_MANIFEST_RETAIL,
   _CRT_MANIFEST_DEBUG, _CRT_MANIFEST_INCONSISTENT). The next time the
   check is performed in the same compilation unit, and the flags are found,
   and error is triggered. Let's prevent that by setting _CRT_NOFORCE_MANIFEST.
*/

#ifdef _DEBUG
# undef _DEBUG
# if defined(_MSC_VER) && _MSC_VER >= 1400
#   define _CRT_NOFORCE_MANIFEST 1
# endif
# include <Python.h>
# define _DEBUG
#else
# include <Python.h>
#endif  

  
  
	#include <include/compile.h>
	#include <include/eval.h>
	#include <include/frameobject.h>
	#include <include/structmember.h>

#endif

//we use these a lot so lets make sure they're everywhere.
#include <string>
#include <vector>
#include <set>
using namespace std;


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__F7EB9042_7381_4C28_8599_7E016F99A232__INCLUDED_)

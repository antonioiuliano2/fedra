// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

// below 3 lines are added by Aki in order to avoid compilation errors.
// 0x0500 is actually for Windwos 2000, however, only this can solve the problem...
// this is tested with VS2005 and VS2008.
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#if !defined(AFX_STDAFX_H__71B46BDE_4B55_413C_8746_A37F551200F7__INCLUDED_)
#define AFX_STDAFX_H__71B46BDE_4B55_413C_8746_A37F551200F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__71B46BDE_4B55_413C_8746_A37F551200F7__INCLUDED_)

/********************************************************************************
*					Open Universe - version 1.0.0.7							*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
*
* THIS SOURCE FILE IS THE PROPERTY OF TANGRAM TEAM AND IS NOT TO
* BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED
* WRITTEN CONSENT OF TANGRAM TEAM.
*
* THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS
* OUTLINED IN THE MIT LICENSE AGREEMENT.TANGRAM TEAM
* GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE
* THIS SOFTWARE ON A SINGLE COMPUTER.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
* https://www.tangram.dev
*
********************************************************************************/


// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently
//VS:NJVYC-BMHX2-G77MM-4XJMR-6Q8QF

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit
#define ISOLATION_AWARE_ENABLED 1
#define COMMERCIAL
#define MAX_BUF_LEN 65535
#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS
#define TMSCHEMA_H // this excludes the deprecated tmschema.h without dependency on _WIN32_WINNT macro
#define _CRT_SECURE_NO_WARNINGS

#include <afxwin.h>         
#include <Uxtheme.h>
#include <vssym32.h>
//#include <afxdisp.h>
#include <afxpriv.h>
#include <Shlobj.h>

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "resource.h"
#include <atlbase.h>
#include <atlimage.h>
#include <afxdisp.h>        // MFC Automation classes

#pragma component(browser, off, references)
#include <mshtml.h>
#pragma component(browser, on, references)

#define XHTMLHTML
#define XHTMLTOOLTIPS
#define XHTMLXML
#define XHTMLDRAGDROP
#define XHTMLTREE_DEMO		
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#include "inc.h"
#include <atlwin.h>
#include "CommonFunction.h"
#include "msaddndr.h"

using namespace ATL;

#include "tangrambase.h"
#include "jni.h"

#pragma warning (disable : 4786)
#pragma warning (disable : 4312)
#pragma warning (disable : 4311)
#pragma warning (disable : 4244)
#pragma warning( disable : 4278 )
#pragma warning( disable : 4146 )
#pragma warning( disable : 4526 )

#define IfFailGo(x) { hr=(x); if (FAILED(hr)) goto Error; }
#define IfFailGoCheck(x, p) { hr=(x); if (FAILED(hr)) goto Error; if(!p) {hr = E_FAIL; goto Error; } }

#pragma comment (lib, "wininet.lib")
#pragma comment (lib, "urlmon.lib")


using namespace std;

#include <ppl.h>
#include <ppltasks.h>
using namespace concurrency;

class CBKWnd;
class CGrid;
class CWPFView;
class CGridHelper;
class CHubble;
class CGalaxy;
class CHelperWnd;
class CEclipseWnd;
class CHubbleDoc;
class CUniverse;
class CGridEvents;
class CHubbleDocWnd;
class CTangramTabCtrl;
class CHubbleAppCtrl;
class CHubbleDocFrame;
class CTangramListView;
class CCosmosEvent;
class CHubbleDocFrame;
class CGalaxyCluster;
class CUniverseMDIMain;
class CGridCollection;
class CCommonCtrl;
class CTangramHtmlTreeWnd;
class CAFXHelperWnd;

typedef vector<CGrid*> CGridVector;

#include "vbe6ext.h"
#include <afxcview.h>
#include <afxdlgs.h>
#include <afxwin.h>

using namespace Office;

#define FORCE_EXPLICIT_DTE_NAMESPACE
#define FORCE_EXPLICIT_PRJ_NAMESPACE

const _TCHAR * JNI_GetStringChars(JNIEnv *env, jstring str);
void JNI_ReleaseStringChars(JNIEnv *env, jstring s, const _TCHAR* data);

void DefaultExceptionProcess(JNIEnv *env);
void DefaultExceptionProcessEx(JNIEnv *env, _TCHAR* errorInfo);

void ThrowComFail(JNIEnv *env, const char* desc, jint hr);
SAFEARRAY *extractSA(JNIEnv *env, jobject arg);
void setSA(JNIEnv *env, jobject arg, SAFEARRAY *sa, int copy);
SAFEARRAY *copySA(SAFEARRAY *psa);


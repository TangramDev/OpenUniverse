/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202102170032           *
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
 *
 * Web Runtime is a new, subversive and Internet Oriented Development Technology
 * for Windows Desktop Software. Its core idea is that, relying on the open-source
 * project of Chromium, developers are allowed to use their familiar software
 * development technology to realize the Main Process of their application system
 * as the Browser Process of Modern Web Browser. Because the Main Process is realized
 * by the developers themselves, the application system developed in this way is
 * completely different from modern web browser. This kind of application will
 * integrate the infrastructure of modern web browser as a part of the system model,
 * the application system contains not only the browser window inherent in the browser,
 * but also other desktop elements oriented to the application, such as WinForms, Java
 * and other mature software elements.
 *
 * There are Three Key Features of Webruntime:
 * 1. Built-in Modern Web Browser: Independent Browser Window and Browser Window
 *    as sub windows of other windows are supported in the application process;
 * 2. DOM Plus: DOMPlus is a natural extension of the standard DOM system.
 *    It allows the application system to support a kind of generalized web pages,
 *    which are composed of standard DOM elements and binary components supported
 *    by the application system;
 * 3. JavaScript for Application: Similar to VBA in MS office, JavaScript will
 *    become a built-in programmable language in the application system, so that
 *    the application system can be expanded and developed for the Internet based
 *    on modern javscript/Web technology.
 *
 * Built-in Full-Featured Modern Web Browser Subsystem
 *		Since the main process of the application system is essentially a Browser
 * Process, the application system can naturally create a new browser window. The
 * difference with standard browsers is that we allow the browser window to be a child
 * window of other windows. in sub-window model, we see a brand-new scene: a multi-tab
 * browser sub-window, we think this structure is more suitable for organizing specific
 * applications. Of course, the application system fully supports the creation of an
 * independent browser window as a part of the complete application system, and the browser
 * window serves as a natural extension of the ecological chain to ensure the richness
 * and diversity of the application system content.
 * DOM Plus:
 *		Because a large number of mature binary components that conform to industry
 * standards have formed a wealth of software asset accumulation, web runtime allows
 * developers to regard these existing accumulation as new DOM elements and then become
 * a part of web pages, this starting point eventually forms "DOM Plus". DOM plus is a
 * natural extension of standard DOM for desktop applications, similar to the extension
 * from "real number system" to "complex number system" in mathematical theory. Undoubtedly,
 * for the development of mathematics, from "real number system" to "complex number system"
 * is a huge leap, which can be said to be a decisive change in the development of modern
 * mathematics. We hope that the evolution from "DOM" to "DOM plus" will have a similar
 * effect, which is bound to be a huge change from desktop software to Internet desktop software.
 *		Different from earlier IE, based on the sandbox mechanism of Modern Web Browser,
 * we let these binary elements run in the Browser Process, completely isolated from the
 * Browser's Renderer Process. This makes full use of the security mechanisms of modern
 * browsers and also takes into account the accumulation of mature software component
 * technologies. Based on such considerations, existing development technologies such as
 * .NET, COM, Java, etc. can be fully applied to Internet software development.
 *
 * Use of this source code is governed by a BSD-style license that
 * can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *******************************************************************************/

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently
// VS:NJVYC-BMHX2-G77MM-4XJMR-6Q8QF

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
class CXobj;
class CWPFView;
class CXobjHelper;
class CCosmos;
class CGalaxy;
class CHelperWnd;
class CEclipseWnd;
class CUniverse;
class CXobjEvents;
class CTangramTabCtrl;
class CCosmosAppCtrl;
class CTangramListView;
class CCosmosEvent;
class CGalaxyCluster;
class CXobjCollection;
class CTangramHtmlTreeWnd;

typedef vector<CXobj*> CXobjVector;

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



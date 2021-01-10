
// WebRuntimeForVS.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "yaoyao.h"
#include <initguid.h>
#include "yaoyao_i.c"
#include "Psapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CYaoyao

BEGIN_MESSAGE_MAP(CYaoyao, CWinApp)
END_MESSAGE_MAP()


// CYaoyao construction

CYaoyao::CYaoyao() noexcept
{
}

// The one and only CYaoyao object

CYaoyao theApp;

// This identifier was generated to be statistically unique for your app
// You may change it if you prefer to choose a specific identifier

const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

BOOL CYaoyao::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	if (!__super::InitInstance())
		return false;
	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	#if !defined(_WIN32_WCE) || defined(_CE_DCOM)
	// Register class factories via CoRegisterClassObject().
	if (FAILED(theApp.RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE)))
		return FALSE;
	#endif // !defined(_WIN32_WCE) || defined(_CE_DCOM)
	// App was launched with /Embedding or /Automation switch.
	// Run app as automation server.
	//if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	//{
	//	// Don't show the main window
	//	return TRUE;
	//}
	// App was launched with /Unregserver or /Unregister switch.  Unregister
	// typelibrary.  Other unregistration occurs in ProcessShellCommand().else 
	if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		UpdateRegistryAppId(FALSE);
		UnregisterServer(TRUE);
		AfxOleUnregisterTypeLib(LIBID_Yaoyao, _wVerMajor, _wVerMinor);
	}
	// App was launched standalone or with other switches (e.g. /Register
	// or /Regserver).  Update registry entries, including typelibrary.
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppRegister)
	{
		UpdateRegistryAppId(TRUE);
		RegisterServer(TRUE);
		AfxOleRegisterTypeLib(AfxGetInstanceHandle(), LIBID_Yaoyao);
		return false;
	}
	//if (!cmdInfo.m_bRunEmbedded && !cmdInfo.m_bRunAutomated)
	//{
	//	return false;
	//}

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.

	//if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;
	CCosmosApp::InitInstance();
	return TRUE;
}

CString CYaoyao::GetNTPXml()
{
	return _T("Default_Mfc.xml");
}

HRESULT CYaoyao::UpdateRegistry(BOOL bRegister)
{
	return theApp.UpdateRegistryFromResource(IDR_CHROMFORVSAPPOBJ, bRegister);
}

HRESULT CYaoyao::CreateInstance(void* pv, REFIID riid, LPVOID* ppv)
{
	if (g_pCosmos)
	{
		return g_pCosmos->QueryInterface(riid, ppv);
	}
	return S_FALSE;
}

int CYaoyao::ExitInstance()
{
#if !defined(_WIN32_WCE) || defined(_CE_DCOM)
	RevokeClassObjects();
#endif
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CYaoyao message handlers




// TheUniverse.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "TheUniverse.h"
#include "MainFrm.h"

#include "TheUniverseDoc.h"
#include "TheUniverseView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTheUniverseApp

BEGIN_MESSAGE_MAP(CTheUniverseApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CTheUniverseApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW_FRAME, &CTheUniverseApp::OnFileNewFrame)
	ON_COMMAND(ID_FILE_NEW, &CTheUniverseApp::OnFileNew)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CTheUniverseApp construction

CTheUniverseApp::CTheUniverseApp() noexcept
{
	m_bHiColorIcons = TRUE;


	m_nAppLook = 0;
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("TheUniverse.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CTheUniverseApp object

CTheUniverseApp theApp;


// CTheUniverseApp initialization

BOOL CTheUniverseApp::InitInstance()
{
	m_mapDOMObj[_T("main_panel")] = RUNTIME_CLASS(CTheUniverseView);
	if (!InitApp())
		return false;
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTheUniverseDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CTheUniverseView));
	if (!pDocTemplate)
		return FALSE;
	m_pDocTemplate = pDocTemplate;
	AddDocTemplate(pDocTemplate);

	return TRUE;
}

int CTheUniverseApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CTheUniverseApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CTheUniverseApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CTheUniverseApp customization load/save methods

void CTheUniverseApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CTheUniverseApp::LoadCustomState()
{
}

void CTheUniverseApp::SaveCustomState()
{
}

// CTheUniverseApp message handlers

void CTheUniverseApp::OnFileNewFrame()
{
	ASSERT(m_pDocTemplate != nullptr);

	CDocument* pDoc = nullptr;
	CFrameWnd* pFrame = nullptr;

	// Create a new instance of the document referenced
	// by the m_pDocTemplate member.
	if (m_pDocTemplate != nullptr)
		pDoc = m_pDocTemplate->CreateNewDocument();

	if (pDoc != nullptr)
	{
		// If creation worked, use create a new frame for
		// that document.
		pFrame = m_pDocTemplate->CreateNewFrame(pDoc, nullptr);
		if (pFrame != nullptr)
		{
			// Set the title, and initialize the document.
			// If document initialization fails, clean-up
			// the frame window and document.

			m_pDocTemplate->SetDefaultTitle(pDoc);
			if (!pDoc->OnNewDocument())
			{
				pFrame->DestroyWindow();
				pFrame = nullptr;
			}
			else
			{
				// Otherwise, update the frame
				m_pDocTemplate->InitialUpdateFrame(pFrame, pDoc, TRUE);
			}
		}
	}

	// If we failed, clean up the document and show a
	// message to the user.

	if (pFrame == nullptr || pDoc == nullptr)
	{
		delete pDoc;
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
	}
}

void CTheUniverseApp::OnFileNew()
{
	CDocument* pDoc = nullptr;
	CFrameWnd* pFrame;
	pFrame = DYNAMIC_DOWNCAST(CFrameWnd, CWnd::GetActiveWindow());

	if (pFrame != nullptr)
		pDoc = pFrame->GetActiveDocument();

	if (pFrame == nullptr || pDoc == nullptr)
	{
		// if it's the first document, create as normal
		CWinApp::OnFileNew();
	}
	else
	{
		// Otherwise, see if we have to save modified, then
		// ask the document to reinitialize itself.
		if (!pDoc->SaveModified())
			return;

		CDocTemplate* pTemplate = pDoc->GetDocTemplate();
		ASSERT(pTemplate != nullptr);

		if (pTemplate != nullptr)
			pTemplate->SetDefaultTitle(pDoc);
		pDoc->OnNewDocument();
	}
}



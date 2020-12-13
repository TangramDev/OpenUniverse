/********************************************************************************
*					Open Universe - version 1.1.4.25							*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
*
* THIS SOURCE FILE IS THE PROPERTY OF TANGRAM TEAM AND IS NOT TO
* BE RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED
* WRITTEN CONSENT OF TANGRAM TEAM.
*
* THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS
* OUTLINED IN THE GPL LICENSE AGREEMENT.TANGRAM TEAM
* GRANTS TO YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE
* THIS SOFTWARE ON A SINGLE COMPUTER.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
* https://www.tangram.dev
*
*
********************************************************************************/

#include "StdAfx.h"
#include "dteinternal.h"
#include "dllmain.h"
#include "dte.h"
#include "GridCLREvent.h"
#include "Cosmos.h"
#include "Browser.h"
#include "WizCtrl.h"
#include "Markup.h"

#ifdef _WIN32
#ifdef ReplaceText
#undef ReplaceText
#endif // ReplaceText
#endif // _WIN32

using namespace System::Threading;
using namespace System::Diagnostics;
using namespace System::Reflection;
using namespace System::Runtime::InteropServices;
using namespace System::IO::Compression;

namespace Cosmos
{
    TangramAppProxy::TangramAppProxy()
    {
        m_pUniverseAppProxy = nullptr;
    }

    TangramAppProxy::~TangramAppProxy()
    {

    }

    void TangramAppProxy::Init()
    {
        //if (m_pUniverseAppProxy)
        //	m_pUniverseAppProxy->Init();
    }

    Grid^ GridCollection::default::get(int iIndex)
    {
        IGrid* pGrid = nullptr;
        m_pGridCollection->get_Item(iIndex, &pGrid);
        return theAppProxy._createObject<IGrid, Grid>(pGrid);
    }

    Grid::Grid(IGrid* pGrid)
    {
        m_hWnd = NULL;
        m_pGridEvent = new CCosmosGridEvent();
        m_pGridEvent->m_pGrid = pGrid;
        m_pGridEvent->m_pGridCLREvent = new CGridCLREvent();
        m_pGridCLREvent = m_pGridEvent->m_pGridCLREvent;
        m_pGridEvent->m_pGridCLREvent->m_pGrid = this;
        HRESULT hr = m_pGridEvent->DispEventAdvise(pGrid);
        if (theApp.m_pHubbleImpl)
            theApp.m_pHubbleImpl->AttachGrid(m_pGridEvent);
        m_pChildNodes = nullptr;
        SetNewNode(pGrid);
        LONGLONG nValue = (LONGLONG)pGrid;
        theAppProxy._insertObject(nValue, this);
        actionData = nullptr;
    }

    Grid::~Grid()
    {
        if (m_pChildNodes)
        {
            delete m_pChildNodes;
        }
        delete m_pGridCLREvent;
        //LONGLONG nValue = (LONGLONG)m_pGrid;
        //theAppProxy._removeObject(nValue);
        SetNewNode(NULL);
    }

    GridCollection^ Grid::ChildNodes::get()
    {
        if (m_pChildNodes == nullptr)
        {
            CComPtr<IGridCollection> pGrids;

            m_pGrid->get_ChildNodes(&pGrids);
            m_pChildNodes = gcnew GridCollection(pGrids);
        }
        return m_pChildNodes;
    }

    GalaxyCluster^ Grid::GalaxyCluster::get()
    {
        IGalaxyCluster* pGalaxyCluster = nullptr;
        m_pGrid->get_GalaxyCluster(&pGalaxyCluster);

        if (pGalaxyCluster)
        {
            return theAppProxy._createObject<IGalaxyCluster, Cosmos::GalaxyCluster>(pGalaxyCluster);
        }
        return nullptr;
    }

    Galaxy^ Grid::Galaxy::get()
    {
        CComPtr<IGalaxy> _pGalaxy;
        m_pGrid->get_Galaxy(&_pGalaxy);
        return theAppProxy._createObject<IGalaxy, Cosmos::Galaxy>(_pGalaxy);
    }

    String^ Grid::Caption::get()
    {
        if (m_pGrid)
        {
            CComBSTR bstrCap("");
            m_pGrid->get_Caption(&bstrCap);
            String^ strCap = BSTR2STRING(bstrCap);
            return strCap;
        }
        return "";
    }

    String^ Grid::URL::get()
    {
        if (m_pGrid)
        {
            ::GridType nType;
            m_pGrid->get_GridType(&nType);
            CComBSTR bstrCap("");
            switch (nType)
            {
            case BlankView:
            {
                m_pGrid->get_URL(&bstrCap);
                return BSTR2STRING(bstrCap);
            }
            break;
            case ActiveX:
            {
                CComVariant var;
                m_pGrid->get_XObject(&var);
            }
            break;
            default:
                break;
            }
        }
        return "";
    }

    void Grid::URL::set(String^ newVal)
    {
        if (m_pGrid)
        {
            ::GridType nType;
            m_pGrid->get_GridType(&nType);
            switch (nType)
            {
            case BlankView:
                if (nType == BlankView)
                {
                    m_pGrid->put_URL(STRING2BSTR(newVal));

                }
                break;
            case ActiveX:
            {
            }
            break;
            default:
                break;
            }
        }
    }

    void Grid::Init()
    {
        LONGLONG h = 0;
        if (m_pGrid)
        {
            m_pGrid->get_Handle(&h);
            ::SendMessage((HWND)h, WM_COSMOSMSG, 1, 0);
        }
    }

    Object^ Grid::PlugIn::get(String^ strObjName)
    {
        Object^ pObj = nullptr;
        if (m_pGrid)
        {
            Grid^ pRootGrid = this->RootGrid;
            if (pRootGrid->m_pPlugInDic == nullptr)
            {
                pRootGrid->m_pPlugInDic = gcnew Dictionary<String^, Object^>();
            }
            if (pRootGrid->m_pPlugInDic->TryGetValue(strObjName, pObj) == false)
            {
                IDispatch* pDisp = nullptr;
                LRESULT hr = m_pGrid->get_AxPlugIn(STRING2BSTR(strObjName), &pDisp);
                if (SUCCEEDED(hr) && pDisp)
                {
                    Object^ pObj = reinterpret_cast<Object^>(Marshal::GetObjectForIUnknown((System::IntPtr)(pDisp)));
                    pRootGrid->m_pPlugInDic[strObjName] = pObj;
                    return pObj;
                }
            }
        }
        return pObj;
    }

    void Grid::Fire_OnTabChange(Grid^ ActivePage, Grid^ OldGrid)
    {
        OnTabChange(ActivePage, OldGrid);
    }

    Grid^ Hubble::Observe(Control^ ctrl, String^ key, String^ strGridXml)
    {
        if (ctrl != nullptr)
        {
            if (ctrl->Dock != DockStyle::Fill && ctrl->Dock != DockStyle::None)
                return nullptr;
            IGalaxy* pGalaxy = nullptr;
            theApp.m_pHubble->GetGalaxy((__int64)ctrl->Handle.ToPointer(), &pGalaxy);
            if (pGalaxy)
            {
                IGrid* pGrid = nullptr;
                BSTR bstrKey = STRING2BSTR(key);
                BSTR bstrXml = STRING2BSTR(strGridXml);
                pGalaxy->Observe(bstrKey, bstrXml, &pGrid);
                ::SysFreeString(bstrKey);
                ::SysFreeString(bstrXml);
                if (pGrid)
                    return theAppProxy._createObject<IGrid, Grid>(pGrid);
                return nullptr;
            }
            HWND hTopWnd = nullptr;
            Control^ topCtrl = ctrl->TopLevelControl;
            if (topCtrl == nullptr)
            {
                hTopWnd = ::GetParent((HWND)ctrl->Handle.ToPointer());
            }
            else
            {
                if (topCtrl->GetType()->IsSubclassOf(Form::typeid))
                {
                    Form^ pForm = (Form^)topCtrl;
                    Form^ pForm2 = nullptr;
                    if (pForm != nullptr && pForm->IsMdiContainer)
                    {
                        bool bMdiClientChild = false;
                        int nCount = pForm->Controls->Count;
                        String^ strName = L"";
                        for (int i = nCount - 1; i >= 0; i--)
                        {
                            Control^ pCtrl = pForm->Controls[i];
                            strName = pCtrl->GetType()->Name->ToLower();
                            if (strName == L"mdiclient")
                            {
                                if (::IsChild((HWND)pCtrl->Handle.ToPointer(), (HWND)ctrl->Handle.ToPointer()))
                                {
                                    bMdiClientChild = true;
                                }
                                break;
                            }
                        }
                        if (bMdiClientChild)
                        {
                            Control^ parent = ctrl->Parent;
                            while (!parent->GetType()->IsSubclassOf(Form::typeid))
                            {
                                parent = parent->Parent;
                            }
                            if (parent != nullptr)
                                pForm2 = (Form^)parent;
                        }
                        else
                        {
                            pForm2 = pForm;
                        }
                    }
                    else
                        pForm2 = pForm;
                    if (pForm2 != nullptr)
                    {
                        GalaxyCluster^ thisGalaxyCluster = Hubble::CreateGalaxyCluster(pForm2, pForm2);
                        if (thisGalaxyCluster != nullptr)
                        {
                            Galaxy^ thisGalaxy = thisGalaxyCluster->CreateGalaxy(ctrl->Handle, ctrl->Name);
                            return thisGalaxy->Observe(key, strGridXml);
                        }
                    }
                }
                else
                {
                    hTopWnd = ::GetParent((HWND)topCtrl->Handle.ToPointer());
                }
            }
            if (hTopWnd != nullptr)
            {
                HWND _hTopWnd = hTopWnd;
                Grid^ pGrid = GetGridFromHandle((IntPtr)hTopWnd);
                while (pGrid == nullptr)
                {
                    hTopWnd = ::GetParent(hTopWnd);
                    pGrid = GetGridFromHandle((IntPtr)hTopWnd);
                }
                if (pGrid != nullptr)
                {
                    IGalaxyCluster* pIGalaxyCluster = nullptr;
                    pGrid->m_pGrid->get_GalaxyCluster(&pIGalaxyCluster);
                    if (pIGalaxyCluster)
                    {
                        String^ strName = ctrl->Name;
                        BSTR bstrName = STRING2BSTR(strName);
                        Grid^ _pRetGrid = nullptr;
                        HWND hWnd = (HWND)ctrl->Handle.ToPointer();
                        pIGalaxyCluster->CreateGalaxy(CComVariant((__int64)0), CComVariant((__int64)hWnd), bstrName, &pGalaxy);
                        if (pGalaxy)
                        {
                            IGrid* pGrid = nullptr;
                            BSTR bstrKey = STRING2BSTR(key);
                            BSTR bstrXml = STRING2BSTR(strGridXml);
                            pGalaxy->Observe(bstrKey, bstrXml, &pGrid);
                            ::SysFreeString(bstrKey);
                            ::SysFreeString(bstrXml);
                            if (pGrid)
                                _pRetGrid = theAppProxy._createObject<IGrid, Grid>(pGrid);
                        }
                        ::SysFreeString(bstrName);
                        ::InvalidateRect(hWnd, nullptr, true);
                        return _pRetGrid;
                    }
                }
                else
                {
                    GalaxyCluster^ thisGalaxyCluster = Hubble::CreateGalaxyCluster((IntPtr)_hTopWnd);
                    if (thisGalaxyCluster != nullptr)
                    {
                        Galaxy^ thisGalaxy = thisGalaxyCluster->CreateGalaxy(ctrl->Handle, ctrl->Name);
                        return thisGalaxy->Observe(key, strGridXml);
                    }
                }
            }
        }
        return nullptr;
    }

    WpfApplication::WpfApplication()
    {
        if (theAppProxy.m_bInitApp == false)
        {
            theAppProxy.m_pHubbleWpfApp = this;
            ::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, 0, 20191022);
        }
    }

    Hubble::Hubble()
    {
        m_pUniverseAppProxy = nullptr;
    }

    Hubble::Hubble(IHubble* pHubble)
    {
    }

    Hubble::~Hubble(void)
    {
        for each (KeyValuePair<String^, Object^>^ dic in Cosmos::Hubble::m_pHubbleCLRObjDic)
        {
            if (dic->Value != nullptr)
            {
                Object^ value = dic->Value;
                delete value;
            }
        }
    }

    int Hubble::HubbleInit(String^ strInit)
    {
        CString strInfo = strInit;
        if (strInfo != _T(""))
        {
            __int64 nPointer = _wtoi64(strInfo);
            if (nPointer)
            {
                theApp.m_pHubbleImpl = (CHubbleImpl*)nPointer;
                theApp.m_pHubbleImpl->m_pCLRProxy = &theAppProxy;
                theApp.m_pHubbleImpl->m_pCosmosAppProxy = static_cast<IUniverseAppProxy*>(&theApp);
                //20180817
                if (theApp.m_pHubble == nullptr)
                {
                    HMODULE hModule = ::GetModuleHandle(_T("universe.dll"));
                    if (hModule) {
                        typedef IHubble* (__stdcall* GetHubble)();
                        GetHubble _pHubbleFunction;
                        _pHubbleFunction = (GetHubble)GetProcAddress(hModule, "GetHubble");
                        if (_pHubbleFunction != NULL) {
                            theApp.m_pHubble = _pHubbleFunction();
                        }
                    }
                }
            }
        }
        return 0;
    }

    void Hubble::InitEclipse()
    {
        if (theApp.m_pHubble && !theApp.m_pHubbleImpl->m_bIsEclipseInit)
            theApp.m_pHubble->InitEclipseApp();
    }

    void Hubble::Run()
    {
        if (::PathFileExists(L"chrome_rt.dll"))
        {
            if (Hubble::WebRuntimeInit())
                return;
        }
        System::Windows::Forms::Application::Run();
    }

    void Hubble::Run(Form^ Mainform)
    {
        if (::PathFileExists(L"chrome_rt.dll"))
        {
            if (Hubble::WebRuntimeInit())
                return;
            Hubble::MainForm::set(Mainform);
        }
        System::Windows::Forms::Application::Run(Mainform);
    }

    void Hubble::Run(ApplicationContext^ context)
    {
        if (::PathFileExists(L"chrome_rt.dll"))
        {
            if (Hubble::WebRuntimeInit())
                return;
        }
        System::Windows::Forms::Application::Run(context);
    }

    Hubble^ Hubble::GetHubble()
    {
        if (theApp.m_pHubble == nullptr)
        {
            HMODULE hModule = ::GetModuleHandle(L"universe.dll");
            if (hModule == nullptr) {
                TCHAR m_szBuffer[MAX_PATH];
                if (SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, 0, m_szBuffer) ==
                    S_OK) {
                    ATL::CString m_strProgramFilePath = ATL::CString(m_szBuffer);
                    m_strProgramFilePath += _T("\\Hubble\\universe.dll");
                    if (::PathFileExists(m_strProgramFilePath)) {
                        hModule = ::LoadLibrary(m_strProgramFilePath);
                    }
                }
            }
            if (hModule) {
                typedef CHubbleImpl* (__stdcall* GetHubbleImpl)(IHubble**);
                GetHubbleImpl _pHubbleImplFunction;
                _pHubbleImplFunction = (GetHubbleImpl)GetProcAddress(hModule, "GetHubbleImpl");
                if (_pHubbleImplFunction != NULL) {
                    theApp.m_pHubbleImpl = _pHubbleImplFunction(&theApp.m_pHubble);
                    if (theApp.m_pHubbleImpl->m_nAppType == 0)
                        theApp.m_pHubbleImpl->m_nAppType = APP_BROWSERAPP;
                    theApp.m_pHubbleImpl->m_pHubbleDelegate = (IHubbleDelegate*)&theApp;
                    theApp.m_pHubbleImpl->m_pUniverseAppProxy = (IUniverseAppProxy*)&theApp;
                    theApp.m_pHubbleImpl->m_pCLRProxy = &theAppProxy;
                }
            }
        }
        if (m_pHubble == nullptr)
            m_pHubble = gcnew Hubble();
        return m_pHubble;
    }

    Hubble^ Hubble::InitHubbleApp(bool bSupportCrashReporting, CosmosAppType AppType)
    {
        if (m_pHubble == nullptr)
            m_pHubble = gcnew Hubble();
        if (theApp.m_bBrowserModeInit)
            return m_pHubble;
        theApp.m_pHubbleImpl->m_nAppType = (DWORD)AppType;
        //m_nAppType = AppType;
        switch (AppType)
        {
        case CosmosAppType::APPBROWSER:
        case CosmosAppType::APPBROWSER_ECLIPSE:
            theApp.InitHubbleApp(bSupportCrashReporting);
            break;
        case CosmosAppType::APPECLIPSE:
            if (theApp.m_pHubble && !theApp.m_pHubbleImpl->m_bIsEclipseInit)
                theApp.m_pHubble->InitEclipseApp();
            break;
        case CosmosAppType::APPBROWSERAPP:
            break;
        }
        return m_pHubble;
    }

    String^ Hubble::ComputeHash(String^ source)
    {
        BSTR bstrSRC = STRING2BSTR(source);
        LPCWSTR srcInfo = OLE2T(bstrSRC);
        std::string strSrc = (LPCSTR)CW2A(srcInfo, CP_UTF8);
        int nSrcLen = (int)strSrc.length();
        CString strRet = _T("");
        theApp.CalculateByteMD5((BYTE*)strSrc.c_str(), nSrcLen, strRet);
        ::SysFreeString(bstrSRC);
        return BSTR2STRING(strRet);
    }

    bool Hubble::SupportCrashReporting::get()
    {
        return theApp.m_pHubbleImpl->m_bIsSupportCrashReporting;
    }

    void Hubble::SupportCrashReporting::set(bool bSupportCrashReporting)
    {
        if (theApp.m_pHubbleImpl->m_bIsSupportCrashReporting == false)
            theApp.m_pHubbleImpl->m_bIsSupportCrashReporting = bSupportCrashReporting;
    }

    CosmosAppType Hubble::AppType::get()
    {
        DWORD _nType = theApp.m_pHubbleImpl->m_nAppType;
        switch (_nType)
        {
        case APP_WIN32:
            return CosmosAppType::APPWIN32;
            break;
        case APP_BROWSER:
            return CosmosAppType::APPBROWSER;
            break;
        case APP_ECLIPSE:
            return CosmosAppType::APPECLIPSE;
            break;
        case APP_BROWSER_ECLIPSE:
            return CosmosAppType::APPBROWSER_ECLIPSE;
            break;
        case APP_BROWSERAPP:
            return CosmosAppType::APPBROWSERAPP;
            break;
        default:
            break;
        }
        return CosmosAppType::APPOTHER;
    }

    void Hubble::AppType::set(CosmosAppType nType)
    {
        if (m_pHubble == nullptr)
            m_pHubble = gcnew Hubble();
        theApp.m_pHubbleImpl->m_nAppType = (DWORD)nType;
    }

    System::Drawing::Icon^ Hubble::DefaultIcon::get()
    {
        if (Cosmos::Hubble::m_pDefaultIcon == nullptr)
        {
            Form^ _pForm = gcnew Form();
            Cosmos::Hubble::m_pDefaultIcon = _pForm->Icon;
        }
        return Cosmos::Hubble::m_pDefaultIcon;
    }

    bool Hubble::WebRuntimeInit()
    {
        if (IsWebRuntimeInit)
            return true;

        HMODULE hModule = ::GetModuleHandle(L"chrome_rt.dll");
        if (hModule == nullptr)
            hModule = ::LoadLibrary(L"chrome_rt.dll");
        if (hModule) {
            typedef bool(__stdcall* _IsChromeRunning)(bool bSupportCrashReporting);
            _IsChromeRunning FuncIsChromeRunning;
            FuncIsChromeRunning = (_IsChromeRunning)GetProcAddress(hModule, "IsBrowserModel");
            if (FuncIsChromeRunning != NULL) {
                IsWebRuntimeInit = FuncIsChromeRunning(false);
                if (theAppProxy.m_bInitApp == false)
                {
                    //if (System::Diagnostics::Debugger::IsAttached)
                    //{
                    //    StartApplication(L"caswebagent.server.1", L"fordebug");
                    //    OnHubbleDebugDelegate += gcnew TangramDebugDelegate(&OnHubble_DebugDelegate);
                    //}
                    ::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, 0, 20191022);
                }
            }
        }
        return IsWebRuntimeInit;
    }

    Browser^ Hubble::HostWebBrowser::get()
    {
        if (theApp.m_pHubble)
        {
            IBrowser* pWebBrowser = nullptr;
            theApp.m_pHubble->get_HostChromeBrowserWnd(&pWebBrowser);
            if (pWebBrowser)
            {
                auto it = theAppProxy.m_mapWebBrowser.find(pWebBrowser);
                if (it != theAppProxy.m_mapWebBrowser.end())
                    return it->second;
            }
        }
        return nullptr;
    }

    Hubble^ Hubble::TangramCore::get()
    {
        return GetHubble();
    }

    Form^ Hubble::MainForm::get()
    {
        return m_pMainForm;
    }

    void Hubble::MainForm::set(Form^ frm)
    {
        if (m_pMainForm == nullptr)
        {
            m_pMainForm = frm;
            if (m_pMainForm != nullptr)
            {
                theApp.m_pHubbleImpl->SetMainWnd((HWND)frm->Handle.ToPointer());
                ::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, 0, 20191004);
            }
        }
    }

    String^ Hubble::CurrentDesigningTangramXml::get()
    {
        IGrid* pDesignerNode = nullptr;
        theApp.m_pHubble->get_DesignNode(&pDesignerNode);
        if (pDesignerNode == NULL)
            return L"";
        if (pDesignerNode)
        {
            CComBSTR bstrXml(L"");
            pDesignerNode->get_DocXml(&bstrXml);
            return BSTR2STRING(bstrXml);
        }
        return nullptr;
    }

    String^ Hubble::AppData::get()
    {
        if (theApp.m_pHubbleVS == nullptr)
        {
            CComVariant var;
            theApp.m_pHubble->get_AppKeyValue(CComBSTR("vstangramhandle"), &var);
            __int64 nHandle = var.llVal;
            if (nHandle)
            {
                theApp.m_pHubbleVS = (IHubble*)nHandle;
            }
        }
        if (String::IsNullOrEmpty(m_strAppData) && theApp.m_pHubbleVS)
        {
            CComVariant var;
            theApp.m_pHubbleVS->get_AppKeyValue(CComBSTR("appdata"), &var);
            CString strXml = OLE2T(var.bstrVal);
            if (strXml != _T(""))
            {
                m_strAppData = BSTR2STRING(strXml);
            }
        }
        return m_strAppData;
    }

    Grid^ Hubble::CreatingGrid::get()
    {
        Object^ pRetObject = nullptr;
        if (theApp.m_pHubble)
        {
            IGrid* pGrid = nullptr;
            theApp.m_pHubble->get_CreatingGrid(&pGrid);
            if (pGrid)
                return theAppProxy._createObject<IGrid, Grid>(pGrid);
        }
        return nullptr;
    }

    Galaxy^ Hubble::ActiveEclipseTopFrame::get()
    {
        Object^ pRetObject = nullptr;
        if (theApp.m_pHubble)
        {
            IWorkBenchWindow* pTopWnd = nullptr;
            IHubbleExtender* pExtender = nullptr;
            theApp.m_pHubble->get_Extender(&pExtender);
            if (pExtender)
            {
                CComQIPtr<IEclipseExtender> pEclipse(pExtender);
                if (pEclipse)
                    pEclipse->get_ActiveWorkBenchWindow(CComBSTR(L""), &pTopWnd);
                if (pTopWnd)
                {
                    IGalaxy* pGalaxy = nullptr;
                    pTopWnd->get_Galaxy(&pGalaxy);
                    if (pGalaxy)
                        return theAppProxy._createObject<IGalaxy, Galaxy>(pGalaxy);
                }
            }
        }
        return nullptr;
    }

    Hubble^ Hubble::RemoteTangram::get(String^ strID)
    {
        if (theApp.m_pHubble)
        {
            IHubble* pHubble = nullptr;
            BSTR bstrID = STRING2BSTR(strID);
            theApp.m_pHubble->get_RemoteHubble(bstrID, &pHubble);
            if (pHubble)
            {
                Hubble^ _pHubble = theAppProxy._createObject<IHubble, Hubble>(pHubble);
                return _pHubble;
            }
        }
        return nullptr;
    }

    String^ Hubble::NTPXml::get()
    {
        BSTR bstrNtpXml = theApp.m_pHubbleImpl->m_strNtpXml.AllocSysString();
        String^ strResult = BSTR2STRING(bstrNtpXml);
        ::SysFreeString(bstrNtpXml);
        return strResult;
    }

    void Hubble::NTPXml::set(String^ strXml)
    {
        CString _strXml = strXml;
        CTangramXmlParse m_Parse;
        if (m_Parse.LoadXml(_strXml))
        {
            CTangramXmlParse* pParse = m_Parse.GetChild(TGM_CLUSTER);
            if (pParse)
            {
                int nCount = pParse->GetCount();
                if (nCount == 1)
                {
                    pParse = pParse->GetChild(TGM_GRID);
                    if (pParse)
                    {
                        theApp.m_pHubbleImpl->m_strNtpXml = _strXml;
                    }
                }
            }
        }
    }

    String^ Hubble::WizData::get()
    {
        return Cosmos::Hubble::m_strWizData;
    }

    void Hubble::WizData::set(String^ strXml)
    {
        Cosmos::Hubble::m_strWizData = strXml;
    }

    TangramAppProxy^ Hubble::AppProxy::get(String^ strKey)
    {
        String^ _strKey = strKey->ToLower()->Trim();
        TangramAppProxy^ _pProxy = nullptr;
        if (m_pHubbleAppProxyDic->TryGetValue(_strKey, _pProxy) != true)
        {
            CComBSTR bstrKey(STRING2BSTR(_strKey));
            LONGLONG llProxy = 0;
            theApp.m_pHubble->LoadDocComponent(bstrKey, &llProxy);
            if (llProxy)
            {
                IUniverseAppProxy* pProxy = (IUniverseAppProxy*)llProxy;
                _pProxy = gcnew TangramAppProxy();
                _pProxy->m_pUniverseAppProxy = pProxy;
                m_pHubbleAppProxyDic[_strKey] = _pProxy;
            }
        }

        return _pProxy;
    }

    WorkBenchWindow^ Hubble::ActiveWorkBenchWindow::get()
    {
        Object^ pRetObject = nullptr;
        if (theApp.m_pHubble)
        {
            IWorkBenchWindow* pTopWnd = nullptr;
            IHubbleExtender* pExtender = nullptr;
            theApp.m_pHubble->get_Extender(&pExtender);
            if (pExtender)
            {
                CComQIPtr<IEclipseExtender> pEclipse(pExtender);
                if (pEclipse)
                    pEclipse->get_ActiveWorkBenchWindow(CComBSTR(L""), &pTopWnd);
                if (pTopWnd)
                {
                    return theAppProxy._createObject<IWorkBenchWindow, WorkBenchWindow>(pTopWnd);
                }
            }
        }
        return nullptr;
    }

    String^ Hubble::AppKeyValue::get(String^ iIndex)
    {
        auto it = theApp.m_pHubbleImpl->m_mapValInfo.find(STRING2BSTR(iIndex));
        if (it != theApp.m_pHubbleImpl->m_mapValInfo.end())
        {
            return BSTR2STRING(it->second.bstrVal);
        }
        //CComVariant bstrVal(::SysAllocString(L""));
        //theApp.m_pHubble->get_AppKeyValue(STRING2BSTR(iIndex), &bstrVal);
        //String^ strVal = BSTR2STRING(bstrVal.bstrVal);
        //::SysFreeString(bstrVal);
        return L"";
    }

    void Hubble::AppKeyValue::set(String^ iIndex, String^ newVal)
    {
        theApp.m_pHubble->put_AppKeyValue(STRING2BSTR(iIndex), CComVariant(STRING2BSTR(newVal)));
    }

    void Hubble::Fire_OnCloudAppIdle()
    {
        OnCloudAppIdle();
    }

    Object^ Hubble::Fire_OnGetSubObjForWebPage(Object^ SourceObj, String^ subObjName)
    {
        return OnGetSubObjForWebPage(SourceObj, subObjName);
    }

    void Hubble::Fire_OnBindCLRObjToWebPage(Object^ SourceObj, Cosmos::Wormhole^ eventSession, String^ eventName)
    {
        Control^ pCtrl = (Control^)SourceObj;
        String^ strEventInfo = L"@" + pCtrl->Name+L"_"+eventName + L"@";
        if (eventSession->m_strEvents->IndexOf(strEventInfo->ToLower()) != -1)
            return;
        eventSession->m_strEvents += strEventInfo->ToLower();
        intptr_t nNode = eventSession->m_pHubbleSession->Getint64(L"gridobj");
        if (nNode)
        {
            IGrid* pGrid = (IGrid*)nNode;
            Grid^ thisNode = theAppProxy._createObject<IGrid, Grid>(pGrid);
            if (thisNode != nullptr)
            {
                thisNode->Fire_OnBindCLRObjToWebPage(SourceObj, eventSession, eventName);
            }
        }
        OnBindCLRObjToWebPage(SourceObj, eventSession, eventName);
    }

    void Hubble::Fire_OnEclipseAppInit()
    {
        OnEclipseAppInit();
    }

    bool Hubble::Fire_OnAppInit()
    {
        if (IsAppInit == false)
        {
            IsAppInit = true;
            return OnAppInit();
        }
        return false;
    }

    void Hubble::Fire_OnClose()
    {
        OnClose();
    }

    Control^ Hubble::GetMDIClient(Form^ pForm)
    {
        if (pForm && pForm->IsMdiContainer)
        {
            String^ strTag = L"";
            Object^ tag = pForm->Tag;
            if (tag != nullptr)
                strTag = tag->ToString();

            CString _strTag = _T("");
            if (String::IsNullOrEmpty(strTag) == false)
                _strTag = STRING2BSTR(strTag);
            if (_strTag == _T(""))
                _strTag = _T("default");
            int nCount = pForm->Controls->Count;
            String^ strName = L"";
            for (int i = nCount - 1; i >= 0; i--)
            {
                Control^ pCtrl = pForm->Controls[i];
                strName = pCtrl->GetType()->Name->ToLower();
                if (strName == L"mdiclient")
                {
                    return pCtrl;
                }
            }
        }
        return nullptr;
    }

#ifndef _WIN64
    bool Hubble::ReplaceHTML(Object^ doc, String^ originHTML, String^ newHTML)
    {
        if (doc == nullptr)
        {
            return false;
        }
        IntPtr nDisp = IntPtr::Zero;
        try
        {
            nDisp = Marshal::GetIDispatchForObject(doc);
            IDispatch* pDisp = (IDispatch*)nDisp.ToPointer();
            if (::GetModuleHandle(_T("msenv.dll")))
            {
                CComQIPtr<VxDTE::Document> pDocument(pDisp);
                if (pDocument)
                {
                    VARIANT_BOOL bSuccess = 0;
                    CComBSTR bstrType("");
                    pDocument->get_Type(&bstrType);
                    CString strType = OLE2T(bstrType);
                    if (strType == _T("Text"))
                    {
                        BSTR bstrNewHTML = STRING2BSTR(newHTML);
                        BSTR bstrOriginHTML = STRING2BSTR(originHTML);
                        pDocument->ReplaceText(bstrOriginHTML, bstrNewHTML, 0, &bSuccess);
                        ::SysFreeString(bstrOriginHTML);
                        ::SysFreeString(bstrNewHTML);
                        return (bool)bSuccess;
                    }
                }
            }
        }
        catch (InvalidCastException^)
        {
        }
        finally
        {
            Marshal::Release(nDisp);
        }
        return false;
    }

    void Hubble::ShowVSToolBox(IntPtr nHandle)
    {
        HWND hWnd = (HWND)nHandle.ToPointer();
        if (::IsWindow(hWnd))
        {
            HWND hWndChild = ::GetWindow(hWnd, GW_CHILD);
            IGalaxy* pGalaxy = nullptr;
            theApp.m_pHubble->GetGalaxy((__int64)hWndChild, &pGalaxy);
            if (pGalaxy)
            {
                pGalaxy->Attach();
            }
        }
    }

    void Hubble::SelectObject(String^ strData, Object^ ObjSelected, IntPtr nHandle) {
        String^ strDebug = L"";
        bool bDebug = false;
        if (strData->IndexOf(L"Replace:") == 0)
        {
            if (ObjSelected != nullptr)
            {
                try
                {
                    IntPtr nDisp = Marshal::GetIDispatchForObject(ObjSelected);
                    IDispatch* pDisp = (IDispatch*)nDisp.ToPointer();
                    theApp.m_pHubble->SelectVSObj(STRING2BSTR(strData), pDisp, (__int64)nHandle.ToPointer());
                    Marshal::Release(nDisp);
                }
                catch (InvalidCastException^)
                {
                }
            }
            return;
        }
        else if (strData->IndexOf(L"Debug:") == 0)
        {
            bDebug = true;
            //strData = strData->Replace(L"Debug:", L"");
        }
        if (theApp.m_pHubbleVS == nullptr)
        {
            CComVariant var;
            theApp.m_pHubble->get_AppKeyValue(CComBSTR("vstangramhandle"), &var);
            __int64 nHandle = var.llVal;
            if (nHandle)
            {
                theApp.m_pHubbleVS = (IHubble*)nHandle;
            }
        }
        if (theApp.m_pHubbleVS)
        {
            if (bDebug == true)
            {
                theApp.m_pHubbleVS->SelectVSObj(STRING2BSTR(strData + L":" + ObjSelected->ToString()), nullptr, (__int64)nHandle.ToPointer());
            }
            else
            {
                if (ObjSelected != nullptr)
                {
                    try
                    {
                        IntPtr nDisp = Marshal::GetIDispatchForObject(ObjSelected);
                        IDispatch* pDisp = (IDispatch*)nDisp.ToPointer();
                        theApp.m_pHubbleVS->SelectVSObj(STRING2BSTR(strData), pDisp, (__int64)nHandle.ToPointer());
                        Marshal::Release(nDisp);
                    }
                    catch (InvalidCastException^)
                    {
                    }
                }
                else
                {
                    theApp.m_pHubbleVS->SelectVSObj(STRING2BSTR(strData), nullptr, (__int64)nHandle.ToPointer());
                }
            }
        }
    }

    void Hubble::SolutionEvent(String^ strSolutionName, String^ PrjName, String^ PrjType, IntPtr eventID)
    {
        if (theApp.m_pHubbleImpl)
        {
            CString strInfo1 = strSolutionName + L"@" + PrjName + L"@" + PrjType;
            for (auto it : theApp.m_pHubbleImpl->m_mapVSWebPage)
            {
                CString str = _T("");
                str.Format(_T("%s|%s|%d"), _T("SolutionEvent"), strInfo1, eventID.ToInt32());
                ::SendMessage(it.first, WM_COSMOSMSG, (WPARAM)str.GetBuffer(), 20200606);
                str.ReleaseBuffer();
            }
        }
    }

    Galaxy^ Hubble::CreateVSToolWnd(String^ strXml)
    {
        if (m_nHWebRuntimeToolWndPane == IntPtr::Zero)
        {
            CComVariant var;
            theApp.m_pHubble->get_AppKeyValue(CComBSTR("webruntimetoolwndpane"), &var);
            m_nHWebRuntimeToolWndPane = (IntPtr)var.llVal;
        }
        if (m_nHWebRuntimeToolWndPane != IntPtr::Zero)
        {
            HWND hWnd = (HWND)m_nHWebRuntimeToolWndPane.ToPointer();
            CString _strXml = strXml;
            LRESULT l = ::SendMessage(hWnd, WM_COSMOSMSG, (WPARAM)_strXml.GetBuffer(), 20200525);
            _strXml.ReleaseBuffer();
            hWnd = (HWND)l;
            if (::IsWindow(hWnd))
            {
                IGalaxy* pGalaxy = (IGalaxy*)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
                return theAppProxy._createObject<IGalaxy, Galaxy>(pGalaxy);
            }
        }
        return nullptr;
    }

    IntPtr Hubble::GetChild(IntPtr nHandle)
    {
        HWND hWnd = (HWND)nHandle.ToPointer();
        if (::IsWindow(hWnd))
        {
            HWND hWndChild = ::GetWindow(hWnd, GW_CHILD);
            TCHAR	szBuffer[MAX_PATH];
            ::GetClassName(hWnd, szBuffer, MAX_PATH);
            CString strClassName = CString(szBuffer);
            if (strClassName == _T("GenericPane"))
            {
                ::GetClassName(hWndChild, szBuffer, MAX_PATH);
                strClassName = CString(szBuffer);
                if (strClassName == _T("TBToolboxPane"))
                {
                    if (theApp.m_bVSToolBoxConnected == true)
                        return (IntPtr)hWndChild;
                    theApp.m_bVSToolBoxConnected = true;
                    if (theApp.m_pHubbleVS == nullptr)
                    {
                        CComVariant var;
                        theApp.m_pHubble->get_AppKeyValue(CComBSTR("vstangramhandle"), &var);
                        __int64 nHandle = var.llVal;
                        if (nHandle)
                        {
                            theApp.m_pHubbleVS = (IHubble*)nHandle;
                        }
                    }
                    if (theApp.m_pHubbleVS)
                    {
                        CComVariant var;
                        theApp.m_pHubbleVS->get_AppKeyValue(CComBSTR("appdata"), &var);
                        CString strXml = OLE2T(var.bstrVal);
                        if (strXml != _T(""))
                        {
                            m_strAppData = BSTR2STRING(strXml);
                            CTangramXmlParse m_Parse;
                            if (m_Parse.LoadXml(strXml))
                            {
                                CTangramXmlParse* pVSParse = m_Parse.GetChild(_T("vstoolbox"));
                                if (pVSParse)
                                {
                                    IGalaxyCluster* pManager = nullptr;
                                    theApp.m_pHubble->CreateGalaxyCluster((LONGLONG)hWnd, &pManager);
                                    if (pManager)
                                    {
                                        IGalaxy* pIGalaxy = nullptr;
                                        pManager->CreateGalaxy(CComVariant((LONGLONG)hWnd), CComVariant((__int64)hWndChild), CComBSTR("default"), &pIGalaxy);
                                        if (pIGalaxy)
                                        {
                                            IGrid* pGrid = nullptr;
                                            pIGalaxy->Observe(CComBSTR("default"), CComBSTR(pVSParse->xml()), &pGrid);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            ::PostMessage(hWnd, WM_COSMOSMSG, (WPARAM)hWndChild, 20200531);
            return (IntPtr)hWndChild;
        }
        return IntPtr::Zero;
    }
#endif

    void Hubble::CreateBKPage(Form^ form, String^ strID)
    {
        Control^ mdiclient = Hubble::GetMDIClient(form);
        if (mdiclient)
        {
            ::SendMessage((HWND)form->Handle.ToPointer(), WM_MDICLIENTCREATED, (WPARAM)mdiclient->Handle.ToInt64(), (LPARAM)STRING2BSTR(strID));
        }
    }

    Grid^ Hubble::ExtendMDIClient(Form^ pForm, String^ strKey, String^ strXml)
    {
        Control^ mdiclient = Hubble::GetMDIClient(pForm);
        if (mdiclient)
        {
            GalaxyCluster^ page = Hubble::CreateGalaxyCluster(pForm, nullptr);
            Galaxy^ frame = page->CreateGalaxy(mdiclient, L"default");
            if (frame)
            {
                if (strKey == L"newdoc")
                {
                    return frame->Observe(strKey, BSTR2STRING(theApp.m_pHubbleImpl->m_strNewDocXml));
                }
                return frame->Observe(strKey, strXml);
            }
        }
        return nullptr;
    }

    Browser^ Hubble::ActiveBrowser()
    {
        IBrowser* pBrowser = nullptr;
        theApp.m_pHubble->get_ActiveChromeBrowserWnd(&pBrowser);
        if (pBrowser)
        {
            auto it = theAppProxy.m_mapWebBrowser.find(pBrowser);
            if (it != theAppProxy.m_mapWebBrowser.end())
                return it->second;
            else
            {
                Browser^ _pBrowser = gcnew Browser(pBrowser);
                theAppProxy.m_mapWebBrowser[pBrowser] = _pBrowser;
                return _pBrowser;
            }
        }
        return nullptr;
    }

    Browser^ Hubble::GetHostBrowser(Object^ obj)
    {
        if (obj == nullptr)
        {
            return nullptr;
        }
        HWND hWnd = nullptr;
        if (obj->GetType()->IsSubclassOf(Control::typeid) || obj->GetType() == Control::typeid)
        {
            Control^ ctrl = (Control^)obj;
            hWnd = (HWND)(ctrl->Handle.ToPointer());
        }
        else if (obj->GetType()->IsSubclassOf(System::Windows::Media::Visual::typeid) ||
            obj->GetType() == System::Windows::Media::Visual::typeid)
        {
            System::Windows::Media::Visual^ vis = (System::Windows::Media::Visual^)obj;
            System::Windows::PresentationSource^ ps = System::Windows::Interop::HwndSource::FromVisual(vis);
            if (ps != nullptr)
            {
                System::Windows::Interop::HwndSource^ hwnd = (System::Windows::Interop::HwndSource^)ps;
                hWnd = (HWND)(hwnd->Handle.ToPointer());
                hWnd = ::GetParent(hWnd);
                if (::IsWindow(hWnd))
                {
                    hWnd = ::GetParent(hWnd);
                }
            }
        }
        if (hWnd == nullptr)
        {
            return nullptr;
        }
        IGrid* pWndGrid = nullptr;
        HRESULT hr = theApp.m_pHubble->GetGridFromHandle((LONGLONG)hWnd, &pWndGrid);
        if (hr != S_OK || pWndGrid == nullptr)
        {
            return nullptr;
        }
        IGalaxy* pGalaxy = nullptr;
        hr = pWndGrid->get_Galaxy(&pGalaxy);
        if (hr != S_OK || pGalaxy == nullptr)
        {
            return nullptr;
        }
        IBrowser* pBrowser = nullptr;
        pGalaxy->get_HostBrowser(&pBrowser);
        if (pBrowser == nullptr)
        {
            return nullptr;
        }
        auto it = theAppProxy.m_mapWebBrowser.find(pBrowser);
        if (it != theAppProxy.m_mapWebBrowser.end())
            return it->second;
        else
        {
            Browser^ _pBrowser = gcnew Browser(pBrowser);
            theAppProxy.m_mapWebBrowser[pBrowser] = _pBrowser;
            return _pBrowser;
        }
    }

    void Hubble::RegComponentForTangram(String^ strIDs, Assembly^ a)
    {
        if (a != nullptr /*&& String::IsNullOrEmpty(strIDs) == false*/)
        {
            strIDs = strIDs->ToLower();
            cli::array<Type^>^ pArray = a->GetExportedTypes();
            for each (Type ^ type in pArray)
            {
                if (type->IsSubclassOf(Control::typeid) || type->IsSubclassOf(System::Windows::FrameworkElement::typeid))
                {
                    String^ strName = type->AssemblyQualifiedName->ToLower();
                    String^ s = strName->Substring(0, strName->IndexOf(", version"))->Replace(" ", "");
                    Type^ _pType = nullptr;
                    if (String::IsNullOrEmpty(strIDs))
                    {
                        if (GetHubble()->m_pHubbleCLRTypeDic->TryGetValue(s, _pType) == false)
                            GetHubble()->m_pHubbleCLRTypeDic->Add(s, type);
                    }
                    else if (strIDs->IndexOf(s) != -1)
                    {
                        if (GetHubble()->m_pHubbleCLRTypeDic->TryGetValue(s, _pType) == false)
                            GetHubble()->m_pHubbleCLRTypeDic->Add(s, type);
                    }
                }
            }
        }
    }

    Grid^ Hubble::GetGridFromHandle(IntPtr handle)
    {
        IGrid* pWndGrid = nullptr;
        HRESULT hr = theApp.m_pHubble->GetGridFromHandle((LONGLONG)handle.ToPointer(), &pWndGrid);
        if (hr != S_OK || pWndGrid == nullptr)
        {
            return nullptr;
        }
        return theAppProxy._createObject<IGrid, Grid>(pWndGrid);
    }

    Grid^ Hubble::GetGridFromControl(Control^ ctrl)
    {
        if (ctrl == nullptr)
        {
            return nullptr;
        }
        IGrid* pWndGrid = nullptr;
        HRESULT hr = theApp.m_pHubble->GetGridFromHandle((LONGLONG)ctrl->Handle.ToPointer(), &pWndGrid);
        if (hr != S_OK || pWndGrid == nullptr)
        {
            return nullptr;
        }
        return theAppProxy._createObject<IGrid, Grid>(pWndGrid);
    }

    void Hubble::UpdateNewTabPageLayout(String^ newTabPageLayout)
    {
        Hubble::NTPXml::set(newTabPageLayout);
    }

    Wormhole^ Hubble::GetWormholeFromObj(Object^ obj)
    {
        Cosmos::Wormhole^ pCloudSession = nullptr;
        bool bExists = Cosmos::Hubble::Wormholes->TryGetValue(obj, pCloudSession);
        if (bExists)
        {
            return pCloudSession;
        }
        return nullptr;
    }

    void Hubble::SendXmlMessage(Grid^ sender, String^ strXml)
    {
        BSTR bstrXml = STRING2BSTR(strXml);
        IWebPage* pPage = nullptr;
        sender->m_pGrid->get_WebPage(&pPage);
        if (pPage)
        {
            BSTR bstrXml = STRING2BSTR(strXml);
            pPage->SendXmlMessage(sender->m_pGrid, bstrXml);
            ::SysFreeString(bstrXml);
        }
        //theApp.m_pHubble->SendXmlMessage(sender->m_pGrid, bstrXml);
        ::SysFreeString(bstrXml);
    }

    void Hubble::BindObjToWebPage(IntPtr hWebPage, Object^ pObj, String^ strWebName)
    {
        HWND hWnd = (HWND)hWebPage.ToPointer();
        CWebPageImpl* pProxy = nullptr;
        auto it = theAppProxy.m_mapChromeWebPage.find(hWnd);
        if (it != theAppProxy.m_mapChromeWebPage.end())
        {
            pProxy = it->second->m_pProxy;
        }
        Cosmos::Wormhole^ pCloudSession = nullptr;
        CSession* pSession = nullptr;
        bool bExists = Cosmos::Hubble::Wormholes->TryGetValue(pObj, pCloudSession);
        if (bExists)
        {
            pSession = pCloudSession->m_pHubbleSession;
            Type^ pType = pObj->GetType();
            pCloudSession->InsertString(_T("msgID"), _T("BindCLRObject"));
            pCloudSession->InsertString(_T("objtype"), pType->FullName);
            pCloudSession->InsertString(_T("objID"), _T(".netobj"));
            pCloudSession->InsertString(_T("action"), _T("webbind"));
            pCloudSession->InsertString(_T("name@page"), strWebName);
            if (pType->IsSubclassOf(Control::typeid))
            {
                pCloudSession->InsertInt64(_T("hwnd"), ((Control^)pObj)->Handle.ToInt64());
            }
            pCloudSession->InsertInt64(_T("domhandle"), (__int64)pSession);
            pCloudSession->InsertLong(_T("autodelete"), 0);
            pCloudSession->m_pHostObj = pObj;
            theAppProxy.m_mapSession2Wormhole[pSession] = pCloudSession;
            pSession->SendMessage();
        }
        else if (theApp.m_pHubbleImpl)
        {
            pSession = theApp.m_pHubbleImpl->CreateCloudSession(pProxy);
            Type^ pType = pObj->GetType();
            pSession->InsertString(_T("msgID"), _T("BindCLRObject"));
            pSession->InsertString(_T("objtype"), pType->FullName);
            pSession->InsertString(_T("objID"), _T(".netobj"));
            pSession->InsertString(_T("action"), _T("webbind"));
            pSession->InsertString(_T("name@page"), strWebName);
            if (pType->IsSubclassOf(Control::typeid))
            {
                pSession->Insertint64(_T("hwnd"), ((Control^)pObj)->Handle.ToInt64());
            }
            pSession->Insertint64(_T("domhandle"), (__int64)pSession);
            pSession->InsertLong(_T("autodelete"), 0);
            if (pCloudSession == nullptr)
                pCloudSession = gcnew Wormhole(pSession);
            pCloudSession->m_pHostObj = pObj;
            theAppProxy.m_mapSession2Wormhole[pSession] = pCloudSession;
            pSession->SendMessage();
        }
    }

    List<String^>^ Hubble::FindFiles(String^ rootPath, String^ fileSpec, bool recursive)
    {
        List<String^>^ pFiles = gcnew List<String^>();
        BSTR bstrRootPath = STRING2BSTR(rootPath);
        BSTR bstrFileSpec = STRING2BSTR(fileSpec);
        CString strRootPath = OLE2T(bstrRootPath);
        CString strFileSpec = OLE2T(bstrFileSpec);
        wchar_t strSearch[MAX_PATH] = L"";
        ::PathCombineW(strSearch, (LPCTSTR)strRootPath, (LPCTSTR)strFileSpec);
        _wfinddata_t fd;
        intptr_t pf = _wfindfirst(strSearch, &fd);
        if (pf != -1)
        {
            do
            {
                CString strFileName = fd.name;
                if (strFileName != _T(".."))
                {
                    if (fd.attrib ^ FILE_ATTRIBUTE_DIRECTORY)
                    {
                        CString strFullName = rootPath + L"\\" + strFileName;
                        String^ fullName = BSTR2STRING(CComBSTR(strFullName));
                        pFiles->Add(fullName);
                    }
                }
            } while (!_wfindnext(pf, &fd));
        }
        return pFiles;
    }

    Dictionary<String^, Type^>^ Hubble::GetFormTypesFromDirectory(String^ directoryPath)
    {
        Dictionary<String^, Type^>^ pFormTypes = gcnew Dictionary<String^, Type^>();
        List<String^>^ pFiles = FindFiles(directoryPath, "*.dll", false);
        for each (String ^ file in pFiles)
        {
            Dictionary<String^, Type^>^ _pFormTypes = GetFormTypesFromAssembly(file);
            for each (KeyValuePair<String^, Type^>^ formType in _pFormTypes)
            {
                pFormTypes[formType->Key] = formType->Value;
            }
        }
        return pFormTypes;
    }

    Dictionary<String^, Type^>^ Hubble::GetFormTypesFromAssembly(String^ assemblyFilePath)
    {
        Dictionary<String^, Type^>^ pFormTypes = gcnew Dictionary<String^, Type^>();
        BSTR bstrFile = STRING2BSTR(assemblyFilePath);
        CString strFile = OLE2T(bstrFile);
        CString strXMLFile = strFile + L".xml";
        CMarkup xml;
        if (xml.Load(strXMLFile) && xml.FindElem())
        {
            CString tagName = xml.GetTagName();
            if (tagName.CompareNoCase(L"FORMS") == 0)
            {
                Assembly^ pAssembly = nullptr;
                try
                {
                    pAssembly = Assembly::LoadFile(assemblyFilePath);
                    if (pAssembly != nullptr)
                    {
                        while (xml.FindChildElem())
                        {
                            CString tagName = xml.GetChildTagName();
                            if (tagName.CompareNoCase(L"FORM") == 0)
                            {
                                CString strType = xml.GetChildAttrib(L"type");
                                BSTR bstrType = strType.AllocSysString();
                                String^ type = BSTR2STRING(bstrType);
                                Type^ pType = pAssembly->GetType(type);
                                SysFreeString(bstrType);
                                if (pType != nullptr)
                                {
                                    pFormTypes[pType->FullName] = pType;
                                }
                            }
                        }
                    }
                }
                catch (Exception ^ e)
                {
                    Debug::WriteLine(L"GetFormTypesFromAssembly: {0}", e->Message);
                }
            }
        }
        else
        {
            CMarkup xml;
            xml.AddElem(L"forms");
            xml.IntoElem();
            BSTR bstrAssemblyFilePath = STRING2BSTR(assemblyFilePath);
            CString filePath = OLE2T(bstrAssemblyFilePath);
            Assembly^ pAssembly = nullptr;
            try
            {
                pAssembly = Assembly::LoadFile(assemblyFilePath);
            }
            catch (Exception ^ e)
            {
                Debug::WriteLine(L"GetFormTypesFromAssembly: {0}", e->Message);
            }
            if (pAssembly != nullptr)
            {
                cli::array<Type^>^ pTypes = pAssembly->GetExportedTypes();
                for each (Type ^ type in pTypes)
                {
                    if (type->IsSubclassOf(Form::typeid))
                    {
                        Form^ pTempInstance = nullptr;
                        try
                        {
                            pTempInstance = (Form^)Activator::CreateInstance(type);
                        }
                        finally
                        {
                            if (pTempInstance)
                            {
                                if (pTempInstance->FormBorderStyle == FormBorderStyle::FixedToolWindow ||
                                    pTempInstance->FormBorderStyle == FormBorderStyle::None ||
                                    pTempInstance->FormBorderStyle == FormBorderStyle::SizableToolWindow)
                                {
                                    // Nothing to do.
                                }
                                else
                                {
                                    pFormTypes[type->FullName] = type;
                                    BSTR bstrFormType = STRING2BSTR(type->FullName);
                                    CString strFormType = OLE2T(bstrFormType);
                                    xml.AddElem(L"form");
                                    xml.SetAttrib(L"type", strFormType);
                                }
                                pTempInstance->Close();
                            }
                        }
                    }
                }
            }
            xml.OutOfElem();
            xml.Save(strXMLFile);
        }
        return pFormTypes;
    }

    Dictionary<String^, Type^>^ Hubble::InitAppFormTypeDic()
    {
        if (m_pAppFormTypeDic == nullptr)
        {
            m_pAppFormTypeDic = gcnew Dictionary<String^, Type^>();
            CString filePath = System::Windows::Forms::Application::ExecutablePath->ToLower();
            int nPos = filePath.ReverseFind('.');
            CString strLib = filePath.Left(nPos).MakeLower();
            CString strAssemblys = _T("");
            String^ _strLib = BSTR2STRING(filePath);
            String^ strAssemblyLib = L"";
            strLib = filePath.MakeLower();
            Assembly^ m_pDotNetAssembly = nullptr;
            try
            {
                m_pDotNetAssembly = Assembly::LoadFile(System::Windows::Forms::Application::ExecutablePath->ToLower());
                if (m_pDotNetAssembly == nullptr)
                    return nullptr;
            }
            catch (ArgumentNullException ^ e)
            {
                Debug::WriteLine(L"Hubble InitAppFormTypeDic: " + e->Message);
            }
            catch (ArgumentException ^ e)
            {
                Debug::WriteLine(L"Hubble InitAppFormTypeDic: " + e->Message);
            }
            catch (FileNotFoundException ^ e)
            {
                Debug::WriteLine(L"Hubble InitAppFormTypeDic: " + e->Message);
            }
            catch (FileLoadException ^ e)
            {
                Debug::WriteLine(L"Hubble InitAppFormTypeDic: " + e->Message);
            }
            catch (BadImageFormatException ^ e)
            {
                Debug::WriteLine(L"Hubble InitAppFormTypeDic: " + e->Message);
            }
            finally
            {
                if (m_pDotNetAssembly != nullptr)
                {
                    cli::array<Type^>^ pArray = m_pDotNetAssembly->GetExportedTypes();
                    for each (Type ^ type in pArray)
                    {
                        if (type->IsSubclassOf(Form::typeid))
                        {
                            Form^ m_pObj = nullptr;
                            try
                            {
                                m_pObj = (Form^)Activator::CreateInstance(type);
                                HICON hIcon = (HICON)m_pObj->Icon->Handle.ToPointer();
                                if (hIcon)
                                {
                                    int nPos = filePath.ReverseFind('.');
                                    CString strLib = filePath.Left(nPos).MakeLower();
                                    CString strPath2 = strLib;
                                    nPos = strPath2.ReverseFind('\\');
                                    CString strPath = strPath2.Left(nPos + 1);
                                    CString strNmae = strPath2.Mid(nPos + 1);
                                    strPath2 = strPath;
                                    strPath2 += STRING2BSTR(type->FullName);
                                    strPath2 += _T(" ");
                                    strPath2 += strNmae;
                                    strPath2 += _T(".ico");
                                    //Write Icon to File Stream
                                    System::IO::FileStream^ fs = gcnew System::IO::FileStream(BSTR2STRING(strPath2), System::IO::FileMode::OpenOrCreate);
                                    m_pObj->Icon->Save(fs);
                                    fs->Close();
                                    //System::IO::Stream^ p = nullptr;
                                    //m_pObj->Icon->Save(p);
                                    //if (p)
                                    //{
                                    //	p->Close();
                                    //}
                                    //CString strPath = strLib;
                                    //strPath += STRING2BSTR(type->FullName);
                                    //strPath += _T(".ico");
                                    //theAppProxy.SaveIcon(hIcon, strPath);
                                    delete fs;
                                }
                            }
                            catch (Exception^)
                            {

                            }
                            finally
                            {
                                if (m_pObj && m_pObj->IsMdiContainer)
                                {
                                    if (m_pAppMDIFormTypeDic == nullptr)
                                    {
                                        m_pAppMDIFormTypeDic = gcnew Dictionary<String^, Type^>();
                                    }
                                    m_pAppMDIFormTypeDic[type->FullName] = type;
                                }
                                else
                                {
                                    if (m_pObj && (m_pObj->FormBorderStyle == FormBorderStyle::FixedToolWindow || m_pObj->FormBorderStyle == FormBorderStyle::None || m_pObj->FormBorderStyle == FormBorderStyle::SizableToolWindow))
                                    {
                                    }
                                    else
                                        m_pAppFormTypeDic[type->FullName] = type;
                                }
                                if (m_pObj)
                                    m_pObj->Close();
                            }
                        }
                    }
                }
            }
            if (m_pDotNetAssembly == nullptr)
            {
                return nullptr;
            }
        }
        return m_pAppFormTypeDic;
    }

    void Hubble::ExtractToDirectory(String^ strSRC, String^ strTarget)
    {
        BSTR bstrSRC = STRING2BSTR(strSRC);
        BSTR bstrTarget = STRING2BSTR(strTarget);
        theApp.m_pHubble->HubbleNotify(bstrSRC, bstrTarget, 10000, 0);
        ::SysFreeString(bstrSRC);
        ::SysFreeString(bstrTarget);
    }

    void Hubble::ExportAllCLRObjInfo()
    {
        theAppProxy.ExportCLRObjInfo(_T(""));
        theAppProxy.ExportAllCLRObjInfo(theApp.m_pHubbleImpl->m_strAppFormsPath);
        theAppProxy.ExportAllCLRObjInfo(theApp.m_pHubbleImpl->m_strAppCommonFormsPath);
        theApp.m_pHubbleImpl->ExportComponentInfo();
    }

    String^ Hubble::AppDataPath::get()
    {
        BSTR bstrAppDataPath = theApp.m_pHubbleImpl->m_strAppDataPath.AllocSysString();
        String^ strResult = BSTR2STRING(bstrAppDataPath);
        ::SysFreeString(bstrAppDataPath);
        return strResult;
    }

    String^ Hubble::AppCommonFormsPath::get()
    {
        BSTR bstrAppCommonFormsPath = theApp.m_pHubbleImpl->m_strAppCommonFormsPath.AllocSysString();
        String^ strResult = BSTR2STRING(bstrAppCommonFormsPath);
        ::SysFreeString(bstrAppCommonFormsPath);
        return strResult;
    }

    String^ Hubble::AppFormsPath::get()
    {
        BSTR bstrAppFormsPath = theApp.m_pHubbleImpl->m_strAppFormsPath.AllocSysString();
        String^ strResult = BSTR2STRING(bstrAppFormsPath);
        ::SysFreeString(bstrAppFormsPath);
        return strResult;
    }

    void Hubble::SetControlRelation(Control^ ctrl, String^ strTypes)
    {
        if (m_pControlRelationDic == nullptr)
            m_pControlRelationDic = gcnew Dictionary<Control^, String^>();

        String^ strIDs = nullptr;
        if (m_pControlRelationDic->TryGetValue(ctrl, strIDs) == false)
        {
            m_pControlRelationDic[ctrl] = strTypes;
        }
        else
        {
            CString str1 = strIDs;
            str1 += _T(";");
            str1 += STRING2BSTR(strTypes);
            str1 += _T(";");
            str1 += _T(";") + str1;
            str1.Replace(_T(";;"), _T(";"));
        }
    }

    void Hubble::BindToCtrl(Control^ source, Control^ target, String^ strXmlData)
    {
        if (target != nullptr)
        {
            if (target->Dock == DockStyle::Fill || target->Dock == DockStyle::None)
            {
                CString strXml = strXmlData;

            }
        }
    }

    void Hubble::BindToCtrl(String^ ctrlID, String^ AssemblyQualifiedName, Control^ target, int nAnchor, String^ strXmlData)
    {
        if (target != nullptr)
        {
            if (target->Dock == DockStyle::Fill || target->Dock == DockStyle::None)
            {
                CString strXml = strXmlData;
            }
        }
    }

    Object^ Hubble::ActiveMethod(String^ strObjID, String^ strMethod, cli::array<Object^, 1>^ p)
    {
        Object^ pRetObj = nullptr;
        Hubble^ pApp = Hubble::GetHubble();
        String^ strIndex = strObjID + L"|" + strMethod;
        MethodInfo^ mi = nullptr;
        Object^ pObj = nullptr;
        if (pApp->m_pHubbleCLRMethodDic->TryGetValue(strIndex, mi) == true)
        {
            try
            {
                pRetObj = mi->Invoke(pObj, p);
            }
            finally
            {
            }
            return pRetObj;
        }

        if (pApp->m_pHubbleCLRObjDic->TryGetValue(strObjID, pObj) == false)
        {
            pObj = CreateObject(strObjID);
            pApp->m_pHubbleCLRObjDic[strObjID] = pObj;
        }
        if (pObj != nullptr)
        {
            MethodInfo^ mi = nullptr;
            try
            {
                mi = pObj->GetType()->GetMethod(strMethod);
                pApp->m_pHubbleCLRMethodDic[strIndex] = mi;
            }
            catch (AmbiguousMatchException ^ e)
            {
                Debug::WriteLine(L"Hubble::ActiveMethod GetMethod: " + e->Message);
            }
            catch (ArgumentNullException ^ e)
            {
                Debug::WriteLine(L"Hubble::ActiveMethod GetMethod: " + e->Message);
            }
            finally
            {
                if (mi != nullptr)
                {
                    try
                    {
                        pRetObj = mi->Invoke(pObj, p);
                    }
                    finally
                    {
                    }
                }
            }
        }

        return pRetObj;
    }

    WorkBenchWindow^ Hubble::RemoteActiveWorkBenchWindow(String^ appID)
    {
        BSTR _strID = STRING2BSTR(appID);
        IHubble* pHubble = nullptr;
        theApp.m_pHubble->get_RemoteHubble(_strID, &pHubble);
        ::SysFreeString(_strID);
        if (pHubble)
        {
            IWorkBenchWindow* pTopWnd = nullptr;
            IHubbleExtender* pExtender = nullptr;
            pHubble->get_Extender(&pExtender);
            if (pExtender)
            {
                CComQIPtr<IEclipseExtender> pEclipse(pExtender);
                if (pEclipse)
                    pEclipse->get_ActiveWorkBenchWindow(_strID, &pTopWnd);
                if (pTopWnd)
                {
                    return theAppProxy._createObject<IWorkBenchWindow, WorkBenchWindow>(pTopWnd);
                }
            }
        }
        return nullptr;
    }

    void Hubble::StartApplication(String^ appID, String^ strXml)
    {
        BSTR _strID = STRING2BSTR(appID);
        BSTR _strXml = STRING2BSTR(strXml);
        //if (appID->ToLower() == L"caswebagent.server.1")
        //{
        //    CComPtr<IHubble> pApp;
        //    pApp.CoCreateInstance(_strID, nullptr, CLSCTX_SERVER);
        //    if (pApp)
        //    {
        //        DWORD dwID = ::GetCurrentProcessId();
        //        CString str = _T("");
        //        str.Format(_T("tangramprocess:%d"), dwID);
        //        CComVariant var;
        //        var.vt = VT_DISPATCH;
        //        var.pdispVal = (IDispatch*)theApp.m_pHubble;
        //        pApp->put_AppKeyValue(CComBSTR(str), var);
        //    }
        //}
        //else
        {
            theApp.m_pHubble->StartApplication(_strID, _strXml);
        }
        ::SysFreeString(_strID);
        ::SysFreeString(_strXml);
    }

    Object^ Hubble::ActiveObjectMethod(Object^ pObj, String^ strMethod, cli::array<Object^, 1>^ p)
    {
        Object^ pRetObj = nullptr;

        if (pObj != nullptr)
        {
            MethodInfo^ mi = nullptr;
            try
            {
                mi = pObj->GetType()->GetMethod(strMethod, gcnew cli::array<Type^, 1>(0));
            }
            catch (AmbiguousMatchException ^ e)
            {
                Debug::WriteLine(L"Hubble::ActiveMethod GetMethod: " + e->Message);
            }
            catch (ArgumentNullException ^ e)
            {
                Debug::WriteLine(L"Hubble::ActiveMethod GetMethod: " + e->Message);
            }
            finally
            {
                if (mi != nullptr)
                {
                    try
                    {
                        pRetObj = mi->Invoke(pObj, p);
                    }
                    finally
                    {
                    }
                }
            }
        }

        return pRetObj;
    }

    GalaxyCluster^ Hubble::CreateGalaxyCluster(IntPtr nPageHandle)
    {
        LONGLONG hWnd = (LONGLONG)nPageHandle.ToInt64();
        IGalaxyCluster* pGalaxyCluster = nullptr;
        theApp.m_pHubble->CreateGalaxyCluster(hWnd, &pGalaxyCluster);
        if (pGalaxyCluster)
        {
            GalaxyCluster^ _pGalaxyCluster = theAppProxy._createObject<IGalaxyCluster, GalaxyCluster>(pGalaxyCluster);
            return _pGalaxyCluster;
        }
        return nullptr;
    }

    GalaxyCluster^ Hubble::CreateGalaxyCluster(Control^ ctrl, Object^ ExternalObj)
    {
        if (ctrl != nullptr)
        {
            if (theApp.m_pHubble == nullptr)
                GetHubble();
            LONGLONG hWnd = ctrl->Handle.ToInt64();
            IGalaxyCluster* pGalaxyCluster = nullptr;
            theApp.m_pHubble->CreateGalaxyCluster(hWnd, &pGalaxyCluster);
            if (pGalaxyCluster)
            {
                GalaxyCluster^ _pGalaxyCluster = theAppProxy._createObject<IGalaxyCluster, GalaxyCluster>(pGalaxyCluster);
                if (_pGalaxyCluster != nullptr && ExternalObj != nullptr)
                {
                }
                return _pGalaxyCluster;
            }
        }
        return nullptr;
    }

    Type^ Hubble::GetType(String^ strObjID)
    {
        if (String::IsNullOrEmpty(strObjID) == true)
            return nullptr;

        String^ m_strID = strObjID->ToLower()->Replace(L" ", L"");
        String^ strLib = nullptr;
        if (m_strID != L"")
        {
            Type^ pType = nullptr;
            Hubble^ pApp = Hubble::GetHubble();
            Monitor::Enter(pApp->m_pHubbleCLRTypeDic);
            String^ strID = nullptr;
            if (pApp->m_pHubbleCLRTypeDic->TryGetValue(m_strID, pType) == false)
            {
                BSTR bstrID = STRING2BSTR(m_strID);
                CString _strID = OLE2T(bstrID);
                ::SysFreeString(bstrID);
                int nIndex = _strID.Find(_T(","));
                if (nIndex != -1)
                {
                    CString s1 = _strID.Mid(nIndex + 1);
                    if (s1 == _T("host"))
                    {
                        CString s = _strID.Left(nIndex);
                        _strID = _strID.Left(nIndex + 1) + theApp.m_pHubbleImpl->m_strExeName;
                        _strID.MakeLower();
                    }
                }
                _strID = theApp.GetLibPathFromAssemblyQualifiedName(_strID);
                if (_strID != _T(""))
                {
                    int nPos = _strID.Find(_T("|"));
                    CString strObjName = _strID.Left(nPos);
                    _strID = _strID.Mid(nPos + 1);
                    nPos = _strID.Find(_T("|"));
                    strID = BSTR2STRING(_strID);
                    CString strLibName = _strID.Left(nPos);
                    strLib = BSTR2STRING(_strID.Mid(nPos + 1));
                    Assembly^ m_pDotNetAssembly = nullptr;
                    bool bSystemObj = false;
                    int nIndex = m_strID->IndexOf(L"system.windows.forms");;
                    if (nIndex != -1)
                    {
                        bSystemObj = true;
                        Control^ pObj = gcnew Control();
                        m_pDotNetAssembly = pObj->GetType()->Assembly;
                    }
                    else
                    {
                        try
                        {
                            if (strLibName.CompareNoCase(_T("Cosmos")) == 0)
                                m_pDotNetAssembly = pApp->GetType()->Assembly;
                            else
                            {
                                auto it = theAppProxy.m_mapAssembly.find(strLib);
                                if (it != theAppProxy.m_mapAssembly.end())
                                    m_pDotNetAssembly = it->second;
                                else
                                {
                                    m_pDotNetAssembly = Assembly::LoadFile(strLib);
                                    theAppProxy.m_mapAssembly[strLib] = m_pDotNetAssembly;
                                }
                            }
                        }
                        catch (ArgumentNullException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject: " + e->Message);
                        }
                        catch (ArgumentException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject: " + e->Message);
                        }
                        catch (FileNotFoundException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject: " + e->Message);
                        }
                        catch (FileLoadException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject: " + e->Message);
                        }
                        catch (BadImageFormatException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject: " + e->Message);
                        }
                    }
                    if (m_pDotNetAssembly != nullptr)
                    {
                        try
                        {
                            pType = m_pDotNetAssembly->GetType(BSTR2STRING(strObjName), true, true);
                        }
                        catch (TypeLoadException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject Assembly->GetType: " + e->Message);
                        }
                        catch (ArgumentNullException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject Assembly->GetType: " + e->Message);
                        }
                        catch (ArgumentException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject Assembly->GetType: " + e->Message);
                        }
                        catch (FileNotFoundException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject Assembly->GetType: " + e->Message);
                        }
                        catch (FileLoadException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject Assembly->GetType: " + e->Message);
                        }
                        catch (BadImageFormatException ^ e)
                        {
                            Debug::WriteLine(L"Hubble CreateObject Assembly->GetType: " + e->Message);
                        }
                        finally
                        {
                            if (pType != nullptr)
                            {
                                pApp->m_pHubbleCLRTypeDic->Add(m_strID, pType);
                            }
                        }
                    }
                }
            }
            Monitor::Exit(pApp->m_pHubbleCLRTypeDic);
            if (pType)
            {
                return pType;
            }
        }
        return nullptr;
    }

    Browser^ Hubble::CreateBrowser(IntPtr ParentHandle, String^ strUrls)
    {
        HWND hPWnd = (HWND)ParentHandle.ToPointer();
        if (theApp.m_pHubbleImpl->m_pBrowserFactory)
        {
            strUrls += L"|";
            strUrls = strUrls->Replace(L"||", L"|");
            CString strPath = theApp.m_pHubbleImpl->m_strAppPath;

            strUrls = strUrls->Replace(L"host:", BSTR2STRING(strPath));
            if (ParentHandle == (IntPtr)1)
            {
                hPWnd = theApp.m_pHubbleImpl->m_hChildHostWnd;
            }

            HWND hWnd = theApp.m_pHubbleImpl->m_pBrowserFactory->CreateBrowser(hPWnd, strUrls);
            IBrowser* pBrowser = (IBrowser*)::SendMessage(hWnd, WM_COSMOSMSG, 20190527, 0);
            auto it = theAppProxy.m_mapWebBrowser.find(pBrowser);
            if (it != theAppProxy.m_mapWebBrowser.end())
                return it->second;
            else
                return gcnew Browser(pBrowser);
        }
        return nullptr;
    }

    Browser^ Hubble::CreateBrowserRemote(IntPtr ParentHandle, String^ strUrls)
    {
        Cosmos::Hubble::GetHubble();
        HWND hPWnd = (HWND)ParentHandle.ToPointer();
        if (theApp.m_pHubbleVS == nullptr)
        {
            CComVariant var;
            theApp.m_pHubble->get_AppKeyValue(CComBSTR("vstangramhandle"), &var);
            __int64 nHandle = var.llVal;
            if (nHandle)
            {
                theApp.m_pHubbleVS = (IHubble*)nHandle;
            }
        }
        if (theApp.m_pHubbleVS)
        {
            CComPtr< IBrowser> pBrowser;
            theApp.m_pHubbleVS->CreateBrowser((__int64)ParentHandle.ToPointer(), STRING2BSTR(strUrls), &pBrowser);
            auto it = theAppProxy.m_mapWebBrowser.find(pBrowser);
            if (it != theAppProxy.m_mapWebBrowser.end())
                return it->second;
            else
                return gcnew Browser(pBrowser);
        }
        return nullptr;
    }

    Form^ Hubble::CreateForm(IWin32Window^ parent, String^ strObjID)
    {
        Object^ m_pObj = nullptr;
        int nIndex = strObjID->IndexOf(L"<");
        if (nIndex == 0)
        {
            CTangramXmlParse m_Parse;
            CString strXml = strObjID;
            if (m_Parse.LoadXml(strXml) || m_Parse.LoadFile(strXml))
            {
                CString strType = m_Parse.attr(_T("type"), _T(""));
                if (strType.CompareNoCase(_T("winform")) == 0)
                {
                    CString strID = m_Parse.attr(_T("objid"), _T(""));
                    CString strName = m_Parse.name();
                    if (strID != _T(""))
                    {
                        CTangramXmlParse* pChild3 = m_Parse.GetChild(_T("mdichild"));
                        if (pChild3)
                        {
                            CTangramXmlParse* pChild4 = m_Parse.GetChild(_T("mdiclient"));
                            int nCount = pChild3->GetCount();
                            if (nCount && pChild4)
                            {
                                CMDIChildFormInfo* pInfo = new CMDIChildFormInfo();
                                theApp.m_pHubbleImpl->m_pCurMDIChildFormInfo = pInfo;
                                for (int i = 0; i < nCount; i++)
                                {
                                    CString strName = pChild3->GetChild(i)->name().MakeLower();
                                    if (pChild4->GetChild(strName))
                                        pInfo->m_mapFormsInfo[strName] = pChild3->GetChild(i)->xml();
                                }
                            }
                        }
                        Object^ pObj = CreateObject(BSTR2STRING(strID));
                        if (pObj != nullptr)
                        {
                            if (pObj->GetType()->IsSubclassOf(Form::typeid))
                            {
                                Form^ thisForm = (Form^)pObj;
                                thisForm->Show(parent);
                            }
                            return (Form^)pObj;
                        }
                    }
                }
            }
            return nullptr;
        }
        return nullptr;
    }

    Cosmos::Wormhole^ Cosmos::Grid::Wormhole::get()
    {
        if (m_pGrid)
        {
            if (m_pSession == nullptr)
            {
                CSession* pSession = theApp.m_pHubbleImpl->GetCloudSession(m_pGrid);
                if (pSession)
                {
                    m_pSession = gcnew Cosmos::Wormhole(pSession);
                    theAppProxy.m_mapSession2Wormhole[pSession] = m_pSession;
                }
            }
        }
        return m_pSession;
    }

    String^ Hubble::GetUIData(Control^ ctrl)
    {
        HWND hWnd = (HWND)ctrl->Handle.ToPointer();
        auto it = theApp.m_pHubbleImpl->m_mapUIData.find(hWnd);
        if (it != theApp.m_pHubbleImpl->m_mapUIData.end())
            return BSTR2STRING(it->second);
        return L"";
    }

    String^ Hubble::GetCtrlTag(Control^ ctrl)
    {
        HWND hWnd = (HWND)ctrl->Handle.ToPointer();
        auto it = theApp.m_pHubbleImpl->m_mapCtrlTag.find(hWnd);
        if (it != theApp.m_pHubbleImpl->m_mapCtrlTag.end())
            return BSTR2STRING(it->second);
        return L"";
    }

    Object^ Hubble::CreateObject(String^ strObjID)
    {
        Object^ m_pObj = nullptr;
        int nIndex = strObjID->IndexOf(L"<");
        if (nIndex == 0)
        {
            CTangramXmlParse m_Parse;
            CString strXml = strObjID;
            if (m_Parse.LoadXml(strXml) || m_Parse.LoadFile(strXml))
            {
                CString strType = m_Parse.attr(_T("type"), _T(""));
                if (strType.CompareNoCase(_T("winform")) == 0)
                {
                    CString strID = m_Parse.attr(_T("objid"), _T(""));
                    CString strName = m_Parse.name();
                    if (strID != _T(""))
                    {
                        CTangramXmlParse* pChild3 = m_Parse.GetChild(_T("mdichild"));
                        if (pChild3)
                        {
                            CTangramXmlParse* pChild4 = m_Parse.GetChild(_T("mdiclient"));
                            int nCount = pChild3->GetCount();
                            if (nCount && pChild4)
                            {
                                CMDIChildFormInfo* pInfo = new CMDIChildFormInfo();
                                theApp.m_pHubbleImpl->m_pCurMDIChildFormInfo = pInfo;
                                for (int i = 0; i < nCount; i++)
                                {
                                    CString strName = pChild3->GetChild(i)->name().MakeLower();
                                    if (pChild4->GetChild(strName))
                                        pInfo->m_mapFormsInfo[strName] = pChild3->GetChild(i)->xml();
                                }
                            }
                        }
                        Object^ pObj = CreateObject(BSTR2STRING(strID));
                        if (pObj != nullptr)
                        {
                            if (pObj->GetType()->IsSubclassOf(Form::typeid))
                            {
                                CString strCaption = m_Parse.attr(_T("caption"), _T(""));
                                Form^ thisForm = (Form^)pObj;
                                if (m_Parse.attrBool(_T("mainwindow")))
                                    theApp.m_pHubbleImpl->m_hMainWnd = (HWND)thisForm->Handle.ToPointer();
                                if (strCaption != _T(""))
                                    thisForm->Text = BSTR2STRING(strCaption);
                                if (thisForm->IsMdiContainer)
                                {
                                    CString strBKPage = m_Parse.attr(_T("mdibkpageid"), _T(""));
                                    if (strBKPage != _T(""))
                                    {
                                        Hubble::CreateBKPage(thisForm, BSTR2STRING(strBKPage));
                                    }
                                }
                                thisForm->Show();
                            }
                            return pObj;
                        }
                    }
                }
            }
            return nullptr;
        }
        Type^ pType = GetType(strObjID);
        if (pType)
        {
            try
            {
                m_pObj = (Object^)Activator::CreateInstance(pType);
            }
            catch (TypeLoadException ^ e)
            {
                Debug::WriteLine(L"Hubble CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (ArgumentNullException ^ e)
            {
                Debug::WriteLine(L"Hubble CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (ArgumentException ^ e)
            {
                Debug::WriteLine(L"Hubble CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (NotSupportedException ^ e)
            {
                Debug::WriteLine(L"Hubble CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (TargetInvocationException ^ e)
            {
                Debug::WriteLine(L"Hubble CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (MethodAccessException ^ e)
            {
                Debug::WriteLine(L"Hubble CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (InvalidComObjectException ^ e)
            {
                Debug::WriteLine(L"Hubble CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (MissingMethodException ^ e)
            {
                Debug::WriteLine(L"Hubble CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (COMException ^ e)
            {
                Debug::WriteLine(L"Hubble CreateObject Activator::CreateInstance: " + e->Message);
            }
        }

        return m_pObj;
    }

    Grid^ Grid::Observe(String^ layerName, String^ layerXML)
    {
        if (m_pGrid)
        {
            BSTR blayerName = STRING2BSTR(layerName);
            BSTR blayerXML = STRING2BSTR(layerXML);
            IGrid* pGrid = nullptr;
            m_pGrid->Observe(blayerName, blayerXML, &pGrid);
            ::SysFreeString(blayerName);
            ::SysFreeString(blayerXML);
            if (pGrid)
            {
                return theAppProxy._createObject<IGrid, Grid>(pGrid);
            }
        }
        return nullptr;
    }

    Grid^ Grid::ObserveChild(int rowNum, int colNum, String^ layerName, String^ layerXML)
    {
        if (m_pGrid)
        {
            BSTR blayerName = STRING2BSTR(layerName);
            BSTR blayerXML = STRING2BSTR(layerXML);
            IGrid* pGrid = nullptr;
            m_pGrid->ObserveEx(rowNum, colNum, blayerName, blayerXML, &pGrid);
            ::SysFreeString(blayerName);
            ::SysFreeString(blayerXML);
            if (pGrid)
            {
                return theAppProxy._createObject<IGrid, Grid>(pGrid);
            }
        }
        return nullptr;
    }

    System::Void Grid::LoadNode(TreeNode^ pGrid, CTangramXmlParse* pParse)
    {
        if (pParse)
        {
            int nCount = pParse->GetCount();
            for (int i = 0; i < nCount; i++)
            {
                CTangramXmlParse* _pParse = pParse->GetChild(i);
                if (_pParse)
                {
                    CString name = _pParse->name();
                    if (name == _T("tangramxml"))
                    {
                        HWND hWnd = (HWND)m_pBindTreeView->Handle.ToPointer();

                        IGalaxy* pGalaxy = nullptr;
                        IGrid* _pGrid = nullptr;
                        m_pGrid->get_Galaxy(&pGalaxy);
                        CComBSTR bstrName("");
                        pGalaxy->get_Name(&bstrName);
                        String^ name = pGrid->Name + L".";
                        name += m_pBindTreeView->Name;
                        name += L"." + BSTR2STRING(bstrName);
                        BSTR strName = STRING2BSTR(name->ToLower());
                        Cosmos::Galaxy^ _pGalaxy = GalaxyCluster->CreateGalaxy(m_pBindTreeView->Handle, name);
                        pGalaxy->Observe(L"default", CComBSTR(_pParse->xml()), &_pGrid);
                        ::SysFreeString(strName);
                    }
                    else
                    {
                        bool isTreeNode = _pParse->attrBool(_T("treenode"), false);
                        if (isTreeNode)
                        {
                            TreeNode^ pChildNode = pGrid->Nodes->Add(BSTR2STRING(_pParse->text()));
                            if (pChildNode)
                            {
                                CString strTagName = name + _T("_tag");
                                CTangramXmlParse* pChild2 = pParse->GetChild(strTagName);
                                if (pChild2)
                                {
                                    pChildNode->Tag = BSTR2STRING(pChild2->xml());
                                }
                                LoadNode(pChildNode, pParse->GetChild(i));
                            }
                        }
                    }
                }
            }
        }
    }

    void Grid::NavigateTreeInit()
    {
        if (m_pBindTreeView != nullptr)
        {
            m_pBindTreeView->NodeMouseDoubleClick += gcnew System::Windows::Forms::TreeNodeMouseClickEventHandler(this, &Cosmos::Grid::OnNodeMouseDoubleClick);
            m_pBindTreeView->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &Cosmos::Grid::OnAfterSelect);

            if (String::IsNullOrEmpty(m_strTreeViewData) == true)
                m_strTreeViewData = Cosmos::Hubble::m_strWizData;
            if (String::IsNullOrEmpty(m_strTreeViewData) == false)
            {
                BSTR bstrXml = STRING2BSTR(m_strTreeViewData);
                CString strXml = OLE2T(bstrXml);
                CTangramXmlParse m_Parse;
                if (m_Parse.LoadXml(strXml) || m_Parse.LoadFile(strXml))
                {
                    if (this->m_pBindTreeView->Nodes->Count == 0)
                    {
                        LoadNode(m_pBindTreeView->Nodes->Add(BSTR2STRING(m_Parse.text())), &m_Parse);
                    }
                }
                ::SysFreeString(bstrXml);
                m_pBindTreeView->ExpandAll();
            }
        }
    }

    void Grid::OnNodeMouseDoubleClick(System::Object^ sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^ e)
    {
        if (e->Node->Tag)
        {
            String^ strTag = e->Node->Tag->ToString();

            {
                Grid^ pHostNode = nullptr;// hostnode->HostNode;
                if (bindTreeNode == nullptr)
                {
                    String^ name = Attribute[L"bindnode"];
                    if (String::IsNullOrEmpty(name) == false)
                    {
                        GridCollection^ pCol = nullptr;
                        RootGrid->GetGrids(name, bindTreeNode, pCol);
                        if (pCol)
                        {
                            delete pCol;
                        }
                    }
                }
                if (bindTreeNode != nullptr)
                    pHostNode = bindTreeNode;
                if (pHostNode == nullptr)
                {
                    pHostNode = HostGrid;
                }
                if (pHostNode)
                {
                    CString strXml = strTag;
                    CTangramXmlParse m_Parse;
                    if (m_Parse.LoadXml(strXml))
                    {
                        CString strAction = m_Parse.attr(_T("action"), _T(""));
                        if (strAction != _T(""))
                        {
                            CTangramXmlParse* pChild = m_Parse.GetChild(strAction);
                            if (pChild)
                            {
                                if (strAction == _T("doubleclick"))
                                    pHostNode->Observe(BSTR2STRING(m_Parse.name()), BSTR2STRING(pChild->xml()));
                                else if (strAction == _T("openurl"))
                                {
                                    Cosmos::Hubble::CreateBrowser(System::IntPtr::Zero, BSTR2STRING(pChild->attr(_T("url"), _T("")) + _T("|")));
                                }
                            }

                        }

                    }
                }
            }
        }
    }

    void Grid::OnAfterSelect(System::Object^ sender, System::Windows::Forms::TreeViewEventArgs^ e)
    {
        if (e->Node->Tag)
        {
            String^ strTag = e->Node->Tag->ToString();

            {
                Grid^ pHostNode = nullptr;
                if (bindTreeNode == nullptr)
                {
                    String^ name = Attribute[L"bindnode"];
                    if (String::IsNullOrEmpty(name) == false)
                    {
                        GridCollection^ pCol = nullptr;
                        RootGrid->GetGrids(name, bindTreeNode, pCol);
                        if (pCol)
                            delete pCol;
                    }
                }
                if (bindTreeNode != nullptr)
                    pHostNode = bindTreeNode;
                //Grid^ 
                if (pHostNode == nullptr)
                {
                    pHostNode = HostGrid;
                }
                if (pHostNode)
                {
                    CString strXml = strTag;
                    CTangramXmlParse m_Parse;
                    if (m_Parse.LoadXml(strXml))
                    {
                        CTangramXmlParse* pChild = m_Parse.GetChild(_T("afterselected"));
                        if (pChild)
                            pHostNode->Observe(BSTR2STRING(m_Parse.name()), BSTR2STRING(pChild->xml()));
                    }
                }
            }
        }
    }

    Object^ Grid::ActiveMethod(String^ strMethod, cli::array<Object^, 1>^ p)
    {
        Object^ pRetObj = nullptr;
        if (m_pHostObj != nullptr)
        {
            MethodInfo^ mi = nullptr;
            if (m_pHubbleCLRMethodDic == nullptr)
                m_pHubbleCLRMethodDic = gcnew Dictionary<String^, MethodInfo^>();
            Object^ pObj = nullptr;
            if (m_pHubbleCLRMethodDic->TryGetValue(strMethod, mi) == true)
            {
                try
                {
                    pRetObj = mi->Invoke(m_pHostObj, p);
                }
                finally
                {
                }
                return pRetObj;
            }
            try
            {
                mi = m_pHostObj->GetType()->GetMethod(strMethod);
                m_pHubbleCLRMethodDic[strMethod] = mi;
            }
            catch (AmbiguousMatchException ^ e)
            {
                Debug::WriteLine(L"Hubble::ActiveMethod GetMethod: " + e->Message);
            }
            catch (ArgumentNullException ^ e)
            {
                Debug::WriteLine(L"Hubble::ActiveMethod GetMethod: " + e->Message);
            }
            finally
            {
                if (mi != nullptr)
                {
                    try
                    {
                        pRetObj = mi->Invoke(m_pHostObj, p);
                    }
                    finally
                    {
                    }
                }
            }
        }

        return pRetObj;
    }

    GalaxyCluster::GalaxyCluster(void)
    {
    }

    GalaxyCluster::GalaxyCluster(IGalaxyCluster* pGalaxyCluster)
    {
        m_pGalaxyCluster = pGalaxyCluster;
        LONGLONG nValue = (LONGLONG)m_pGalaxyCluster;
        theAppProxy._insertObject(nValue, this);
        m_pHubbleClrEvent = new CGalaxyClusterEvent();
        m_pHubbleClrEvent->DispEventAdvise(m_pGalaxyCluster);
        m_pHubbleClrEvent->m_pGalaxyCluster = this;
    }

    GalaxyCluster::~GalaxyCluster()
    {
        if (m_pHubbleClrEvent)
        {
            m_pHubbleClrEvent->DispEventUnadvise(m_pGalaxyCluster);
            LONGLONG nValue = (LONGLONG)m_pGalaxyCluster;
            theAppProxy._removeObject(nValue);
            delete m_pHubbleClrEvent;
            m_pHubbleClrEvent = nullptr;
        }
    }

    void GalaxyCluster::ObserveGalaxys(String^ strFrames, String^ strKey, String^ bstrXml, bool bSaveToConfigFile)
    {
        m_pGalaxyCluster->ObserveGalaxys(STRING2BSTR(strFrames), STRING2BSTR(strKey), STRING2BSTR(bstrXml), bSaveToConfigFile);
    }

    Grid^ GalaxyCluster::GetGrid(String^ strName, String^ strGalaxyName)
    {
        if (String::IsNullOrEmpty(strName) || String::IsNullOrEmpty(strGalaxyName))
            return nullptr;
        BSTR bstrXml = STRING2BSTR(strName);
        BSTR bstrGalaxyName = STRING2BSTR(strGalaxyName);
        CComPtr<IGrid> pGrid;
        m_pGalaxyCluster->GetGrid(bstrXml, bstrGalaxyName, &pGrid);
        Grid^ pRetNode = nullptr;
        if (pGrid)
        {
            pRetNode = theAppProxy._createObject<IGrid, Grid>(pGrid);
        }
        ::SysFreeString(bstrXml);
        ::SysFreeString(bstrGalaxyName);
        return pRetNode;
    }

    String^ GalaxyCluster::GetPageXML()
    {
        BSTR bstrXML;
        m_pGalaxyCluster->get_GalaxyClusterXML(&bstrXML);
        String^ strXML = BSTR2STRING(bstrXML);
        return strXML;
    }

    void Galaxy::SendMessage(String^ strFrom, String^ strTo, String^ strMsgId, String^ strMsgContent, String^ strExtra)
    {
        if (m_pGalaxy)
        {
            __int64 nHandle;
            m_pGalaxy->get_HWND(&nHandle);
            HWND hWnd = (HWND)nHandle;
            IPCMsg msg;
            msg.m_strId = L"MESSAGE";
            msg.m_strParam1 = strFrom;
            msg.m_strParam2 = strTo;
            msg.m_strParam3 = strMsgId;
            msg.m_strParam4 = strMsgContent;
            msg.m_strParam5 = strExtra;
            theApp.m_pHubbleImpl->m_pCurrentIPCMsg = &msg;
            ::SendMessage(hWnd, WM_HUBBLE_DATA, (WPARAM)&msg, 20200203);
        }
        theApp.m_pHubbleImpl->m_pCurrentIPCMsg = nullptr;
    }

    Grid^ Galaxy::Observe(String^ layerName, String^ layerXML)
    {
        Cosmos::Grid^ pRetNode = nullptr;
        BSTR blayerName = STRING2BSTR(layerName);
        BSTR blayerXML = STRING2BSTR(layerXML);
        CComPtr<IGrid> pGrid;
        m_pGalaxy->Observe(blayerName, blayerXML, &pGrid);
        if (pGrid)
        {
            pRetNode = theAppProxy._createObject<IGrid, Cosmos::Grid>(pGrid);
            Cosmos::Grid^ pRetNode2 = nullptr;
            if (!TryGetValue(layerName, pRetNode2))
            {
                Add(layerName, pRetNode);
            }
        }
        ::SysFreeString(blayerName);
        ::SysFreeString(blayerXML);
        return pRetNode;
    }

    Object^ Galaxy::FrameData::get(String^ iIndex)
    {
        CComVariant bstrVal(::SysAllocString(L""));
        m_pGalaxy->get_GalaxyData(STRING2BSTR(iIndex), &bstrVal);
        return Marshal::GetObjectForNativeVariant((IntPtr)&bstrVal);
    }

    void Galaxy::FrameData::set(String^ iIndex, Object^ newVal)
    {
        IntPtr nPtr = (IntPtr)0;
        Marshal::GetNativeVariantForObject(newVal, nPtr);
        m_pGalaxy->put_GalaxyData(STRING2BSTR(iIndex), *(VARIANT*)nPtr.ToInt64());
    }
}

void Cosmos::Hubble::OnHubble_DebugDelegate(System::String^ strInfo)
{
    if (String::IsNullOrEmpty(strInfo) == false)
    {
        strInfo->Replace("ShowWebPage:", "");
        Hubble::CreateBrowser(IntPtr::Zero, strInfo->Replace(L"ShowWebPage:", L""));
    }
}

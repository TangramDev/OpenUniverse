/********************************************************************************
*					Open Universe - version 0.2.0								*
*********************************************************************************
* Copyright (C) 2002-2020 by Tangram Team.   All Rights Reserved.				*
*
* This SOURCE CODE is governed by a BSD - style license that can be
* found in the LICENSE file.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com
* https://www.tangram.dev
********************************************************************************/

#include "StdAfx.h"
#include "dllmain.h"
#include "StarCLREvent.h"
#include "Cosmos.h"
#include "ChromeWebBrowser.h"

using namespace System::Threading;
using namespace System::Diagnostics;
using namespace System::Reflection;
using namespace System::Runtime::InteropServices;

namespace Cosmos
{
    Star::Star(IStar* pNode)
    {
        m_hWnd = NULL;
        m_pStarEvent = new CCosmosNodeEvent();
        m_pStarEvent->m_pStar = pNode;
        m_pStarEvent->m_pStarCLREvent = new CStarCLREvent();
        m_pStarCLREvent = m_pStarEvent->m_pStarCLREvent;
        m_pStarEvent->m_pStarCLREvent->m_pStar = this;
        HRESULT hr = m_pStarEvent->DispEventAdvise(pNode);
        if (theApp.m_pHubbleImpl)
            theApp.m_pHubbleImpl->AttachNode(m_pStarEvent);
        m_pStar = pNode;
        LONGLONG nValue = (LONGLONG)pNode;
        theAppProxy._insertObject(nValue, this);
        m_pChromeBrowserProxy = nullptr;
    }

    Star::~Star()
    {
        delete m_pStarCLREvent;
        m_pStar = NULL;
    }

    GalaxyCluster^ Star::GalaxyCluster::get()
    {
        IGalaxyCluster* pGalaxyCluster = nullptr;
        m_pStar->get_GalaxyCluster(&pGalaxyCluster);

        if (pGalaxyCluster)
        {
            return theAppProxy._createObject<IGalaxyCluster, Cosmos::GalaxyCluster>(pGalaxyCluster);
        }
        return nullptr;
    }

    Quasar^ Star::Quasar::get()
    {
        CComPtr<IQuasar> pTangramFrame;
        m_pStar->get_Quasar(&pTangramFrame);

        Cosmos::Quasar^ pQuasar = theAppProxy._createObject<IQuasar, Cosmos::Quasar>(pTangramFrame);
        return pQuasar;
    }

    String^ Star::Caption::get()
    {
        if (m_pStar)
        {
            CComBSTR bstrCap("");
            m_pStar->get_Caption(&bstrCap);
            String^ strCap = Marshal::PtrToStringUni((System::IntPtr)LPTSTR(LPCTSTR(bstrCap)));
            return strCap;
        }
        return "";
    }

    String^ Star::URL::get()
    {
        if (m_pStar)
        {
            ::StarType nType;
            m_pStar->get_StarType(&nType);
            CComBSTR bstrCap("");
            switch (nType)
            {
            case BlankView:
            {
                m_pStar->get_URL(&bstrCap);
                return BSTR2STRING(bstrCap);
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
        return "";
    }

    void Star::URL::set(String^ newVal)
    {
        if (m_pStar)
        {
            ::StarType nType;
            m_pStar->get_StarType(&nType);
            switch (nType)
            {
            case BlankView:
                if (nType == BlankView)
                {
                    m_pStar->put_URL(STRING2BSTR(newVal));

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

    void Star::Init()
    {
        LONGLONG h = 0;
        if (m_pStar)
        {
            m_pStar->get_Handle(&h);
            ::SendMessage((HWND)h, WM_COSMOSMSG, 1, 0);
        }
    }

    Object^ Star::PlugIn::get(String^ strObjName)
    {
        Object^ pObj = nullptr;
        if (m_pStar)
        {
            Star^ pRootNode = this->RootStar;
            if (pRootNode->m_pPlugInDic == nullptr)
            {
                pRootNode->m_pPlugInDic = gcnew Dictionary<String^, Object^>();
            }
            if (pRootNode->m_pPlugInDic->TryGetValue(strObjName, pObj) == false)
            {
                IDispatch* pDisp = nullptr;
                LRESULT hr = m_pStar->get_AxPlugIn(STRING2BSTR(strObjName), &pDisp);
                if (SUCCEEDED(hr) && pDisp)
                {
                    Object^ pObj = reinterpret_cast<Object^>(Marshal::GetObjectForIUnknown((System::IntPtr)(pDisp)));
                    pRootNode->m_pPlugInDic[strObjName] = pObj;
                    return pObj;
                }

            }
        }
        return pObj;
    }

    void Star::Fire_OnTabChange(int nActivePage, int nOldActivePage)
    {
        OnTabChange(nActivePage, nOldActivePage);
        Star^ pActiveStar = GetStar(0, nActivePage);
        Star^ pOldStar = GetStar(0, nOldActivePage);
    }

    Hubble::Hubble()
    {
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
                GetHubbleImpl _pTangramImplFunction;
                _pTangramImplFunction = (GetHubbleImpl)GetProcAddress(hModule, "GetHubbleImpl");
                if (_pTangramImplFunction != NULL) {
                    theApp.m_pHubbleImpl = _pTangramImplFunction(&theApp.m_pHubble);
                    if (theApp.m_pHubbleImpl->m_nAppType == 0)
                        theApp.m_pHubbleImpl->m_nAppType = APP_BROWSERAPP;
                    theApp.m_pHubbleImpl->m_pHubbleDelegate = (IHubbleDelegate*)&theApp;
                    theApp.m_pHubbleImpl->m_pUniverseAppProxy = (IHubbleAppProxy*)&theApp;
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
            break;
        case CosmosAppType::APPBROWSERAPP:
            break;
        }
        return m_pHubble;
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
        if (IsChromeRunning)
            return true;

        HMODULE hModule = ::GetModuleHandle(L"chrome_rt.dll");
        if (hModule == nullptr)
            hModule = ::LoadLibrary(L"chrome_rt.dll");
        if (hModule) {
            typedef bool(__stdcall* _IsChromeRunning)(bool bSupportCrashReporting);
            _IsChromeRunning FuncIsChromeRunning;
            FuncIsChromeRunning = (_IsChromeRunning)GetProcAddress(hModule, "IsBrowserModel");
            if (FuncIsChromeRunning != NULL) {
                IsChromeRunning = FuncIsChromeRunning(false);
                if (theAppProxy.m_bInitApp == false)
                {
                    ::PostAppMessage(::GetCurrentThreadId(), WM_COSMOSMSG, 0, 20191022);
                }
            }
        }
        return IsChromeRunning;
    }

    Browser^ Hubble::HostWebBrowser::get()
    {
        if (theApp.m_pHubble)
        {
            IBrowser* pWebBrowser = nullptr;
            theApp.m_pHubble->get_HostChromeBrowserWnd(&pWebBrowser);
            if (pWebBrowser)
            {
                auto it = theAppProxy.m_mapChromeWebBrowser.find(pWebBrowser);
                if (it != theAppProxy.m_mapChromeWebBrowser.end())
                    return it->second;
            }
        }
        return nullptr;
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

    Star^ Hubble::CreatingStar::get()
    {
        Object^ pRetObject = nullptr;
        if (theApp.m_pHubble)
        {
            IStar* pNode = nullptr;
            theApp.m_pHubble->get_CreatingStar(&pNode);
            if (pNode)
                return theAppProxy._createObject<IStar, Star>(pNode);
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
                    pParse = pParse->GetChild(TGM_NODE);
                    if (pParse)
                    {
                        theApp.m_pHubbleImpl->m_strNtpXml = _strXml;
                    }
                }
            }
        }
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
        intptr_t nNode = eventSession->m_pWormhole->Getint64(L"nodeobj");
        if (nNode)
        {
            IStar* pNode = (IStar*)nNode;
            Star^ thisNode = theAppProxy._createObject<IStar, Star>(pNode);
            if (thisNode != nullptr)
            {
                thisNode->Fire_OnBindCLRObjToWebPage(SourceObj, eventSession, eventName);
            }
        }
        OnBindCLRObjToWebPage(SourceObj, eventSession, eventName);
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

    Star^ Hubble::ExtendMDIClient(Form^ pForm, String^ strKey, String^ strXml)
    {
        Control^ mdiclient = Hubble::GetMDIClient(pForm);
        if (mdiclient)
        {
            GalaxyCluster^ page = Hubble::CreateGalaxyCluster(pForm, nullptr);
            Quasar^ frame = page->CreateQuasar(mdiclient, L"default");
            if (frame)
            {
                return frame->Observe(strKey, strXml);
            }
        }
        return nullptr;
    }

    Browser^ Hubble::ActiveBrowser()
    {
        IBrowser* pChromeWebBrowser = nullptr;
        theApp.m_pHubble->get_ActiveChromeBrowserWnd(&pChromeWebBrowser);
        if (pChromeWebBrowser)
        {
            auto it = theAppProxy.m_mapChromeWebBrowser.find(pChromeWebBrowser);
            if (it != theAppProxy.m_mapChromeWebBrowser.end())
                return it->second;
            else
            {
                Browser^ pBrowser = gcnew Browser(pChromeWebBrowser);
                theAppProxy.m_mapChromeWebBrowser[pChromeWebBrowser] = pBrowser;
                return pBrowser;
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
        IStar* pWndNode = nullptr;
        HRESULT hr = theApp.m_pHubble->GetStarFromHandle((LONGLONG)hWnd, &pWndNode);
        if (hr != S_OK || pWndNode == nullptr)
        {
            return nullptr;
        }
        IQuasar* pQuasar = nullptr;
        hr = pWndNode->get_Quasar(&pQuasar);
        if (hr != S_OK || pQuasar == nullptr)
        {
            return nullptr;
        }
        IBrowser* pChromeWebBrowser = nullptr;
        pQuasar->get_HostBrowser(&pChromeWebBrowser);
        if (pChromeWebBrowser == nullptr)
        {
            return nullptr;
        }
        auto it = theAppProxy.m_mapChromeWebBrowser.find(pChromeWebBrowser);
        if (it != theAppProxy.m_mapChromeWebBrowser.end())
            return it->second;
        else
        {
            Browser^ pBrowser = gcnew Browser(pChromeWebBrowser);
            theAppProxy.m_mapChromeWebBrowser[pChromeWebBrowser] = pBrowser;
            return pBrowser;
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
                theApp.m_pHubbleImpl->m_pCosmosAppProxy = static_cast<IHubbleAppProxy*>(&theApp);
                //20180817
                if (theApp.m_pHubble == nullptr)
                {
                    HMODULE hModule = ::GetModuleHandle(_T("universe.dll"));
                    if (hModule) {
                        typedef IHubble* (__stdcall* GetTangram)();
                        GetTangram _pTangramFunction;
                        _pTangramFunction = (GetTangram)GetProcAddress(hModule, "GetTangram");
                        if (_pTangramFunction != NULL) {
                            theApp.m_pHubble = _pTangramFunction();
                        }
                    }
                }
            }
        }
        return 0;
    };

    Star^ Hubble::GetStarFromHandle(IntPtr handle)
    {
        IStar* pWndNode = nullptr;
        HRESULT hr = theApp.m_pHubble->GetStarFromHandle((LONGLONG)handle.ToPointer(), &pWndNode);
        if (hr != S_OK || pWndNode == nullptr)
        {
            return nullptr;
        }
        return theAppProxy._createObject<IStar, Star>(pWndNode);
    }

    Star^ Hubble::GetNodeFromControl(Control^ ctrl)
    {
        if (ctrl == nullptr)
        {
            return nullptr;
        }
        IStar* pWndNode = nullptr;
        HRESULT hr = theApp.m_pHubble->GetStarFromHandle((LONGLONG)ctrl->Handle.ToPointer(), &pWndNode);
        if (hr != S_OK || pWndNode == nullptr)
        {
            return nullptr;
        }
        return theAppProxy._createObject<IStar, Star>(pWndNode);
    }

    void Hubble::UpdateNewTabPageLayout(String^ newTabPageLayout)
    {
        Hubble::NTPXml::set(newTabPageLayout);
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
        
        Wormhole^ pCloudSession = nullptr;
        CSession* pSession = nullptr;
        bool bExists = Hubble::WebBindEventDic->TryGetValue(pObj, pCloudSession);
        if (bExists)
        {
            pSession = pCloudSession->m_pWormhole;
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

    String^ Hubble::AppDataPath::get()
    {
        BSTR bstrAppDataPath = theApp.m_pHubbleImpl->m_strAppDataPath.AllocSysString();
        String^ strResult = BSTR2STRING(bstrAppDataPath);
        ::SysFreeString(bstrAppDataPath);
        return strResult;
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
        IGalaxyCluster* pGalaxyCluster = nullptr;
        theApp.m_pHubble->CreateGalaxyCluster(nPageHandle.ToInt64(), &pGalaxyCluster);
        if (pGalaxyCluster)
        {
            return theAppProxy._createObject<IGalaxyCluster, GalaxyCluster>(pGalaxyCluster);
        }
        return nullptr;
    }

    GalaxyCluster^ Hubble::CreateGalaxyCluster(Control^ ctrl, Object^ ExternalObj)
    {
        if (ctrl != nullptr)
        {
            if (theApp.m_pHubble == nullptr)
                GetHubble();
            IGalaxyCluster* pGalaxyCluster = nullptr;
            theApp.m_pHubble->CreateGalaxyCluster(ctrl->Handle.ToInt64(), &pGalaxyCluster);
            if (pGalaxyCluster)
            {
                return theAppProxy._createObject<IGalaxyCluster, GalaxyCluster>(pGalaxyCluster);
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
        Type^ pType = nullptr;
        if (m_strID != L"")
        {
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
        }
        return pType;
    }

    Browser^ Hubble::CreateBrowser(IntPtr ParentHandle, String^ strUrls)
    {
        HWND hPWnd = (HWND)ParentHandle.ToPointer();
        if (theApp.m_pHubbleImpl->m_pBrowserFactory)
        {
            strUrls += L"|";
            strUrls = strUrls->Replace(L"||", L"|");
            HWND hWnd = theApp.m_pHubbleImpl->m_pBrowserFactory->CreateBrowser(hPWnd, strUrls);
            IBrowser* pBrowser = (IBrowser*)::SendMessage(hWnd, WM_COSMOSMSG, 20190527, 0);
            auto it = theAppProxy.m_mapChromeWebBrowser.find(pBrowser);
            if (it != theAppProxy.m_mapChromeWebBrowser.end())
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

    Wormhole^ Star::Wormhole::get()
    {
        if (m_pStar)
        {
            __int64 nHandle = 0;
            m_pStar->get_Handle(&nHandle);
            if (m_pWormhole == nullptr)
            {
                CSession* pSession = theApp.m_pHubbleImpl->GetCloudSession(m_pStar);
                if (pSession)
                {
                    m_pWormhole = gcnew Cosmos::Wormhole(pSession);
                    theAppProxy.m_mapSession2Wormhole[pSession] = m_pWormhole;
                }
            }
        }
        return m_pWormhole;
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

    Star^ Star::Observe(String^ layerName, String^ layerXML)
    {
        if (m_pStar)
        {
            BSTR blayerName = STRING2BSTR(layerName);
            BSTR blayerXML = STRING2BSTR(layerXML);
            IStar* pNode = nullptr;
            m_pStar->Observe(blayerName, blayerXML, &pNode);
            ::SysFreeString(blayerName);
            ::SysFreeString(blayerXML);
            if (pNode)
            {
                return theAppProxy._createObject<IStar, Star>(pNode);
            }
        }
        return nullptr;
    }

    Star^ Star::ObserveChild(int rowNum, int colNum, String^ layerName, String^ layerXML)
    {
        if (m_pStar)
        {
            BSTR blayerName = STRING2BSTR(layerName);
            BSTR blayerXML = STRING2BSTR(layerXML);
            IStar* pNode = nullptr;
            m_pStar->ObserveEx(rowNum, colNum, blayerName, blayerXML, &pNode);
            ::SysFreeString(blayerName);
            ::SysFreeString(blayerXML);
            if (pNode)
            {
                return theAppProxy._createObject<IStar, Star>(pNode);
            }
        }
        return nullptr;
    }

    //Object^ Star::ActiveMethod(String^ strMethod, cli::array<Object^, 1>^ p)
    //{
    //    Object^ pRetObj = nullptr;
    //    if (m_pHostObj != nullptr)
    //    {
    //        MethodInfo^ mi = nullptr;
    //        if (m_pHubbleCLRMethodDic == nullptr)
    //            m_pHubbleCLRMethodDic = gcnew Dictionary<String^, MethodInfo^>();
    //        Object^ pObj = nullptr;
    //        if (m_pHubbleCLRMethodDic->TryGetValue(strMethod, mi) == true)
    //        {
    //            try
    //            {
    //                pRetObj = mi->Invoke(m_pHostObj, p);
    //            }
    //            finally
    //            {
    //            }
    //            return pRetObj;
    //        }
    //        try
    //        {
    //            mi = m_pHostObj->GetType()->GetMethod(strMethod);
    //            m_pHubbleCLRMethodDic[strMethod] = mi;
    //        }
    //        catch (AmbiguousMatchException ^ e)
    //        {
    //            Debug::WriteLine(L"Hubble::ActiveMethod GetMethod: " + e->Message);
    //        }
    //        catch (ArgumentNullException ^ e)
    //        {
    //            Debug::WriteLine(L"Hubble::ActiveMethod GetMethod: " + e->Message);
    //        }
    //        finally
    //        {
    //            if (mi != nullptr)
    //            {
    //                try
    //                {
    //                    pRetObj = mi->Invoke(m_pHostObj, p);
    //                }
    //                finally
    //                {
    //                }
    //            }
    //        }
    //    }

    //    return pRetObj;
    //}

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

    void GalaxyCluster::ObserveQuasars(String^ strFrames, String^ strKey, String^ bstrXml, bool bSaveToConfigFile)
    {
        m_pGalaxyCluster->ObserveQuasars(STRING2BSTR(strFrames), STRING2BSTR(strKey), STRING2BSTR(bstrXml), bSaveToConfigFile);
    }

    Star^ GalaxyCluster::GetStar(String^ strXml, String^ strFrameName)
    {
        if (String::IsNullOrEmpty(strXml) || String::IsNullOrEmpty(strFrameName))
            return nullptr;
        BSTR bstrXml = STRING2BSTR(strXml);
        BSTR bstrFrameName = STRING2BSTR(strFrameName);
        CComPtr<IStar> pNode;
        m_pGalaxyCluster->GetStar(bstrXml, bstrFrameName, &pNode);
        Star^ pRetNode = nullptr;
        if (pNode)
        {
            pRetNode = theAppProxy._createObject<IStar, Star>(pNode);
        }
        ::SysFreeString(bstrXml);
        ::SysFreeString(bstrFrameName);
        return pRetNode;
    }

    String^ GalaxyCluster::GetPageXML()
    {
        BSTR bstrXML;
        m_pGalaxyCluster->get_GalaxyClusterXML(&bstrXML);
        String^ strXML = BSTR2STRING(bstrXML);
        return strXML;
    }

    void Quasar::SendMessage(String^ strFrom, String^ strTo, String^ strMsgId, String^ strMsgContent, String^ strExtra)
    {
        if (m_pQuasar)
        {
            __int64 nHandle;
            m_pQuasar->get_HWND(&nHandle);
            HWND hWnd = (HWND)nHandle;
            IPCMsg msg;
            msg.m_strId = L"MESSAGE";
            msg.m_strParam1 = strFrom;
            msg.m_strParam2 = strTo;
            msg.m_strParam3 = strMsgId;
            msg.m_strParam4 = strMsgContent;
            msg.m_strParam5 = strExtra;
            theApp.m_pHubbleImpl->m_pCurrentIPCMsg = &msg;
            ::SendMessage(hWnd, WM_TANGRAMDATA, (WPARAM)&msg, 20200203);
        }
        theApp.m_pHubbleImpl->m_pCurrentIPCMsg = nullptr;
    }

    Star^ Quasar::Observe(String^ layerName, String^ layerXML)
    {
        Star^ pRetNode = nullptr;
        BSTR blayerName = STRING2BSTR(layerName);
        BSTR blayerXML = STRING2BSTR(layerXML);
        CComPtr<IStar> pNode;
        m_pQuasar->Observe(blayerName, blayerXML, &pNode);
        if (pNode)
        {
            pRetNode = theAppProxy._createObject<IStar, Star>(pNode);
            Star^ pRetNode2 = nullptr;
            if (!TryGetValue(layerName, pRetNode2))
            {
                Add(layerName, pRetNode);
            }
        }
        ::SysFreeString(blayerName);
        ::SysFreeString(blayerXML);
        return pRetNode;
    }

    Object^ Quasar::FrameData::get(String^ iIndex)
    {
        CComVariant bstrVal(::SysAllocString(L""));
        m_pQuasar->get_QuasarData(STRING2BSTR(iIndex), &bstrVal);
        return Marshal::GetObjectForNativeVariant((IntPtr)&bstrVal);
    }

    void Quasar::FrameData::set(String^ iIndex, Object^ newVal)
    {
        IntPtr nPtr = (IntPtr)0;
        Marshal::GetNativeVariantForObject(newVal, nPtr);
        m_pQuasar->put_QuasarData(STRING2BSTR(iIndex), *(VARIANT*)nPtr.ToInt64());
    }
}

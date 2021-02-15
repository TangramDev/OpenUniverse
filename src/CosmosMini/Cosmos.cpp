/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202102150030
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
 * There are Three Key Features of Webruntime:
 * 1. Built-in Modern Web Browser;
 * 2. DOM Plus: DOMPlus is a natural extension of the standard DOM system.
 *    It allows the application system to support a kind of generalized web pages,
 *    which are composed of standard DOM elements and binary components supported
 *    by the application system;
 * 3. JavaScript for Application: Similar to VBA in MS office, JavaScript will
 *    become a built-in programmable language in the application system, so that
 *    the application system can be expanded and developed for the Internet based
 *    on modern javscript/Web technology.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *
 *******************************************************************************/

#include "StdAfx.h"
#include "dllmain.h"
#include "GridCLREvent.h"
#include "Cosmos.h"

using namespace System::Threading;
using namespace System::Diagnostics;
using namespace System::Reflection;
using namespace System::Runtime::InteropServices;

namespace Universe
{
    Xobj::Xobj(IXobj* pXobj)
    {
        m_hWnd = NULL;
        m_pXobjEvent = new CCosmosXobjEvent();
        m_pXobjEvent->m_pXobj = pXobj;
        m_pXobjEvent->m_pXobjCLREvent = new CXobjCLREvent();
        m_pXobjCLREvent = m_pXobjEvent->m_pXobjCLREvent;
        m_pXobjEvent->m_pXobjCLREvent->m_pXobj = this;
        HRESULT hr = m_pXobjEvent->DispEventAdvise(pXobj);
        if (theApp.m_pCosmosImpl)
            theApp.m_pCosmosImpl->AttachXobj(m_pXobjEvent);
        m_pXobj = pXobj;
        LONGLONG nValue = (LONGLONG)pXobj;
        theAppProxy._insertObject(nValue, this);
    }

    Xobj::~Xobj()
    {
        delete m_pXobjCLREvent;
        m_pXobj = NULL;
    }

    Galaxy^ Xobj::Galaxy::get()
    {
        CComPtr<IGalaxy> pGalaxy;
        m_pXobj->get_Galaxy(&pGalaxy);

        return theAppProxy._createObject<IGalaxy, Universe::Galaxy>(pGalaxy);
    }

    String^ Xobj::Caption::get()
    {
        if (m_pXobj)
        {
            CComBSTR bstrCap("");
            m_pXobj->get_Caption(&bstrCap);
            String^ strCap = BSTR2STRING(bstrCap);
            return strCap;
        }
        return "";
    }

    String^ Xobj::URL::get()
    {
        if (m_pXobj)
        {
            ::XobjType nType;
            m_pXobj->get_XobjType(&nType);
            CComBSTR bstrCap("");
            switch (nType)
            {
            case BlankView:
            {
                m_pXobj->get_URL(&bstrCap);
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

    void Xobj::URL::set(String^ newVal)
    {
        if (m_pXobj)
        {
            ::XobjType nType;
            m_pXobj->get_XobjType(&nType);
            switch (nType)
            {
            case BlankView:
                if (nType == BlankView)
                {
                    m_pXobj->put_URL(STRING2BSTR(newVal));

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

    void Xobj::Init()
    {
        LONGLONG h = 0;
        if (m_pXobj)
        {
            m_pXobj->get_Handle(&h);
            ::SendMessage((HWND)h, WM_COSMOSMSG, 1, 0);
        }
    }

    void Xobj::Fire_OnTabChange(Xobj^ ActivePage, Xobj^ OldXobj)
    {
        OnTabChange(ActivePage, OldXobj);
    }

    Cosmos::Cosmos(ICosmos* pCosmos)
    {
    }

    Cosmos::~Cosmos(void)
    {
    }

    Cosmos^ Cosmos::GetCosmos()
    {
        if (theApp.m_pCosmos == nullptr)
        {
            HMODULE hModule = ::GetModuleHandle(L"universe.dll");
            if (hModule == nullptr) {
                TCHAR m_szBuffer[MAX_PATH];
                if (SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, 0, m_szBuffer) ==
                    S_OK) {
                    ATL::CString m_strProgramFilePath = ATL::CString(m_szBuffer);
                    m_strProgramFilePath += _T("\\Cosmos\\universe.dll");
                    if (::PathFileExists(m_strProgramFilePath)) {
                        hModule = ::LoadLibrary(m_strProgramFilePath);
                    }
                }
            }
            if (hModule) {
                typedef CCosmosImpl* (__stdcall* GetCosmosImpl)(ICosmos**);
                GetCosmosImpl _pCosmosImplFunction;
                _pCosmosImplFunction = (GetCosmosImpl)GetProcAddress(hModule, "GetCosmosImpl");
                if (_pCosmosImplFunction != NULL) {
                    theApp.m_pCosmosImpl = _pCosmosImplFunction(&theApp.m_pCosmos);
                    if (theApp.m_pCosmosImpl->m_nAppType == 0)
                        theApp.m_pCosmosImpl->m_nAppType = APP_BROWSERAPP;
                    theApp.m_pCosmosImpl->m_pCosmosDelegate = (ICosmosDelegate*)&theApp;
                    theApp.m_pCosmosImpl->m_pUniverseAppProxy = (IUniverseAppProxy*)&theApp;
                    theApp.m_pCosmosImpl->m_pCLRProxy = &theAppProxy;
                }
            }
        }
        if (m_pCosmos == nullptr)
            m_pCosmos = gcnew Cosmos();
        return m_pCosmos;
    }

    Cosmos^ Cosmos::InitCosmosApp(bool bSupportCrashReporting, CosmosAppType AppType)
    {
        if (m_pCosmos == nullptr)
            m_pCosmos = gcnew Cosmos();
        if (theApp.m_bBrowserModeInit)
            return m_pCosmos;
        theApp.m_pCosmosImpl->m_nAppType = (DWORD)AppType;
        //m_nAppType = AppType;
        switch (AppType)
        {
        case CosmosAppType::APPBROWSER:
        case CosmosAppType::APPBROWSER_ECLIPSE:
            theApp.InitCosmosApp(bSupportCrashReporting);
            break;
        case CosmosAppType::APPECLIPSE:
            break;
        case CosmosAppType::APPBROWSERAPP:
            break;
        }
        return m_pCosmos;
    }

    Xobj^ Cosmos::CreatingXobj::get()
    {
        Object^ pRetObject = nullptr;
        if (theApp.m_pCosmos)
        {
            IXobj* pXobj = nullptr;
            theApp.m_pCosmos->get_CreatingXobj(&pXobj);
            if (pXobj)
                return theAppProxy._createObject<IXobj, Xobj>(pXobj);
        }
        return nullptr;
    }

    bool Cosmos::Fire_OnAppInit()
    {
        if (IsAppInit == false)
        {
            IsAppInit = true;
            return OnAppInit();
        }
        return false;
    }

    void Cosmos::Fire_OnClose()
    {
        OnClose();
    }

    int Cosmos::CosmosInit(String^ strInit)
    {
        CString strInfo = strInit;
        if (strInfo != _T(""))
        {
            __int64 nPointer = _wtoi64(strInfo);
            if (nPointer)
            {
                theApp.m_pCosmosImpl = (CCosmosImpl*)nPointer;
                theApp.m_pCosmosImpl->m_pCLRProxy = &theAppProxy;
                theApp.m_pCosmosImpl->m_pCosmosAppProxy = static_cast<IUniverseAppProxy*>(&theApp);
                //20180817
                if (theApp.m_pCosmos == nullptr)
                {
                    HMODULE hModule = ::GetModuleHandle(_T("universe.dll"));
                    if (hModule) {
                        typedef ICosmos* (__stdcall* GetCosmos)();
                        GetCosmos _pCosmosFunction;
                        _pCosmosFunction = (GetCosmos)GetProcAddress(hModule, "GetCosmos");
                        if (_pCosmosFunction != NULL) {
                            theApp.m_pCosmos = _pCosmosFunction();
                        }
                    }
                }
            }
        }
        return 0;
    };

    Xobj^ Cosmos::GetXobjFromHandle(IntPtr handle)
    {
        IXobj* pWndXobj = nullptr;
        CosmosInfo* pInfo = (CosmosInfo*)::GetProp((HWND)handle.ToPointer(), _T("CosmosInfo"));
        if (pInfo)
            pWndXobj = pInfo->m_pXobj;
        else
            return nullptr;
        return theAppProxy._createObject<IXobj, Xobj>(pWndXobj);
    }

    Xobj^ Cosmos::GetXobjFromControl(Control^ ctrl)
    {
        if (ctrl == nullptr)
        {
            return nullptr;
        }
        IXobj* pWndXobj = nullptr;
        HWND hCtrl = (HWND)ctrl->Handle.ToPointer();
        auto it = theApp.m_pCosmosImpl->m_mapXobj.find(hCtrl);
        if (it != theApp.m_pCosmosImpl->m_mapXobj.end())
        {
            pWndXobj = it->second;
            return theAppProxy._createObject<IXobj, Xobj>(pWndXobj);
        }
        CosmosInfo* pInfo = (CosmosInfo*)::GetProp((HWND)hCtrl, _T("CosmosInfo"));
        if (pInfo)
            pWndXobj = pInfo->m_pXobj;
        else
            return nullptr;
        return theAppProxy._createObject<IXobj, Xobj>(pWndXobj);
    }

    Xobj^ Cosmos::Observe(Control^ ctrl, String^ key, String^ strXobjXml)
    {
        if (ctrl != nullptr)
        {
            if (ctrl->Dock != DockStyle::Fill && ctrl->Dock != DockStyle::None)
                return nullptr;
            IGalaxy* pGalaxy = nullptr;
            theApp.m_pCosmos->GetGalaxy((__int64)ctrl->Handle.ToPointer(), &pGalaxy);
            if (pGalaxy)
            {
                IXobj* pXobj = nullptr;
                BSTR bstrKey = STRING2BSTR(key);
                BSTR bstrXml = STRING2BSTR(strXobjXml);
                pGalaxy->Observe(bstrKey, bstrXml, &pXobj);
                ::SysFreeString(bstrKey);
                ::SysFreeString(bstrXml);
                if (pXobj)
                    return theAppProxy._createObject<IXobj, Xobj>(pXobj);
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
                                if(::IsChild((HWND)pCtrl->Handle.ToPointer(),(HWND)ctrl->Handle.ToPointer()))
                                { 
                                    bMdiClientChild = true;
                                }
                                break;
                            }
                        }
                        if(bMdiClientChild)
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
                        IGalaxyCluster* pGalaxyCluster = nullptr;
                        theApp.m_pCosmos->CreateGalaxyCluster(pForm2->Handle.ToInt64(), &pGalaxyCluster);
                        if (pGalaxyCluster != nullptr)
                        {
                            IGalaxy* pGalaxy = nullptr;
                            BSTR bstrName = STRING2BSTR(ctrl->Name);
                            Xobj^ thisXobj = nullptr;
                            pGalaxyCluster->CreateGalaxy(CComVariant((__int64)0), CComVariant((__int64)ctrl->Handle.ToInt64()), bstrName, &pGalaxy);
                            if (pGalaxy)
                            {
                                IXobj* pXobj = nullptr;
                                BSTR bstrKey = STRING2BSTR(key);
                                BSTR bstrXml = STRING2BSTR(strXobjXml);
                                pGalaxy->Observe(bstrKey, bstrXml, &pXobj);
                                if (pXobj)
                                {
                                    thisXobj = theAppProxy._createObject<IXobj, Xobj>(pXobj);
                                }
                                ::SysFreeString(bstrKey);
                                ::SysFreeString(bstrXml);
                            }
                            ::SysFreeString(bstrName);
                            return thisXobj;
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
                Xobj^ pXobj = GetXobjFromHandle((IntPtr)hTopWnd);
                while (pXobj == nullptr)
                {
                    hTopWnd = ::GetParent(hTopWnd);
                    pXobj = GetXobjFromHandle((IntPtr)hTopWnd);
                }

                Xobj^ _pRetXobj = nullptr;
                IGalaxyCluster* pGalaxyCluster = nullptr;
                if (pXobj != nullptr)
                {
                    pXobj->m_pXobj->get_GalaxyCluster(&pGalaxyCluster);
                    if (pGalaxyCluster)
                    {
                        String^ strName = ctrl->Name;
                        BSTR bstrName = STRING2BSTR(strName);
                        HWND hWnd = (HWND)ctrl->Handle.ToPointer();
                        pGalaxyCluster->CreateGalaxy(CComVariant((__int64)0), CComVariant((__int64)hWnd), bstrName, &pGalaxy);
                        if (pGalaxy)
                        {
                            IXobj* pXobj = nullptr;
                            BSTR bstrKey = STRING2BSTR(key);
                            BSTR bstrXml = STRING2BSTR(strXobjXml);
                            pGalaxy->Observe(bstrKey, bstrXml, &pXobj);
                            ::SysFreeString(bstrKey);
                            ::SysFreeString(bstrXml);
                            if(pXobj)
                                _pRetXobj = theAppProxy._createObject<IXobj, Xobj>(pXobj);
                        }
                        ::SysFreeString(bstrName);
                        ::InvalidateRect(hWnd, nullptr, true);
                    }
                }
                else
                {
                    theApp.m_pCosmos->CreateGalaxyCluster((__int64)_hTopWnd, &pGalaxyCluster);
                    if (pGalaxyCluster != nullptr)
                    {
                        BSTR bstrName = STRING2BSTR(ctrl->Name);
                        pGalaxyCluster->CreateGalaxy(CComVariant((__int64)0), CComVariant((__int64)ctrl->Handle.ToInt64()), bstrName, &pGalaxy);
                        if (pGalaxy)
                        {
                            IXobj* pXobj = nullptr;
                            BSTR bstrKey = STRING2BSTR(key);
                            BSTR bstrXml = STRING2BSTR(strXobjXml);
                            pGalaxy->Observe(bstrKey, bstrXml, &pXobj);
                            ::SysFreeString(bstrKey);
                            ::SysFreeString(bstrXml);
                            if (pXobj)
                                _pRetXobj = theAppProxy._createObject<IXobj, Xobj>(pXobj);
                        }
                        ::SysFreeString(bstrName);
                    }
                }
                return _pRetXobj;
            }
        }
        return nullptr;
    }

    Type^ Cosmos::GetType(String^ strObjID)
    {
        if (String::IsNullOrEmpty(strObjID) == true)
            return nullptr;

        String^ m_strID = strObjID->ToLower()->Replace(L" ", L"");
        String^ strLib = nullptr;
        Type^ pType = nullptr;
        if (m_strID != L"")
        {
            Monitor::Enter(m_pCosmosCLRTypeDic);
            String^ strID = nullptr;
            if (m_pCosmosCLRTypeDic->TryGetValue(m_strID, pType) == false)
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
                        _strID = _strID.Left(nIndex + 1) + theApp.m_pCosmosImpl->m_strExeName;
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
                                m_pDotNetAssembly = GetCosmos()->GetType()->Assembly;
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
                            Debug::WriteLine(L"Cosmos CreateObject: " + e->Message);
                        }
                        catch (ArgumentException ^ e)
                        {
                            Debug::WriteLine(L"Cosmos CreateObject: " + e->Message);
                        }
                        catch (FileNotFoundException ^ e)
                        {
                            Debug::WriteLine(L"Cosmos CreateObject: " + e->Message);
                        }
                        catch (FileLoadException ^ e)
                        {
                            Debug::WriteLine(L"Cosmos CreateObject: " + e->Message);
                        }
                        catch (BadImageFormatException ^ e)
                        {
                            Debug::WriteLine(L"Cosmos CreateObject: " + e->Message);
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
                            Debug::WriteLine(L"Cosmos CreateObject Assembly->GetType: " + e->Message);
                        }
                        catch (ArgumentNullException ^ e)
                        {
                            Debug::WriteLine(L"Cosmos CreateObject Assembly->GetType: " + e->Message);
                        }
                        catch (ArgumentException ^ e)
                        {
                            Debug::WriteLine(L"Cosmos CreateObject Assembly->GetType: " + e->Message);
                        }
                        catch (FileNotFoundException ^ e)
                        {
                            Debug::WriteLine(L"Cosmos CreateObject Assembly->GetType: " + e->Message);
                        }
                        catch (FileLoadException ^ e)
                        {
                            Debug::WriteLine(L"Cosmos CreateObject Assembly->GetType: " + e->Message);
                        }
                        catch (BadImageFormatException ^ e)
                        {
                            Debug::WriteLine(L"Cosmos CreateObject Assembly->GetType: " + e->Message);
                        }
                        finally
                        {
                            if (pType != nullptr)
                            {
                                m_pCosmosCLRTypeDic->Add(m_strID, pType);
                            }
                        }
                    }
                }
            }
            Monitor::Exit(m_pCosmosCLRTypeDic);
        }
        return pType;
    }

    Form^ Cosmos::CreateForm(IWin32Window^ parent, String^ strObjID)
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

    String^ Cosmos::GetCtrlTag(Control^ ctrl)
    {
        HWND hWnd = (HWND)ctrl->Handle.ToPointer();
        auto it = theApp.m_pCosmosImpl->m_mapCtrlTag.find(hWnd);
        if (it != theApp.m_pCosmosImpl->m_mapCtrlTag.end())
            return BSTR2STRING(it->second);
        return L"";
    }

    String^ Cosmos::GetUIData(Control^ ctrl)
    {
        HWND hWnd = (HWND)ctrl->Handle.ToPointer();
        auto it = theApp.m_pCosmosImpl->m_mapUIData.find(hWnd);
        if (it != theApp.m_pCosmosImpl->m_mapUIData.end())
            return BSTR2STRING(it->second);
        return L"";
    }

    Object^ Cosmos::CreateObject(String^ strObjID)
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
                                    theApp.m_pCosmosImpl->m_hMainWnd = (HWND)thisForm->Handle.ToPointer();
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
                Debug::WriteLine(L"Cosmos CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (ArgumentNullException ^ e)
            {
                Debug::WriteLine(L"Cosmos CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (ArgumentException ^ e)
            {
                Debug::WriteLine(L"Cosmos CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (NotSupportedException ^ e)
            {
                Debug::WriteLine(L"Cosmos CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (TargetInvocationException ^ e)
            {
                Debug::WriteLine(L"Cosmos CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (MethodAccessException ^ e)
            {
                Debug::WriteLine(L"Cosmos CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (InvalidComObjectException ^ e)
            {
                Debug::WriteLine(L"Cosmos CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (MissingMethodException ^ e)
            {
                Debug::WriteLine(L"Cosmos CreateObject Activator::CreateInstance: " + e->Message);
            }
            catch (COMException ^ e)
            {
                Debug::WriteLine(L"Cosmos CreateObject Activator::CreateInstance: " + e->Message);
            }
        }

        return m_pObj;
    }

    Xobj^ Xobj::Observe(String^ layerName, String^ layerXML)
    {
        if (m_pXobj)
        {
            BSTR blayerName = STRING2BSTR(layerName);
            BSTR blayerXML = STRING2BSTR(layerXML);
            IXobj* pXobj = nullptr;
            m_pXobj->Observe(blayerName, blayerXML, &pXobj);
            ::SysFreeString(blayerName);
            ::SysFreeString(blayerXML);
            if (pXobj)
            {
                return theAppProxy._createObject<IXobj, Xobj>(pXobj);
            }
        }
        return nullptr;
    }

    Xobj^ Xobj::ObserveChild(int rowNum, int colNum, String^ layerName, String^ layerXML)
    {
        if (m_pXobj)
        {
            BSTR blayerName = STRING2BSTR(layerName);
            BSTR blayerXML = STRING2BSTR(layerXML);
            IXobj* pXobj = nullptr;
            m_pXobj->ObserveEx(rowNum, colNum, blayerName, blayerXML, &pXobj);
            ::SysFreeString(blayerName);
            ::SysFreeString(blayerXML);
            if (pXobj)
            {
                return theAppProxy._createObject<IXobj, Xobj>(pXobj);
            }
        }
        return nullptr;
    }

    Xobj^ Galaxy::Observe(String^ layerName, String^ layerXML)
    {
        Universe::Xobj^ pRetNode = nullptr;
        BSTR blayerName = STRING2BSTR(layerName);
        BSTR blayerXML = STRING2BSTR(layerXML);
        CComPtr<IXobj> pXobj;
        m_pGalaxy->Observe(blayerName, blayerXML, &pXobj);
        if (pXobj)
        {
            pRetNode = theAppProxy._createObject<IXobj, Universe::Xobj>(pXobj);
            Universe::Xobj^ pRetNode2 = nullptr;
            if (!TryGetValue(layerName, pRetNode2))
            {
                Add(layerName, pRetNode);
            }
        }
        ::SysFreeString(blayerName);
        ::SysFreeString(blayerXML);
        return pRetNode;
    }
}

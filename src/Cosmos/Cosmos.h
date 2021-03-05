/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202103060044           *
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
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
 * Use of this source code is governed by a BSD-style license that
 * can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *******************************************************************************/

#pragma once
#include "CosmosProxy.h"
#include <map>

using namespace std;
using namespace System;
using namespace System::IO;
using namespace System::Windows;
using namespace System::Reflection;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;
using namespace System::ComponentModel;
using namespace System::Collections::Generic;
using namespace System::Threading;
using namespace System::Threading::Tasks;

using System::Runtime::InteropServices::Marshal;

class CXobjCLREvent;
class CCosmosXobjEvent;
extern CCosmosProxy theAppProxy;

namespace Universe
{
	/// <summary>
	/// Summary for Cosmos
	/// </summary>

	ref class Xobj;
	ref class Galaxy;
	ref class Browser;

	public enum class CosmosAppType
	{
		APPWIN32 = APP_WIN32,
		APPBROWSER = APP_BROWSER,
		APPECLIPSE = APP_ECLIPSE,
		APPBROWSER_ECLIPSE = APP_BROWSER_ECLIPSE,
		APPBROWSERAPP = APP_BROWSERAPP,
		APPOTHER = 0
	};

	public ref class Xobj : public Dictionary<String^, Xobj^>, public IWin32Window
	{
	public:
		Xobj(IXobj* pXobj);
		~Xobj();

		IXobj* m_pXobj;
		Object^ m_pHostObj = nullptr;
		CCosmosXobjEvent* m_pXobjEvent;
		CXobjCLREvent* m_pXobjCLREvent;
		Wormhole^ m_pSession = nullptr;

		// A notification has been created for all nodes in the current layout.
		delegate void RootXobjCreated(IntPtr nHandle, String^ strUrl, Xobj^ pRootXobj);
		event RootXobjCreated^ OnRootXobjCreated;
		void Fire_RootXobjCreated(IntPtr nHandle, String^ strUrl, Xobj^ pRootXobj)
		{
			OnRootXobjCreated(nHandle, strUrl, pRootXobj);
		}

		delegate void ObserveComplete(Xobj^ sender);
		event ObserveComplete^ OnObserveComplete;
		void Fire_ObserveComplete(Xobj^ sender)
		{
			OnObserveComplete(sender);
		}

		delegate void Destroy(Xobj^ sender);
		event Destroy^ OnDestroy;
		void Fire_OnDestroy(Xobj^ sender)
		{
			OnDestroy(sender);
		}

		delegate void DocumentComplete(Xobj^ sender, Object^ pHtmlDoc, String^ strURL);
		event DocumentComplete^ OnDocumentComplete;
		void Fire_OnDocumentComplete(Xobj^ sender, Object^ pHtmlDoc, String^ strURL)
		{
			OnDocumentComplete(sender, pHtmlDoc, strURL);
		}

		delegate void TabChange(Xobj^ ActiveXobj, Xobj^ OldXobj);
		event TabChange^ OnTabChange;
		void Fire_OnTabChange(Xobj^ ActiveXobj, Xobj^ OldXobj);

		delegate void MessageHandle(String^ strFrom, String^ strTo, String^ strMsgId, String^ strPayload, String^ strExtra);
		event MessageHandle^ OnIPCMessageReceived;
		void Fire_OnIPCMessageReceived(String^ strFrom, String^ strTo, String^ strMsgId, String^ strPayload, String^ strExtra)
		{
			OnIPCMessageReceived(strFrom, strTo, strMsgId, strPayload, strExtra);
		}

		delegate void CloudMessageHandle(Wormhole^ cloudSession);
		event CloudMessageHandle^ OnCloudMessageReceived;
		void Fire_OnCloudMessageReceived(Wormhole^ cloudSession)
		{
			OnCloudMessageReceived(cloudSession);
		}

		delegate void BindCLRObjToWebPage(Object^ SourceObj, Wormhole^ eventSession, String^ eventName);
		event BindCLRObjToWebPage^ OnBindCLRObjToWebPage;
		void Fire_OnBindCLRObjToWebPage(Object^ SourceObj, Wormhole^ eventSession, String^ eventName)
		{
			Control^ pCtrl = (Control^)SourceObj;
			OnBindCLRObjToWebPage(SourceObj, eventSession, eventName);
		}

		String^ SendIPCMessage(String^ strTo, String^ strPayload, String^ strExtra, String^ strMsgId)
		{
			BSTR bstrRet;
			HRESULT hr = m_pXobj->SendIPCMessage(STRING2BSTR(strTo), STRING2BSTR(strPayload), STRING2BSTR(strExtra), STRING2BSTR(strMsgId), &bstrRet);
			if (hr == S_OK)
			{
				return BSTR2STRING(bstrRet);
			}
			return "";
		}

		property Wormhole^ Wormhole
		{
			Universe::Wormhole^ get();
		}

	public:
		Xobj^ Observe(String^ layerName, String^ layerXML);
		Xobj^ ObserveChild(int rowNum, int colName, String^ layerName, String^ layerXML);

		property String^ Caption
		{
			String^ get();
			void set(String^ strCaption)
			{
				BSTR bstrCap = STRING2BSTR(strCaption);
				m_pXobj->put_Caption(bstrCap);
				::SysFreeString(bstrCap);
			}
		}

		property String^ Name
		{
			String^ get()
			{
				if (m_pXobj)
				{
					CComBSTR bstrCap("");
					m_pXobj->get_Name(&bstrCap);
					return BSTR2STRING(bstrCap);
				}
				return "";
			}

			void set(String^ newVal)
			{
				if (m_pXobj)
				{
					m_pXobj->put_Name(STRING2BSTR(newVal));
				}
			}
		}

		property String^ URL
		{
			String^ get();
			void set(String^ newVal);
		}

		property Object^ XObject
		{
			Object^ get()
			{
				if (m_pHostObj != nullptr)
					return m_pHostObj;

				VARIANT var;
				m_pXobj->get_XObject(&var);

				try
				{
					m_pHostObj = Marshal::GetObjectForNativeVariant((System::IntPtr) & var);
				}
				catch (InvalidOleVariantTypeException^ e)
				{
					e->Message;
				}
				catch (...)
				{

				}
				return m_pHostObj;
			}
		}

		property System::Drawing::Color rgbMiddle
		{
			System::Drawing::Color get()
			{
				OLE_COLOR rgb;
				m_pXobj->get_rgbMiddle(&rgb);
				return System::Drawing::ColorTranslator::FromOle(rgb);
			}
			void set(System::Drawing::Color newVal)
			{
				m_pXobj->put_rgbMiddle(System::Drawing::ColorTranslator::ToOle(newVal));
			}
		}

		property System::Drawing::Color rgbLeftTop
		{
			System::Drawing::Color get()
			{
				OLE_COLOR rgb;
				m_pXobj->get_rgbLeftTop(&rgb);
				return System::Drawing::ColorTranslator::FromOle(rgb);
			}
			void set(System::Drawing::Color newVal)
			{
				m_pXobj->put_rgbLeftTop(System::Drawing::ColorTranslator::ToOle(newVal));
			}
		}

		property System::Drawing::Color rgbRightBottom
		{
			System::Drawing::Color get()
			{
				OLE_COLOR rgb;
				m_pXobj->get_rgbRightBottom(&rgb);
				return System::Drawing::ColorTranslator::FromOle(rgb);
			}
			void set(System::Drawing::Color newVal)
			{
				m_pXobj->put_rgbRightBottom(System::Drawing::ColorTranslator::ToOle(newVal));
			}
		}

		property Object^ Extender
		{
			Object^ get()
			{
				Object^ pRetObject = nullptr;
				IDispatch* pDisp = NULL;
				m_pXobj->get_Extender(&pDisp);
				if (pDisp)
					pRetObject = Marshal::GetObjectForIUnknown((IntPtr)pDisp);
				return pRetObject;
			}
			void set(Object^ obj)
			{
				IDispatch* pDisp = (IDispatch*)Marshal::GetIUnknownForObject(obj).ToPointer();
				m_pXobj->put_Extender(pDisp);
			}
		}

		property Object^ Tag
		{
			Object^ get()
			{
				Object^ pRetObject = nullptr;
				VARIANT var;
				m_pXobj->get_Tag(&var);
				try
				{
					pRetObject = Marshal::GetObjectForNativeVariant((System::IntPtr) & var);
				}
				catch (InvalidOleVariantTypeException^)
				{

				}
				catch (NotSupportedException^)
				{

				}
				return pRetObject;
			}

			void set(Object^ obj)
			{
				try
				{
					VARIANT var;
					Marshal::GetNativeVariantForObject(obj, (System::IntPtr) & var);
					m_pXobj->put_Tag(var);
				}
				catch (ArgumentException^ e)
				{
					e->Data->ToString();
				}
			}
		}

		property int ActivePage
		{
			int get()
			{
				int nPage = 0;
				m_pXobj->get_ActivePage(&nPage);

				return nPage;
			}

			void set(int nPage)
			{
				m_pXobj->put_ActivePage(nPage);
			}
		}

		property Galaxy^ Galaxy
		{
			Universe::Galaxy^ get();
		}

		property Universe::Galaxy^ HostGalaxy
		{
			Universe::Galaxy^ get()
			{
				CComPtr<IGalaxy> pGalaxy;
				m_pXobj->get_HostGalaxy(&pGalaxy);
				if (pGalaxy)
				{
					return theAppProxy._createObject<IGalaxy, Universe::Galaxy>(pGalaxy);
				}
				return nullptr;
			}
		}

		property Xobj^ RootXobj
		{
			Xobj^ get()
			{
				CComPtr<IXobj> pRootXobj;
				m_pXobj->get_RootXobj(&pRootXobj);

				return theAppProxy._createObject<IXobj, Xobj>(pRootXobj);
			}
		}

		property Xobj^ ParentXobj
		{
			Xobj^ get()
			{
				CComPtr<IXobj> pXobj;
				m_pXobj->get_ParentXobj(&pXobj);

				return theAppProxy._createObject<IXobj, Xobj>(pXobj);
			}
		}

		property Xobj^ HostXobj
		{
			Xobj^ get()
			{
				IXobj* pXobj = nullptr;
				m_pXobj->get_HostXobj(&pXobj);
				if (pXobj == NULL)
					return nullptr;
				return theAppProxy._createObject<IXobj, Xobj>(pXobj);
			}
		}

		property int Col
		{
			int get()
			{
				long nValue = 0;
				m_pXobj->get_Col(&nValue);
				return nValue;
			}
		}

		property int Row
		{
			int get()
			{
				long nValue = 0;
				m_pXobj->get_Row(&nValue);
				return nValue;
			}
		}

		property int Hmin
		{
			int get()
			{
				int nValue = 0;
				m_pXobj->get_Hmin(&nValue);
				return nValue;
			}
			void set(int newVal)
			{
				m_pXobj->put_Hmin(newVal);
			}
		}

		property int Hmax
		{
			int get()
			{
				int nValue = 0;
				m_pXobj->get_Hmax(&nValue);
				return nValue;
			}
			void set(int newVal)
			{
				m_pXobj->put_Hmax(newVal);
			}
		}

		property int Vmin
		{
			int get()
			{
				int nValue = 0;
				m_pXobj->get_Vmin(&nValue);
				return nValue;
			}
			void set(int newVal)
			{
				m_pXobj->put_Vmin(newVal);
			}
		}

		property int Vmax
		{
			int get()
			{
				int nValue = 0;
				m_pXobj->get_Vmax(&nValue);
				return nValue;
			}
			void set(int newVal)
			{
				m_pXobj->put_Vmax(newVal);
			}
		}

		property int Rows
		{
			int get()
			{
				long nValue = 0;
				m_pXobj->get_Rows(&nValue);
				return nValue;
			}
		}

		property int Cols
		{
			int get()
			{
				long nValue = 0;
				m_pXobj->get_Cols(&nValue);
				return nValue;
			}
		}

		property ::XobjType XobjType
		{
			::XobjType get()
			{
				::XobjType type;
				m_pXobj->get_XobjType(&type);
				return (::XobjType)type;
			}
		}

		property IntPtr Handle
		{
			virtual IntPtr get()
			{
				if (m_hWnd)
					return (IntPtr)m_hWnd;
				__int64 h = 0;
				m_pXobj->get_Handle(&h);
				m_hWnd = (HWND)h;
				return (IntPtr)h;
			}
		}

		property String^ Attribute[String^]
		{
			String ^ get(String ^ strKey)
			{
				BSTR bstrVal;
				m_pXobj->get_Attribute(STRING2BSTR(strKey),&bstrVal);

				return BSTR2STRING(bstrVal);
			}

			void set(String ^ strKey, String ^ strVal)
			{
				m_pXobj->put_Attribute(
					STRING2BSTR(strKey),
					STRING2BSTR(strVal));
			}
		}

		Xobj^ GetXobj(int nRow, int nCol)
		{
			IXobj* pXobj;
			m_pXobj->GetXobj(nRow, nCol, &pXobj);
			return theAppProxy._createObject<IXobj, Xobj>(pXobj);
		}

		Xobj^ GetChildXobj(String^ strName)
		{
			IXobj* pXobj;
			BSTR bstrName = STRING2BSTR(strName);
			m_pXobj->GetChildXobjByName(bstrName, &pXobj);
			::SysFreeString(bstrName);
			return theAppProxy._createObject<IXobj, Xobj>(pXobj);
		}

	private:
		HWND m_hWnd;
		void Init();
	};

	public ref class Galaxy : public Dictionary<String^, Xobj^>
	{
	public:
		Galaxy(IGalaxy* pGalaxy)
		{
			m_pGalaxy = pGalaxy;
		}
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Galaxy()
		{
			LONGLONG nValue = (LONGLONG)m_pGalaxy;
			theAppProxy._removeObject(nValue);
		}

		Xobj^ Observe(String^ layerName, String^ layerXML);

		property IntPtr Handle
		{
			IntPtr get()
			{
				__int64 nHandle;
				m_pGalaxy->get_HWND(&nHandle);
				return (IntPtr)nHandle;
			}
		}

		property Xobj^ VisibleXobj
		{
			Xobj^ get()
			{
				IXobj* pXobj = nullptr;
				m_pGalaxy->get_VisibleXobj(&pXobj);
				return theAppProxy._createObject<IXobj, Xobj>(pXobj);
			}
		}
	private:
		IGalaxy* m_pGalaxy;
	};

	public ref class Cosmos
	{
	public:
		Cosmos(ICosmos* pCosmos);
		~Cosmos();
	private:
		Cosmos() {};
		static bool IsAppInit = false;
		static bool IsWebRuntimeInit = false;

		static Cosmos^ m_pCosmos;
		static Form^ m_pMainForm = nullptr;
		static Dictionary<String^, Type^>^ m_pCosmosCLRTypeDic = gcnew Dictionary<String^, Type^>();
		static Dictionary<Object^, Wormhole^>^ m_pWormholes = gcnew Dictionary<Object^, Wormhole^>();

		static Cosmos^ InitCosmosApp(bool bSupportCrashReporting, CosmosAppType AppType);
		static bool WebRuntimeInit();
		static Type^ GetType(String^ ObjID);
		static void OnFormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);
	public:
		static int CosmosInit(String^ strInit);
		static System::Drawing::Icon^ m_pDefaultIcon = nullptr;

		static Object^ CreateObject(String^ ObjID);
		static String^ GetCtrlTag(Control^ ctrl);
		static String^ GetUIData(Control^ ctrl);
		static Form^ CreateForm(IWin32Window^ parent, String^ ObjID);
		//static Browser^ ActiveBrowser();
		//static Browser^ GetHostBrowser(Object^ obj);
		static Xobj^ GetXobjFromHandle(IntPtr handle);
		static Xobj^ GetXobjFromControl(Control^ ctrl);
		static Xobj^ Observe(Control^ ctrl, String^ key, String^ strXobjXml);
		static void BindObjToWebPage(IntPtr hWebPage, Object^ pObj, String^ name);
		static Wormhole^ GetWormholeFromObj(Object^ obj);
		static void SendXmlMessage(Xobj^ sender, String^ strXml);

		static Cosmos^ GetCosmos();

		static void Run();
		static void Run(Form^ Mainform);
		static void Run(ApplicationContext^ context);

		static property Dictionary<Object^, Wormhole^>^ Wormholes
		{
			Dictionary<Object^, Wormhole^>^ get()
			{
				return m_pWormholes;
			};
		}

		static property Form^ MainForm
		{
			Form^ get();
			void set(Form^ frm);
		}

		static property Browser^ HostWebBrowser
		{
			Browser^ get();
		}

		//static property System::Drawing::Icon^ DefaultIcon
		//{
		//	System::Drawing::Icon^ get();
		//}

		//static property CosmosAppType AppType
		//{
		//	CosmosAppType get();
		//	void set(CosmosAppType nType);
		//}

		static property Xobj^ CreatingXobj
		{
			Xobj^ get();
		}

		static Browser^ CreateBrowser(IntPtr ParentHandle, String^ strUrls);

		static property String^ NTPXml
		{
			String^ get();
			void set(String^ strXml);
		}

		delegate void Close();
		static event Close^ OnClose;
		static void Fire_OnClose();

		//delegate void CloudAppIdle();
		//static event CloudAppIdle^ OnCloudAppIdle;
		//static void Fire_OnCloudAppIdle();

		delegate bool TangramAppInit();
		static TangramAppInit^ pOnAppInit;
		// Ref https://docs.microsoft.com/en-us/previous-versions/58cwt3zh%28v%3dvs.140%29#custom-accessor-methods
		static event TangramAppInit^ OnAppInit
		{
			void add(TangramAppInit^ p)
			{
				pOnAppInit = static_cast<TangramAppInit^>(Delegate::Combine(pOnAppInit, p));
			}
			void remove(TangramAppInit^ p)
			{
				pOnAppInit = static_cast<TangramAppInit^>(Delegate::Remove(pOnAppInit, p));
			}
			bool raise()
			{
				if (pOnAppInit != nullptr)
				{
					return pOnAppInit->Invoke();
				}
				return true;
			}
		};
		static bool Fire_OnAppInit();

		delegate System::Drawing::Icon^ GetAppIcon(int nIndex);
		static event GetAppIcon^ OnGetAppIcon;
		static System::Drawing::Icon^ Fire_OnGetAppIcon(int nIndex)
		{
			return OnGetAppIcon(nIndex);
		}

		delegate Object^ GetSubObjForWebPage(Object^ SourceObj, String^ subObjName);
		static event GetSubObjForWebPage^ OnGetSubObjForWebPage;
		static Object^ Fire_OnGetSubObjForWebPage(Object^ SourceObj, String^ subObjName);

		delegate void BindCLRObjToWebPage(Object^ SourceObj, Wormhole^ eventSession, String^ eventName);
		static event BindCLRObjToWebPage^ OnBindCLRObjToWebPage;
		static void Fire_OnBindCLRObjToWebPage(Object^ SourceObj, Wormhole^ eventSession, String^ eventName);

		delegate void OpenComplete(IntPtr hWnd, String^ bstrUrl, Xobj^ pRootXobj);
		static event OpenComplete^ OnObserverComplete;
		static void Fire_OnObserverComplete(IntPtr hWnd, String^ bstrUrl, Xobj^ pRootXobj)
		{
			OnObserverComplete(hWnd, bstrUrl, pRootXobj);
		}

		delegate void CosmosMsg(IntPtr hWnd, String^ strType, String^ strParam1, String^ strParam2);
		static event CosmosMsg^ OnCosmosMsg;
		static void Fire_OnCosmosMsg(IntPtr hWnd, String^ strType, String^ strParam1, String^ strParam2)
		{
			OnCosmosMsg(hWnd, strType, strParam1, strParam2);
		}

		delegate void CosmosMsgReceived(Wormhole^ cloudSession);
		static event CosmosMsgReceived^ OnCosmosMsgReceived;
		static void Fire_OnCosmosMsgReceived(Wormhole^ cloudSession)
		{
			OnCosmosMsgReceived(cloudSession);
		}

		delegate void CustomizedDOMElement(IntPtr hWnd, String^ strRuleName, String^ strHTML);
		static event CustomizedDOMElement^ OnCustomizedDOMElement;
		static void Fire_OnCustomizedDOMElement(IntPtr hWnd, String^ strRuleName, String^ strHTML)
		{
			OnCustomizedDOMElement(hWnd, strRuleName, strHTML);
		}

		delegate void FormNodeCreated(String^ bstrObjID, Form^ pForm, Xobj^ pXobj);
		static event FormNodeCreated^ OnFormNodeCreated;
		static void Fire_OnFormNodeCreated(String^ bstrObjID, Form^ pForm, Xobj^ pXobj)
		{
			OnFormNodeCreated(bstrObjID, pForm, pXobj);
		}
	};
}

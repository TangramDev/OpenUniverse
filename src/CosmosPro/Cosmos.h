/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202103100048
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
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *
 *******************************************************************************/

#pragma once
#include "CosmosProxy.h"
#include <map>

using namespace std;
using namespace System;
using namespace System::Windows;
using namespace System::Reflection;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::IO;
using namespace System::Threading;
using namespace System::Threading::Tasks;

using System::Runtime::InteropServices::Marshal;

extern CCosmosProxy theAppProxy;
class CXobjCLREvent;
class CCosmosXobjEvent;

namespace Universe
{
	//[Guid("54499D5E-AC2F-4F8B-9782-C00A9BB2F4E2")]
	//[ComVisibleAttribute(true)]
	//[InterfaceTypeAttribute(ComInterfaceType::InterfaceIsIDispatch)]
	//public interface class IManagerExtender
	//{
	//	[DispId(0x000001)]
	//	virtual void CloseForm(System::Object^ pForm);
	//	[DispId(0x000002)]
	//	virtual void OnCloseForm(long long hFormWnd);
	//};

	/// <summary>
	/// Summary for Cosmos
	/// </summary>
	interface class ICosmosApp;

	ref class Xobj;
	ref class GalaxyCluster;
	ref class Galaxy;
	ref class WorkBenchWindow;
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

	template<typename T>
	private ref class TangramBaseEnumerator : public System::Collections::IEnumerator
	{
	public:
		TangramBaseEnumerator(T^ v, int iCount)
		{
			m_pT = v;
			nCount = iCount;
			Reset();
		}

		virtual property Object^ Current
		{
			Object^ get()
			{
				if (index < 0 || index >= nCount)
				{
					throw gcnew InvalidOperationException();
				}
				return m_pT->default[index];
			}
		}

		virtual bool MoveNext()
		{
			index++;
			return (index < nCount);
		}

		virtual void Reset()
		{
			index = -1;
		}

	private:
		int index;
		long nCount;
		T^ m_pT;
	};

	public ref class XobjCollection : public System::Collections::IEnumerable
	{
	public:
		XobjCollection(IXobjCollection* pXobjs)
		{
			if (pXobjs)
			{
				m_pXobjCollection = pXobjs;
				m_pXobjCollection->AddRef();
			}
		};

		~XobjCollection()
		{
			if (m_pXobjCollection)
			{
				DWORD dw = m_pXobjCollection->Release();
				while (dw)
					dw = m_pXobjCollection->Release();
			}
		}

	private:
		IXobjCollection* m_pXobjCollection;
	public:
		virtual System::Collections::IEnumerator^ GetEnumerator()
		{
			return gcnew TangramBaseEnumerator<XobjCollection>(this, StarCount);
		}

		property Xobj^ default[int]
		{
			Xobj ^ get(int iIndex);
		}

		property int StarCount
		{
			int get()
			{
				long n = 0;
				m_pXobjCollection->get_XobjCount(&n);
				return n;
			}
		}
	};

	public ref class Xobj : public Dictionary<String^, Xobj^> , public IWin32Window
	{
	public:
		Xobj(IXobj* pXobj);
		~Xobj();

		IXobj* m_pXobj;
		Object^ m_pHostObj = nullptr;
		ListView^ m_pBindListView = nullptr;
		TreeView^ m_pBindTreeView = nullptr;
		CCosmosXobjEvent* m_pXobjEvent;
		CXobjCLREvent* m_pXobjCLREvent;
		Universe::Wormhole^ m_pSession = nullptr;

		void NavigateTreeInit();
		//virtual void CloseForm(Object^ pForm){};
		//virtual void OnCloseForm(long long hFormWnd){};
		delegate void XobjAddInCreated(Xobj^ sender, Object^ pAddIndisp, String^ bstrAddInID, String^ bstrAddInXml);
		event XobjAddInCreated^ OnXobjAddInCreated;
		void Fire_XobjAddInCreated(Xobj^ sender, Object^ pAddIndisp, String^ bstrAddInID, String^ bstrAddInXml)
		{
			OnXobjAddInCreated(sender, pAddIndisp, bstrAddInID, bstrAddInXml);
		}

		delegate void XobjAddInsCreated(Xobj^ sender);
		event XobjAddInsCreated^ OnXobjAddInsCreated;
		void Fire_XobjAddInsCreated(Xobj^ sender)
		{
			OnXobjAddInsCreated(sender);
		}

		// A notification has been created for all nodes in the current layout.
		delegate void RootXobjCreated(IntPtr nHandle, String^ strUrl, Xobj^ pRootXobj);
		event RootXobjCreated^ OnRootXobjCreated;
		void Fire_RootXobjCreated(IntPtr nHandle, String^ strUrl, Xobj^ pRootXobj)
		{
			OnRootXobjCreated(nHandle, strUrl, pRootXobj);
			// Notify all child nodes
			for each (Xobj ^ pChildNode in this->ChildNodes)
			{
				pChildNode->Fire_RootXobjCreated(nHandle, strUrl, pRootXobj);
			}
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

		delegate void BindCLRObjToWebPage(Object^ SourceObj, Universe::Wormhole^ eventSession, String^ eventName);
		event BindCLRObjToWebPage^ OnBindCLRObjToWebPage;
		void Fire_OnBindCLRObjToWebPage(Object^ SourceObj, Universe::Wormhole^ eventSession, String^ eventName)
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

		property Wormhole^ Wormhole
		{
			Universe::Wormhole^ get();
		}

		property TreeView^ BindTreeView
		{
			void set(TreeView^ treeview)
			{
				if (m_pBindTreeView == nullptr)
					m_pBindTreeView = treeview;
			}
		}

		property ListView^ BindListView
		{
			void set(ListView^ Listview)
			{
				if (m_pBindListView == nullptr)
					m_pBindListView = Listview;
			}
		}

		property String^ BindTreeViewData
		{
			void set(String^ treeviewData)
			{
				if (m_strTreeViewData == nullptr)
					m_strTreeViewData = treeviewData;
			}
		}

		property String^ BindListViewData
		{
			void set(String^ listviewData)
			{
				if (m_strListViewData == nullptr)
					m_strListViewData = listviewData;
			}
		}

	private:
		void SetNewNode(IXobj* pXobj)
		{
			if (m_pXobj != NULL)
			{
				m_pXobj = NULL;
			}

			if (pXobj != NULL)
			{
				m_pXobj = pXobj;
			}
		}

		void Init();

	public:
		//void SaveCosmosDoc(String^ m_strName);
		Object^ ActiveMethod(String^ strMethod, cli::array<Object^, 1>^ p);
		//[DispId(0x000001)]
		//[ComVisibleAttribute(true)]
		//Object^ ExecScript(String^ strScript);
		Xobj^ Observe(String^ layerName, String^ layerXML);
		Xobj^ ObserveChild(int rowNum, int colName, String^ layerName, String^ layerXML);

		property String^ Caption
		{
			String^ get();
			void set(String^ strCaption)
			{
				m_pXobj->put_Caption(STRING2BSTR(strCaption));
			}
		}

		[BrowsableAttribute(false)]
		property GalaxyCluster^ GalaxyCluster
		{
			Universe::GalaxyCluster^ get();
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

		[BrowsableAttribute(false)]
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
				catch (InvalidOleVariantTypeException ^ e)
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

		[BrowsableAttribute(false)]
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
				catch (ArgumentException ^ e)
				{
					e->Data->ToString();
				}
			}
		}

		property Object^ ActionData
		{
			Object^ get()
			{
				return actionData;
			}

			void set(Object^ obj)
			{
				actionData = obj;
			}
		}

		[BrowsableAttribute(false)]
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

		[BrowsableAttribute(false)]
		property Galaxy^ Galaxy
		{
			Universe::Galaxy^ get();
		}

		[BrowsableAttribute(false)]
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

		[BrowsableAttribute(false)]
		property Object^ PlugIn[String ^]
		{
			Object ^ get(String ^ strName);
		}

			[BrowsableAttribute(false)]
		property XobjCollection^ ChildNodes
		{
			XobjCollection^ get();
		}

		[BrowsableAttribute(false)]
		property XobjCollection^ Objects[XobjType]
		{
			XobjCollection ^ get(XobjType nValue)
			{
				CComPtr<IXobjCollection> pXobjs = NULL;
				m_pXobj->get_Objects((long)nValue,&pXobjs);
				return gcnew XobjCollection(pXobjs);
			}
		}

			[BrowsableAttribute(false)]
		property Xobj^ RootXobj
		{
			Xobj^ get()
			{
				CComPtr<IXobj> pRootXobj;
				m_pXobj->get_RootXobj(&pRootXobj);

				return theAppProxy._createObject<IXobj, Xobj>(pRootXobj);
			}
		}

		[BrowsableAttribute(false)]
		property Xobj^ ParentXobj
		{
			Xobj^ get()
			{
				CComPtr<IXobj> pRootXobj;
				m_pXobj->get_ParentXobj(&pRootXobj);

				return theAppProxy._createObject<IXobj, Xobj>(pRootXobj);
			}
		}

		[BrowsableAttribute(false)]
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

		[BrowsableAttribute(false)]
		property int Col
		{
			int get()
			{
				long nValue = 0;
				m_pXobj->get_Col(&nValue);
				return nValue;
			}
		}

		[BrowsableAttribute(false)]
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
				return type;
			}
		}

		[BrowsableAttribute(false)]
		property String^ Attribute[String ^]
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

	public:
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

		int GetXobjs(String^ strName, Xobj^% pXobj, XobjCollection^% pXobjCollection)
		{
			IXobjCollection* pXobjs = nullptr;
			IXobj* pTNode = nullptr;

			pXobj = nullptr;
			pXobjCollection = nullptr;

			long nCount;
			m_pXobj->GetXobjs(STRING2BSTR(strName),
				&pTNode, &pXobjs, &nCount);

			pXobj = theAppProxy._createObject<IXobj, Xobj>(pTNode);

			pXobjCollection = gcnew XobjCollection(pXobjs);

			return nCount;
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

		HWND m_hWnd;
		String^ m_strListViewData = nullptr;
		String^ m_strTreeViewData = nullptr;
		Object^ actionData;
		Xobj^ bindTreeNode = nullptr;
		Xobj^ bindListViewNode = nullptr;
		Dictionary<String^, MethodInfo^>^ m_pCosmosCLRMethodDic = nullptr;
		Dictionary<String^, Object^>^ m_pPlugInDic = nullptr;
		System::Void LoadNode(TreeNode^ pXobj, CTangramXmlParse* pParse);

		XobjCollection^ m_pChildNodes;
		void OnAfterSelect(System::Object^ sender, System::Windows::Forms::TreeViewEventArgs^ e);
		void OnNodeMouseDoubleClick(System::Object^ sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^ e);
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

	public:
		Xobj^ Observe(String^ layerName, String^ layerXML);

		void SendMessage(String^ strFrom, String^ strTo, String^ strMsgId, String^ strMsgContent, String^ strExtra);
		
		void Attach(bool bAttach)
		{
			if (m_pGalaxy)
			{
				if (bAttach)
				{
					m_pGalaxy->Attach();
				}
				else
				{
					m_pGalaxy->Detach();
				}
			}
		}

		property String^ Name
		{
			String^ get()
			{
				CComBSTR bstrName("");
				m_pGalaxy->get_Name(&bstrName);
				return BSTR2STRING(bstrName);
			}
		}

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
			Universe::Xobj ^ get()
			{
				IXobj* pXobj = nullptr;
				m_pGalaxy->get_VisibleXobj(&pXobj);
				return theAppProxy._createObject<IXobj, Universe::Xobj>(pXobj);
			}
		}

		property Xobj^ Xobj[Object ^]
		{
			Universe::Xobj ^ get(Object ^ index)
			{
				VARIANT var;
				Marshal::GetNativeVariantForObject(index,(System::IntPtr) & var);
				IXobj* pXobj = nullptr;
				m_pGalaxy->get_Xobj(var, &pXobj);
				return theAppProxy._createObject<IXobj, Universe::Xobj>(pXobj);
			}
		}

			property XobjCollection^ RootXobjs
		{
			XobjCollection^ get()
			{
				CComPtr<IXobjCollection> pXobjs = NULL;
				m_pGalaxy->get_RootXobjs(&pXobjs);

				return gcnew XobjCollection(pXobjs.p);
			}
		}

		property GalaxyCluster^ GalaxyCluster
		{
			Universe::GalaxyCluster^ get()
			{
				CComPtr<IGalaxyCluster> pGalaxyCluster = NULL;
				m_pGalaxy->get_GalaxyCluster(&pGalaxyCluster);

				return theAppProxy._createObject<IGalaxyCluster, ::Universe::GalaxyCluster>(pGalaxyCluster);
			}
		}
		private:
			IGalaxy* m_pGalaxy;
	};

	public ref class WpfApplication : public System::Windows::Application
	{
	public:
		WpfApplication();
		~WpfApplication() {};
	};

	public ref class Cosmos
	{
	public:
		Cosmos(ICosmos* pCosmos);
		~Cosmos();
	private:
		Cosmos();
		static bool IsAppInit = false;
		static bool IsWebRuntimeInit = false;

		static Cosmos^ m_pCosmos;
		static Dictionary<String^, MethodInfo^>^ m_pCosmosCLRMethodDic = gcnew Dictionary<String^, MethodInfo^>();
		static Dictionary<String^, Type^>^ m_pCosmosCLRTypeDic = gcnew Dictionary<String^, Type^>();
		static Dictionary<String^, Type^>^ m_pAppFormTypeDic = nullptr;
		static Dictionary<String^, Type^>^ m_pAppMDIFormTypeDic = nullptr;
		static Dictionary<Control^, String^>^ m_pControlRelationDic = nullptr;
		static Dictionary<Object^, Wormhole^>^ m_pWormholes = gcnew Dictionary<Object^, Wormhole^>();
		static Dictionary<String^, String^>^ CustomizeDictionary = gcnew Dictionary<String^, String^>();
		static Dictionary<String^, Object^>^ m_pCosmosCLRObjDic = gcnew Dictionary<String^, Object^>();
	public:
#ifndef _WIN64
		static IntPtr m_nHWebRuntimeToolWndPane = IntPtr::Zero;
		static Dictionary<String^, String^>^ replacementsDictionary = nullptr;
		static Control^ m_pWizForm = nullptr;
#endif
		static String^ m_strWizData = L"";
		static String^ m_strAppData = L"";
		static System::Drawing::Icon^ m_pDefaultIcon = nullptr;
		static Form^ m_pMainForm = nullptr;
		//static Dictionary<String^, TangramAppProxy^>^ m_pCosmosAppProxyDic = gcnew Dictionary<String^, TangramAppProxy^>();
		static Dictionary<Object^, Xobj^>^ m_pFrameworkElementDic = gcnew Dictionary<Object^, Xobj^>();

		static String^ ComputeHash(String^ source);
		static int CosmosInit(String^ strInit);
		static GalaxyCluster^ CreateGalaxyCluster(IntPtr nPageHandle);
		static GalaxyCluster^ CreateGalaxyCluster(Control^ ctrl, Object^ ExternalObj);
		static String^ GetUIData(Control^ ctrl);
		static String^ GetCtrlTag(Control^ ctrl);
		static Object^ CreateObject(String^ ObjID);
		static Form^ CreateForm(IWin32Window^ parent, String^ ObjID);
		static Type^ GetType(String^ ObjID);
		static Object^ ActiveMethod(String^ strObjID, String^ strMethod, cli::array<Object^, 1>^ p);
		static Object^ ActiveObjectMethod(Object^ pObj, String^ strMethod, cli::array<Object^, 1>^ p);
		static void StartApplication(String^ appID, String^ strXML);
		static WorkBenchWindow^ RemoteActiveWorkBenchWindow(String^ appID);
		static Control^ GetMDIClient(Form^ pForm);
		static Xobj^ ExtendMDIClient(Form^ pForm, String^ strKey, String^ strXml);
		static Dictionary<String^, Type^>^ InitAppFormTypeDic();
		static Dictionary<String^, Type^>^ GetFormTypesFromAssembly(String^ assemblyFilePath);
		static Dictionary<String^, Type^>^ GetFormTypesFromDirectory(String^ directoryPath);
		static List<String^>^ FindFiles(String^ rootPath, String^ fileSpec, bool recursive);
		static Browser^ ActiveBrowser();
		static Browser^ GetHostBrowser(Object^ obj);
		static Xobj^ GetXobjFromHandle(IntPtr handle);
		static Xobj^ GetXobjFromControl(Control^ ctrl);
		static void RegComponentForTangram(String^ strIDs, Assembly^ a);
		static void UpdateNewTabPageLayout(String^ newTabPageLayout);
		static void BindObjToWebPage(IntPtr hWebPage, Object^ pObj, String^ name);
		static Wormhole^ GetWormholeFromObj(Object^ obj);
		static Xobj^ Observe(Control^ ctrl, String^ key, String^ strXobjXml);

		Universe::ICosmosApp^ m_pUniverseAppProxy;

		static void SetControlRelation(Control^ ctrl, String^ strTypes);

		static void ExportAllCLRObjInfo();

		static Cosmos^ GetCosmos();

		static Cosmos^ InitCosmosApp(bool bSupportCrashReporting, CosmosAppType AppType);

		static bool WebRuntimeInit();
		static void InitEclipse();
		static void SendXmlMessage(Xobj^ sender, String^ strXml);

		static void Run();
		static void Run(Form^ Mainform);
		static void Run(ApplicationContext^ context);
		static void ExtractToDirectory(String^ strSRC, String^ strTarget);
		static Xobj^ GetXobjFromObj(Object ^ obj)
		{
			Xobj^ pXobj = nullptr;
			if (m_pFrameworkElementDic->TryGetValue(obj, pXobj))
			{
				return pXobj;
			}
			return nullptr;
		}

		static void CreateBKPage(Form^ form, String^ strID);

		static String^ GetObjAssemblyName(Object ^ obj)
		{
			if (obj == nullptr)
				return L"";
			Assembly ^ a = nullptr;
			String ^ strName = L"";
			try
			{
				a = Assembly::GetAssembly(obj->GetType());
			}
			catch (System::ArgumentNullException^)
			{

			}
			finally
			{
				if (a != nullptr)
				{
					strName = a->FullName;
					strName = strName->Substring(0, strName->IndexOf(","));
				}
			}
			return strName;
		}

		static String^ LoadCLRResource(Object ^ destObj, String ^ resourceName)
		{
			Assembly ^ a = nullptr;
			if (resourceName == L"" || resourceName == nullptr)
				resourceName = L"tangramresource.xml";
			String ^ strName = L"";
			try
			{
				if (destObj == nullptr)
					a = Assembly::GetExecutingAssembly();
				else
				{
					if (destObj->GetType()->IsSubclassOf(Assembly::typeid))
						a = (Assembly ^)destObj;
					else
						a = Assembly::GetAssembly(destObj->GetType());
				}
			}
			catch (ArgumentNullException^)
			{

			}
			finally
			{
				if (a != nullptr)
				{
					strName = a->FullName;
					strName = strName->Substring(0, strName->IndexOf(","));
				}
			}

			System::IO::Stream^ sm = nullptr;
			try
			{
				sm = a->GetManifestResourceStream(strName + "." + resourceName);
			}
			catch (...)
			{

			}
			finally
			{
				if (sm != nullptr)
				{
					cli::array<unsigned char,1>^ bs = gcnew cli::array<unsigned char,1>((int)sm->Length);
					sm->Read(bs, 0, (int)sm->Length);
					sm->Close();

					System::Text::UTF8Encoding^ con = gcnew System::Text::UTF8Encoding();

					strName = con->GetString(bs);
					delete sm;
				}
			}
			return strName;
		}

		static void BindToCtrl(Control^ source, Control^ target, String^ strXmlData);
		static void BindToCtrl(String^ ctrlID, String^ AssemblyQualifiedName, Control^ target, int nAnchor, String^ strXmlData);
		delegate void CosmosActionDelegate(Xobj^ SourceObj, String^ strInfo);
		static event CosmosActionDelegate^ OnCosmosActionDelegate;
		static void Fire_OnCosmosActionDelegate(Xobj^ SourceObj, String^ strInfo)
		{
			OnCosmosActionDelegate(SourceObj, strInfo);
		}

		static property Dictionary<String^, String^>^ CustomizeDic
		{
			Dictionary<String^, String^>^ get()
			{
				return CustomizeDictionary;
			};
		}

		static property Dictionary<Object^, Universe::Wormhole^>^ Wormholes
		{
			Dictionary<Object^, Universe::Wormhole^>^ get()
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

		static property System::Drawing::Icon^ DefaultIcon
		{
			System::Drawing::Icon^ get();
		}

		static property bool SupportCrashReporting
		{
			bool get();
			void set(bool bSupportCrashReporting);
		}

		static property CosmosAppType AppType
		{
			CosmosAppType get();
			void set(CosmosAppType nType);
		}

		static property Cosmos^ TangramCore
		{
			Cosmos^ get();
		}

		static property Xobj^ CreatingXobj
		{
			Xobj^ get();
		}

		static property Galaxy^ ActiveEclipseTopFrame
		{
			Galaxy^ get();
		}

		static Browser^ CreateBrowser(IntPtr ParentHandle, String^ strUrls);
		//static property Dictionary<String^, Type^>^ FormTypeDictionary
		//{
		//	Dictionary<String^, Type^>^ get()
		//	{
		//		if (Universe::Universe::m_pAppFormTypeDicEvent != nullptr)
		//			Universe::Universe::m_pAppFormTypeDicEvent->WaitOne();
		//		return m_pAppFormTypeDic;
		//	}
		//}

		//static property Dictionary<String^, Type^>^ MDIFormTypeDictionary
		//{
		//	Dictionary<String^, Type^>^ get()
		//	{
		//		if (Universe::Universe::m_pAppFormTypeDicEvent != nullptr)
		//			Universe::Universe::m_pAppFormTypeDicEvent->WaitOne();
		//		return m_pAppMDIFormTypeDic;
		//	}
		//}

		//static property Dictionary<String^, TangramAppProxy^>^ TangramAppProxyDictionary
		//{
		//	Dictionary<String^, TangramAppProxy^>^ get()
		//	{
		//		return m_pCosmosAppProxyDic;
		//	}
		//}

		static property Cosmos^ RemoteTangram[String ^]
		{
			Cosmos ^ get(String ^ strAppID);
		}

		static property WorkBenchWindow^ ActiveWorkBenchWindow
		{
			WorkBenchWindow^ get();
		}

		static property String^ AppCommonFormsPath
		{
			String^ get();
		}

		static property String^ AppFormsPath
		{
			String^ get();
		}

		static property String^ AppDataPath
		{
			String^ get();
		}

		static property String^ NTPXml
		{
			String^ get();
			void set(String^ strXml);
		}

		static property String^ WizData
		{
			String^ get();
			void set(String^ strXml);
		}

		delegate void Close();
		static event Close^ OnClose;
		static void Fire_OnClose();

		delegate void CloudAppIdle();
		static event CloudAppIdle^ OnCloudAppIdle;
		static void Fire_OnCloudAppIdle();

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

		delegate void EclipseAppInit();
		static event EclipseAppInit^ OnEclipseAppInit;
		static void Fire_OnEclipseAppInit();

		delegate System::Drawing::Icon^ GetAppIcon(int nIndex);
		static event GetAppIcon^ OnGetAppIcon;
		static System::Drawing::Icon^ Fire_OnGetAppIcon(int nIndex)
		{
			return OnGetAppIcon(nIndex);
		}

		delegate Object^ GetSubObjForWebPage(Object^ SourceObj, String^ subObjName);
		static event GetSubObjForWebPage^ OnGetSubObjForWebPage;
		static Object^ Fire_OnGetSubObjForWebPage(Object^ SourceObj, String^ subObjName);

		delegate void BindCLRObjToWebPage(Object^ SourceObj, Universe::Wormhole^ eventSession ,String^ eventName);
		static event BindCLRObjToWebPage^ OnBindCLRObjToWebPage;
		static void Fire_OnBindCLRObjToWebPage(Object^ SourceObj, Universe::Wormhole^ eventSession, String^ eventName);

		delegate void OpenComplete(IntPtr hWnd, String^ bstrUrl, Xobj^ pRootXobj);
		static event OpenComplete^ OnObserverComplete;
		static void Fire_OnObserverComplete(IntPtr hWnd, String^ bstrUrl, Xobj^ pRootXobj)
		{
			OnObserverComplete(hWnd, bstrUrl, pRootXobj);
		}

		delegate void AppMsgLoop(IntPtr hWnd, IntPtr msg, IntPtr wParam, IntPtr lParam);
		static event AppMsgLoop^ OnAppMsgLoop;
		static void Fire_OnAppMsgLoop(IntPtr hWnd, IntPtr msg, IntPtr wParam, IntPtr lParam)
		{
			OnAppMsgLoop(hWnd, msg, wParam, lParam);
		}

		delegate void TangramDebugDelegate(String^ strInfo);
		static event TangramDebugDelegate^ OnCosmosDebugDelegate;
		static void Fire_OnCosmosDebugDelegate(String^ strDebugInfo)
		{
			OnCosmosDebugDelegate(strDebugInfo);
		}

		delegate void CosmosMsg(IntPtr hWnd, String^ strType, String^ strParam1, String^ strParam2);
		static event CosmosMsg^ OnCosmosMsg;
		static void Fire_OnCosmosMsg(IntPtr hWnd, String^ strType, String^ strParam1, String^ strParam2)
		{
			OnCosmosMsg(hWnd, strType, strParam1, strParam2);
		}

		delegate void CosmosMsgReceived(Universe::Wormhole^ cloudSession);
		static event CosmosMsgReceived^ OnCosmosMsgReceived;
		static void Fire_OnCosmosMsgReceived(Universe::Wormhole^ cloudSession)
		{
			OnCosmosMsgReceived(cloudSession);
		}

		delegate void CustomizedDOMElement(IntPtr hWnd, String^ strRuleName, String^ strHTML);
		static event CustomizedDOMElement^ OnCustomizedDOMElement;
		static void Fire_OnCustomizedDOMElement(IntPtr hWnd, String^ strRuleName, String^ strHTML)
		{
			OnCustomizedDOMElement(hWnd, strRuleName, strHTML);
		}

		//delegate void TangramAppData(String^ strAppData);
		//static event TangramAppData^ OnCosmosAppData;
		//static void Fire_OnCosmosAppData(String^ strAppData)
		//{
		//	OnCosmosAppData(strAppData);
		//}

		delegate void FormNodeCreated(String^ bstrObjID, Form^ pForm, Xobj^ pXobj);
		static event FormNodeCreated^ OnFormNodeCreated;
		static void Fire_OnFormNodeCreated(String^ bstrObjID, Form^ pForm, Xobj^ pXobj)
		{
			OnFormNodeCreated(bstrObjID, pForm, pXobj);
		}

		property String^ AppKeyValue[String ^]
		{
			String ^ get(String ^ iIndex);
			void set(String^ iIndex, String^ newVal);
		}
};

	/// <summary>
	/// GalaxyCluster 
	/// </summary>
	//[ComSourceInterfacesAttribute(Universe::IManagerExtender::typeid)]
	public ref class GalaxyCluster : public IWin32Window, public Dictionary<String^, Galaxy^>
	{
	public:
		GalaxyCluster(void);
		GalaxyCluster(IGalaxyCluster* pGalaxyCluster);

		IGalaxyCluster* m_pGalaxyCluster;
		property IntPtr Handle
		{
			virtual IntPtr get()
			{
				LONGLONG h = 0;
				m_pGalaxyCluster->get_Handle(&h);
				return (IntPtr)h;
			};
		}

		property String^ GalaxyName[Control ^]
		{
			String ^ get(Control ^ ctrl)
			{
				if (ctrl != nullptr)
				{
					LONGLONG hWnd = ctrl->Handle.ToInt64();
					BSTR bstrName = ::SysAllocString(L"");
					m_pGalaxyCluster->get_GalaxyName(hWnd, &bstrName);
					String^ strRet = BSTR2STRING(bstrName);
					::SysFreeString(bstrName);
					return strRet;
				}
				return String::Empty;
			}
		}

		property String^ GalaxyName[IntPtr]
		{
			String ^ get(IntPtr handle)
			{
				if (::IsWindow((HWND)handle.ToInt64()))
				{
					BSTR bstrName = ::SysAllocString(L"");
					m_pGalaxyCluster->get_GalaxyName(handle.ToInt32(), &bstrName);
					String^ strRet = BSTR2STRING(bstrName);
					::SysFreeString(bstrName);
					return strRet;
				}
				return String::Empty;
			}
		}

		property Object^ Extender[String ^]
		{
			Object ^ get(String ^ strName)
			{
				BSTR bstrName = STRING2BSTR(strName);
				CComPtr<IDispatch> pDisp;
				m_pGalaxyCluster->get_Extender(bstrName, &pDisp);
				::SysFreeString(bstrName);
				return Marshal::GetObjectForIUnknown((IntPtr)pDisp.p);
			}

			void set(String ^ strName, Object ^ newObj)
			{
				IDispatch* pDisp = (IDispatch*)Marshal::GetIUnknownForObject(newObj).ToPointer();
				m_pGalaxyCluster->put_Extender(STRING2BSTR(strName), pDisp);
			}
		}

		property Galaxy^ Galaxys[Object ^]
		{
			Galaxy^ get(Object ^ obj)
			{
				CComVariant m_v;
				IntPtr handle = (IntPtr)& m_v;
				Marshal::GetNativeVariantForObject(obj, handle);
				CComPtr<IGalaxy> pGalaxy;
				m_pGalaxyCluster->get_Galaxy(m_v, &pGalaxy);
				if (pGalaxy)
				{
					return theAppProxy._createObject<IGalaxy, Galaxy>(pGalaxy);
				}
				return nullptr;
			}
		}

		Xobj^ GetXobj(String^ strName, String^ strGalaxyName);

		Galaxy^ CreateGalaxy(Control^ ctrl, String^ strName)
		{
			Galaxy^ pRetGalaxy = nullptr;
			if (ctrl != nullptr && String::IsNullOrEmpty(strName) == false)
			{
				if (TryGetValue(strName, pRetGalaxy))
				{
					return pRetGalaxy;
				}
				CComPtr<IGalaxy> pGalaxy;
				m_pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((LONGLONG)ctrl->Handle.ToInt64()), STRING2BSTR(strName), &pGalaxy);
				if (pGalaxy)
				{
					pRetGalaxy = theAppProxy._createObject<IGalaxy, Galaxy>(pGalaxy);
					if (pRetGalaxy != nullptr)
						Add(strName, pRetGalaxy);
				}
			}
			return pRetGalaxy;
		}

		Galaxy^ CreateGalaxy(IntPtr handle, String ^ strName)
		{
			Galaxy^ pRetGalaxy = nullptr;
			if (::IsWindow((HWND)handle.ToInt64()) && String::IsNullOrEmpty(strName) == false)
			{
				if (TryGetValue(strName, pRetGalaxy))
				{
					return pRetGalaxy;
				}
				CComPtr<IGalaxy> pGalaxy;
				m_pGalaxyCluster->CreateGalaxy(CComVariant(0), CComVariant((LONGLONG)handle.ToInt64()), STRING2BSTR(strName), &pGalaxy);
				if (pGalaxy)
				{
					pRetGalaxy = theAppProxy._createObject<IGalaxy, Galaxy>(pGalaxy);
					if (pRetGalaxy != nullptr)
						Add(strName, pRetGalaxy);
				}
			}
			return pRetGalaxy;
		}

		void ObserveGalaxys(String ^ strFrames, String ^ strKey, String ^ bstrXml, bool bSaveToConfigFile);

		delegate void DocumentComplete(GalaxyCluster^ sender, Object ^ pHtmlDoc, String ^ strURL);
		event DocumentComplete^ OnDocumentComplete;
		void Fire_OnDocumentComplete(GalaxyCluster^ sender, Object ^ pHtmlDoc, String ^ strURL)
		{
			OnDocumentComplete(sender, pHtmlDoc, strURL);
		}

		delegate void IPCMsg(Galaxy^ sender, String ^ strType, String ^ strContent, String ^ strFeature);
		event IPCMsg^ OnIPCMsg;
		void Fire_OnIPCMsg(Galaxy^ sender, String ^ strType, String ^ strContent, String ^ strFeature)
		{
			OnIPCMsg(sender, strType, strContent, strFeature);
		}

		delegate void Destroy(Xobj ^ sender);
		event Destroy^ OnDestroy;
		void Fire_OnDestroy(Xobj ^ sender)
		{
			OnDestroy(sender);
		}

		delegate void TabChange(Xobj ^ sender, int nActivePage, int nOldActivePage);
		event TabChange^ OnTabChange;
		void Fire_OnTabChange(Xobj ^ sender, int nActivePage, int nOldActivePage)
		{
			OnTabChange(sender, nActivePage, nOldActivePage);
		}

		delegate void GalaxyClusterLoad(GalaxyCluster^ sender);
		event GalaxyClusterLoad^ OnGalaxyClusterLoad;
		void Fire_OnGalaxyClusterLoad(GalaxyCluster^ sender)
		{
			OnGalaxyClusterLoad(sender);
		}

		String^ GetPageXML();

	protected:
		CGalaxyClusterEvent* m_pCosmosClrEvent;

		~GalaxyCluster();
	};

	///// <summary>
	///// Summary for TangramApplicationContext
	///// </summary>
	//public ref class TangramAppContext : public ApplicationContext
	//{
	//public:
	//	TangramAppContext();
	//};

	public ref class EclipseView
	{
	public:
		IEclipseCtrl* m_pCtrl;
		EclipseView(IEclipseCtrl* pCtrl)
		{
			m_pCtrl = pCtrl;
		};

		property IntPtr Handle
		{
			virtual IntPtr get()
			{
				LONGLONG h = 0;
				m_pCtrl->get_HWND(&h);
				return (IntPtr)h;
			};
		}

		Xobj^ Observe(String^ strGalaxyName, String^ strKey, String^ strXml)
		{
			if (m_pCtrl)
			{
				IXobj* pXobj = nullptr;
				m_pCtrl->Observe(STRING2BSTR(strGalaxyName), STRING2BSTR(strKey), STRING2BSTR(strXml), &pXobj);
				if (pXobj)
				{
					return theAppProxy._createObject<IXobj, Xobj>(pXobj);
				}
			}
			return nullptr;
		}
	};

	public ref class WorkBenchWindow : public System::Collections::IEnumerable
	{
	public:
		WorkBenchWindow(IWorkBenchWindow* pWorkBenchWindow)
		{
			m_pWorkBenchWindow = pWorkBenchWindow;
			//SetNewNodeCollection(pXobjs);
		};

		~WorkBenchWindow()
		{
			//SetNewNodeCollection(NULL);
		}

		void Active()
		{
			if (m_pWorkBenchWindow)
			{
				LONGLONG h = 0;
				m_pWorkBenchWindow->get_Handle(&h);
				HWND hwnd = (HWND)h;
				hwnd = ::GetAncestor(hwnd, GA_ROOT);
				::SetFocus(hwnd);
			}
		}

		Xobj^ SWTExtend(String^ strKey, String^ strXml)
		{
			if (m_pWorkBenchWindow)
			{
				IXobj* pXobj = nullptr;
				m_pWorkBenchWindow->Observe(STRING2BSTR(strKey), STRING2BSTR(strXml), &pXobj);
				if (pXobj)
					return theAppProxy._createObject<IXobj, Xobj>(pXobj);
			}
			return nullptr;
		}

		Xobj^ SWTExtendInView(int nView, String^ strKey, String^ strXml)
		{
			if (m_pWorkBenchWindow)
			{
				IEclipseCtrl* pCtrl = nullptr;
				m_pWorkBenchWindow->get_Ctrl(CComVariant((int)nView), &pCtrl);
				if (pCtrl)
				{
					IXobj* pXobj = nullptr;
					pCtrl->Observe(CComBSTR(L"EclipseView"), STRING2BSTR(strKey), STRING2BSTR(strXml), &pXobj);
					if (pXobj)
						return theAppProxy._createObject<IXobj, Xobj>(pXobj);
				}

			}
			return nullptr;
		}

	private:
		IWorkBenchWindow* m_pWorkBenchWindow;

	public:
		virtual System::Collections::IEnumerator^ GetEnumerator()
		{
			return gcnew TangramBaseEnumerator<WorkBenchWindow>(this, NodeCount);
		}

		void CloseTangramUI()
		{
			if (m_pWorkBenchWindow)
				m_pWorkBenchWindow->CloseTangramUI();
		}

		property EclipseView^ default[int]
		{
			EclipseView ^ get(int iIndex)
			{
				if (m_pWorkBenchWindow)
				{
					long nCount = 0;
					m_pWorkBenchWindow->get_Count(&nCount);
					if (iIndex >= nCount)
						return nullptr;
					else
					{
						IEclipseCtrl* pCtrl = nullptr;
						m_pWorkBenchWindow->get_Ctrl(CComVariant((long)iIndex), &pCtrl);
						if (pCtrl)
						{
							return theAppProxy._createObject<IEclipseCtrl, EclipseView>(pCtrl);
						}
					}
				}
				return nullptr;
			};
		}

			property int NodeCount
		{
			int get()
			{
				long n = 0;
				m_pWorkBenchWindow->get_Count(&n);
				return n;
			}
		}
	};

	//public ref class TangramHwndHost : public System::Windows::Interop::HwndHost
	//{
	//public:
	//	TangramHwndHost()
	//	{
	//	};

	//	TangramHwndHost(double height, double width)
	//	{
	//		hostHeight = (int)height;
	//		hostWidth = (int)width;
	//	};

	//	virtual ~TangramHwndHost()
	//	{
	//	};

	//	int hostHeight, hostWidth;

	//	IntPtr hwndControl;
	//	IntPtr hwndHost;

	//protected:
	//	HandleRef^ BuildWindowCore(HandleRef^ hwndParent) /*override*/
	//	{
	//		hwndControl = IntPtr::Zero;
	//		hwndHost = IntPtr::Zero;
	//		return gcnew HandleRef(this, hwndHost);
	//	}		
	//	
	//	IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, bool% handled) override
	//	{
	//		handled = false;
	//		return IntPtr::Zero;
	//	}

	//	void DestroyWindowCore(HandleRef^ hwnd) /*override*/
	//	{
	//		::DestroyWindow((HWND)hwnd->Handle.ToPointer());
	//	}
	//};

	public interface class ICosmosEventObjProxy
	{
	};

	public interface class ICosmosWndXobjProxy
	{
		virtual void OnObserverComplete();
		virtual void OnDestroy();
		virtual void OnXobjAddInCreated(Object^ pAddIndisp, String^ bstrAddInID, String^ bstrAddInXml);
		virtual void OnXobjAddInsCreated();
		virtual void OnXobjDocumentComplete(Object^ ExtenderDisp, String^ bstrURL);
		virtual void OnControlNotify(Xobj^ sender, LONG NotifyCode, LONG CtrlID, IntPtr CtrlHandle, String^ CtrlClassName);
		virtual void OnTabChange(LONG ActivePage, LONG OldPage);
	};

	public interface class IGalaxyClusterProxy
	{
		virtual void OnGalaxyClusterLoaded(GalaxyCluster^ sender, String^ url);
		virtual void OnXobjCreated(Xobj^ pXobjCreated);
		virtual void OnAddInCreated(Xobj^ pRootXobj, Object^ pAddIn, String^ strID, String^ strAddInXml);
		virtual void OnBeforeOpenXml(String^ bstrXml, IntPtr hWnd);
		virtual void OnOpenXmlComplete(String^ bstrXml, IntPtr hWnd, Xobj^ pRetRootNode);
		virtual void OnDestroy();
		virtual void OnNodeMouseActivate(Xobj^ pActiveNode);
		virtual void OnClrControlCreated(Xobj^ Node, Control^ Ctrl, String^ CtrlName, IntPtr CtrlHandle);
		virtual void OnTabChange(Xobj^ sender, LONG ActivePage, LONG OldPage);
		virtual void OnControlNotify(Xobj^ sender, LONG NotifyCode, LONG CtrlID, IntPtr CtrlHandle, String^ CtrlClassName);
		virtual void OnCosmosEvent(ICosmosEventObjProxy^ NotifyObj);
	};

	public interface class IGalaxyProxy
	{
		virtual void OnExtend(Xobj^ pRetNode, String^ bstrKey, String^ bstrXml);
	};

	public interface class IVSProxy
	{
		virtual void OnConnect(String^ bstrKey, IntPtr nHandle);
	};

	public interface class ICosmosApp
	{
		[DispId(0x000001)]
		virtual void TangramClose();
		[DispId(0x000002)]
		virtual ICosmosWndXobjProxy^ OnXobjInit(Xobj^ pNewNode);
		[DispId(0x000003)]
		virtual IGalaxyProxy^ OnGalaxyCreated(Galaxy^ pNewFrame);
		[DispId(0x000004)]
		virtual IGalaxyClusterProxy^ OnGalaxyClusterCreated(GalaxyCluster^ pNewPage);
		[DispId(0x000005)]
		virtual void OnObserverComplete(IntPtr hWnd, String^ bstrUrl, Xobj^ pRootXobj);
		[DispId(0x000006)]
		virtual void OnCosmosEvent(ICosmosEventObjProxy^ NotifyObj);
	};
}

/********************************************************************************
*					Open Universe - version 0.0.1								*
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

class CStarCLREvent;
class CCosmosNodeEvent;
extern CCosmosProxy theAppProxy;

namespace Cosmos
{
	/// <summary>
	/// Summary for Hubble
	/// </summary>

	ref class Star;
	ref class Quasar;
	ref class Herschel;
	ref class GalaxyCluster;

	public enum class CosmosAppType
	{
		APPWIN32 = APP_WIN32,
		APPBROWSER = APP_BROWSER,
		APPECLIPSE = APP_ECLIPSE,
		APPBROWSER_ECLIPSE = APP_BROWSER_ECLIPSE,
		APPBROWSERAPP = APP_BROWSERAPP,
		APPOTHER = 0
	};

	public ref class Star
	{
	public:
		Star(IStar* pNode);
		~Star();

		IStar* m_pStar;
		Object^ m_pHostObj = nullptr;
		CCosmosNodeEvent* m_pStarEvent;
		CStarCLREvent* m_pStarCLREvent;
		Cosmos::Wormhole^ m_pWormhole = nullptr;
		CHerschelImpl* m_pChromeBrowserProxy;

		delegate void NodeAddInCreated(Star^ sender, Object^ pAddIndisp, String^ bstrAddInID, String^ bstrAddInXml);
		event NodeAddInCreated^ OnNodeAddInCreated;
		void Fire_NodeAddInCreated(Star^ sender, Object^ pAddIndisp, String^ bstrAddInID, String^ bstrAddInXml)
		{
			OnNodeAddInCreated(sender, pAddIndisp, bstrAddInID, bstrAddInXml);
		}

		delegate void NodeAddInsCreated(Star^ sender);
		event NodeAddInsCreated^ OnNodeAddInsCreated;
		void Fire_NodeAddInsCreated(Star^ sender)
		{
			OnNodeAddInsCreated(sender);
		}

		// A notification has been created for all nodes in the current layout.
		delegate void RootNodeCreated(IntPtr nHandle, String^ strUrl, Star^ pRootNode);
		event RootNodeCreated^ OnRootNodeCreated;
		void Fire_RootNodeCreated(IntPtr nHandle, String^ strUrl, Star^ pRootNode)
		{
			OnRootNodeCreated(nHandle, strUrl, pRootNode);
		}

		delegate void OpenComplete(Star^ sender);
		event OpenComplete^ OnOpenComplete;
		void Fire_OpenComplete(Star^ sender)
		{
			OnOpenComplete(sender);
		}

		delegate void Destroy(Star^ sender);
		event Destroy^ OnDestroy;
		void Fire_OnDestroy(Star^ sender)
		{
			OnDestroy(sender);
		}

		delegate void DocumentComplete(Star^ sender, Object^ pHtmlDoc, String^ strURL);
		event DocumentComplete^ OnDocumentComplete;
		void Fire_OnDocumentComplete(Star^ sender, Object^ pHtmlDoc, String^ strURL)
		{
			OnDocumentComplete(sender, pHtmlDoc, strURL);
		}

		delegate void TabChange(int nActivePage, int nOldActivePage);
		event TabChange^ OnTabChange;
		void Fire_OnTabChange(int nActivePage, int nOldActivePage);

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

		delegate void BindCLRObjToWebPage(Object^ SourceObj, Cosmos::Wormhole^ eventSession, String^ eventName);
		event BindCLRObjToWebPage^ OnBindCLRObjToWebPage;
		void Fire_OnBindCLRObjToWebPage(Object^ SourceObj, Cosmos::Wormhole^ eventSession, String^ eventName)
		{
			OnBindCLRObjToWebPage(SourceObj, eventSession, eventName);
		}

		String^ SendIPCMessage(String^ strTo, String^ strPayload, String^ strExtra, String^ strMsgId)
		{
			BSTR bstrRet;
			HRESULT hr = m_pStar->SendIPCMessage(STRING2BSTR(strTo), STRING2BSTR(strPayload), STRING2BSTR(strExtra), STRING2BSTR(strMsgId), &bstrRet);
			if (hr == S_OK)
			{
				return BSTR2STRING(bstrRet);
			}
			return "";
		}

		property Wormhole^ Wormhole
		{
			Cosmos::Wormhole^ get();
		}

	public:
		void Init();
		Star^ Observe(String^ layerName, String^ layerXML);
		Star^ ObserveChild(int rowNum, int colName, String^ layerName, String^ layerXML);

		property String^ Caption
		{
			String^ get();
			void set(String^ strCaption)
			{
				m_pStar->put_Caption(STRING2BSTR(strCaption));
			}
		}

		[BrowsableAttribute(false)]
		property GalaxyCluster^ GalaxyCluster
		{
			Cosmos::GalaxyCluster^ get();
		}

		property String^ Name
		{
			String^ get()
			{
				if (m_pStar)
				{
					CComBSTR bstrCap("");
					m_pStar->get_Name(&bstrCap);
					return BSTR2STRING(bstrCap);
				}
				return "";
			}

			void set(String^ newVal)
			{
				if (m_pStar)
				{
					m_pStar->put_Name(STRING2BSTR(newVal));
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
				m_pStar->get_XObject(&var);

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
				m_pStar->get_rgbMiddle(&rgb);
				return System::Drawing::ColorTranslator::FromOle(rgb);
			}
			void set(System::Drawing::Color newVal)
			{
				m_pStar->put_rgbMiddle(System::Drawing::ColorTranslator::ToOle(newVal));
			}
		}

		property System::Drawing::Color rgbLeftTop
		{
			System::Drawing::Color get()
			{
				OLE_COLOR rgb;
				m_pStar->get_rgbLeftTop(&rgb);
				return System::Drawing::ColorTranslator::FromOle(rgb);
			}
			void set(System::Drawing::Color newVal)
			{
				m_pStar->put_rgbLeftTop(System::Drawing::ColorTranslator::ToOle(newVal));
			}
		}

		property System::Drawing::Color rgbRightBottom
		{
			System::Drawing::Color get()
			{
				OLE_COLOR rgb;
				m_pStar->get_rgbRightBottom(&rgb);
				return System::Drawing::ColorTranslator::FromOle(rgb);
			}
			void set(System::Drawing::Color newVal)
			{
				m_pStar->put_rgbRightBottom(System::Drawing::ColorTranslator::ToOle(newVal));
			}
		}

		[BrowsableAttribute(false)]
		property Object^ Extender
		{
			Object^ get()
			{
				Object^ pRetObject = nullptr;
				IDispatch* pDisp = NULL;
				m_pStar->get_Extender(&pDisp);
				if (pDisp)
					pRetObject = Marshal::GetObjectForIUnknown((IntPtr)pDisp);
				return pRetObject;
			}
			void set(Object^ obj)
			{
				IDispatch* pDisp = (IDispatch*)Marshal::GetIUnknownForObject(obj).ToPointer();
				m_pStar->put_Extender(pDisp);
			}
		}

		property Object^ Tag
		{
			Object^ get()
			{
				Object^ pRetObject = nullptr;
				VARIANT var;
				m_pStar->get_Tag(&var);
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
					m_pStar->put_Tag(var);
				}
				catch (ArgumentException^ e)
				{
					e->Data->ToString();
				}
			}
		}

		[BrowsableAttribute(false)]
		property int ActivePage
		{
			int get()
			{
				int nPage = 0;
				m_pStar->get_ActivePage(&nPage);

				return nPage;
			}

			void set(int nPage)
			{
				m_pStar->put_ActivePage(nPage);
			}
		}

		[BrowsableAttribute(false)]
		property Quasar^ Quasar
		{
			Cosmos::Quasar^ get();
		}

		[BrowsableAttribute(false)]
		property Cosmos::Quasar^ HostFrame
		{
			Cosmos::Quasar^ get()
			{
				CComPtr<IQuasar> pTangramFrame;
				m_pStar->get_HostQuasar(&pTangramFrame);
				if (pTangramFrame)
				{
					Cosmos::Quasar^ pQuasar = theAppProxy._createObject<IQuasar, Cosmos::Quasar>(pTangramFrame);
					return pQuasar;
				}
				return nullptr;
			}
		}

		[BrowsableAttribute(false)]
		property Object^ PlugIn[String^]
		{
			Object ^ get(String ^ strName);
		}

		[BrowsableAttribute(false)]
		property Star^ RootStar
		{
			Star^ get()
			{
				CComPtr<IStar> pRootNode;
				m_pStar->get_RootStar(&pRootNode);

				return theAppProxy._createObject<IStar, Star>(pRootNode);
			}
		}

		[BrowsableAttribute(false)]
		property Star^ ParentStar
		{
			Star^ get()
			{
				CComPtr<IStar> pNode;
				m_pStar->get_ParentStar(&pNode);

				return theAppProxy._createObject<IStar, Star>(pNode);
			}
		}

		[BrowsableAttribute(false)]
		property Star^ HostStar
		{
			Star^ get()
			{
				IStar* pNode = nullptr;
				m_pStar->get_HostStar(&pNode);
				if (pNode == NULL)
					return nullptr;
				return theAppProxy._createObject<IStar, Star>(pNode);
			}
		}

		[BrowsableAttribute(false)]
		property int Col
		{
			int get()
			{
				long nValue = 0;
				m_pStar->get_Col(&nValue);
				return nValue;
			}
		}

		[BrowsableAttribute(false)]
		property int Row
		{
			int get()
			{
				long nValue = 0;
				m_pStar->get_Row(&nValue);
				return nValue;
			}
		}

		property int Hmin
		{
			int get()
			{
				int nValue = 0;
				m_pStar->get_Hmin(&nValue);
				return nValue;
			}
			void set(int newVal)
			{
				m_pStar->put_Hmin(newVal);
			}
		}

		property int Hmax
		{
			int get()
			{
				int nValue = 0;
				m_pStar->get_Hmax(&nValue);
				return nValue;
			}
			void set(int newVal)
			{
				m_pStar->put_Hmax(newVal);
			}
		}

		property int Vmin
		{
			int get()
			{
				int nValue = 0;
				m_pStar->get_Vmin(&nValue);
				return nValue;
			}
			void set(int newVal)
			{
				m_pStar->put_Vmin(newVal);
			}
		}

		property int Vmax
		{
			int get()
			{
				int nValue = 0;
				m_pStar->get_Vmax(&nValue);
				return nValue;
			}
			void set(int newVal)
			{
				m_pStar->put_Vmax(newVal);
			}
		}

		property int Rows
		{
			int get()
			{
				long nValue = 0;
				m_pStar->get_Rows(&nValue);
				return nValue;
			}
		}

		property int Cols
		{
			int get()
			{
				long nValue = 0;
				m_pStar->get_Cols(&nValue);
				return nValue;
			}
		}

		property ::StarType StarType
		{
			::StarType get()
			{
				::StarType type;
				m_pStar->get_StarType(&type);
				return (::StarType)type;
			}
		}

		property __int64 Handle
		{
			__int64 get()
			{
				if (m_hWnd)
					return (__int64)m_hWnd;
				__int64 h = 0;
				m_pStar->get_Handle(&h);
				m_hWnd = (HWND)h;
				return h;
			}
		}

		[BrowsableAttribute(false)]
		property String^ Attribute[String^]
		{
			String ^ get(String ^ strKey)
			{
				BSTR bstrVal;
				m_pStar->get_Attribute(STRING2BSTR(strKey),&bstrVal);

				return BSTR2STRING(bstrVal);
			}

			void set(String ^ strKey, String ^ strVal)
			{
				m_pStar->put_Attribute(
					STRING2BSTR(strKey),
					STRING2BSTR(strVal));
			}
		}

	public:
		Star^ GetStar(int nRow, int nCol)
		{
			IStar* pNode;
			m_pStar->GetStar(nRow, nCol, &pNode);
			return theAppProxy._createObject<IStar, Star>(pNode);
		}

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>

		HWND m_hWnd;
		Dictionary<String^, MethodInfo^>^ m_pTangramCLRMethodDic = nullptr;
		Dictionary<String^, Object^>^ m_pPlugInDic = nullptr;
	};

	public ref class Quasar : public Dictionary<String^, Star^>
	{
	public:
		Quasar(IQuasar* pQuasar)
		{
			m_pQuasar = pQuasar;
		}
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Quasar()
		{
			LONGLONG nValue = (LONGLONG)m_pQuasar;
			theAppProxy._removeObject(nValue);
		}

		IQuasar* m_pQuasar;

	public:
		Star^ Observe(String^ layerName, String^ layerXML);

		void SendMessage(String^ strFrom, String^ strTo, String^ strMsgId, String^ strMsgContent, String^ strExtra);

		void Attach(bool bAttach)
		{
			if (m_pQuasar)
			{
				if (bAttach)
				{
					m_pQuasar->Attach();
				}
				else
				{
					m_pQuasar->Detach();
				}
			}
		}

		property Object^ FrameData[String^]
		{
			Object ^ get(String ^ iIndex);
			void set(String^ iIndex, Object^ newVal);
		}

			property String^ Name
		{
			String^ get()
			{
				CComBSTR bstrName("");
				m_pQuasar->get_Name(&bstrName);
				return BSTR2STRING(bstrName);
			}
		}

		property IntPtr Handle
		{
			IntPtr get()
			{
				__int64 nHandle;
				m_pQuasar->get_HWND(&nHandle);
				return (IntPtr)nHandle;
			}
		}

		property Star^ WndStar[Object^]
		{
			Star ^ get(Object ^ index)
			{
				VARIANT var;
				Marshal::GetNativeVariantForObject(index,(System::IntPtr) & var);
				IStar* pNode = nullptr;
				m_pQuasar->get_Star(var, &pNode);
				return theAppProxy._createObject<IStar, Star>(pNode);
			}
		}

		property GalaxyCluster^ Page
		{
			GalaxyCluster^ get()
			{
				CComPtr<IGalaxyCluster> pGalaxyCluster = NULL;
				m_pQuasar->get_GalaxyCluster(&pGalaxyCluster);

				return theAppProxy._createObject<IGalaxyCluster, GalaxyCluster>(pGalaxyCluster);
			}
		}
	};

	public ref class Hubble
	{
	public:
		Hubble(IHubble* pHubble);
		~Hubble();
	private:
		Hubble();
		static bool IsAppInit = false;
		static bool IsChromeRunning = false;

		static Hubble^ m_pHubble;
		static Dictionary<String^, MethodInfo^>^ m_pTangramCLRMethodDic = gcnew Dictionary<String^, MethodInfo^>();
		static Dictionary<String^, Type^>^ m_pTangramCLRTypeDic = gcnew Dictionary<String^, Type^>();
		static Dictionary<Object^, Wormhole^>^ m_pCloudEventDic = gcnew Dictionary<Object^, Wormhole^>();
	public:
		static int HubbleInit(String^ strInit);
		static System::Drawing::Icon^ m_pDefaultIcon = nullptr;
		static Form^ m_pMainForm = nullptr;
		static Dictionary<String^, Object^>^ m_pTangramCLRObjDic = gcnew Dictionary<String^, Object^>();
		static Dictionary<Object^, Star^>^ m_pFrameworkElementDic = gcnew Dictionary<Object^, Star^>();

		static GalaxyCluster^ CreateGalaxyCluster(IntPtr nPageHandle);
		static GalaxyCluster^ CreateGalaxyCluster(Control^ ctrl, Object^ ExternalObj);
		static Object^ CreateObject(String^ ObjID);
		static Form^ CreateForm(IWin32Window^ parent, String^ ObjID);
		static Type^ GetType(String^ ObjID);
		static Object^ ActiveMethod(String^ strObjID, String^ strMethod, cli::array<Object^, 1>^ p);
		static Object^ ActiveObjectMethod(Object^ pObj, String^ strMethod, cli::array<Object^, 1>^ p);
		static Control^ GetMDIClient(Form^ pForm);
		static Star^ ExtendMDIClient(Form^ pForm, String^ strKey, String^ strXml);
		static Herschel^ ActiveBrowser();
		static Herschel^ GetHostBrowser(Object^ obj);
		static Star^ GetStarFromHandle(IntPtr handle);
		static Star^ GetNodeFromControl(Control^ ctrl);
		//static void RegComponentForTangram(String^ strIDs, Assembly^ a);
		static void UpdateNewTabPageLayout(String^ newTabPageLayout);
		static void BindObjToWebPage(IntPtr hWebPage, Object^ pObj, String^ name);

		static Hubble^ GetHubble();

		static Hubble^ InitTangramApp(bool bSupportCrashReporting, CosmosAppType AppType);

		static bool WebRuntimeInit();

		static void Run();
		static void Run(Form^ Mainform);
		static void Run(ApplicationContext^ context);
		static Star^ GetStarFromObj(Object^ obj)
		{
			Star^ pNode = nullptr;
			if (m_pFrameworkElementDic->TryGetValue(obj, pNode))
			{
				return pNode;
			}
			return nullptr;
		}


		static String^ GetObjAssemblyName(Object^ obj)
		{
			if (obj == nullptr)
				return L"";
			Assembly^ a = nullptr;
			String^ strName = L"";
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

		static String^ LoadCLRResource(Object^ destObj, String^ resourceName)
		{
			Assembly^ a = nullptr;
			if (resourceName == L"" || resourceName == nullptr)
				resourceName = L"tangramresource.xml";
			String^ strName = L"";
			try
			{
				if (destObj == nullptr)
					a = Assembly::GetExecutingAssembly();
				else
				{
					if (destObj->GetType()->IsSubclassOf(Assembly::typeid))
						a = (Assembly^)destObj;
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
					cli::array<byte, 1>^ bs = gcnew cli::array<byte, 1>((int)sm->Length);
					sm->Read(bs, 0, (int)sm->Length);
					sm->Close();

					System::Text::UTF8Encoding^ con = gcnew System::Text::UTF8Encoding();

					strName = con->GetString(bs);
					delete sm;
				}
			}
			return strName;
		}

		delegate void TangramActionDelegate(Star^ SourceObj, String^ strInfo);
		static event TangramActionDelegate^ OnTangramActionDelegate;
		static void Fire_OnTangramActionDelegate(Star^ SourceObj, String^ strInfo)
		{
			OnTangramActionDelegate(SourceObj, strInfo);
		}

		static property Dictionary<Object^, Cosmos::Wormhole^>^ WebBindEventDic
		{
			Dictionary<Object^, Cosmos::Wormhole^>^ get()
			{
				return m_pCloudEventDic;
			};
		}

		static property Form^ MainForm
		{
			Form^ get();
			void set(Form^ frm);
		}

		static property Herschel^ HostWebBrowser
		{
			Herschel^ get();
		}

		static property Object^ Application
		{
			Object^ get();
			void set(Object^ obj);
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

		static property String^ CurrentDesigningTangramXml
		{
			String^ get();
		}

		static property Star^ CreatingStar
		{
			Star^ get();
		}

		static Herschel^ CreateBrowser(IntPtr ParentHandle, String^ strUrls);

		static property String^ AppDataPath
		{
			String^ get();
		}

		static property String^ NTPXml
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

		delegate System::Drawing::Icon^ GetAppIcon(int nIndex);
		static event GetAppIcon^ OnGetAppIcon;
		static System::Drawing::Icon^ Fire_OnGetAppIcon(int nIndex)
		{
			return OnGetAppIcon(nIndex);
		}

		delegate Object^ GetSubObjForWebPage(Object^ SourceObj, String^ subObjName);
		static event GetSubObjForWebPage^ OnGetSubObjForWebPage;
		static Object^ Fire_OnGetSubObjForWebPage(Object^ SourceObj, String^ subObjName);

		delegate void BindCLRObjToWebPage(Object^ SourceObj, Cosmos::Wormhole^ eventSession, String^ eventName);
		static event BindCLRObjToWebPage^ OnBindCLRObjToWebPage;
		static void Fire_OnBindCLRObjToWebPage(Object^ SourceObj, Cosmos::Wormhole^ eventSession, String^ eventName);

		delegate void OpenComplete(IntPtr hWnd, String^ bstrUrl, Star^ pRootNode);
		static event OpenComplete^ OnOpenComplete;
		static void Fire_OnOpenComplete(IntPtr hWnd, String^ bstrUrl, Star^ pRootNode)
		{
			OnOpenComplete(hWnd, bstrUrl, pRootNode);
		}

		delegate void AppMsgLoop(IntPtr hWnd, IntPtr msg, IntPtr wParam, IntPtr lParam);
		static event AppMsgLoop^ OnAppMsgLoop;
		static void Fire_OnAppMsgLoop(IntPtr hWnd, IntPtr msg, IntPtr wParam, IntPtr lParam)
		{
			OnAppMsgLoop(hWnd, msg, wParam, lParam);
		}

		delegate void HubbleMsg(IntPtr hWnd, String^ strType, String^ strParam1, String^ strParam2);
		static event HubbleMsg^ OnHubbleMsg;
		static void Fire_OnHubbleMsg(IntPtr hWnd, String^ strType, String^ strParam1, String^ strParam2)
		{
			OnHubbleMsg(hWnd, strType, strParam1, strParam2);
		}

		delegate void HubbleMsgReceived(Cosmos::Wormhole^ cloudSession);
		static event HubbleMsgReceived^ OnHubbleMsgReceived;
		static void Fire_OnHubbleMsgReceived(Cosmos::Wormhole^ cloudSession)
		{
			OnHubbleMsgReceived(cloudSession);
		}

		delegate void RenderHTMLElement(IntPtr hWnd, String^ strRuleName, String^ strHTML);
		static event RenderHTMLElement^ OnRenderHTMLElement;
		static void Fire_OnRenderHTMLElement(IntPtr hWnd, String^ strRuleName, String^ strHTML)
		{
			OnRenderHTMLElement(hWnd, strRuleName, strHTML);
		}

		//delegate void TangramAppData(String^ strAppData);
		//static event TangramAppData^ OnTangramAppData;
		//static void Fire_OnTangramAppData(String^ strAppData)
		//{
		//	OnTangramAppData(strAppData);
		//}

		delegate void FormNodeCreated(String^ bstrObjID, Form^ pForm, Star^ pNode);
		static event FormNodeCreated^ OnFormNodeCreated;
		static void Fire_OnFormNodeCreated(String^ bstrObjID, Form^ pForm, Star^ pNode)
		{
			OnFormNodeCreated(bstrObjID, pForm, pNode);
		}

		property String^ AppKeyValue[String^]
		{
			String ^ get(String ^ iIndex);
			void set(String^ iIndex, String^ newVal);
		}
	};

	/// <summary>
	/// GalaxyCluster 
	/// </summary>
	//[ComSourceInterfacesAttribute(Universe::IManagerExtender::typeid)]
	public ref class GalaxyCluster : public IWin32Window, public Dictionary<String^, Quasar^>
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

		property String^ QuasarName[Control^]
		{
			String ^ get(Control ^ ctrl)
			{
				if (ctrl != nullptr)
				{
					LONGLONG hWnd = ctrl->Handle.ToInt64();
					BSTR bstrName = ::SysAllocString(L"");
					m_pGalaxyCluster->get_QuasarName(hWnd, &bstrName);
					String^ strRet = BSTR2STRING(bstrName);
					::SysFreeString(bstrName);
					return strRet;
				}
				return String::Empty;
			}
		}

			property String^ QuasarName[IntPtr]
		{
			String ^ get(IntPtr handle)
			{
				if (::IsWindow((HWND)handle.ToInt64()))
				{
					BSTR bstrName = ::SysAllocString(L"");
					m_pGalaxyCluster->get_QuasarName(handle.ToInt32(), &bstrName);
					String^ strRet = BSTR2STRING(bstrName);
					::SysFreeString(bstrName);
					return strRet;
				}
				return String::Empty;
			}
		}

			property Object^ Extender[String^]
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

			property Quasar^ Quasars[Object^]
		{
			Quasar ^ get(Object ^ obj)
			{
				CComVariant m_v;
				IntPtr handle = (IntPtr)&m_v;
				Marshal::GetNativeVariantForObject(obj, handle);
				CComPtr<IQuasar> pQuasar;
				m_pGalaxyCluster->get_Quasar(m_v, &pQuasar);
				if (pQuasar)
				{
					return theAppProxy._createObject<IQuasar, Quasar>(pQuasar);
				}
				return nullptr;
			}
		}

		Star^ GetStar(String^ bstrXml, String^ bstrFrameName);

		Quasar^ CreateQuasar(Control^ ctrl, String^ strName)
		{
			Quasar^ pRetQuasar = nullptr;
			if (ctrl != nullptr && String::IsNullOrEmpty(strName) == false)
			{
				if (TryGetValue(strName, pRetQuasar))
				{
					return pRetQuasar;
				}
				CComPtr<IQuasar> pQuasar;
				m_pGalaxyCluster->CreateQuasar(CComVariant(0), CComVariant((LONGLONG)ctrl->Handle.ToInt64()), STRING2BSTR(strName), &pQuasar);
				if (pQuasar)
				{
					pRetQuasar = theAppProxy._createObject<IQuasar, Quasar>(pQuasar);
					if (pRetQuasar != nullptr)
						Add(strName, pRetQuasar);
				}
			}
			return pRetQuasar;
		}

		Quasar^ CreateQuasar(IntPtr handle, String^ strName)
		{
			Quasar^ pRetQuasar = nullptr;
			if (::IsWindow((HWND)handle.ToInt64()) && String::IsNullOrEmpty(strName) == false)
			{
				if (TryGetValue(strName, pRetQuasar))
				{
					return pRetQuasar;
				}
				CComPtr<IQuasar> pQuasar;
				m_pGalaxyCluster->CreateQuasar(CComVariant(0), CComVariant((LONGLONG)handle.ToInt64()), STRING2BSTR(strName), &pQuasar);
				if (pQuasar)
				{
					pRetQuasar = theAppProxy._createObject<IQuasar, Quasar>(pQuasar);
					if (pRetQuasar != nullptr)
						Add(strName, pRetQuasar);
				}
			}
			return pRetQuasar;
		}

		void ObserveQuasars(String^ strFrames, String^ strKey, String^ bstrXml, bool bSaveToConfigFile);

		delegate void DocumentComplete(GalaxyCluster^ sender, Object^ pHtmlDoc, String^ strURL);
		event DocumentComplete^ OnDocumentComplete;
		void Fire_OnDocumentComplete(GalaxyCluster^ sender, Object^ pHtmlDoc, String^ strURL)
		{
			OnDocumentComplete(sender, pHtmlDoc, strURL);
		}

		delegate void IPCMsg(Quasar^ sender, String^ strType, String^ strContent, String^ strFeature);
		event IPCMsg^ OnIPCMsg;
		void Fire_OnIPCMsg(Quasar^ sender, String^ strType, String^ strContent, String^ strFeature)
		{
			OnIPCMsg(sender, strType, strContent, strFeature);
		}

		delegate void Destroy(Star^ sender);
		event Destroy^ OnDestroy;
		void Fire_OnDestroy(Star^ sender)
		{
			OnDestroy(sender);
		}

		delegate void TabChange(Star^ sender, int nActivePage, int nOldActivePage);
		event TabChange^ OnTabChange;
		void Fire_OnTabChange(Star^ sender, int nActivePage, int nOldActivePage)
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
		CGalaxyClusterEvent* m_pTangramClrEvent;

		~GalaxyCluster();
	};
}

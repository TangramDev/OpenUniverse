/********************************************************************************
*					Open Universe - version 1.0.1.14							*
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
class CGridCLREvent;
class CCosmosGridEvent;

namespace Cosmos
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
	/// Summary for Hubble
	/// </summary>
	interface class IHubbleApp;

	ref class Grid;
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

	public ref class TangramAppProxy
	{
	public:
		TangramAppProxy();
		~TangramAppProxy();

		void Init();

		IUniverseAppProxy* m_pUniverseAppProxy;
	};

	public ref class GridCollection : public System::Collections::IEnumerable
	{
	public:
		GridCollection(IGridCollection* pGrids)
		{
			if (pGrids)
			{
				m_pGridCollection = pGrids;
				m_pGridCollection->AddRef();
			}
		};

		~GridCollection()
		{
			if (m_pGridCollection)
			{
				DWORD dw = m_pGridCollection->Release();
				while (dw)
					dw = m_pGridCollection->Release();
			}
		}

	private:
		IGridCollection* m_pGridCollection;
	public:
		virtual System::Collections::IEnumerator^ GetEnumerator()
		{
			return gcnew TangramBaseEnumerator<GridCollection>(this, StarCount);
		}

		property Grid^ default[int]
		{
			Grid ^ get(int iIndex);
		}

		property int StarCount
		{
			int get()
			{
				long n = 0;
				m_pGridCollection->get_GridCount(&n);
				return n;
			}
		}
	};

	public ref class Grid
	{
	public:
		Grid(IGrid* pGrid);
		~Grid();

		IGrid* m_pGrid;
		Object^ m_pHostObj = nullptr;
		ListView^ m_pBindListView = nullptr;
		TreeView^ m_pBindTreeView = nullptr;
		CCosmosGridEvent* m_pGridEvent;
		CGridCLREvent* m_pGridCLREvent;
		Cosmos::Wormhole^ m_pSession = nullptr;

		void NavigateTreeInit();
		//virtual void CloseForm(Object^ pForm){};
		//virtual void OnCloseForm(long long hFormWnd){};

		delegate void GridAddInCreated(Grid^ sender, Object^ pAddIndisp, String^ bstrAddInID, String^ bstrAddInXml);
		event GridAddInCreated^ OnGridAddInCreated;
		void Fire_GridAddInCreated(Grid^ sender, Object^ pAddIndisp, String^ bstrAddInID, String^ bstrAddInXml)
		{
			OnGridAddInCreated(sender, pAddIndisp, bstrAddInID, bstrAddInXml);
		}

		delegate void GridAddInsCreated(Grid^ sender);
		event GridAddInsCreated^ OnGridAddInsCreated;
		void Fire_GridAddInsCreated(Grid^ sender)
		{
			OnGridAddInsCreated(sender);
		}

		// A notification has been created for all nodes in the current layout.
		delegate void RootGridCreated(IntPtr nHandle, String^ strUrl, Grid^ pRootGrid);
		event RootGridCreated^ OnRootGridCreated;
		void Fire_RootGridCreated(IntPtr nHandle, String^ strUrl, Grid^ pRootGrid)
		{
			OnRootGridCreated(nHandle, strUrl, pRootGrid);
			// Notify all child nodes
			for each (Grid ^ pChildNode in this->ChildNodes)
			{
				pChildNode->Fire_RootGridCreated(nHandle, strUrl, pRootGrid);
			}
		}

		delegate void ObserveComplete(Grid^ sender);
		event ObserveComplete^ OnObserveComplete;
		void Fire_ObserveComplete(Grid^ sender)
		{
			OnObserveComplete(sender);
		}

		delegate void Destroy(Grid^ sender);
		event Destroy^ OnDestroy;
		void Fire_OnDestroy(Grid^ sender)
		{
			OnDestroy(sender);
		}

		delegate void DocumentComplete(Grid^ sender, Object^ pHtmlDoc, String^ strURL);
		event DocumentComplete^ OnDocumentComplete;
		void Fire_OnDocumentComplete(Grid^ sender, Object^ pHtmlDoc, String^ strURL)
		{
			OnDocumentComplete(sender, pHtmlDoc, strURL);
		}

		delegate void TabChange(Grid^ ActiveGrid, Grid^ OldGrid);
		event TabChange^ OnTabChange;
		void Fire_OnTabChange(Grid^ ActiveGrid, Grid^ OldGrid);

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
			HRESULT hr = m_pGrid->SendIPCMessage(STRING2BSTR(strTo), STRING2BSTR(strPayload), STRING2BSTR(strExtra), STRING2BSTR(strMsgId), &bstrRet);
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
		void SetNewNode(IGrid* pGrid)
		{
			if (m_pGrid != NULL)
			{
				m_pGrid = NULL;
			}

			if (pGrid != NULL)
			{
				m_pGrid = pGrid;
			}
		}

		void Init();

	public:
		//void SaveHubbleDoc(String^ m_strName);
		Object^ ActiveMethod(String^ strMethod, cli::array<Object^, 1>^ p);
		//[DispId(0x000001)]
		//[ComVisibleAttribute(true)]
		//Object^ ExecScript(String^ strScript);
		Grid^ Observe(String^ layerName, String^ layerXML);
		Grid^ ObserveChild(int rowNum, int colName, String^ layerName, String^ layerXML);

		property String^ Caption
		{
			String^ get();
			void set(String^ strCaption)
			{
				m_pGrid->put_Caption(STRING2BSTR(strCaption));
			}
		}

		property bool SaveToConfigFile
		{
			void set(bool value)
			{
				m_pGrid->put_SaveToConfigFile(value);
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
				if (m_pGrid)
				{
					CComBSTR bstrCap("");
					m_pGrid->get_Name(&bstrCap);
					return BSTR2STRING(bstrCap);
				}
				return "";
			}

			void set(String^ newVal)
			{
				if (m_pGrid)
				{
					m_pGrid->put_Name(STRING2BSTR(newVal));
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
				m_pGrid->get_XObject(&var);

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
				m_pGrid->get_rgbMiddle(&rgb);
				return System::Drawing::ColorTranslator::FromOle(rgb);
			}
			void set(System::Drawing::Color newVal)
			{
				m_pGrid->put_rgbMiddle(System::Drawing::ColorTranslator::ToOle(newVal));
			}
		}

		property System::Drawing::Color rgbLeftTop
		{
			System::Drawing::Color get()
			{
				OLE_COLOR rgb;
				m_pGrid->get_rgbLeftTop(&rgb);
				return System::Drawing::ColorTranslator::FromOle(rgb);
			}
			void set(System::Drawing::Color newVal)
			{
				m_pGrid->put_rgbLeftTop(System::Drawing::ColorTranslator::ToOle(newVal));
			}
		}

		property System::Drawing::Color rgbRightBottom
		{
			System::Drawing::Color get()
			{
				OLE_COLOR rgb;
				m_pGrid->get_rgbRightBottom(&rgb);
				return System::Drawing::ColorTranslator::FromOle(rgb);
			}
			void set(System::Drawing::Color newVal)
			{
				m_pGrid->put_rgbRightBottom(System::Drawing::ColorTranslator::ToOle(newVal));
			}
		}

		[BrowsableAttribute(false)]
		property Object^ Extender
		{
			Object^ get()
			{
				Object^ pRetObject = nullptr;
				IDispatch* pDisp = NULL;
				m_pGrid->get_Extender(&pDisp);
				if (pDisp)
					pRetObject = Marshal::GetObjectForIUnknown((IntPtr)pDisp);
				return pRetObject;
			}
			void set(Object^ obj)
			{
				IDispatch* pDisp = (IDispatch*)Marshal::GetIUnknownForObject(obj).ToPointer();
				m_pGrid->put_Extender(pDisp);
			}
		}

		property Object^ Tag
		{
			Object^ get()
			{
				Object^ pRetObject = nullptr;
				VARIANT var;
				m_pGrid->get_Tag(&var);
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
					m_pGrid->put_Tag(var);
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
				m_pGrid->get_ActivePage(&nPage);

				return nPage;
			}

			void set(int nPage)
			{
				m_pGrid->put_ActivePage(nPage);
			}
		}

		[BrowsableAttribute(false)]
		property Galaxy^ Galaxy
		{
			Cosmos::Galaxy^ get();
		}

		[BrowsableAttribute(false)]
		property Cosmos::Galaxy^ HostGalaxy
		{
			Cosmos::Galaxy^ get()
			{
				CComPtr<IGalaxy> pGalaxy;
				m_pGrid->get_HostGalaxy(&pGalaxy);
				if (pGalaxy)
				{
					return theAppProxy._createObject<IGalaxy, Cosmos::Galaxy>(pGalaxy);
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
		property GridCollection^ ChildNodes
		{
			GridCollection^ get();
		}

		[BrowsableAttribute(false)]
		property GridCollection^ Objects[GridType]
		{
			GridCollection ^ get(GridType nValue)
			{
				CComPtr<IGridCollection> pGrids = NULL;
				m_pGrid->get_Objects((long)nValue,&pGrids);
				return gcnew GridCollection(pGrids);
			}
		}

			[BrowsableAttribute(false)]
		property Grid^ RootGrid
		{
			Grid^ get()
			{
				CComPtr<IGrid> pRootGrid;
				m_pGrid->get_RootGrid(&pRootGrid);

				return theAppProxy._createObject<IGrid, Grid>(pRootGrid);
			}
		}

		[BrowsableAttribute(false)]
		property Grid^ ParentGrid
		{
			Grid^ get()
			{
				CComPtr<IGrid> pRootGrid;
				m_pGrid->get_ParentGrid(&pRootGrid);

				return theAppProxy._createObject<IGrid, Grid>(pRootGrid);
			}
		}

		[BrowsableAttribute(false)]
		property Grid^ HostGrid
		{
			Grid^ get()
			{
				IGrid* pGrid = nullptr;
				m_pGrid->get_HostGrid(&pGrid);
				if (pGrid == NULL)
					return nullptr;
				return theAppProxy._createObject<IGrid, Grid>(pGrid);
			}
		}

		[BrowsableAttribute(false)]
		property int Col
		{
			int get()
			{
				long nValue = 0;
				m_pGrid->get_Col(&nValue);
				return nValue;
			}
		}

		[BrowsableAttribute(false)]
		property int Row
		{
			int get()
			{
				long nValue = 0;
				m_pGrid->get_Row(&nValue);
				return nValue;
			}
		}

		property int Hmin
		{
			int get()
			{
				int nValue = 0;
				m_pGrid->get_Hmin(&nValue);
				return nValue;
			}
			void set(int newVal)
			{
				m_pGrid->put_Hmin(newVal);
			}
		}

		property int Hmax
		{
			int get()
			{
				int nValue = 0;
				m_pGrid->get_Hmax(&nValue);
				return nValue;
			}
			void set(int newVal)
			{
				m_pGrid->put_Hmax(newVal);
			}
		}

		property int Vmin
		{
			int get()
			{
				int nValue = 0;
				m_pGrid->get_Vmin(&nValue);
				return nValue;
			}
			void set(int newVal)
			{
				m_pGrid->put_Vmin(newVal);
			}
		}

		property int Vmax
		{
			int get()
			{
				int nValue = 0;
				m_pGrid->get_Vmax(&nValue);
				return nValue;
			}
			void set(int newVal)
			{
				m_pGrid->put_Vmax(newVal);
			}
		}

		property int Rows
		{
			int get()
			{
				long nValue = 0;
				m_pGrid->get_Rows(&nValue);
				return nValue;
			}
		}

		property int Cols
		{
			int get()
			{
				long nValue = 0;
				m_pGrid->get_Cols(&nValue);
				return nValue;
			}
		}

		property ::GridType GridType
		{
			::GridType get()
			{
				::GridType type;
				m_pGrid->get_GridType(&type);
				return type;
			}
		}

		property __int64 Handle
		{
			__int64 get()
			{
				if (m_hWnd)
					return (__int64)m_hWnd;
				__int64 h = 0;
				m_pGrid->get_Handle(&h);
				m_hWnd = (HWND)h;
				return h;
			}
		}

		[BrowsableAttribute(false)]
		property String^ Attribute[String ^]
		{
			String ^ get(String ^ strKey)
			{
				BSTR bstrVal;
				m_pGrid->get_Attribute(STRING2BSTR(strKey),&bstrVal);

				return BSTR2STRING(bstrVal);
			}

			void set(String ^ strKey, String ^ strVal)
			{
				m_pGrid->put_Attribute(
					STRING2BSTR(strKey),
					STRING2BSTR(strVal));
			}
		}

	public:
		Grid^ GetGrid(int nRow, int nCol)
		{
			IGrid* pGrid;
			m_pGrid->GetGrid(nRow, nCol, &pGrid);
			return theAppProxy._createObject<IGrid, Grid>(pGrid);
		}

		int GetGrids(String^ strName, Grid^% pGrid, GridCollection^% pGridCollection)
		{
			IGridCollection* pGrids = nullptr;
			IGrid* pTNode = nullptr;

			pGrid = nullptr;
			pGridCollection = nullptr;

			long nCount;
			m_pGrid->GetGrids(STRING2BSTR(strName),
				&pTNode, &pGrids, &nCount);

			pGrid = theAppProxy._createObject<IGrid, Grid>(pTNode);

			pGridCollection = gcnew GridCollection(pGrids);

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
		Grid^ bindTreeNode = nullptr;
		Grid^ bindListViewNode = nullptr;
		Dictionary<String^, MethodInfo^>^ m_pHubbleCLRMethodDic = nullptr;
		Dictionary<String^, Object^>^ m_pPlugInDic = nullptr;
		System::Void LoadNode(TreeNode^ pGrid, CTangramXmlParse* pParse);

		GridCollection^ m_pChildNodes;
		void OnAfterSelect(System::Object^ sender, System::Windows::Forms::TreeViewEventArgs^ e);
		void OnNodeMouseDoubleClick(System::Object^ sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^ e);
};

	public ref class Galaxy : public Dictionary<String^, Grid^>
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
		Grid^ Observe(String^ layerName, String^ layerXML);

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

		property Object^ FrameData[String ^]
		{
			Object ^ get(String ^ iIndex);
			void set(String^ iIndex, Object^ newVal);
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

		property Grid^ VisibleGrid
		{
			Cosmos::Grid ^ get()
			{
				IGrid* pGrid = nullptr;
				m_pGalaxy->get_VisibleGrid(&pGrid);
				return theAppProxy._createObject<IGrid, Cosmos::Grid>(pGrid);
			}
		}

		property Grid^ Grid[Object ^]
		{
			Cosmos::Grid ^ get(Object ^ index)
			{
				VARIANT var;
				Marshal::GetNativeVariantForObject(index,(System::IntPtr) & var);
				IGrid* pGrid = nullptr;
				m_pGalaxy->get_Grid(var, &pGrid);
				return theAppProxy._createObject<IGrid, Cosmos::Grid>(pGrid);
			}
		}

			property GridCollection^ RootGrids
		{
			GridCollection^ get()
			{
				CComPtr<IGridCollection> pGrids = NULL;
				m_pGalaxy->get_RootGrids(&pGrids);

				return gcnew GridCollection(pGrids.p);
			}
		}

		property GalaxyCluster^ GalaxyCluster
		{
			Cosmos::GalaxyCluster^ get()
			{
				CComPtr<IGalaxyCluster> pGalaxyCluster = NULL;
				m_pGalaxy->get_GalaxyCluster(&pGalaxyCluster);

				return theAppProxy._createObject<IGalaxyCluster, Cosmos::GalaxyCluster>(pGalaxyCluster);
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

	public ref class Hubble
	{
	public:
		Hubble(IHubble* pHubble);
		~Hubble();
	private:
		Hubble();
		static bool IsAppInit = false;
		static bool IsWebRuntimeInit = false;

		static Hubble^ m_pHubble;
		static Dictionary<String^, MethodInfo^>^ m_pHubbleCLRMethodDic = gcnew Dictionary<String^, MethodInfo^>();
		static Dictionary<String^, Type^>^ m_pHubbleCLRTypeDic = gcnew Dictionary<String^, Type^>();
		static Dictionary<String^, Type^>^ m_pAppFormTypeDic = nullptr;
		static Dictionary<String^, Type^>^ m_pAppMDIFormTypeDic = nullptr;
		static Dictionary<Control^, String^>^ m_pControlRelationDic = nullptr;
		static Dictionary<Object^, Wormhole^>^ m_pWormholes = gcnew Dictionary<Object^, Wormhole^>();
		static Dictionary<String^, String^>^ CustomizeDictionary = gcnew Dictionary<String^, String^>();
		static Dictionary<String^, Object^>^ m_pHubbleCLRObjDic = gcnew Dictionary<String^, Object^>();
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
		static Dictionary<String^, TangramAppProxy^>^ m_pHubbleAppProxyDic = gcnew Dictionary<String^, TangramAppProxy^>();
		static Dictionary<Object^, Grid^>^ m_pFrameworkElementDic = gcnew Dictionary<Object^, Grid^>();

		static String^ ComputeHash(String^ source);
		static int HubbleInit(String^ strInit);
		static GalaxyCluster^ CreateGalaxyCluster(IntPtr nPageHandle);
		static GalaxyCluster^ CreateGalaxyCluster(Control^ ctrl, Object^ ExternalObj);
		static Object^ CreateObject(String^ ObjID);
		static Form^ CreateForm(IWin32Window^ parent, String^ ObjID);
		static Type^ GetType(String^ ObjID);
		static Object^ ActiveMethod(String^ strObjID, String^ strMethod, cli::array<Object^, 1>^ p);
		static Object^ ActiveObjectMethod(Object^ pObj, String^ strMethod, cli::array<Object^, 1>^ p);
		static void StartApplication(String^ appID, String^ strXML);
		static WorkBenchWindow^ RemoteActiveWorkBenchWindow(String^ appID);
		static Control^ GetMDIClient(Form^ pForm);
		static Grid^ ExtendMDIClient(Form^ pForm, String^ strKey, String^ strXml);
		static Dictionary<String^, Type^>^ InitAppFormTypeDic();
		static Dictionary<String^, Type^>^ GetFormTypesFromAssembly(String^ assemblyFilePath);
		static Dictionary<String^, Type^>^ GetFormTypesFromDirectory(String^ directoryPath);
		static List<String^>^ FindFiles(String^ rootPath, String^ fileSpec, bool recursive);
		static Browser^ ActiveBrowser();
		static Browser^ GetHostBrowser(Object^ obj);
		static Grid^ GetGridFromHandle(IntPtr handle);
		static Grid^ GetGridFromControl(Control^ ctrl);
		static void RegComponentForTangram(String^ strIDs, Assembly^ a);
		static void UpdateNewTabPageLayout(String^ newTabPageLayout);
		static void BindObjToWebPage(IntPtr hWebPage, Object^ pObj, String^ name);
		static Grid^ Observe(Control^ ctrl, String^ key, String^ strGridXml);

		Cosmos::IHubbleApp^ m_pUniverseAppProxy;

		static void SetControlRelation(Control^ ctrl, String^ strTypes);

		static void ExportAllCLRObjInfo();

		static Hubble^ GetHubble();

		static Hubble^ InitHubbleApp(bool bSupportCrashReporting, CosmosAppType AppType);

		static bool WebRuntimeInit();
		static void InitEclipse();

		static void Run();
		static void Run(Form^ Mainform);
		static void Run(ApplicationContext^ context);
		static void ExtractToDirectory(String^ strSRC, String^ strTarget);
		static Grid^ GetGridFromObj(Object ^ obj)
		{
			Grid^ pGrid = nullptr;
			if (m_pFrameworkElementDic->TryGetValue(obj, pGrid))
			{
				return pGrid;
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
					cli::array<byte,1>^ bs = gcnew cli::array<byte,1>((int)sm->Length);
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
#ifndef _WIN64
		static property Dictionary<String^, String^>^ WizDic
		{
			Dictionary<String^, String^>^ get()
			{
				return replacementsDictionary;
			};
			void set(Dictionary<String^, String^>^ Dic)
			{
				replacementsDictionary = Dic;
			};
		}

		static void SelectObject(String^ strData, Object^ ObjSelected, IntPtr nHandle);
		static void SolutionEvent(String^ strSolutionName, String^ PrjName, String^ PrjType, IntPtr eventID);

		static Galaxy^ CreateVSToolWnd(String^ strXm);
		static IntPtr GetChild(IntPtr nHandle);
		static void ShowVSToolBox(IntPtr nHandle);
		static bool ReplaceHTML(Object^ doc, String^ originHTML, String^ newHTML);
#endif
		delegate void HubbleActionDelegate(Grid^ SourceObj, String^ strInfo);
		static event HubbleActionDelegate^ OnHubbleActionDelegate;
		static void Fire_OnHubbleActionDelegate(Grid^ SourceObj, String^ strInfo)
		{
			OnHubbleActionDelegate(SourceObj, strInfo);
		}

		static property Dictionary<String^, String^>^ CustomizeDic
		{
			Dictionary<String^, String^>^ get()
			{
				return CustomizeDictionary;
			};
		}

		static property Dictionary<Object^, Cosmos::Wormhole^>^ Wormholes
		{
			Dictionary<Object^, Cosmos::Wormhole^>^ get()
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

		static property Hubble^ TangramCore
		{
			Hubble^ get();
		}

		static property String^ CurrentDesigningTangramXml
		{
			String^ get();
		}

		static property String^ AppData
		{
			String^ get();
		}

		static property Grid^ CreatingGrid
		{
			Grid^ get();
		}

		static property Galaxy^ ActiveEclipseTopFrame
		{
			Galaxy^ get();
		}

		static Browser^ CreateBrowser(IntPtr ParentHandle, String^ strUrls);
		static Browser^ CreateBrowserRemote(IntPtr ParentHandle, String^ strUrls);
		//static property Dictionary<String^, Type^>^ FormTypeDictionary
		//{
		//	Dictionary<String^, Type^>^ get()
		//	{
		//		if (Cosmos::Hubble::m_pAppFormTypeDicEvent != nullptr)
		//			Cosmos::Hubble::m_pAppFormTypeDicEvent->WaitOne();
		//		return m_pAppFormTypeDic;
		//	}
		//}

		//static property Dictionary<String^, Type^>^ MDIFormTypeDictionary
		//{
		//	Dictionary<String^, Type^>^ get()
		//	{
		//		if (Cosmos::Hubble::m_pAppFormTypeDicEvent != nullptr)
		//			Cosmos::Hubble::m_pAppFormTypeDicEvent->WaitOne();
		//		return m_pAppMDIFormTypeDic;
		//	}
		//}

		static property Dictionary<String^, TangramAppProxy^>^ TangramAppProxyDictionary
		{
			Dictionary<String^, TangramAppProxy^>^ get()
			{
				return m_pHubbleAppProxyDic;
			}
		}

		static property Hubble^ RemoteTangram[String ^]
		{
			Hubble ^ get(String ^ strAppID);
		}

		static property TangramAppProxy^ AppProxy[String ^]
		{
			TangramAppProxy ^ get(String ^ strAppID);
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

		delegate void BindCLRObjToWebPage(Object^ SourceObj, Cosmos::Wormhole^ eventSession ,String^ eventName);
		static event BindCLRObjToWebPage^ OnBindCLRObjToWebPage;
		static void Fire_OnBindCLRObjToWebPage(Object^ SourceObj, Cosmos::Wormhole^ eventSession, String^ eventName);

		delegate void OpenComplete(IntPtr hWnd, String^ bstrUrl, Grid^ pRootGrid);
		static event OpenComplete^ OnObserverComplete;
		static void Fire_OnObserverComplete(IntPtr hWnd, String^ bstrUrl, Grid^ pRootGrid)
		{
			OnObserverComplete(hWnd, bstrUrl, pRootGrid);
		}

		delegate void AppMsgLoop(IntPtr hWnd, IntPtr msg, IntPtr wParam, IntPtr lParam);
		static event AppMsgLoop^ OnAppMsgLoop;
		static void Fire_OnAppMsgLoop(IntPtr hWnd, IntPtr msg, IntPtr wParam, IntPtr lParam)
		{
			OnAppMsgLoop(hWnd, msg, wParam, lParam);
		}

		delegate void TangramDebugDelegate(String^ strInfo);
		static event TangramDebugDelegate^ OnHubbleDebugDelegate;
		static void Fire_OnHubbleDebugDelegate(String^ strDebugInfo)
		{
			OnHubbleDebugDelegate(strDebugInfo);
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

		delegate void CustomizedDOMElement(IntPtr hWnd, String^ strRuleName, String^ strHTML);
		static event CustomizedDOMElement^ OnCustomizedDOMElement;
		static void Fire_OnCustomizedDOMElement(IntPtr hWnd, String^ strRuleName, String^ strHTML)
		{
			OnCustomizedDOMElement(hWnd, strRuleName, strHTML);
		}

		//delegate void TangramAppData(String^ strAppData);
		//static event TangramAppData^ OnHubbleAppData;
		//static void Fire_OnHubbleAppData(String^ strAppData)
		//{
		//	OnHubbleAppData(strAppData);
		//}

		delegate void FormNodeCreated(String^ bstrObjID, Form^ pForm, Grid^ pGrid);
		static event FormNodeCreated^ OnFormNodeCreated;
		static void Fire_OnFormNodeCreated(String^ bstrObjID, Form^ pForm, Grid^ pGrid)
		{
			OnFormNodeCreated(bstrObjID, pForm, pGrid);
		}

		property String^ AppKeyValue[String ^]
		{
			String ^ get(String ^ iIndex);
			void set(String^ iIndex, String^ newVal);
		}
		static void OnHubble_DebugDelegate(System::String^ strInfo);
};

	/// <summary>
	/// GalaxyCluster 
	/// </summary>
	//[ComSourceInterfacesAttribute(Cosmos::IManagerExtender::typeid)]
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

		Grid^ GetGrid(String^ strName, String^ strGalaxyName);

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

		delegate void Destroy(Grid ^ sender);
		event Destroy^ OnDestroy;
		void Fire_OnDestroy(Grid ^ sender)
		{
			OnDestroy(sender);
		}

		delegate void TabChange(Grid ^ sender, int nActivePage, int nOldActivePage);
		event TabChange^ OnTabChange;
		void Fire_OnTabChange(Grid ^ sender, int nActivePage, int nOldActivePage)
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
		CGalaxyClusterEvent* m_pHubbleClrEvent;

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

		Grid^ Observe(String^ strGalaxyName, String^ strKey, String^ strXml)
		{
			if (m_pCtrl)
			{
				IGrid* pGrid = nullptr;
				m_pCtrl->Observe(STRING2BSTR(strGalaxyName), STRING2BSTR(strKey), STRING2BSTR(strXml), &pGrid);
				if (pGrid)
				{
					return theAppProxy._createObject<IGrid, Grid>(pGrid);
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
			//SetNewNodeCollection(pGrids);
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

		Grid^ SWTExtend(String^ strKey, String^ strXml)
		{
			if (m_pWorkBenchWindow)
			{
				IGrid* pGrid = nullptr;
				m_pWorkBenchWindow->Observe(STRING2BSTR(strKey), STRING2BSTR(strXml), &pGrid);
				if (pGrid)
					return theAppProxy._createObject<IGrid, Grid>(pGrid);
			}
			return nullptr;
		}

		Grid^ SWTExtendInView(int nView, String^ strKey, String^ strXml)
		{
			if (m_pWorkBenchWindow)
			{
				IEclipseCtrl* pCtrl = nullptr;
				m_pWorkBenchWindow->get_Ctrl(CComVariant((int)nView), &pCtrl);
				if (pCtrl)
				{
					IGrid* pGrid = nullptr;
					pCtrl->Observe(CComBSTR(L"EclipseView"), STRING2BSTR(strKey), STRING2BSTR(strXml), &pGrid);
					if (pGrid)
						return theAppProxy._createObject<IGrid, Grid>(pGrid);
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

	public interface class IHubbleEventObjProxy
	{
	};

	public interface class IHubbleWndGridProxy
	{
		virtual void OnObserverComplete();
		virtual void OnDestroy();
		virtual void OnGridAddInCreated(Object^ pAddIndisp, String^ bstrAddInID, String^ bstrAddInXml);
		virtual void OnGridAddInsCreated();
		virtual void OnGridDocumentComplete(Object^ ExtenderDisp, String^ bstrURL);
		virtual void OnControlNotify(Grid^ sender, LONG NotifyCode, LONG CtrlID, IntPtr CtrlHandle, String^ CtrlClassName);
		virtual void OnTabChange(LONG ActivePage, LONG OldPage);
	};

	public interface class IGalaxyClusterProxy
	{
		virtual void OnGalaxyClusterLoaded(GalaxyCluster^ sender, String^ url);
		virtual void OnGridCreated(Grid^ pGridCreated);
		virtual void OnAddInCreated(Grid^ pRootGrid, Object^ pAddIn, String^ strID, String^ strAddInXml);
		virtual void OnBeforeOpenXml(String^ bstrXml, IntPtr hWnd);
		virtual void OnOpenXmlComplete(String^ bstrXml, IntPtr hWnd, Grid^ pRetRootNode);
		virtual void OnDestroy();
		virtual void OnNodeMouseActivate(Grid^ pActiveNode);
		virtual void OnClrControlCreated(Grid^ Node, Control^ Ctrl, String^ CtrlName, IntPtr CtrlHandle);
		virtual void OnTabChange(Grid^ sender, LONG ActivePage, LONG OldPage);
		virtual void OnControlNotify(Grid^ sender, LONG NotifyCode, LONG CtrlID, IntPtr CtrlHandle, String^ CtrlClassName);
		virtual void OnHubbleEvent(IHubbleEventObjProxy^ NotifyObj);
	};

	public interface class IGalaxyProxy
	{
		virtual void OnExtend(Grid^ pRetNode, String^ bstrKey, String^ bstrXml);
	};

	public interface class IVSProxy
	{
		virtual void OnConnect(String^ bstrKey, IntPtr nHandle);
	};

	public interface class IHubbleApp
	{
		[DispId(0x000001)]
		virtual void TangramClose();
		[DispId(0x000002)]
		virtual IHubbleWndGridProxy^ OnGridInit(Grid^ pNewNode);
		[DispId(0x000003)]
		virtual IGalaxyProxy^ OnGalaxyCreated(Galaxy^ pNewFrame);
		[DispId(0x000004)]
		virtual IGalaxyClusterProxy^ OnGalaxyClusterCreated(GalaxyCluster^ pNewPage);
		[DispId(0x000005)]
		virtual void OnObserverComplete(IntPtr hWnd, String^ bstrUrl, Grid^ pRootGrid);
		[DispId(0x000006)]
		virtual void OnHubbleEvent(IHubbleEventObjProxy^ NotifyObj);
	};
}

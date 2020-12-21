/********************************************************************************
*					DOM Plus for Application - Version 1.1.5.30                 *
*********************************************************************************
* Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.				*
*
* This SOURCE CODE is governed by a BSD - style license that can be
* found in the LICENSE file.
*
* CONTACT INFORMATION:
* mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
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

class CXobjCLREvent;
class CCosmosXobjEvent;
extern CCosmosProxy theAppProxy;

namespace DOMPlus
{
	/// <summary>
	/// Summary for Cosmos
	/// </summary>

	ref class Xobj;
	ref class Galaxy;

	public enum class CosmosAppType
	{
		APPWIN32 = APP_WIN32,
		APPBROWSER = APP_BROWSER,
		APPECLIPSE = APP_ECLIPSE,
		APPBROWSER_ECLIPSE = APP_BROWSER_ECLIPSE,
		APPBROWSERAPP = APP_BROWSERAPP,
		APPOTHER = 0
	};

	public ref class Xobj
	{
	public:
		Xobj(IXobj* pXobj);
		~Xobj();

		IXobj* m_pXobj;
		Object^ m_pHostObj = nullptr;
		CCosmosXobjEvent* m_pXobjEvent;
		CXobjCLREvent* m_pXobjCLREvent;

		// A notification has been created for all nodes in the current layout.
		delegate void RootGridCreated(IntPtr nHandle, String^ strUrl, Xobj^ pRootGrid);
		event RootGridCreated^ OnRootGridCreated;
		void Fire_RootGridCreated(IntPtr nHandle, String^ strUrl, Xobj^ pRootGrid)
		{
			OnRootGridCreated(nHandle, strUrl, pRootGrid);
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

		delegate void TabChange(Xobj^ ActiveGrid, Xobj^ OldGrid);
		event TabChange^ OnTabChange;
		void Fire_OnTabChange(Xobj^ ActiveGrid, Xobj^ OldGrid);

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

		property Drawing::Color rgbLeftTop
		{
			Drawing::Color get()
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

		property Drawing::Color rgbRightBottom
		{
			Drawing::Color get()
			{
				OLE_COLOR rgb;
				m_pXobj->get_rgbRightBottom(&rgb);
				return Drawing::ColorTranslator::FromOle(rgb);
			}
			void set(Drawing::Color newVal)
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
			DOMPlus::Galaxy^ get();
		}

		property DOMPlus::Galaxy^ HostGalaxy
		{
			DOMPlus::Galaxy^ get()
			{
				CComPtr<IGalaxy> pGalaxy;
				m_pXobj->get_HostGalaxy(&pGalaxy);
				if (pGalaxy)
				{
					return theAppProxy._createObject<IGalaxy, DOMPlus::Galaxy>(pGalaxy);
				}
				return nullptr;
			}
		}

		property Xobj^ RootGrid
		{
			Xobj^ get()
			{
				CComPtr<IXobj> pRootGrid;
				m_pXobj->get_RootGrid(&pRootGrid);

				return theAppProxy._createObject<IXobj, Xobj>(pRootGrid);
			}
		}

		property Xobj^ ParentGrid
		{
			Xobj^ get()
			{
				CComPtr<IXobj> pXobj;
				m_pXobj->get_ParentGrid(&pXobj);

				return theAppProxy._createObject<IXobj, Xobj>(pXobj);
			}
		}

		property Xobj^ HostGrid
		{
			Xobj^ get()
			{
				IXobj* pXobj = nullptr;
				m_pXobj->get_HostGrid(&pXobj);
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

		property ::GridType GridType
		{
			::GridType get()
			{
				::GridType type;
				m_pXobj->get_GridType(&type);
				return (::GridType)type;
			}
		}

		property __int64 Handle
		{
			__int64 get()
			{
				if (m_hWnd)
					return (__int64)m_hWnd;
				__int64 h = 0;
				m_pXobj->get_Handle(&h);
				m_hWnd = (HWND)h;
				return h;
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

		Xobj^ GetGrid(int nRow, int nCol)
		{
			IXobj* pXobj;
			m_pXobj->GetGrid(nRow, nCol, &pXobj);
			return theAppProxy._createObject<IXobj, Xobj>(pXobj);
		}

		Xobj^ GetChildGrid(String^ strName)
		{
			IXobj* pXobj;
			BSTR bstrName = STRING2BSTR(strName);
			m_pXobj->GetChildGridByName(bstrName, &pXobj);
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

		property Xobj^ VisibleGrid
		{
			DOMPlus::Xobj^ get()
			{
				IXobj* pXobj = nullptr;
				m_pGalaxy->get_VisibleGrid(&pXobj);
				return theAppProxy._createObject<IXobj, DOMPlus::Xobj>(pXobj);
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

		static Cosmos^ m_pCosmos;
		static Form^ m_pMainForm = nullptr;
		static Dictionary<String^, Type^>^ m_pCosmosCLRTypeDic = gcnew Dictionary<String^, Type^>();

		static Cosmos^ InitCosmosApp(bool bSupportCrashReporting, CosmosAppType AppType);
		static Type^ GetType(String^ ObjID);
	public:
		static int CosmosInit(String^ strInit);
		static System::Drawing::Icon^ m_pDefaultIcon = nullptr;
		static String^ GetUIData(Control^ ctrl);
		static String^ GetCtrlTag(Control^ ctrl);
		static Object^ CreateObject(String^ ObjID);
		static Form^ CreateForm(IWin32Window^ parent, String^ ObjID);
		static Xobj^ GetGridFromHandle(IntPtr handle);
		static Xobj^ GetGridFromControl(Control^ ctrl);
		static Xobj^ Observe(Control^ ctrl, String^ key, String^ strGridXml);

		static Cosmos^ GetCosmos();

		static property Xobj^ CreatingGrid
		{
			Xobj^ get();
		}

		delegate void Close();
		static event Close^ OnClose;
		static void Fire_OnClose();

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

		delegate void OpenComplete(IntPtr hWnd, String^ bstrUrl, Xobj^ pRootGrid);
		static event OpenComplete^ OnObserverComplete;
		static void Fire_OnObserverComplete(IntPtr hWnd, String^ bstrUrl, Xobj^ pRootGrid)
		{
			OnObserverComplete(hWnd, bstrUrl, pRootGrid);
		}

		delegate void FormNodeCreated(String^ bstrObjID, Form^ pForm, Xobj^ pXobj);
		static event FormNodeCreated^ OnFormNodeCreated;
		static void Fire_OnFormNodeCreated(String^ bstrObjID, Form^ pForm, Xobj^ pXobj)
		{
			OnFormNodeCreated(bstrObjID, pForm, pXobj);
		}
	};
}

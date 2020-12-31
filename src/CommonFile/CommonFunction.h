/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202101010001
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

#pragma once
#include <atlstr.h>

#define STRING2BSTR(x) (static_cast<BSTR>(Marshal::StringToBSTR(x).ToPointer()))
#define BSTR2STRING(x) (Marshal::PtrToStringUni((System::IntPtr)LPTSTR(LPCTSTR(x))))

class CCommonFunction
{
public:
	//CCommonFunction(void);
	//~CCommonFunction(void);

	template<class T> 
	static BOOL ClearObject(T* pObject)
	{
		BOOL bRet = FALSE;
		if (pObject != NULL)
		{
			ULONG dw = pObject->Release();
			while(dw > 0)
				dw = pObject->Release();
			bRet = TRUE;			
		}
		return bRet;
	}

	template<class T>
	static T* CreateObject()
	{
		CComObject<T>* pv;
		CComObject<T>::CreateInstance(&pv);
		pv->AddRef();
		return pv;
	}

	//BSTR String2BSTR(String^ str)
	//{
	//	IntPtr ip = Marshal::StringToBSTR(str);
	//	BSTR bstr = static_cast<BSTR>(ip.ToPointer());
	//	Marshal::FreeBSTR(ip);
	//	return bstr;
	//}

	//static CString GetApplicationFilename()
	//{
	//	TCHAR _szFile[MAX_PATH * 2] = {0};	
	//	GetModuleFileName(NULL,_szFile,MAX_PATH);

	//	return _szFile;
	//}

	//static CString GetApplicationPath()
	//{
	//	CString strPath = GetApplicationFilename();
	//	int iPos = strPath.ReverseFind(_T('\\'));
	//	return strPath.Left(iPos);
	//}
	
	//static HRESULT InvokeMember(IDispatch* pDisp, BSTR bstrMember, WORD wFlags = DISPATCH_METHOD, 
	//	DISPPARAMS* pParams = NULL, VARIANT* pRet = NULL, 
	//	REFIID riid = IID_NULL, LCID lcid = LOCALE_USER_DEFAULT, 
	//	EXCEPINFO* pExcepInfo = NULL, UINT* puArgErr = NULL)
	//{
	//	HRESULT hr = S_OK;
	//	if (pDisp)
	//	{
	//		DISPID dispid;
	//		hr = pDisp->GetIDsOfNames(IID_NULL, &bstrMember, 1, LOCALE_USER_DEFAULT, &dispid);
	//		if(hr == S_OK)
	//		{
	//			CComVariant varResult;				
	//			hr = pDisp->Invoke(dispid, riid, lcid, wFlags, pParams, pRet, pExcepInfo, puArgErr);				
	//		}
	//	}
	//	else
	//	{
	//		hr = E_FAIL;
	//	}
	//	return S_OK;
	//}
};


// Yaoyao.h : main header file for the Yaoyao application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "yaoyao_h.h"


// CYaoyao:
// See yaoyao.cpp for the implementation of this class
//

class CYaoyao :
	public CCosmosAppEx,
	public CComObjectRootBase,
	public CComCoClass<CYaoyao>,
	public CAtlMfcModule
{
public:
	CYaoyao() noexcept;
	DECLARE_LIBID(LIBID_Yaoyao);

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	// Add by TangramTeam:
	virtual CString GetNTPXml();

	static HRESULT WINAPI UpdateRegistry(BOOL bRegister);
	static HRESULT WINAPI CreateInstance(void* pv, REFIID riid, LPVOID* ppv);
	DECLARE_MESSAGE_MAP()
};

TANGRAM_OBJECT_ENTRY_AUTO(CLSID_Yaoyao, CYaoyao)

extern CYaoyao theApp;

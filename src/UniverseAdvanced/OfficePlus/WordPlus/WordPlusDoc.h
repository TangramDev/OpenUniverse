/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202102050025
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
#include "../OfficeAddin.h"
#include "MSWord.h"
#include "WordPlusEvents.h"
using namespace Word;
using namespace OfficePlus::WordPlus::WordPlusEvent;

namespace OfficePlus
{
	namespace WordPlus
	{
		class CWordObject;
		class CWordAddin;
		class CWordDocument : 
			public CWordDocumentEvents,
			public COfficeDocument,
			public map<HWND, CWordObject*>
		{
		public:
			CWordDocument(void);
			~CWordDocument(void);
			_Document*				m_pDoc;
			map<CString, CString>	m_mapDocUIInfo;

			void __stdcall OnClose();
			void InitDocument();
		};

		class CWordObject :
			public COfficeObject
		{
		public:
			CWordObject(void);

			BOOL					m_bDesignState;
			BOOL					m_bDesignTaskPane;

			CWordDocument*			m_pWordPlusDoc;
			void OnObjDestory();
		};
	}
}

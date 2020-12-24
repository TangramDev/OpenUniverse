/********************************************************************************
 *					DOM Plus for Application - Version 1.1.7.40
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
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

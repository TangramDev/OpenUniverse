/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202012290004
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

#include "../../stdafx.h"
#include "WordPlusEvents.h"

namespace OfficePlus
{
	namespace WordPlus
	{
		namespace WordPlusEvent
		{
			_ATL_FUNC_INFO Startup = { CC_STDCALL,VT_EMPTY,0,NULL };
			_ATL_FUNC_INFO Quit = { CC_STDCALL,VT_EMPTY,0,NULL };
			_ATL_FUNC_INFO DocumentChange = { CC_STDCALL,VT_EMPTY,0,NULL };
			_ATL_FUNC_INFO DocumentOpen = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO DocumentBeforeClose = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO DocumentBeforePrint = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO DocumentBeforeSave = { CC_STDCALL,VT_EMPTY,3,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO NewDocument = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WindowActivate = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WindowDeactivate = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WindowSelectionChange = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WindowBeforeRightClick = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO WindowBeforeDoubleClick = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO EPostagePropertyDialog = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO EPostageInsert = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO MailMergeAfterMerge = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO MailMergeAfterRecordMerge = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO MailMergeBeforeMerge = { CC_STDCALL,VT_EMPTY,4,VT_I4 | VT_BYREF,VT_I4,VT_I4,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO MailMergeBeforeRecordMerge = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO MailMergeDataSourceLoad = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO MailMergeDataSourceValidate = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO MailMergeWizardSendToCustom = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO MailMergeWizardStateChange = { CC_STDCALL,VT_EMPTY,4,VT_I4 | VT_BYREF,VT_INT | VT_BYREF,VT_INT | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO WindowSize = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO XMLSelectionChange = { CC_STDCALL,VT_EMPTY,4,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO XMLValidationError = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO DocumentSync = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 };
			//_ATL_FUNC_INFO EPostageInsertEx = {CC_STDCALL,VT_EMPTY,11,VT_I4|VT_BYREF,VT_INT,VT_INT,VT_INT,VT_INT,VT_INT,VT_INT,VT_BSTR,VT_BSTR,VT_BOOL,VT_BOOL|VT_BYREF};
			_ATL_FUNC_INFO MailMergeDataSourceValidate2 = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO ProtectedViewWindowOpen = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ProtectedViewWindowBeforeEdit = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO ProtectedViewWindowBeforeClose = { CC_STDCALL,VT_EMPTY,3,VT_I4 | VT_BYREF,VT_INT,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO ProtectedViewWindowSize = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ProtectedViewWindowActivate = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ProtectedViewWindowDeactivate = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };


			_ATL_FUNC_INFO New = { CC_STDCALL,VT_EMPTY,0,NULL };
			_ATL_FUNC_INFO Open = { CC_STDCALL,VT_EMPTY,0,NULL };
			_ATL_FUNC_INFO Close = { CC_STDCALL,VT_EMPTY,0,NULL };
			_ATL_FUNC_INFO Sync = { CC_STDCALL,VT_EMPTY,1,VT_I4 };
			_ATL_FUNC_INFO XMLAfterInsert = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL };
			_ATL_FUNC_INFO XMLBeforeDelete = { CC_STDCALL,VT_EMPTY,3,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF,VT_BOOL };
			_ATL_FUNC_INFO ContentControlAfterAdd = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL };
			_ATL_FUNC_INFO ContentControlBeforeDelete = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL };
			_ATL_FUNC_INFO ContentControlOnExit = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO ContentControlOnEnter = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ContentControlBeforeStoreUpdate = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BSTR | VT_BYREF };
			_ATL_FUNC_INFO ContentControlBeforeContentUpdate = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BSTR | VT_BYREF };
			_ATL_FUNC_INFO BuildingBlockInsert = { CC_STDCALL,VT_EMPTY,5,VT_I4 | VT_BYREF,VT_BSTR,VT_BSTR,VT_BSTR,VT_BSTR };
		}
	}
}

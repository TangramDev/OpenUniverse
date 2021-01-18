/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202101180012
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

#include "../../stdafx.h"
#include "ProjectPlusEvents.h"

namespace OfficePlus
{
	namespace ProjectPlus
	{
		namespace ProjectPlusEvent
		{
			_ATL_FUNC_INFO NewProject = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeTaskDelete = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeResourceDelete = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeAssignmentDelete = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeTaskChange = { CC_STDCALL,VT_EMPTY,4,VT_I4 | VT_BYREF,VT_I4,VT_VARIANT,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeResourceChange = { CC_STDCALL,VT_EMPTY,4,VT_I4 | VT_BYREF,VT_I4,VT_VARIANT,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeAssignmentChange = { CC_STDCALL,VT_EMPTY,4,VT_I4 | VT_BYREF,VT_I4,VT_VARIANT,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeTaskNew = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeResourceNew = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeAssignmentNew = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeClose = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforePrint = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeSave = { CC_STDCALL,VT_EMPTY,3,VT_I4 | VT_BYREF,VT_BOOL,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO ProjectCalculate = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };

			_ATL_FUNC_INFO WindowGoalAreaChange = { CC_STDCALL,VT_EMPTY,1,VT_I4 };
			_ATL_FUNC_INFO WindowSelectionChange = { CC_STDCALL,VT_EMPTY,3,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF,VT_VARIANT };
			_ATL_FUNC_INFO WindowBeforeViewChange = { CC_STDCALL,VT_EMPTY,5,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF,VT_BOOL,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WindowViewChange = { CC_STDCALL,VT_EMPTY,4,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF,VT_BOOL };
			_ATL_FUNC_INFO WindowActivate = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WindowDeactivate = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WindowSidepaneDisplayChange = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL };
			_ATL_FUNC_INFO WindowSidepaneTaskChange = { CC_STDCALL,VT_EMPTY,3,VT_I4 | VT_BYREF,VT_I4,VT_BOOL };
			_ATL_FUNC_INFO WorkpaneDisplayChange = { CC_STDCALL,VT_EMPTY,1,VT_BOOL };
			_ATL_FUNC_INFO LoadWebPage = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BSTR | VT_BYREF };
			_ATL_FUNC_INFO ProjectAfterSave = { CC_STDCALL,VT_EMPTY,0,NULL };
			_ATL_FUNC_INFO ProjectTaskNew = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 };
			_ATL_FUNC_INFO ProjectResourceNew = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 };
			_ATL_FUNC_INFO ProjectAssignmentNew = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 };
			//_ATL_FUNC_INFO ProjectBeforeSaveBaseline = {CC_STDCALL,VT_EMPTY,9,
			//	VT_I4|VT_BYREF,
			//	VT_BOOL,
			//	VT_I2,
			//	VT_I2,
			//	VT_I2,
			//	VT_BOOL,
			//	VT_BOOL,
			//	VT_BOOL,
			//	VT_I4|VT_BYREF};
			_ATL_FUNC_INFO ProjectBeforeClearBaseline = { CC_STDCALL,VT_EMPTY,6,VT_I4 | VT_BYREF,VT_BOOL,VT_I4,VT_I4,VT_BOOL,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeClose2 = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforePrint2 = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeSave2 = { CC_STDCALL,VT_EMPTY,3,VT_I4 | VT_BYREF,VT_BOOL,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeTaskDelete2 = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeResourceDelete2 = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeAssignmentDelete2 = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeTaskChange2 = { CC_STDCALL,VT_EMPTY,4,VT_I4 | VT_BYREF,VT_I4,VT_VARIANT,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeResourceChange2 = { CC_STDCALL,VT_EMPTY,4,VT_I4 | VT_BYREF,VT_I4,VT_VARIANT,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeAssignmentChange2 = { CC_STDCALL,VT_EMPTY,4,VT_I4 | VT_BYREF,VT_I4,VT_VARIANT,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeTaskNew2 = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeResourceNew2 = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ProjectBeforeAssignmentNew2 = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ApplicationBeforeClose = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO OnUndoOrRedo = { CC_STDCALL,VT_EMPTY,3,VT_BSTR, VT_BSTR,VT_BOOL };
			_ATL_FUNC_INFO AfterCubeBuilt = { CC_STDCALL,VT_EMPTY,1,VT_BSTR | VT_BYREF };
			_ATL_FUNC_INFO LoadWebPane = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BSTR | VT_BYREF };
			_ATL_FUNC_INFO JobStart = { CC_STDCALL,VT_EMPTY,5,VT_BSTR, VT_BSTR,VT_BSTR,VT_I4,VT_I4 };
			_ATL_FUNC_INFO JobCompleted = { CC_STDCALL,VT_EMPTY,5,VT_BSTR, VT_BSTR,VT_BSTR,VT_I4,VT_I4 };
			_ATL_FUNC_INFO SaveStartingToServer = { CC_STDCALL,VT_EMPTY,2,VT_BSTR, VT_BSTR };
			_ATL_FUNC_INFO SaveCompletedToServer = { CC_STDCALL,VT_EMPTY,2,VT_BSTR, VT_BSTR };
			_ATL_FUNC_INFO ProjectBeforePublish = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO PaneActivate = { CC_STDCALL,VT_EMPTY,0,NULL };
			_ATL_FUNC_INFO SecondaryViewChange = { CC_STDCALL,VT_EMPTY,4,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF,VT_BOOL };
			_ATL_FUNC_INFO IsFunctionalitySupported = { CC_STDCALL,VT_EMPTY,2,VT_BSTR, VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ConnectionStatusChanged = { CC_STDCALL,VT_EMPTY,1,VT_BOOL };

			_ATL_FUNC_INFO Open = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO BeforeClose = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO BeforeSave = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO BeforePrint = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO Calculate = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO Change = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO Activate = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO Deactivate = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
		}
	}
}
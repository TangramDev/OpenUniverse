/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202103100048
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
#include "ExcelPlusEvents.h"

namespace OfficePlus
{
	namespace ExcelPlus
	{
		namespace ExcelPlusEvent
		{
			_ATL_FUNC_INFO Open = { CC_STDCALL,VT_EMPTY,0,NULL };
			_ATL_FUNC_INFO Activate = { CC_STDCALL,VT_EMPTY,0,NULL };
			_ATL_FUNC_INFO Deactivate = { CC_STDCALL,VT_EMPTY,0,NULL };
			_ATL_FUNC_INFO BeforeClose = { CC_STDCALL,VT_EMPTY,1,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO BeforeSave = { CC_STDCALL,VT_EMPTY,2,VT_BOOL,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO BeforePrint = { CC_STDCALL,VT_EMPTY,1,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO NewSheet = { CC_STDCALL,VT_EMPTY,1,VT_DISPATCH };
			_ATL_FUNC_INFO AddinInstall = { CC_STDCALL,VT_EMPTY,0,NULL };
			_ATL_FUNC_INFO AddinUninstall = { CC_STDCALL,VT_EMPTY,0,NULL };
			_ATL_FUNC_INFO PivotTableCloseConnection = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO PivotTableOpenConnection = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO BeforeXmlImport = { CC_STDCALL,VT_EMPTY,4,VT_I4 | VT_BYREF,VT_BSTR,VT_BOOL,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO AfterXmlImport = { CC_STDCALL,VT_EMPTY,3,VT_I4 | VT_BYREF,VT_BOOL,VT_I4 };
			_ATL_FUNC_INFO BeforeXmlExport = { CC_STDCALL,VT_EMPTY,3,VT_I4 | VT_BYREF,VT_BSTR,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO AfterXmlExport = { CC_STDCALL,VT_EMPTY,3,VT_I4 | VT_BYREF,VT_BSTR,VT_I4 };
			_ATL_FUNC_INFO RowsetComplete = { CC_STDCALL,VT_EMPTY,3,VT_BSTR,VT_BSTR,VT_BOOL };
			_ATL_FUNC_INFO AfterSave = { CC_STDCALL,VT_EMPTY,1,VT_BOOL };
			_ATL_FUNC_INFO NewChart = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ModelChange = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO SheetPivotTableChangeSync = { CC_STDCALL,VT_EMPTY,2,VT_DISPATCH,VT_I4 | VT_BYREF };

			_ATL_FUNC_INFO WindowResize2 = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WindowActivate2 = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WindowDeactivate2 = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO SheetSelectionChange2 = { CC_STDCALL,VT_EMPTY,2,VT_DISPATCH,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO SheetBeforeDoubleClick2 = { CC_STDCALL,VT_EMPTY,3,VT_DISPATCH,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO SheetBeforeRightClick2 = { CC_STDCALL,VT_EMPTY,3,VT_DISPATCH,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO SheetActivate2 = { CC_STDCALL,VT_EMPTY,1,VT_DISPATCH };
			_ATL_FUNC_INFO SheetDeactivate2 = { CC_STDCALL,VT_EMPTY,1,VT_DISPATCH };
			_ATL_FUNC_INFO SheetCalculate2 = { CC_STDCALL,VT_EMPTY,1,VT_DISPATCH };
			_ATL_FUNC_INFO SheetChange2 = { CC_STDCALL,VT_EMPTY,2,VT_DISPATCH,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO SheetFollowHyperlink2 = { CC_STDCALL,VT_EMPTY,2,VT_DISPATCH,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO SheetPivotTableUpdate2 = { CC_STDCALL,VT_EMPTY,2,VT_DISPATCH,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO PivotTableCloseConnection2 = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO PivotTableOpenConnection2 = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO Sync2 = { CC_STDCALL,VT_EMPTY,1,VT_I4 };
			_ATL_FUNC_INFO BeforeXmlImport2 = { CC_STDCALL,VT_EMPTY,4,VT_I4 | VT_BYREF,VT_BSTR,VT_BOOL,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO AfterXmlImport2 = { CC_STDCALL,VT_EMPTY,3,VT_I4 | VT_BYREF,VT_BOOL,VT_I4 };
			_ATL_FUNC_INFO BeforeXmlExport2 = { CC_STDCALL,VT_EMPTY,3,VT_I4 | VT_BYREF,VT_BSTR,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO AfterXmlExport2 = { CC_STDCALL,VT_EMPTY,3,VT_I4 | VT_BYREF,VT_BSTR,VT_I4 };
			_ATL_FUNC_INFO RowsetComplete2 = { CC_STDCALL,VT_EMPTY,3,VT_BSTR,VT_BSTR,VT_BOOL };
			_ATL_FUNC_INFO SheetPivotTableAfterValueChange2 = { CC_STDCALL,VT_EMPTY,3,VT_DISPATCH,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO SheetPivotTableBeforeAllocateChanges2 = { CC_STDCALL,VT_EMPTY,5,VT_DISPATCH,VT_I4 | VT_BYREF,VT_I4,VT_I4,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO SheetPivotTableBeforeCommitChanges2 = { CC_STDCALL,VT_EMPTY,5,VT_DISPATCH,VT_I4 | VT_BYREF,VT_I4,VT_I4,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO SheetPivotTableBeforeDiscardChanges2 = { CC_STDCALL,VT_EMPTY,4,VT_DISPATCH,VT_I4 | VT_BYREF,VT_I4,VT_I4 };
			_ATL_FUNC_INFO SheetPivotTableChangeSync2 = { CC_STDCALL,VT_EMPTY,2,VT_DISPATCH,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO AfterSave2 = { CC_STDCALL,VT_EMPTY,1,VT_BOOL };
			_ATL_FUNC_INFO NewChart2 = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO SheetLensGalleryRenderComplete2 = { CC_STDCALL,VT_EMPTY,1,VT_DISPATCH };
			_ATL_FUNC_INFO SheetTableUpdate2 = { CC_STDCALL,VT_EMPTY,2,VT_DISPATCH,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ModelChange2 = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO SheetBeforeDelete2 = { CC_STDCALL,VT_EMPTY,1,VT_DISPATCH };

			_ATL_FUNC_INFO SelectionChange = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO BeforeDoubleClick2 = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO BeforeRightClick = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO Calculate = { CC_STDCALL,VT_EMPTY,0,NULL };
			_ATL_FUNC_INFO Change = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO FollowHyperlink = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO PivotTableUpdate = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO PivotTableAfterValueChange = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO PivotTableBeforeAllocateChanges = { CC_STDCALL,VT_EMPTY,4,VT_I4 | VT_BYREF,VT_I4,VT_I4,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO PivotTableBeforeCommitChanges = { CC_STDCALL,VT_EMPTY,4,VT_I4 | VT_BYREF,VT_I4,VT_I4,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO PivotTableBeforeDiscardChanges = { CC_STDCALL,VT_EMPTY,3,VT_I4 | VT_BYREF,VT_I4,VT_I4 };
			_ATL_FUNC_INFO PivotTableChangeSync = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO LensGalleryRenderComplete = { CC_STDCALL,VT_EMPTY,0,NULL };
			_ATL_FUNC_INFO TableUpdate = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO BeforeDelete = { CC_STDCALL,VT_EMPTY,0,NULL };

			_ATL_FUNC_INFO Resize = { CC_STDCALL,VT_EMPTY,0,NULL };
			_ATL_FUNC_INFO MouseDown = { CC_STDCALL,VT_EMPTY,4,VT_I4,VT_I4,VT_I4,VT_I4 };
			_ATL_FUNC_INFO MouseUp = { CC_STDCALL,VT_EMPTY,4,VT_I4,VT_I4,VT_I4,VT_I4 };
			_ATL_FUNC_INFO MouseMove = { CC_STDCALL,VT_EMPTY,4,VT_I4,VT_I4,VT_I4,VT_I4 };
			_ATL_FUNC_INFO BeforeRightClick2 = { CC_STDCALL,VT_EMPTY,1,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO DragPlot = { CC_STDCALL,VT_EMPTY,0,NULL };
			_ATL_FUNC_INFO DragOver = { CC_STDCALL,VT_EMPTY,0,NULL };
			_ATL_FUNC_INFO BeforeDoubleClick = { CC_STDCALL,VT_EMPTY,4,VT_I4,VT_I4,VT_I4,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO Select = { CC_STDCALL,VT_EMPTY,3,VT_I4,VT_I4,VT_I4 };
			_ATL_FUNC_INFO SeriesChange = { CC_STDCALL,VT_EMPTY,2,VT_I4,VT_I4 };

			_ATL_FUNC_INFO NewWorkbook = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO SheetSelectionChange = { CC_STDCALL,VT_EMPTY,2,VT_DISPATCH,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO SheetBeforeDoubleClick = { CC_STDCALL,VT_EMPTY,3,VT_DISPATCH,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO SheetBeforeRightClick = { CC_STDCALL,VT_EMPTY,3,VT_DISPATCH,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO SheetActivate = { CC_STDCALL,VT_EMPTY,1,VT_DISPATCH };
			_ATL_FUNC_INFO SheetDeactivate = { CC_STDCALL,VT_EMPTY,1,VT_DISPATCH };
			_ATL_FUNC_INFO SheetCalculate = { CC_STDCALL,VT_EMPTY,1,VT_DISPATCH };
			_ATL_FUNC_INFO SheetChange = { CC_STDCALL,VT_EMPTY,2,VT_DISPATCH,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WorkbookOpen = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WorkbookActivate = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WorkbookDeactivate = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WorkbookBeforeClose = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO WorkbookBeforeSave = { CC_STDCALL,VT_EMPTY,3,VT_I4 | VT_BYREF,VT_BOOL,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO WorkbookBeforePrint = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO WorkbookNewSheet = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_DISPATCH };
			_ATL_FUNC_INFO WorkbookAddinInstall = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WorkbookAddinUninstall = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WindowResize = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WindowActivate = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WindowDeactivate = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO SheetFollowHyperlink = { CC_STDCALL,VT_EMPTY,2,VT_DISPATCH,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO SheetPivotTableUpdate = { CC_STDCALL,VT_EMPTY,2,VT_DISPATCH,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WorkbookPivotTableCloseConnection = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WorkbookPivotTableOpenConnection = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WorkbookSync = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 };
			_ATL_FUNC_INFO WorkbookBeforeXmlImport = { CC_STDCALL,VT_EMPTY,5,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF,VT_BSTR,VT_BOOL,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO WorkbookAfterXmlImport = { CC_STDCALL,VT_EMPTY,4,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF,VT_BOOL,VT_I4 };
			_ATL_FUNC_INFO WorkbookBeforeXmlExport = { CC_STDCALL,VT_EMPTY,4,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF,VT_BSTR,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO WorkbookAfterXmlExport = { CC_STDCALL,VT_EMPTY,4,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF,VT_BSTR,VT_I4 };
			_ATL_FUNC_INFO WorkbookRowsetComplete = { CC_STDCALL,VT_EMPTY,4,VT_I4 | VT_BYREF,VT_BSTR,VT_BSTR,VT_BOOL };
			_ATL_FUNC_INFO AfterCalculate = { CC_STDCALL,VT_EMPTY,0,NULL };
			_ATL_FUNC_INFO SheetPivotTableAfterValueChange = { CC_STDCALL,VT_EMPTY,3,VT_DISPATCH,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO SheetPivotTableBeforeAllocateChanges = { CC_STDCALL,VT_EMPTY,5,VT_DISPATCH,VT_I4 | VT_BYREF,VT_I4,VT_I4,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO SheetPivotTableBeforeCommitChanges = { CC_STDCALL,VT_EMPTY,5,VT_DISPATCH,VT_I4 | VT_BYREF,VT_I4,VT_I4,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO SheetPivotTableBeforeDiscardChanges = { CC_STDCALL,VT_EMPTY,4,VT_DISPATCH,VT_I4 | VT_BYREF,VT_I4,VT_I4 };
			_ATL_FUNC_INFO ProtectedViewWindowOpen = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ProtectedViewWindowBeforeEdit = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO ProtectedViewWindowBeforeClose = { CC_STDCALL,VT_EMPTY,3,VT_I4 | VT_BYREF,VT_I4,VT_BOOL | VT_BYREF };
			_ATL_FUNC_INFO ProtectedViewWindowResize = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ProtectedViewWindowActivate = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO ProtectedViewWindowDeactivate = { CC_STDCALL,VT_EMPTY,1,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WorkbookAfterSave = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_BOOL };
			_ATL_FUNC_INFO WorkbookNewChart = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO SheetLensGalleryRenderComplete = { CC_STDCALL,VT_EMPTY,1,VT_DISPATCH };
			_ATL_FUNC_INFO SheetTableUpdate = { CC_STDCALL,VT_EMPTY,2,VT_DISPATCH,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO WorkbookModelChange = { CC_STDCALL,VT_EMPTY,2,VT_I4 | VT_BYREF,VT_I4 | VT_BYREF };
			_ATL_FUNC_INFO SheetBeforeDelete = { CC_STDCALL,VT_EMPTY,1,VT_DISPATCH };
		}
	}
}
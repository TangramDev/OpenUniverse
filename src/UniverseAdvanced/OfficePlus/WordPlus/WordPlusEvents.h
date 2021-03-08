/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202103080046
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
#include "msword.h"

using namespace Office;
using namespace Word; 

namespace OfficePlus
{
	namespace WordPlus
	{
		namespace WordPlusEvent
		{
			extern _ATL_FUNC_INFO BuildingBlockInsert;
			extern _ATL_FUNC_INFO Close;
			extern _ATL_FUNC_INFO ContentControlAfterAdd;
			extern _ATL_FUNC_INFO ContentControlBeforeDelete;
			extern _ATL_FUNC_INFO ContentControlOnExit;
			extern _ATL_FUNC_INFO ContentControlOnEnter;
			extern _ATL_FUNC_INFO ContentControlBeforeStoreUpdate;
			extern _ATL_FUNC_INFO ContentControlBeforeContentUpdate;
			extern _ATL_FUNC_INFO DocumentChange;
			extern _ATL_FUNC_INFO DocumentOpen;
			extern _ATL_FUNC_INFO DocumentBeforeClose;
			extern _ATL_FUNC_INFO DocumentBeforePrint;
			extern _ATL_FUNC_INFO DocumentBeforeSave;
			extern _ATL_FUNC_INFO DocumentSync;
			extern _ATL_FUNC_INFO EPostagePropertyDialog;
			extern _ATL_FUNC_INFO EPostageInsert;
			extern _ATL_FUNC_INFO MailMergeAfterMerge;
			extern _ATL_FUNC_INFO MailMergeAfterRecordMerge;
			extern _ATL_FUNC_INFO MailMergeBeforeMerge;
			extern _ATL_FUNC_INFO MailMergeBeforeRecordMerge;
			extern _ATL_FUNC_INFO MailMergeDataSourceLoad;
			extern _ATL_FUNC_INFO MailMergeDataSourceValidate;
			extern _ATL_FUNC_INFO MailMergeWizardSendToCustom;
			extern _ATL_FUNC_INFO MailMergeWizardStateChange;
			extern _ATL_FUNC_INFO MailMergeDataSourceValidate2;
			extern _ATL_FUNC_INFO New;
			extern _ATL_FUNC_INFO NewDocument;
			extern _ATL_FUNC_INFO Open;
			extern _ATL_FUNC_INFO ProtectedViewWindowOpen;
			extern _ATL_FUNC_INFO ProtectedViewWindowBeforeEdit;
			extern _ATL_FUNC_INFO ProtectedViewWindowBeforeClose;
			extern _ATL_FUNC_INFO ProtectedViewWindowSize;
			extern _ATL_FUNC_INFO ProtectedViewWindowActivate;
			extern _ATL_FUNC_INFO ProtectedViewWindowDeactivate;
			extern _ATL_FUNC_INFO Quit;
			extern _ATL_FUNC_INFO Startup;
			extern _ATL_FUNC_INFO Sync;
			extern _ATL_FUNC_INFO WindowSize;
			extern _ATL_FUNC_INFO WindowActivate;
			extern _ATL_FUNC_INFO WindowDeactivate;
			extern _ATL_FUNC_INFO WindowSelectionChange;
			extern _ATL_FUNC_INFO WindowBeforeRightClick;
			extern _ATL_FUNC_INFO WindowBeforeDoubleClick;
			extern _ATL_FUNC_INFO XMLSelectionChange;
			extern _ATL_FUNC_INFO XMLValidationError;
			extern _ATL_FUNC_INFO XMLAfterInsert;
			extern _ATL_FUNC_INFO XMLBeforeDelete;

			class CWordAppEvents2 : public IDispEventSimpleImpl</*nID =*/ 1, CWordAppEvents2, &__uuidof(ApplicationEvents2)>
			{
			public:
				virtual void __stdcall OnStartup() {}
				virtual void __stdcall OnQuit() {}
				virtual void __stdcall OnDocumentChange() {}
				virtual void __stdcall OnDocumentOpen(_Document* Doc) {}
				virtual void __stdcall OnDocumentBeforeClose(_Document* Doc, VARIANT_BOOL* Cancel) {}
				virtual void __stdcall OnDocumentBeforePrint(_Document* Doc, VARIANT_BOOL* Cancel) {}
				virtual void __stdcall OnDocumentBeforeSave(_Document* Doc, VARIANT_BOOL* SaveAsUI, VARIANT_BOOL* Cancel) {}
				virtual void __stdcall OnNewDocument(_Document* Doc) {}
				virtual void __stdcall OnWindowActivate(_Document* Doc, Word::Window* Wn) {}
				virtual void __stdcall OnWindowDeactivate(_Document* Doc, Word::Window* Wn) {}
				virtual void __stdcall OnWindowSelectionChange(Selection* Sel) {}
				virtual void __stdcall OnWindowBeforeRightClick(Selection* Sel, VARIANT_BOOL* Cancel) {}
				virtual void __stdcall OnWindowBeforeDoubleClick(Selection* Sel, VARIANT_BOOL* Cancel) {}

				BEGIN_SINK_MAP(CWordAppEvents2)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents2), /*dispid =*/ 0x00000001, OnStartup, &Startup)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents2), /*dispid =*/ 0x00000002, OnQuit, &Quit)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents2), /*dispid =*/ 0x00000003, OnDocumentChange, &DocumentChange)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents2), /*dispid =*/ 0x00000004, OnDocumentOpen, &DocumentOpen)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents2), /*dispid =*/ 0x00000006, OnDocumentBeforeClose, &DocumentBeforeClose)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents2), /*dispid =*/ 0x00000007, OnDocumentBeforePrint, &DocumentBeforePrint)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents2), /*dispid =*/ 0x00000008, OnDocumentBeforeSave, &DocumentBeforeSave)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents2), /*dispid =*/ 0x00000009, OnNewDocument, &NewDocument)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents2), /*dispid =*/ 0x0000000a, OnWindowActivate, &WindowActivate)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents2), /*dispid =*/ 0x0000000b, OnWindowDeactivate, &WindowDeactivate)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents2), /*dispid =*/ 0x0000000c, OnWindowSelectionChange, &WindowSelectionChange)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents2), /*dispid =*/ 0x0000000d, OnWindowBeforeRightClick, &WindowBeforeRightClick)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents2), /*dispid =*/ 0x0000000e, OnWindowBeforeDoubleClick, &WindowBeforeDoubleClick)
				END_SINK_MAP()
			};

			class CWordAppEvents3 : public IDispEventSimpleImpl</*nID =*/ 1, CWordAppEvents3, &__uuidof(ApplicationEvents3)>
			{
			public:
				virtual void __stdcall OnEPostagePropertyDialog(_Document* Doc) {}
				virtual void __stdcall OnEPostageInsert(_Document* Doc) {}
				virtual void __stdcall OnMailMergeAfterMerge(_Document* Doc, Word::Document* DocResult) {}
				virtual void __stdcall OnMailMergeAfterRecordMerge(_Document* Doc) {}
				virtual void __stdcall OnMailMergeBeforeMerge(_Document* Doc, long StartRecord, long EndRecord, VARIANT_BOOL* Cancel) {}
				virtual void __stdcall OnMailMergeBeforeRecordMerge(_Document* Doc, VARIANT_BOOL* Cancel) {}
				virtual void __stdcall OnMailMergeDataSourceLoad(_Document* Doc) {}
				virtual void __stdcall OnMailMergeDataSourceValidate(_Document* Doc, VARIANT_BOOL* Handled) {}
				virtual void __stdcall OnMailMergeWizardSendToCustom(_Document* Doc) {}
				virtual void __stdcall OnMailMergeWizardStateChange(_Document* Doc, int* FromState, int* ToState, VARIANT_BOOL* Handled) {}
				virtual void __stdcall OnWindowSize(_Document* Doc, Word::Window* Wn) {}

				BEGIN_SINK_MAP(CWordAppEvents3)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents3), /*dispid =*/ 0x0000000f, OnEPostagePropertyDialog, &EPostagePropertyDialog)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents3), /*dispid =*/ 0x00000010, OnEPostageInsert, &EPostageInsert)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents3), /*dispid =*/ 0x00000011, OnMailMergeAfterMerge, &MailMergeAfterMerge)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents3), /*dispid =*/ 0x00000012, OnMailMergeAfterRecordMerge, &MailMergeAfterRecordMerge)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents3), /*dispid =*/ 0x00000013, OnMailMergeBeforeMerge, &MailMergeBeforeMerge)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents3), /*dispid =*/ 0x00000014, OnMailMergeBeforeRecordMerge, &MailMergeBeforeRecordMerge)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents3), /*dispid =*/ 0x00000015, OnMailMergeDataSourceLoad, &MailMergeDataSourceLoad)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents3), /*dispid =*/ 0x00000016, OnMailMergeDataSourceValidate, &MailMergeDataSourceValidate)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents3), /*dispid =*/ 0x00000017, OnMailMergeWizardSendToCustom, &MailMergeWizardSendToCustom)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents3), /*dispid =*/ 0x00000018, OnMailMergeWizardStateChange, &MailMergeWizardStateChange)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents3), /*dispid =*/ 0x00000019, OnWindowSize, &WindowSize)
				END_SINK_MAP()
			};

			class CWordAppEvents4 : public IDispEventSimpleImpl</*nID =*/ 1, CWordAppEvents4, &__uuidof(ApplicationEvents4)>
			{
			public:
				virtual void __stdcall OnXMLSelectionChange(Word::Selection* Sel, XMLNode* OldXMLNode, XMLNode* NewXMLNode, long* Reason) {}
				virtual void __stdcall OnXMLValidationError(XMLNode* XMLNode) {}
				virtual void __stdcall OnDocumentSync(_Document* Doc, MsoSyncEventType SyncEvent) {}
				virtual void __stdcall OnMailMergeDataSourceValidate2(_Document* Doc, VARIANT_BOOL* Handled) {}
				virtual void __stdcall OnProtectedViewWindowOpen(ProtectedViewWindow* PvWindow) {}
				virtual void __stdcall OnProtectedViewWindowBeforeEdit(ProtectedViewWindow* PvWindow, VARIANT_BOOL* Cancel) {}
				virtual void __stdcall OnProtectedViewWindowBeforeClose(ProtectedViewWindow* PvWindow, int CloseReason, VARIANT_BOOL* Cancel) {}
				virtual void __stdcall OnProtectedViewWindowSize(ProtectedViewWindow* PvWindow) {}
				virtual void __stdcall OnProtectedViewWindowActivate(ProtectedViewWindow* PvWindow) {}
				virtual void __stdcall OnProtectedViewWindowDeactivate(ProtectedViewWindow* PvWindow) {}

				BEGIN_SINK_MAP(CWordAppEvents4)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents4), /*dispid =*/ 0x0000001a, OnXMLSelectionChange, &XMLSelectionChange)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents4), /*dispid =*/ 0x0000001b, OnXMLValidationError, &XMLValidationError)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents4), /*dispid =*/ 0x0000001c, OnDocumentSync, &DocumentSync)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents4), /*dispid =*/ 0x0000001e, OnMailMergeDataSourceValidate2, &MailMergeDataSourceValidate2)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents4), /*dispid =*/ 0x0000001f, OnProtectedViewWindowOpen, &ProtectedViewWindowOpen)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents4), /*dispid =*/ 0x00000020, OnProtectedViewWindowBeforeEdit, &ProtectedViewWindowBeforeEdit)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents4), /*dispid =*/ 0x00000021, OnProtectedViewWindowBeforeClose, &ProtectedViewWindowBeforeClose)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents4), /*dispid =*/ 0x00000022, OnProtectedViewWindowSize, &ProtectedViewWindowSize)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents4), /*dispid =*/ 0x00000023, OnProtectedViewWindowActivate, &ProtectedViewWindowActivate)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(ApplicationEvents4), /*dispid =*/ 0x00000024, OnProtectedViewWindowDeactivate, &ProtectedViewWindowDeactivate)
				END_SINK_MAP()
			};

			class CWordDocumentEvents : public IDispEventSimpleImpl</*nID =*/ 1, CWordDocumentEvents, &__uuidof(Word::DocumentEvents)>
			{
			public:
				virtual void __stdcall OnNew() {}
				virtual void __stdcall OnOpen() {}
				virtual void __stdcall OnClose() {}

				BEGIN_SINK_MAP(CWordDocumentEvents)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(Word::DocumentEvents), /*dispid =*/ 0x00000004, OnNew, &New)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(Word::DocumentEvents), /*dispid =*/ 0x00000005, OnOpen, &Open)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(Word::DocumentEvents), /*dispid =*/ 0x00000006, OnClose, &Close)
				END_SINK_MAP()
			};

			class CWordDocumentEvents2 : public IDispEventSimpleImpl</*nID =*/ 1, CWordDocumentEvents2, &__uuidof(DocumentEvents2)>
			{
			public:
				virtual void __stdcall OnSync(MsoSyncEventType SyncEvent) {}
				virtual void __stdcall OnXMLAfterInsert(XMLNode* NewXMLNode, VARIANT_BOOL InUndoRedo) {}
				virtual void __stdcall OnXMLBeforeDelete(Word::Range* DeletedRange, XMLNode* OldXMLNode, VARIANT_BOOL InUndoRedo) {}
				virtual void __stdcall OnContentControlAfterAdd(ContentControl* NewContentControl, VARIANT_BOOL InUndoRedo) {}
				virtual void __stdcall OnContentControlBeforeDelete(ContentControl* OldContentControl, VARIANT_BOOL InUndoRedo) {}
				virtual void __stdcall OnContentControlOnExit(ContentControl* ContentControl, VARIANT_BOOL* Cancel) {}
				virtual void __stdcall OnContentControlOnEnter(ContentControl* ContentControl) {}
				virtual void __stdcall OnContentControlBeforeStoreUpdate(ContentControl* ContentControl, BSTR* Content) {}
				virtual void __stdcall OnContentControlBeforeContentUpdate(ContentControl* ContentControl, BSTR* Content) {}
				virtual void __stdcall OnBuildingBlockInsert(Word::Range* Range, BSTR Name, BSTR Category, BSTR BlockType, BSTR Template) {}

				BEGIN_SINK_MAP(CWordDocumentEvents2)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(DocumentEvents2), /*dispid =*/ 0x00000007, OnSync, &Sync)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(DocumentEvents2), /*dispid =*/ 0x00000008, OnXMLAfterInsert, &XMLAfterInsert)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(DocumentEvents2), /*dispid =*/ 0x00000009, OnXMLBeforeDelete, &XMLBeforeDelete)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(DocumentEvents2), /*dispid =*/ 0x0000000c, OnContentControlAfterAdd, &ContentControlAfterAdd)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(DocumentEvents2), /*dispid =*/ 0x0000000d, OnContentControlBeforeDelete, &ContentControlBeforeDelete)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(DocumentEvents2), /*dispid =*/ 0x0000000e, OnContentControlOnExit, &ContentControlOnExit)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(DocumentEvents2), /*dispid =*/ 0x0000000f, OnContentControlOnEnter, &ContentControlOnEnter)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(DocumentEvents2), /*dispid =*/ 0x00000010, OnContentControlBeforeStoreUpdate, &ContentControlBeforeStoreUpdate)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(DocumentEvents2), /*dispid =*/ 0x00000011, OnContentControlBeforeContentUpdate, &ContentControlBeforeContentUpdate)
					SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(DocumentEvents2), /*dispid =*/ 0x00000012, OnBuildingBlockInsert, &BuildingBlockInsert)
				END_SINK_MAP()
			};
		}
	}
}
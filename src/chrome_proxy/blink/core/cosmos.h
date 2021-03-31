/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202103310057           *
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
 * Use of this source code is governed by a BSD-style license that
 * can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *******************************************************************************/

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_H_

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <iterator>
#include <regex>

#include "base/run_loop.h"
#include "third_party/blink/renderer/bindings/core/v8/script_value.h"
#include "third_party/blink/renderer/bindings/core/v8/serialization/transferables.h"
#include "third_party/blink/renderer/core/execution_context/context_lifecycle_observer.h"
#include "third_party/blink/renderer/core/dom/events/event_target.h"
#include "third_party/blink/renderer/core/event_type_names.h"

#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/assertions.h"
#include "third_party/blink/renderer/platform/wtf/forward.h"
#include "third_party/blink/renderer/platform/wtf/uuid.h"
#include "cosmos_xobj.h"

#define IPC_CLR_CONTROL_CREARED							20200220
#define IPC_CLR_CONTROL_CREARED_ID						_T("Cosmos_CLR_Control_Created")
#define IPC_NODE_CREARED								20200221
#define IPC_NODE_CREARED_ID								_T("Cosmos_WndXobj_Created")
#define IPC_NODE_ONMOUSEACTIVATE						20200222
#define IPC_NODE_ONMOUSEACTIVATE_ID						_T("Cosmos_WndXobj_OnMouseActivate")
#define IPC_MDIWINFORM_ACTIVEMDICHILD					20200224
#define IPC_MDIWINFORM_ACTIVEMDICHILD_ID				_T("MdiWinForm_ActiveMdiChild")

#define IPC_HUBBLE_MSG_BASE								20201202
#define IPC_HUBBLE_MSG_BindCLRObject					(IPC_HUBBLE_MSG_BASE + 1)
#define IPC_HUBBLE_MSG_FIRE_EVENT						(IPC_HUBBLE_MSG_BASE + 2)
#define IPC_HUBBLE_MSG_WINFORM_CREATED					(IPC_HUBBLE_MSG_BASE + 3)
#define IPC_HUBBLE_MSG_WINFORM_ONCLOSE					(IPC_HUBBLE_MSG_BASE + 4)
#define IPC_HUBBLE_MSG_Tangram_WndXobj_Created			(IPC_HUBBLE_MSG_BASE + 5)
#define IPC_HUBBLE_MSG_BIND_NATIVEOBJ_IPC_MSG			(IPC_HUBBLE_MSG_BASE + 6)
#define IPC_HUBBLE_MSG_MdiWinForm_ActiveMdiChild		(IPC_HUBBLE_MSG_BASE + 7)
#define IPC_HUBBLE_MSG_MdiWinForm_Ready					(IPC_HUBBLE_MSG_BASE + 8)
#define IPC_HUBBLE_MSG_MDIFORM_ALLMDICHILDREMOVED		(IPC_HUBBLE_MSG_BASE + 9)
#define IPC_HUBBLE_MSG_OPEN_XML_SPLITTER				(IPC_HUBBLE_MSG_BASE + 10)

namespace blink {
	using namespace std;

	class Element;
	class LocalFrame;
	class Document;
	class ScriptState;
	class CosmosXobj;
	class CosmosNode;
	class CosmosEvent;
	class CosmosGalaxy;
	class CosmosControl;
	class CosmosWinform;
	class ExceptionState;
	class CosmosCompositor;
	class DocumentFragment;
	class WebLocalFrameClient;
	class SerializedScriptValue;

	//for callback:
	class CallbackFunctionBase;

	class V8ApplicationCallback;

	class CORE_EXPORT Cosmos final :
	public CosmosXobj,
		public DOMWindowClient{
	DEFINE_WRAPPERTYPEINFO();
	USING_GARBAGE_COLLECTED_MIXIN(Cosmos);
		friend  class CosmosWinform;

	public:
	 static Cosmos* Create(LocalFrame* frame) { return MakeGarbageCollected<Cosmos>(frame); }

	 void Trace(blink::Visitor*) override;

	 // Called when an event listener has been successfully added.
	 void AddedEventListener(const AtomicString& event_type,
							 RegisteredEventListener&) override;

	 DEFINE_ATTRIBUTE_EVENT_LISTENER(MessageReceived, kCosmos)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(CosmosMessageReceived, kCosmosmessage)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(MdiChildActivate, kMdichildactivate)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(XobjCreated, kXobjcreated)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(GalaxyCreated, kCosmosgalaxycreated)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(LoadCloudForm, kLoadcloudform)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(LoadMdiCloudForm, kLoadmdicloudform)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(CloudFormClosed, kCloudformclosed)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(CosmosObjectCreated, kCosmosobjectcreated)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(BindCLRObject, kBindclrobject)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(CloudMessageForObject, kCloudmessageforobject)

	 String url();
	 CosmosXobj* getNamedItem(const AtomicString&) const;
	 CosmosXobj* setNamedItem(CosmosXobj*, ExceptionState&);
	 void setNamedItem(const String& name, CosmosXobj* var, ExceptionState&);

	 void NamedPropertyEnumerator(Vector<String>& names, ExceptionState&) const;
	 bool NamedPropertyQuery(const AtomicString&, ExceptionState&) const;

	 void wait(bool bwait);
	 void sendMessage(const String& id, const String& param1, const String& param2, const String& param3, const String& param4, const String& param5);
	 void sendMessage(CosmosXobj* msg, V8ApplicationCallback* callback, bool bwait);
	 void openUrl(const String& url, long nBrowserWndOpenDisposition, V8ApplicationCallback* callback, bool bwait);
	 void Observe(const String& key, const String& strXml, V8ApplicationCallback* callback);
	 void Observe(const String& key, Element* elem, V8ApplicationCallback* callback);
	 void ObserveClient(const String& key, const String& strXml, V8ApplicationCallback* callback);
	 void ObserveClient(const String& key, Element* elem, V8ApplicationCallback* callback);

	 // DOM method
	 void defineElement(const String& tagName, const String& html);
	 void renderElement(const String& tagName, const String& html);

	 //WinForm API:
	 CosmosWinform* createWinForm(Element* elem, const long FormType, V8ApplicationCallback* callback);
	 CosmosWinform* createWinForm(const String& strFormXml, const long FormType, V8ApplicationCallback* callback);
	 CosmosWinform* newWinForm(int64_t handle, CosmosXobj* obj);
	 CosmosWinform* CreateForm(int64_t handle, CosmosXobj* obj);
	 //CLR API:
	 CosmosXobj* createObject(Element* elem, V8ApplicationCallback* callback);
	 CosmosXobj* createObject(const String& strObjXml, V8ApplicationCallback* callback);

	 //CosmosNode API
	 void MdiChildActive(CosmosXobj* xobj);
	 void MdiChildReady(CosmosXobj* xobj);
	 void AllMdiChildRemoved(CosmosXobj* xobj);
	 void ProcessMessage(CosmosXobj* xobj);
	 void OnMessage(Element* elem, const String& eventName);
	 void DispatchXobjEvent(CosmosXobj* xObj, const String& ctrlName, const String& eventName);

	 CosmosNode* createCosmosNode(CosmosXobj* xobj);
	 CosmosNode* createCosmosWinform(CosmosXobj* xobj);
	 void CosmosObjCreated(CosmosXobj* xobj);
	 CosmosNode* getXobj(Element* elem, CosmosXobj* grid);
	 CosmosNode* getXobj(const int64_t nodeHandle);
	 CosmosNode* getXobj(const String& galaxyName, const String& clusterName, const String& gridName);

	 CosmosGalaxy* getGalaxy(const String& wndName);
	 CosmosGalaxy* getGalaxy(const int64_t wndHandle);

	 // Non-js method
	 void waitMessage();
	 void releaseMessage();

	 // EventTarget overrides:
	 const AtomicString& InterfaceName() const override;
	 ExecutionContext* GetExecutionContext() const override;

	 explicit Cosmos(LocalFrame*);
	 ~Cosmos() override;
	 void Close();

	 CosmosXobj* newVar(const String& strName);
	 void invokeWinFormCreatedCallback(CosmosWinform* form);
	 bool enableConsoleInfo_ = false;
	 String url_;
	 mutable Member<Element> helperElem_;
	 mutable Member<Element> CosmosElem_;
	 mutable Member<Element> m_pVisibleContentElement;

	 HeapHashMap<String, Member<CosmosXobj>> mapCloudSession_;
	 HeapHashMap<int64_t, Member<CosmosNode>> m_mapCosmosNode;
	 HeapHashMap<int64_t, Member<CosmosGalaxy>> m_mapCosmosGalaxy;
	 HeapHashMap<int64_t, Member<CosmosWinform>> m_mapWinForm;

	 map<int64_t, CosmosNode*> m_mapCosmosNodeforEvent;
	 map<int64_t, CosmosWinform*> m_mapCosmosWinformforEvent;
	 map<int64_t, CosmosControl*> m_mapCosmosControlforEvent;
	 map<wstring, CosmosGalaxy*> m_mapCosmosGalaxy2;
   private:
	 base::RunLoop run_loop_;
	 bool is_pending_;
	 Vector<String> pending_messages_;
	 HeapHashMap<int64_t, Member<CallbackFunctionBase>> mapCallbackFunction_;
	};
}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_H_

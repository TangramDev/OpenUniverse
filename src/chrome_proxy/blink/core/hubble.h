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

#define IPC_CLR_CONTROL_CREARED							20200220
#define IPC_CLR_CONTROL_CREARED_ID						_T("Tangram_CLR_Control_Created")
#define IPC_NODE_CREARED								20200221
#define IPC_NODE_CREARED_ID								_T("Tangram_WndGrid_Created")
#define IPC_NODE_ONMOUSEACTIVATE						20200222
#define IPC_NODE_ONMOUSEACTIVATE_ID						_T("Tangram_WndGrid_OnMouseActivate")
#define IPC_MDIWINFORM_ACTIVEMDICHILD					20200224
#define IPC_MDIWINFORM_ACTIVEMDICHILD_ID				_T("MdiWinForm_ActiveMdiChild")

#define IPC_HUBBLE_MSG_BASE								20201202
#define IPC_HUBBLE_MSG_BindCLRObject					(IPC_HUBBLE_MSG_BASE + 1)
#define IPC_HUBBLE_MSG_FIRE_EVENT						(IPC_HUBBLE_MSG_BASE + 2)
#define IPC_HUBBLE_MSG_WINFORM_CREATED					(IPC_HUBBLE_MSG_BASE + 3)
#define IPC_HUBBLE_MSG_WINFORM_ONCLOSE					(IPC_HUBBLE_MSG_BASE + 4)
#define IPC_HUBBLE_MSG_Tangram_WndGrid_Created			(IPC_HUBBLE_MSG_BASE + 5)
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
	class HubbleXobj;
	class HubbleNode;
	class HubbleEvent;
	class HubbleGalaxy;
	class HubbleControl;
	class HubbleWinform;
	class ExceptionState;
	class HubbleCompositor;
	class WebLocalFrameClient;
	class SerializedScriptValue;

	//for callback:
	class CallbackFunctionBase;

	class V8HubbleCallback;
	class V8ApplicationCallback;

	class CORE_EXPORT Hubble final :
	public EventTargetWithInlineData,
		public DOMWindowClient{
	DEFINE_WRAPPERTYPEINFO();
	USING_GARBAGE_COLLECTED_MIXIN(Hubble);
		friend  class HubbleWinform;

	public:
	 static Hubble* Create(LocalFrame* frame) { return MakeGarbageCollected<Hubble>(frame); }

	 void Trace(blink::Visitor*) override;

	 // Called when an event listener has been successfully added.
	 void AddedEventListener(const AtomicString& event_type,
							 RegisteredEventListener&) override;

	 DEFINE_ATTRIBUTE_EVENT_LISTENER(MessageReceived, kHubble)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(HubbleMessageReceived, kHubblemessage)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(MdiChildActivate, kMdichildactivate)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(GridCreated, kGridcreated)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(GalaxyCreated, kHubblegalaxycreated)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(LoadWinForm, kLoadwinform)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(LoadMdiWinForm, kLoadmdiwinform)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(WinFormClosed, kWinformclosed)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(CosmosObjectCreated, kCosmosobjectcreated)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(BindCLRObject, kBindclrobject)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(CloudMessageForObject, kCloudmessageforobject)

	 HubbleXobj* xobj();
	 String url();
	 HubbleXobj* getNamedItem(const AtomicString&) const;
	 HubbleXobj* setNamedItem(HubbleXobj*, ExceptionState&);
	 void setNamedItem(const String& name, HubbleXobj* var, ExceptionState&);

	 void NamedPropertyEnumerator(Vector<String>& names, ExceptionState&) const;
	 bool NamedPropertyQuery(const AtomicString&, ExceptionState&) const;

	 void wait(bool bwait);
	 void sendMessage(const String& id, const String& param1, const String& param2, const String& param3, const String& param4, const String& param5);
	 void sendMessage(HubbleXobj* msg, V8ApplicationCallback* callback, bool bwait);
	 void openUrl(const String& url, long nBrowserWndOpenDisposition, V8ApplicationCallback* callback, bool bwait);
	 void Observe(const String& key, const String& strXml, V8ApplicationCallback* callback);
	 void addEventListener(const String& eventName, V8ApplicationCallback* callback);
	 void removeEventListener(const String& eventName);
	 void disConnect();
	 void fireEvent(const String& eventName, HubbleXobj* eventParam);

	 // DOM method
	 void defineElement(const String& tagName, const String& html);
	 void renderElement(const String& tagName, const String& html);

	 //WinForm API:
	 HubbleWinform* createWinForm(const String& strFormXml, const long FormType, V8ApplicationCallback* callback);
	 HubbleWinform* newWinForm(int64_t handle, HubbleXobj* obj);
	 HubbleWinform* CreateForm(int64_t handle, HubbleXobj* obj);

	 //HubbleNode API
	 void MdiChildActive(HubbleXobj* xobj);
	 void MdiChildReady(HubbleXobj* xobj);
	 void AllMdiChildRemoved(HubbleXobj* xobj);
	 void ProcessMessage(HubbleXobj* xobj);
	 void OnMessage(Element* elem, const String& eventName);
	 void DispatchGridEvent(HubbleXobj* xObj, const String& ctrlName, const String& eventName);

	 HubbleNode* createHubbleNode(HubbleXobj* xobj);
	 HubbleNode* createHubbleWinform(HubbleXobj* xobj);
	 void CosmosObjCreated(HubbleXobj* xobj);
	 HubbleNode* getGrid(Element* e);
	 HubbleNode* getGrid(const int64_t nodeHandle);
	 HubbleNode* getGrid(const String& galaxyName, const String& clusterName, const String& gridName);

	 HubbleGalaxy* getGalaxy(const String& wndName);
	 HubbleGalaxy* getGalaxy(const int64_t wndHandle);

	 String getStr(const String& strKey);
	 void setStr(const String& strKey, const String& value);
	 long getLong(const String& strKey);
	 void setLong(const String& strKey, long value);
	 int64_t getInt64(const String& strKey);
	 void setInt64(const String& strKey, int64_t value);
	 float getFloat(const String& strKey);
	 void setFloat(const String& strKey, float value);

	 // Non-js method
	 void waitMessage();
	 void releaseMessage();

	 // EventTarget overrides:
	 const AtomicString& InterfaceName() const override;
	 ExecutionContext* GetExecutionContext() const override;

	 explicit Hubble(LocalFrame*);
	 ~Hubble() override;
	 void Close();

	 HubbleXobj* newVar(const String& strName);
	 void invokeWinFormCreatedCallback(HubbleWinform* form);

	 WebLocalFrameClient* m_pRenderframeImpl;
	 String url_;
	 mutable Member<Element> helperElem_;
	 mutable Member<HubbleXobj> innerXobj_;
	 mutable Member<Element> ObjectInfoElem_;
	 mutable Member<Element> m_pVisibleContentElement;

	 HeapHashMap<String, Member<HubbleXobj>> mapCloudSession_;
	 HeapHashMap<int64_t, Member<HubbleNode>> m_mapHubbleNode;
	 HeapHashMap<int64_t, Member<HubbleGalaxy>> m_mapHubbleGalaxy;
	 HeapHashMap<int64_t, Member<HubbleWinform>> m_mapWinForm;
	 HeapHashMap<int64_t, Member<HubbleCompositor>> m_mapHubbleCompositor;

	 map<int64_t, HubbleNode*> m_mapHubbleNodeforEvent;
	 map<int64_t, HubbleWinform*> m_mapHubbleWinformforEvent;
	 map<int64_t, HubbleControl*> m_mapHubbleControlforEvent;
	 map<wstring, HubbleGalaxy*> m_mapHubbleGalaxy2;
   private:
	 base::RunLoop run_loop_;
	 bool is_pending_;
	 Vector<String> pending_messages_;
	 HeapHashMap<String, Member<V8HubbleCallback>> mapHubbleCallback_;
	 HeapHashMap<int64_t, Member<CallbackFunctionBase>> mapCallbackFunction_;
	};
}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_H_

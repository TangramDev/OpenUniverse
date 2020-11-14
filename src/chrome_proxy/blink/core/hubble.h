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

namespace blink {
	using namespace std;

	class Element;
	class LocalFrame;
	class Document;
	class ScriptState;
	class HubbleXobj;
	class HubbleNode;
	class HubbleWindow;
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
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(HubbleWindowCreated, kHubblewindowcreated)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(LoadWinForm, kLoadwinform)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(LoadMdiWinForm, kLoadmdiwinform)
	 //DEFINE_ATTRIBUTE_EVENT_LISTENER(LoadMdiChildWinForm, kLoadmdichildwinform)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(WinFormClosed, kWinformclosed)
	 DEFINE_ATTRIBUTE_EVENT_LISTENER(BindCLRObject, kBindclrobject)

	 HubbleXobj* xobj();
	 HubbleNode* topGrid();
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
	 HubbleNode* createHubbleNode(HubbleXobj* xobj);
	 HubbleNode* getGrid(const int64_t nodeHandle);

	 HubbleWindow* getWindow(const String& wndName);
	 HubbleWindow* getWindow(const int64_t wndHandle);

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
	 mutable Member<HubbleXobj> innerXobj_;
	 mutable Member<HubbleNode> topGrid_;
	 HeapHashMap<String, Member<HubbleXobj>> mapCloudSession_;
	 HeapHashMap<int64_t, Member<HubbleNode>> m_mapHubbleNode;
	 HeapHashMap<int64_t, Member<HubbleWindow>> m_mapHubbleWindow;
	 HeapHashMap<int64_t, Member<HubbleWinform>> m_mapWinForm;
	 HeapHashMap<int64_t, Member<HubbleCompositor>> m_mapHubbleCompositor;

	 map<int64_t, HubbleNode*> m_mapHubbleNodeforEvent;
	 map<int64_t, HubbleWinform*> m_mapHubbleWinformforEvent;
	 map<int64_t, HubbleControl*> m_mapHubbleControlforEvent;
	 map<wstring, HubbleWindow*> m_mapHubbleWindow2;
   private:
	 base::RunLoop run_loop_;
	 bool is_pending_;
	 Vector<String> pending_messages_;
	 HeapHashMap<String, Member<V8HubbleCallback>> mapHubbleCallback_;
	 HeapHashMap<int64_t, Member<CallbackFunctionBase>> mapCallbackFunction_;
	};
}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_H_

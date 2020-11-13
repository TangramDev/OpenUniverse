// begin Add by HubbleTeam
#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_WINDOW_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_WINDOW_H_

#include <map>

#include "third_party/blink/renderer/bindings/core/v8/script_value.h"
#include "third_party/blink/renderer/bindings/core/v8/serialization/transferables.h"
#include "third_party/blink/renderer/core/execution_context/context_lifecycle_observer.h"
#include "third_party/blink/renderer/core/dom/events/event_target.h"
#include "third_party/blink/renderer/core/event_type_names.h"

#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/assertions.h"
#include "third_party/blink/renderer/platform/wtf/forward.h"

namespace blink {

using namespace std;

class Hubble;
class Document;
class ScriptState;
class HubbleXobj;
class HubbleNode;
class ExceptionState;
class WebLocalFrameClient;
class SerializedScriptValue;
class V8ApplicationCallback;

class CORE_EXPORT HubbleWindow final : public EventTargetWithInlineData,
									  public DOMWindowClient {
  DEFINE_WRAPPERTYPEINFO();
  USING_GARBAGE_COLLECTED_MIXIN(HubbleWindow);

 public:
  static HubbleWindow* Create(LocalFrame* frame) { return MakeGarbageCollected<HubbleWindow>(frame); }
  static HubbleWindow* Create(LocalFrame* frame, const String& strName);

  void Trace(blink::Visitor*) override;

  // Called when an event listener has been successfully added.
  void AddedEventListener(const AtomicString& event_type,
                          RegisteredEventListener&) override;

  int64_t handle();
  String name();
  String getid();
  HubbleXobj* xobj();

  // Message method
  void sendMessage(HubbleXobj* msg, V8ApplicationCallback* callback);

  void addEventListener(const String& eventName, V8ApplicationCallback* callback);
  void removeEventListener(const String& eventName);
  void disConnect();
  void fireEvent(const String& eventName, HubbleXobj* eventParam);
  void invokeCallback(wstring callbackid, HubbleXobj* callbackParam);

  HubbleNode* getGrid(const String& nodeName);
  HubbleNode* getGrid(const long nodeHandle);

  DEFINE_ATTRIBUTE_EVENT_LISTENER(MessageReceived, kHubblewindow)
      DEFINE_ATTRIBUTE_EVENT_LISTENER(MdiChildActivate, kMdichildactivate)
      DEFINE_ATTRIBUTE_EVENT_LISTENER(GridCreated, kGridcreated)

  // EventTarget overrides:
  const AtomicString& InterfaceName() const override;
  ExecutionContext* GetExecutionContext() const override;

  HubbleWindow(LocalFrame*);
  HubbleWindow(LocalFrame*, const String& strWindowName);

  ~HubbleWindow() override;

  int64_t handle_ = 0;
  mutable Member<HubbleNode> m_pHostNode;

  mutable Member<Hubble> hubble_;
  mutable Member<HubbleXobj> innerXobj_;

  WebLocalFrameClient* m_pRenderframeImpl;
  
  map<int64_t, HubbleNode*> m_mapHubbleNode;
  map<wstring, HubbleNode*> m_mapHubbleNode2;
private:
  String id_;
  String name_;
  String m_strWindowXml;
  HeapHashMap<String, Member<V8ApplicationCallback>> mapHubbleEventCallback_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_WINDOW_H_
// end Add by HubbleTeam

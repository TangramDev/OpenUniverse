// begin Add by HubbleTeam
#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_CONTROL_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_CONTROL_H_

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
class HubbleXobj;
class HubbleNode;
class HubbleWindow;
class HubbleWinform;
class HubbleControl;

class Document;
class ScriptState;
class ExceptionState;
class V8HubbleCallback;
class V8ApplicationCallback;

class WebLocalFrameClient;
class SerializedScriptValue;

class CORE_EXPORT HubbleControl final : public EventTargetWithInlineData,
									  public DOMWindowClient {
  DEFINE_WRAPPERTYPEINFO();
  USING_GARBAGE_COLLECTED_MIXIN(HubbleControl);

 public:
  static HubbleControl* Create(LocalFrame* frame) { return MakeGarbageCollected<HubbleControl>(frame); }
  static HubbleControl* Create(LocalFrame* frame, const String& strHandle);

  void Trace(blink::Visitor*) override;

  // Called when an event listener has been successfully added.
  void AddedEventListener(const AtomicString& event_type,
                          RegisteredEventListener&) override;

  int64_t handle();
  String name();
  String type();
  String getid();
  HubbleXobj* xobj();
  HubbleNode* parentGrid();
  HubbleWinform* parentForm();
  HubbleControl* parentControl();

  // Message method
  void sendMessage(HubbleXobj* msg, V8ApplicationCallback* callback);
  void invokeCallback(wstring callbackid, HubbleXobj* callbackParam);

  DEFINE_ATTRIBUTE_EVENT_LISTENER(MessageReceived, kHubblecontrol)

  // EventTarget overrides:
  const AtomicString& InterfaceName() const override;
  ExecutionContext* GetExecutionContext() const override;

  HubbleControl(LocalFrame*);
  HubbleControl(LocalFrame*, const String& strNodeXml);

  ~HubbleControl() override;

  // Binding Event:
  void BindCtrlEvents(const String& strEvents); 
  void addEventListener(const String& eventName, V8ApplicationCallback* callback);
  void fireEvent(const String& eventName, HubbleXobj* eventParam);
  void removeEventListener(const String& eventName);

  int64_t handle_ = 0;
  String name_;
  String type_;
  String events_;
  String webpageid_;

  mutable Member<Hubble> hubble_;
  mutable Member<HubbleXobj> innerXobj_;
  mutable Member<HubbleNode> m_pParentNode;
  mutable Member<HubbleWinform> m_pParentForm;
  mutable Member<HubbleControl> m_pParentControl;
  WebLocalFrameClient* m_pRenderframeImpl;
  map<std::wstring, int> m_mapEvent;
private:
  String id_;
  HeapHashMap<String, Member<V8ApplicationCallback>> mapHubbleEventCallback_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_CONTROL_H_
// end Add by HubbleTeam

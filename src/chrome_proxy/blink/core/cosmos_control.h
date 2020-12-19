// begin Add by CosmosTeam
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

class Cosmos;
class CosmosXobj;
class CosmosNode;
class CosmosGalaxy;
class CosmosWinform;
class CosmosControl;

class Document;
class ScriptState;
class ExceptionState;
class V8CosmosCallback;
class V8ApplicationCallback;

class WebLocalFrameClient;
class SerializedScriptValue;

class CORE_EXPORT CosmosControl final : public EventTargetWithInlineData,
									  public DOMWindowClient {
  DEFINE_WRAPPERTYPEINFO();
  USING_GARBAGE_COLLECTED_MIXIN(CosmosControl);

 public:
  static CosmosControl* Create(LocalFrame* frame) { return MakeGarbageCollected<CosmosControl>(frame); }
  static CosmosControl* Create(LocalFrame* frame, const String& strHandle);

  void Trace(blink::Visitor*) override;

  // Called when an event listener has been successfully added.
  void AddedEventListener(const AtomicString& event_type,
                          RegisteredEventListener&) override;

  int64_t handle();
  String name();
  String type();
  String getid();
  CosmosXobj* xobj();
  CosmosNode* parentGrid();
  CosmosWinform* parentForm();
  CosmosControl* parentControl();

  // Message method
  void sendMessage(CosmosXobj* msg, V8ApplicationCallback* callback);
  void invokeCallback(wstring callbackid, CosmosXobj* callbackParam);

  DEFINE_ATTRIBUTE_EVENT_LISTENER(MessageReceived, kCosmoscontrol)

  // EventTarget overrides:
  const AtomicString& InterfaceName() const override;
  ExecutionContext* GetExecutionContext() const override;

  CosmosControl(LocalFrame*);
  CosmosControl(LocalFrame*, const String& strNodeXml);

  ~CosmosControl() override;

  // Binding Event:
  void BindCtrlEvents(const String& strEvents); 
  void addEventListener(const String& eventName, V8ApplicationCallback* callback);
  void fireEvent(const String& eventName, CosmosXobj* eventParam);
  void removeEventListener(const String& eventName);

  int64_t handle_ = 0;
  String name_;
  String type_;
  String events_;
  String webpageid_;

  mutable Member<Cosmos> cosmos_;
  mutable Member<CosmosXobj> innerXobj_;
  mutable Member<CosmosNode> m_pParentNode;
  mutable Member<CosmosWinform> m_pParentForm;
  mutable Member<CosmosControl> m_pParentControl;
  WebLocalFrameClient* m_pRenderframeImpl;
  map<std::wstring, int> m_mapEvent;
private:
  String id_;
  HeapHashMap<String, Member<V8ApplicationCallback>> mapCosmosEventCallback_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_CONTROL_H_
// end Add by CosmosTeam

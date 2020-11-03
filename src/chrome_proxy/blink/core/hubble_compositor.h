// begin Add by HubbleTeam
#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_COMPOSITOR_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_COMPOSITOR_H_

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

class Document;
class HubbleXobj;
class ScriptState;
class ExceptionState;
class V8HubbleCallback;
class WebLocalFrameClient;
class SerializedScriptValue;

class CORE_EXPORT HubbleCompositor final : public EventTargetWithInlineData,
									  public DOMWindowClient {
  DEFINE_WRAPPERTYPEINFO();
  USING_GARBAGE_COLLECTED_MIXIN(HubbleCompositor);

 public:
  static HubbleCompositor* Create(LocalFrame* frame) { return MakeGarbageCollected<HubbleCompositor>(frame); }
  static HubbleCompositor* Create(LocalFrame* frame, const String& strHandle);

  void Trace(blink::Visitor*) override;

  // Called when an event listener has been successfully added.
  void AddedEventListener(const AtomicString& event_type,
                          RegisteredEventListener&) override;

  String name();
  String getid();

  // Message method
  void sendMessage(const String& id, const String& param1, const String& param2, const String& param3, const String& param4, const String& param5);
  

  DEFINE_ATTRIBUTE_EVENT_LISTENER(MessageReceived, kHubblecompositor)
  DEFINE_ATTRIBUTE_EVENT_LISTENER(GridCreated, kGridcreated)

  // EventTarget overrides:
  const AtomicString& InterfaceName() const override;
  ExecutionContext* GetExecutionContext() const override;

  HubbleCompositor(LocalFrame*);
  HubbleCompositor(LocalFrame*, const String& strNodeXml);

  ~HubbleCompositor() override;

  WebLocalFrameClient* m_pRenderframeImpl;
  mutable Member<HubbleXobj> innerXobj_;
private:
  String id_;
  String name_;
  String m_strGridXml;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_COMPOSITOR_H_
// end Add by HubbleTeam

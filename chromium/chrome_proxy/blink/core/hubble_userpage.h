// begin Add by HubbleTeam
#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_USERPAGE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_USERPAGE_H_

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

class HubbleXobj;
class HubbleNode;
class HubbleWindow;
class HubbleControl;
class HubbleWinform;
class ExceptionState;
class HubbleCompositor;

//for callback:
class CallbackFunctionBase;

class V8HubbleCallback;
class V8ApplicationCallback;

class Document;
class ScriptState;
class ExceptionState;
class V8HubbleCallback;
class WebLocalFrameClient;
class SerializedScriptValue;

class CORE_EXPORT HubbleUserpage final : public EventTargetWithInlineData,
									  public DOMWindowClient {
  DEFINE_WRAPPERTYPEINFO();
  USING_GARBAGE_COLLECTED_MIXIN(HubbleUserpage);

 public:
  static HubbleUserpage* Create(LocalFrame* frame) { return MakeGarbageCollected<HubbleUserpage>(frame); }
  static HubbleUserpage* Create(LocalFrame* frame, const String& strHandle);

  void Trace(blink::Visitor*) override;

  // Called when an event listener has been successfully added.
  void AddedEventListener(const AtomicString& event_type,
                          RegisteredEventListener&) override;

  String name();

  // Message method
  void sendMessage(const String& id, const String& param1, const String& param2, const String& param3, const String& param4, const String& param5);
  

  DEFINE_ATTRIBUTE_EVENT_LISTENER(UserPageMessageReceived, kHubbleuserpage)

  // EventTarget overrides:
  const AtomicString& InterfaceName() const override;
  ExecutionContext* GetExecutionContext() const override;

  explicit HubbleUserpage(LocalFrame*);
  HubbleUserpage(LocalFrame*, const String& strNodeXml);

  ~HubbleUserpage() override;

  mutable Member<HubbleXobj> innerXobj_;
  WebLocalFrameClient* web_local_frame_client;
private:
  String name_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_USERPAGE_H_
// end Add by HubbleTeam

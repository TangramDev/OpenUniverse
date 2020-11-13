// begin Add by HubbleTeam
#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_WINFORM_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_WINFORM_H_

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

//for callback:
class CallbackFunctionBase;
class V8ApplicationCallback;

class Document;
class ScriptState;
class ExceptionState;
class WebLocalFrameClient;
class SerializedScriptValue;
class V8ApplicationCallback;

class CORE_EXPORT HubbleWinform final : public EventTargetWithInlineData,
									  public DOMWindowClient {
  DEFINE_WRAPPERTYPEINFO();
  USING_GARBAGE_COLLECTED_MIXIN(HubbleWinform);

 public:
  static HubbleWinform* Create(LocalFrame* frame) { return MakeGarbageCollected<HubbleWinform>(frame); }
  static HubbleWinform* Create(LocalFrame* frame, const String& strHandle);

  void Trace(blink::Visitor*) override;

  // Called when an event listener has been successfully added.
  void AddedEventListener(const AtomicString& event_type,
                          RegisteredEventListener&) override;

  int64_t handle();
  String name();
  String getid();
  HubbleXobj* xobj();
  HubbleNode* mdibindgrid();
  HubbleWinform* mdiParent();

  // Message method
  void addEventListener(const String& eventName, V8ApplicationCallback* callback);
  void addEventListener(const String& subObjName, const String& eventName, V8ApplicationCallback* callback);
  void removeEventListener(const String& eventName);
  void disConnect();
  void fireEvent(const String& eventName, HubbleXobj* eventParam);
  void sendMessage(HubbleXobj* msg, V8ApplicationCallback* callback);
  void invokeCallback(wstring callbackid, HubbleXobj* callbackParam);
  void SyncCtrlTextChange(const String& strcontrols, V8ApplicationCallback* callback);

  DEFINE_ATTRIBUTE_EVENT_LISTENER(MessageReceived, kHubblewinform)
      DEFINE_ATTRIBUTE_EVENT_LISTENER(MdiChildActivate, kMdichildactivate)
      DEFINE_ATTRIBUTE_EVENT_LISTENER(GridCreated, kGridcreated)
      DEFINE_ATTRIBUTE_EVENT_LISTENER(LoadMdiChildWinForm, kLoadmdichildwinform)
      DEFINE_ATTRIBUTE_EVENT_LISTENER(LoadWinForm, kLoadwinform)
      DEFINE_ATTRIBUTE_EVENT_LISTENER(WinFormClosed, kWinformclosed)

  // EventTarget overrides:
  const AtomicString& InterfaceName() const override;
  ExecutionContext* GetExecutionContext() const override;

  HubbleWinform(LocalFrame*);
  HubbleWinform(LocalFrame*, const String& strNodeXml);

  ~HubbleWinform() override;

  int64_t handle_ = 0;
  mutable Member<Hubble> hubble_;
  mutable Member<HubbleXobj> innerXobj_;
  mutable Member<HubbleNode> m_pBindMdiNode;
  WebLocalFrameClient* m_pRenderframeImpl;
  map<int64_t, Member<HubbleControl>> m_mapChildControl;
  map<std::wstring, Member<HubbleControl>> m_mapChildControl2;
private:
  String id_;
  String name_;
  String m_strGridXml;
  HeapHashMap<String, Member<V8ApplicationCallback>> mapHubbleEventCallback_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_WINFORM_H_
// end Add by HubbleTeam

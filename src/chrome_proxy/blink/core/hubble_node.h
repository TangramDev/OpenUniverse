// begin Add by HubbleTeam
#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_NODE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_NODE_H_

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
class Element;
class HubbleXobj;
class HubbleGalaxy;
class HubbleWinform;
class HubbleControl;

class Document;
class ScriptState;
class ExceptionState;
class V8ApplicationCallback;
class WebLocalFrameClient;
class SerializedScriptValue;

class CORE_EXPORT HubbleNode final : public EventTargetWithInlineData,
									  public DOMWindowClient {
  DEFINE_WRAPPERTYPEINFO();
  USING_GARBAGE_COLLECTED_MIXIN(HubbleNode);

 public:
  static HubbleNode* Create(LocalFrame* frame) { return MakeGarbageCollected<HubbleNode>(frame); }
  static HubbleNode* Create(LocalFrame* frame, const String& strHandle);

  void Trace(blink::Visitor*) override;

  // Called when an event listener has been successfully added.
  void AddedEventListener(const AtomicString& event_type,
                          RegisteredEventListener&) override;

  long row();
  long col();
  long rows();
  long cols();
  int64_t handle();
  HubbleXobj* xobj();

  HubbleNode* root();
  HubbleNode* parentGrid();
  HubbleGalaxy* parentGalaxy();
  HubbleWinform* parentForm();

  int64_t hwnd();
  String name();
  String objtype();
  String getid();

  // Message method

  void sendMessage(const String& id, const String& param1, const String& param2, const String& param3, const String& param4, const String& param5);
  void sendMessageToGrid(HubbleXobj* msg);
  void sendMessageToGrid(HubbleNode* node);
  void addEventListener(const String& eventName, V8ApplicationCallback* callback);
  void addEventListener(const String& subObjName, const String& eventName, V8ApplicationCallback* callback);
  void removeEventListener(const String& eventName);
  void disConnect();
  void fireEvent(const String& eventName, HubbleXobj* eventParam);
  void sendMessage(HubbleXobj* msg, V8ApplicationCallback* callback);
  void invokeCallback(wstring callbackid, HubbleXobj* callbackParam);
  void SyncCtrlTextChange(const String& strcontrols, V8ApplicationCallback* callback);
  void ShowWebContent(const String& strParentDivName, const String& strDivName);

  //Control Bind API:
  void setControlVal(const String& CtrlID, int64_t CtrlHandle, const String& strVal);
  void GetControlsNames(V8ApplicationCallback* callback);

  // Node API:
  HubbleNode* getChild(long nIndex);
  HubbleNode* getChild(long row, long col);
  HubbleNode* getChild(const String& strName);
  HubbleNode* getGrid(const String& strName);
  void Observe(const String& strKey, const String& xml, V8ApplicationCallback* callback);
  void ObserveEx(const String& strKey, const String& xml, long row, long col, V8ApplicationCallback* callback);
  void ObserveCtrl(const String& strCtrlName, const String& strKey, const String& xml, V8ApplicationCallback* callback);

  DEFINE_ATTRIBUTE_EVENT_LISTENER(MessageReceived, kHubble)
  DEFINE_ATTRIBUTE_EVENT_LISTENER(MdiChildActivate, kMdichildactivate)
  DEFINE_ATTRIBUTE_EVENT_LISTENER(GridCreated, kGridcreated)
  DEFINE_ATTRIBUTE_EVENT_LISTENER(HubbleControlCreated, kHubblecontrolcreated)
  DEFINE_ATTRIBUTE_EVENT_LISTENER(LoadWinForm, kLoadwinform)
  DEFINE_ATTRIBUTE_EVENT_LISTENER(CloudMessageForGrid, kCloudmessageforgrid)
  DEFINE_ATTRIBUTE_EVENT_LISTENER(WinFormClosed, kWinformclosed)

  // EventTarget overrides:
  const AtomicString& InterfaceName() const override;
  ExecutionContext* GetExecutionContext() const override;

  HubbleNode(LocalFrame*);
  HubbleNode(LocalFrame*, const String& strNodeXml);
  HubbleNode* AddChild(int64_t nHandle, const String& strNodeName, blink::Hubble*);
  //HubbleControl* AddCtrl(int64_t nCtrlHandle, const String& strCtrlName, const String& strCtrlType, const String& strCtrlData, const String& strWebPageID, blink::Hubble*);

  ~HubbleNode() override;

  int64_t handle_ = 0;

  String name_;

  mutable Member<Hubble> hubble_;
  mutable Member<HubbleXobj> innerXobj_;
  mutable Member<HubbleWinform> m_pParentForm;
  mutable Member<Element> m_pVisibleContentElement;

  WebLocalFrameClient* m_pRenderframeImpl;
  map<int, HubbleNode*> m_mapChildNode;
  map<wstring, HubbleNode*> m_mapGrid;
  map<wstring, HubbleNode*> m_mapChildNode2;

private:
  String id_;
  HeapHashMap<String, Member<V8ApplicationCallback>> mapHubbleEventCallback_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_NODE_H_
// end Add by HubbleTeam

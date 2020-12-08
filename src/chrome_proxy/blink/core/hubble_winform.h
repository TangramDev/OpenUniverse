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
#include "third_party/blink/renderer/core/dom/element.h"

namespace blink {

using namespace std;

class Hubble;
class DOMParser;
class HubbleXobj;
class HubbleNode;
class HubbleGalaxy;
class HubbleWinform;
class HubbleControl;

//for callback:
class CallbackFunctionBase;
class V8ApplicationCallback;

class Document;
class ScriptState;
class ExceptionState;
class DocumentFragment;
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
  bool isReady();
  long formType();

  Element* uiElement();
  Element* formElement();
  Element* eventElement();
  Element* messageElement();
  Element* propertyElement();

  String msgID();
  void setMsgID(const String& value);

  HubbleXobj* xobj();
  HubbleNode* mdibindgrid();
  HubbleWinform* mdiParent();
  HubbleWinform* activeMDIChild();
  HubbleNode* mdiwebbindgrid();
  DocumentFragment* docFragment();
  HubbleNode* getGrid(const String& galaxyName, const String& clusterName, const String& gridName);
  HubbleGalaxy* getGalaxy(const String& galaxyName);
  String getStr(const String& strKey);
  void setStr(const String& strKey, const String& value);
  long getLong(const String& strKey);
  void setLong(const String& strKey, long value);
  int64_t getInt64(const String& strKey);
  void setInt64(const String& strKey, int64_t value);
  float getFloat(const String& strKey);
  void setFloat(const String& strKey, float value);
  void DispatchGridEvent(Element* elem, const String& eventName);
  void ProcessFormMessage(const String& msgID);

  void InitWinForm();
  //void DispatchGridEvent(HubbleXobj* xObj, const String& ctrlName, const String& eventName);

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
      DEFINE_ATTRIBUTE_EVENT_LISTENER(GalaxyCreated, kHubblegalaxycreated)
      DEFINE_ATTRIBUTE_EVENT_LISTENER(AllMdiChildRemoved, kAllmdichildremoved)
      DEFINE_ATTRIBUTE_EVENT_LISTENER(LoadMdiChildWinForm, kLoadmdichildwinform)
      DEFINE_ATTRIBUTE_EVENT_LISTENER(CloudMessageForWinForm, kCloudmessageforwinform)
      DEFINE_ATTRIBUTE_EVENT_LISTENER(MdiChildWinFormReady, kMdichildwinformready)
      DEFINE_ATTRIBUTE_EVENT_LISTENER(MdiWinFormReady, kMdiwinformready)
      DEFINE_ATTRIBUTE_EVENT_LISTENER(WinFormReady, kWinformready)
      DEFINE_ATTRIBUTE_EVENT_LISTENER(LoadWinForm, kLoadwinform)
      DEFINE_ATTRIBUTE_EVENT_LISTENER(WinFormClosed, kWinformclosed)

  // EventTarget overrides:
  const AtomicString& InterfaceName() const override;
  ExecutionContext* GetExecutionContext() const override;

  HubbleWinform(LocalFrame*);
  HubbleWinform(LocalFrame*, const String& strNodeXml);

  ~HubbleWinform() override;

  bool isReady_ = false;
  int64_t handle_ = 0;
  int64_t m_nMdiwebbindgridhandle = 0;
  mutable Member<Hubble> hubble_;
  mutable Member<HubbleXobj> innerXobj_;
  mutable Member<Element> m_pContentElement;
  mutable Member<HubbleNode> m_pBindMdiNode;
  mutable Member<HubbleNode> m_pWebBindMdiNode;
  mutable Member<HubbleWinform> m_pMDIParent;
  mutable Member<HubbleWinform> m_pActiveMDIChild;
  mutable Member <DocumentFragment> DocumentFragment_;
  mutable Member<Element> uiElem_;
  mutable Member<Element> formElem_;
  mutable Member<Element> eventElem_;
  mutable Member<Element> messageElem_;
  mutable Member<Element> propertyElem_;

  map<wstring, HubbleGalaxy*> m_mapHubbleGalaxy;

  WebLocalFrameClient* m_pRenderframeImpl;
  map < wstring, Element* > m_mapEventInfo;
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

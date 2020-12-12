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
#include "hubble_xobj.h"

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

class CORE_EXPORT HubbleWinform final : 
    public HubbleXobj{
  DEFINE_WRAPPERTYPEINFO();

 public:
  static HubbleWinform* Create() { return MakeGarbageCollected<HubbleWinform>(); }
  static HubbleWinform* Create(const String& strHandle);

  void Trace(blink::Visitor*) override;

  int64_t handle();
  String name();
  String getid();
  bool isReady();
  long formType();

  HubbleNode* mdibindgrid();
  HubbleWinform* mdiParent();
  HubbleWinform* activeMDIChild();
  HubbleNode* mdiwebbindgrid();
  HubbleNode* getGrid(Element* elem);
  HubbleNode* getGrid(const String& galaxyName, const String& clusterName, const String& gridName);
  HubbleGalaxy* getGalaxy(const String& galaxyName);
  void DispatchGridEvent(Element* elem, const String& eventName);
  void ProcessFormMessage(const String& msgID);

  void InitWinForm();
  //void DispatchGridEvent(HubbleXobj* xObj, const String& ctrlName, const String& eventName);


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

  HubbleWinform();
  HubbleWinform(const String& strNodeXml);

  ~HubbleWinform() override;

  bool isReady_ = false;
  int64_t handle_ = 0;
  int64_t m_nMdiwebbindgridhandle = 0;
  mutable Member<Element> m_pContentElement;
  mutable Member<HubbleNode> m_pBindMdiNode;
  mutable Member<HubbleNode> m_pWebBindMdiNode;
  mutable Member<HubbleWinform> m_pMDIParent;
  mutable Member<HubbleWinform> m_pActiveMDIChild;

  map<wstring, HubbleGalaxy*> m_mapHubbleGalaxy;

  map<int64_t, Member<HubbleControl>> m_mapChildControl;
  map<std::wstring, Member<HubbleControl>> m_mapChildControl2;
                                      
private:
  String name_;
  String m_strGridXml;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_WINFORM_H_
// end Add by HubbleTeam

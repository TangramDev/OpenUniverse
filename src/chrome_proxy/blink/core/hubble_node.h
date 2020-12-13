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
#include "hubble_xobj.h"

namespace blink {

using namespace std;

class Hubble;
class Element;
class DOMParser;
class HubbleXobj;
class HubbleGalaxy;
class HubbleWinform;
class HubbleControl;

class Document;
class ScriptState;
class ExceptionState;
class DocumentFragment;
class V8ApplicationCallback;
class WebLocalFrameClient;
class SerializedScriptValue;

class CORE_EXPORT HubbleNode final : 
	public HubbleXobj{
  DEFINE_WRAPPERTYPEINFO();

 public:
  static HubbleNode* Create() { return MakeGarbageCollected<HubbleNode>(); }
  static HubbleNode* Create(const String& strHandle);

  void Trace(blink::Visitor*) override;

  long row();
  long col();
  long rows();
  long cols();
  int64_t handle();

  HubbleNode* root();
  HubbleNode* parentGrid();
  HubbleGalaxy* parentGalaxy();
  HubbleWinform* parentForm();

  int64_t hwnd();
  String objtype();
  String getid();

  // Message method
  void sendMessage(const String& id, const String& param1, const String& param2, const String& param3, const String& param4, const String& param5);
  void sendMessageToGrid(HubbleXobj* msg);
  void sendMessageToGrid(HubbleNode* node);
  void ShowWebContent(const String& strParentDivName, const String& strDivName);

  //Control Bind API:
  void setControlVal(const String& CtrlID, int64_t CtrlHandle, const String& strVal);
  void GetControlsNames(V8ApplicationCallback* callback);

  HubbleNode* getChild(long nIndex);
  HubbleNode* getChild(long row, long col);
  HubbleNode* getChild(const String& strName);
  HubbleNode* getGrid(const String& strName);
  void Observe(const String& strKey, const String& xml, V8ApplicationCallback* callback);
  void ObserveEx(const String& strKey, const String& xml, long row, long col, V8ApplicationCallback* callback);
  void ObserveCtrl(const String& strCtrlName, const String& strKey, const String& xml, V8ApplicationCallback* callback);

  DEFINE_ATTRIBUTE_EVENT_LISTENER(MdiChildActivate, kMdichildactivate)
  DEFINE_ATTRIBUTE_EVENT_LISTENER(HubbleControlCreated, kHubblecontrolcreated)
  DEFINE_ATTRIBUTE_EVENT_LISTENER(LoadWinForm, kLoadwinform)
  DEFINE_ATTRIBUTE_EVENT_LISTENER(CloudMessageForGrid, kCloudmessageforgrid)
  DEFINE_ATTRIBUTE_EVENT_LISTENER(WinFormClosed, kWinformclosed)

  HubbleNode();
  HubbleNode(const String& strNodeXml);
  HubbleNode* AddChild(int64_t nHandle, const String& strNodeName, blink::Hubble*);

  ~HubbleNode() override;

  int64_t handle_ = 0;

  String name_;

  mutable Member<HubbleNode> rootNode_ ;
  mutable Member<HubbleWinform> m_pParentForm;

  map<int, HubbleNode*> m_mapChildNode;
  map<wstring, HubbleNode*> m_mapGrid;
  map<wstring, HubbleNode*> m_mapChildNode2;

private:
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_NODE_H_
// end Add by HubbleTeam

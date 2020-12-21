// begin Add by CosmosTeam
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
#include "cosmos_xobj.h"

namespace blink {

using namespace std;

class Cosmos;
class Element;
class DOMParser;
class CosmosXobj;
class CosmosGalaxy;
class CosmosWinform;
class CosmosControl;

class Document;
class ScriptState;
class ExceptionState;
class DocumentFragment;
class V8ApplicationCallback;
class WebLocalFrameClient;
class SerializedScriptValue;

class CORE_EXPORT CosmosNode final : 
	public CosmosXobj{
  DEFINE_WRAPPERTYPEINFO();

 public:
  static CosmosNode* Create() { return MakeGarbageCollected<CosmosNode>(); }
  static CosmosNode* Create(const String& strHandle);

  void Trace(blink::Visitor*) override;

  long row();
  long col();
  long rows();
  long cols();
  int64_t handle();

  CosmosNode* root();
  CosmosNode* parentGrid();
  CosmosGalaxy* parentGalaxy();
  CosmosWinform* parentForm();

  int64_t hwnd();
  String objtype();
  String getid();

  // Message method
  void sendMessage(const String& id, const String& param1, const String& param2, const String& param3, const String& param4, const String& param5);
  void sendMessageToGrid(CosmosXobj* msg);
  void sendMessageToGrid(CosmosNode* node);
  void ShowWebContent(const String& strParentDivName, const String& strDivName);

  //Control Bind API:
  void setControlVal(const String& CtrlID, int64_t CtrlHandle, const String& strVal);
  void GetControlsNames(V8ApplicationCallback* callback);

  CosmosNode* getChild(long nIndex);
  CosmosNode* getChild(long row, long col);
  CosmosNode* getChild(const String& strName);
  CosmosNode* getXobj(const String& strName);
  void Observe(const String& strKey, const String& xml, V8ApplicationCallback* callback);
  void ObserveEx(const String& strKey, const String& xml, long row, long col, V8ApplicationCallback* callback);
  void ObserveCtrl(const String& strCtrlName, const String& strKey, const String& xml, V8ApplicationCallback* callback);

  DEFINE_ATTRIBUTE_EVENT_LISTENER(MdiChildActivate, kMdichildactivate)
  DEFINE_ATTRIBUTE_EVENT_LISTENER(CosmosControlCreated, kCosmoscontrolcreated)
  DEFINE_ATTRIBUTE_EVENT_LISTENER(LoadWinForm, kLoadwinform)
  DEFINE_ATTRIBUTE_EVENT_LISTENER(CloudMessageForXobj, kCloudmessageforxobj)
  DEFINE_ATTRIBUTE_EVENT_LISTENER(WinFormClosed, kWinformclosed)

  CosmosNode();
  CosmosNode(const String& strNodeXml);
  CosmosNode* AddChild(int64_t nHandle, const String& strNodeName, blink::Cosmos*);

  ~CosmosNode() override;

  int64_t handle_ = 0;

  String name_;

  mutable Member<CosmosNode> rootNode_ ;
  mutable Member<CosmosWinform> m_pParentForm;

  map<int, CosmosNode*> m_mapChildNode;
  map<wstring, CosmosNode*> m_mapGrid;
  map<wstring, CosmosNode*> m_mapChildNode2;

private:
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_NODE_H_
// end Add by CosmosTeam

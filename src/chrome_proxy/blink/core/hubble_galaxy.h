// begin Add by HubbleTeam
#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_WINDOW_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_WINDOW_H_

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
class Document;
class ScriptState;
class HubbleXobj;
class HubbleNode;
class ExceptionState;
class WebLocalFrameClient;
class SerializedScriptValue;
class V8ApplicationCallback;

class CORE_EXPORT HubbleGalaxy final : public HubbleXobj{
  DEFINE_WRAPPERTYPEINFO();

 public:
  static HubbleGalaxy* Create() { return MakeGarbageCollected<HubbleGalaxy>(); }
  static HubbleGalaxy* Create(const String& strName);

  void Trace(blink::Visitor*) override;

  int64_t handle();
  String name();
  String getid();

  HubbleNode* getGrid(const String& clusterName, const String& nodeName);
  HubbleNode* getGrid(const long nodeHandle);

  DEFINE_ATTRIBUTE_EVENT_LISTENER(MessageReceived, kHubblegalaxy)
      DEFINE_ATTRIBUTE_EVENT_LISTENER(MdiChildActivate, kMdichildactivate)
      DEFINE_ATTRIBUTE_EVENT_LISTENER(GridCreated, kGridcreated)

  HubbleGalaxy();
  HubbleGalaxy(const String& strWindowName);

  ~HubbleGalaxy() override;

  int64_t handle_ = 0;

  map<wstring, HubbleNode*> m_mapRootNode;
  map<int64_t, HubbleNode*> m_mapHubbleNode;
  map<wstring, HubbleNode*> m_mapHubbleNode2;
private:
  String m_strWindowXml;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_WINDOW_H_
// end Add by HubbleTeam

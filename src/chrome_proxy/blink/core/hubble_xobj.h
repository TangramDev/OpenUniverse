#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_VAR_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_VAR_H_

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
#include "c:/universework/openuniverse/src/chrome_proxy/third_party/ChromeRenderDomProxy.h"

namespace blink {

using namespace std;

class Hubble;
class HubbleNode;
class HubbleGalaxy;
class HubbleWinform;
class Document;
class ScriptState;
class ExceptionState;
class V8HubbleCallback;
class WebLocalFrameClient;
class SerializedScriptValue;
class V8ApplicationCallback;

class CORE_EXPORT HubbleXobj final : public ScriptWrappable{
  DEFINE_WRAPPERTYPEINFO();
friend class Hubble;
 public:
  static HubbleXobj* Create() { return MakeGarbageCollected<HubbleXobj>(); }
  static HubbleXobj* Create(const String& strName);

  void Trace(blink::Visitor*) override;

  HubbleXobj();
  HubbleXobj(const String& strNodeName);

  ~HubbleXobj() override;

  String name();
  String getid();
  HubbleNode* grid();
  HubbleWinform* form();
  HubbleGalaxy* galaxy();

  String getStr(const String& strKey);
  void setStr(const String& strKey, const String& value);
  long getLong(const String& strKey);
  void setLong(const String& strKey, long value);
  int64_t getInt64(const String& strKey);
  void setInt64(const String& strKey, int64_t value);
  float getFloat(const String& strKey);
  void setFloat(const String& strKey, float value);

  Element* getVisibleElement(const String& strKey);
  void setVisibleElement(const String& strKey, Element* value);

  void addEventListener(const String& eventName, V8ApplicationCallback* callback);
  void addEventListener(const String& subObjName, const String& eventName, V8ApplicationCallback* callback);
  void removeEventListener(const String& eventName);
  void disConnect();
  void fireEvent(const String& eventName, HubbleXobj* eventParam);
  void sendMessage(HubbleXobj* msg, V8ApplicationCallback* callback);
  void invokeCallback(wstring callbackid, HubbleXobj* callbackParam);

  String id_;
  CommonUniverse::IPCSession session_;
  WebLocalFrameClient* m_pRenderframeImpl;
  mutable Member<Hubble> hubble_;
  HeapHashMap<String, Member<Element>> mapVisibleElem;
  HeapHashMap<String, Member<V8ApplicationCallback>> mapHubbleEventCallback_;
private:
  String name_;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_TANGRAM_VAR_H_

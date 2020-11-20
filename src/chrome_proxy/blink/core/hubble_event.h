#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_EVENTS_TANGRAM_EVENT_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_EVENTS_TANGRAM_EVENT_H_

#include "third_party/blink/renderer/core/dom/events/Event.h"
#include "third_party/blink/renderer/core/dom/element.h"

namespace blink {

  class Element;
  class ScriptState;
  class HubbleXobj;
  class HubbleNode;
  class HubbleGalaxy;
  class HubbleWinform;

  class CORE_EXPORT HubbleEvent : public Event {
    DEFINE_WRAPPERTYPEINFO();

  public:
    static HubbleEvent* Create() { return MakeGarbageCollected<HubbleEvent>(); }
    
    static HubbleEvent* Create(const AtomicString& event_type, HubbleXobj* var);

    static HubbleEvent* Create(const AtomicString& event_type,
      const String& eventId,
      const String& param1,
      const String& param2,
      const String& param3,
      const String& param4,
      const String& param5,
      const int64_t handleSource,
      const int64_t handleTarget,
        HubbleXobj*);

    static HubbleEvent* Create(const AtomicString& event_type,
      const String& eventId,
      const String& param1,
      const int64_t handleSource,
      const String& param3,
      const int64_t handleTarget,
      const String& param5);

    String eventId();
    void setEventId(const String&);
    String param1();
    void setParam1(const String&);
    String param2();
    void setParam2(const String&);
    String param3();
    void setParam3(const String&);
    String param4();
    void setParam4(const String&);
    String param5();
    void setParam5(const String&);
    HubbleXobj* xobj();
    void setXobj(HubbleXobj*);

    int64_t handleSource();
    void setHandleSource(const int64_t);
    int64_t handleTarget();
    void setHandleTarget(const int64_t);

    int64_t hwnd();
    String name();
    String objtype();
    HubbleNode* grid();
    HubbleWinform* form();
    HubbleGalaxy* galaxy();

    ~HubbleEvent() override;

    HubbleEvent();

    HubbleEvent(const AtomicString& event_type,
      const String& eventId,
      const String& param1,
      const String& param2,
      const String& param3,
      const String& param4,
      const String& param5,
      const int64_t handleSource,
      const int64_t handleTarget,
        HubbleXobj* session);

    void Trace(blink::Visitor*) override;

  private:
    String eventId_;
    String param1_;
    String param2_;
    String param3_;
    String param4_;
    String param5_;
    int64_t handleSource_;
    int64_t handleTarget_;
    Member<HubbleXobj> session_;
  };

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_EVENTS_PROGRESS_EVENT_H_

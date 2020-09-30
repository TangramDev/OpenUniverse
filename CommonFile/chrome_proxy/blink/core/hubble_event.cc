#include "hubble_xobj.h"
#include "hubble_event.h"
#include "hubble_node.h"
#include "hubble_winform.h"

namespace blink {

  HubbleEvent* HubbleEvent::Create(const AtomicString& event_type,
    const String& eventId,
    const String& param1,
    const String& param2,
    const String& param3,
    const String& param4,
    const String& param5,
    const int64_t handleSource,
    const int64_t handleTarget, HubbleXobj* session) {
      return MakeGarbageCollected<HubbleEvent>(event_type, eventId, param1, param2, param3, param4, param5, handleSource, handleTarget, session);
  }

  HubbleEvent* HubbleEvent::Create(const AtomicString& event_type,
    const String& eventId,
    const String& param1,
    const int64_t handleSource,
    const String& param3,
    const int64_t handleTarget,
    const String& param5) {
      return MakeGarbageCollected<HubbleEvent>(event_type, eventId, param1, "", param3, "", param5, handleSource, handleTarget, nullptr);
  }
  
  HubbleEvent* HubbleEvent::Create(const AtomicString& event_type, HubbleXobj* session)
  {
      return MakeGarbageCollected<HubbleEvent>(event_type, "", "", "", "", "", "", 0, 0, session);
  }

  void HubbleEvent::Trace(blink::Visitor* visitor) {
      Event::Trace(visitor);
      visitor->Trace(session_);
  }

  HubbleXobj* HubbleEvent::xobj()
  {
      return session_;
  }

  void HubbleEvent::setXobj(HubbleXobj* session)
  {
      session_ = session;
  }

  String HubbleEvent::eventId() {
      return eventId_;
  }

  int64_t HubbleEvent::handleSource() {
      return handleSource_;
  }

  void HubbleEvent::setHandleSource(const int64_t handleSource) {
      handleSource_ = handleSource;
  }

  int64_t HubbleEvent::handleTarget() {
      return handleTarget_;
  }

  void HubbleEvent::setHandleTarget(const int64_t handleTarget) {
      handleTarget_ = handleTarget;
  }

  void HubbleEvent::setEventId(const String& eventId) {
      eventId_ = eventId;
  }

  String HubbleEvent::param1() {
      return param1_;
  }

  void HubbleEvent::setParam1(const String& param1) {
      param1_ = param1;
  }

  String HubbleEvent::param2() {
    return param2_;
  }

  void HubbleEvent::setParam2(const String& param2) {
    param2_ = param2;
  }

  String HubbleEvent::param3() {
    return param3_;
  }

  void HubbleEvent::setParam3(const String& param3) {
    param3_ = param3;
  }

  String HubbleEvent::param4() {
    return param4_;
  }

  void HubbleEvent::setParam4(const String& param4) {
      param4_ = param4;
  }

  String HubbleEvent::param5() {
    return param5_;
  }

  void HubbleEvent::setParam5(const String& param5) {
    param5_ = param5;
  }

  int64_t HubbleEvent::hwnd()
  {
      int64_t h = session_->getInt64(L"gridobjhandle");
      if(h==0)
          h = session_->getInt64(L"formhandle");
      if(h==0)
          h = session_->getInt64(L"hwnd");
      if(h==0)
          h = session_->getInt64(L"hWnd");
      return h;
  }

  String HubbleEvent::name() {
    return session_->name();
  }

  String HubbleEvent::objtype() {
    return session_->getStr(L"objtype");
  }

  HubbleWinform* HubbleEvent::form()
  {
      if (session_)
          return session_->form();
      return nullptr;
  }

  HubbleNode* HubbleEvent::grid()
  {
      if (session_)
          return session_->grid();
      return nullptr;
  }

  HubbleWindow* HubbleEvent::window()
  {
      if (session_)
      {
          HubbleNode* node = session_->grid();
          if (node)
          {
              return node->parentWindow();
          }
      }
      return nullptr;
  }

  HubbleEvent::HubbleEvent() : Event("", Bubbles::kNo, Cancelable::kNo) {

  }

  HubbleEvent::HubbleEvent(const AtomicString& event_type,
    const String& eventId,
    const String& param1,
    const String& param2,
    const String& param3,
    const String& param4,
    const String& param5,
    const int64_t handleSource,
    const int64_t handleTarget,
      HubbleXobj* session)
    : Event(event_type, Bubbles::kNo, Cancelable::kNo) {
    eventId_ = eventId;
    param1_ = param1;
    param2_ = param2;
    param3_ = param3;
    param4_ = param4;
    param5_ = param5;
    handleSource_ = handleSource;
    handleTarget_ = handleTarget;
    session_ = session;
  }

  HubbleEvent::~HubbleEvent() = default;

}  // namespace blink

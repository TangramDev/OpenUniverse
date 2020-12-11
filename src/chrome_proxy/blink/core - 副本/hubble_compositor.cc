#include "hubble.h"
#include "hubble_xobj.h"
#include "hubble_node.h"
#include "hubble_event.h"
#include "hubble_winform.h"
#include "hubble_compositor.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/html/html_element.h"
#include "third_party/blink/renderer/core/html/html_head_element.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/public/web/web_local_frame_client.h"
#include "third_party/blink/renderer/core/frame/web_local_frame_impl.h"

namespace blink {

HubbleCompositor::HubbleCompositor(LocalFrame* frame) : DOMWindowClient(frame) {
	m_pRenderframeImpl = nullptr;
	id_ = WTF::CreateCanonicalUUIDString();
}

HubbleCompositor::~HubbleCompositor() {
}

HubbleCompositor* HubbleCompositor::Create(LocalFrame* frame, const String& strNodeXml) {
	return MakeGarbageCollected<HubbleCompositor>(frame, strNodeXml);
}

HubbleCompositor::HubbleCompositor(LocalFrame* frame, const String& strNodeXml) : DOMWindowClient(frame)
{
	m_strGridXml = strNodeXml;
}

void HubbleCompositor::Trace(blink::Visitor* visitor) {
  EventTargetWithInlineData::Trace(visitor);
  ScriptWrappable::Trace(visitor);
  DOMWindowClient::Trace(visitor);
  visitor->Trace(innerXobj_);
}

void HubbleCompositor::AddedEventListener(const AtomicString& event_type,
                                 RegisteredEventListener& registered_listener) {
  EventTargetWithInlineData::AddedEventListener(event_type,
                                                registered_listener);
}

String HubbleCompositor::getid()
{
	return id_;
}

String HubbleCompositor::name() {
	return name_;
}

void HubbleCompositor::sendMessage(const String& id, const String& param1, const String& param2, const String& param3, const String& param4, const String& param5) {
	if (m_pRenderframeImpl) {
		WebString webstr = id;
		std::wstring u16_id = webstr.Utf16();
		webstr = param1;
		std::wstring u16_param1 = webstr.Utf16();
		webstr = param2;
		std::wstring u16_param2 = webstr.Utf16();
		webstr = param3;
		std::wstring u16_param3 = webstr.Utf16();
		webstr = param4;
		std::wstring u16_param4 = webstr.Utf16();
		webstr = param5;
		std::wstring u16_param5 = webstr.Utf16();
		m_pRenderframeImpl->SendHubbleMessage(u16_id, u16_param1, u16_param2, u16_param3, u16_param4, u16_param5);
	}
}

const AtomicString& HubbleCompositor::InterfaceName() const {
  return event_target_names::kHubbleCompositor;
}

ExecutionContext* HubbleCompositor::GetExecutionContext() const {
  return DomWindow()->document();
}

}  // namespace blink
// end Add by HubbleTeam

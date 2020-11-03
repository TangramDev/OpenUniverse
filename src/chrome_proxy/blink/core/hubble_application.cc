#include "hubble.h"
#include "hubble_xobj.h"
#include "hubble_node.h"
#include "hubble_event.h"
#include "hubble_winform.h"
#include "hubble_application.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/html/html_element.h"
#include "third_party/blink/renderer/core/html/html_head_element.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/public/web/web_local_frame_client.h"
#include "third_party/blink/renderer/core/frame/web_local_frame_impl.h"

namespace blink {

	HubbleApplication::HubbleApplication(LocalFrame* frame) : DOMWindowClient(frame) {
		web_local_frame_client = nullptr;
		innerXobj_ = HubbleXobj::Create(L"");
	}

	HubbleApplication::~HubbleApplication() {
	}

	HubbleApplication* HubbleApplication::Create(LocalFrame* frame, const String& strNodeXml) {
		return MakeGarbageCollected<HubbleApplication>(frame, strNodeXml);
	}

	HubbleApplication::HubbleApplication(LocalFrame* frame, const String& strNodeXml) : DOMWindowClient(frame)
	{
		name_ = strNodeXml;
	}

	void HubbleApplication::Trace(blink::Visitor* visitor) {
		EventTargetWithInlineData::Trace(visitor);
		ScriptWrappable::Trace(visitor);
		DOMWindowClient::Trace(visitor);
		visitor->Trace(innerXobj_);
	}

	void HubbleApplication::AddedEventListener(const AtomicString& event_type,
		RegisteredEventListener& registered_listener) {
		EventTargetWithInlineData::AddedEventListener(event_type,
			registered_listener);
	}

	String HubbleApplication::name() {
		return name_;
	}

	void HubbleApplication::sendMessage(const String& id, const String& param1, const String& param2, const String& param3, const String& param4, const String& param5) {
		WebLocalFrameImpl* web_local_frame_impl = WebLocalFrameImpl::FromFrame(GetFrame());
		// Null when opening a new tab.
		if (web_local_frame_impl != nullptr) {
			WebLocalFrameClient* web_local_frame_client = web_local_frame_impl->Client();
			if (web_local_frame_client) {
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
				web_local_frame_client->SendHubbleMessage(u16_id, u16_param1, u16_param2, u16_param3, u16_param4, u16_param5);
			}
		}
	}

	const AtomicString& HubbleApplication::InterfaceName() const {
		return event_target_names::kHubbleApplication;
	}

	ExecutionContext* HubbleApplication::GetExecutionContext() const {
		return DomWindow()->document();
	}

}  // namespace blink

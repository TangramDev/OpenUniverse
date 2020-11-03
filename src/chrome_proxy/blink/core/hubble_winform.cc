#include "hubble.h"
#include "hubble_node.h"
#include "hubble_xobj.h"
#include "hubble_event.h"
#include "hubble_winform.h"
#include "hubble_control.h"
#include "hubble_window.h"
#include "hubble_compositor.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/html/html_element.h"
#include "third_party/blink/renderer/core/html/html_head_element.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/public/web/web_local_frame_client.h"
#include "third_party/blink/renderer/core/frame/web_local_frame_impl.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_application_callback.h"
#include "third_party/blink/renderer/platform/wtf/uuid.h"

namespace blink {

	HubbleWinform::HubbleWinform(LocalFrame* frame) : DOMWindowClient(frame) {
		m_pRenderframeImpl = nullptr;
		id_ = WTF::CreateCanonicalUUIDString();
	}

	HubbleWinform::~HubbleWinform() {
		Hubble* pHubble = hubble_.Get();
		if (pHubble)
		{
			auto it = pHubble->m_mapHubbleWinformforEvent.find(handle_);
			if (it != pHubble->m_mapHubbleWinformforEvent.end())
				pHubble->m_mapHubbleWinformforEvent.erase(it);

			auto it2 = pHubble->m_mapWinForm.find(handle_);
			if (it2 != pHubble->m_mapWinForm.end())
				pHubble->m_mapWinForm.erase(it2);
		}
		mapHubbleEventCallback_.clear();
	}

	HubbleWinform* HubbleWinform::Create(LocalFrame* frame, const String& strNodeName) {
		return MakeGarbageCollected<HubbleWinform>(frame, strNodeName);
	}

	HubbleWinform::HubbleWinform(LocalFrame* frame, const String& strNodeName) : DOMWindowClient(frame)
	{
		name_ = strNodeName;
	}

	void HubbleWinform::Trace(blink::Visitor* visitor) {
		EventTargetWithInlineData::Trace(visitor);
		ScriptWrappable::Trace(visitor);
		DOMWindowClient::Trace(visitor);
		visitor->Trace(hubble_);
		visitor->Trace(innerXobj_);
		visitor->Trace(mapHubbleEventCallback_);
	}

	void HubbleWinform::AddedEventListener(const AtomicString& event_type,
		RegisteredEventListener& registered_listener) {
		EventTargetWithInlineData::AddedEventListener(event_type,
			registered_listener);
	}

	String HubbleWinform::name() {
		return name_;
	}

	HubbleXobj* HubbleWinform::xobj()
	{
		return innerXobj_;
	}

	String HubbleWinform::getid()
	{
		return id_;
	}

	int64_t HubbleWinform::handle() {
		return handle_;
	}

	void HubbleWinform::SyncCtrlTextChange(const String& strcontrols, V8ApplicationCallback* callback)
	{
		if (callback)
		{
			innerXobj_->setStr(L"eventtype", L"SyncCtrlTextChange");
			innerXobj_->setStr(L"ctrls", strcontrols);
			innerXobj_->addEventListener(L"SyncCtrlTextChange", L"OnTextChanged", callback);
		}
	}

	void HubbleWinform::addEventListener(const String& eventName, V8ApplicationCallback* callback)
	{
		blink::Hubble* pHubble = (blink::Hubble*)DomWindow()->apppage();
		if (callback)
		{
			auto it = innerXobj_->session_.m_mapString.find(L"objID");
			if (it != innerXobj_->session_.m_mapString.end())
			{
				pHubble->m_mapHubbleWinformforEvent[handle_] = this;
				mapHubbleEventCallback_.insert(eventName, callback);
				innerXobj_->session_.m_mapString[L"listenerType"] = WebString(eventName).Utf16();
				innerXobj_->session_.m_mapString[L"SenderType"] = L"HubbleForm";
				innerXobj_->session_.m_mapString[L"Sender"] = WebString(id_).Utf16();
				sendMessage(innerXobj_, nullptr);
			}
		}
	}

	void HubbleWinform::addEventListener(const String& subObjName, const String& eventName, V8ApplicationCallback* callback)
	{
		if (callback)
		{
			innerXobj_->addEventListener(subObjName, eventName, callback);
		}
	}

	void HubbleWinform::fireEvent(const String& eventName, HubbleXobj* eventParam)
	{
		auto itcallback = mapHubbleEventCallback_.find(eventName);
		if (itcallback != mapHubbleEventCallback_.end())
		{
			blink::V8ApplicationCallback* callback = (blink::V8ApplicationCallback*)itcallback->value;
			ScriptState* callback_relevant_script_state = callback->CallbackRelevantScriptState();
			ScriptState::Scope callback_relevant_context_scope(callback_relevant_script_state);
			callback->InvokeAndReportException(nullptr, eventParam);
		}
	}

	const AtomicString& HubbleWinform::InterfaceName() const {
		return event_target_names::kHubbleWinForm;
	}

	ExecutionContext* HubbleWinform::GetExecutionContext() const {
		return DomWindow()->document();
	}

	void HubbleWinform::removeEventListener(const String& eventName)
	{
		auto it = mapHubbleEventCallback_.find(eventName);
		if (it != mapHubbleEventCallback_.end())
			mapHubbleEventCallback_.erase(it);
	}

	void HubbleWinform::disConnect()
	{
		int nSize = mapHubbleEventCallback_.size();
		if (nSize)
		{
			while (mapHubbleEventCallback_.size())
			{
				auto it = mapHubbleEventCallback_.begin();
				mapHubbleEventCallback_.erase(it);
			}
		}
	}

	void HubbleWinform::sendMessage(HubbleXobj* msg, V8ApplicationCallback* callback)
	{
		if (m_pRenderframeImpl)
		{
			if (msg == nullptr)
				msg = innerXobj_;
			msg->setStr(L"senderid", id_);
			if (callback)
			{
				String callbackid_ = WTF::CreateCanonicalUUIDString();
				msg->setStr(L"callbackid", callbackid_);
				mapHubbleEventCallback_.insert(callbackid_, callback);
				WebString strID = callbackid_;
				m_pRenderframeImpl->m_mapHubbleSession[strID.Utf16()] = this;
			}
			m_pRenderframeImpl->SendHubbleMessageEx(msg->session_);
		}
	}

	void HubbleWinform::invokeCallback(wstring callbackid, HubbleXobj* callbackParam)
	{
		auto itcallback = mapHubbleEventCallback_.find(String(callbackid.c_str()));
		if (itcallback != mapHubbleEventCallback_.end())
		{
			blink::V8ApplicationCallback* callback = (blink::V8ApplicationCallback*)itcallback->value.Get();
			mapHubbleEventCallback_.erase(itcallback);
			ScriptState* callback_relevant_script_state = callback->CallbackRelevantScriptState();
			ScriptState::Scope callback_relevant_context_scope(callback_relevant_script_state);
			callback->InvokeAndReportException(nullptr, callbackParam);
		}
	}

}  // namespace blink

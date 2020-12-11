#include "hubble.h"
#include "hubble_node.h"
#include "hubble_event.h"
#include "hubble_xobj.h"
#include "hubble_winform.h"
#include "hubble_control.h"
#include "hubble_galaxy.h"
#include "hubble_compositor.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/html/html_element.h"
#include "third_party/blink/renderer/core/html/html_head_element.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/public/web/web_local_frame_client.h"
#include "third_party/blink/renderer/core/frame/web_local_frame_impl.h"
#include "third_party/blink/renderer/platform/bindings/callback_function_base.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_application_callback.h"

namespace blink {

	HubbleControl::HubbleControl(LocalFrame* frame) : DOMWindowClient(frame) {
		handle_ = 0;
		type_ = L"";
		events_ = L"";
		id_ = WTF::CreateCanonicalUUIDString();
		m_pRenderframeImpl = nullptr;
	}

	HubbleControl::~HubbleControl() {
		Hubble* pHubble = hubble_.Get();
		if (pHubble)
		{
			auto it = pHubble->m_mapHubbleControlforEvent.find(handle_);
			if (it != pHubble->m_mapHubbleControlforEvent.end())
				pHubble->m_mapHubbleControlforEvent.erase(it);
		}
		mapHubbleEventCallback_.clear();
	}

	HubbleControl* HubbleControl::Create(LocalFrame* frame, const String& strNodeName) {
		return MakeGarbageCollected<HubbleControl>(frame, strNodeName);
	}

	HubbleControl::HubbleControl(LocalFrame* frame, const String& strNodeName) : DOMWindowClient(frame)
	{
		name_ = strNodeName;
	}

	int64_t HubbleControl::handle() {
		return handle_;
	}

	HubbleNode* HubbleControl::parentGrid()
	{
		return m_pParentNode;
	}

	HubbleWinform* HubbleControl::parentForm()
	{
		return m_pParentForm;
	}

	HubbleControl* HubbleControl::parentControl()
	{
		return m_pParentControl;
	}

	void HubbleControl::BindCtrlEvents(const String& strEvents)
	{
		if (handle_)
		{
			if (m_pRenderframeImpl) {
				m_pRenderframeImpl->m_mapHubbleControl[handle_] = (long)this;
				WebString webstr = strEvents;
				std::wstring _strBindEvents = webstr.Utf16();
				webstr = name_;
				std::wstring _strCtrlID = webstr.Utf16();
				webstr = type_;
				std::wstring _strCtrlType = webstr.Utf16();
				m_pRenderframeImpl->SendHubbleMessage(L"TANGRAM_CTRL_BIND_MESSAGE", _strCtrlID, handle_, 2020, _strBindEvents, _strCtrlType);
			}
		}
	}

	void HubbleControl::addEventListener(const String& eventName, V8ApplicationCallback* callback)
	{
		blink::Hubble* pHubble = (blink::Hubble*)DomWindow()->cosmos();
		if (callback)
		{
			auto it = innerXobj_->session_.m_mapString.find(L"objID");
			if (it != innerXobj_->session_.m_mapString.end())
			{
				pHubble->m_mapHubbleControlforEvent[handle_] = this;
				mapHubbleEventCallback_.insert(eventName, callback);
				innerXobj_->session_.m_mapString[L"listenerType"] = WebString(eventName).Utf16();
				innerXobj_->session_.m_mapString[L"SenderType"] = L"HubbleControl";
				innerXobj_->session_.m_mapString[L"Sender"] = WebString(id_).Utf16();
				sendMessage(innerXobj_, nullptr);
			}
		}
	}

	void HubbleControl::removeEventListener(const String& eventName)
	{
		auto it = mapHubbleEventCallback_.find(eventName);
		if (it != mapHubbleEventCallback_.end())
			mapHubbleEventCallback_.erase(it);
	}

	void HubbleControl::fireEvent(const String& eventName, HubbleXobj* eventParam)
	{
		auto itcallback = mapHubbleEventCallback_.find(eventName);
		if (itcallback != mapHubbleEventCallback_.end())
		{
			blink::V8ApplicationCallback* callback = (blink::V8ApplicationCallback*)itcallback->value.Get();
			ScriptState* callback_relevant_script_state = callback->CallbackRelevantScriptState();
			ScriptState::Scope callback_relevant_context_scope(callback_relevant_script_state);
			callback->InvokeAndReportException(nullptr, eventParam);
		}
	}

	void HubbleControl::sendMessage(HubbleXobj* msg, V8ApplicationCallback* callback)
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

	void HubbleControl::invokeCallback(wstring callbackid, HubbleXobj* callbackParam)
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

	void HubbleControl::Trace(blink::Visitor* visitor) {
		EventTargetWithInlineData::Trace(visitor);
		ScriptWrappable::Trace(visitor);
		DOMWindowClient::Trace(visitor);
		visitor->Trace(hubble_);
		visitor->Trace(innerXobj_);
		visitor->Trace(m_pParentNode);
		visitor->Trace(m_pParentForm);
		visitor->Trace(m_pParentControl);
		visitor->Trace(mapHubbleEventCallback_);
	}

	void HubbleControl::AddedEventListener(const AtomicString& event_type,
		RegisteredEventListener& registered_listener) {
		EventTargetWithInlineData::AddedEventListener(event_type,
			registered_listener);
	}

	String HubbleControl::name() {
		return name_;
	}

	String HubbleControl::type() {
		return type_;
	}

	HubbleXobj* HubbleControl::xobj()
	{
		return innerXobj_;
	}

	const AtomicString& HubbleControl::InterfaceName() const {
		return event_target_names::kHubbleControl;
	}

	ExecutionContext* HubbleControl::GetExecutionContext() const {
		return DomWindow()->document();
	}

}  // namespace blink

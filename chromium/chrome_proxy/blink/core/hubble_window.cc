#include "hubble.h"
#include "hubble_xobj.h"
#include "hubble_node.h"
#include "hubble_window.h"
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
#include "third_party/blink/renderer/bindings/core/v8/v8_application_callback.h"

namespace blink {

	HubbleWindow::HubbleWindow(LocalFrame* frame) : DOMWindowClient(frame) {
		m_pHostNode = nullptr;
		m_pRenderframeImpl = nullptr;
		id_ = WTF::CreateCanonicalUUIDString();
	}

	HubbleWindow::~HubbleWindow() {
		Hubble* pHubble = hubble_.Get();
		if (pHubble)
		{
			WebString str = name_;
			auto it = pHubble->m_mapHubbleWindow2.find(str.Utf16());
			if (it != pHubble->m_mapHubbleWindow2.end())
				pHubble->m_mapHubbleWindow2.erase(it);

			auto it2 = pHubble->m_mapHubbleWindow.find(handle_);
			if (it2 != pHubble->m_mapHubbleWindow.end())
				pHubble->m_mapHubbleWindow.erase(it2);
		}
	}

	HubbleWindow* HubbleWindow::Create(LocalFrame* frame, const String& strWindowName) {
		return MakeGarbageCollected<HubbleWindow>(frame, strWindowName);
	}

	HubbleWindow::HubbleWindow(LocalFrame* frame, const String& strWindowName) : DOMWindowClient(frame)
	{
		name_ = strWindowName;
	}

	void HubbleWindow::Trace(blink::Visitor* visitor) {
		EventTargetWithInlineData::Trace(visitor);
		ScriptWrappable::Trace(visitor);
		DOMWindowClient::Trace(visitor);
		visitor->Trace(hubble_);
		visitor->Trace(innerXobj_);
		visitor->Trace(m_pHostNode);
		visitor->Trace(mapHubbleEventCallback_);
	}

	void HubbleWindow::AddedEventListener(const AtomicString& event_type,
		RegisteredEventListener& registered_listener) {
		EventTargetWithInlineData::AddedEventListener(event_type,
			registered_listener);
	}

	String HubbleWindow::name() {
		return name_;
	}

	String HubbleWindow::getid()
	{
		return id_;
	}

	int64_t HubbleWindow::handle() {
		return handle_;
	}

	HubbleXobj* HubbleWindow::xobj()
	{
		return innerXobj_;
	}

	HubbleNode* HubbleWindow::getGrid(const String& nodeName)
	{
		WebString strName = nodeName;
		auto it = m_mapHubbleNode2.find(strName.Utf16());
		if (it != m_mapHubbleNode2.end())
			return it->second;
		return nullptr;
	}

	HubbleNode* HubbleWindow::getGrid(const long nodeHandle)
	{
		auto it = m_mapHubbleNode.find(nodeHandle);
		if (it != m_mapHubbleNode.end())
			return it->second;
		return nullptr;
	}

	const AtomicString& HubbleWindow::InterfaceName() const {
		return event_target_names::kHubbleWindow;
	}

	ExecutionContext* HubbleWindow::GetExecutionContext() const {
		return DomWindow()->document();
	}

	void HubbleWindow::addEventListener(const String& eventName, V8ApplicationCallback* callback)
	{
		if (callback)
		{
			auto it = innerXobj_->session_.m_mapString.find(L"objID");
			if (it != innerXobj_->session_.m_mapString.end())
			{
				hubble_->m_mapHubbleWindow2[WebString(id_).Utf16()] = this;
				mapHubbleEventCallback_.insert(eventName, callback);
				innerXobj_->session_.m_mapString[L"listenerType"] = WebString(eventName).Utf16();
				innerXobj_->session_.m_mapString[L"SenderType"] = L"HubbleControl";
				innerXobj_->session_.m_mapString[L"Sender"] = WebString(id_).Utf16();
				sendMessage(innerXobj_, nullptr);
			}
		}
	}

	void HubbleWindow::removeEventListener(const String& eventName)
	{
		auto it = mapHubbleEventCallback_.find(eventName);
		if (it != mapHubbleEventCallback_.end())
			mapHubbleEventCallback_.erase(it);
	}

	void HubbleWindow::disConnect()
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

	void HubbleWindow::fireEvent(const String& eventName, HubbleXobj* eventParam)
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

	void HubbleWindow::sendMessage(HubbleXobj* msg, V8ApplicationCallback* callback)
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

	void HubbleWindow::invokeCallback(wstring callbackid, HubbleXobj* callbackParam)
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

#include "hubble.h"
#include "hubble_xobj.h"
#include "hubble_node.h"
#include "hubble_galaxy.h"
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

	HubbleGalaxy::HubbleGalaxy(LocalFrame* frame) : DOMWindowClient(frame) {
		m_pHostNode = nullptr;
		m_pRenderframeImpl = nullptr;
		id_ = WTF::CreateCanonicalUUIDString();
	}

	HubbleGalaxy::~HubbleGalaxy() {
		Hubble* pHubble = hubble_.Get();
		if (pHubble)
		{
			WebString str = name_;
			auto it = pHubble->m_mapHubbleGalaxy2.find(str.Utf16());
			if (it != pHubble->m_mapHubbleGalaxy2.end())
				pHubble->m_mapHubbleGalaxy2.erase(it);

			auto it2 = pHubble->m_mapHubbleGalaxy.find(handle_);
			if (it2 != pHubble->m_mapHubbleGalaxy.end())
				pHubble->m_mapHubbleGalaxy.erase(it2);
		}
	}

	HubbleGalaxy* HubbleGalaxy::Create(LocalFrame* frame, const String& strWindowName) {
		return MakeGarbageCollected<HubbleGalaxy>(frame, strWindowName);
	}

	HubbleGalaxy::HubbleGalaxy(LocalFrame* frame, const String& strWindowName) : DOMWindowClient(frame)
	{
		name_ = strWindowName;
	}

	void HubbleGalaxy::Trace(blink::Visitor* visitor) {
		EventTargetWithInlineData::Trace(visitor);
		ScriptWrappable::Trace(visitor);
		DOMWindowClient::Trace(visitor);
		visitor->Trace(hubble_);
		visitor->Trace(innerXobj_);
		visitor->Trace(m_pHostNode);
		visitor->Trace(mapHubbleEventCallback_);
	}

	void HubbleGalaxy::AddedEventListener(const AtomicString& event_type,
		RegisteredEventListener& registered_listener) {
		EventTargetWithInlineData::AddedEventListener(event_type,
			registered_listener);
	}

	String HubbleGalaxy::name() {
		return name_;
	}

	String HubbleGalaxy::getid()
	{
		return id_;
	}

	int64_t HubbleGalaxy::handle() {
		return handle_;
	}

	HubbleXobj* HubbleGalaxy::xobj()
	{
		return innerXobj_;
	}

	HubbleNode* HubbleGalaxy::getGrid(const String& clusterName, const String& nodeName)
	{
		if (nodeName == "undefined")
		{
			auto it = m_mapRootNode.find(WebString(clusterName).Utf16());
			if (it != m_mapRootNode.end())
				return it->second;
			return nullptr;
		}
		String strName = clusterName + "__" + nodeName;
		auto it = m_mapHubbleNode2.find(WebString(strName).Utf16());
		if (it != m_mapHubbleNode2.end())
			return it->second;
		return nullptr;
	}

	HubbleNode* HubbleGalaxy::getGrid(const long nodeHandle)
	{
		auto it = m_mapHubbleNode.find(nodeHandle);
		if (it != m_mapHubbleNode.end())
			return it->second;
		return nullptr;
	}

	const AtomicString& HubbleGalaxy::InterfaceName() const {
		return event_target_names::kHubbleGalaxy;
	}

	ExecutionContext* HubbleGalaxy::GetExecutionContext() const {
		return DomWindow()->document();
	}

	void HubbleGalaxy::addEventListener(const String& eventName, V8ApplicationCallback* callback)
	{
		if (callback)
		{
			auto it = innerXobj_->session_.m_mapString.find(L"objID");
			if (it != innerXobj_->session_.m_mapString.end())
			{
				hubble_->m_mapHubbleGalaxy2[WebString(id_).Utf16()] = this;
				mapHubbleEventCallback_.insert(eventName, callback);
				innerXobj_->session_.m_mapString[L"listenerType"] = WebString(eventName).Utf16();
				innerXobj_->session_.m_mapString[L"SenderType"] = L"HubbleControl";
				innerXobj_->session_.m_mapString[L"Sender"] = WebString(id_).Utf16();
				sendMessage(innerXobj_, nullptr);
			}
		}
	}

	void HubbleGalaxy::removeEventListener(const String& eventName)
	{
		auto it = mapHubbleEventCallback_.find(eventName);
		if (it != mapHubbleEventCallback_.end())
			mapHubbleEventCallback_.erase(it);
	}

	void HubbleGalaxy::disConnect()
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

	void HubbleGalaxy::fireEvent(const String& eventName, HubbleXobj* eventParam)
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

	void HubbleGalaxy::sendMessage(HubbleXobj* msg, V8ApplicationCallback* callback)
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

	void HubbleGalaxy::invokeCallback(wstring callbackid, HubbleXobj* callbackParam)
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

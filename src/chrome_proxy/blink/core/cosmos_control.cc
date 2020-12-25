/********************************************************************************
 *            DOM Plus for Application - Version 1.1.8.202012250001
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
// Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *
 *******************************************************************************/

#include "cosmos.h"
#include "cosmos_node.h"
#include "cosmos_event.h"
#include "cosmos_xobj.h"
#include "cosmos_winform.h"
#include "cosmos_control.h"
#include "cosmos_galaxy.h"
#include "cosmos_compositor.h"

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

	CosmosControl::CosmosControl(LocalFrame* frame) : DOMWindowClient(frame) {
		handle_ = 0;
		type_ = L"";
		events_ = L"";
		id_ = WTF::CreateCanonicalUUIDString();
		m_pRenderframeImpl = nullptr;
	}

	CosmosControl::~CosmosControl() {
		Cosmos* pCosmos = cosmos_.Get();
		if (pCosmos)
		{
			auto it = pCosmos->m_mapCosmosControlforEvent.find(handle_);
			if (it != pCosmos->m_mapCosmosControlforEvent.end())
				pCosmos->m_mapCosmosControlforEvent.erase(it);
		}
		mapCosmosEventCallback_.clear();
	}

	CosmosControl* CosmosControl::Create(LocalFrame* frame, const String& strNodeName) {
		return MakeGarbageCollected<CosmosControl>(frame, strNodeName);
	}

	CosmosControl::CosmosControl(LocalFrame* frame, const String& strNodeName) : DOMWindowClient(frame)
	{
		name_ = strNodeName;
	}

	int64_t CosmosControl::handle() {
		return handle_;
	}

	CosmosNode* CosmosControl::parent()
	{
		return m_pParentNode;
	}

	CosmosWinform* CosmosControl::parentForm()
	{
		return m_pParentForm;
	}

	CosmosControl* CosmosControl::parentControl()
	{
		return m_pParentControl;
	}

	void CosmosControl::BindCtrlEvents(const String& strEvents)
	{
		if (handle_)
		{
			if (m_pRenderframeImpl) {
				//m_pRenderframeImpl->m_mapCosmosControl[handle_] = (long)this;
				WebString webstr = strEvents;
				std::wstring _strBindEvents = webstr.Utf16();
				webstr = name_;
				std::wstring _strCtrlID = webstr.Utf16();
				webstr = type_;
				std::wstring _strCtrlType = webstr.Utf16();
				m_pRenderframeImpl->SendCosmosMessage(L"TANGRAM_CTRL_BIND_MESSAGE", _strCtrlID, handle_, 2020, _strBindEvents, _strCtrlType);
			}
		}
	}

	void CosmosControl::addEventListener(const String& eventName, V8ApplicationCallback* callback)
	{
		blink::Cosmos* pCosmos = (blink::Cosmos*)DomWindow()->cosmos();
		if (callback)
		{
			auto it = innerXobj_->session_.m_mapString.find(L"objID");
			if (it != innerXobj_->session_.m_mapString.end())
			{
				pCosmos->m_mapCosmosControlforEvent[handle_] = this;
				mapCosmosEventCallback_.insert(eventName, callback);
				innerXobj_->session_.m_mapString[L"listenerType"] = WebString(eventName).Utf16();
				innerXobj_->session_.m_mapString[L"SenderType"] = L"CosmosControl";
				innerXobj_->session_.m_mapString[L"Sender"] = WebString(id_).Utf16();
				sendMessage(innerXobj_, nullptr);
			}
		}
	}

	void CosmosControl::removeEventListener(const String& eventName)
	{
		auto it = mapCosmosEventCallback_.find(eventName);
		if (it != mapCosmosEventCallback_.end())
			mapCosmosEventCallback_.erase(it);
	}

	void CosmosControl::fireEvent(const String& eventName, CosmosXobj* eventParam)
	{
		auto itcallback = mapCosmosEventCallback_.find(eventName);
		if (itcallback != mapCosmosEventCallback_.end())
		{
			blink::V8ApplicationCallback* callback = (blink::V8ApplicationCallback*)itcallback->value.Get();
			ScriptState* callback_relevant_script_state = callback->CallbackRelevantScriptState();
			ScriptState::Scope callback_relevant_context_scope(callback_relevant_script_state);
			callback->InvokeAndReportException(nullptr, eventParam);
		}
	}

	void CosmosControl::sendMessage(CosmosXobj* msg, V8ApplicationCallback* callback)
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
				mapCosmosEventCallback_.insert(callbackid_, callback);
				WebString strID = callbackid_;
				m_pRenderframeImpl->m_mapCosmosSession[strID.Utf16()] = this;
			}
			m_pRenderframeImpl->SendCosmosMessageEx(msg->session_);
		}
	}

	void CosmosControl::invokeCallback(wstring callbackid, CosmosXobj* callbackParam)
	{
		auto itcallback = mapCosmosEventCallback_.find(String(callbackid.c_str()));
		if (itcallback != mapCosmosEventCallback_.end())
		{
			blink::V8ApplicationCallback* callback = (blink::V8ApplicationCallback*)itcallback->value.Get();
			mapCosmosEventCallback_.erase(itcallback);
			ScriptState* callback_relevant_script_state = callback->CallbackRelevantScriptState();
			ScriptState::Scope callback_relevant_context_scope(callback_relevant_script_state);
			callback->InvokeAndReportException(nullptr, callbackParam);
		}
	}

	void CosmosControl::Trace(blink::Visitor* visitor) {
		EventTargetWithInlineData::Trace(visitor);
		ScriptWrappable::Trace(visitor);
		DOMWindowClient::Trace(visitor);
		visitor->Trace(cosmos_);
		visitor->Trace(innerXobj_);
		visitor->Trace(m_pParentNode);
		visitor->Trace(m_pParentForm);
		visitor->Trace(m_pParentControl);
		visitor->Trace(mapCosmosEventCallback_);
	}

	void CosmosControl::AddedEventListener(const AtomicString& event_type,
		RegisteredEventListener& registered_listener) {
		EventTargetWithInlineData::AddedEventListener(event_type,
			registered_listener);
	}

	String CosmosControl::name() {
		return name_;
	}

	String CosmosControl::type() {
		return type_;
	}

	CosmosXobj* CosmosControl::xobj()
	{
		return innerXobj_;
	}

	const AtomicString& CosmosControl::InterfaceName() const {
		return event_target_names::kCosmosControl;
	}

	ExecutionContext* CosmosControl::GetExecutionContext() const {
		return DomWindow()->document();
	}

}  // namespace blink

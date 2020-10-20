#include "hubble.h"
#include "hubble_xobj.h"
#include "hubble_node.h"
#include "hubble_event.h"
#include "hubble_window.h"
#include "hubble_winform.h"
#include "hubble_control.h"
#include "hubble_compositor.h"
#include <stdlib.h>
#include <string>
#include <iostream>

#include "base/guid.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/html/html_element.h"
#include "third_party/blink/renderer/core/html/html_head_element.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/public/web/web_local_frame_client.h"
#include "third_party/blink/renderer/core/frame/web_local_frame_impl.h"
#include "third_party/blink/renderer/platform/bindings/callback_function_base.h"

#include "third_party/blink/renderer/bindings/core/v8/v8_hubble_callback.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_application_callback.h"

#include "C:\src\tangram\Source\chrome_proxy\third_party\ChromeRenderDomProxy.h"

namespace blink {

	Hubble::Hubble(LocalFrame* frame) : DOMWindowClient(frame) {
		is_pending_ = false;
		innerXobj_ = newVar(L"");
		url_ = L"";
		m_pRenderframeImpl = nullptr;
	}

	Hubble::~Hubble() {
	}

	void Hubble::Trace(blink::Visitor* visitor) {
		EventTargetWithInlineData::Trace(visitor);
		ScriptWrappable::Trace(visitor);
		DOMWindowClient::Trace(visitor);
		visitor->Trace(m_mapHubbleNode);
		visitor->Trace(m_mapHubbleWindow);
		visitor->Trace(m_mapWinForm);
		visitor->Trace(m_mapHubbleCompositor);
		visitor->Trace(mapHubbleCallback_);
		visitor->Trace(mapCallbackFunction_);
		visitor->Trace(innerXobj_);
		visitor->Trace(mapCloudSession_);
	}

	String Hubble::url()
	{
		return DomWindow()->document()->Url().GetString();
	}

	HubbleXobj* Hubble::xobj()
	{
		return innerXobj_;
	}

	void Hubble::wait(bool bwait)
	{
		if (bwait)
			run_loop_.Run();
		else
			run_loop_.Quit();
	}

	void Hubble::AddedEventListener(const AtomicString& event_type,
		RegisteredEventListener& registered_listener) {
		EventTargetWithInlineData::AddedEventListener(event_type,
			registered_listener);
	}

	void Hubble::sendMessage(const String& id, const String& param1, const String& param2, const String& param3, const String& param4, const String& param5) {
		if (m_pRenderframeImpl == nullptr)
		{
			m_pRenderframeImpl = WebLocalFrameImpl::FromFrame(GetFrame())->Client();
			innerXobj_->m_pRenderframeImpl = m_pRenderframeImpl;
		}
		if (m_pRenderframeImpl) {
			if (is_pending_) {
				String message = id + "%%%" + param1 + "%%%" + param2 + "%%%" + param3 + "%%%" + param4 + "%%%" + param5;
				pending_messages_.push_back(message);
			}
			else {
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
	}

	// Define a element
	void Hubble::defineElement(const String& tagName, const String& html) {
		sendMessage(L"DEFINE_ELEMENT", tagName, html, L"", L"", L"");
	}

	// Render an element.
	void Hubble::renderElement(const String& tagName, const String& html) {
		sendMessage(L"RENDER_ELEMENT", tagName, html, L"", L"", L"");
	}

	void Hubble::waitMessage() {
		is_pending_ = true;
	}

	void Hubble::releaseMessage() {
		is_pending_ = false;
		if (m_pRenderframeImpl) {
			if (pending_messages_.size() > 0) {
				String stringBuffer = "";
				for (wtf_size_t i = 0; i < pending_messages_.size(); i++) {
					if (i > 0) {
						stringBuffer = stringBuffer + "$$$";
					}
					stringBuffer = stringBuffer + pending_messages_[i];
				}
				WebString bundle = stringBuffer;
				std::wstring type = L"AGGREGATED_MESSAGE";
				std::wstring param1 = bundle.Utf16();
				m_pRenderframeImpl->SendHubbleMessage(type, param1, L"", L"", L"", L"");
				pending_messages_.clear();
			}
		}
	}

	HubbleXobj* Hubble::newVar(const String& strName)
	{
		HubbleXobj* var = HubbleXobj::Create(strName);
		var->m_pRenderframeImpl = m_pRenderframeImpl;
		var->id_ = WTF::CreateCanonicalUUIDString();
		var->session_.m_mapString[L"sessionid"] = WebString(var->id_).Utf16();
		var->hubble_ = this;
		return var;
	}

	HubbleWinform* Hubble::createWinForm(const String& strFormXml, const long FormType, V8ApplicationCallback* callback)
	{
		HubbleWinform* form = HubbleWinform::Create(DomWindow()->GetFrame(), strFormXml);
		form->hubble_ = this;
		form->innerXobj_ = newVar(L"");
		form->handle_ = (int64_t)form;
		if (callback)
			mapCallbackFunction_.insert(form->handle_, callback);
		form->m_pRenderframeImpl = m_pRenderframeImpl;
		m_mapWinForm.insert(form->handle_, form);
		if (m_pRenderframeImpl) {
			//WebString webstr = strFormXml;
			//std::wstring _strFormXml = webstr.Utf16();
			form->innerXobj_->setStr(L"msgID", L"CREATE_WINFORM");
			form->innerXobj_->setStr(L"objID", L"WinForm");
			form->innerXobj_->setInt64(L"form", (int64_t)form);
			form->innerXobj_->setInt64(L"innerXobj", (int64_t)form->innerXobj_.Get());
			form->innerXobj_->setStr(L"formXml", strFormXml);
			form->innerXobj_->setLong(L"formType", FormType);
			m_pRenderframeImpl->SendHubbleMessageEx(form->innerXobj_->session_);
		}
		return form;
	}

	HubbleWinform* Hubble::newWinForm(int64_t handle, HubbleXobj* obj)
	{
		HubbleWinform* form = HubbleWinform::Create(DomWindow()->GetFrame(), L"");
		form->hubble_ = this;
		form->innerXobj_ = obj;
		form->handle_ = handle;
		form->innerXobj_->setStr(L"objID", L"WinForm");
		form->m_pRenderframeImpl = m_pRenderframeImpl;
		m_mapWinForm.insert(form->handle_, form);
		return form;
	}

	HubbleNode* Hubble::createHubbleNode(HubbleXobj* xobj)
	{
		__int64 handle = xobj->getInt64(L"gridobjhandle");
		__int64 nRootHandle = xobj->getInt64(L"rootgridhandle");
		String strname = xobj->getStr(L"name@page");
		blink::HubbleWindow* window = nullptr;
		bool bNewWindow = false;
		if (nRootHandle)
		{
			auto it1 = m_mapHubbleWindow.find(nRootHandle);
			if (it1 != m_mapHubbleWindow.end())
				window = it1->value;
			else
			{
				bNewWindow = true;
				window = HubbleWindow::Create(DomWindow()->GetFrame(), strname);
				window->hubble_ = this;
				window->innerXobj_ = xobj;
				window->handle_ = handle;
				window->m_pRenderframeImpl = m_pRenderframeImpl;
				m_mapHubbleWindow.insert(handle, window);
				WebString str = strname;
				m_mapHubbleWindow2[str.Utf16()] = window;
			}
		}

		HubbleNode* node = HubbleNode::Create(DomWindow()->GetFrame(), strname);
		node->hubble_ = this;
		node->innerXobj_ = xobj;
		node->handle_ = handle;
		node->m_pRenderframeImpl = m_pRenderframeImpl;
		m_mapHubbleNode.insert(handle, node);
		if(bNewWindow)
			DispatchEvent(*blink::HubbleEvent::Create(blink::event_type_names::kHubblewindowcreated, xobj));

		__int64 nPHandle = xobj->getInt64(L"parentgridhandle");
		if (nPHandle)
		{
			auto it = m_mapHubbleNode.find(nPHandle);
			if (it != m_mapHubbleNode.end())
			{
				WebString str = strname;
				it->value->m_mapChildNode2[str.Utf16()] = node;
			}
		}
		if (window)
		{
			window->m_mapHubbleNode[handle] = node;
			window->DispatchEvent(*blink::HubbleEvent::Create(blink::event_type_names::kGridcreated, xobj));
		}
		
		nPHandle = xobj->getInt64(L"parentFormHandle");
		if (nPHandle)
		{
			auto it = m_mapWinForm.find(nPHandle);
			if (it != m_mapWinForm.end())
			{
				HubbleWinform* form = it->value.Get();
				form->DispatchEvent(*blink::HubbleEvent::Create(blink::event_type_names::kGridcreated, xobj));
			}
		}

		DispatchEvent(*blink::HubbleEvent::Create(blink::event_type_names::kGridcreated, xobj));

		return node;
	}

	HubbleNode* Hubble::getGrid(const int64_t nodeHandle)
	{
		auto it = m_mapHubbleNode.find(nodeHandle);
		if (it != m_mapHubbleNode.end())
			return it->value;
		return nullptr;
	}

	HubbleWindow* Hubble::getWindow(const String& wndName)
	{
		WebString str = wndName;
		auto it = m_mapHubbleWindow2.find(str.Utf16());
		if (it != m_mapHubbleWindow2.end())
			return it->second;
		return nullptr;
	}

	HubbleWindow* Hubble::getWindow(const int64_t wndHandle)
	{
		if (wndHandle)
		{
			auto it = m_mapHubbleWindow.find(wndHandle);
			if (it != m_mapHubbleWindow.end())
				return it->value;
		}
		return nullptr;
	}

	//void Hubble::sendIPCMessage(const String& type, const String& param1, const String& param2) {
	//	if (m_pRenderframeImpl) {
	//		if (is_bundling_) {
	//			String message = type + "%%%" + param1 + "%%%" + param2;
	//			pending_messages_.push_back(message);
	//		}
	//		else {
	//			WebString webstr = type;
	//			std::wstring u16_type = webstr.Utf16();
	//			webstr = param1;
	//			std::wstring u16_param1 = webstr.Utf16();
	//			webstr = param2;
	//			std::wstring u16_param2 = webstr.Utf16();
	//			//m_pRenderframeImpl->SendHubbleMessage(u16_type, u16_param1, u16_param2);
	//		}
	//	}
	//}

	const AtomicString& Hubble::InterfaceName() const {
		return event_target_names::kHubble;
	}

	ExecutionContext* Hubble::GetExecutionContext() const {
		return DomWindow()->document();
	}

	void Hubble::addEventListener(const String& eventName, V8ApplicationCallback* callback)
	{
		if (callback)
		{
			innerXobj_->mapHubbleEventCallback_.insert(eventName, callback);
		}
	}

	void Hubble::removeEventListener(const String& eventName)
	{
		auto it = innerXobj_->mapHubbleEventCallback_.find(eventName);
		if (it != innerXobj_->mapHubbleEventCallback_.end())
			innerXobj_->mapHubbleEventCallback_.erase(it);
	}

	void Hubble::disConnect()
	{
		int nSize = innerXobj_->mapHubbleEventCallback_.size();
		if (nSize)
		{
			while (innerXobj_->mapHubbleEventCallback_.size())
			{
				auto it = innerXobj_->mapHubbleEventCallback_.begin();
				innerXobj_->mapHubbleEventCallback_.erase(it);
			}
		}
	}

	void Hubble::invokeWinFormCreatedCallback(HubbleWinform* form)
	{
		auto itcallback = mapCallbackFunction_.find((int64_t)form);
		if (itcallback != mapCallbackFunction_.end())
		{
			blink::V8ApplicationCallback* callback = (blink::V8ApplicationCallback*)itcallback->value.Get();
			ScriptState* callback_relevant_script_state = callback->CallbackRelevantScriptState();
			ScriptState::Scope callback_relevant_context_scope(callback_relevant_script_state);
			callback->InvokeAndReportException(nullptr, form->innerXobj_);
			mapCallbackFunction_.erase(itcallback);
		}
	}

	void Hubble::fireEvent(const String& eventName, HubbleXobj* eventParam)
	{
		auto itcallback = innerXobj_->mapHubbleEventCallback_.find(eventName);
		if (itcallback != innerXobj_->mapHubbleEventCallback_.end())
		{
			blink::V8ApplicationCallback* callback = (blink::V8ApplicationCallback*)itcallback->value.Get();
			ScriptState* callback_relevant_script_state = callback->CallbackRelevantScriptState();
			ScriptState::Scope callback_relevant_context_scope(callback_relevant_script_state);
			callback->InvokeAndReportException(nullptr, eventParam);
		}
	}

	void Hubble::sendMessage(HubbleXobj* msg, V8ApplicationCallback* callback, bool bwait=false)
	{
		if (m_pRenderframeImpl)
		{
			if (msg == nullptr)
				msg = innerXobj_;
			msg->setStr(L"senderid", innerXobj_->getid());
			if (callback)
			{
				String callbackid_ = WTF::CreateCanonicalUUIDString();
				msg->setStr(L"callbackid", callbackid_);
				innerXobj_->mapHubbleEventCallback_.insert(callbackid_, callback);
				WebString strID = callbackid_;
				m_pRenderframeImpl->m_mapHubbleSession[strID.Utf16()] = this;
			}
			m_pRenderframeImpl->SendHubbleMessageEx(msg->session_);
		}
		//if (bwait)
		//	run_loop_.Run();
	}

	void Hubble::openUrl(const String& url, long nBrowserWndOpenDisposition, V8ApplicationCallback* callback, bool bwait=false)
	{
		if (m_pRenderframeImpl)
		{
			innerXobj_->setStr(L"senderid", innerXobj_->getid());
			innerXobj_->setStr(L"msgID", L"OPEN_URL");
			innerXobj_->setStr(L"openurl", url);
			innerXobj_->setLong(L"BrowserWndOpenDisposition", nBrowserWndOpenDisposition);
			if (callback)
			{
				String callbackid_ = WTF::CreateCanonicalUUIDString();
				innerXobj_->setStr(L"callbackid", callbackid_);
				innerXobj_->mapHubbleEventCallback_.insert(callbackid_, callback);
				WebString strID = callbackid_;
				m_pRenderframeImpl->m_mapHubbleSession[strID.Utf16()] = this;
			}
			m_pRenderframeImpl->SendHubbleMessageEx(innerXobj_->session_);
		}
		//if (bwait)
		//	run_loop_.Run();
	}

	HubbleXobj* Hubble::getNamedItem(const AtomicString& name) const {
		auto it = mapCloudSession_.find(name);
		if (it != mapCloudSession_.end())
			return it->value.Get();
		return nullptr;
	}

	void Hubble::setNamedItem(const String& name, HubbleXobj* var, ExceptionState&)
	{
		DCHECK(var);
		mapCloudSession_.insert(name, var);
	}

	HubbleXobj* Hubble::setNamedItem(HubbleXobj* var, ExceptionState& exception_state) {
		DCHECK(var);
		mapCloudSession_.insert(var->name(), var);
		return var;
	}

	void Hubble::NamedPropertyEnumerator(Vector<String>& names,
		ExceptionState&) const {
		for (auto it : mapCloudSession_) {
			names.UncheckedAppend(it.value->name());
		}
	}

	bool Hubble::NamedPropertyQuery(const AtomicString& name,
		ExceptionState& exception_state) const {
		Vector<String> properties;
		NamedPropertyEnumerator(properties, exception_state);
		return properties.Contains(name);
	}

	void Hubble::Close()
	{
		while (mapCloudSession_.size())
		{
			mapCloudSession_.erase(mapCloudSession_.begin());
		}
		while (m_mapHubbleNode.size())
		{
			m_mapHubbleNode.erase(m_mapHubbleNode.begin());
		}
		while (m_mapHubbleWindow.size())
		{
			m_mapHubbleWindow.erase(m_mapHubbleWindow.begin());
		}
		while (m_mapWinForm.size())
		{
			m_mapWinForm.erase(m_mapWinForm.begin());
		}
		while (m_mapHubbleCompositor.size())
		{
			m_mapHubbleCompositor.erase(m_mapHubbleCompositor.begin());
		}
		while (mapHubbleCallback_.size())
		{
			mapHubbleCallback_.erase(mapHubbleCallback_.begin());
		}
		while (mapCallbackFunction_.size())
		{
			mapCallbackFunction_.erase(mapCallbackFunction_.begin());
		}
	}

}  // namespace blink

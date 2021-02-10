/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202102100029           *
 ********************************************************************************
 * Copyright (C) 2002-2021 by Tangram Team.   All Rights Reserved.
 * There are Three Key Features of Webruntime:
 * 1. Built-in Modern Web Browser: Independent Browser Window and Browser Window
 *    as sub windows of other windows are supported in the application process;
 * 2. DOM Plus: DOMPlus is a natural extension of the standard DOM system.
 *    It allows the application system to support a kind of generalized web pages,
 *    which are composed of standard DOM elements and binary components supported
 *    by the application system;
 * 3. JavaScript for Application: Similar to VBA in MS office, JavaScript will
 *    become a built-in programmable language in the application system, so that
 *    the application system can be expanded and developed for the Internet based
 *    on modern javscript/Web technology.
 * Use of this source code is governed by a BSD-style license that
 * can be found in the LICENSE file.
 *
 * CONTACT INFORMATION:
 * mailto:tangramteam@outlook.com or mailto:sunhuizlz@yeah.net
 * https://www.tangram.dev
 *******************************************************************************/

#include "cosmos.h"
#include "cosmos_node.h"
#include "cosmos_event.h"
#include "cosmos_galaxy.h"
#include "cosmos_winform.h"
#include "cosmos_control.h"
#include "cosmos_compositor.h"
#include <stdlib.h>
#include <iostream>

#include "base/guid.h"
#include "base/strings/string_split.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/document_fragment.h"
#include "third_party/blink/renderer/core/dom/dom_token_list.h"
#include "third_party/blink/renderer/core/dom/element.h"
#include "third_party/blink/renderer/core/dom/node.h"
#include "third_party/blink/renderer/core/dom/node_list.h"
#include "third_party/blink/renderer/core/dom/name_node_list.h"
#include "third_party/blink/renderer/core/html/html_element.h"
#include "third_party/blink/renderer/core/html/html_head_element.h"
#include "third_party/blink/renderer/core/dom/class_collection.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/public/web/web_local_frame_client.h"
#include "third_party/blink/renderer/core/frame/web_local_frame_impl.h"
#include "third_party/blink/renderer/platform/bindings/callback_function_base.h"

#include "third_party/blink/renderer/bindings/core/v8/v8_cosmos_callback.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_application_callback.h"
#include "third_party/blink/renderer/core/xml/dom_parser.h"
#include "third_party/blink/renderer/bindings/core/v8/string_or_trusted_html.h"

#include "../../third_party/ChromeRenderDomProxy.h"

namespace blink {

	Cosmos::Cosmos(LocalFrame* frame) : CosmosXobj(), DOMWindowClient(frame) {
		url_ = L"";
		is_pending_ = false;
		helperElem_ = nullptr;
		CosmosElem_ = nullptr;
		m_pRenderframeImpl = nullptr;
		DocumentFragment_ = nullptr;
		m_pVisibleContentElement = nullptr;
		ExceptionState exception_state(V8PerIsolateData::MainThreadIsolate(), ExceptionState::kSetterContext,
			"Element", "creatHelper");
		helperElem_ = DomWindow()->document()->CreateElementForBinding("cosmos", exception_state);
	}

	Cosmos::~Cosmos() {
	}

	void Cosmos::Trace(blink::Visitor* visitor) {
		EventTargetWithInlineData::Trace(visitor);
		ScriptWrappable::Trace(visitor);
		CosmosXobj::Trace(visitor);
		DOMWindowClient::Trace(visitor);
		visitor->Trace(helperElem_);
		visitor->Trace(CosmosElem_);
		visitor->Trace(m_mapWinForm);
		visitor->Trace(m_mapCosmosNode);
		visitor->Trace(mapCloudSession_);
		visitor->Trace(m_mapCosmosGalaxy);
		visitor->Trace(mapCallbackFunction_);
		visitor->Trace(m_pVisibleContentElement);
	}

	String Cosmos::url()
	{
		return DomWindow()->document()->Url().GetString();
	}

	void Cosmos::wait(bool bwait)
	{
		if (bwait)
			run_loop_.Run();
		else
			run_loop_.Quit();
	}

	void Cosmos::AddedEventListener(const AtomicString& event_type,
		RegisteredEventListener& registered_listener) {
		EventTargetWithInlineData::AddedEventListener(event_type,
			registered_listener);
	}

	void Cosmos::sendMessage(const String& id, const String& param1, const String& param2, const String& param3, const String& param4, const String& param5) {
		if (m_pRenderframeImpl == nullptr)
		{
			m_pRenderframeImpl = WebLocalFrameImpl::FromFrame(GetFrame())->Client();
			m_pRenderframeImpl = m_pRenderframeImpl;
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
				m_pRenderframeImpl->SendCosmosMessage(u16_id, u16_param1, u16_param2, u16_param3, u16_param4, u16_param5);
			}
		}
	}

	// Define a element
	void Cosmos::defineElement(const String& tagName, const String& html) {
		sendMessage(L"DEFINE_ELEMENT", tagName, html, L"", L"", L"");
	}

	// Render an element.
	void Cosmos::renderElement(const String& tagName, const String& html) {
		sendMessage(L"RENDER_ELEMENT", tagName, html, L"", L"", L"");
	}

	void Cosmos::waitMessage() {
		is_pending_ = true;
	}

	void Cosmos::releaseMessage() {
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
				m_pRenderframeImpl->SendCosmosMessage(type, param1, L"", L"", L"", L"");
				pending_messages_.clear();
			}
		}
	}

	CosmosXobj* Cosmos::newVar(const String& strName)
	{
		CosmosXobj* var = CosmosXobj::Create(strName);
		var->m_pRenderframeImpl = m_pRenderframeImpl;
		var->cosmos_ = this;
		return var;
	}

	CosmosWinform* Cosmos::createWinForm(Element* elem, const long FormType, V8ApplicationCallback* callback)
	{
		CosmosWinform* form = CosmosWinform::Create("");
		form->cosmos_ = this;
		form->handle_ = (int64_t)form;
		if (callback)
			mapCallbackFunction_.insert(form->handle_, callback);
		form->m_pRenderframeImpl = m_pRenderframeImpl;
		m_mapWinForm.insert(form->handle_, form);

		if (m_pRenderframeImpl) {
			form->setStr(L"msgID", L"CREATE_WINFORM");
			form->setStr(L"objID", L"WinForm");
			form->setInt64(L"form", (int64_t)form);
			form->setStr(L"formXml", elem->OuterHTMLAsString());
			form->setLong(L"formType", FormType);
			m_pRenderframeImpl->SendCosmosMessageEx(form->session_);
		}
		return form;
	}

	CosmosWinform* Cosmos::createWinForm(const String& strFormXml, const long FormType, V8ApplicationCallback* callback)
	{
		CosmosWinform* form = CosmosWinform::Create("");
		form->cosmos_ = this;
		form->handle_ = (int64_t)form;
		if (callback)
			mapCallbackFunction_.insert(form->handle_, callback);
		form->m_pRenderframeImpl = m_pRenderframeImpl;
		m_mapWinForm.insert(form->handle_, form);

		if (m_pRenderframeImpl) {
			form->setStr(L"msgID", L"CREATE_WINFORM");
			form->setStr(L"objID", L"WinForm");
			form->setInt64(L"form", (int64_t)form);
			form->setStr(L"formXml", strFormXml);
			form->setLong(L"formType", FormType);
			m_pRenderframeImpl->SendCosmosMessageEx(form->session_);
		}
		return form;
	}

	CosmosXobj* Cosmos::createObject(Element* elem,V8ApplicationCallback* callback)
	{
		CosmosXobj* var = CosmosXobj::Create("");
		var->m_pRenderframeImpl = m_pRenderframeImpl;
		var->cosmos_ = this;
		mapCloudSession_.insert(var->id_, var);
		if (callback)
			mapCallbackFunction_.insert((__int64)var, callback);
		if (m_pRenderframeImpl) {
			var->setStr(L"msgID", L"CREATE_CLROBJ");
			var->setStr(L"objID", L"CLROBJ");
			var->setInt64(L"objhandle", (int64_t)var);
			var->setStr(L"objXml", elem->OuterHTMLAsString());
			m_pRenderframeImpl->SendCosmosMessageEx(var->session_);
		}
		return var;
	}

	CosmosXobj* Cosmos::createObject(const String& strObjXml, V8ApplicationCallback* callback)
	{
		CosmosXobj* var = CosmosXobj::Create("");
		var->m_pRenderframeImpl = m_pRenderframeImpl;
		var->cosmos_ = this;
		mapCloudSession_.insert(var->id_, var);
		if (callback)
			mapCallbackFunction_.insert((__int64)var, callback);
		if (m_pRenderframeImpl) {
			var->setStr(L"msgID", L"CREATE_CLROBJ");
			var->setStr(L"objID", L"CLROBJ");
			var->setInt64(L"objhandle", (int64_t)var);
			var->setStr(L"objXml", strObjXml);
			m_pRenderframeImpl->SendCosmosMessageEx(var->session_);
		}
		return var;
	}

	CosmosWinform* Cosmos::newWinForm(int64_t handle, CosmosXobj* obj)
	{
		CosmosWinform* form = CosmosWinform::Create(L"");
		form->cosmos_ = this;
		form->handle_ = handle;
		form->setStr(L"objID", L"WinForm");
		form->m_pRenderframeImpl = m_pRenderframeImpl;
		m_mapWinForm.insert(form->handle_, form);
		return form;
	}

	CosmosWinform* Cosmos::CreateForm(int64_t handle, CosmosXobj* obj)
	{
		CosmosWinform* form = (CosmosWinform*)obj;
		form->cosmos_ = this;
		form->handle_ = handle;
		form->setStr(L"objID", L"WinForm");
		form->m_pRenderframeImpl = m_pRenderframeImpl;
		m_mapWinForm.insert(form->handle_, form);
		form->InitWinForm();

		long nFormType = obj->getLong(L"WinFormType");
		switch (nFormType)
		{
		case 0:
			DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kLoadwinform, obj));
			break;
		case 1:
			DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kLoadmdiwinform, obj));
			break;
		case 2:
		{
			CosmosWinform* parentform = form->mdiParent();
			if (parentform)
			{
				parentform->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kLoadmdichildwinform, obj));
				//form->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kMdichildactivate, obj));
				//parentform->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kMdichildactivate, obj));
			}
		}
		break;
		default:
			break;
		}
		return form;
	}

	void Cosmos::MdiChildActive(CosmosXobj* xobj)
	{
		__int64 handle = xobj->getInt64(L"active_mdichildhandle");
		if (handle)
		{
			CosmosWinform* form = nullptr;
			CosmosWinform* parentmdiform = nullptr;
			auto it = m_mapWinForm.find(handle);
			if (it != m_mapWinForm.end())
			{
				form = it->value.Get();
				parentmdiform = form->mdiParent();
				parentmdiform->m_pActiveMDIChild = form;
				ExceptionState exception_state(nullptr,
					ExceptionState::kExecutionContext,
					"MdiChildActive",
					"");
				if (form->m_pContentElement == nullptr)
				{
					ClassCollection* contentCollection = DomWindow()->document()->getElementsByClassName(/*parentmdiform->name() + */"applicationcontents");
					if (contentCollection)
					{
						HTMLCollection* contentsElements = contentCollection->item(0)->Children();
						if (contentsElements)
						{
							String strname = form->name();
							WebString _strName = strname;
							for (Element* contentElement : *contentsElements)
							{
								if (contentElement->classList().contains(_strName))
								{
									contentElement->classList().remove({ "hidden" }, exception_state);
									contentElement->classList().add({ "show" }, exception_state);
									form->m_pContentElement = contentElement;
									if (m_pVisibleContentElement == nullptr)
										m_pVisibleContentElement = contentElement;
									else
									{
										m_pVisibleContentElement->classList().remove({ "show" }, exception_state);
										m_pVisibleContentElement->classList().add({ "hidden" }, exception_state);
										m_pVisibleContentElement = contentElement;
										break;
									}
								}
								else
								{
									contentElement->classList().remove({ "show" }, exception_state);
									contentElement->classList().add({ "hidden" }, exception_state);
								}

							}
						}
					}
				}
				else
				{
					if (m_pVisibleContentElement != form->m_pContentElement)
					{
						form->m_pContentElement->classList().remove({ "hidden" }, exception_state);
						form->m_pContentElement->classList().add({ "show" }, exception_state);
						m_pVisibleContentElement->classList().remove({ "show" }, exception_state);
						m_pVisibleContentElement->classList().add({ "hidden" }, exception_state);
						m_pVisibleContentElement = form->m_pContentElement;
					}
				}
				if (parentmdiform && form)
				{
					form->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kMdichildactivate, xobj));
					parentmdiform->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kMdichildactivate, xobj));
				}
			}
		}
	}

	void Cosmos::AllMdiChildRemoved(CosmosXobj* xobj)
	{
		__int64 nHandle = xobj->getInt64(L"formhandle");
		CosmosWinform* form = nullptr;
		auto it = m_mapWinForm.find(nHandle);
		if (it != m_mapWinForm.end())
		{
			form = it->value.Get();
			form->m_pActiveMDIChild = nullptr;
			if (m_pVisibleContentElement)
			{
				ExceptionState exception_state(nullptr,
					ExceptionState::kExecutionContext,
					"AllMdiChildRemoved",
					"");
				m_pVisibleContentElement->classList().remove({ "show" }, exception_state);
				m_pVisibleContentElement->classList().add({ "hidden" }, exception_state);
				m_pVisibleContentElement = nullptr;
				ClassCollection* contentCollection = DomWindow()->document()->getElementsByClassName(/*form->name() + */"applicationcontents");
				if (contentCollection)
				{
					HTMLCollection* contentsElements = contentCollection->item(0)->Children();
					if (contentsElements)
					{
						for (Element* contentElement : *contentsElements)
						{
							if (contentElement->classList().contains(WebString(form->name())))
							{
								contentElement->classList().remove({ "hidden" }, exception_state);
								contentElement->classList().add({ "show" }, exception_state);
								break;
							}
						}
					}
				}
			}

			sendMessage("TANGRAM_UI_MESSAGE", "__VIEWPORT_DEFAULT__", L"", L"", L"", L"");
			form->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kAllmdichildremoved, xobj));
		}
	}

	void Cosmos::OnMessage(Element* e, const String& eventName)
	{
		Element* element = static_cast<Element*>(e->childNodes()->item(1));
		if (!!element) {
			for (unsigned int i = 1; i < element->childNodes()->length(); i++) {
				Element* elem = static_cast<Element*>(element->childNodes()->item(i));
				if (elem)
				{
					CosmosNode* xobjfortarget = nullptr;
					Node* pNode = elem;
					if (pNode->getNodeType() == 1)
					{
						AtomicString _strHandle = elem->getAttribute(AtomicString(eventName));
						if (_strHandle.IsNull() == false && _strHandle != "")
						{
							String s = _strHandle;
							wstring _strHandle = WebString(s).Utf16();
							__int64 nHandle = std::stoll(_strHandle);
							auto it = m_mapCosmosNode.find(nHandle);
							if (it != m_mapCosmosNode.end())
							{
								xobjfortarget = it->value.Get();
							}
						}
					}
					if (xobjfortarget == nullptr)
					{
						xobjfortarget = getXobj(elem, nullptr);
						if (!!xobjfortarget)
						{
							__int64 nHandle = xobjfortarget->handle();
							wstring strHandle = std::to_wstring(nHandle);
							elem->setAttribute(AtomicString(eventName), AtomicString(String(strHandle.c_str())));
						}
					}
					if (!!xobjfortarget) {
						xobjfortarget->setWorkElement(elem);
						String strMsgID = e->GetIdAttribute() + "_" + eventName;
						xobjfortarget->setMsgID(strMsgID);
						xobjfortarget->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kCloudmessageforxobj, xobjfortarget));
					}
				}
			}
		}
	}

	void Cosmos::DispatchXobjEvent(CosmosXobj* xObj, const String& ctrlName, const String& eventName)
	{
		xObj->fireEvent(eventName + "@" + ctrlName, xObj);
		bool bFormMsgProcessed = false;
		bool bXobjMsgProcessed = false;
		HTMLCollection* list2 = nullptr;
		String ctrlName_ = ctrlName;
		if (ctrlName.IsNull() || ctrlName == "")
		{
			ctrlName_ = xObj->getStr(L"name@page");
		}
		String strXml = xObj->getStr(eventName + "Xml");
		if (strXml.IsNull() || strXml == "")
		{
			String strEvent = eventName + "@" + ctrlName;
			wstring strIndex = WebString(strEvent.LowerASCII()).Utf16();
			auto it = xObj->m_mapElement.find(strIndex);
			if (it != xObj->m_mapElement.end())
			{
				list2 = it->second->Children();
			}
		}
		else
		{
			HTMLCollection* list = xObj->DocumentFragment_->getElementsByTagName(AtomicString(xObj->getStr(eventName + "TagName")));
			if (list->length())
			{
				list = list->item(0)->getElementsByTagName(AtomicString(eventName.LowerASCII()));
				if (list->length())
				{
					list2 = list->item(0)->Children();
				}
			}
		}
		if (!!list2)
		{
			for (unsigned int i = 0; i < list2->length(); i++)
			{
				CosmosNode* xobjfortarget = nullptr;
				Element* elem = (Element*)list2->item(i);
				Node* pNode = elem;
				if (pNode->getNodeType() == 1) {
					AtomicString _strHandle = elem->getAttribute("targetgrid");
					if (_strHandle.IsNull() == false && _strHandle != "")
					{
						String s = _strHandle;
						wstring _strHandle = WebString(s).Utf16();
						__int64 nHandle = std::stoll(_strHandle);
						auto it = m_mapCosmosNode.find(nHandle);
						if (it != m_mapCosmosNode.end())
						{
							xobjfortarget = it->value.Get();
						}
					}

					if (xobjfortarget == nullptr)
					{
						AtomicString target = elem->getAttribute("target");
						if (target == "" || target.IsNull())
						{
							if (xObj->grid())
								xobjfortarget = xObj->grid();
							else
							{
								if (xObj->form())
								{
									xObj->form()->element_ = elem;
									xObj->form()->setMsgID(ctrlName_ + "_" + eventName);
									xObj->form()->setSender(xObj);
									bFormMsgProcessed = true;
									xObj->form()->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kCloudmessageforwinform, xObj));
								}
							}
						}
						else {
							xobjfortarget = getXobj(elem, xObj);
						}
						if (xobjfortarget)
						{
							bXobjMsgProcessed = true;
							__int64 nHandle = xobjfortarget->handle();
							wstring strHandle = std::to_wstring(nHandle);
							elem->setAttribute("targetgrid", AtomicString(String(strHandle.c_str())));
						}
					}
					if (!!xobjfortarget) {
						xobjfortarget->element_ = elem;
						xobjfortarget->setMsgID(ctrlName_ + "_" + eventName);
						xobjfortarget->setSender(xObj);
						xobjfortarget->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kCloudmessageforxobj, xobjfortarget));
						xobjfortarget->setMsgID(ctrlName_ + "_" + eventName);
						xobjfortarget->setStr("eventdata", elem->OuterHTMLAsString());
						m_pRenderframeImpl->SendCosmosMessageEx(xobjfortarget->session_);
					}
				}
			}
		}
		if(xObj->form()&&!bFormMsgProcessed)
			xObj->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kCloudmessageforwinform, xObj));
		else if(xObj->grid()&&!bXobjMsgProcessed)
			xObj->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kCloudmessageforxobj, xObj));
	}

	void Cosmos::ProcessMessage(CosmosXobj* xobj)
	{
		long isObj = xobj->getLong(L"isobject");
		if (isObj)
		{
			xobj->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kCloudmessageforobject, xobj));
			DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kCloudmessageforobject, xobj));
			return;
		}
		else
		{
			__int64 nHandle = xobj->getInt64(L"formhandle");
			if (nHandle)
			{
				CosmosWinform* form = nullptr;
				auto it = m_mapWinForm.find(nHandle);
				if (it != m_mapWinForm.end())
				{
					form = it->value.Get();
					form->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kCloudmessageforwinform, xobj));
					form->ProcessFormMessage(xobj->getStr(L"msgID"));
				}
			}
			nHandle = xobj->getInt64(L"xobjhandle");
			if (nHandle)
			{
				auto it = m_mapCosmosNode.find(nHandle);
				if (it != m_mapCosmosNode.end())
				{
					CosmosNode* grid = it->value.Get();
					grid->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kCloudmessageforxobj, xobj));
					grid->ProcessNodeMessage(xobj->getStr(L"msgID"));
				}
			}
		}
		DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kCosmosmessage, xobj));
	}

	void Cosmos::MdiChildReady(CosmosXobj* xobj)
	{
		__int64 handle = xobj->getInt64(L"ready_mdichildhandle");
		if (handle)
		{
			CosmosWinform* form = nullptr;
			CosmosWinform* parentmdiform = nullptr;
			auto it = m_mapWinForm.find(handle);
			if (it != m_mapWinForm.end())
			{
				form = it->value.Get();
				parentmdiform = form->mdiParent();
			}
			if (parentmdiform && form)
			{
				form->isReady_ = true;
				form->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kMdichildwinformready, xobj));
				parentmdiform->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kMdichildwinformready, xobj));
			}
		}
	}

	void Cosmos::CosmosObjCreated(CosmosXobj* xobj)
	{
		if (CosmosElem_ == nullptr)
		{
			HTMLCollection* list = DomWindow()->document()->getElementsByTagName("cosmos");
			if (list->length())
			{
				CosmosElem_ = list->item(0);
			}
		}
		String ObjTypeTag = xobj->getStr(L"ObjectTypeTag");
		if (CosmosElem_ && ObjTypeTag != "")
		{
			String ObjType = xobj->getStr(L"objid");
			String sessionid = xobj->getStr(L"sessionid");
			if (ObjType != "" && ObjType.IsNull() == false)
			{
				HTMLCollection* list = CosmosElem_->getElementsByTagName(AtomicString(ObjType.LowerASCII()));
				if (list->length())
				{
					list = list->item(0)->getElementsByTagName(AtomicString(ObjTypeTag.LowerASCII()));
					if (list->length())
					{
						Element* e = list->item(0);
						if (e)
						{
							mapCloudSession_.insert(sessionid, xobj);
							xobj->setLong("isobject", 1);
							xobj->setStr("objectxml", e->OuterHTMLAsString());
							xobj->sendMessage(xobj, nullptr);
						}
					}
				}
			}
			DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kCosmosobjectcreated, xobj));
		}
	}

	CosmosNode* Cosmos::createCosmosNode(CosmosXobj* xobj)
	{
		CosmosNode* node = (CosmosNode*)xobj;
		__int64 handle = node->handle_;
		auto itNode = m_mapCosmosNode.find(handle);
		if (itNode != m_mapCosmosNode.end())
			return itNode->value.Get();
		node->cosmos_ = this;
		CosmosWinform* parentform = nullptr;
		CosmosWinform* parentmdiform = nullptr;
		String strGalaxyname = xobj->getStr(L"galaxy");
		String strClustername = xobj->getStr(L"cluster");;
		String strname = xobj->getStr(L"name@page");
		m_mapCosmosNode.insert(handle, node);

		__int64 nPHandle = xobj->getInt64(L"parenthandle");
		if (nPHandle)
		{
			auto it = m_mapCosmosNode.find(nPHandle);
			if (it != m_mapCosmosNode.end())
			{
				WebString str = strname;
				it->value->m_mapChildNode2[str.Utf16()] = node;
			}
		}
		__int64 nGalaxyHandle = xobj->getInt64(L"Galaxyhandle");
		blink::CosmosGalaxy* pGalaxy = nullptr;
		CosmosNode* m_pRootNode = nullptr;
		bool bNewGalaxy = false;
		if (nGalaxyHandle)
		{
			auto it1 = m_mapCosmosGalaxy.find(nGalaxyHandle);
			if (it1 != m_mapCosmosGalaxy.end())
			{
				__int64 nRootHandle = xobj->getInt64(L"rootgridhandle");
				auto it = m_mapCosmosNode.find(nRootHandle);
				if (it != m_mapCosmosNode.end())
				{
					m_pRootNode = it->value;
				}
				pGalaxy = it1->value;
			}
			else
			{
				bNewGalaxy = true;
				m_pRootNode = node;
			}
		}
		nPHandle = xobj->getInt64(L"parentFormHandle");
		if (nPHandle)
		{
			auto it = m_mapWinForm.find(nPHandle);
			if (it != m_mapWinForm.end())
			{
				parentform = it->value.Get();
				node->m_pParentForm = parentform;
				if (parentform->m_nMdiwebbindgridhandle == handle)
					parentform->m_pWebBindMdiNode = node;
				parentform->m_mapCosmosGalaxy[WebString(strGalaxyname).Utf16()] = pGalaxy;
			}
			it = m_mapWinForm.find(xobj->getInt64(L"parentMDIFormHandle"));
			if (it != m_mapWinForm.end())
			{
				parentmdiform = it->value.Get();
			}
		}

		String strMessageXml = "";

		if (m_pRootNode)
		{
			node->rootNode_ = m_pRootNode;
			WebString str = strname;
			m_pRootNode->m_mapXobj[str.Utf16()] = node;

			strMessageXml = xobj->getStr(L"cosmosxml");
			if (enableConsoleInfo_)
			{
				GetFrame()->AddMessageToConsole(blink::mojom::ConsoleMessageLevel::kInfo, "Create a New Xobj:\n", false);
				GetFrame()->AddMessageToConsole(blink::mojom::ConsoleMessageLevel::kInfo, strMessageXml, false);
			}
			if (node == m_pRootNode && strMessageXml.IsNull() == false && strMessageXml != "")
			{
				node->DocumentFragment_ = DomWindow()->document()->createDocumentFragment();
				if (node->DocumentFragment_)
				{
					ExceptionState exception_state(V8PerIsolateData::MainThreadIsolate(), ExceptionState::kSetterContext,
						"Element", "outerHTML");
					helperElem_->SetInnerHTMLFromString(strMessageXml, exception_state);
					node->DocumentFragment_->appendChild(helperElem_->firstChild());
				}
			}
			xobj->setStr(L"cosmosxml", L"");
		}

		if (m_pRootNode->DocumentFragment_)
		{
			node->DocumentFragment_ = m_pRootNode->DocumentFragment_;
			String name(std::to_string(handle).c_str());
			blink::NameNodeList* list_ = m_pRootNode->DocumentFragment_->getElementsByName(AtomicString(name));
			if (list_->length())
			{
				node->hostElem_ = (Element*)list_->item(0);
				if (enableConsoleInfo_)
				{
					GetFrame()->AddMessageToConsole(blink::mojom::ConsoleMessageLevel::kInfo, "Xobj HTML Code:\n", false);
					GetFrame()->AddMessageToConsole(blink::mojom::ConsoleMessageLevel::kInfo, node->hostElem_->OuterHTMLAsString(), false);
				}
				for (unsigned int i = 0; i < node->hostElem_->Children()->length(); i++)
				{
					Element* e = node->hostElem_->Children()->item(i);
					Node* pNode = e;
					if (pNode->getNodeType() == 1)
					{
						String strTagName = e->tagName().LowerASCII();
						if (strTagName == "messagemap" && node->messageElem_ == nullptr)
						{
							node->messageElem_ = e;
						}
						else if (strTagName == "referencemap" && node->refElem_ == nullptr)
						{
							node->refElem_ = e;
							HTMLCollection* list2 = e->Children();
							unsigned int nlength = list2->length();
							if (nlength)
							{
								bool breferenced = false;
								for (unsigned int i = 0; i < nlength; i++)
								{
									Element* e = list2->item(i);
									Node* pNode = e;
									if (pNode->getNodeType() == 1)
									{
										CosmosNode* grid = getXobj(e, node);
										if (grid) {
											breferenced = true;
											AtomicString name = grid->hostElem_->getAttribute("name");
											e->setAttribute("handle", name);
										}
									}
								}
								if (breferenced)
								{
									node->setStr(L"msgID", L"SET_REFGRIDS_IPC_MSG");
									node->setStr(L"RefInfo", node->refElem_->OuterHTMLAsString());
									m_pRenderframeImpl->SendCosmosMessageEx(node->session_);
								}
							}
						}
						else if (strTagName == "eventmap" && node->eventElem_ == nullptr)
						{
							node->eventElem_ = e;
							HTMLCollection* list = e->Children();
							unsigned int nCount = list->length();
							if (nCount)
							{
								for (unsigned int i = 0; i < nCount; i++)
								{
									Element* elemEvent = list->item(i);
									Node* pNode = elemEvent;
									if (pNode->getNodeType() == 1)
									{
										String name = elemEvent->tagName().LowerASCII();
										String eventname = elemEvent->getAttribute("event");
										String strIndex = eventname.LowerASCII() + "@" + name;
										wstring key = WebString(strIndex).Utf16();
										auto it = node->m_mapElement.find(key);
										if (it == node->m_mapElement.end())
										{
											node->setStr(L"msgID", L"BIND_NATIVEOBJ_IPC_MSG");
											node->setStr(L"BindObj", name);
											node->setStr(L"Bindevent", eventname);
											node->m_mapElement[key] = elemEvent;
											m_pRenderframeImpl->SendCosmosMessageEx(node->session_);
											if (enableConsoleInfo_)
												GetFrame()->AddMessageToConsole(blink::mojom::ConsoleMessageLevel::kInfo, elemEvent->OuterHTMLAsString(), true);
										}
									}
								}
							}
						}
						if (node->messageElem_ && node->refElem_ && node->eventElem_)
							break;
					}
				}
			}

			node->setStr(L"msgID", L"GRIDOBJ_LOAD_IPC_MSG");
			m_pRenderframeImpl->SendCosmosMessageEx(node->session_);
		}
		if (bNewGalaxy)
		{
			pGalaxy = CosmosGalaxy::Create(strGalaxyname);
			pGalaxy->cosmos_ = this;
			pGalaxy->handle_ = nGalaxyHandle;
			m_mapCosmosGalaxy.insert(nGalaxyHandle, pGalaxy);
			WebString str = strGalaxyname;
			m_mapCosmosGalaxy2[str.Utf16()] = pGalaxy;
			str = strClustername;
			pGalaxy->m_mapRootNode[str.Utf16()] = node;
			DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kCosmosgalaxycreated, xobj));
		}

		if (pGalaxy)
		{
			strClustername = strClustername + L"__";
			strClustername = strClustername + strname;
			pGalaxy->m_mapCosmosNode[handle] = node;
			WebString str = strClustername;
			pGalaxy->m_mapCosmosNode2[str.Utf16()] = node;
			pGalaxy->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kXobjcreated, xobj));
		}

		if (parentform)
		{
			if (bNewGalaxy)
				parentform->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kCosmosgalaxycreated, xobj));

			if (parentmdiform && parentform)
			{
				CosmosNode* pNode = parentform->mdibindgrid();
				if (pNode == nullptr)
				{
					int64_t nHandle = xobj->getInt64(L"BindMdiXobjHandle");
					if (nHandle)
					{
						auto it1 = m_mapCosmosNode.find(nHandle);
						if (it1 != m_mapCosmosNode.end())
						{
							parentform->m_pBindMdiNode = it1->value.Get();
						}
					}

					nHandle = xobj->getInt64(L"mdiwebbindgridhandle");
					if (nHandle)
					{
						parentform->m_nMdiwebbindgridhandle = nHandle;
						auto it1 = m_mapCosmosNode.find(nHandle);
						if (it1 != m_mapCosmosNode.end())
						{
							parentform->m_pWebBindMdiNode = it1->value.Get();
						}
					}
				}
				parentform->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kXobjcreated, xobj));
				return node;
			}
			else if (parentmdiform)
			{
				parentmdiform->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kXobjcreated, xobj));
				return node;
			}
			else if (parentform)
			{
				parentform->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kXobjcreated, xobj));
				return node;
			}
		}
		DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kXobjcreated, xobj));

		return node;
	}

	CosmosNode* Cosmos::createCosmosWinform(CosmosXobj* xobj)
	{
		blink::CosmosWinform* form = nullptr;
		__int64 handle = xobj->getInt64(L"form");
		if (handle)
		{
			auto it = m_mapWinForm.find(handle);
			if (it != m_mapWinForm.end()) {
				form = it->value;
				handle = xobj->getInt64(L"formhandle");
				if (handle)
				{
					m_mapWinForm.erase(it);
					m_mapWinForm.insert(handle, form);
					form->handle_ = handle;
					form->InitWinForm();
				}
				if (form) {
					invokeWinFormCreatedCallback(form);
					DispatchEvent(*blink::CosmosEvent::Create(
						blink::event_type_names::kLoadwinform, xobj));
				}
			}
		}
		else
		{
			handle = xobj->getInt64(L"formhandle");
			form = CreateForm(handle, xobj);
		}
		return nullptr;
	}

	CosmosNode* Cosmos::getXobj(Element* elem, CosmosXobj* grid)
	{
		if (elem)
		{
			Node* pNode = elem;
			if (pNode->getNodeType() == 1) {
				AtomicString target = elem->getAttribute("target");
				AtomicString galaxy = elem->getAttribute("galaxy");
				AtomicString cluster = elem->getAttribute("cluster");
				if (galaxy == "" || galaxy.IsNull() == true)
					galaxy = "default";
				if (cluster == "" || cluster.IsNull() == true)
					cluster = "__viewport_default__";
				if (target.IsNull() == true || target == "")
				{
					auto it = m_mapCosmosGalaxy2.find(WebString(galaxy).Utf16());
					if (it != m_mapCosmosGalaxy2.end())
					{
						auto it2 = it->second->m_mapRootNode.find(WebString(cluster).Utf16());
						if (it2 != it->second->m_mapRootNode.end())
							return it2->second;
						return nullptr;
					}
				}
				else
				{
					CosmosNode* xobjfortarget = getXobj(galaxy, cluster, target);
					if (xobjfortarget == nullptr && grid)
					{
						CosmosWinform* form = grid->form();
						if (grid->grid())
						{
							form = grid->grid()->parentForm();
						}
						if (form)
						{
							xobjfortarget = form->getXobj(galaxy, cluster, target);
							if (xobjfortarget == nullptr && form->mdiParent())
							{
								xobjfortarget = form->mdiParent()->getXobj(galaxy, cluster, target);
							}
						}
					}
					if (!!xobjfortarget) {
						return xobjfortarget;
					}
				}
			}
		}
		return nullptr;
	}

	CosmosNode* Cosmos::getXobj(const int64_t nodeHandle)
	{
		auto it = m_mapCosmosNode.find(nodeHandle);
		if (it != m_mapCosmosNode.end())
			return it->value;
		return nullptr;
	}

	CosmosNode* Cosmos::getXobj(const String& galaxyName, const String& clusterName, const String& gridName)
	{
		String clusterName_ = clusterName;
		if (clusterName_ == "undefined" || clusterName_ == "" || clusterName_.IsNull() == true)
			clusterName_ = "default";
		auto it = m_mapCosmosGalaxy2.find(WebString(galaxyName).Utf16());
		if (it != m_mapCosmosGalaxy2.end())
		{
			if (gridName == "undefined" || gridName == "" || gridName.IsNull() == true)
			{
				auto it2 = it->second->m_mapRootNode.find(WebString(clusterName_).Utf16());
				if (it2 != it->second->m_mapRootNode.end())
					return it2->second;
				return nullptr;
			}

			clusterName_ = clusterName + "__" + gridName;
			auto it2 = it->second->m_mapCosmosNode2.find(WebString(clusterName_).Utf16());
			if (it2 != it->second->m_mapCosmosNode2.end())
				return it2->second;
		}
		return nullptr;
	}

	CosmosGalaxy* Cosmos::getGalaxy(const String& wndName)
	{
		WebString str = wndName;
		auto it = m_mapCosmosGalaxy2.find(str.Utf16());
		if (it != m_mapCosmosGalaxy2.end())
			return it->second;
		return nullptr;
	}

	CosmosGalaxy* Cosmos::getGalaxy(const int64_t wndHandle)
	{
		if (wndHandle)
		{
			auto it = m_mapCosmosGalaxy.find(wndHandle);
			if (it != m_mapCosmosGalaxy.end())
				return it->value;
		}
		return nullptr;
	}

	const AtomicString& Cosmos::InterfaceName() const {
		return event_target_names::kCosmos;
	}

	ExecutionContext* Cosmos::GetExecutionContext() const {
		return DomWindow()->document();
	}

	void Cosmos::Observe(const String& key, const String& strXml, V8ApplicationCallback* callback)
	{
		sendMessage("TANGRAM_UI_MESSAGE", key, L"", strXml, L"", L"");
	}

	void Cosmos::Observe(const String& key, Element* elem, V8ApplicationCallback* callback)
	{
		sendMessage("TANGRAM_UI_MESSAGE", key, L"", elem->OuterHTMLAsString(), L"", L"");
	}

	void Cosmos::ObserveClient(const String& key, const String& strXml, V8ApplicationCallback* callback)
	{
		sendMessage("Client_UI_MESSAGE", key, L"", strXml, L"", L"");
	}

	void Cosmos::ObserveClient(const String& key, Element* elem, V8ApplicationCallback* callback)
	{
		sendMessage("Client_UI_MESSAGE", key, L"", elem->OuterHTMLAsString(), L"", L"");
	}

	void Cosmos::invokeWinFormCreatedCallback(CosmosWinform* form)
	{
		auto itcallback = mapCallbackFunction_.find((int64_t)form);
		if (itcallback != mapCallbackFunction_.end())
		{
			blink::V8ApplicationCallback* callback = (blink::V8ApplicationCallback*)itcallback->value.Get();
			ScriptState* callback_relevant_script_state = callback->CallbackRelevantScriptState();
			ScriptState::Scope callback_relevant_context_scope(callback_relevant_script_state);
			callback->InvokeAndReportException(form, form);
			mapCallbackFunction_.erase(itcallback);
		}
	}

	void Cosmos::sendMessage(CosmosXobj* msg, V8ApplicationCallback* callback, bool bwait = false)
	{
		if (m_pRenderframeImpl)
		{
			if (msg == nullptr)
				msg = this;
			msg->setStr(L"senderid", getid());
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
		GetFrame()->AddMessageToConsole(blink::mojom::ConsoleMessageLevel::kInfo,"test",true);
		//((RenderFrame*)m_pRenderframeImpl)->AddMessageToConsole(blink::mojom::ConsoleMessageLevel::kInfo,
		//	"test");
		//if (bwait)
		//	run_loop_.Run();
	}

	void Cosmos::openUrl(const String& url, long nBrowserWndOpenDisposition, V8ApplicationCallback* callback, bool bwait = false)
	{
		if (m_pRenderframeImpl)
		{
			setStr(L"senderid", getid());
			setStr(L"msgID", L"OPEN_URL");
			setStr(L"openurl", url);
			setLong(L"BrowserWndOpenDisposition", nBrowserWndOpenDisposition);
			if (callback)
			{
				String callbackid_ = WTF::CreateCanonicalUUIDString();
				setStr(L"callbackid", callbackid_);
				mapCosmosEventCallback_.insert(callbackid_, callback);
				WebString strID = callbackid_;
				m_pRenderframeImpl->m_mapCosmosSession[strID.Utf16()] = this;
			}
			m_pRenderframeImpl->SendCosmosMessageEx(session_);
		}
		//if (bwait)
		//	run_loop_.Run();
	}

	CosmosXobj* Cosmos::getNamedItem(const AtomicString& name) const {
		auto it = mapCloudSession_.find(name);
		if (it != mapCloudSession_.end())
			return it->value.Get();
		return nullptr;
	}

	void Cosmos::setNamedItem(const String& name, CosmosXobj* var, ExceptionState&)
	{
		DCHECK(var);
		mapCloudSession_.insert(name, var);
	}

	CosmosXobj* Cosmos::setNamedItem(CosmosXobj* var, ExceptionState& exception_state) {
		DCHECK(var);
		mapCloudSession_.insert(var->name(), var);
		return var;
	}

	void Cosmos::NamedPropertyEnumerator(Vector<String>& names,
		ExceptionState&) const {
		for (auto it : mapCloudSession_) {
			names.UncheckedAppend(it.value->name());
		}
	}

	bool Cosmos::NamedPropertyQuery(const AtomicString& name,
		ExceptionState& exception_state) const {
		Vector<String> properties;
		NamedPropertyEnumerator(properties, exception_state);
		return properties.Contains(name);
	}

	void Cosmos::Close()
	{
		while (mapCloudSession_.size())
		{
			mapCloudSession_.erase(mapCloudSession_.begin());
		}
		while (m_mapCosmosNode.size())
		{
			m_mapCosmosNode.erase(m_mapCosmosNode.begin());
		}
		while (m_mapCosmosGalaxy.size())
		{
			m_mapCosmosGalaxy.erase(m_mapCosmosGalaxy.begin());
		}
		while (m_mapWinForm.size())
		{
			m_mapWinForm.erase(m_mapWinForm.begin());
		}
		while (mapCallbackFunction_.size())
		{
			mapCallbackFunction_.erase(mapCallbackFunction_.begin());
		}
	}

}  // namespace blink

/********************************************************************************
 *           Web Runtime for Application - Version 1.0.0.202012290004           *
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
#include "cosmos_winform.h"
#include "cosmos_control.h"
#include "cosmos_galaxy.h"
#include "cosmos_compositor.h"

#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/dom/document_fragment.h"
#include "third_party/blink/renderer/core/dom/node.h"
#include "third_party/blink/renderer/core/dom/node_list.h"
#include "third_party/blink/renderer/core/html/html_element.h"
#include "third_party/blink/renderer/core/html/html_head_element.h"
#include "third_party/blink/renderer/core/dom/class_collection.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/public/web/web_local_frame_client.h"
#include "third_party/blink/renderer/core/frame/web_local_frame_impl.h"
#include "third_party/blink/renderer/bindings/core/v8/v8_application_callback.h"
#include "third_party/blink/renderer/platform/wtf/uuid.h"
#include "third_party/blink/renderer/core/xml/dom_parser.h"
#include "third_party/blink/renderer/bindings/core/v8/string_or_trusted_html.h"

namespace blink {

	CosmosWinform::CosmosWinform() : CosmosXobj() {
		m_pBindMdiNode = nullptr;
		m_pWebBindMdiNode = nullptr;
		m_pRenderframeImpl = nullptr;
		m_pContentElement = nullptr;
		m_pMDIParent = nullptr;
		m_pActiveMDIChild = nullptr;
	}

	CosmosWinform::CosmosWinform(const String& strNodeName)
	{
		name_ = "";
	}

	CosmosWinform::~CosmosWinform() {
		Cosmos* pCosmos = cosmos_.Get();
		if (pCosmos)
		{
			auto it = pCosmos->m_mapCosmosWinformforEvent.find(handle_);
			if (it != pCosmos->m_mapCosmosWinformforEvent.end())
				pCosmos->m_mapCosmosWinformforEvent.erase(it);

			auto it2 = pCosmos->m_mapWinForm.find(handle_);
			if (it2 != pCosmos->m_mapWinForm.end())
				pCosmos->m_mapWinForm.erase(it2);
		}
		mapCosmosEventCallback_.clear();
	}

	CosmosWinform* CosmosWinform::Create(const String& strFormXml) {
		return MakeGarbageCollected<CosmosWinform>(strFormXml);
	}

	void CosmosWinform::Trace(blink::Visitor* visitor) {
		EventTargetWithInlineData::Trace(visitor);
		ScriptWrappable::Trace(visitor);
		CosmosXobj::Trace(visitor);
		visitor->Trace(cosmos_);
		visitor->Trace(m_pBindMdiNode);
		visitor->Trace(m_pMDIParent);
		visitor->Trace(m_pActiveMDIChild);
		visitor->Trace(m_pContentElement);
		visitor->Trace(m_pWebBindMdiNode);
	}

	CosmosNode* CosmosWinform::mdibindgrid()
	{
		if (m_pBindMdiNode)
			return m_pBindMdiNode;
		return nullptr;
	}

	CosmosNode* CosmosWinform::mdiwebbindgrid()
	{
		if (m_nMdiwebbindgridhandle)
		{
			if (m_pWebBindMdiNode == nullptr)
			{
				Cosmos* pCosmos = cosmos_.Get();
				auto it1 = pCosmos->m_mapCosmosNode.find(m_nMdiwebbindgridhandle);
				if (it1 != pCosmos->m_mapCosmosNode.end())
				{
					m_pWebBindMdiNode = it1->value.Get();
				}
			}
		}
		if (m_pWebBindMdiNode)
			return m_pWebBindMdiNode;
		return nullptr;
	}

	CosmosWinform* CosmosWinform::mdiParent()
	{
		if (m_pMDIParent == nullptr)
		{
			Cosmos* pCosmos = cosmos_.Get();
			int64_t nHandle = getInt64("mdiformhandle");
			if (nHandle && pCosmos)
			{
				pCosmos->m_mapWinForm.find(nHandle);
				auto it1 = pCosmos->m_mapWinForm.find(nHandle);
				if (it1 != pCosmos->m_mapWinForm.end())
				{
					m_pMDIParent = it1->value;
					return it1->value.Get();
				}
			}

		}
		return m_pMDIParent.Get();
	}

	CosmosGalaxy* CosmosWinform::getGalaxy(const String& galaxyName)
	{
		auto it = m_mapCosmosGalaxy.find(WebString(galaxyName).Utf16());
		if (it != m_mapCosmosGalaxy.end())
			return it->second;
		return nullptr;
	}

	CosmosNode* CosmosWinform::getXobj(const String& galaxyName, const String& clusterName, const String& gridName)
	{
		String clusterName_ = clusterName;
		if (clusterName == "undefined" || clusterName == "" || clusterName.IsNull() == true)
			clusterName_ = "default";
		String gridName_ = gridName;
		if (gridName == "undefined" || gridName == "" || gridName.IsNull() == true)
			gridName_ = "default";
		auto it = m_mapCosmosGalaxy.find(WebString(galaxyName).Utf16());
		if (it != m_mapCosmosGalaxy.end())
		{
			clusterName_ = clusterName_ + "__" + gridName_;
			auto it2 = it->second->m_mapCosmosNode2.find(WebString(clusterName_).Utf16());
			if (it2 != it->second->m_mapCosmosNode2.end())
				return it2->second;
		}
		return nullptr;
	}

	CosmosNode* CosmosWinform::getXobj(Element* elem)
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
					cluster = "default";
				if (target.IsNull() == true || target == "")
				{
					auto it = m_mapCosmosGalaxy.find(WebString(galaxy).Utf16());
					if (it != m_mapCosmosGalaxy.end())
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
					if (!!xobjfortarget) {
						return xobjfortarget;
					}
					else
					{
						return cosmos_->getXobj(elem, this);
					}
				}
			}
		}
		return nullptr;
	}

	long CosmosWinform::formType()
	{
		long nFormType = getLong(L"WinFormType");

		return nFormType;
	}

	bool CosmosWinform::isReady()
	{
		if (isReady_)
			return true;
		return false;
	}

	int64_t CosmosWinform::handle() {
		return getInt64(L"formhandle");
	}

	void CosmosWinform::DispatchXobjEvent(Element* e, const String& eventName)
	{
		Element* element = static_cast<Element*>(e->childNodes()->item(1));
		if (!!element) {
			for (unsigned int i = 1; i < element->childNodes()->length(); i++) {
				Element* elem = static_cast<Element*>(element->childNodes()->item(i));
				if (elem)
				{
					Node* pNode = elem;
					if (pNode->getNodeType() == 1) {
						AtomicString target = "";
						if (elem->hasAttribute("target"))
						{
							target = elem->getAttribute("target");
							if (target != "") {
								AtomicString galaxy = elem->getAttribute("galaxy");
								AtomicString cluster = elem->getAttribute("cluster");
								if (galaxy == "")
									galaxy = "default";
								if (cluster == "")
									cluster = "default";
								CosmosNode* xobjfortarget = getXobj(galaxy, cluster, target);
								if (xobjfortarget == nullptr)
									xobjfortarget = cosmos_.Get()->getXobj(galaxy, cluster, target);
								if (!!xobjfortarget) {
									xobjfortarget->setWorkElement(elem);
									String msgID = e->GetIdAttribute() + "_" + eventName;
									xobjfortarget->setMsgID(msgID);
									xobjfortarget->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kCloudmessageforxobj, xobjfortarget));
									xobjfortarget->setMsgID(msgID);
									xobjfortarget->setStr("eventdata", elem->OuterHTMLAsString());
									m_pRenderframeImpl->SendCosmosMessageEx(xobjfortarget->session_);
								}
							}
						}
					}
				}
			}
		}
	}

	void CosmosWinform::InitWinForm()
	{
		String strMessageXml = getStr(L"cosmosxml");
		if (strMessageXml.IsNull() == false && strMessageXml != "")
		{
			DocumentFragment_ = cosmos_->DomWindow()->document()->createDocumentFragment();
			if (DocumentFragment_)
			{
				ExceptionState exception_state(V8PerIsolateData::MainThreadIsolate(), ExceptionState::kSetterContext,
					"Element", "outerHTML");
				cosmos_->helperElem_->SetInnerHTMLFromString(strMessageXml, exception_state);
				DocumentFragment_->appendChild(cosmos_->helperElem_->firstChild());

				if (DocumentFragment_->Children()->length())
				{
					hostElem_ = DocumentFragment_->Children()->item(0);
					HTMLCollection* list_ = hostElem_->Children();
					for (unsigned int i = 0; i < list_->length(); i++)
					{
						Element* elem = list_->item(i);
						Node* pNode = elem;
						if (pNode->getNodeType() == 1)
						{
							String name = elem->tagName().LowerASCII();
							if (name == "messagemap" && messageElem_ == nullptr)
							{
								messageElem_ = elem;
							}
							else if (name == "referencemap" && refElem_ == nullptr)
							{
								refElem_ = elem;
								HTMLCollection* list2 = elem->Children();
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
											CosmosNode* grid = getXobj(e);
											if (grid) {
												breferenced = true;
												AtomicString name = grid->hostElem_->getAttribute("name");
												e->setAttribute("handle", name);
											}
										}
									}
									if (breferenced)
									{
										setStr(L"msgID", L"SET_REFGRIDS_IPC_MSG");
										setStr(L"RefInfo", refElem_->OuterHTMLAsString());
										m_pRenderframeImpl->SendCosmosMessageEx(session_);
									}
								}
							}
							else if (name == "eventmap" && eventElem_ == nullptr)
							{
								eventElem_ = elem;

								HTMLCollection* list = eventElem_->Children();
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
											auto it = m_mapElement.find(key);
											if (it == m_mapElement.end())
											{
												setStr(L"msgID", L"BIND_NATIVEOBJ_IPC_MSG");
												setStr(L"BindObj", name);
												setStr(L"Bindevent", eventname);
												m_mapElement[key] = elemEvent;
												m_pRenderframeImpl->SendCosmosMessageEx(session_);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		setStr(L"cosmosxml", L"");
	}

	CosmosWinform* CosmosWinform::activeMDIChild()
	{
		if (m_pActiveMDIChild)
			return m_pActiveMDIChild.Get();
		return nullptr;
	}

	void CosmosWinform::ProcessFormMessage(const String& msgID)
	{
		if (messageElem_ && msgID.IsNull() == false && msgID != "")
		{
			CosmosNode* xobjfortarget = nullptr;
			HTMLCollection* list = messageElem_->getElementsByTagName(AtomicString(msgID.LowerASCII()));
			for (unsigned int i = 0; i < list->length(); i++)
			{
				HTMLCollection* plist = list->item(i)->Children();
				for (unsigned int i = 0; i < plist->length(); i++)
				{
					Element* elem = plist->item(i);
					AtomicString target = "";
					target = elem->getAttribute("target");
					if (target.IsNull() || target == "")
					{
					}
					else {
						AtomicString galaxy = elem->getAttribute("galaxy");
						AtomicString cluster = elem->getAttribute("cluster");
						if (galaxy == "")
							galaxy = "default";
						if (cluster == "")
							cluster = "default";

						xobjfortarget = cosmos_->getXobj(galaxy, cluster, target);
						if (xobjfortarget == nullptr) {
							xobjfortarget = getXobj(galaxy, cluster, target);
						}
						if (xobjfortarget == nullptr) {
							CosmosWinform* Parentform = mdiParent();
							if (Parentform)
								xobjfortarget = Parentform->getXobj(galaxy, cluster, target);
						}
						if (xobjfortarget == nullptr) {
							long nFormType = getLong(L"WinFormType");
							if (nFormType == 2)
							{
								CosmosWinform* childform = activeMDIChild();
								if (childform)
									xobjfortarget = childform->getXobj(galaxy, cluster, target);
							}
						}
					}
					if (!!xobjfortarget) {
						xobjfortarget->setWorkElement(elem);
						xobjfortarget->setMsgID(msgID);
						xobjfortarget->setSender(this);
						xobjfortarget->DispatchEvent(*blink::CosmosEvent::Create(blink::event_type_names::kCloudmessageforxobj, xobjfortarget));
					}
				}
			}
		}
	}
}  // namespace blink

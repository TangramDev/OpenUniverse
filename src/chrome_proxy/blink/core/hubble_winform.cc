#include "hubble.h"
#include "hubble_node.h"
#include "hubble_event.h"
#include "hubble_winform.h"
#include "hubble_control.h"
#include "hubble_galaxy.h"
#include "hubble_compositor.h"

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

	HubbleWinform::HubbleWinform():HubbleXobj(){
		m_strFormXml = "";
		m_pBindMdiNode = nullptr;
		m_pWebBindMdiNode = nullptr;
		m_pRenderframeImpl = nullptr;
		m_pContentElement = nullptr;
		m_pMDIParent = nullptr;
		m_pActiveMDIChild = nullptr;
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

	HubbleWinform* HubbleWinform::Create(const String& strFormXml) {
		return MakeGarbageCollected<HubbleWinform>(strFormXml);
	}

	HubbleWinform::HubbleWinform(const String& strNodeName) 
	{
		m_strFormXml = strNodeName;
		name_ = strNodeName;
	}

	void HubbleWinform::Trace(blink::Visitor* visitor) {
		EventTargetWithInlineData::Trace(visitor);
		ScriptWrappable::Trace(visitor);
		HubbleXobj::Trace(visitor);
		visitor->Trace(hubble_);
		visitor->Trace(m_pBindMdiNode);
		visitor->Trace(m_pMDIParent);
		visitor->Trace(m_pActiveMDIChild);
		visitor->Trace(m_pContentElement);
		visitor->Trace(m_pWebBindMdiNode);
	}

	HubbleNode* HubbleWinform::mdibindgrid()
	{
		if (m_pBindMdiNode)
			return m_pBindMdiNode;
		return nullptr;
	}

	HubbleNode* HubbleWinform::mdiwebbindgrid()
	{
		if (m_nMdiwebbindgridhandle)
		{
			if (m_pWebBindMdiNode == nullptr)
			{
				Hubble* pHubble = hubble_.Get();
				auto it1 = pHubble->m_mapHubbleNode.find(m_nMdiwebbindgridhandle);
				if (it1 != pHubble->m_mapHubbleNode.end())
				{
					m_pWebBindMdiNode = it1->value.Get();
				}
			}
		}
		if (m_pWebBindMdiNode)
			return m_pWebBindMdiNode;
		return nullptr;
	}

	HubbleWinform* HubbleWinform::mdiParent()
	{
		if (m_pMDIParent == nullptr)
		{
			Hubble* pHubble = hubble_.Get();
			int64_t nHandle = getInt64("mdiformhandle");
			if (nHandle&&pHubble)
			{
				pHubble->m_mapWinForm.find(nHandle);
				auto it1 = pHubble->m_mapWinForm.find(nHandle);
				if (it1 != pHubble->m_mapWinForm.end())
				{
					m_pMDIParent = it1->value;
					return it1->value.Get();
				}
			}

		}
		return m_pMDIParent.Get();
	}

	String HubbleWinform::getid()
	{
		return id_;
	}

	HubbleGalaxy* HubbleWinform::getGalaxy(const String& galaxyName)
	{
		auto it = m_mapHubbleGalaxy.find(WebString(galaxyName).Utf16());
		if (it != m_mapHubbleGalaxy.end())
			return it->second;
		return nullptr;
	}

	HubbleNode* HubbleWinform::getGrid(const String& galaxyName, const String& clusterName, const String& gridName)
	{
		String clusterName_ = clusterName;
		if (clusterName == "undefined" || clusterName == "" || clusterName.IsNull() == true)
			clusterName_ = "default";
		String gridName_ = gridName;
		if (gridName == "undefined" || gridName == "" || gridName.IsNull() == true)
			gridName_ = "default";
		auto it = m_mapHubbleGalaxy.find(WebString(galaxyName).Utf16());
		if (it != m_mapHubbleGalaxy.end())
		{
			clusterName_ = clusterName_ + "__" + gridName_;
			auto it2 = it->second->m_mapHubbleNode2.find(WebString(clusterName_).Utf16());
			if (it2 != it->second->m_mapHubbleNode2.end())
				return it2->second;
		}
		return nullptr;
	}

	HubbleNode* HubbleWinform::getGrid(Element* elem)
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
					auto it = m_mapHubbleGalaxy.find(WebString(galaxy).Utf16());
					if (it != m_mapHubbleGalaxy.end())
					{
						auto it2 = it->second->m_mapRootNode.find(WebString(cluster).Utf16());
						if (it2 != it->second->m_mapRootNode.end())
							return it2->second;
						return nullptr;
					}
				}
				else
				{
					HubbleNode* gridfortarget = getGrid(galaxy, cluster, target);
					if (!!gridfortarget) {
						return gridfortarget;
					}
				}
			}
		}
		return nullptr;
	}

	long HubbleWinform::formType()
	{
		long nFormType = getLong(L"WinFormType");
		
		return nFormType;
	}

	bool HubbleWinform::isReady()
	{
		if (isReady_)
			return true;
		return false;
	}

	int64_t HubbleWinform::handle() {
		return getInt64(L"formhandle");
	}

	void HubbleWinform::DispatchGridEvent(Element* e, const String& eventName)
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
								HubbleNode* gridfortarget = getGrid(galaxy, cluster, target);
								if (gridfortarget == nullptr)
									gridfortarget = hubble_.Get()->getGrid(galaxy, cluster, target);
								if (!!gridfortarget) {
									gridfortarget->setWorkElement(elem);
									gridfortarget->setMsgID(e->GetIdAttribute() + "_" + eventName);
									gridfortarget->DispatchEvent(*blink::HubbleEvent::Create(blink::event_type_names::kCloudmessageforgrid, gridfortarget));
								}
							}
						}
					}
				}
			}
		}
	}

	void HubbleWinform::InitWinForm()
	{
		String strMessageXml = getStr(L"hubblexml");
		if (strMessageXml.IsNull() == false && strMessageXml != "")
		{
			DocumentFragment_ = hubble_->DomWindow()->document()->createDocumentFragment();
			if (DocumentFragment_)
			{
				ExceptionState exception_state(V8PerIsolateData::MainThreadIsolate(), ExceptionState::kSetterContext,
					"Element", "outerHTML");
				hubble_->helperElem_->SetInnerHTMLFromString(strMessageXml, exception_state);
				DocumentFragment_->appendChild(hubble_->helperElem_->firstChild());

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
							if (name == "messagemap")
							{
								messageElem_ = elem;
							}
							else
							{
								HTMLCollection* list = elem->getElementsByTagName("eventmap");
								for (unsigned int index = 0; index < list->length(); index++)
								{
									HTMLCollection* list2 = list->item(index)->Children();
									if (list2->length())
									{
										for (unsigned int i = 0; i < list2->length(); i++)
										{
											Element* e = list2->item(i);
											Node* pNode = e;
											Element* pPNode = (Element*)e->parentNode()->parentNode();
											if (pPNode && pPNode == elem && pNode->getNodeType() == 1)
											{
												String name = e->tagName().LowerASCII();
												String parentname = pPNode->tagName().LowerASCII();
												String strIndex = name + "@" + parentname;
												wstring key = WebString(strIndex).Utf16();
												auto it = m_mapElement.find(key);
												if (it == m_mapElement.end())
												{
													setStr(L"msgID", L"BIND_NATIVEOBJ_IPC_MSG");
													setStr(L"BindObj", parentname);
													setStr(L"Bindevent", name);
													m_mapElement[key] = e;
													m_pRenderframeImpl->SendHubbleMessageEx(session_);
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
		}
		setStr(L"hubblexml", L"");
	}

	HubbleWinform* HubbleWinform::activeMDIChild()
	{
		if(m_pActiveMDIChild)
			return m_pActiveMDIChild.Get();
		return nullptr;
	}

	void HubbleWinform::ProcessFormMessage(const String& msgID)
	{
		if (messageElem_ && msgID.IsNull() == false && msgID != "")
		{
			HubbleNode* gridfortarget = nullptr;
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

						gridfortarget = hubble_->getGrid(galaxy, cluster, target);
						if (gridfortarget == nullptr) {
							gridfortarget = getGrid(galaxy, cluster, target);
						}
						if (gridfortarget == nullptr) {
							HubbleWinform* Parentform = mdiParent();
							if(Parentform)
								gridfortarget = Parentform->getGrid(galaxy, cluster, target);
						}
						if (gridfortarget == nullptr) {
							long nFormType = getLong(L"WinFormType");
							if (nFormType == 2)
							{
								HubbleWinform* childform = activeMDIChild();
								if(childform)
									gridfortarget = childform->getGrid(galaxy, cluster, target);
							}
						}
					}
					if (!!gridfortarget) {
						gridfortarget->setWorkElement(elem);
						gridfortarget->setMsgID(msgID);
						gridfortarget->setSender(this);
						gridfortarget->DispatchEvent(*blink::HubbleEvent::Create(blink::event_type_names::kCloudmessageforgrid, gridfortarget));
					}
				}
			}
		}
	}
}  // namespace blink

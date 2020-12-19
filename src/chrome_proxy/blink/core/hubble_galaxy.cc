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

	HubbleGalaxy::HubbleGalaxy() {
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

	HubbleGalaxy* HubbleGalaxy::Create(const String& strWindowName) {
		return MakeGarbageCollected<HubbleGalaxy>(strWindowName);
	}

	HubbleGalaxy::HubbleGalaxy(const String& strWindowName)
	{
		name_ = strWindowName;
	}

	void HubbleGalaxy::Trace(blink::Visitor* visitor) {
		EventTargetWithInlineData::Trace(visitor);
		ScriptWrappable::Trace(visitor);
		HubbleXobj::Trace(visitor);
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

}  // namespace blink

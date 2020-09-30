#pragma once

#include <string>

using FrameMsg_TANGRAM_HOST_String_Map =
std::map<std::wstring, std::wstring>;

using FrameMsg_TANGRAM_HOST_LONG_Map =
std::map<std::wstring, long>;

using FrameMsg_TANGRAM_HOST_INT64_Map =
std::map<std::wstring, __int64>;

using FrameMsg_TANGRAM_HOST_FLOAT_Map =
std::map<std::wstring, float>;

namespace CommonUniverse {
	class IPCSession
	{
	public:
		IPCSession() {}
		virtual ~IPCSession() {}

		std::map<std::wstring, std::wstring> m_mapString;
		std::map<std::wstring, long> m_mapLong;
		std::map<std::wstring, float> m_mapFloat;
		std::map<std::wstring, __int64> m_mapint64;
	};

	class CChromeWebFrameClient {
	public:
		CChromeWebFrameClient() {}

		virtual ~CChromeWebFrameClient() {}

		std::map<__int64, __int64> m_mapHubbleNode;
		std::map<__int64, __int64> m_mapHubbleForm;
		std::map<__int64, __int64> m_mapHubbleWindow;
		std::map<__int64, __int64> m_mapHubbleControl;
		std::map<std::wstring, void*> m_mapHubbleSession;
		std::map<std::wstring, void*> m_mapHubbleNodeforCallback;
		std::map<std::wstring, void*> m_mapHubbleWindowforCallback;
		std::map<std::wstring, void*> m_mapHubbleFormforCallback;
		std::map<std::wstring, void*> m_mapHubbleControlforCallback;

		virtual void SendHubbleMessage(std::wstring strId,
			std::wstring strParam1,
			std::wstring strParam2,
			std::wstring strParam3,
			std::wstring strParam4,
			std::wstring strParam5) {}
			
		virtual void SendHubbleMessage(std::wstring strId,
			std::wstring strParam1,
			__int64 nHandle,
			__int64 nID,
			std::wstring strParam4,
			std::wstring strParam5) {}
			
		virtual void SendHubbleMessage(std::wstring strId,
			std::wstring strParam1,
			__int64 fromhandle,
			std::wstring strParam3,
			__int64 tohandle,
			std::wstring strParam5) {}

		virtual void SendHubbleMessageEx(IPCSession& var) {}

		virtual void SendHubbleMessage5(
			FrameMsg_TANGRAM_HOST_String_Map /* string map */,
			FrameMsg_TANGRAM_HOST_LONG_Map /* long map*/,
			FrameMsg_TANGRAM_HOST_INT64_Map /* int64 map*/,
			FrameMsg_TANGRAM_HOST_FLOAT_Map /* float ,ap */) {}

		virtual void OnHubbleMessage(long messageIndex,
			std::wstring strId,
			std::wstring strParam1,
			std::wstring strParam2,
			std::wstring strParam3,
			std::wstring strParam4,
			std::wstring strParam5) {}
	};

}  // namespace TangramCommon

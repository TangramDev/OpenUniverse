#pragma once

extern _ATL_FUNC_INFO NavigateComplete;
extern _ATL_FUNC_INFO Destroy;
extern _ATL_FUNC_INFO DocumentComplete;

class CStarEvents : public IDispEventSimpleImpl</*nID =*/ 1, CStarEvents, &__uuidof(_IStarEvents)>
{
public:
	CStarEvents(){};
	virtual ~CStarEvents(){};
	virtual void __stdcall  OnNavigateComplete(){}
	virtual void __stdcall  OnDestroy(){}
	virtual void __stdcall  OnDocumentComplete(IDispatch* pDocdisp, BSTR bstrUrl){}
	BEGIN_SINK_MAP(CStarEvents)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IStarEvents), /*dispid =*/ 0x00000001, OnNavigateComplete, &NavigateComplete)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IStarEvents), /*dispid =*/ 0x00000002, OnDestroy, &Destroy)
		SINK_ENTRY_INFO(/*nID =*/ 1, __uuidof(_IStarEvents), /*dispid =*/ 0x00000003, OnDocumentComplete, &DocumentComplete)
	END_SINK_MAP()
};
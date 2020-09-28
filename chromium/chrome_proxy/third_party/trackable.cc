#include "c:/src/tangram/source/chrome_proxy/third_party/trackable.h"
#include <wchar.h>

#define WM_TRACKABLEWINDOWCREATED (WM_USER + 0x00001000)
#define WM_TRACKABLEWINDOWMSG (WM_USER + 0x00001001)
#define WM_TRACKABLEWINDOWEDMSG (WM_USER + 0x00001002)
#define WM_TRACKABLEWINDOWCLIENTMSG (WM_USER + 0x00001003)
#define WM_TRACKABLEWINDOWCLIENTEDMSG (WM_USER + 0x00001004)

static LRESULT CALLBACK TrackableWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_TRACKABLEWINDOWMSG || uMsg == WM_TRACKABLEWINDOWCLIENTEDMSG)
    {
        LONG_PTR lpUserData = ::GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (lpUserData != NULL)
        {
            TrackableObjectMeta* pMeta = (TrackableObjectMeta*)lpUserData;
            TrackableChromiumObject* pObject = (TrackableChromiumObject*)pMeta->m_pObject;
            if (uMsg == WM_TRACKABLEWINDOWMSG)
            {
                pObject->TrackWindowMsgHandle(wParam, lParam);
            }
            else if (uMsg == WM_TRACKABLEWINDOWCLIENTEDMSG)
            {
                pObject->TrackWindowEndMsgHandle(wParam, lParam);
            }
        }
    }
    LRESULT lr = ::DefWindowProc(hwnd, uMsg, wParam, lParam);
    if (uMsg == WM_TRACKABLEWINDOWMSG)
    {
        ::PostMessage(hwnd, WM_TRACKABLEWINDOWEDMSG, wParam, lParam);
    }
    else if (uMsg == WM_TRACKABLEWINDOWCLIENTMSG)
    {
        ::PostMessage(hwnd, WM_TRACKABLEWINDOWCLIENTEDMSG, wParam, lParam);
    }
    return lr;
}

TrackableChromiumObject::TrackableChromiumObject()
{

}

TrackableChromiumObject::~TrackableChromiumObject()
{

}

void TrackableChromiumObject::StartTrack(TrackableChromiumObject* pThis, int nType)
{
    RegisterWindowClass();
    m_hTrackableWnd = ::CreateWindowEx(
        0, TRACKABLE_WINDOW_CLASS_NAME, L"",
        0, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);
    wchar_t lpszInfo[256];
    swprintf(lpszInfo, L"TANGRAM - INFO - A traceable window (0x%08x) has been created. (Source\\chrome_proxy\\third_party\\trackable.cc)\r\n", m_hTrackableWnd);
    ::OutputDebugString(lpszInfo);
    TrackableObjectMeta* pMeta = new TrackableObjectMeta;
    pMeta->m_nType = nType;
    pMeta->m_pObject = (void*)pThis;
    ::SetWindowLongPtr(m_hTrackableWnd, GWLP_USERDATA, (LONG_PTR)pMeta);
    // Post a message to the main thread.
    ::PostThreadMessage(::GetCurrentThreadId(), WM_TRACKABLEWINDOWCREATED, NULL, (LPARAM)m_hTrackableWnd);
}

void TrackableChromiumObject::EndTrack()
{
    if (::IsWindow(m_hTrackableWnd))
    {
        TrackableObjectMeta* pMeta = (TrackableObjectMeta*)::GetWindowLongPtr(m_hTrackableWnd, GWLP_USERDATA);
        delete pMeta;
        ::DestroyWindow(m_hTrackableWnd);
    }
    m_hTrackableWnd = 0;
}

void TrackableChromiumObject::PostMsgToTrackWindowClient(WPARAM wParam, LPARAM lParam)
{
    ::PostMessage(m_hTrackableWnd, WM_TRACKABLEWINDOWCLIENTMSG, wParam, lParam);
}

void TrackableChromiumObject::RegisterWindowClass()
{
    static bool isRegisterd;
    if (!isRegisterd)
    {
        isRegisterd = true;
        WNDCLASSEX wcx = {};
        wcx.cbSize = sizeof(WNDCLASSEX);
        wcx.lpfnWndProc = ::TrackableWindowProc;
        wcx.hInstance = ::GetModuleHandle(NULL);
        wcx.lpszClassName = TRACKABLE_WINDOW_CLASS_NAME;

        ::RegisterClassEx(&wcx);
    }
}

#ifndef TRACKABLE_CHROMIUM_OBJECT_MESSAGE_WINDOW
#define TRACKABLE_CHROMIUM_OBJECT_MESSAGE_WINDOW

#include <windows.h>

typedef struct
{
    int m_nType;
    void* m_pObject;
} TrackableObjectMeta;

static const wchar_t* TRACKABLE_WINDOW_CLASS_NAME = L"TrackableCrObjectMsgWnd";

class TrackableChromiumObject
{
public:
    HWND m_hTrackableWnd = 0;

    TrackableChromiumObject();
    virtual ~TrackableChromiumObject();

    void StartTrack(TrackableChromiumObject* pThis, int nType);
    void EndTrack();
    void PostMsgToTrackWindowClient(WPARAM wParam, LPARAM lParam);
    virtual void TrackWindowMsgHandle(WPARAM wParam, LPARAM lParam) {}
    virtual void TrackWindowEndMsgHandle(WPARAM wParam, LPARAM lParam) {}

private:
    void RegisterWindowClass();
};

#endif // TRACKABLE_CHROMIUM_OBJECT_MESSAGE_WINDOW
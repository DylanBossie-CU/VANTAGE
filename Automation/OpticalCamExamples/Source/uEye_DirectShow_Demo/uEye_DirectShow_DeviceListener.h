#if !defined(DS_DEVICELISTENER_H_)
#define DS_DEVICELISTENER_H_

#include <vector>

#include "uEye_DirectShow_Thread.h"

#define WM_DST_DEVICELIST_CHANGED (IS_UEYE_MESSAGE + 0x0100)

class DeviceListener
{
public:
    DeviceListener(CWnd* mainWindow);
    ~DeviceListener();

private:
    DWORD ThreadFunc();

    CWnd* m_hMainWindow;
    Thread<DeviceListener> m_thread;
    bool m_done;
    std::vector<HANDLE> m_eventHandles;
};

#endif /* DS_DEVICELISTENER_H_ */

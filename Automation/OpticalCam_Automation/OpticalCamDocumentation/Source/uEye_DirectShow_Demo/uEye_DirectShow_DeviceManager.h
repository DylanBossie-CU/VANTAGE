#if !defined(DS_DEVICEMANAGER_H_)
#define DS_DEVICEMANAGER_H_

#include <vector>

#include "uEye_DirectShow_Device.h"

class DeviceManager
{
public:
    static std::vector<Device> GetDeviceList();

private:
    DeviceManager();
    ~DeviceManager();
};

#endif /* DS_DEVICEMANAGER_H_*/

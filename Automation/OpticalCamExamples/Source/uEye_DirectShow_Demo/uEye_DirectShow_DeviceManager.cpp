#include "stdafx.h"

#include <iostream>

#include <dshow.h>

#include "uEye_DirectShow_DeviceManager.h"

DeviceManager::DeviceManager()
{
}

DeviceManager::~DeviceManager()
{
}

class ComInitializer
{
public:
    ComInitializer()
    {
        CoInitialize(NULL);
    }
    ~ComInitializer()
    {
        // CoUninitialize();
    }
};


std::vector<Device> DeviceManager::GetDeviceList()
{
    ComInitializer com;
    std::vector<Device> ret;

    HRESULT result = S_OK;

    // create system device enumerator
    CComPtr<ICreateDevEnum> pSystemDeviceEnumerator = NULL;
    result = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, 
        IID_ICreateDevEnum, (void**)&pSystemDeviceEnumerator);

    if(FAILED(result))
    {
        return ret;
    }

    // create video input device enumerator from system device enumerator
    CComPtr<IEnumMoniker> pVideoInputDeviceEnumerator = NULL;
    result = pSystemDeviceEnumerator->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, 
        &pVideoInputDeviceEnumerator, 0);

    if(FAILED(result) || pVideoInputDeviceEnumerator == NULL)
    {
        return ret;
    }

    // loop each connected device
    IMoniker* pMoniker = NULL;
    while(pVideoInputDeviceEnumerator->Next(1, &pMoniker, NULL) == S_OK)
    {
        if(pMoniker != NULL)
        {
            ret.push_back(Device(pMoniker));
        }
    }

    return ret;
}

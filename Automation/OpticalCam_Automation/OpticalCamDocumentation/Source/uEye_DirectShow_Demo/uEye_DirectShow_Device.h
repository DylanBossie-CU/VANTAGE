#if !defined(DS_DEVICE_H_)
#define DS_DEVICE_H_

#include <dshow.h>
#pragma warning(disable: 4995)
#include <string>
#pragma warning(default: 4995)

class Device
{
private:
    CComPtr<IMoniker> m_pMoniker;
    std::wstring m_name;

public:
    explicit Device(CComPtr<IMoniker> pMoniker);
    ~Device();

    CComPtr<IMoniker> GetHandle();
    std::wstring GetName();

};

#endif /* DS_DEVICE_H_ */

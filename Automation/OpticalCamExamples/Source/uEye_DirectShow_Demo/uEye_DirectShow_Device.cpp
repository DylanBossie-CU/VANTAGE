#include "stdafx.h"

#include "uEye_DirectShow_Device.h"

Device::Device(CComPtr<IMoniker> pMoniker)
    : m_pMoniker(pMoniker)
{
    if(pMoniker != NULL)
    {
        HRESULT result = S_OK;

        VARIANT var;
        VariantInit(&var);

        // create property bag for extended information
        CComPtr<IPropertyBag> pTempPropBag = NULL;
        result = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void**)&pTempPropBag);
        if(FAILED(result))
        {
            pTempPropBag = NULL;
        }
        else
        {
            CComPtr<IPropertyBag> pPropBag(pTempPropBag);

            // get the name of this filter
            result = pPropBag->Read(L"FriendlyName", &var, 0);
            if(!FAILED(result))
            {
                m_name = var.bstrVal;

                VariantClear(&var);
            }
        }
    }
}

Device::~Device()
{
}

CComPtr<IMoniker> Device::GetHandle()
{
    return m_pMoniker;
}

std::wstring Device::GetName()
{
    return m_name;
}

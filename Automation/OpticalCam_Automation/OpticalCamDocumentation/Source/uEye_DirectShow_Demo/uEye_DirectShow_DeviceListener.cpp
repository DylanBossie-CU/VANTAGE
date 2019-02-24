#include "stdafx.h"

#include <iostream>

#include "uEyeCaptureInterface.h"

#include "uEye_DirectShow_Device.h"
#include "uEye_DirectShow_DeviceManager.h"
#include "uEye_DirectShow_SteadyTimer.h"
#include "uEye_DirectShow_DeviceListener.h"

DeviceListener::DeviceListener(CWnd* mainWindow)
    : m_thread(this, &DeviceListener::ThreadFunc)
    , m_done(false)
    , m_hMainWindow(mainWindow)
{
    // create event handle
    HANDLE evConnect = CreateEvent(NULL, TRUE, FALSE, NULL);

    // initialize the event
    INT nRet = is_InitEvent(NULL, evConnect, IS_SET_EVENT_NEW_DEVICE);
    if(nRet != IS_SUCCESS)
    {
        std::cout << "Could not initialize device connect event." << std::endl;
    }
    else
    {
        // enable the event so it can be received
        nRet = is_EnableEvent(NULL, IS_SET_EVENT_NEW_DEVICE);
        if(nRet != IS_SUCCESS)
        {
            std::cout << "Could not enable device connect event." << std::endl;
        }
    }

    // create event handle
    HANDLE evDisconnect = CreateEvent(NULL, TRUE, FALSE, NULL);

    // initialize the event
    nRet = is_InitEvent(NULL, evConnect, IS_SET_EVENT_REMOVAL);
    if(nRet != IS_SUCCESS)
    {
        std::cout << "Could not initialize device removal event." << std::endl;
    }
    else
    {
        // enable the event so it can be received
        nRet = is_EnableEvent(NULL, IS_SET_EVENT_REMOVAL);
        if(nRet != IS_SUCCESS)
        {
            std::cout << "Could not enable device removal event." << std::endl;
        }
    }

    // add registered events to the list for listening
    m_eventHandles.push_back(evConnect);
    m_eventHandles.push_back(evDisconnect);

    // create the listening thread
    m_thread.start();
    
}

DeviceListener::~DeviceListener()
{
    INT nRet = IS_NO_SUCCESS;

    // unregister device connect event
    nRet = is_DisableEvent(NULL, IS_SET_EVENT_NEW_DEVICE);
    if(nRet != IS_SUCCESS)
    {
        std::cout << "Could not disable device connect event." << std::endl;
    }
    else
    {
        nRet = is_ExitEvent(NULL, IS_SET_EVENT_NEW_DEVICE);
        if(nRet != IS_SUCCESS)
        {
            std::cout << "Could not exit device connect event." << std::endl;
        }
    }

    // unregister device disconnect event
    nRet = is_DisableEvent(NULL, IS_SET_EVENT_REMOVAL);
    if(nRet != IS_SUCCESS)
    {
        std::cout << "Could not disable device removal event." << std::endl;
    }
    else
    {
        nRet = is_ExitEvent(NULL, IS_SET_EVENT_REMOVAL);
        if(nRet != IS_SUCCESS)
        {
            std::cout << "Could not exit device removal event." << std::endl;
        }
    }

    // close handles
    for(std::vector<HANDLE>::iterator it = m_eventHandles.begin(); it != m_eventHandles.end(); ++it)
    {
        HANDLE handle = *it;
        if(handle != NULL)
        {
            // free the handle
            CloseHandle(handle);
        }
    }

    m_eventHandles.clear();

    if(!m_done)
    {
        // stop the thread gracefully and wait for its termination
        m_done = true;
        m_thread.join();
    }
}

DWORD DeviceListener::ThreadFunc()
{
    // initialize COM framework for this thread
    CoInitialize(NULL);

    // get current device list
    std::vector<Device> deviceList = DeviceManager::GetDeviceList();

    while(!m_done)
    {
        // wait x milliseconds for one of the listed events to occur
        // Note: its important to have a timeout here, so the thread 
        // can check if it should stop or continue listening
        DWORD dwRet = WaitForMultipleObjects((DWORD)m_eventHandles.size(), &m_eventHandles[0], FALSE, 100);
        switch(dwRet)
        {
        case WAIT_OBJECT_0:
            {
                // timer for timeout for waiting until the list changed
                SteadyTimerS timer;
                bool deviceListChanged = false;

                // wait until the list has actually changed
                while(timer.GetElapsedTime() < 5 && !deviceListChanged) // wait a maximum of 5 seconds
                {
                    std::vector<Device> temp = DeviceManager::GetDeviceList();

                    // if size differs the list changed
                    if(temp.size() != deviceList.size())
                    {
                        deviceListChanged = true;
                        break;
                    }

                    // device count is equal, so check if a new device replaced the old one
                    for(std::vector<Device>::iterator temp_it = temp.begin(); temp_it != temp.end(); temp_it++)
                    {
                        bool found = false;
                        for(std::vector<Device>::iterator list_it = deviceList.begin(); list_it != 
                            deviceList.end(); list_it++)
                        {
                            std::wstring tempName = temp_it->GetName();
                            std::wstring listName = list_it->GetName();
                            if(tempName == listName)
                            {
                                found = true;
                                break;
                            }
                        }
                        if(!found)
                        {
                            deviceListChanged = true;
                            break;
                        }
                    }

                    // when device list has not changed sleep a bit
                    if(!deviceListChanged)
                    {
                        Sleep(50);
                    }
                }

                // when the devicelist has updated. update the list to compare to
                if(deviceListChanged)
                {
                    deviceList = DeviceManager::GetDeviceList();

                    // Send message to main window
                    PostMessage(m_hMainWindow->m_hWnd, WM_DST_DEVICELIST_CHANGED, NULL, NULL);

                    // loop through every event to reset it
                    for(std::vector<HANDLE>::iterator it = m_eventHandles.begin(); 
                        it != m_eventHandles.end(); ++it)
                    {
                        HANDLE handle = *it;
                        if(handle != NULL)
                        {
                            // reset the event
                            ResetEvent(handle);
                        }
                    }  
                }
                break;
            }
        case WAIT_TIMEOUT:
            {
                // continue if the function timed out
                continue;
            }
        default:
            // something else happened (nothing nessesary to do here)
            break;
        }
    }

    // deinitialize COM framework for this thread
    CoUninitialize();

    return 0;
}

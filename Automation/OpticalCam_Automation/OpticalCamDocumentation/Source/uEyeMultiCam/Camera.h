#pragma once

#include "uEye.h"
#include <vector>

//memorys used to store the different images
struct Memory
{
    Memory(void)
        : pcImageMemory(NULL)
        , lMemoryId(0)
        , lSequenceId(0)
    {
    }

    char* pcImageMemory;
    INT lMemoryId;
    INT lSequenceId;
};

struct Camera
{
    Camera(void)
        : hCam(0)
        , m_cnNumberOfSeqMemory(3)
        , RenderMode(IS_RENDER_FIT_TO_WINDOW)
        , nBitsPerPixel(32)
        , bLive(false)
        , bOpened(false)
    {
        m_vecMemory.reserve(m_cnNumberOfSeqMemory);
        m_vecMemory.resize(m_cnNumberOfSeqMemory);

        memset(&SensorInfo, 0, sizeof(SensorInfo));
        memset(&CamInfo, 0, sizeof(CamInfo));
    }

    INT AllocImageMems(INT nSizeX, INT nSizeY, INT nBitsPerPixel)
    {
        INT nRet = IS_SUCCESS;

        for (MemoryVector::iterator iter = m_vecMemory.begin(); iter != m_vecMemory.end(); ++iter)
        {
            nRet = is_AllocImageMem(hCam, nSizeX, nSizeY, nBitsPerPixel, &(iter->pcImageMemory), &(iter->lMemoryId));

            if (IS_SUCCESS != nRet)
            {
                FreeImageMems();
                break;
            }
        }

        return nRet;
    }

    INT FreeImageMems(void)
    {
        INT nRet = IS_SUCCESS;

        // Free the allocated buffer
        for (MemoryVector::iterator iter = m_vecMemory.begin(); iter != m_vecMemory.end(); ++iter)
        {
            if (NULL != iter->pcImageMemory)
            {
                nRet = is_FreeImageMem(hCam, iter->pcImageMemory, iter->lMemoryId);
            }

            iter->pcImageMemory = NULL;
            iter->lMemoryId = 0;
            iter->lSequenceId = 0;
        }

        return nRet;
    }

    INT InitSequence(void)
    {
        INT nRet = IS_SUCCESS;

        int i = 0;
        for (MemoryVector::iterator iter = m_vecMemory.begin(); iter != m_vecMemory.end(); ++iter, i++)
        {
            nRet = is_AddToSequence(hCam, iter->pcImageMemory, iter->lMemoryId);

            if (IS_SUCCESS != nRet)
            {
                ClearSequence();
                break;
            }

            iter->lSequenceId = i + 1;
        }

        return nRet;
    }

    INT ClearSequence(void)
    {
        return is_ClearSequence(hCam);
    }

    INT GetLastMem(char** ppLastMem, INT& lMemoryId, INT& lSequenceId)
    {
        INT nRet = IS_NO_SUCCESS;

        int dummy = 0;
        char *pLast = NULL;
        char *pMem = NULL;

        nRet = is_GetActSeqBuf(hCam, &dummy, &pMem, &pLast);

        if ((IS_SUCCESS == nRet) && (NULL != pLast))
        {
            nRet = IS_NO_SUCCESS;

            for (MemoryVector::iterator iter = m_vecMemory.begin(); iter != m_vecMemory.end(); ++iter)
            {
                if (pLast == iter->pcImageMemory)
                {
                    *ppLastMem = iter->pcImageMemory;
                    lMemoryId = iter->lMemoryId;
                    lSequenceId = iter->lSequenceId;
                    nRet = IS_SUCCESS;

                    break;
                }
            }
        }

        return nRet;
    }

    typedef std::vector<Memory> MemoryVector;
    MemoryVector m_vecMemory;
    const INT m_cnNumberOfSeqMemory;

	HIDS		hCam;
	int			RenderMode;
	SENSORINFO	SensorInfo;
	CAMINFO		CamInfo;
    int         nBitsPerPixel;
	bool		bLive;
    bool        bOpened;
};
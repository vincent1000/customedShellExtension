// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#include "dll.h"
#include "Logger.h"
#include <iostream>
#include <fstream>
#include <ctime>
#define LOGTAG "DLL"
typedef HRESULT (*PFNCREATEINSTANCE)(REFIID riid, void **ppvObject);
struct CLASS_OBJECT_INIT
{
    const CLSID *pClsid;
    PFNCREATEINSTANCE pfnCreate;
};

// add classes supported by this module here
const CLASS_OBJECT_INIT c_rgClassObjectInit[] =
{
    { &__uuidof(CExplorerCommandVerb),         CExplorerCommandVerb_CreateInstance },
    { &__uuidof(CExplorerCommandStateHandler), CExplorerCommandStateHandler_CreateInstance },
};


long g_cRefModule = 0;

// Handle the the DLL's module
HINSTANCE g_hInst = NULL;

// Standard DLL functions
STDAPI_(BOOL) DllMain(HINSTANCE hInstance, DWORD dwReason, void *)
{
    FileWrite(__FUNCTION__,__LINE__);
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        g_hInst = hInstance;
        DisableThreadLibraryCalls(hInstance);
    }
    FileWrite(__FUNCTION__,__LINE__);
    return TRUE;
}

STDAPI DllCanUnloadNow()
{
    FileWrite(__FUNCTION__,__LINE__);
    // Only allow the DLL to be unloaded after all outstanding references have been released
    return (g_cRefModule == 0) ? S_OK : S_FALSE;
}

void DllAddRef()
{
    FileWrite(__FUNCTION__,__LINE__);
    InterlockedIncrement(&g_cRefModule);
}

void DllRelease()
{
    FileWrite(__FUNCTION__,__LINE__);
    InterlockedDecrement(&g_cRefModule);
}

class CClassFactory : public IClassFactory
{
public:
    static HRESULT CreateInstance(REFCLSID clsid, const CLASS_OBJECT_INIT *pClassObjectInits, size_t cClassObjectInits, REFIID riid, void **ppv)
    {
        FileWrite(__FUNCTION__,__LINE__);
        *ppv = NULL;
        HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;
        for (size_t i = 0; i < cClassObjectInits; i++)
        {
            if (clsid == *pClassObjectInits[i].pClsid)
            {
                IClassFactory *pClassFactory = new (std::nothrow) CClassFactory(pClassObjectInits[i].pfnCreate);
                hr = pClassFactory ? S_OK : E_OUTOFMEMORY;
                if (SUCCEEDED(hr))
                {
                    hr = pClassFactory->QueryInterface(riid, ppv);
                    pClassFactory->Release();
                }
                break; // match found
            }
        }
        FileWrite(__FUNCTION__,__LINE__);
        return hr;
    }

    CClassFactory(PFNCREATEINSTANCE pfnCreate) : _cRef(1), _pfnCreate(pfnCreate)
    {
        FileWrite(__FUNCTION__,__LINE__);
        DllAddRef();
    }

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void ** ppv)
    {
        FileWrite(__FUNCTION__,__LINE__);
        static const QITAB qit[] =
        {
            QITABENT(CClassFactory, IClassFactory),
            { 0 }
        };
        return QISearch(this, qit, riid, ppv);
    }

    IFACEMETHODIMP_(ULONG) AddRef()
    {
        FileWrite(__FUNCTION__,__LINE__);
        return InterlockedIncrement(&_cRef);
    }

    IFACEMETHODIMP_(ULONG) Release()
    {
        FileWrite(__FUNCTION__,__LINE__);
        long cRef = InterlockedDecrement(&_cRef);
        if (cRef == 0)
        {
            delete this;
        }
        return cRef;
    }

    // IClassFactory
    IFACEMETHODIMP CreateInstance(IUnknown *punkOuter, REFIID riid, void **ppv)
    {
        FileWrite(__FUNCTION__,__LINE__);
        return punkOuter ? CLASS_E_NOAGGREGATION : _pfnCreate(riid, ppv);
    }

    IFACEMETHODIMP LockServer(BOOL fLock)
    {
        FileWrite(__FUNCTION__,__LINE__);
        if (fLock)
        {
            DllAddRef();
        }
        else
        {
            DllRelease();
        }
        FileWrite(__FUNCTION__,__LINE__);
        return S_OK;
    }

private:
    ~CClassFactory()
    {
        DllRelease();
    }

    long _cRef;
    PFNCREATEINSTANCE _pfnCreate;
};


STDAPI DllGetClassObject(REFCLSID clsid, REFIID riid, void **ppv)
{
    FileWrite(__FUNCTION__,__LINE__);
    return CClassFactory::CreateInstance(clsid, c_rgClassObjectInit, ARRAYSIZE(c_rgClassObjectInit), riid, ppv);
}

HRESULT RegisterUnregister(bool fRegister)
{
    FileWrite(__FUNCTION__,__LINE__);
    HRESULT hr = CExplorerCommandStateHandler_RegisterUnRegister(fRegister);
    if (SUCCEEDED(hr))
    {
        hr = CExplorerCommandVerb_RegisterUnRegister(fRegister);
    }
    FileWrite(__FUNCTION__,__LINE__);
    return hr;
}

STDAPI DllRegisterServer()
{
    FileWrite(__FUNCTION__,__LINE__);
    return RegisterUnregister(true);
}

STDAPI DllUnregisterServer()
{
    FileWrite(__FUNCTION__,__LINE__);
    return RegisterUnregister(false);
}

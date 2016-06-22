#pragma once

// minimal implementation of ICwSidetoneNotify
static const DISPID CwQueuedDISPID(101);
static const DISPID CwFinishedDISPID(102);

class MyNotifier : public IDispatch
{
public:
    MyNotifier() : m_RefCount(1)
    {    }

    HRESULT STDMETHODCALLTYPE QueryInterface(
        REFIID riid,
        void **ppvObject)
    {
        if (riid == IID_IUnknown)
            *ppvObject = this;
        else if (riid == IID_IDispatch)
            *ppvObject = this;
        else if (riid == __uuidof(ContestSuperSimDispLib::ICwSidetoneNotify))
            *ppvObject = this;
        else
            return E_NOINTERFACE;
        AddRef();
        return S_OK;
    }

    ULONG STDMETHODCALLTYPE AddRef(void)
    {        return ++m_RefCount;    }

    ULONG STDMETHODCALLTYPE Release(void)
    {        return --m_RefCount;    }

    HRESULT STDMETHODCALLTYPE GetTypeInfoCount( UINT *pctinfo)  {        return E_NOTIMPL;    }

    HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
    {        return E_NOTIMPL;    }

    HRESULT STDMETHODCALLTYPE GetIDsOfNames(
        REFIID riid, LPOLESTR *rgszNames, UINT cNames, ULONG lcid,
        DISPID *rgDispId)
    {
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE Invoke(
        DISPID dispIdMember,        REFIID riid,
        LCID lcid,        WORD wFlags,
        DISPPARAMS *pDispParams,        VARIANT *pVarResult,
        EXCEPINFO *pExcepInfo,        UINT *puArgErr)
    {
        if (dispIdMember == CwQueuedDISPID) // 
        {
            if (pDispParams->cArgs != 1)
                return E_INVALIDARG;
            if (pDispParams->rgvarg[0].vt != VT_I4)
                return E_INVALIDARG;
            int messageId = pDispParams->rgvarg[0].intVal;
            std::cout << "Nofication received that message " << messageId << " queued." << std::endl;
        }
        else if (dispIdMember == CwFinishedDISPID)
        {
            if (pDispParams->cArgs != 2)
                return E_INVALIDARG;
            if (pDispParams->rgvarg[0].vt != VT_BSTR)
                return E_INVALIDARG;
            if (pDispParams->rgvarg[1].vt != VT_I4)
                return E_INVALIDARG;
            // these are its arguments
            int messageId = pDispParams->rgvarg[1].intVal;
            std::wstring asSent(pDispParams->rgvarg[0].bstrVal);
            std::wcout << L"Nofication received that message " << messageId << L" finished: " << asSent << std::endl;
        }
        else 
            return E_INVALIDARG;
        return S_OK;
    }

    ULONG m_RefCount;
};

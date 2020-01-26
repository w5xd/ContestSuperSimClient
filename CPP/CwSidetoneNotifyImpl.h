#pragma once
#include <functional>
static const DISPID CwQueuedDISPID(1);
static const DISPID CwFinishedDISPID(2);

// minimal implementation of ICwSidetoneNotify
// The simulator calls this IDispatch implementation while its running CW sidetone
// Clients need to implement this to send CW only if they do NOT use the 
// simulator's WinKey over named pipes.
class MyNotifier : public IDispatch
{
public:
    MyNotifier(const std::function<void()> &doneFcn = [](){})
        : m_RefCount(0)
        , m_lastMessageStarted(0)
        , m_doneFcn(doneFcn)
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
    {    
        ULONG ret = --m_RefCount;   
        if (ret == 0)
            delete this;
        return ret;
    }

    HRESULT STDMETHODCALLTYPE GetTypeInfoCount( UINT *pctinfo)  {
        return E_NOTIMPL;    }

    HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
    {        return E_NOTIMPL;    }

    HRESULT STDMETHODCALLTYPE GetIDsOfNames(
        REFIID , LPOLESTR *rgszNames, UINT cNames, ULONG lcid,
        DISPID *rgDispId)
    {
        if (_wcsicmp(rgszNames[0], L"CwQueued") == 0)
            rgDispId[0] = CwQueuedDISPID;
        else  if (_wcsicmp(rgszNames[0], L"CwFinished") == 0)
            rgDispId[0] = CwFinishedDISPID;
        else
            return DISP_E_UNKNOWNNAME;
        return S_OK;;
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
            m_lastMessageStarted = messageId;
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
            if ((messageId >= m_lastMessageStarted) && m_doneFcn)
                m_doneFcn();
        }
        else 
            return E_INVALIDARG;
        return S_OK;
    }

    ULONG m_RefCount;
    int m_lastMessageStarted;
    std::function<void()> m_doneFcn;
};

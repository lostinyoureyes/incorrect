/////////////////////////////////////////////////////////////////////////////
//
// CWmpplugin1PropPage.h : Declaration of CWmpplugin1PropPage
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __CWMPPLUGIN1PROPPAGE_H_
#define __CWMPPLUGIN1PROPPAGE_H_

#include "resource.h"
#include "wmpplugin1.h"

// {3F78ABEB-CBAE-4D20-9F00-939C4B73AF97}
DEFINE_GUID(CLSID_Wmpplugin1PropPage, 0x3f78abeb, 0xcbae, 0x4d20, 0x9f, 0, 0x93, 0x9c, 0x4b, 0x73, 0xaf, 0x97);

/////////////////////////////////////////////////////////////////////////////
// CWmpplugin1PropPage
class ATL_NO_VTABLE CWmpplugin1PropPage :
    public CComObjectRootEx<CComMultiThreadModel>,
    public CComCoClass<CWmpplugin1PropPage, &CLSID_Wmpplugin1PropPage>,
    public IPropertyPageImpl<CWmpplugin1PropPage>,
    public CDialogImpl<CWmpplugin1PropPage>
{
public:
            CWmpplugin1PropPage(); 
    virtual ~CWmpplugin1PropPage(); 
    

    enum {IDD = IDD_WMPPLUGIN1PROPPAGE};
    

DECLARE_REGISTRY_RESOURCEID(IDR_WMPPLUGIN1PROPPAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CWmpplugin1PropPage) 
    COM_INTERFACE_ENTRY(IPropertyPage)
END_COM_MAP()

BEGIN_MSG_MAP(CWmpplugin1PropPage)
    CHAIN_MSG_MAP(IPropertyPageImpl<CWmpplugin1PropPage>)
    COMMAND_HANDLER(IDC_SCALEFACTOR, EN_CHANGE, OnChangeScale)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
END_MSG_MAP()

    STDMETHOD(SetObjects)(ULONG nObjects, IUnknown** ppUnk);
    STDMETHOD(Apply)(void);

    LRESULT (OnChangeScale)(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
    LRESULT (OnInitDialog)(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
    CComPtr<IWmpplugin1> m_spWmpplugin1;  // pointer to plug-in interface
};

#endif // __CWMPPLUGIN1PROPPAGE_H_

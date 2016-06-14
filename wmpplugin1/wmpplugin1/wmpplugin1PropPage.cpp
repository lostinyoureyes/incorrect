/////////////////////////////////////////////////////////////////////////////
//
// CWmpplugin1PropPage.cpp : Implementation of the property page for CWmpplugin1
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <stdio.h>
#include "wmpplugin1.h"
#include "Wmpplugin1PropPage.h"

/////////////////////////////////////////////////////////////////////////////
// CWmpplugin1Prop::CWmpplugin1Prop
// Constructor

CWmpplugin1PropPage::CWmpplugin1PropPage()
{
    m_dwTitleID = IDS_TITLEPROPPAGE;
    m_dwHelpFileID = IDS_HELPFILEPROPPAGE;
    m_dwDocStringID = IDS_DOCSTRINGPROPPAGE;
}

/////////////////////////////////////////////////////////////////////////////
// CWmpplugin1Prop::~CWmpplugin1Prop
// Destructor

CWmpplugin1PropPage::~CWmpplugin1PropPage()
{
}

/////////////////////////////////////////////////////////////////////////////
// CWmpplugin1Prop::SetObjects
//

STDMETHODIMP CWmpplugin1PropPage::SetObjects(ULONG nObjects, IUnknown** ppUnk)
{
    // find our plug-in object, if it was passed in
    for (DWORD i = 0; i < nObjects; i++)
    {
        CComPtr<IWmpplugin1> pPlugin;

        IUnknown    *pUnknown = ppUnk[i];
        if (pUnknown)
        {
            HRESULT hr = pUnknown->QueryInterface(__uuidof(IWmpplugin1), (void**)&pPlugin); // Get a pointer to the plug-in.
            if ((SUCCEEDED(hr)) && (pPlugin))
            {
                // save plug-in interface
                m_spWmpplugin1 = pPlugin;
                break;
            }
        }
    }

    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CWmpplugin1Prop::Apply
//

STDMETHODIMP CWmpplugin1PropPage::Apply(void)
{
    WCHAR   wszStr[MAXSTRING] = { 0 };
    double  fScaleFactor = 1.0;

    GetDlgItemText(IDC_SCALEFACTOR, wszStr, sizeof(wszStr) / sizeof(wszStr[0]));
    swscanf_s(wszStr, L"%lf", &fScaleFactor);    

    // make sure scale factor is valid
    if ((fScaleFactor < 0.0) ||
        (fScaleFactor > 1.0))
    {
        if (::LoadString(_Module.GetResourceInstance(), IDS_SCALERANGEERROR, wszStr, sizeof(wszStr) / sizeof(wszStr[0])))
        {
            MessageBox(wszStr);
        }

        return E_FAIL;
    }

    // update the registry
    CRegKey key;
    LONG    lResult;

    lResult = key.Create(HKEY_CURRENT_USER, kwszPrefsRegKey);
    if (ERROR_SUCCESS == lResult)
    {
        DWORD dwValue = (DWORD) (fScaleFactor * 65536);
        lResult = key.SetValue(kwszPrefsScaleFactor, REG_DWORD, &dwValue, sizeof(dwValue));
    }

    // update the plug-in
    if (m_spWmpplugin1)
    {
        m_spWmpplugin1->put_scale(fScaleFactor);
    }   

    m_bDirty = FALSE; // Tell the property page to disable Apply.
    
    return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CWmpplugin1Prop::OnChangeScale
//

LRESULT CWmpplugin1PropPage::OnChangeScale(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled){

    SetDirty(TRUE); // Enable Apply.
    
    return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CWmpplugin1Prop::OnInitDialog
//

LRESULT CWmpplugin1PropPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    double  fScaleFactor = 1.0;

    // read scale factor from plug-in if it is available
    if (m_spWmpplugin1)
    {
        m_spWmpplugin1->get_scale(&fScaleFactor);
    }   
    else // otherwise read scale factor from registry
    {
        CRegKey key;
        LONG    lResult;

        lResult = key.Open(HKEY_CURRENT_USER, kwszPrefsRegKey, KEY_READ);
        if (ERROR_SUCCESS == lResult)
        {
            DWORD   dwValue = 0;
            DWORD dwType = 0;
            ULONG uLength = sizeof(dwValue);
            lResult = key.QueryValue(kwszPrefsScaleFactor, &dwType, &dwValue, &uLength);

            if (ERROR_SUCCESS == lResult)
            {
                fScaleFactor = dwValue / 65536.0;
            }
        }
    }

    WCHAR   wszStr[MAXSTRING];

    swprintf_s(wszStr, L"%0.2f", fScaleFactor);  
    SetDlgItemText(IDC_SCALEFACTOR, wszStr);

    return 0;
}


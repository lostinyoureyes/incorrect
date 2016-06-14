/////////////////////////////////////////////////////////////////////////////
//
// wmpplugin1.h : Declaration of CWmpplugin1
//
// Note: Requires DirectX 8 SDK or later.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
/////////////////////////////////////////////////////////////////////////////
  
#ifndef __CWMPPLUGIN1_H_
#define __CWMPPLUGIN1_H_

#include "resource.h"
#include <mediaobj.h>       // The IMediaObject header from the DirectX SDK.
#define MFT_UNIQUE_METHOD_NAMES // Makes IMFTransform method names unique so that they can be
                                // implemented on the same interface as IMediaObject.
#include <mftransform.h>    // The IMFTransform header from Windows SDK.
#include <mfidl.h>          // For IMFGetService definition.
#include "wmpservices.h"    // The header containing the WMP interface definitions.
#include "./wmpplugin1PS/wmpplugin1_h.h"  // Generated from the IDL file during proxy/stub compilation.

const DWORD UNITS = 10000000;  // 1 sec = 1 * UNITS
const DWORD MAXSTRING = 1024;

// registry location for preferences
const WCHAR kwszPrefsRegKey[] = L"Software\\wmpplugin1\\DSP Plugin";
const WCHAR kwszPrefsScaleFactor[] = L"ScaleFactor";

// {51601248-1647-4BA3-B952-3D46B5C9DE50}
DEFINE_GUID(CLSID_Wmpplugin1, 0x51601248, 0x1647, 0x4ba3, 0xb9, 0x52, 0x3d, 0x46, 0xb5, 0xc9, 0xde, 0x50);

/////////////////////////////////////////////////////////////////////////////
// CWmpplugin1
/////////////////////////////////////////////////////////////////////////////

class ATL_NO_VTABLE CWmpplugin1 : 
    public CComObjectRootEx<CComMultiThreadModel>,
    public CComCoClass<CWmpplugin1, &CLSID_Wmpplugin1>,
    public IMediaObject,
    public IWMPPluginEnable,
    public ISpecifyPropertyPages,
    public IPropertyBag,
    public IWmpplugin1
{
public:
    CWmpplugin1();
    virtual ~CWmpplugin1();

DECLARE_REGISTRY_RESOURCEID(IDR_WMPPLUGIN1)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CWmpplugin1)
    COM_INTERFACE_ENTRY(IWmpplugin1)
    COM_INTERFACE_ENTRY(IMediaObject)
    COM_INTERFACE_ENTRY(IWMPPluginEnable)
    COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
    COM_INTERFACE_ENTRY(IPropertyBag)
END_COM_MAP()

    // CComCoClass Overrides
    HRESULT FinalConstruct();
    void    FinalRelease();

    // IWmpplugin1 methods
    STDMETHOD(get_scale)(double *pVal);
    STDMETHOD(put_scale)(double newVal);
    STDMETHOD(LoadResourceImage)(IStream **ppStream);

    // IMediaObject methods
    STDMETHOD( GetStreamCount )( 
                   DWORD *pcInputStreams,
                   DWORD *pcOutputStreams
                   );
    
    STDMETHOD( GetInputStreamInfo )( 
                   DWORD dwInputStreamIndex,
                   DWORD *pdwFlags
                   );
    
    STDMETHOD( GetOutputStreamInfo )( 
                   DWORD dwOutputStreamIndex,
                   DWORD *pdwFlags
                   );
    
    STDMETHOD( GetInputType )( 
                   DWORD dwInputStreamIndex,
                   DWORD dwTypeIndex,
                   DMO_MEDIA_TYPE *pmt
                   );
    
    STDMETHOD( GetOutputType )( 
                   DWORD dwOutputStreamIndex,
                   DWORD dwTypeIndex,
                   DMO_MEDIA_TYPE *pmt
                   );
    
    STDMETHOD( SetInputType )( 
                   DWORD dwInputStreamIndex,
                   const DMO_MEDIA_TYPE *pmt,
                   DWORD dwFlags
                   );
    
    STDMETHOD( SetOutputType )( 
                   DWORD dwOutputStreamIndex,
                   const DMO_MEDIA_TYPE *pmt,
                   DWORD dwFlags
                   );
    
    STDMETHOD( GetInputCurrentType )( 
                   DWORD dwInputStreamIndex,
                   DMO_MEDIA_TYPE *pmt
                   );
    
    STDMETHOD( GetOutputCurrentType )( 
                   DWORD dwOutputStreamIndex,
                   DMO_MEDIA_TYPE *pmt
                   );
    
    STDMETHOD( GetInputSizeInfo )( 
                   DWORD dwInputStreamIndex,
                   DWORD *pcbSize,
                   DWORD *pcbMaxLookahead,
                   DWORD *pcbAlignment
                   );
    
    STDMETHOD( GetOutputSizeInfo )( 
                   DWORD dwOutputStreamIndex,
                   DWORD *pcbSize,
                   DWORD *pcbAlignment
                   );
    
    STDMETHOD( GetInputMaxLatency )( 
                   DWORD dwInputStreamIndex,
                   REFERENCE_TIME *prtMaxLatency
                   );
    
    STDMETHOD( SetInputMaxLatency )( 
                   DWORD dwInputStreamIndex,
                   REFERENCE_TIME rtMaxLatency
                   );
    
    STDMETHOD( Flush )( void );
    
    STDMETHOD( Discontinuity )( 
                   DWORD dwInputStreamIndex
                   );
    
    STDMETHOD( AllocateStreamingResources )( void );
    
    STDMETHOD( FreeStreamingResources )( void );
    
    STDMETHOD( GetInputStatus )( 
                   DWORD dwInputStreamIndex,
                   DWORD *pdwFlags
                   );
    
    STDMETHOD( ProcessInput )( 
                   DWORD dwInputStreamIndex,
                   IMediaBuffer *pBuffer,
                   DWORD dwFlags,
                   REFERENCE_TIME rtTimestamp,
                   REFERENCE_TIME rtTimelength
                   );
    
    STDMETHOD( ProcessOutput )( 
                   DWORD dwFlags,
                   DWORD cOutputBufferCount,
                   DMO_OUTPUT_DATA_BUFFER *pOutputBuffers,
                   DWORD *pdwStatus
                   );

    STDMETHOD( Lock )( LONG bLock );

    // Note: need to override CComObjectRootEx::Lock to avoid
    // ambiguity with IMediaObject::Lock. The override just
    // calls through to the base class implementation.

    // CComObjectRootEx overrides
    void Lock()
    {
        CComObjectRootEx<CComMultiThreadModel>::Lock();
    }


    // IWMPPluginEnable methods
    STDMETHOD( SetEnable )( BOOL fEnable );
    STDMETHOD( GetEnable )( BOOL *pfEnable );


    // ISpecifyPropertyPages methods
    STDMETHOD( GetPages )(CAUUID *pPages);

    // IPropertyBag methods
    STDMETHOD ( Read )(
                    LPCWSTR pwszPropName,
                    VARIANT *pVar,
                    IErrorLog *pErrorLog
                    );

    STDMETHOD ( Write )(
                    LPCWSTR pwszPropName,
                    VARIANT *pVar
                    );

    static const GUID* k_guidValidSubtypes[];
    static const DWORD k_dwValidSubtypesCount;
    
private:
    HRESULT DoProcessOutput(
                BYTE *pbOutputData,             // Pointer to the output buffer
                const BYTE *pbInputData,        // Pointer to the input buffer
                DWORD *cbBytesProcessed);       // Number of bytes processed
    HRESULT ValidateMediaType(
                const DMO_MEDIA_TYPE *pmtTarget,    // target media type to verify
                const DMO_MEDIA_TYPE *pmtPartner);  // partner media type to verify
    DWORD GetSampleSize( const DMO_MEDIA_TYPE* pmt ); // Get the sample size from the media type.
    HRESULT GetVideoInfoParameters(             // Get the important information out of a VIDEOINFOHEADER
                BYTE  * const pbData,        
                DWORD *pdwWidth,         
                DWORD *pdwHeight,        
                LONG  *plStrideInBytes,  
                BYTE **ppbTop,           
                bool bYuv);
    HRESULT GetBitmapInfoHeader(
                const DMO_MEDIA_TYPE *pmt,
                BITMAPINFOHEADER *pbmih);
    HRESULT GetTargetRECT(
                const DMO_MEDIA_TYPE *pmt,
                RECT *prcTarget);
    HRESULT ProcessYUY2(
                BYTE *pbIputData, 
                BYTE *pbOutputData);
    HRESULT ProcessYV12(
                BYTE *pbIputData, 
                BYTE *pbOutputData);
    HRESULT ProcessNV12(
                BYTE *pbInputData,
                BYTE *pbOutputData);
    HRESULT ProcessUYVY(
                BYTE *pbIputData, 
                BYTE *pbOutputData);
    HRESULT Process24Bit(
                BYTE *pbIputData, 
                BYTE *pbOutputData);
    HRESULT Process32Bit(
                BYTE *pbIputData, 
                BYTE *pbOutputData);
    HRESULT Process565(
                BYTE *pbIputData, 
                BYTE *pbOutputData);
    HRESULT Process555(
                BYTE *pbIputData, 
                BYTE *pbOutputData);


    DMO_MEDIA_TYPE          m_mtInput;          // Stores the input format structure
    DMO_MEDIA_TYPE          m_mtOutput;         // Stores the output format structure

    CComPtr<IMediaBuffer>   m_spInputBuffer;    // Smart pointer to the input buffer object

   
    DWORD                   m_dwBufferSize;

    bool                    m_bValidTime;       // Is timestamp valid?
    bool                    m_bValidLength;
    REFERENCE_TIME          m_rtTimestamp;      // Stores the input buffer timestamp
    REFERENCE_TIME          m_rtTimelength;     // Stores the input buffer timelength.

    double                  m_fScaleFactor;     // Scale factor
    BOOL                    m_bEnabled;         // TRUE if enabled
};

#endif //__CWMPPLUGIN1_H_

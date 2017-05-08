// CAVICAP.H
// (c) Vadim Gorbatenko, 1996-99
// CAviCap window
// Started: 11.09.96
//______________________________________________________________________________

//Options:  on connection action:
//BB #define	ON_CONNECT_TEST_BIBITSCOUNT		// Testing for available bits resolution
//BB #define	ON_CONNECT_CHECK_DRIVERLIST		// Create full list of installed drivers
//BB #define	ON_CONNECT_CHECK_VALIDFORMATS	// Create full list of available formats
//#define	CAPTURE_PRIORITY_CONTROL	
#define DEFAULT_FORCE_SINGLE_FRAME		// Skip cached frames for GrabOne
//#define	DEFAULT_USED_DOSMEMORY		// std setting for driver
//#define	DEFAULT_STEP_CAPTUREAT2X	// get average frame
#define	AVICAP_WINDOW_ID	0xffff

#if !defined(_INC_VFW)
    #define	NODRAWDIB
    #define	NOAVIFMT
    #define	NOMMREG
    #define	NOAVIFILE
    #define	NOMCIWND
    #define	NOMSACM
    #include "vfw.h"
#endif

typedef VOID (CALLBACK *PROGRESSCALLBACK)(LPSTR , DWORD);
typedef VOID (CALLBACK *DRAWCALLBACK)(BOOL);
typedef LRESULT (CALLBACK *FRAMECALLBACK)(HWND , LPVIDEOHDR);

#define	ANY_DRIVER				-1	// default driver(for connection only)
#define	MAXCAPDRIVERS			10	// maximum drivers list size
#define DEFAULT_PREVIEW_RATE	50	// default preview rate = 50 msec
#define	MAX_VALID_BUFFERS_COUNT 10	// maximum internal buffer for Vxd(testing)
#define MIN_FRAME_PERIOD		20	// msec -> 50 frames per second (can't do faster!)

//Image Bits resolution
#define	BITS01	0x0001		// B&W Mono
#define	BITS04	0x0002		// 16  Colors
#define	BITS08	0x0004		// 256 Colors
#define	BITS16	0x0008		// 16 bits RGB
#define	BITS24	0x0010		// 24 bits RGB
#define	BITS32	0x0020		// 32 bits RGB

//struct for gathering capture driver information
typedef struct
{
	CHAR	psName[128];				  //name of driver
	DWORD	dwInternalBufCnt;			 //driver cache size
	DWORD	dwCurBiBitCount;			//current resolution
	DWORD	dwBiBitsCountSupported; //see abow for flags
    BOOL	fNTSC;			//this is NTSC videostandard
	DWORD	dwCompressed;		//is compressed
	SIZE	szFrameSize;		//Current frame size
	BOOL	fFormatDlgSup;	//"Format"  dialog supported
	BOOL	fSourceDlgSup;	//"Source"  dialog supported
	BOOL	fDisplayDlgSup;	//"Display" dialog supported
} CAPDRVINFO;

class CAviCap: public CWnd
{
public:
	enum { visible = WS_CHILD|WS_VISIBLE,hidden=WS_CHILD};

	CAviCap();
	~CAviCap();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAviCap)
	public:
	virtual BOOL	Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, BOOL fAutoSize=TRUE);
	//}}AFX_VIRTUAL

	//
	// Connection & disconnection
	//
	BOOL	ConnectWithDriver(LPCSTR Name=NULL);//connect by name
	BOOL	ConnectWithDriver(int indx);		//connect by index
	BOOL	Disconnect();

	//
	// Standard AVICap window COMMANDs.
	// Implemeted via cap[xxx] macros	
	// See SDK for more information
	//
	BOOL	StartPreview(BOOL offOn);
	BOOL	GetPreviewFlag();
	BOOL	StartOverlay(BOOL offOn);
	BOOL	StartSeq(BOOL offOn);
	BOOL	SavePalette(LPCSTR file);
	BOOL	LoadPalette(LPCSTR file);
	BOOL	PastePalette();
	BOOL	CreatePalette(int frames,int colors);
	BOOL	CopyFrame();			 // copy image to clipboard
	BOOL	SetPreviewRate(int Rate);// Rate == xx msec
	BOOL	SetCapSetup(CAPTUREPARMS *parms);
	BOOL	GetCapStatus(CAPSTATUS *status);
	BOOL	GetCapSetup(CAPTUREPARMS *parms);
			//Standard AVICap window Dialogs
	BOOL	CompressDlg();
	BOOL	DisplayDlg();
	BOOL	FormatDlg();
	BOOL	VSourceDlg();
			//Setup standard AVICap window callbacks
	BOOL	SetStreamCallBack(LPVOID cb);
	BOOL	SetYiedCallBack(LPVOID cb);
	BOOL	SetErrorCallBack(LPVOID cb);
	// 
	// Enhanced capturing control
	//
	BOOL	SetFrameCallBack(FRAMECALLBACK cb);
	void	ResetCache();			//Reset internal driver buffers queue(if exist)
	BOOL	SetFormat(LPBITMAPINFO bmpinf);
	BOOL	SetBitResolution(UINT reqBitsCount);
	BOOL	SetFrameSize(int x, int y);
	BOOL	SetFrameSize(CSize sz);
	BOOL	GrabOneFrame();		  // manual capturing
	BOOL	SetShowFlag(BOOL fsf);// enable/disable painting
	BOOL	Cancel();			  // to cancel long-time operations
	//
	// Extended formats information
	//
	BOOL			GetDriverInfo(CAPDRVINFO *cdi);//get complete driver information
	CSize			GetFrameSize();
	DWORD			GetRequiredBufferSize();
	LPBITMAPINFO	GetCurrentFormat();
	BOOL			IsSupportedBitsResolution(UINT bitsCount);
	DWORD			GetCurrentBitsResolution();
					//get list of supported frame size
	BOOL			EnumValidFrameSize(CSize &sz, int indx);
	//
	// Extended  setup information
	//
	BOOL		GetDriverCaps(CAPDRIVERCAPS *caps);
	//
	// Additional driver(s) information
	//	
			//get comlpete capture drivers list
	int		GetDriversList(CStringArray *Names, CStringArray *Vers);
	LPCSTR	GetDriverName(); //get current driver name
	LPCSTR	GetDriverVer();	 //get current driver version string
	int		GetDriverIndex();//get current driver index
	//
	// Setup specific callbacks for drawing and progress notification
	//
			//setup callback for drawing in capture window
	VOID	SetDrawCallBack(DRAWCALLBACK cb);
			//setup callback for progress notification
	VOID	SetProgressCallback(PROGRESSCALLBACK cb);
	//
	// misc
	//
			//adjust window size and position  according current frame size
	void	AdjustPlacement(BOOL fStretch=FALSE);
	BOOL	IsReady();		//is connected with driver?
	int		GetLastError(); //last error code
	//
	// Optional attribute for quick connection(without testing)
	// Note if you performs quick connection you can't determine
	// internal driver buffers queue size or you have to modify
	// _connect2Driver method
	BOOL		m_DoQuickConnection; 

protected:

	CAPDRIVERCAPS	_capDriverCaps;
	CAPSTATUS		_capStatus;
	CAPTUREPARMS	_captureParms;
	LPBITMAPINFO	_bmpInfo;
	CStringArray	_drvNames;
	CStringArray 	_drvVers;
	int			_curDriver;
	BOOL 		_fDialogIsUp;
	BOOL		_fPreviewIsOn;
	BOOL		_fOverlayIsOn;
	BOOL		_fSeqIsOn;
	int			_previewRate;
	DWORD		_biBitsCountSupported;
	CDWordArray	_formats;
	CDWordArray	_cmprs_formats;
	BOOL		_1FrameCallBackInstalled;
	CSize		_smallFrame;
	int			_internalBufCnt;
	BOOL		_fShow;
	int			iLastError;
	BOOL		_autosize;

	volatile BOOL	_capAbort;

  	PROGRESSCALLBACK	_progressCB;
	DRAWCALLBACK		_drwcb;

	int		_GetInternalBuffersCount();
	BOOL	_IsImageCompressed();
	void	_notify(LPSTR lp=NULL);
	void	_notify(LPSTR lp, DWORD pc);
	void	_selfInit();
	void	_getDrvList();
	BOOL	_getCaps();
	BOOL	_getStatus();
	BOOL	_getSetup();
	BOOL	_setSetup();
	BOOL	_getFormat();
	BOOL	_connect2Driver(int indx);
	BOOL	_testBiBitsCount();
	BOOL	_testValidFormats();
	BOOL	_testInternalBuffers();
	BOOL	_testBitsFormat(UINT bits);
	BOOL	_testFrameFormat(int x, int y);

	LPBITMAPINFO	_mk_LPBITMAPINFO(UINT bitsReqCode,int x, int y);
	int				_getBMIsize(UINT bitsReq, UINT bmiColors);
	int				_getBMIsize(LPBITMAPINFO);
	DWORD			_calcBufferSize(LPBITMAPINFOHEADER bih);
	
	void		_pushResolution();
	void		_popResolution();
	void		_autoSize();
	int			_totalDrv();
protected:
	//{{AFX_MSG(CAviCap)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//Error codes
#define	CAP_CREATE_DUP				-1	/*can't connect twice!*/
#define	CAP_WINCREATION_FAILED		-2	/*failed to create capture window*/
#define	CAP_NO_CAPDEVICE_INSTALLED	-3	/*no capture device found*/
#define	CAP_DRIVERNAME_NOT_FOUND	-4	/*no capture driver found*/
#define	CAP_ILLEGAL_DRIVERID		-5	/*out of drivers list*/
#define	CAP_CONNECTION_FAILED		-6	/*no comments*/
#define	CAP_INIT_FAILED				-7	/*initialization step failed*/
#define	CAP_TEST_FAILED				-8	/*can't test device*/
#define	CAP_CALLNOWINDOW			-9	/*window not created*/
#define	CAP_CALLNOCONNECTION		-10	/*no connection with capture driver*/


//	CAviCap inlines
inline	int	CAviCap::_totalDrv()	
	{return _drvNames.GetSize();}
inline	BOOL	CAviCap::GetPreviewFlag()
	{return _fPreviewIsOn;}
inline	VOID	CAviCap::SetDrawCallBack(DRAWCALLBACK cb) 
	{_drwcb = cb;}
inline 	VOID	CAviCap::SetProgressCallback(PROGRESSCALLBACK cb) 
	{_progressCB= cb;}
inline	void	CAviCap::AdjustPlacement(BOOL fStretch)
	{_autosize=!fStretch; _autoSize(); }
inline	BOOL	CAviCap::IsReady()	
	{return (_curDriver!=-1)&&GetSafeHwnd();}
inline	int		CAviCap::GetLastError()
	{int ret=iLastError;iLastError=0; return ret;}	
inline	BOOL	CAviCap::_getStatus()
	{return capGetStatus (GetSafeHwnd(), &_capStatus, sizeof (CAPSTATUS));}
inline	BOOL	CAviCap::_getSetup()
	{return capCaptureGetSetup (GetSafeHwnd(), &_captureParms, sizeof CAPTUREPARMS);}
inline	BOOL	CAviCap::_setSetup()
	{return capCaptureSetSetup (GetSafeHwnd(), &_captureParms, sizeof CAPTUREPARMS);}
inline	void	CAviCap::_notify(LPSTR lp)
	{if(_progressCB) (*_progressCB)(lp,100);}
inline	void	CAviCap::_notify(LPSTR lp, DWORD pc)
	{if(_progressCB) (*_progressCB)(lp,pc);}
inline	BOOL	CAviCap::SetFrameSize(CSize sz)
	{return SetFrameSize(sz.cx,sz.cy);}
inline	LPCSTR	CAviCap::GetDriverName()	
	{return (_curDriver==-1||!_drvNames.GetSize()) ? NULL : (LPCSTR)_drvNames[_curDriver];}
inline	LPCSTR	CAviCap::GetDriverVer()	
	{return (_curDriver==-1||!_drvNames.GetSize()) ? NULL : (LPCSTR)_drvVers[_curDriver];}
inline	int		CAviCap::GetDriverIndex() 
	{return _curDriver;}
inline	BOOL	CAviCap::SavePalette(LPCSTR file) 
	{return (_curDriver==-1) ? FALSE : (BOOL)capPaletteSave(GetSafeHwnd(), file);}
inline	BOOL	CAviCap::LoadPalette(LPCSTR file) 
	{return (_curDriver==-1) ? FALSE : (BOOL)capPaletteOpen(GetSafeHwnd(), file);}
inline	BOOL	CAviCap::PastePalette()
	{return (_curDriver==-1) ? FALSE : (BOOL)capPalettePaste(GetSafeHwnd());}
inline	BOOL	CAviCap::CreatePalette(int frames,int colors)
	{return (_curDriver==-1) ? FALSE : (BOOL)capPaletteAuto(GetSafeHwnd(), frames, colors);};
inline	BOOL	CAviCap::CopyFrame()
	{return (_curDriver==-1) ? FALSE : (BOOL)capEditCopy(GetSafeHwnd());}
inline	BOOL	CAviCap::SetShowFlag(BOOL fsf)
	{BOOL os=_fShow; _fShow=fsf; return os;}
inline	CSize	CAviCap::GetFrameSize()
	{return (_curDriver==-1) ? CSize(0,0) : CSize(((LPBITMAPINFOHEADER)_bmpInfo)->biWidth,((LPBITMAPINFOHEADER)_bmpInfo)->biHeight);}
inline	DWORD	CAviCap::GetRequiredBufferSize()
	{return (_curDriver==-1) ? FALSE : _calcBufferSize((LPBITMAPINFOHEADER)_bmpInfo);}
inline	LPBITMAPINFO	CAviCap::GetCurrentFormat()
	{return _bmpInfo;}
inline	DWORD	CAviCap::GetCurrentBitsResolution()
	{_getFormat(); return  _bmpInfo ? ((LPBITMAPINFOHEADER)_bmpInfo)->biBitCount:0;}
inline	BOOL	CAviCap::CompressDlg()
	{return (_curDriver==-1) ? FALSE : (BOOL)capDlgVideoCompression(GetSafeHwnd());}
inline	BOOL	CAviCap::DisplayDlg()
	{BOOL r = (_curDriver==-1) ? FALSE : (BOOL)capDlgVideoDisplay(GetSafeHwnd());_autoSize();return r;}	
inline	BOOL	CAviCap::FormatDlg()
	{BOOL r = (_curDriver==-1) ? FALSE : (BOOL)capDlgVideoFormat(GetSafeHwnd());_autoSize();return r;}
inline	BOOL	CAviCap::VSourceDlg()
	{BOOL r = (_curDriver==-1) ? FALSE : (BOOL)capDlgVideoSource(GetSafeHwnd());_autoSize();return r;}
inline	BOOL	CAviCap::Cancel()
	{return _capAbort=TRUE;}

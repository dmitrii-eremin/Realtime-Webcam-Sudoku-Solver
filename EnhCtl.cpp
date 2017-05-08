// EnhCtl.cpp : implementation file
//

#include "stdafx.h"
#include "AviCapTest.h"
#include "EnhCtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EnhCtl dialog


EnhCtl::EnhCtl(CWnd* pParent /*=NULL*/)
	: CDialog(EnhCtl::IDD, pParent)
{
	//{{AFX_DATA_INIT(EnhCtl)
	m_iFormat = -1;
	m_iResolution = -1;
	//}}AFX_DATA_INIT
}


void EnhCtl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EnhCtl)
	DDX_Control(pDX, IDC_RESOLUTION, m_Resolutions);
	DDX_Control(pDX, IDC_FORMATLIST, m_Formats);
	DDX_CBIndex(pDX, IDC_FORMATLIST, m_iFormat);
	DDX_CBIndex(pDX, IDC_RESOLUTION, m_iResolution);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EnhCtl, CDialog)
	//{{AFX_MSG_MAP(EnhCtl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EnhCtl message handlers

BOOL EnhCtl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	for(int i=0;i<m_formats_str.GetSize();i++)
	{
		m_Formats.AddString(m_formats_str[i]);
	}
	for(int i=0;i<m_Resolutions_str.GetSize();i++)
	{
		m_Resolutions.AddString(m_Resolutions_str[i]);
	}
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

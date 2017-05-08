// Info.cpp : implementation file
//

#include "stdafx.h"
#include "AviCapTest.h"
#include "Info.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Info dialog


Info::Info(CWnd* pParent /*=NULL*/)
	: CDialog(Info::IDD, pParent)
{
	//{{AFX_DATA_INIT(Info)
	m_InfoBuffer = _T("");
	//}}AFX_DATA_INIT
}


void Info::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Info)
	DDX_Text(pDX, IDC_EDIT1, m_InfoBuffer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Info, CDialog)
	//{{AFX_MSG_MAP(Info)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Info message handlers

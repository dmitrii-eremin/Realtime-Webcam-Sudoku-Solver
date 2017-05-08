#if !defined(AFX_ENHCTL_H__90863A42_0EBD_11D3_A49E_0080AD10B34E__INCLUDED_)
#define AFX_ENHCTL_H__90863A42_0EBD_11D3_A49E_0080AD10B34E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// EnhCtl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// EnhCtl dialog

class EnhCtl : public CDialog
{
// Construction
public:
	EnhCtl(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(EnhCtl)
	enum { IDD = IDD_DIALOG2 };
	CComboBox	m_Resolutions;
	CComboBox	m_Formats;
	int		m_iFormat;
	int		m_iResolution;
	//}}AFX_DATA

	CStringArray m_formats_str;
	CStringArray m_Resolutions_str;
	UINT		 m_bitsRes[6];
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EnhCtl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(EnhCtl)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENHCTL_H__90863A42_0EBD_11D3_A49E_0080AD10B34E__INCLUDED_)

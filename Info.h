#if !defined(AFX_INFO_H__79A28182_8939_11D2_B4B8_444553540000__INCLUDED_)
#define AFX_INFO_H__79A28182_8939_11D2_B4B8_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Info.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Info dialog

class Info : public CDialog
{
// Construction
public:
	Info(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Info)
	enum { IDD = IDD_DIALOG1 };
	CString	m_InfoBuffer;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Info)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Info)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFO_H__79A28182_8939_11D2_B4B8_444553540000__INCLUDED_)

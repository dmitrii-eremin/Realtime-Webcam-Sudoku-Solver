// AviCapTestDoc.h : interface of the CAviCapTestDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVICAPTESTDOC_H__5D2A240C_891F_11D2_B4B8_444553540000__INCLUDED_)
#define AFX_AVICAPTESTDOC_H__5D2A240C_891F_11D2_B4B8_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CAviCapTestDoc : public CDocument
{
protected: // create from serialization only
	CAviCapTestDoc();
	DECLARE_DYNCREATE(CAviCapTestDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAviCapTestDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAviCapTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAviCapTestDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AVICAPTESTDOC_H__5D2A240C_891F_11D2_B4B8_444553540000__INCLUDED_)

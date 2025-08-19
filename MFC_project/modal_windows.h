#pragma once
#define STATIC_BUTTON 100
#define DYNAMIC_BUTTON 101
#include <afxwin.h>
#include <afxdlgs.h>
#include <afxdialogex.h>
//¥Î©ó½m²ßmodal dialog
class CModalWinsDialog : public CDialog {
private:
    DLGTEMPLATE* CreateDialogTemplate();
protected:
    virtual BOOL OnInitDialog()override;
    afx_msg virtual void PostNcDestroy()override;
    afx_msg void OnDestroy();
    DECLARE_MESSAGE_MAP()
public:
    CModalWinsDialog(CWnd* pParent=nullptr);
    virtual ~CModalWinsDialog();
    DLGTEMPLATE* pTemplate;
};

class MyDialog :public CWnd {
public:
    MyDialog();
    ~MyDialog();
    BOOL Create(CWnd* pParent);
    void CreateModal(CWnd* pParent);
protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg virtual void PostNcDestroy()override;
	afx_msg void OnStaticButtonClicked();
	afx_msg void OnDynamicButtonClicked();
    DECLARE_MESSAGE_MAP()
private:
    CButton m_static_button,*m_dynamic_button;
};
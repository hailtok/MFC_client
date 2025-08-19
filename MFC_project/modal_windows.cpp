#include"modal_windows.h"
BEGIN_MESSAGE_MAP(CModalWinsDialog, CDialog)
    
END_MESSAGE_MAP()
CModalWinsDialog::CModalWinsDialog(CWnd* pParent) :CDialog(nullptr, pParent) {
    pTemplate=CreateDialogTemplate();
}
DLGTEMPLATE* CModalWinsDialog::CreateDialogTemplate() {
    // 配置記憶體
    BYTE* pMem = new BYTE[1024];
    ZeroMemory(pMem, 1024);

    DLGTEMPLATE* pDlg = (DLGTEMPLATE*)pMem;

    // 基本對話框樣式
    pDlg->style = DS_SETFONT | DS_CENTER | WS_VISIBLE | WS_CAPTION | WS_SYSMENU;
    pDlg->cx = 205;
    pDlg->cy = 150;

    BYTE* p = (BYTE*)(pDlg + 1);

    // 無 menu
    *(WORD*)p = 0; p += sizeof(WORD);

    // 使用預設 dialog class
    *(WORD*)p = 0; p += sizeof(WORD);

    // Caption 字串
    wcscpy_s((WCHAR*)p, 32, L"MODAL");
    p += (wcslen(L"MODAL") + 1) * sizeof(WCHAR);

    // Font 設定
    *(WORD*)p = 9;  p += sizeof(WORD); // 字體大小
    wcscpy_s((WCHAR*)p, 32, L"MS Shell Dlg");

    return pDlg;
}
BOOL CModalWinsDialog::OnInitDialog()
{
    return CDialog::OnInitDialog();
}
//destroy UI in client area
void CModalWinsDialog::OnDestroy() {
    CDialog::OnDestroy();
}
//destroy UI in non-client area
void CModalWinsDialog::PostNcDestroy() {
    //delete this;
}
//delete the memory allocated 
CModalWinsDialog::~CModalWinsDialog() {
    delete pTemplate;
}

BEGIN_MESSAGE_MAP(MyDialog, CWnd)
    ON_WM_CREATE()
    ON_WM_DESTROY()
    ON_BN_CLICKED(DYNAMIC_BUTTON, &MyDialog::OnDynamicButtonClicked)
    ON_BN_CLICKED(STATIC_BUTTON, &MyDialog::OnStaticButtonClicked)
END_MESSAGE_MAP()
MyDialog::MyDialog() {
    m_dynamic_button = new CButton();
}
BOOL MyDialog::Create(CWnd* pParent) {
    LPCTSTR className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW);
    return CWnd::CreateEx(
        0, className, _T("Window"),
        WS_OVERLAPPEDWINDOW,
        100, 100, 400, 300,
        pParent->GetSafeHwnd(),
        nullptr
    );
}
void MyDialog::CreateModal(CWnd* pParent) {
    MyDialog* pWnd = new MyDialog;
    pWnd->Create(pParent);
    pWnd->ShowWindow(SW_SHOW);
    if (pParent)
        pParent->EnableWindow(FALSE);
    MSG msg;
    while (pWnd->IsWindowVisible() && GetMessage(&msg, nullptr, 0, 0))
    {
        if (msg.message == WM_DESTROY|| msg.message == WM_CLOSE) {
            break;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    if (pParent) {
        pParent->EnableWindow(TRUE);
    }
}
void MyDialog::OnDynamicButtonClicked() {
    AfxMessageBox(_T("Dynamic Button Clicked!"));
}
void MyDialog::OnStaticButtonClicked() {
	AfxMessageBox(_T("Static Button Clicked!"));
}
int MyDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    m_dynamic_button->Create(_T("Dynamic Button"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(80, 135, 200, 165), this, DYNAMIC_BUTTON);
	m_static_button.Create(_T("Static Button"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(200, 135, 320, 165), this, STATIC_BUTTON);
    return TRUE;
}
void MyDialog::OnDestroy() {
    if (m_dynamic_button&&m_dynamic_button->GetSafeHwnd()) {
        m_dynamic_button->DestroyWindow();
    }
    if (m_static_button.GetSafeHwnd()) {
        m_static_button.DestroyWindow();
    }
}
void MyDialog::PostNcDestroy() {
    delete this;
}
MyDialog::~MyDialog() {
	delete m_dynamic_button;
}
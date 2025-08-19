#include"dialog.h"
BEGIN_MESSAGE_MAP(CMainDialog, CDialog)
    ON_BN_CLICKED(MODAL_BUTTON, &CMainDialog::OnModalButtonClicked)
    ON_BN_CLICKED(MODELESS_BUTTTON, &CMainDialog::OnModelessButtonClicked)
END_MESSAGE_MAP()
CMainDialog::CMainDialog() {
    pTemplate = this->CreateDialogTemplate();
    this->InitModalIndirect(pTemplate);
    mydialog = new MyDialog();
}
CMainDialog::CMainDialog(UINT, CWnd* pParent){
    pTemplate = this->CreateDialogTemplate();
    this->InitModalIndirect(pTemplate);
    mydialog = new MyDialog();
}
CMainDialog::~CMainDialog() {
    delete pTemplate;
}
DLGTEMPLATE* CMainDialog::CreateDialogTemplate() {
    // �t�m�O����
    BYTE* pMem = new BYTE[1024];
    ZeroMemory(pMem, 1024);

    DLGTEMPLATE* pDlg = (DLGTEMPLATE*)pMem;

    // �򥻹�ܮؼ˦�
    pDlg->style = DS_SETFONT | DS_CENTER | WS_VISIBLE | WS_CAPTION | WS_SYSMENU;
    pDlg->cx = 205;
    pDlg->cy = 150;

    BYTE* p = (BYTE*)(pDlg + 1);

    // �L menu
    *(WORD*)p = 0; p += sizeof(WORD);

    // �ϥιw�] dialog class
    *(WORD*)p = 0; p += sizeof(WORD);

    // Caption �r��
    wcscpy_s((WCHAR*)p, 32, L"WINDOW");
    p += (wcslen(L"WINDOW") + 1) * sizeof(WCHAR);

    // Font �]�w
    *(WORD*)p = 9;  p += sizeof(WORD); // �r��j�p
    wcscpy_s((WCHAR*)p, 32, L"MS Shell Dlg");

    return pDlg;
}
BOOL CMainDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    // ���o��ܮؤj�p
    CRect clientRect;
    GetClientRect(&clientRect);

    int dialogWidth = clientRect.Width();
    int dialogHeight = clientRect.Height();

    int btnWidth = 100;
    int btnHeight = 40;
    int btnY = dialogHeight / 2 - btnHeight / 2;
    m_btnLeft.Create(_T("Modal"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        CRect(dialogWidth / 4 - btnWidth / 2, btnY, dialogWidth / 4 + btnWidth / 2, btnY + btnHeight),
        this, MODAL_BUTTON);

    m_btnRight.Create(_T("Modeless"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        CRect(3 * dialogWidth / 4 - btnWidth / 2, btnY, 3 * dialogWidth / 4 + btnWidth / 2, btnY + btnHeight),
        this, MODELESS_BUTTTON);

    return TRUE;
}

void CMainDialog::OnModalButtonClicked()
{
    if (pModelessDialog) {
        pModelessDialog->DestroyWindow();
        pModelessDialog = nullptr;
    }
	CModalWinsDialog modalDialog;
    modalDialog.InitModalIndirect(modalDialog.pTemplate,this);
    modalDialog.DoModal();

    /*
    mydialog = new MyDialog;
    mydialog->CreateModal(this);
    */
}
void CMainDialog::OnModelessButtonClicked()
{
    if (pModelessDialog&& ::IsWindow(pModelessDialog->GetSafeHwnd())) {
        pModelessDialog->ShowWindow(SW_SHOW);
    }
    else {
        pModelessDialog = new CModelessWinsDialog(this);
        BOOL ret = pModelessDialog->CreateIndirect(pModelessDialog->pTemplate, this);
        pModelessDialog->ShowWindow(SW_SHOW);
    }
    
    
    /*
    mydialog = new MyDialog;
    mydialog->Create(this);
    mydialog->ShowWindow(SW_SHOW);
    */
}
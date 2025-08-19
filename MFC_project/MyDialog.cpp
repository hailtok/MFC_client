#include "MyDialog.h"
BEGIN_MESSAGE_MAP(CMyDialog, CDialogEx)
    ON_WM_CTLCOLOR()
    ON_WM_MEASUREITEM()
    ON_WM_DRAWITEM()
    ON_LBN_SELCHANGE(ID_LISTBOX, &CMyDialog::OnLbnSelChangeList)
    ON_CONTROL_RANGE(BN_CLICKED, BUTTON_BASE,
        BUTTON_BASE + button_count - 1, &CMyDialog::ButtonsClicked_Event)
    ON_MESSAGE(WM_SOCKET_NOTIFY, &CMyDialog::OnSocketNotify)
    ON_WM_COPYDATA()
END_MESSAGE_MAP()
CMyDialog::CMyDialog() {
    DLGTEMPLATE* pTemplate = this->CreateDialogTemplate();
    this->InitModalIndirect(pTemplate);
    shared_file = NULL;
    for (int i = 0; i < string_count; ++i) {
        UI_string.push_back(_T(""));
    }
    m_Font = new CFont();
    m_ID_Recipe_NameList = new CListBox();
    Light_Setting_ComboBox = new CComboBox();
    Inspection_Setting_ComboBox = new CComboBox();
    Edits = new CEdit * [edit_count];
    for (int i = 0; i < edit_count; ++i) {
        Edits[i] = new CEdit();
    }
    Buttons = new CButton * [button_count];
    for (int i = 0; i < button_count; ++i) {
        Buttons[i] = new CButton();
    }
    Statics = new CStatic * [static_count];
    for (int i = 0; i < static_count; ++i) {
        Statics[i] = new CStatic();
    }
    Client_Socket = new CClientSocket(this);
    share_memory_static = new CStatic();
    shared_memory = new Shared_Memory();
}
CMyDialog::CMyDialog(UINT Template,CWnd* pParent):CDialogEx(Template, pParent){
    shared_file = NULL;
    for (int i = 0;i< string_count;++i) {
        UI_string.push_back(_T(""));
    }
    m_Font = new CFont();
    m_ID_Recipe_NameList = new CListBox();
    Light_Setting_ComboBox = new CComboBox();
    Inspection_Setting_ComboBox = new CComboBox();
	Edits = new CEdit * [edit_count];
	Buttons = new CButton * [button_count];
	Statics = new CStatic * [static_count];
    Client_Socket = new CClientSocket(this);
	share_memory_static = new CStatic();
    shared_memory=new Shared_Memory();
}
DLGTEMPLATE* CMyDialog::CreateDialogTemplate() {
    // �t�m�O����
    BYTE* pMem = new BYTE[1024];
    ZeroMemory(pMem, 1024);

    DLGTEMPLATE* pDlg = (DLGTEMPLATE*)pMem;

    // �򥻹�ܮؼ˦�
    pDlg->style = DS_SETFONT | DS_FIXEDSYS | WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | DS_CENTER;
    pDlg->cx = 480;
    pDlg->cy = 360;

    BYTE* p = (BYTE*)(pDlg + 1);

    // �L menu
    *(WORD*)p = 0; p += sizeof(WORD);

    // �ϥιw�] dialog class
    *(WORD*)p = 0; p += sizeof(WORD);

    // Caption �r��
    wcscpy_s((WCHAR*)p, 32, L"ReceiverWindowTitle");
    p += (wcslen(L"ReceiverWindowTitle") + 1) * sizeof(WCHAR);

    // Font �]�w
    *(WORD*)p = 10;  p += sizeof(WORD); // �r��j�p
    wcscpy_s((WCHAR*)p, 32, L"Microsoft YaHei UI");

    return pDlg;
}
CMyDialog::~CMyDialog(){
    delete m_ID_Recipe_NameList;
	delete Light_Setting_ComboBox;
    delete Inspection_Setting_ComboBox;
    for (int i = 0; i < edit_count; ++i) {
        delete Edits[i];
    }
    delete[] Edits;
    for (int i = 0; i < button_count; ++i) {
        delete Buttons[i];
	}
	delete[] Buttons;
    for (int i = 0; i < static_count; ++i) {
        delete Statics[i];
    }
	delete[] Statics;
    delete shared_memory;
}
BOOL CMyDialog::OnInitDialog(){
    CDialogEx::OnInitDialog();
    //initialize font
    m_Font->CreateFont(
        18, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS, _T("Microsoft JhengHei"));
    //initialize edit
    for (int i = 0; i < edit_count; ++i) {
        CRect rect;
        if (i < 2) {
            rect = CRect(420 + i * 240, 18, 550 + i * 240, 53);
        }
        else {
            rect = CRect(525, 70 + (i - 2) * 53, 790, 105 + (i - 2) * 53);
        }
        Edits[i]->Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL | ES_NOHIDESEL | WS_TABSTOP,
            rect, this, RECIPE_ID_EDIT + i);
        Edits[i]->SetFont(m_Font);
    }
	//initialize listbox
    m_ID_Recipe_NameList->Create(
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP |
        LBS_OWNERDRAWFIXED | LBS_HASSTRINGS| LBS_NOTIFY,
        CRect(10,18, 250, 450),
        this,
        ID_LISTBOX
    );
    m_ID_Recipe_NameList->SetFont(m_Font);
    //initialize combobox
	Light_Setting_ComboBox->Create(CBS_DROPDOWN | WS_VSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
        CRect(525, 280, 790,410), this, LIGHTSETTING_COMBOBOX);
	Inspection_Setting_ComboBox->Create(CBS_DROPDOWN | WS_VSCROLL | WS_CHILD | WS_VISIBLE | WS_TABSTOP,
        CRect(525, 335, 790, 465), this, INSPECTIONSETTING_COMBOBOX);
	//initialize button
    for (int i = 0;i<button_count;++i) {
        CRect rect;
        if (i < 4) {
            rect = CRect(525 , 384 + i* 53, 788, 423 + i * 53);
            CString temp;
            temp.Format(_T("edit screen %d"), i + 1);
            Buttons[i]->Create(temp, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                rect, this, BUTTON_BASE + i);
        }
        else {
            if ((i-4)/2==0) {
                rect = CRect(263+(i - 4) % 2* 275, 595, 540 + (i - 4) % 2 * 275, 665);
            }
            else {
                rect = CRect(263 + (i - 4) % 2 * 275,665, 540 + (i - 4) % 2 * 275, 735);
            }
            Buttons[i]->Create(_T(""), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | WS_TABSTOP,
                rect, this, BUTTON_BASE + i);
        }
    }
    //initialize static
    for (int i = 0; i < static_count; ++i) {
        CRect rect;
        if (i < 2) {
            rect = CRect(263 + i * 262, 18, 420 + i * 262, 53);
        }
        else {
            rect = CRect(263, 70 + (i - 2) * 53, 527, 105 + (i - 2) * 53);
        }
        Statics[i]->Create(_T(""),              
            WS_CHILD | WS_VISIBLE | SS_LEFT| SS_OWNERDRAW,  
            rect,                              
            this,                              
            STATIC_BASE + i);
    }
    //�]�w�r��j�p�M�r��
    Light_Setting_ComboBox->SetFont(m_Font);
    Inspection_Setting_ComboBox->SetFont(m_Font);
    //find specific files for combobox
	//setting Light_Setting_ComboBox
    std::vector<CString>files;
    files=find_specfic_file(_T(".\\"), _T("aaa"));
    for (int i = 0; i < files.size(); ++i) {
        Light_Setting_ComboBox->AddString(files[i]);
    }
    //settting Inspection_Setting_ComboBox
    files = find_specfic_file(_T(".\\"), _T("bbb"));
    for (int i = 0; i < files.size(); ++i) {
        Inspection_Setting_ComboBox->AddString(files[i]);
    }
    //setting ui langauge
    Read_UI_language_ini_file();
    Read_ini_file_method1();
    //Read_ini_file_method2();

    AfxSocketInit();
	//setting socket receive event, after receiving data, post a message to the dialog
    Client_Socket->SetOnReceive([this]() {
        ::PostMessage(Client_Socket->GetParent()->GetSafeHwnd(), WM_SOCKET_NOTIFY, (WPARAM)Client_Socket->GetFileSize(), 0);
    });
    Client_Socket->Create();  // �ʺA������� port
    //Client_Socket->Connect(_T("127.0.0.1"), 1100);  // �s�u���� server
    if (!Client_Socket->Connect(_T("127.0.0.1"), 1100)) {
        int err = Client_Socket->GetLastError();
    }
    shared_memory->Create();
    // ���o listbox ��m
    CWnd* pListBox = GetDlgItem(ID_LISTBOX);
    CRect rcListBox;
    pListBox->GetWindowRect(&rcListBox);
    ScreenToClient(&rcListBox);

	//only for testing, remove later
    // �]�w share_memory_static �b listbox �U��
    int staticWidth = rcListBox.Width();
    int staticHeight = 200; // �A�Q�n������
    int staticX = rcListBox.left;
    int staticY = rcListBox.bottom + 10; // listbox �U�� 10px
    share_memory_static->Create(
        _T(""), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW,
        CRect(staticX, staticY, staticX + staticWidth, staticY + staticHeight),
        this, BMP_STATIC
    );
    return TRUE;
}
void CMyDialog::Read_UI_language_ini_file() {
    CString iniPath = _T(".\\config_ui_language_utf8.ini");
    std::ifstream file(iniPath);
    std::string line,language="us-en";
    std::vector<CString> UI_name = {("RECIPE_ID"),("REFERENCE_RECIPE"),("Recipe_Name"),
        ("MATERIAL_WIDTH"),("SHEET_LENGTH"),("OFFSET"),("LIGHT_SETTING"),
        ("INSPECTION_SETTING"), ("SIZE_FILTER"), ("DEFECT_TYPE_LABEL"),
        ("DEFECT_TYPE_REJECTOR"), ("RESERVED"), ("INPUT_BUTTON"),("CANCEL_BUTTON"),
        ("SAVE_BUTTON"), ("CLOSE_BUTTON") };
    bool InSection = false;
    while (std::getline(file,line)) {
        if (line.empty()) continue;
        //���������y��
        if (line == "[LANGUAGE]") {
            std::getline(file, line);
            int eqPos = line.find('=');
            if (eqPos == std::string::npos) continue;
            language = line.substr(eqPos + 1);
            while (std::getline(file, line)) {
                if (line=="["+language+"]") {
                    break;
                }
            }
            //������y����UI�W��
            for (int i = 0; i < UI_name.size(); ++i) {
                std::getline(file, line);
                int eqPos = line.find('=');
                std::string val = line.substr(eqPos + 1);
                CString text = Utf8ToWideChar(val).c_str();
                if (i < static_count) {
                    CWnd* wnd = GetDlgItem(STATIC_BASE + i);
                    wnd->SetWindowText(text);
                }
                else {
                    CWnd* wnd = GetDlgItem(INPUT_BUTTON + i - static_count);
                    wnd->SetWindowText(text);
                }
            }
            break;
        }
    }

}
void CMyDialog::Read_ini_file_method1() {
    //Ū��ini�ɥH��m_ID_Recipe_NameList��l��
    //method 1
    m_ID_Recipe_NameList->AddString(_T("ID,Recipe Name"));
    CString iniPath = _T(".\\config.ini"),CountStr;
    GetPrivateProfileString(_T("array_length"),_T("UI_COUNT"),_T(""),CountStr.GetBuffer(256),256, iniPath);
    for (int i = 0, UI_count = _ttoi(CountStr);i< UI_count;++i) {
        CString section;
        section.Format(_T("%d"), i);
        std::vector<CString>UI_cstring(keys.size());
        for (int i = 0;i<keys.size();++i) {
            GetPrivateProfileString(section, keys[i], _T(""), UI_cstring[i].GetBuffer(256), 256, iniPath); UI_cstring[i].ReleaseBuffer();
        }
        //��LIGHTSETTING_COMBOBOX�S���ȡA�]�w��Combobox�����Ĥ@�ӿﶵ
        if (UI_cstring[6].IsEmpty() && Light_Setting_ComboBox->GetCount() > 0) {
            Light_Setting_ComboBox->GetLBText(0, UI_cstring[6]);
        }
        //��INSPECTIONSETTING_COMBOBOX�S���ȡA�]�w��Combobox�����Ĥ@�ӿﶵ
        if (UI_cstring[7].IsEmpty() && Inspection_Setting_ComboBox->GetCount() > 0) {
            Inspection_Setting_ComboBox->GetLBText(0, UI_cstring[7]);
        }
        m_ID_Recipe_NameList->AddString(UI_cstring[0] + _T(",") + UI_cstring[1]);
        lists[_wtoi(UI_cstring[0])] = UI_cstring;
    }
}
void CMyDialog::Read_ini_file_method2() {
    //method2
    m_ID_Recipe_NameList->AddString(_T("ID,Recipe Name"));
    CString iniPath = _T(".\\config2.ini"), section = _T("parameter"),countStr;
    GetPrivateProfileString(section,_T("UI_COUNT"), _T(""), countStr.GetBuffer(256), 256, iniPath); countStr.ReleaseBuffer();
    int index = 0;
    for (int i = 0, UI_count = _ttoi(countStr);i<UI_count;++i) {
        std::vector<CString>UI_section_cstring(keys.size()),UI_cstring(keys.size());
        for (int i = 0;i<keys.size();++i) {
            UI_section_cstring[i].Format(_T("%s %d"), (LPCTSTR)keys[i], index);
            GetPrivateProfileString(section, UI_section_cstring[i], _T(""), UI_cstring[i].GetBuffer(256), 256, iniPath); UI_cstring[i].ReleaseBuffer();
        }
        //��LIGHTSETTING_COMBOBOX�S���ȡA�]�w��Combobox�����Ĥ@�ӿﶵ
        if (UI_cstring[6].IsEmpty() && Light_Setting_ComboBox->GetCount() > 0) {
            Light_Setting_ComboBox->GetLBText(0, UI_cstring[6]);
        }
        //��INSPECTIONSETTING_COMBOBOX�S���ȡA�]�w��Combobox�����Ĥ@�ӿﶵ
        if (UI_cstring[7].IsEmpty() && Inspection_Setting_ComboBox->GetCount() > 0) {
            Inspection_Setting_ComboBox->GetLBText(0, UI_cstring[7]);
        }
        m_ID_Recipe_NameList->AddString(UI_cstring[0] + _T(",") + UI_cstring[1]);
        lists[_wtoi(UI_cstring[0])] = UI_cstring;
        ++index;
    }
}
void CMyDialog::Write_ini_file_method1() {
    //�Nlists������Ƽg�Jini��
    int index = 0;
    CString iniPath = _T(".\\config.ini");
    // ���M�����ɮ�
    DeleteFile(iniPath);
    //�g�JBOM
    CFile file;
    if (!file.Open(iniPath, CFile::modeRead)) {
        // UTF-16 LE BOM: 0xFF 0xFE
        CFile bomFile(iniPath, CFile::modeCreate | CFile::modeWrite);
        WORD bom = 0xFEFF;
        bomFile.Write(&bom, sizeof(bom));
        bomFile.Close();
    }
    //method 1:
    //�Nlistbox������Ƽƶq�g�Wini
    CString countStr;
    countStr.Format(_T("%zu"), lists.size());
    WritePrivateProfileString(_T("array_length"),_T("UI_COUNT"), countStr, iniPath);
    //�g�J�C�@�����
    for (const auto& item : lists) {
        CString section;
        section.Format(_T("%d"), index);
        for (int i = 0; i < keys.size(); ++i) {
            WritePrivateProfileString(section, keys[i], item.second[i], iniPath);
        }
        ++index;
    }
}
void CMyDialog::Write_ini_file_method2() {
    //method 2:
    CString iniPath = _T(".\\config2.ini");
    // ���M�����ɮ�
    DeleteFile(iniPath);
    //�g�JBOM
    CFile file;
    if (!file.Open(iniPath, CFile::modeRead)) {
        // UTF-16 LE BOM: 0xFF 0xFE
        CFile bomFile(iniPath, CFile::modeCreate | CFile::modeWrite);
        WORD bom = 0xFEFF;
        bomFile.Write(&bom, sizeof(bom));
        bomFile.Close();
    }
    CString section, countStr;
    section.Format(_T("parameter"));
    countStr.Format(_T("%zu"), lists.size());
    WritePrivateProfileString(section, _T("UI_COUNT"), countStr, iniPath);
    int index = 0;
    for (auto it = lists.begin(); it != lists.end(); ++it, ++index) {
        for (int i = 0; i < keys.size(); ++i) {
            CString temp_str;
            temp_str.Format(_T("%s %d"),(LPCTSTR)keys[i], index);
            WritePrivateProfileString(section, temp_str, it->second[i], iniPath);
        }
    }
}
void CMyDialog::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) {
    if (nIDCtl == ID_LISTBOX)
    {
        lpMeasureItemStruct->itemHeight = 28; // �A�Q�n���C�C����
    }
}
void CMyDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct){
    if (nIDCtl == ID_LISTBOX) {
        DrawListBoxs(nIDCtl, lpDrawItemStruct);
    }
    else if (nIDCtl == INPUT_BUTTON || nIDCtl == CANCEL_BUTTON || nIDCtl == SAVE_BUTTON || nIDCtl == CLOSE_BUTTON) {
        DrawButtons(nIDCtl, lpDrawItemStruct);
    }
    else if (nIDCtl == RECIPE_ID_EDIT || nIDCtl == REF_RECIPE_ID_EDIT || nIDCtl == RECIPE_NAME_EDIT ||
        nIDCtl == MATERIAL_WIDTH_EDIT || nIDCtl == SHEET_LENGTH_EDIT || nIDCtl == OFFSET_EDIT) {
        DrawEdits(nIDCtl, lpDrawItemStruct);
    }
    else {
        DrawStatics(nIDCtl, lpDrawItemStruct);
    }
}
void CMyDialog::DrawListBoxs(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) {
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    int itemID = lpDrawItemStruct->itemID;
    if (itemID < 0) return;
    // ������
    int colWidth = (lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left) / 4;
    CRect rc1 = lpDrawItemStruct->rcItem;
    rc1.right = rc1.left + colWidth;
    CRect rc2 = lpDrawItemStruct->rcItem;
    rc2.left = rc1.right;
    // �I����
    COLORREF bgColor = (itemID == 0) ? RGB(217, 225, 242) : RGB(217, 217, 217);
    CBrush brush(bgColor);
    pDC->FillRect(&lpDrawItemStruct->rcItem, &brush);
    // ���o���e
    CString text;
    m_ID_Recipe_NameList->GetText(itemID, text);
    // �����Ρ]���]�γr�����j����^
    CString col1, col2;
    int pos = text.Find(_T(','));
    if (pos >= 0) {
        col1 = text.Left(pos);
        col2 = text.Mid(pos + 1);
    }
    else {
        col1 = text;
    }
    // ��r�C��
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(0, 0, 0));
    pDC->DrawText(" " + col1, &rc1, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    pDC->DrawText(" " + col2, &rc2, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
    // ��줧���[�W�ݦV��u
    CPen pen(PS_DOT, 1, RGB(100, 100, 100));
    CPen* oldPen = pDC->SelectObject(&pen);
    int x = rc1.right;
    pDC->MoveTo(x, lpDrawItemStruct->rcItem.top);
    pDC->LineTo(x, lpDrawItemStruct->rcItem.bottom);
    pDC->SelectObject(oldPen);
    // ��V��u���j
    oldPen = pDC->SelectObject(&pen);
    int y = lpDrawItemStruct->rcItem.bottom - 1;
    pDC->MoveTo(lpDrawItemStruct->rcItem.left, y);
    pDC->LineTo(lpDrawItemStruct->rcItem.right, y);
    pDC->SelectObject(oldPen);
    // �����
    if (lpDrawItemStruct->itemState & ODS_SELECTED && itemID != 0)
        pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
}
void CMyDialog::DrawStatics(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) {
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    // �e�L�Ŧ�I��
    CBrush brush(RGB(217, 225, 242));
    pDC->FillRect(&lpDrawItemStruct->rcItem, &brush);
    // �e�¦����
    pDC->DrawEdge(&lpDrawItemStruct->rcItem, EDGE_BUMP, BF_RECT);
    // �e��r�]�m���^
    CString strText;
    GetDlgItem(nIDCtl)->GetWindowText(strText);
    pDC->SetBkMode(TRANSPARENT);
    pDC->DrawText(strText, &lpDrawItemStruct->rcItem, ES_LEFT | DT_VCENTER | DT_SINGLELINE);
}
void CMyDialog::DrawButtons(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) {
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rc = lpDrawItemStruct->rcItem;
    // �e�`�Ŧ�I��
    CBrush brush(RGB(189, 215, 238));
    pDC->FillRect(&rc, &brush);
    // �e�¦����
    pDC->DrawEdge(&lpDrawItemStruct->rcItem, EDGE_BUMP, BF_RECT);
    // �]�w�r��
    CFont* pOldFont = pDC->SelectObject(m_Font);
    // ���o���s��r
    CString strText;
    GetDlgItem(nIDCtl)->GetWindowText(strText);
    // �]�w��r�C��]�¦�^
    pDC->SetTextColor(RGB(0, 0, 0));
    pDC->SetBkMode(TRANSPARENT);
    // �m��ø�s��r
    pDC->DrawText(strText, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    pDC->SelectObject(pOldFont);
}
void CMyDialog::DrawEdits(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) {
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    COLORREF bgColor = RGB(217, 217, 217);
    CBrush brush(bgColor);
    pDC->FillRect(&lpDrawItemStruct->rcItem, &brush);
}
void CMyDialog::fill_empty_listbox() {
    //��sListbox���e
    m_ID_Recipe_NameList->ResetContent();
    m_ID_Recipe_NameList->AddString(_T("ID,Recipe Name"));
    for (const auto& item : lists) {
        m_ID_Recipe_NameList->AddString(item.second[0] + _T(",") + item.second[1]);
    }
}
// �B�zLISTBOX�ƥ�
void CMyDialog::OnLbnSelChangeList(){
	//����Listbox�����Ĥ@�����خɡA�N��ܶ��اאּ�ĤG��
    int sel = m_ID_Recipe_NameList->GetCurSel();
    if (sel>lists.size()) {
        return;
    }
    if (sel == 0 && m_ID_Recipe_NameList->GetCount() > 1) {
        m_ID_Recipe_NameList->SetCurSel(1);
        sel = 1;
    }
    CString index_str;
    m_ID_Recipe_NameList->GetText(sel, index_str);
	int index = _wtoi(index_str);
    for (int i = 0;i<edit_count;++i) {
        Edits[i]->SetWindowText(lists[index][i]);
    }
    Light_Setting_ComboBox->FindStringExact(0, lists[index][6]) >=0? Light_Setting_ComboBox->SetWindowText(lists[index][6]) : Light_Setting_ComboBox->SetWindowText(_T(""));
    Inspection_Setting_ComboBox->FindStringExact(0, lists[index][7])>=0 ? Inspection_Setting_ComboBox->SetWindowText(lists[index][7]) : Inspection_Setting_ComboBox->SetWindowText(_T(""));
}
// �B�zBUTTON�ƥ�
void CMyDialog::ButtonsClicked_Event(UINT ID) {
    switch (ID) {
    case SIZE_FILTER_BUTTON:
        AfxMessageBox(_T("Size_Filter_Button_Clicked"));
        break;
    case DEFECT_TYPE_LABEL_BUTTON:
        AfxMessageBox(_T("Defect_Type_Label_Button_Clicked"));
        break;
    case DEFECT_TYPE_REJECTOR_BUTTON:
        AfxMessageBox(_T("Defect_type_Rejector_Button_Clicked"));
        break;
    case RESERVED_BUTTON:
        AfxMessageBox(_T("Reserved_Button_Clicked"));
        break;
    case INPUT_BUTTON:
        Input_Button_Event();
        break;
    case CANCEL_BUTTON:
        Cancel_Button_Event();
        break;
    case SAVE_BUTTON:
        Write_ini_file_method1();
        Write_ini_file_method2();
        break;
    case CLOSE_BUTTON:
        EndDialog(IDOK);
        break;
    }
}
void CMyDialog::Input_Button_Event() {
    //only for testing, remove later
    cs.Lock();
    std::vector<char> imgBuffer = ReadImageFile(_T("return.bmp"));
    shared_file = shared_memory->Write_Shared_Memory(imgBuffer, imgBuffer.size());
    size_t size = imgBuffer.size();
    Client_Socket->Send(&size, sizeof(size_t));
    UpdateData(TRUE);
    // �ˬd���O�_���ť�
    for (int i = 0; i < edit_count; ++i) {
        Edits[i]->GetWindowText(UI_string[i]);
    }
    Light_Setting_ComboBox->GetWindowText(UI_string[6]);
    Inspection_Setting_ComboBox->GetWindowText(UI_string[7]);
    if (UI_string[0].IsEmpty()) {
        AfxMessageBox(_T("�ť�ID"));
        return;
    }
    m_ID_Recipe_NameList->AddString(UI_string[0] + "," + UI_string[2]);
    auto pos = lists.find(_wtoi(UI_string[0]));
	//�p�GID�w�s�b�A�h��s��ơA�_�h�s�W�@�����
    if (pos != lists.end()) {
        pos->second = UI_string;
    }
    else {
        lists.insert({ _wtoi(UI_string[0]),UI_string});
    }
	//��sListbox���e
    m_ID_Recipe_NameList->ResetContent();
    m_ID_Recipe_NameList->AddString(_T("ID,Recipe Name"));
    fill_empty_listbox();
}
void CMyDialog::Cancel_Button_Event() {
	//���o��ܪ����د���
    CString index_str;
    Edits[0]->GetWindowText(index_str);
	int index = _wtoi(index_str);
	//�R����ܪ�����
    lists.erase(index);
    for (int i = 0;i<edit_count;++i) {
        Edits[i]->SetWindowText(_T(""));
    }
	Light_Setting_ComboBox->SetWindowText(_T(""));
    Inspection_Setting_ComboBox->SetWindowText(_T(""));
    fill_empty_listbox();
}
LRESULT CMyDialog::OnSocketNotify(WPARAM wParam, LPARAM lParam) {
    imgBuffer = shared_memory->Read_Shared_Memory(Client_Socket->GetFileSize());
    // �e�X�Ϥ�
    DrawImageBuffer(share_memory_static, imgBuffer);
    cs.Unlock();
    return TRUE;
}
BOOL CMyDialog::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) {
    if (pCopyDataStruct->dwData == 1)  
    {
        CString receivedText((LPCTSTR)pCopyDataStruct->lpData);
        AfxMessageBox(_T("������: ") + receivedText);
        return TRUE;  // ��ܤw�B�z
    }
    return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}

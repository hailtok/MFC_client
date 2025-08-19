#include<iostream>
#include <fstream>
#include <string>
#include"setting_dialog.h"
#include"MyDialog.h"
#include "MyApp.h"
CMyApp theApp;

BOOL CMyApp::InitInstance()
{
    CWinApp::InitInstance();
    //�B�z�y����ܡAŪ��ini�ɮת�����
    CString ini_file_path = _T(".\\config_ui_language_utf8.ini");
    Setting_Dialog* language_setting = new Setting_Dialog();
    language_setting->SetIniFilePath(ini_file_path);
    if (GetIniLanguage(ini_file_path).empty()) {
        language_setting->DoModal();
    }
    delete language_setting;
    CMyDialog dialog;
	m_pMainWnd = &dialog; 
	dialog.DoModal();
    /*
    CMainDialog maindialog;
    m_pMainWnd = &maindialog;
    maindialog.DoModal();
    */
    return FALSE; // �������ε{��
}


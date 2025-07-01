
// PluginTester.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
    #error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "CommonData.h"
#include "PluginInterface.h"

// CPluginTesterApp: 
// �йش����ʵ�֣������ PluginTester.cpp
//

class CPluginTesterApp : public CWinApp, public ITrafficMonitor
{
public:
    CPluginTesterApp();
    int DPI(int pixel);
    static CString LoadText(UINT id);
    HICON GetIcon(UINT id);

    Language m_language;
    std::wstring m_config_path;
    HICON m_plugin_icon;

private:
    void LoadConfig();
    void SaveConfig() const;

private:
    int m_dpi{ 96 };
    std::map<UINT, HICON> m_icons;

// ��д
public:
    virtual BOOL InitInstance();

// ʵ��

    DECLARE_MESSAGE_MAP()
    afx_msg void OnHelp();

    // ͨ�� ITrafficMonitor �̳�
    int GetAPIVersion() override;
    const wchar_t* GetVersion() override;
    double GetMonitorValue(ITrafficMonitor::MonitorItem item) override;
    const wchar_t* GetMonitorValueString(MonitorItem item, int is_main_window) override;
    void ShowNotifyMessage(const wchar_t* strMsg) override;
    unsigned short GetLanguageId() const override;
    const wchar_t* GetPluginConfigDir() const override;
    int GetDPI(ITrafficMonitor::DPIType type) const override;
    unsigned int GetThemeColor() const override;
};

extern CPluginTesterApp theApp;
#pragma once

#include "resource.h"
// CClientIdDlg 对话框

class CClientIdDlg : public CDialog
{
	DECLARE_DYNAMIC(CClientIdDlg)

public:
	CClientIdDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClientIdDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nClientId;
};

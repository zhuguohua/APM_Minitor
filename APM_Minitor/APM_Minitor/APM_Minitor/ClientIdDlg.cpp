// ClientIdDlg.cpp : 实现文件
//

#include "APM_Minitor/stdafx.h"
#include "ClientIdDlg.h"
#include "afxdialogex.h"


// CClientIdDlg 对话框

IMPLEMENT_DYNAMIC(CClientIdDlg, CDialog)

CClientIdDlg::CClientIdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientIdDlg::IDD, pParent)
	, m_nClientId(0)
{

}

CClientIdDlg::~CClientIdDlg()
{
}

void CClientIdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CLIENT_ID, m_nClientId);
	DDV_MinMaxInt(pDX, m_nClientId, 0, 100000);
}


BEGIN_MESSAGE_MAP(CClientIdDlg, CDialog)
END_MESSAGE_MAP()


// CClientIdDlg 消息处理程序
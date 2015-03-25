#include "stdafx.h"
#include "Gprs.h"
#include "Socket_Client.h"
#include "Main_Frame.h"
#include "Application.h"

BEGIN_MESSAGE_MAP(Class_Main_App, CWinApp)
	ON_COMMAND(ID_BUTTON_RTL, Do_Rtl)
	ON_COMMAND(ID_BUTTON_OPEN_UMBRELLA, Do_Open_Umbrella)
	ON_COMMAND(ID_BUTTON_CONNECT_OR_NOT, DoConnetOrNot)
END_MESSAGE_MAP()

// global variary
Class_Main_App theApp;

Socket_Client sc_Sock_Client;
Gprs gp_Gprs;
volatile BOOL g_bDoConnect;


Class_Main_App::~Class_Main_App()
{
	if (ul_Thread_Handle != 0)
	{
		CloseHandle((HANDLE)ul_Thread_Handle);
	}
}
 
BOOL Class_Main_App::InitInstance()
{
	ul_Thread_Handle = 0;
	un_Thread_Id = 0;
	g_bDoConnect = FALSE;

	m_adm_APM_Data.f_ALT_m = 0.0;
	m_adm_APM_Data.f_AS_m_s = 0.0;
	m_adm_APM_Data.f_Bat_v = 0.0;
	m_adm_APM_Data.f_Cur_a = 0.0;
	m_adm_APM_Data.f_DTL_m = 0.0;
	m_adm_APM_Data.f_DTW_m = 0.0;
	m_adm_APM_Data.f_GS_m_s = 0.0;
	m_adm_APM_Data.f_LAT = 0.0;
	m_adm_APM_Data.f_LNG = 0.0;
	m_adm_APM_Data.f_VS_m_s = 0.0;
	m_adm_APM_Data.n_CWN = 0;
	m_adm_APM_Data.n_Fly_Mode = ~0;
	m_adm_APM_Data.n_NWN = 0;

	m_pMainWnd = new Class_Main_Frame("MAV 数据监视器");
	m_pMainWnd->ShowWindow(m_nCmdShow);
	m_pMainWnd->UpdateWindow();



	Create_New_Thread(Thread_Func);

	return TRUE;
}


void Class_Main_App::Create_New_Thread(P_Thread_Func p_Func)
{
	ul_Thread_Handle = _beginthreadex(NULL, 0, p_Func, 0, 0, &un_Thread_Id);

	return;
}

void Class_Main_App::Update_APM_Data(APM_DATA_MINITOR &adm_APM_Data)
{
	memcpy((void *)(&m_adm_APM_Data), (void *)(&adm_APM_Data), sizeof(APM_DATA_MINITOR));
	m_pMainWnd->Invalidate(0);
	//m_pMainWnd->RedrawWindow();
	//Client_Rect_Repaint();
	//m_pMainWnd->UpdateWindow();
	return;
}

void Class_Main_App::Client_Rect_Repaint(void)
{
	m_pMainWnd->Invalidate(0);
	//((Class_Main_Frame *)m_pMainWnd)->Client_Rect_Repaint();
	//m_pMainWnd->UpdateWindow();
	//AfxMessageBox(_T("!!"));
	return;
}

P_APM_DATA_MINITOR Class_Main_App::Get_APM_Data(void)
{
	return (P_APM_DATA_MINITOR)(&m_adm_APM_Data);
}

void Class_Main_App::Do_Rtl()
{
	//AfxMessageBox(_T("RTL"));
	sc_Sock_Client.Send_Data_Protol(0, GPRS_DATA_CMD_RTL);
	return;
}

void Class_Main_App::Do_Open_Umbrella()
{
	sc_Sock_Client.Send_Data_Protol(0, GPRS_DATA_CMD_OPEN_UMBRELLA);
	return;
}


void Class_Main_App::DoConnetOrNot()
{
	if (g_bDoConnect == TRUE)
	{
		g_bDoConnect = FALSE;
		((Class_Main_Frame *)m_pMainWnd)->m_pButtonConnectOrNot->SetWindowTextW(_T("连接"));
		sc_Sock_Client.NeedCloseSocket();
	}
	else
	{
		g_bDoConnect = TRUE;
		((Class_Main_Frame *)m_pMainWnd)->m_pButtonConnectOrNot->SetWindowTextW(_T("断开"));
	}
	
}

unsigned int __stdcall Thread_Func(void *p)
{
	while (1)
	{
		if (!sc_Sock_Client.Socket_Client_Init(&gp_Gprs))
		{
		}

	connect_server:
		if (g_bDoConnect == FALSE)
		{
			goto connect_server;
		}

		//printf("Begin to Connect\n");
		if (!sc_Sock_Client.Try_Connect_Server("121.42.15.225", 5990))
		{
			goto connect_server;
		}
		
		if (!sc_Sock_Client.Try_Login_Server())
		{
			goto connect_server;
		}
		//printf("Connect Success\n");
		if (sc_Sock_Client.Do_Communication() == 0)
		{
			//printf("Connect close\n");
			//printf("**********************************\n");
			goto connect_server;
		}
	}

	return 0;
}




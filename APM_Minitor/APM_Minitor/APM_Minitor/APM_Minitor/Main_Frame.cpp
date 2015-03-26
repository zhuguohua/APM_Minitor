#include "stdafx.h"
#include "Gprs.h"
#include "Socket_Client.h"
#include "Application.h"
#include "Main_Frame.h"
#include "../resource.h"


BEGIN_MESSAGE_MAP(Class_Main_Frame, CFrameWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

extern Socket_Client sc_Sock_Client;

Class_Main_Frame::Class_Main_Frame(const CString str_Frame_Name)
{
	POINT point_Top_Point, point_Bottom_Point;
	int n_Step = 400;

	m_p_Button_Rtl = NULL;
	m_p_Button_Open_Umbrella = NULL;

	point_Top_Point.x = 300;
	point_Top_Point.y = 200;
	point_Bottom_Point.x = point_Top_Point.x + 400;
	point_Bottom_Point.y = point_Top_Point.y + 400;
	CRect cr_Rect(point_Top_Point, point_Bottom_Point);

	Create(NULL, str_Frame_Name, 13565952UL, cr_Rect);
	Create_Button(50, 320);

	//CRect rect = CRect(100, 100, 100, 100);
	//CFrameWnd *pView = new CFrameWnd();
	//pView->Create(NULL, _T("AA"), WS_CHILD | WS_VISIBLE, rect, this);
	//pView->ShowWindow(SW_SHOW);

	return;
}

Class_Main_Frame::~Class_Main_Frame()
{
	if (m_p_Button_Rtl != NULL)
	{
		delete m_p_Button_Rtl;
	}

	if (m_p_Button_Open_Umbrella != NULL)
	{
		delete m_p_Button_Open_Umbrella;
	}
}

void Class_Main_Frame::Client_Rect_Repaint(void)
{
	CRect rect;
	GetClientRect(&rect);

	InvalidateRect(&rect, 1);

	return;
}

//void Create_Button(CString str_Button_Name, int n_ID, int n_X = 0, int n_Y = 0, int n_Width = 40, int n_Height = 30);
void Class_Main_Frame::Create_Button(int n_X, int n_Y)
{
	int n_Width = 40, n_Height = 30;

	m_p_Button_Rtl = new CButton;
	m_p_Button_Rtl->Create(_T("返航"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(n_X, n_Y, n_X + n_Width, n_Y + n_Height), this, ID_BUTTON_RTL);

	n_X += 60;
	m_p_Button_Open_Umbrella = new CButton;
	m_p_Button_Open_Umbrella->Create(_T("开伞"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(n_X, n_Y, n_X + n_Width, n_Y + n_Height), this, ID_BUTTON_OPEN_UMBRELLA);

	n_X += 60;
	m_pButtonConnectOrNot = new CButton;
	m_pButtonConnectOrNot->Create(_T("连接"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(n_X, n_Y, n_X + n_Width, n_Y + n_Height), this, ID_BUTTON_CONNECT_OR_NOT);

	//n_X += 60;
	//m_pClientIdEdit = new CEdit;
	//m_pClientIdEdit->Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(n_X, n_Y, n_X + n_Width, n_Y + n_Height), this, ID_CLIENT_ID_EDIT);

	m_p_Button_Rtl->ShowWindow(SW_SHOW);
	m_p_Button_Open_Umbrella->ShowWindow(SW_SHOW);
	m_pButtonConnectOrNot->ShowWindow(SW_SHOW);

	//m_pClientIdEdit->ShowWindow(SW_SHOW);
}

void Class_Main_Frame::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	P_APM_DATA_MINITOR p_APM_Data = ((Class_Main_App *)AfxGetApp())->Get_APM_Data();
	int n_X = 10, n_Y = 10;
	int n_X_Step = 20, n_Y_Step = 20;
	CString str;
	CString str1;

	static unsigned int ui32_Time = 0;
	static unsigned int ui32_Count = 0;

	GetClientRect(&rect);

	str = "飞行模式 :";
	str1 = "                     ";
	str = str + str1;
	dc.TextOutW(n_X, n_Y, str);
	str.Format(_T("飞行模式 : %d"), p_APM_Data->n_Fly_Mode);
	str = "飞行模式 : ";
	str1 = P_MODE_LIST[p_APM_Data->n_Fly_Mode];
	dc.TextOutW(n_X, n_Y, str+str1);

	str = "电流(A) :";
	str1 = "                     ";
	str = str + str1;
	dc.TextOutW(n_X, n_Y += n_Y_Step, str);
	str.Format(_T("电流(A) : %3.3f"), p_APM_Data->f_Cur_a );
	dc.TextOutW(n_X, n_Y, str);

	str = "电压(V): ";
	str1 = "                     ";
	str = str + str1;
	dc.TextOutW(n_X, n_Y += n_Y_Step, str);
	str.Format(_T("电压(V): %3.3f"), p_APM_Data->f_Bat_v);
	dc.TextOutW(n_X, n_Y, str);

	str = "垂直速度(m/s) : ";
	str1 = "                     ";
	str = str + str1;
	dc.TextOutW(n_X, n_Y += n_Y_Step, str);
	str.Format(_T("垂直速度(m/s) : %3.2f"), p_APM_Data->f_VS_m_s);
	dc.TextOutW(n_X, n_Y, str);

	str = "空速(m/s) :  ";
	str1 = "                     ";
	str = str + str1;
	dc.TextOutW(n_X, n_Y += n_Y_Step, str);
	str.Format(_T("空速(m/s) : %3.2f"), p_APM_Data->f_AS_m_s);
	dc.TextOutW(n_X, n_Y, str);

	str = "地速(m/s)  :  ";
	str1 = "                     ";
	str = str + str1;
	dc.TextOutW(n_X, n_Y += n_Y_Step, str);
	str.Format(_T("地速(m/s) : %3.2f"), p_APM_Data->f_GS_m_s);
	dc.TextOutW(n_X, n_Y, str);

	str = "当前航点 :  ";
	str1 = "                     ";
	str = str + str1;
	dc.TextOutW(n_X, n_Y += n_Y_Step, str);
	str.Format(_T("当前航点 : %d"), p_APM_Data->n_CWN);
	dc.TextOutW(n_X, n_Y, str);

	str = "下一个航点 :                            ";
	dc.TextOutW(n_X, n_Y += n_Y_Step, str);
	str.Format(_T("下一个航点 : %d"), p_APM_Data->n_NWN);
	dc.TextOutW(n_X, n_Y, str);

	str = "飞机与下一个航点距离(m) :                            ";
	dc.TextOutW(n_X, n_Y += n_Y_Step, str);
	str.Format(_T("飞机与下一个航点距离(m) : %6.2f"), p_APM_Data->f_DTW_m);
	dc.TextOutW(n_X, n_Y, str);

	str = "飞机与起飞点距离(m):                           ";
	dc.TextOutW(n_X, n_Y += n_Y_Step, str);
	str.Format(_T("飞机与起飞点距离(m) : %6.2f"), p_APM_Data->f_DTL_m);
	dc.TextOutW(n_X, n_Y, str);

	str = "飞机当前相对高度(m):                            ";
	dc.TextOutW(n_X, n_Y += n_Y_Step, str);
	str.Format(_T("飞机当前相对高度(m) : %6.2f"), p_APM_Data->f_ALT_m);
	dc.TextOutW(n_X, n_Y, str);

	str = "纬度(度):                                  ";
	dc.TextOutW(n_X, n_Y += n_Y_Step, str);
	str.Format(_T("纬度(度) : %3.6f"), p_APM_Data->f_LAT);
	dc.TextOutW(n_X, n_Y, str);

	str = "经度(度) :                                ";
	dc.TextOutW(n_X, n_Y += n_Y_Step, str);
	str.Format(_T("经度(度) : %3.6f"), p_APM_Data->f_LNG);
	dc.TextOutW(n_X, n_Y, str);

	n_Y += n_Y_Step;
	if (cs_Connect_State.ui16_Recv_New_Server_Hartbeat == 1)
	{
		str = "服务器心跳包状态:                          ";
		dc.TextOutW(n_X, n_Y, str);
		cs_Connect_State.ui16_Recv_New_Server_Hartbeat = 0;
		str.Format(_T("服务器心跳包状态 : 正常"));
		ui32_Time = (unsigned)sc_Sock_Client.Get_Time_Second();
		dc.TextOutW(n_X, n_Y, str);
	}
	else
	{
		if (sc_Sock_Client.Get_Time_Second() - ui32_Time >= 10)
		{
			str = "服务器心跳包状态:                          ";
			dc.TextOutW(n_X, n_Y, str);
			str.Format(_T("服务器心跳包状态 : 超时"));
			dc.TextOutW(n_X, n_Y, str);
		}
	}

	str = "Count:                                ";
	dc.TextOutW(n_X, n_Y += n_Y_Step, str);
	str.Format(_T("Count: %d"), ui32_Count++);
	dc.TextOutW(n_X, n_Y, str);
	
}
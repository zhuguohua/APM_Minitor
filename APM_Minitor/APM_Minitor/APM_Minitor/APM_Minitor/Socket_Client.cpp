#include "stdafx.h"
#include "Main_Frame.h"
#include "Application.h"
#include "Gprs.h"
#include "Socket_Client.h"


#pragma comment(lib, "ws2_32.lib")

char ch_MANUAL_Mode[] = "MANUAL";
char ch_CIRCLE_Mode[] = "CIRCLE";
char ch_STABILIZE_Mode[] = "STABILIZE";
char ch_TRAINING_Mode[] = "TRAINING";
char ch_ACRO_Mode[] = "ACRO";
char ch_FLY_BY_WIRE_A_Mode[] = "FBWA";
char ch_FLY_BY_WIRE_B_Mode[] = "FBWB";
char ch_CRUISE_Mode[] = "CRUISE";
char ch_AUTO_Mode[] = "AUTO";
char ch_RTL_Mode[] = "RTL";
char ch_LOITER_Mode[] = "LOITER";
char ch_GUIDED_Mode[] = "GUIDED";
char ch_INITIALISING_Mode[] = "INITIALISING";
char ch_NOT_Mode[] = "Not Fly Mode";

char *P_MODE_LIST[] =
{
	 ch_MANUAL_Mode,
	 ch_CIRCLE_Mode,
	 ch_STABILIZE_Mode,
	 ch_TRAINING_Mode,
	 ch_ACRO_Mode,
	 ch_FLY_BY_WIRE_A_Mode,
	 ch_FLY_BY_WIRE_B_Mode,
	 ch_CRUISE_Mode,
	 ch_NOT_Mode,
	 ch_NOT_Mode,
	 ch_AUTO_Mode,
	 ch_RTL_Mode,
	 ch_LOITER_Mode,
	 ch_NOT_Mode,
	 ch_NOT_Mode,
	 ch_GUIDED_Mode,
	 ch_INITIALISING_Mode
};

char ch_Hartbeat[] = { 'S', 13, '0', 'C', '0', '0', '0', '0', 'E', 'R', 'R' };


volatile CONNECT_STATE cs_Connect_State;
char _ch_Buff_[1000];

BOOL Socket_Client::Socket_Client_Init(void)
{
	
	return Socket_Client_Init(NULL);
}

BOOL Socket_Client::Socket_Client_Init(Gprs *p_Gprs)
{
	so_Sock_Client = NULL;
	ul64_Second = 0;
	ul64_Def_Time = 0;

	m_bNeedCloseConnection = FALSE;
	m_nClientId = 1;

	p_ch_Buff = _ch_Buff_;

	this->p_Gprs = p_Gprs;

	w_Version_Requested = MAKEWORD(1, 1);
	n_Error = WSAStartup(w_Version_Requested, &wsa_Data);

	if (n_Error != 0)
	{
		return FALSE;
	}

	if (LOBYTE(wsa_Data.wVersion) != 1 || HIBYTE(wsa_Data.wVersion) != 1)
	{
		WSACleanup();
		return FALSE;
	}


	return TRUE;
}


BOOL Socket_Client::Try_Connect_Server(const char *p_ch_Server_IP, int n_COM)
{
	so_Sock_Client = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN sa_Addr_Srv;
	sa_Addr_Srv.sin_addr.S_un.S_addr = inet_addr(p_ch_Server_IP);
	sa_Addr_Srv.sin_family = AF_INET;
	sa_Addr_Srv.sin_port = htons(n_COM);


	if (connect(so_Sock_Client, (SOCKADDR *)&sa_Addr_Srv, sizeof (SOCKADDR)) != 0)
	{
		return FALSE;
	}

	FD_ZERO(&fd_Sock);
	FD_SET(so_Sock_Client, &fd_Sock);

	return TRUE;
}


int Socket_Client::Do_Communication(void)
{
	char ch_Hartbeat[] = { 'S', 13, MINITOR_HEARTBEAT_PACKET, 1, 1, 0, '5', '5', 'E', 'R', 'R' };
	ULONG64 ul64_Second_1 = 0;

	if (p_Gprs == NULL)
		return -1;

	GetLocalTime(&sys_Time);
	ul64_System_Time_S = sys_Time.wSecond + sys_Time.wMinute * 60 + sys_Time.wHour * 3600 + sys_Time.wDay * 24 * 60 * 60;
	ul64_Second = ul64_System_Time_S;
	while (1)
	{
		if (m_bNeedCloseConnection == TRUE)
		{
			m_bNeedCloseConnection = FALSE;
			Close_Socket_Client();
			break;
		}

		GetLocalTime(&sys_Time);
		ul64_System_Time_S = sys_Time.wSecond + sys_Time.wMinute * 60 + sys_Time.wHour * 3600 + sys_Time.wDay * 24 * 60 * 60;
		ul64_Def_Time = ul64_System_Time_S - ul64_Second;

		if (ul64_System_Time_S - ul64_Second_1 >= 1)
		{
			ul64_Second_1 = ul64_System_Time_S;
			((Class_Main_App *)AfxGetApp())->Client_Rect_Repaint();
		}

		if (ul64_Def_Time >= 3)
		{
			ul64_Second = ul64_System_Time_S;
			Send(ch_Hartbeat, 11);
		}

		if (Wait_For_Data(3) > 0)
		{
			n_Ret = Recv(ch_Buff, 1000);
			if (n_Ret <= 0)
			{
				//AfxMessageBox(_T("SER "));
				Close_Socket_Client();
				break;
			}
			
			if (p_Gprs->Try_Capture_Gprs_Data_Packet(ch_Buff, n_Ret))
			{
				//Send("OK\r\n", strlen("OK\r\n") + 1);
			}
			else
			{
				//Send("NO DATA\r\n", strlen("NO DATA\r\n") + 1);
			}

			((Class_Main_App *)AfxGetApp())->Update_APM_Data(*(p_Gprs->Get_APM_Data()));
		}//end if (Wait_For_Data(3) > 0)
		else
		{
			//printf("\nNO DATA RECV\n");
		}
	}

	return 0;
}

int Socket_Client::Wait_For_Data(int n_Timeout_s)
{
	fd_Read = fd_Sock;
	tv_Timeout.tv_sec = n_Timeout_s;
	tv_Timeout.tv_usec = 0;
	return select(0, &fd_Read, NULL, NULL, &tv_Timeout);
}


ULONG64 Socket_Client::Get_Time_Second(void)
{
	GetLocalTime(&sys_Time);
	ul64_System_Time_S = sys_Time.wSecond + sys_Time.wMinute * 60 + sys_Time.wHour * 3600 + sys_Time.wDay * 24 * 60 * 60;

	return ul64_System_Time_S;
}

int Socket_Client::Send(const char *p_ch_Buff_Src, int n_Length)
{
	return send(so_Sock_Client, p_ch_Buff_Src, n_Length, 0);
}

int Socket_Client::Recv(char *p_ch_Buff_Dst, int n_Length)
{
	return recv(so_Sock_Client, p_ch_Buff_Dst, n_Length, 0);
}

void Socket_Client::NeedCloseSocket(void){
	m_bNeedCloseConnection = TRUE;
}
void Socket_Client::Close_Socket_Client(void)
{
	closesocket(so_Sock_Client);
	return;
}

void Socket_Client::Socket_Cleanup(void)
{
	WSACleanup();
	return;
}


int Socket_Client::Send_Data_Protol(unsigned int ui32_Data_To_Send, GPRS_DATA_FLAG gpf_Flag)
{
	BYTE ch_Index = 0;
	char ch_Send[100];
	uint16_t ui16_checksum = 0;

	ch_Send[ch_Index++] = 'S';
	ch_Send[ch_Index++] = (BYTE)gpf_Flag;
	ch_Send[ch_Index++] = (BYTE)(ui32_Data_To_Send);
	ch_Send[ch_Index++] = (BYTE)(ui32_Data_To_Send >> 8);
	ch_Send[ch_Index++] = (BYTE)(ui32_Data_To_Send >> 16);
	ch_Send[ch_Index++] = (BYTE)(ui32_Data_To_Send >> 24);
	ch_Send[ch_Index++] = (BYTE)ui16_checksum;
	ch_Send[ch_Index++] = (BYTE)(ui16_checksum >> 8);
	ch_Send[ch_Index++] = 'E';

	return Send(ch_Send, ch_Index);
}


BOOL Socket_Client::Try_Login_Server(void)
{
	int n_Ret = 0;
	char ch[4] = {0};
	ch[0] = m_nClientId;// client id
	ch[1] = STATION_CLIENT_TYPE;// client type
	int *p = (int *)ch;

	//CString str;
	//str.Format(_T("%d"), m_nClientId);
	//AfxMessageBox(str);

	int n_Count = 0;

	p_Gprs->Ready_To_Login_Server();
	p_Gprs->SetCurrentClientId(m_nClientId);

	while (1)
	{
		Send_Data_Protol(*p, GPRS_DATA_LOGIN);

		if (Wait_For_Data(3) > 0)
		{
			n_Ret = Recv(p_ch_Buff, 1000);
			if (n_Ret <= 0)
			{
				Close_Socket_Client();
				return FALSE;
			}

			if (p_Gprs->Try_Capture_Gprs_Data_Packet(p_ch_Buff, n_Ret))
			{
				if (p_Gprs->Check_Login_Server())
				{
					return TRUE;
				}
				else
				{
					AfxMessageBox(_T("Close socket"));
					Close_Socket_Client();
					return FALSE;
				}
			}
		}//end if (Wait_For_Data(3) > 0)
		else
		{
			if (n_Count++ >= 5)
			{
				Close_Socket_Client();
				return FALSE;
			}
		}
	}
	return FALSE;
}















#ifndef _SOCKET_CLIENT_H_
#define _SOCKET_CLIENT_H_

#include "APM_Define.h"

extern char *P_MODE_LIST[];

typedef struct _IPHeader
{
	UCHAR iphVerLen;
	UCHAR ipTOS;
	USHORT ipLength;
	USHORT ipID;
	USHORT ipFlags;
	UCHAR ipTTL;
	UCHAR ipProtocol;
	USHORT ipChecksum;
	ULONG ipSource;
	ULONG ipDestination;
}IP_Header, *P_IP_Header;

typedef struct _ICMP_Header
{
	UCHAR uc_ICMP_Type;
	UCHAR uc_ICMP_Code;
	USHORT us_ICMP_Checksum;

	USHORT us_ICMP_ID;
	USHORT us_ICMP_Sequence;
	ULONG	ul_ICMP_Timestamp;
}ICMP_Header, *P_ICMP_Header;


class Socket_Client
{
public:
	Socket_Client(){}
	~Socket_Client(){}

private:
	SOCKET so_Sock_Client;

	WORD w_Version_Requested;
	WSADATA wsa_Data;
	int n_Error;
	int n_Ret;

	Gprs *p_Gprs;

	fd_set fd_Sock, fd_Read;
	struct timeval tv_Timeout;

	SYSTEMTIME sys_Time;
	ULONG64 ul64_Second;
	ULONG64 ul64_Def_Time;
	ULONG64 ul64_System_Time_S;

	volatile BOOL m_bNeedCloseConnection;

	char *p_ch_Buff;
	#define ch_Buff p_ch_Buff

public:
	volatile int m_nClientId;

public:
	BOOL Socket_Client_Init(Gprs *p_Gprs);
	BOOL Socket_Client_Init(void);
	BOOL Try_Connect_Server(const char *p_ch_Server_IP, int n_COM);
	void Close_Socket_Client(void);
	void NeedCloseSocket(void);
	void Socket_Cleanup(void);
	int Do_Communication(void);
	int Send(const char *p_ch_Buff_Src, int n_Length);
	int Recv(char *p_ch_Buff_Dst, int n_Length);
	int Send_Data_Protol(unsigned int ui32_Data_To_Send, GPRS_DATA_FLAG gpf_Flag);
	BOOL Try_Login_Server(void);
	ULONG64 Get_Time_Second(void);

	int Wait_For_Data(int n_Timeout_s);

};



#endif


#ifndef _GPRS_H_
#define _GPRS_H_

#include "APM_Define.h"

class Gprs
{
public:
	Gprs();
	~Gprs(){}

private:
	GPRS_DATA_PACKET_INFO m_gdps_Gprs_Packet_Info;
	APM_DATA_MINITOR adm_APM_Data;
	BOOL b_Login_Success;
	// the id received from sever
	int m_nClientIdRecv;
	int m_nCurrentClientId;

public:
	BOOL Try_Capture_Gprs_Data_Packet(char *p_ch_Src, int n_Count);
	P_GPRS_DATA_PACKET_INFO Get_Gprs_Data_Packet_Info(void);
	BOOL Parse_Gprs_Data(void);
	float Get_Target(const uint8_t *p_ch_Data, int n_Param);
	BOOL Check_Login_Server(void);
	void Ready_To_Login_Server(void);

	int GetClientIdRecv(void);
	void SetCurrentClientId(int nClientId);

	P_APM_DATA_MINITOR Get_APM_Data(void)
	{
		return &adm_APM_Data;
	}
};


#endif




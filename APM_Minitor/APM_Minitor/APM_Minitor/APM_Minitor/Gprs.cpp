#include "stdafx.h"
#include "Gprs.h"

Gprs::Gprs()
{
	adm_APM_Data.f_ALT_m = 0.0;
	adm_APM_Data.f_AS_m_s = 0.0;
	adm_APM_Data.f_Bat_v = 0.0;
	adm_APM_Data.f_Cur_a = 0.0;
	adm_APM_Data.f_DTL_m = 0.0;
	adm_APM_Data.f_DTW_m = 0.0;
	adm_APM_Data.f_GS_m_s = 0.0;
	adm_APM_Data.f_LAT = 0.0;
	adm_APM_Data.f_LNG = 0.0;
	adm_APM_Data.f_VS_m_s = 0.0;
	adm_APM_Data.n_CWN = 0;
	adm_APM_Data.n_Fly_Mode = ~0;
	adm_APM_Data.n_NWN = 0;

	b_Login_Success = 0;

	return;
}


BOOL Gprs::Try_Capture_Gprs_Data_Packet(char *p_ch_Src, int n_Count)
{
	int i = 0;
	char ch_Read;
	BOOL b_Recv_New;

	b_Recv_New = FALSE;

	for (i = 0; i < n_Count; i++)
	{
		ch_Read = p_ch_Src[i];
		if (!m_gdps_Gprs_Packet_Info.ui8_Recv_First)
		{
			if (ch_Read == GPRS_DATA_START_CHARACTER)
			{
				m_gdps_Gprs_Packet_Info.ui8_Recv_First = 1;
				m_gdps_Gprs_Packet_Info.ui8_Recv_End = 0;
				m_gdps_Gprs_Packet_Info.ch_Gprs_Data_Buff[m_gdps_Gprs_Packet_Info.ui8_Gprs_Data_Buff_Index] = ch_Read;
			}
			continue;
		}

		if (!m_gdps_Gprs_Packet_Info.ui8_Find_E && !m_gdps_Gprs_Packet_Info.ui8_Find_ESC && ch_Read == GPRS_DATA_ESC)
		{
			m_gdps_Gprs_Packet_Info.ui8_Find_ESC = 1;
			continue;
		}

		m_gdps_Gprs_Packet_Info.ui8_Gprs_Data_Buff_Index++;
		if (m_gdps_Gprs_Packet_Info.ui8_Gprs_Data_Buff_Index >= GPRS_DATA_PACKET_BUFF_SIZE)
		{
			i--;
			m_gdps_Gprs_Packet_Info.ui8_Gprs_Data_Buff_Index = 0;
			m_gdps_Gprs_Packet_Info.ui8_Recv_End = 0;
			m_gdps_Gprs_Packet_Info.ui8_Recv_First = 0;
			m_gdps_Gprs_Packet_Info.ui8_Find_E = 0;
			continue;
		}

		if (m_gdps_Gprs_Packet_Info.ui8_Find_ESC)
		{
			m_gdps_Gprs_Packet_Info.ui8_Find_ESC = 0;
			m_gdps_Gprs_Packet_Info.ch_Gprs_Data_Buff[m_gdps_Gprs_Packet_Info.ui8_Gprs_Data_Buff_Index] = ch_Read;
			continue;
		}

		if (ch_Read == GPRS_DATA_START_CHARACTER && !m_gdps_Gprs_Packet_Info.ui8_Find_E)
		{
			m_gdps_Gprs_Packet_Info.ui8_Gprs_Data_Buff_Index = 0;
			m_gdps_Gprs_Packet_Info.ch_Gprs_Data_Buff[m_gdps_Gprs_Packet_Info.ui8_Gprs_Data_Buff_Index] = ch_Read;
			continue;
		}

		if (m_gdps_Gprs_Packet_Info.ui8_Recv_First)
		{
			m_gdps_Gprs_Packet_Info.ch_Gprs_Data_Buff[m_gdps_Gprs_Packet_Info.ui8_Gprs_Data_Buff_Index] = ch_Read;
			if (!m_gdps_Gprs_Packet_Info.ui8_Find_E && ch_Read == GPRS_DATA_END_CHARACTER)
			{
				m_gdps_Gprs_Packet_Info.ui8_Find_E = 1;
				continue;
			}

			if (m_gdps_Gprs_Packet_Info.ui8_Find_E && ch_Read != GPRS_DATA_END_CHARACTER)
			{
				i--;
				m_gdps_Gprs_Packet_Info.ui8_Gprs_Data_Buff_Index = 0;
				m_gdps_Gprs_Packet_Info.ui8_Recv_End = 1;
				m_gdps_Gprs_Packet_Info.ui8_Recv_First = 0;
				m_gdps_Gprs_Packet_Info.ui8_Find_E = 0;

				Parse_Gprs_Data();

				b_Recv_New = TRUE;
			}
		}// end if (n_Recv_First && !n_Recv_End)
	}// for (j = 0; j < (int)i16_Bytes; j++)

	return b_Recv_New;
}



P_GPRS_DATA_PACKET_INFO Gprs::Get_Gprs_Data_Packet_Info(void)
{
	return &m_gdps_Gprs_Packet_Info;
}

BOOL Gprs::Parse_Gprs_Data(void)
{
	P_GPRS_DATA_PACKET p_ch_Buff = (P_GPRS_DATA_PACKET)m_gdps_Gprs_Packet_Info.ch_Gprs_Data_Buff;
	P_GPRS_LOGIN_DATA_PACKET pLDP;

	switch (p_ch_Buff->ch_Flag)
	{
	case GPRS_DATA_FLY_MODE:
		adm_APM_Data.n_Fly_Mode = (int)Get_Target(p_ch_Buff->ui8_Data, 1);
		//printf_s("\nRecv Fly Mode: %d\n", adm_APM_Data.n_Fly_Mode);
		break;
	case GPRS_DATA_CUR: //NEXT WAYPOINT NO.
		adm_APM_Data.f_Cur_a = Get_Target(p_ch_Buff->ui8_Data, 1000);
		break;
	case GPRS_DATA_BAT:
		adm_APM_Data.f_Bat_v = Get_Target(p_ch_Buff->ui8_Data, 1000);
		break;
	case GPRS_DATA_VS: //NEXT WAYPOINT NO.
		adm_APM_Data.f_VS_m_s = Get_Target(p_ch_Buff->ui8_Data, 1000);
		break;
	case GPRS_DATA_AS:
		adm_APM_Data.f_AS_m_s = Get_Target(p_ch_Buff->ui8_Data, 1000);
		break;
	case GPRS_DATA_GS: //NEXT WAYPOINT NO.
		adm_APM_Data.f_GS_m_s = Get_Target(p_ch_Buff->ui8_Data, 1000);
		break;
	case GPRS_DATA_CWN:
		adm_APM_Data.n_CWN = (int)Get_Target(p_ch_Buff->ui8_Data, 1);
		break;
	case GPRS_DATA_NWN: //NEXT WAYPOINT NO.
		adm_APM_Data.n_NWN = (int)Get_Target(p_ch_Buff->ui8_Data, 1);
		break;
	case GPRS_DATA_DTW:
		adm_APM_Data.f_DTW_m = Get_Target(p_ch_Buff->ui8_Data, 1000);
		break;
	case GPRS_DATA_DTL: //NEXT WAYPOINT NO.
		adm_APM_Data.f_DTL_m = Get_Target(p_ch_Buff->ui8_Data, 1000);
		break;
	case GPRS_DATA_ALT: //NEXT WAYPOINT NO.
		adm_APM_Data.f_ALT_m = Get_Target(p_ch_Buff->ui8_Data, 1000);
		break;
	case GPRS_DATA_LAT: //NEXT WAYPOINT NO.
		adm_APM_Data.f_LAT = Get_Target(p_ch_Buff->ui8_Data, 1000000);
		break;
	case GPRS_DATA_LNG: //NEXT WAYPOINT NO.
		adm_APM_Data.f_LNG = Get_Target(p_ch_Buff->ui8_Data, 1000000);
		break;
	case GPRS_DATA_HARTBEAT:
		b_Login_Success = 1;
		cs_Connect_State.ui16_Recv_New_Server_Hartbeat = 1;
		//printf_s("\nRecv a Hartbeat\n");
		//return TRUE;
		break;
	case GPRS_DATA_LOGIN:
		pLDP = (P_GPRS_LOGIN_DATA_PACKET)p_ch_Buff;
		m_nClientIdRecv = pLDP->ui8_Client_Id;
		if (pLDP->ui8_Client_Id != m_nCurrentClientId)
		{
			b_Login_Success = 0;
		}
		else
		{
			b_Login_Success = 1;
		}
		
		break;
	default:
		break;
	}

	return FALSE;
}


int Gprs::GetClientIdRecv(void)
{
	return m_nClientIdRecv;
}

void Gprs::SetCurrentClientId(int nClientId)
{
	m_nCurrentClientId = nClientId;
}

void Gprs::Ready_To_Login_Server(void)
{
	b_Login_Success = 0;
}

BOOL Gprs::Check_Login_Server(void)
{
	return b_Login_Success;
}

float Gprs::Get_Target(const uint8_t *p_ch_Data, int n_Param)
{
	int n_Temp;
	float f_Temp;

	n_Temp = 0;
	n_Temp |= p_ch_Data[3];
	n_Temp <<= 8;
	n_Temp |= p_ch_Data[2];
	n_Temp <<= 8;
	n_Temp |= p_ch_Data[1];
	n_Temp <<= 8;
	n_Temp |= p_ch_Data[0];

	f_Temp = (float)(n_Temp / (0.0 + n_Param));

	return f_Temp;
}







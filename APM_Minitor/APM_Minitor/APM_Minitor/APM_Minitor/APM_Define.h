#ifndef _APM_DEFINE_H_
#define _APM_DEFINE_H_

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif
#ifndef uint16_t
typedef unsigned short uint16_t;
#endif

#define GPRS_DATA_ESC 'A'
#define GPRS_DATA_START_CHARACTER  'S'
#define GPRS_DATA_END_CHARACTER  'E'

#define GPRS_DATA_PACKET_SIZE 9
#define GPRS_DATA_PACKET_BUFF_SIZE  GPRS_DATA_PACKET_SIZE + 2

enum Heartbeat_Packet_Type
{
	SERVER_HEARTBEAT_PACKET = 0,
	MINITOR_HEARTBEAT_PACKET,
	APM_HEARTBEAT_PACKET,
};

typedef enum _CLIENT_TYPE_
{
	NO_CLIENT_TYPE = 0,
	STATION_CLIENT_TYPE,
	APM_CLIENT_TYPE
}CLIENT_TYPE, P_CLIENT_TYPE;

enum FlightMode {
	MANUAL = 0,
	CIRCLE = 1,
	STABILIZE = 2,
	TRAINING = 3,
	ACRO = 4,
	FLY_BY_WIRE_A = 5,
	FLY_BY_WIRE_B = 6,
	CRUISE = 7,
	AUTO = 10,
	RTL = 11,
	LOITER = 12,
	GUIDED = 15,
	INITIALISING = 16
};

enum GPRS_DATA_FLAG
{
	GPRS_DATA_FLY_MODE = 0,
	GPRS_DATA_CUR,
	GPRS_DATA_BAT,
	GPRS_DATA_VS,
	GPRS_DATA_AS,
	GPRS_DATA_GS,
	GPRS_DATA_CWN, // CURRENT WAYPOINT NO.
	GPRS_DATA_NWN,//NEXT WAYPOINT NO.
	GPRS_DATA_DTW,//DISTANCE TO WAYPOINT
	GPRS_DATA_DTL,//DISTANCE TO LAUNCH
	GPRS_DATA_ALT,//ALTITUDE
	GPRS_DATA_LAT,
	GPRS_DATA_LNG,
	GPRS_DATA_HARTBEAT,
	GPRS_DATA_LOGIN,
	GPRS_DATA_CMD_RTL,
	GPRS_DATA_CMD_OPEN_UMBRELLA
};

typedef struct _APM_DATA_MINITOR_
{
	int n_Fly_Mode;
	float f_Cur_a;
	float f_Bat_v;
	float f_VS_m_s;
	float f_AS_m_s;
	float f_GS_m_s;
	int n_CWN;
	int n_NWN;
	float f_DTW_m;//DISTANCE TO WAYPOINT
	float f_DTL_m;//DISTANCE TO LAUNCH
	float f_ALT_m;//ALTITUDE
	float f_LAT;
	float f_LNG;
}APM_DATA_MINITOR, *P_APM_DATA_MINITOR;

typedef struct _GPRS_DATA_PACKET_
{
	char ch_Start;
	uint8_t ch_Flag;
	uint8_t ui8_Data[4];
	uint16_t ui16_Checksum;
	char ch_Stop;
}GPRS_DATA_PACKET, *P_GPRS_DATA_PACKET;

typedef struct _GPRS_LOGIN_DATA_
{
	char ch_Start;
	uint8_t ui8_Data_Flag;
	// data
	uint8_t ui8_Client_Id;
	uint8_t ui8_Client_Type;
	uint8_t ui8_Reserve[2];
	// ==============
	uint16_t ui16_Checksum;
	char ch_Stop;
}GPRS_LOGIN_DATA_PACKET, *P_GPRS_LOGIN_DATA_PACKET;

typedef struct _GPRS_DATA_PACKET_INFO_
{
	uint8_t ui8_Recv_End : 1;
	uint8_t ui8_Recv_First : 1;
	uint8_t ui8_Find_E : 1;
	uint8_t ui8_Find_ESC : 1;
	uint8_t ui8_Reserve : 4;
	uint8_t ui8_Gprs_Data_Buff_Index;
	char ch_Gprs_Data_Buff[GPRS_DATA_PACKET_BUFF_SIZE];
}GPRS_DATA_PACKET_INFO, *P_GPRS_DATA_PACKET_INFO;


typedef struct _CONNECT_STATE_
{
	uint16_t ui16_Server_Hartbeat_State : 2;
	uint16_t ui16_APM_Hartbeat_State : 2;
	uint16_t ui16_Recv_New_Server_Hartbeat : 1;
}CONNECT_STATE, *P_CONNECT_STATE;

extern volatile CONNECT_STATE cs_Connect_State;

#endif

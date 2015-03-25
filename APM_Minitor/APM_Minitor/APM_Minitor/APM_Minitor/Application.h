#ifndef _APPLICATION_
#define _APPLICATION_

#include "APM_Define.h"

typedef unsigned int __stdcall P_Thread_Func(void *);
unsigned int __stdcall Thread_Func(void *p);

class Class_Main_App : public CWinApp
{
public:
	Class_Main_App(){}
	~Class_Main_App();

private:
	unsigned long ul_Thread_Handle;
	unsigned int un_Thread_Id;
	Class_Main_App *p_this_App;

public:
	volatile APM_DATA_MINITOR m_adm_APM_Data;

public:
	virtual BOOL InitInstance(void);
	virtual void Create_New_Thread(P_Thread_Func p_Func);
	void Update_APM_Data(APM_DATA_MINITOR &adm_APM_Data);
	P_APM_DATA_MINITOR Get_APM_Data(void);

	void Client_Rect_Repaint(void);
	void Do_Rtl(void);
	void Do_Open_Umbrella(void);
	void DoConnetOrNot(void);

	DECLARE_MESSAGE_MAP()
};


#endif

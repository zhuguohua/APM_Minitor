#ifndef _MAIN_FRAME_
#define _MAIN_FRAME_

#define ID_BUTTON_RTL 10000
#define ID_BUTTON_OPEN_UMBRELLA 10001
#define ID_BUTTON_CONNECT_OR_NOT 10002

class Class_Main_Frame : public CFrameWnd
{
public:
	Class_Main_Frame()
	{}
	Class_Main_Frame(const CString str_Frame_Name);
	~Class_Main_Frame();
private:
	CButton *m_p_Button_Rtl;
	CButton *m_p_Button_Open_Umbrella;
public:
	CButton *m_pButtonConnectOrNot;
public:
	virtual void OnPaint();
	void Client_Rect_Repaint(void);
	void Create_Button(int n_X = 0, int n_Y = 0);

	DECLARE_MESSAGE_MAP()
};


#endif



#pragma once

#include <FL/Fl.H>
#include <FL/gl.h>
#include <FL/glu.h>
#include <FL/glut.H> 
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Tiled_Image.H>
#include <FL/Fl_Image_Surface.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Browser.H>
#include <FL/fl_ask.H>

#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Input.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Table.H>
#include "utils/intvec.h"
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_Image.H>
#include <FL/Fl_Radio_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Secret_Input.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Radio_Round_Button.H>
#include <FL/Fl_Native_File_Chooser.H>
#include "include/macrodef.h"

class CTskTimer;
 
class CNewTaskFrame:public Fl_Window{
public:
    CNewTaskFrame(int w, int h, const char* title= 0);
    CNewTaskFrame(int x, int y, int w, int h, const char* title = 0);
    void Init();
    //void draw();
    //int  handle(int event);
    void BuildTskEditWnd(IN CTskTimer* pTskTimer);
	Fl_Radio_Round_Button* m_pCurrTimeRdo
		, *m_psolarCalendarRdo
		, *m_beforeRdo
		, *m_freqMRdo,*m_freqDRdo
		,*m_afterRdo;
	Fl_Input* m_pTskNameIpt,
		*m_offsetDIpt,
		*m_offsetHIpt,
		*m_offsetMIpt,
		*m_offsetSIpt,
		*m_intvlMonIpt,
		*m_intvlDIpt,
		*m_intvlHIpt,
		*m_intvlMinuIpt,
		*m_intvlSIpt,
		*m_exefnIpt,
		*m_execmdlineIpt
		;

	Fl_Choice *m_yearChoice,*m_monChoice,*m_dayChoice,*m_actionChoice;
	Fl_Input* m_hourIpt,*m_minIpt,*m_secIpt,*m_exectimesIpt;
};

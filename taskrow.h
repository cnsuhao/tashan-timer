#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Input.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Table.H>
#include <FL/Fl_Group.H>
#include <FL/fl_ask.H>

class CTskTimer;

#define LINEHEIGHT 35

class CTaskRow:public Fl_Group
{
 public:
    CTaskRow(int,int,int,int, const char * = 0, bool bDrawFrame =false);
    ~CTaskRow();
    void Build();

    static XPoint unitWHs[];
    static XPoint unitSpaces[];
    void UpdateUI();
    int BuildRow(const int pX, const int pY, const int nLineH,struct CBtnStruc* );
    int handle(int event);
    bool m_bMouseEnter;
    bool m_bLeftBtnDown;
    virtual void draw();

    Fl_Image* m_icons;
    CTskTimer* m_pTskTimer;
};



#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Input.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Table.H>
#include <FL/Fl_Group.H>

class CTaskRow:public Fl_Group
{
 public:
    CTaskRow(int,int,int,int, const char * = 0, bool bDrawFrame =false);
    ~CTaskRow();
    void Build();

    static XPoint unitWHs[];
    static XPoint unitSpaces[];
    int BuildRow(const int pX, const int pY, const int nLineH,struct CBtnStruc* );
};




#include "newtask_frame.h"
#include "Fl_Drable_Window.h"
#include <FL/Fl_Radio_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Secret_Input.H>
#include <FL/Fl_Multiline_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Toggle_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Color_Chooser.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Radio_Round_Button.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <time.h>
# include <stdio.h>
# include <stdlib.h>

#ifdef WIN32
#include <stdlib.h>	// atoi
#endif /*WIN32*/

void ok_cb(Fl_Widget* wid, void *data)
{
    Fl_Group* fg  = (Fl_Group*)wid->parent();    
    Fl_Group* wnd = (Fl_Group*)fg->parent();
    if(fg->find(wid)==0)
    {
        //cancel
        wnd->hide();
    }else
    {
        //gene xml
    }
}

void startdate_cb(Fl_Widget* wid, void *data)
{
    Fl_Group* fgBtns  = (Fl_Group*)wid->parent();
    Fl_Group* fg=(Fl_Group*)fgBtns->parent();
    Fl_Group* wnd = (Fl_Group*)fg->parent();
    Fl_Group* pDateOffset = (Fl_Group*)wnd->child(wnd->find(fg)+1);
    
    time_t now;
    struct tm *timenow;
    time(&now);
    //time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now
    timenow   =   localtime(&now);

    Fl_Choice *oY,*oMon,*oD;
    int nObj = fg->find(fgBtns);
    int nYear = timenow->tm_year;
    oY = (Fl_Choice*)fg->child(nObj+1); oY->value(nYear);
    oMon = (Fl_Choice*)fg->child(nObj+3);  oMon->value(timenow->tm_mon);  
    oD = (Fl_Choice*)fg->child(nObj+5);oD->value(timenow->tm_mday-1);

    Fl_Input *oH=(Fl_Input*)pDateOffset->child(1),
           *oMin=(Fl_Input*)pDateOffset->child(3),
             *oS=(Fl_Input*)pDateOffset->child(5);
    
    char chs[20];
    
    sprintf(chs,"%d",timenow->tm_hour);
    oH->value(chs);
    sprintf(chs,"%d",timenow->tm_min);
    oMin->value(chs);
    sprintf(chs,"%d",timenow->tm_sec);
    oS->value(chs);
    
    if(fgBtns->find(wid)==0)
    {
        //当前
        oY->deactivate(),oMon->deactivate(),oD->deactivate();
        oH->deactivate(),oMin->deactivate(),oS->deactivate();        
    }else
    {
        oY->activate(),oMon->activate(),oD->activate();
        oH->activate(),oMin->activate(),oS->activate();      
    }
    ((Fl_Button*)wid)->value(1);    
}

void doscheme(Fl_Choice *c, void *) {

}

void actionChoice(Fl_Choice *c, void *)
{
    Fl_Group* pGrp =  (Fl_Group*)c->parent();
    Fl_Group* pChildGrp = (Fl_Group*)pGrp->child(2);
    if(strcmp(c->text(c->value()),"指定应用")==0)
    {
         pChildGrp->activate();
    }else
    {
        pChildGrp->deactivate();
    }
    //fl_alert(pChildGrp->label());
}

void fileChoose(Fl_Widget* wid, void *data)
{
    Fl_Group* fg = (Fl_Group*)wid->parent();
 Fl_Native_File_Chooser fnfc;
 fnfc.title("Pick a file");
 fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
#ifdef WIN32
 fnfc.filter("程序\t*.exe\n"
             "歌曲\t*.{mp3,rm,wma,mp4}\n");
#else
 fnfc.filter("程序\t*\n"
             "歌曲\t*.{mp3,rm,wma,mp4}");
#endif
 fnfc.directory("/home/lixing/");
 int nObj = fg->find(wid);
 Fl_Input* fnIpt =  (Fl_Input*)fg->child(nObj-1);
 if ( fnfc.show() == 0 && fnfc.count()>0)
 {
    fnIpt->value(fnfc.filename(0));
 }
}

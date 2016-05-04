
#include "newtask_frame.h"
#include "tsktimer/TskTimer.h"
#include "tsktimer/TskTimerMgr.h"
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
#include "utils/utils.h"

void CNewTaskFrame::UpdateTsk()
{   
    //fl_assert(m_pTskTimer);
    CTskTimer* pTskTimer  = m_pTskTimer;
    if(!pTskTimer) return;
    
	CParams*   pTimeParam = pTskTimer->m_pTime;
    Fl_Button* pRdo=m_pCurrTimeRdo;
    TCHAR chBuf[200];
    
	time_t now;
	struct tm *tmNow;
	time(&now);
	tmNow   =   localtime(&now);

    int ntmp=0;
	int Y=0,M=0,D=0,y= tmNow->tm_year,m= tmNow->tm_mon,d=tmNow->tm_mday,h=tmNow->tm_hour,minu=tmNow->tm_min,sec=tmNow->tm_sec;    
	if(pTimeParam)
	{
        if(m_pCurrTimeRdo->value()==1)
        {
            pTimeParam->chType=0;
            Y = y;M=m;D=d;
        }
        else
        {
            pTimeParam->chType=1;
            //pTimeParam->m_pParam0 's format similar to  2013.2.24
            Y = 1900+m_yearChoice->value();
            M = m_monChoice->value();
            D = m_dayChoice->value();
            
            h    = atoi(m_hourIpt->value()); CTskTimer::ValidTime(h,    ETT_HOUR);
            minu = atoi(m_minIpt->value());  CTskTimer::ValidTime(minu, ETT_MINU);
            sec  = atoi(m_secIpt->value());  CTskTimer::ValidTime(sec,  ETT_SEC);   
        }
        if(!pTimeParam->m_pParam0) pTimeParam->m_pParam0 = (TCHAR*)malloc(MAX_PATH*sizeof(TCHAR));
        sprintf(pTimeParam->m_pParam0, "%d.%d.%d",Y,M,D);
        
        if(!pTimeParam->m_pParam1) pTimeParam->m_pParam1 = (TCHAR*)malloc(MAX_PATH*sizeof(TCHAR));        
        sprintf(pTimeParam->m_pParam1, "%d:%d:%d",h,minu,sec);         
    }//if(pTimeParam)
    
	CParams*   pOffsetParam  = pTskTimer->m_pOffset;
	if(pOffsetParam)
	{
        if(m_beforeRdo->value()==1)
        {
            pOffsetParam->chType = 0;
        }else pOffsetParam->chType = 1;
        
        //pOffsetParam->m_pParam0 's format similar to  day.h:m:sec
        D    = atoi(m_offsetDIpt->value()); D %= 10000;//CTskTimer::ValidTime(D,    ETT_DAY);
        h    = atoi(m_offsetHIpt->value()); CTskTimer::ValidTime(h,    ETT_HOUR);
        minu = atoi(m_offsetMIpt->value()); CTskTimer::ValidTime(minu, ETT_MINU);
        sec  = atoi(m_offsetSIpt->value()); CTskTimer::ValidTime(sec,  ETT_SEC);
        
        if(!pOffsetParam->m_pParam0) pOffsetParam->m_pParam0 = (TCHAR*)malloc(MAX_PATH*sizeof(TCHAR));  
        sprintf(pOffsetParam->m_pParam0, "%d.%d:%d:%d",D,h,minu,sec);
    }
    
    CParams*   pFreqParam  = pTskTimer->m_pFreq;
	if(pFreqParam)
	{
        if(!pFreqParam->m_pParam0) pFreqParam->m_pParam0=(TCHAR*)malloc(MAX_PATH*sizeof(TCHAR));
        pFreqParam->m_pParam0[0]=0;
        ntmp = atoi(m_exectimesIpt->value());
        if(ntmp>=-1)
            sprintf(pFreqParam->m_pParam0, "%d", ntmp);

        if(!pFreqParam->m_pParam1) pFreqParam->m_pParam1 = (TCHAR*)malloc(MAX_PATH*sizeof(TCHAR));
        pFreqParam->m_pParam1[0]=0;
        if(m_freqMRdo->value()==1)
        {
            pFreqParam->chType = 0;
            CParams::CopyParam(pFreqParam->m_pParam1, m_intvlMonIpt->value());
        }else
        {
            pFreqParam->chType = 1;
            //pFreqParam->m_pParam1 's format similar to  day.h:m:sec
            D    = atoi(m_intvlDIpt->value()); D %= 10000;//CTskTimer::ValidTime(D,    ETT_DAY);
            h    = atoi(m_intvlHIpt->value());    CTskTimer::ValidTime(h,    ETT_HOUR);
            minu = atoi(m_intvlMinuIpt->value()); CTskTimer::ValidTime(minu, ETT_MINU);
            sec  = atoi(m_intvlSIpt->value());    CTskTimer::ValidTime(sec,  ETT_SEC);
            
            sprintf(pFreqParam->m_pParam1, "%d.%d:%d:%d",D,h,minu,sec);            
        }
    }

    ntmp = _tcslen(m_pTskNameIpt->value())+1;
    CParams::CopyParam(pTskTimer->m_pTskName, m_pTskNameIpt->value());
	if(pTskTimer->m_pActions)
	{
		for(int i=0,n=pTskTimer->m_nActionsCount;i<n;i++)
		{
			CParams* pAcParam = pTskTimer->m_pActions[i];
            pAcParam->chType = m_actionChoice->value();
            
            CParams::CopyParam(pAcParam->m_pParam0, m_exefnIpt->value());
            CParams::CopyParam(pAcParam->m_pParam1, m_execmdlineIpt->value());
            //pAcParam->m_pParam2 place exe related fn
            if(!pAcParam->m_pParam2) pAcParam->m_pParam2 = (TCHAR*)malloc(MAX_PATH*sizeof(TCHAR));
            if(!nsYLX::CUtil::GetRelatedName(pAcParam->m_pParam0, pAcParam->m_pParam2, MAX_PATH*sizeof(TCHAR),0))
            {
                pAcParam->m_pParam2[0]=0;
            }
            
			break;//only dispaly the first task action
		}//for
	}//if(pTskTimer->m_nActionsCount>0)    
    
}//UpdateTsk

void ok_cb(Fl_Widget* wid, void *data)
{
    Fl_Group* fg  = (Fl_Group*)wid->parent();    
    Fl_Group* wnd = (Fl_Group*)fg->parent();
    CNewTaskFrame* pThis = (CNewTaskFrame*)data;
    pThis->m_nRslt = -1;
    if(fg->find(wid)==1)
    {
        //cancel
    }else
    {
        //gene xml
        pThis->UpdateTsk();
        pThis->m_nRslt = 0;
    }
    wnd->hide();    
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
    
    if(strcmp(c->text(c->value()),"指定应用")==0
     ||strcmp(c->text(c->value()),"播放音乐")==0)
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

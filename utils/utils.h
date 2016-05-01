
#pragma once
#include "FL/Fl_Image.H"
#include "FL/Fl_RGB_Image.H"
#include "../include/stdafx.h"

namespace nsYLX
{
class CUtil
{
 public:
    static void ClearMemForSplit(IN TCHAR **&rtn, IN int rtnCount);
    static int  Split(OUT TCHAR **&rtn,  IN  const int rtnCount, IN const TCHAR * sz, IN const TCHAR * tsTag);
    static Fl_Image* copy(Fl_Image* srcImg, int x, int y, int W, int H);
};
 
}


#pragma once
#include "FL/Fl_Image.H"
#include "FL/Fl_RGB_Image.H"

class CUtil
{
 public:
    static Fl_RGB_Image* cutImg(Fl_Image* src, int x, int y, int w, int h);
    static Fl_Image* copy(Fl_Image* srcImg, int x, int y, int W, int H);
};


#include "utils.h"
#include <FL/Fl_Image.H>
#include <FL/Fl_Tiled_Image.H>
#include <FL/Fl_Image_Surface.H>
#include <assert.h>
#  include <FL/Fl_Export.H>

#  include <stdio.h>
#  include <stdarg.h>
#  include <string.h>
#  ifdef HAVE_STRINGS_H
#    include <strings.h>
#  endif /* HAVE_STRINGS_H */
#  include <ctype.h>


Fl_Image* CUtil::copy(Fl_Image* srcImg, int x, int y, int W, int H)
{
    Fl_RGB_Image   *new_image;	// New RGB image
    uchar		   *new_array;	// New array for image data
    const uchar* data_ = (const uchar*)srcImg->data()[0];
    int D  = srcImg->d();
    int LD = D*srcImg->w();
    if(x+W> srcImg->w())
    {
        x = srcImg->w() - W;
    }
    if(y+H > srcImg->h())
    {
        y = srcImg->h() - H;
    }
    const uchar *src = (const uchar *)data_+(x*D + y*LD);

    if (data_)
    {
      // Make a copy of the image data and return a new Fl_RGB_Image...
      new_array = new uchar[W * H * D];
      uchar *dst = new_array;
      int wd = W*D, wld = srcImg->w()*D;

        for (int dy=0; dy<H; dy++)
        {
          memcpy(dst, src, wd);
          src += wld;
          dst += wd;
        }

      new_image = new Fl_RGB_Image(new_array, W, H, D);
      new_image->alloc_array = 1;

      return new_image;
    } else
        return new Fl_RGB_Image(src, W, H, D, srcImg->ld()); 
}

Fl_RGB_Image* CUtil::cutImg(Fl_Image* src, int x, int y, int w, int h)
{
//Fl_Image* img = src->copy(x,y,w,h);
    
    return 0; 
}

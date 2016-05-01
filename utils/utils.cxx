
#include "../include/stdafx.h"
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

namespace nsYLX
{
    
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

void CUtil::ClearMemForSplit(IN TCHAR **&rtn, IN int rtnCount)
{
	//ASSERT(rtn!=NULL && rtnCount>0);
	if (!rtn  || rtnCount<=0)
	{
		return ;
	}
	for (int i=0;i<rtnCount;i++)
	{
		TCHAR* p = rtn[i];
		if (p)
		{
			delete[] p;p=0;
		}

	}
	delete[] rtn;rtn=0;
}
//因为_tcstok函数的关系，该函数用在多线程下必须加锁
int CUtil::Split(OUT TCHAR **&rtn, IN const int rtnMaxCount, IN  const TCHAR * szCp, IN const TCHAR * tsTag)
{
    assert(rtn==NULL && rtnMaxCount>0);
	if (rtn  || rtnMaxCount<=0 || !szCp || !tsTag)
	{
		return 0;
	}
	int nLen = _tcslen(szCp)+1;

	TCHAR *sz = new TCHAR[nLen];
	_tcscpy(sz,szCp);

	nLen = (_tcslen(tsTag)+1)*2;
	TCHAR* seps = new TCHAR[nLen];//;
	_tcscpy(seps, tsTag );
    
	TCHAR* token = _tcstok( sz , seps ); 
 
	rtn  = new TCHAR*[rtnMaxCount];
	for (int i=0;i<rtnMaxCount;i++)
	{
		rtn[i] = 0;
	}
	int i=0;
	int nTmp=0;
	while( token != NULL )
	{
		if (rtn)
		{
			if (i>=rtnMaxCount)
			{
				break;
			}
			nTmp = _tcslen(token)+1;
			rtn[i] = new TCHAR[nTmp];
			_tcscpy(rtn[i],token);
		}	
		token = _tcstok( NULL, seps );
		i++;
	}//
 
	delete[] seps;  
	delete[] sz;
	return i;
}//void CUtil::Split(ATL::CString rtn[],  ATL::CString sz, ATL::CString tsTag)

}//namespace nsYlx

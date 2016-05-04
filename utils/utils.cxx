
#include "../include/stdafx.h"
#include "utils.h"
#include <FL/Fl_Image.H>
#include <FL/Fl_Tiled_Image.H>
#include <FL/Fl_Image_Surface.H>
#include <assert.h>
#include <FL/Fl_Export.H>
#include <FL/Fl_Window.H>   //CenterWnd

#  include <stdio.h>
#  include <stdarg.h>
#  include <string.h>
#  ifdef HAVE_STRINGS_H
#    include <strings.h>
#  endif /* HAVE_STRINGS_H */
#  include <ctype.h>

#include<unistd.h>
#include<dirent.h>
#include<string>
namespace nsYLX
{

void CUtil::CenterWnd(Fl_Window* pWnd, Fl_Window* pParent)
{
    if(!pWnd || !pParent) return ;
    
    float nScreenW = pParent->w();
    float nScreenH = pParent->h();
    int winW=pWnd->w(),winH=pWnd->h();
    int dx = (nScreenW-winW)/2 + pParent->x();
    int dy = (nScreenH-winH)/2 + pParent->y();
    
    pWnd->position(dx,dy);    
}
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
    
//becasue use _tcstok，so this func isn't thread safe.
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
    
	void CUtil::GetModulePath(IN OUT TCHAR* ptchCurrPath,IN int nMaxLen,IN HINSTANCE hInst)
	{
		if (!ptchCurrPath)
		{
			return;
		}
		memset(ptchCurrPath, 0, nMaxLen*sizeof(TCHAR));
        
		#if defined(_WIN32) || defined(__WINDOWS__)
			GetModuleFileName(hInst, ptchCurrPath, nMaxLen - 2);
		#else
			int n = readlink("/proc/self/exe", ptchCurrPath, nMaxLen-1);
			if(n<0)
			{
				return ;
			}
			std::string path = ptchCurrPath;
			int nLen = path.rfind('/')+1;
			ptchCurrPath[nLen]=0;
		#endif 

		for (int i= _tcslen(ptchCurrPath)-1;i>=0;i--)
		{
			if (ptchCurrPath[i]==_T(':') )
			{
				ptchCurrPath[i+1]=_T('/');
				ptchCurrPath[i+2]=_T('\0');
				break;
			}else if(ptchCurrPath[i]==_T('\\') || ptchCurrPath[i]==_T('/'))
			{
				ptchCurrPath[i+1]=_T('\0');
				break;
			}
		}//for

		CUtil::AddTailBackslash(ptchCurrPath);
	}

	//通过输入相对路径名称，比对当前目录，获得对应的全路径
	bool CUtil::ParseRelatedName(OUT TCHAR* pthsFullName,  IN  TCHAR* ptchsRelated , IN  int nFullNameBufSize, IN TCHAR* ptchsCurrPath)
	{
		bool bRtn = false;
		bool bNeedDelCurrPath =false;
		if (!pthsFullName || !ptchsRelated)
		{
			return false;
		}

		int nLen = _tcslen(ptchsRelated);
		if (nLen==0)
		{
			return false;
		}
		memset(pthsFullName,0,nFullNameBufSize);

		if (ptchsCurrPath==0)
		{
			ptchsCurrPath = new TCHAR[MAX_PATH];
			CUtil::GetModulePath(IN OUT ptchsCurrPath,IN   MAX_PATH,IN  NULL);
			bNeedDelCurrPath = true;
		}


		TCHAR* pFile = ptchsRelated;
		int nPos =0;
		if (pFile[0]==_T('.') && pFile[1] == _T('/'))
		{
			//pFile = pFile +2;
			nPos +=2;
		}else if ( pFile[0] == _T('/'))
		{
			//pFile = pFile +1;
			nPos +=1;
		}

		//获得../的次数
		int nLevel =0;
		for (int i= nPos ;i<nLen-2;i++)
		{
			if (  pFile[i]==_T('.')
				&&pFile[i+1]==_T('.')
				&&pFile[i+2]==_T('/')
				)
			{
				nLevel++;
				i+=2;
			}
		}//for

		GetParentPath(IN ptchsCurrPath, OUT pthsFullName, IN  nFullNameBufSize, IN  nLevel);

		_tcscat(pthsFullName, &ptchsRelated[nPos+3*nLevel]);

		if (bNeedDelCurrPath)
		{
			delete[] ptchsCurrPath;
		}
		return true;
	}
    
	//通过输入全路径名称，比对当前目录，获得对应的相对路径
	bool CUtil::GetRelatedName(IN TCHAR* pthsFullName,   OUT TCHAR* ptchsRelated , IN  int nRelatedBufSize, IN TCHAR* ptchsCurrPath)
	{
		bool bRtn = false;
        int nLevel=0;
		bool bNeedDelCurrPath =false;
		if (!pthsFullName || !ptchsRelated || nRelatedBufSize<=0)
		{
			return false;
		}

		memset(ptchsRelated,0,nRelatedBufSize);

		if (ptchsCurrPath==0)
		{
			ptchsCurrPath = new TCHAR[MAX_PATH];
			CUtil::GetModulePath(IN OUT ptchsCurrPath,IN   MAX_PATH,IN  NULL);
			bNeedDelCurrPath = true;
		}
       
		int nSrcLen = _tcslen(pthsFullName);
		int nCurrLen = _tcslen(ptchsCurrPath);

		if (nSrcLen==0 || nCurrLen<3)
		{
			if (bNeedDelCurrPath)
			{
				delete[] ptchsCurrPath;ptchsCurrPath=0;
			}
			return false;
		}

		CUtil::ToLower(IN OUT pthsFullName, IN nSrcLen);
		CUtil::ToLower(IN OUT ptchsCurrPath, IN nCurrLen);

		CUtil::ChangeBackSlash(IN OUT pthsFullName, IN nSrcLen);
		//首先判断，全路径文件名，和当前目录，是不是同根，不同根的话，没有取相对路径的必要。
		//使用状态机 
		//c : / xx / x.../xx.exe
		//0 1 2 3  2 3   2  4 
		int nState = 0,nPos=0;
		TCHAR* pFull = pthsFullName;
		while (*pFull !=_T('\0'))
		{
            switch(nState)
			{
			case 0:
				if ( !((*pFull >_T('a') && *pFull < _T('z') )|| (*pFull > _T('A') && *pFull < _T('Z') )))
				{
                    goto _BAD;//非法字符
				}
				if (*pFull != ptchsCurrPath[0])
				{
					goto _BAD;//不同根的话，没有取相对路径的必要。
				}
				nState =1;
				break;
			case 1:
				if (*pFull != _T(':'))
				{
					goto _BAD;//非法字符
				}
				nState =2;
				break;
			case 2:
				while (*pFull != _T('/') && *pFull!=_T('\0'))
				{
					if (*pFull != ptchsCurrPath[nPos] ||  ptchsCurrPath[nPos]==_T('\0') )
					{
						goto _ENDWHILE;
					}
					pFull++;
					nPos++;
				} 
				break; 
			}
			pFull++;
			nPos++;
		}

		fl_assert(*pFull!= _T('/'));
		//分析相对路径的格式有如下几种：
		/*
		/xx/xxx
		./xx/xx
		../../...
		./../...
		*/
_ENDWHILE://中途退出WHILE循环
	    nLevel=0;
		if (nCurrLen <= nPos)
		{
			//全路径为当前路径的子目录
			fl_assert(nPos == nCurrLen);
			//那么我寻找从pFull开始，到pthsFullName结束，到底有几个'/'
			while (*pFull && *pFull !=_T('/'))
			{
				pFull--;
				nPos--;
			}
			_tcscpy(ptchsRelated, pFull);//ptchsRelated 存放了 /xx/x...格式
		}else
		{
			while (*pFull && *pFull !=_T('/'))
			{
				//回退到字符'/'的位置
				pFull--;
				nPos--;
			}
			//那么我寻找从nPos开始，到ptchsCurrPath结束，到底有几个'/'
			pFull = &ptchsCurrPath[nPos];
			while (*pFull)
			{
				if (*pFull == _T('/'))
				{
					nLevel++;
				}
				pFull++;
			}

			//nLevel代表..的个数
			for (int i=0;i<nLevel;i++)
			{
				ptchsRelated[i*3] =_T('.');
				ptchsRelated[i*3+1] =_T('.');
				ptchsRelated[i*3+2] =_T('/');
			}
			pFull = &pthsFullName[nPos];
			_tcscpy(ptchsRelated, pFull+1);//ptchsRelated 存放了 ../../......./xx/x...格式
		}

_BAD:
		if (bNeedDelCurrPath)
		{
			delete[] ptchsCurrPath;
		}

		return bRtn;
	}

	void CUtil::AddTailBackslash(IN OUT TCHAR* szCurrentPath)
	{
		if (!szCurrentPath)
		{
			return;
		}
 
		int nLen = _tcslen(szCurrentPath);
		if (nLen==0)
		{
			return ;
		}

		for (int i=0;i<nLen;i++)
		{
			if (szCurrentPath[i]==_T('\\'))
			{
				szCurrentPath[i]=_T('/');
			}
		}

		if (szCurrentPath[nLen-1]!='/')
		{
			//会造成潜在的内存越界访问，小心一点
			szCurrentPath[nLen]='/';
		}
	}//
	void CUtil::ChangeRBackSlash(IN OUT TCHAR* szCurrentPath, IN int nLen)
	{
		if (!szCurrentPath)
		{
			return;
		}

		for (int i=0;i<nLen;i++)
		{
			if (szCurrentPath[i]==_T('/'))
			{
				szCurrentPath[i]=_T('\\');
			}
		}

	}// void CUtil::ChangeRBackSlash(IN OUT TCHAR* szCurrentPath);

	void CUtil::ToLower(IN OUT TCHAR* szCurrentPath, IN int nLen)
	{
		if (!szCurrentPath)
		{
			return;
		}

		for (int i=0;i<nLen;i++)
		{
			szCurrentPath[i]=_totlower(szCurrentPath[i]); 
		}
	}

	void CUtil::ToUpper(IN OUT TCHAR* szCurrentPath, IN int nLen)
	{
		if (!szCurrentPath)
		{
			return;
		}

		for (int i=0;i<nLen;i++)
		{ 
			szCurrentPath[i]=_totupper(szCurrentPath[i]); 
		}
	}

	void CUtil::ChangeBackSlash(IN OUT TCHAR* szCurrentPath, IN int nLen)
	{
		if (!szCurrentPath)
		{
			return;
		}

		for (int i=0;i<nLen;i++)
		{
			if (szCurrentPath[i]==_T('\\'))
			{
				szCurrentPath[i]=_T('/');
			}
		}

	}// void CUtil::ChangeBackSlash(IN OUT TCHAR* szCurrentPath);
    
	int CUtil::GetParentPath(IN const TCHAR* ptchsPath, OUT TCHAR* pOutPath, IN int nOutPathSize, IN int nLevel)
	{
		int nRtn =0;
		if (!ptchsPath)
		{
			return nRtn;
		}

		int nLen = _tcslen(ptchsPath);
		if (0==nLen || !pOutPath || nOutPathSize<=0 || nLevel<0)
		{
			return 0;
		}

		TCHAR *pTchsTmp = new TCHAR[nLen+1];
		_tcscpy(pTchsTmp, ptchsPath);

		ChangeBackSlash(pTchsTmp, nLen);

		int nTmp = _tcslen(pTchsTmp);

		int nIndex  = nTmp-1;
		for (int i= nTmp-1;i>=0;i--)
		{
			if (pTchsTmp[i] == _T('/'))
			{
				if (nLevel-- == 0)
				{
					nIndex  = i;
					break;
				}
			}
		}

		memset(pOutPath, 0, nOutPathSize);
		_tcsncpy(pOutPath,pTchsTmp, nIndex+1);

		nRtn = nIndex+1;

		delete[] pTchsTmp;

		return nRtn;
	}//    
}//namespace nsYlx

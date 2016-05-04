
#pragma once
#include "FL/Fl_Image.H"
#include "FL/Fl_RGB_Image.H"
#include "../include/stdafx.h"

class Fl_Window;

namespace nsYLX
{
class CUtil
{
 public:
    static void ClearMemForSplit(IN TCHAR **&rtn, IN int rtnCount);
    static int  Split(OUT TCHAR **&rtn,  IN  const int rtnCount, IN const TCHAR * sz, IN const TCHAR * tsTag);
    static Fl_Image* copy(Fl_Image* srcImg, int x, int y, int W, int H);
    static bool ParseRelatedName(OUT TCHAR* pthsFullName, IN  TCHAR* ptchsRelated , IN  int nFullNameBufSize,  IN TCHAR* ptchsCurrPath=0);
    static bool GetRelatedName(IN TCHAR* pthsFullName,    OUT TCHAR* ptchsRelated , IN  int nRelatedBufSize,   IN TCHAR* ptchsCurrPath=0);

	static void ToLower(IN OUT TCHAR* szCurrentPath, IN int nLen);
	static void ToUpper(IN OUT TCHAR* szCurrentPath, IN int nLen);
	static void ChangeBackSlash(IN OUT TCHAR* szCurrentPath, IN int nLen);
	static void ChangeRBackSlash(IN OUT TCHAR* szCurrentPath, IN int nLen);
    static void AddTailBackslash(IN OUT TCHAR* szCurrentPath);
    static void GetModulePath(IN OUT TCHAR* ptchCurrPath,IN int nMaxLen,IN HINSTANCE hInst=NULL);
    
	//nLevel代表所要返回的上级目录层数，默认参数nLevel=1，表示获取上一级目录，nLevel=2，表示获取上一级的上一级目录，以此类推
	//返回值表示的是返回的目录长度，0表示失败。pOutPath需要调用者分配和释放内存
	static int GetParentPath(IN const TCHAR* ptchsPath, OUT TCHAR* pOutPath, IN int nOutPathSize, IN int nLevel=1);
    static void CenterWnd(Fl_Window* pWnd, Fl_Window* pParent);
};
 
}

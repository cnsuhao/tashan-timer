

#ifdef _WIN32
#include <windows.h>
#elif defined(__LINUX__)
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <inttypes.h>
#include <locale.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>
#endif // WIN32
#include "Calendar.h"


#if defined(WIN32) && defined(_DEBUG)
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif



namespace nsYLX
{

	CCalendar::CCalendar(void)
	{
	}

	CCalendar::~CCalendar(void)
	{
	}


	//http://bbs.csdn.net/topics/60066802

	#define BEGINYEAR 1900
	#define YEARCOUNT 150
	struct LUNARINDEX //农历大小月数据索引结构，数据时间自1900年至2050年
	{
		long  Days;   // 天数,表示该农历年1月1日距公历元年元月1日的天数
		short Mon13;  // 该数据的0至12位分别标明农历1至13月的大小,含闰月
		short Leap;   // 该年的农历闰月值,15表示无闰月。
	} LunarIndex[] =
	{
		{693626, 0X16D2,  8}, {694010, 0X0752, 15}, {694364, 0X0EA5, 15},
		{694719, 0X164A,  5}, {695102, 0X064B, 15}, {695456, 0X0A9B, 15},
		{695811, 0X1556,  4}, {696195, 0X056A, 15}, {696549, 0X0B59, 15},
		{696904, 0X1752,  2}, {697288, 0X0752, 15}, {697642, 0X1B25,  6},
		{698026, 0X0B25, 15}, {698380, 0X0A4B, 15}, {698734, 0X14AB,  5},
		{699118, 0X02AD, 15}, {699472, 0X056B, 15}, {699827, 0X0B69,  2},
		{700211, 0X0DA9, 15}, {700566, 0X1D92,  7}, {700950, 0X0E92, 15},
		{701304, 0X0D25, 15}, {701658, 0X1A4D,  5}, {702042, 0X0A56, 15},
		{702396, 0X02B6, 15}, {702750, 0X15B5,  4}, {703135, 0X06D4, 15},
		{703489, 0X0EA9, 15}, {703844, 0X1E92,  2}, {704228, 0X0E92, 15},
		{704582, 0X0D26,  6}, {704965, 0X052B, 15}, {705319, 0X0A57, 15},
		{705674, 0X12B6,  5}, {706058, 0X0B5A, 15}, {706413, 0X06D4, 15},
		{706767, 0X0EC9,  3}, {707151, 0X0749, 15}, {707505, 0X1693,  7},
		{707889, 0X0A93, 15}, {708243, 0X052B, 15}, {708597, 0X0A5B,  6},
		{708981, 0X0AAD, 15}, {709336, 0X056A, 15}, {709690, 0X1B55,  4},
		{710075, 0X0BA4, 15}, {710429, 0X0B49, 15}, {710783, 0X1A93,  2},
		{711167, 0X0A95, 15}, {711521, 0X152D,  7}, {711905, 0X0536, 15},
		{712259, 0X0AAD, 15}, {712614, 0X15AA,  5}, {712998, 0X05B2, 15},
		{713352, 0X0DA5, 15}, {713707, 0X1D4A,  3}, {714091, 0X0D4A, 15},
		{714445, 0X0A95,  8}, {714828, 0X0A97, 15}, {715183, 0X0556, 15},
		{715537, 0X0AB5,  6}, {715921, 0X0AD5, 15}, {716276, 0X06D2, 15},
		{716630, 0X0EA5,  4}, {717014, 0X0EA5, 15}, {717369, 0X064A, 15},
		{717722, 0X0C97,  3}, {718106, 0X0A9B, 15}, {718461, 0X155A,  7},
		{718845, 0X056A, 15}, {719199, 0X0B69, 15}, {719554, 0X1752,  5},
		{719938, 0X0B52, 15}, {720292, 0X0B25, 15}, {720646, 0X164B,  4},
		{721030, 0X0A4B, 15}, {721384, 0X14AB,  8}, {721768, 0X02AD, 15},
		{722122, 0X056D, 15}, {722477, 0X0B69,  6}, {722861, 0X0DA9, 15},
		{723216, 0X0D92, 15}, {723570, 0X1D25,  4}, {723954, 0X0D25, 15},
		{724308, 0X1A4D, 10}, {724692, 0X0A56, 15}, {725046, 0X02B6, 15},
		{725400, 0X05B5,  6}, {725784, 0X06D5, 15}, {726139, 0X0EA9, 15},
		{726494, 0X1E92,  5}, {726878, 0X0E92, 15}, {727232, 0X0D26, 15},
		{727586, 0X0A56,  3}, {727969, 0X0A57, 15}, {728324, 0X14D6,  8},
		{728708, 0X035A, 15}, {729062, 0X06D5, 15}, {729417, 0X16C9,  5},
		{729801, 0X0749, 15}, {730155, 0X0693, 15}, {730509, 0X152B,  4},
		{730893, 0X052B, 15}, {731247, 0X0A5B, 15}, {731602, 0X155A,  2},
		{731986, 0X056A, 15}, {732340, 0X1B55,  7}, {732725, 0X0BA4, 15},
		{733079, 0X0B49, 15}, {733433, 0X1A93,  5}, {733817, 0X0A95, 15},
		{734171, 0X052D, 15}, {734525, 0X0AAD,  4}, {734909, 0X0AB5, 15},
		{735264, 0X15AA,  9}, {735648, 0X05D2, 15}, {736002, 0X0DA5, 15},
		{736357, 0X1D4A,  6}, {736741, 0X0D4A, 15}, {737095, 0X0C95, 15},
		{737449, 0X152E,  4}, {737833, 0X0556, 15}, {738187, 0X0AB5, 15},
		{738542, 0X15B2,  2}, {738926, 0X06D2, 15}, {739280, 0X0EA5,  6},
		{739664, 0X0725, 15}, {740018, 0X064B, 15}, {740372, 0X0C97,  5},
		{740756, 0X0CAB, 15}, {741111, 0X055A, 15}, {741465, 0X0AD6,  3},
		{741849, 0X0B69, 15}, {742204, 0X1752, 11}, {742588, 0X0B52, 15},
		{742942, 0X0B25, 15}, {743296, 0X1A4B,  6}, {743680, 0X0A4B, 15},
		{744034, 0X04AB, 15}, {744388, 0X055B,  5}, {744772, 0X05AD, 15},
		{745127, 0X0B6A, 15}, {745482, 0X1B52,  2}, {745866, 0X0D92, 15},
		{746220, 0X1D25,  7}, {746604, 0X0D25, 15}, {746958, 0X0A55, 15},
		{747312, 0X14AD,  5}, {747696, 0X04B6, 15}, {748050, 0X05B5, 15},
		{748405, 0X1DAA,  3}
	};


	// 判断输入的公历年份是否为闰年,是闰年则函数返回1,否则返回0
	int CCalendar::IsLeapYear( int Year )
	{
		if ( Year % 4 )return 0;

		if ( Year % 100 )return 1;

		if ( Year % 400 )return 0;
		else return 1;
	}

	//---------------------------------------------------------------------------
	// 计算输入公历年份的1月1日为从公元1年1月1日以来的第几天,返回天数
	long CCalendar::YearDays( int Year )
	{
		Year--;
		return( Year*365 + Year / 4 - Year / 100 + Year / 400 + 1 );
	}

	//---------------------------------------------------------------------------
	// 计算输入的公历日期为从公元1年1月1日以来的第几天,返回总天数
	long CCalendar::TotalDays( int Year, int Month, int Day )
	{
		const int MonthDays[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		long Days = 0L;
		int i;

		if ( Month <= 0 )Month = 1;

		if ( Month > 12 )
		{
			Month--;
			Year += Month / 12;
			Month %= 12;
			Month++;
		}

		for ( i = 1; i < Month; i++ )Days += MonthDays[i];

		if ( IsLeapYear( Year ) && Month > 2 )Days += 1;

		Days += YearDays( Year );
		return Days + Day - 1;
	}


	//输入农历年月日，获得公历的年月日,如果是闰月则输入是加一，如农历五月M1=4,则闰五月M1=5,如此类推。。。
	bool CCalendar::GetSolarDate(int Y1, int M1, int D1, int& Y2, int& M2, int& D2 )
	{
		Y2 = Y1;
		M2=0;
		D2=0;

		long TDays = 0;

		short MBit[13] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096};

		int Sum, Lest, i, Offset;
		Offset = Y1 - BEGINYEAR;

		if ( Offset < 0 || Offset > YEARCOUNT ) return false ;

		TDays = LunarIndex[Offset].Days;

		for ( Sum = i = 0; i < M1; i++ )
		{
			if ( ( LunarIndex[Offset].Mon13&MBit[i] ) != 0 )
			{
				Sum += 30; 
			}
			else
			{
				Sum += 29; 
			}
		}

		if ( i == 13 )  return false;

		TDays += Sum;
		TDays += D1-1;
		
		AbsToDate( TDays, Y2, M2, D2 );

		//考虑闰月情况,如果输入的是农历闰5月，则参数M1= 6,所以这里要加 1
		if ( LunarIndex[Offset].Leap <= M1 +1)
		{
			//M2++;
		}

		return true;
	}//

	//---------------------------------------------------------------------------
	// 判断输入的公历年月日是否存在,如果存在则返回1,否则返回0
	int CCalendar::IsLegalDate( int Year, int Month, int Day )
	{
		const int MonthDays[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

		if ( Year <= 0 || Month <= 0 || Day <= 0 )return 0;

		if ( IsLeapYear( Year ) && Month == 2 && Day <= 29 )return 1;

		if ( Month <= 12 && Day <= MonthDays[Month] )return 1;

		return 0;
	}

	//---------------------------------------------------------------------------
	// 返回指定的公历某年某月的天数，返回0表示无效年月
	int CCalendar::GetSolarMonthDays( int Year, int Month )
	{
		const int MonthDays[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

		if ( IsLegalDate( Year, Month, 1 ) == 0 )return 0;

		if ( Month == 2 && IsLeapYear( Year ) )return 29;

		return MonthDays[Month];
	}
	//---------------------------------------------------------------------------
	// 计算输入公历的第一个日期到第二个日期需经历的天数,返回天数
	long CCalendar::SolarMidDays( int Y1, int M1, int D1, int Y2, int M2, int D2 )
	{
		return( TotalDays( Y2, M2, D2 ) - TotalDays( Y1, M1, D1 ) );
	}
	//---------------------------------------------------------------------------
	// 计算输入农历的第一个日期到第二个日期需经历的天数,返回天数
	//long LunarMidDays(int Y1,int M1,int D1,int Y2,int M2,int D2)
	//{ if(GetSolar(Y1,M1,D1)==0)return 0;
	//if(GetSolar(Y2,M2,D2)==0)return 0;
	//return SolarMidDays(Y1,M1,D1,Y2,M2,D2);
	//}

	//---------------------------------------------------------------------------
	// 输入公历年月日,返回农历年月日,函数返回1表示成功,0表示失败
	// 如果M2返回负值,则说明是农历的闰月,月份值为其绝对值.
	int CCalendar::GetLunar( int Y1, int M1, int D1, int &Y2, int &M2, int &D2 )
	{
		short MBit[13] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096};
		long TDays;
		int Sum, Lest, i, Offset;
		Offset = Y1 - BEGINYEAR;

		if ( Offset < 0 || Offset > YEARCOUNT )return 0;

		TDays = TotalDays( Y1, M1, D1 );
		Y2 = Y1;

		if ( LunarIndex[Offset].Days > TDays )
		{
			if ( Offset == 0 )return 0;

			Offset--;
			Y2--;
		}

		Lest = ( int )( TDays - LunarIndex[Offset].Days );

		for ( Sum = i = 0; i < 13; i++ )
		{
			if ( ( LunarIndex[Offset].Mon13&MBit[i] ) != 0 )
			{
				Sum += 30;

				if ( Sum > Lest )
				{
					Sum -= 30;
					break;
				}
			}
			else
			{
				Sum += 29;

				if ( Sum > Lest )
				{
					Sum -= 29;
					break;
				}
			}
		}

		if ( i == 13 )return 0;

		i++;

		if ( i > LunarIndex[Offset].Leap )
		{
			i--;

			if ( i == LunarIndex[Offset].Leap )i = -i;
		}

		M2 = i;
		D2 = Lest - Sum + 1;
		return 1;
	}

	//---------------------------------------------------------------------------
	// 输入自公元1年1月1日以来的天数,返回对应的公元日期年月日
	void CCalendar::AbsToDate( long Days, int &Year, int &Month, int &Day )
	{
		const int MonthDays[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		long Y1, Y2, Y;
		int i, Lest, Sum;

		if ( Days <= 0 )return;

		for ( Y1 = ( Days - 1 ) / 366 + 1, Y2 = ( Days - 1 ) / 365 + 1; Y1 != Y2; )
		{
			Y = ( Y1 + Y2 ) / 2;

			if ( Days < YearDays( Y + 1 ) )Y2 = Y;
			else Y1 = Y + 1;
		}

		Year = Y1;
		Lest = ( int )( Days - YearDays( Y1 ) ) + 1;

		for ( Sum = 0, i = 1; i <= 12; i++ )
		{
			Sum += MonthDays[i];

			if ( i == 2 && IsLeapYear( Year ) )Sum += 1;

			if ( Sum >= Lest )break;
		}

		Month = i;
		Sum -= MonthDays[i];

		if ( Month == 2 && IsLeapYear( Year ) )Sum -= 1;

		Day = Lest - Sum;
	}
	//---------------------------------------------------------------------------
	// 输入年月日,函数返回该天是星期几,返回0表示是星期日.
	int CCalendar::GetWeek( int Year, int Month, int Day )
	{
		return ( int )( TotalDays( Year, Month, Day ) % 7 );
	}
	//---------------------------------------------------------------------------
	// 输入节气序号，返回节气的中文名称指针并拷贝在TermName中
	void CCalendar::GetTermName( int Term, char *TermName )
	{
		char STermName[24][5] = {"小寒", "大寒", "立春", "雨水", "惊蛰", "春分",
			"清明", "谷雨", "立夏", "小满", "芒种", "夏至", "小暑", "大暑", "立秋",
			"处暑", "白露", "秋分", "寒露", "霜降", "立冬", "小雪", "大雪", "冬至"
		};

		if ( Term < 0 || Term >= 24 )return;

		if ( TermName != NULL )
			for ( int i = 0; i < 5; i++ )TermName[i] = STermName[Term][i];
	}


	//---------------------------------------------------------------------------
	// 输入农历纪年（天干地支）号，返回年号的中文名称拷贝在EraName中
	void CCalendar::GetChineseEraName( int Era, char *EraName )
	{
		int TG = ( Era % 10 ) * 2, DZ = ( Era % 12 ) * 2;
		char TGName[24] = "甲乙丙丁戊己庚辛壬癸";
		char DZName[26] = "子丑寅卯辰巳午未申酉戌亥";
		EraName[0] = TGName[TG];
		EraName[1] = TGName[TG+1];
		EraName[2] = DZName[DZ];
		EraName[3] = DZName[DZ+1];
		EraName[4] = 0;
	}
	//---------------------------------------------------------------------------
	// 输入农历日号，返回日号的中文名称拷贝在LunarName中
	void CCalendar::GetLunarDateName( int Day, char *LunarName )
	{
		char FirstName[18] = {"初十廿三二十三十"};
		char LastName[22] = {"一二三四五六七八九十"};
		LunarName[4] = 0;

		if ( Day < 1 || Day > 39 )return;

		if ( Day == 20 )
		{
			for ( int i = 0; i < 4; i++ )LunarName[i] = FirstName[8+i];

			return;
		}

		if ( Day == 30 )
		{
			for ( int i = 0; i < 4; i++ )LunarName[i] = FirstName[12+i];

			return;
		}

		int Ten = ( ( Day - 1 ) / 10 ) * 2, GeWei = ( ( Day - 1 ) % 10 ) * 2;
		LunarName[0] = FirstName[Ten];
		LunarName[1] = FirstName[Ten+1];
		LunarName[2] = LastName[GeWei];
		LunarName[3] = LastName[GeWei+1];
	}

	//---------------------------------------------------------------------------
	// 输入农历月号，返回月号的中文名称拷贝在MonthName中
	void CCalendar::GetLunarMonthName( int Month, char *MonthName )
	{
		char MnName[12][8] = {"正月", "二月", "三月", "四月", "五月", "六月", "七月",
			"八月", "九月", "十月", "十一月", "十二月"
		};
		char *Ptr = MonthName, LeapName[3] = "闰";

		if ( Month < -12 || Month > 12 || Month == 0 )return;

		if ( Month < 0 )
		{
			MonthName[0] = LeapName[0];
			MonthName[1] = LeapName[1];
			Month = -Month;
			Ptr = MonthName + 2;
		}

		for ( int i = 0; i < 7; i++ )Ptr[i] = MnName[Month-1][i];
	}
	//---------------------------------------------------------------------------
	// 输入公历年份，返回农历纪年号
	int CCalendar::GetChineseEra( int Year )
	{
		return ( Year + 56 ) % 60;
	}

	//---------------------------------------------------------------------------
	// 输入公元年和节气号,返回该节气所在的年月日和时分Y,M,D,H,N
	int CCalendar::GetTermTime( int X, int T, int &Y, int &M, int &D, int &H, int &N )
	{
		const double SolarTerms[24] =   // 以下的数据为1996年的节气数据
		{
			5.39792,   20.11597,   34.88542,   49.71111,   64.64236,
			79.67847,   94.85278,  110.15000,  125.58472,  141.12361,
			156.76667,  172.46111,  188.19931,  203.91458,  219.60417,
			235.20347,  250.71806,  266.09514,  281.35972,  296.47431,
			311.48125,  326.36458,  341.17569,  355.91528
		};
	#define YEARTIME 365.2425  // 定义一个回归年的天天数(格里历)
		//#define YEARTIME 365.24219878125
		double Hours, Lest, BaseTime = -0.7875, SumDays = ( double )YEARTIME * ( X - 1 );

		// if YEARTIME=365.2425 then BaseTime=-0.7875 !!!
		// if YEARTIME=365.24219878125 then BaseTime=-0.186568594 !!!
		if ( T < 0 || T > 23 )return 0;
		else SumDays += SolarTerms[T] + BaseTime;

		long SumAbsDays = ( long )SumDays;
		Lest = SumDays - ( double )SumAbsDays;
		AbsToDate( SumAbsDays + 1, Y, M, D );
		Hours = ( double )Lest * 24.0;
		H = ( int )Hours;
		N = ( int )( ( Hours - ( double )H ) * 60.0 + 0.5 );
		return 1;
	#undef YEARTIME
	}


}

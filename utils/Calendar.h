#pragma once



namespace nsYLX
{

	class  CCalendar
	{
	public:
		CCalendar(void);
		~CCalendar(void);

		//日历转换算法
		static int  IsLeapYear(int Year);
		static long YearDays(int Year);
		static long TotalDays(int Year,int Month,int Day);
		static int  GetLunar(int Y1,int M1,int D1,int &Y2,int &M2,int &D2);
		static int  IsLegalDate(int Year,int Month,int Day);
		static int  GetSolarMonthDays(int Year,int Month);
		static long SolarMidDays(int Y1,int M1,int D1,int Y2,int M2,int D2);
		static void AbsToDate(long Days,int &Year,int &Month,int &Day);
		static bool GetSolarDate(int Y1, int M1, int D1, int& Y2, int& M2, int& D2 );
		static int  GetWeek(int Year,int Month,int Day); 
		static void GetTermName(int Term,char *TermName);
		static void GetChineseEraName(int Era,char *EraName);
		static void GetLunarDateName(int Day,char *LunarName);
		static void GetLunarMonthName(int Month,char *MonthName);
		static int  GetChineseEra(int Year);
		static int  GetTermTime(int X,int T,int &Y,int &M,int &D,int &H,int &N);
	};

}

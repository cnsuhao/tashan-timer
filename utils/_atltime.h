#pragma once



class CTimeSpan
{
public:
	CTimeSpan() throw();
	CTimeSpan( __time64_t time ) throw();
	CTimeSpan( LONG lDays, int nHours, int nMins, int nSecs ) throw();

	LONGLONG GetDays() const throw();
	LONGLONG GetTotalHours() const throw();
	LONG GetHours() const throw();
	LONGLONG GetTotalMinutes() const throw();
	LONG GetMinutes() const throw();
	LONGLONG GetTotalSeconds() const throw();
	LONG GetSeconds() const throw();

	__time64_t GetTimeSpan() const throw();

	CTimeSpan operator+( CTimeSpan span ) const throw();
	CTimeSpan operator-( CTimeSpan span ) const throw();
	CTimeSpan& operator+=( CTimeSpan span ) throw();
	CTimeSpan& operator-=( CTimeSpan span ) throw();
	bool operator==( CTimeSpan span ) const throw();
	bool operator!=( CTimeSpan span ) const throw();
	bool operator<( CTimeSpan span ) const throw();
	bool operator>( CTimeSpan span ) const throw();
	bool operator<=( CTimeSpan span ) const throw();
	bool operator>=( CTimeSpan span ) const throw();

private:
	__time64_t m_timeSpan;
};

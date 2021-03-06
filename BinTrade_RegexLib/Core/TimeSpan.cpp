/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "TimeSpan.h"

#include "Exception.h"


namespace Regexs
{
	const long long TimeSpan::TicksPerMillisecond = 10000i64;
	const long long TimeSpan::TicksPerSecond = TicksPerMillisecond * 1000i64;
	const long long TimeSpan::TicksPerMinute = TicksPerSecond * 60i64;
	const long long TimeSpan::TicksPerHour = TicksPerMinute * 60i64;
	const long long TimeSpan::TicksPerDay = TicksPerHour * 24i64;

	const double TimeSpan::MillisecondsPerTick = 1.0 / TicksPerMillisecond;
	const double TimeSpan::SecondsPerTick = 1.0 / TicksPerSecond;
	const double TimeSpan::MinutesPerTick = 1.0 / TicksPerMinute;
	const double TimeSpan::HoursPerTick = 1.0 / TicksPerHour;
	const double TimeSpan::DaysPerTick = 1.0 / TicksPerDay;

	const int TimeSpan::MillisPerSecond = 1000;
	const int TimeSpan::MillisPerMinute = MillisPerSecond * 60;
	const int TimeSpan::MillisPerHour = MillisPerMinute * 60;
	const int TimeSpan::MillisPerDay = MillisPerHour * 24;

	const long long TimeSpan::MaxSeconds = 9223372036854775807i64 / TicksPerSecond;
	const long long TimeSpan::MinSeconds = -9223372036854775806i64 / TicksPerSecond;
	const long long TimeSpan::MaxMilliSeconds = 9223372036854775807i64 / TicksPerMillisecond;
	const long long TimeSpan::MinMilliSeconds = -9223372036854775806i64 / TicksPerMillisecond;

	const long long TimeSpan::TicksPerTenthSecond = TicksPerMillisecond * 100;

	const TimeSpan TimeSpan::Zero(0);
	const TimeSpan TimeSpan::MaxValue(9223372036854775807i64);
	const TimeSpan TimeSpan::MinValue(-9223372036854775806i64);

	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	TimeSpan::TimeSpan() : ticks(0)
	{
	}

	//---------------------------------------------------------------------------
	TimeSpan::TimeSpan(long long ticks) : ticks(ticks)
	{
	}

	//---------------------------------------------------------------------------
	TimeSpan::TimeSpan(int hours, int minutes, int seconds) : ticks(TimeToTicks(hours, minutes, seconds))
	{
	}

	//---------------------------------------------------------------------------
	TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds)
	{
		const long long totalMilliSeconds = ((long long)days * 3600 * 24 + (long long)hours * 3600 + (long long)minutes * 60 + seconds) * 1000;
#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (totalMilliSeconds > MaxMilliSeconds || totalMilliSeconds < MinMilliSeconds)
		{
			throw ArgumentOutOfRangeException("milliseconds");
		}
#endif
		ticks = totalMilliSeconds * TicksPerMillisecond;
	}

	//---------------------------------------------------------------------------
	TimeSpan::TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds)
	{
		const long long totalMilliSeconds = ((long long)days * 3600 * 24 + (long long)hours * 3600 + (long long)minutes * 60 + seconds) * 1000 + milliseconds;
#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (totalMilliSeconds > MaxMilliSeconds || totalMilliSeconds < MinMilliSeconds)
		{
			throw ArgumentOutOfRangeException("milliseconds");
		}
#endif
		ticks = totalMilliSeconds * TicksPerMillisecond;
	}



	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	int TimeSpan::GetDays() const
	{
		return (int)(ticks / TicksPerDay);
	}

	//---------------------------------------------------------------------------
	int TimeSpan::GetHours() const
	{
		return (int)((ticks / TicksPerHour) % 24);
	}

	//---------------------------------------------------------------------------
	int TimeSpan::GetMinutes() const
	{
		return (int)((ticks / TicksPerMinute) % 60);
	}

	//---------------------------------------------------------------------------
	int TimeSpan::GetSeconds() const
	{
		return (int)((ticks / TicksPerSecond) % 60);
	}

	//---------------------------------------------------------------------------
	int TimeSpan::GetMilliseconds() const
	{
		return (int)((ticks / TicksPerMillisecond) % 1000);
	}

	//---------------------------------------------------------------------------
	long long TimeSpan::GetTicks() const
	{
		return ticks;
	}

	//---------------------------------------------------------------------------
	double TimeSpan::GetTotalDays() const
	{
		return ticks * DaysPerTick;
	}

	//---------------------------------------------------------------------------
	double TimeSpan::GetTotalHours() const
	{
		return ticks * HoursPerTick;
	}

	//---------------------------------------------------------------------------
	double TimeSpan::GetTotalMinutes() const
	{
		return ticks * MinutesPerTick;
	}

	//---------------------------------------------------------------------------
	double TimeSpan::GetTotalSeconds() const
	{
		return ticks * SecondsPerTick;
	}

	//---------------------------------------------------------------------------
	double TimeSpan::GetTotalMilliseconds() const
	{
		const double temp = ticks * MillisecondsPerTick;
		if (temp > MaxMilliSeconds)
		{
			return MaxMilliSeconds;
		}
		if (temp < MinMilliSeconds)
		{
			return MinMilliSeconds;
		}
		return temp;
	}

	//---------------------------------------------------------------------------
	//Runtime-Functions
	//---------------------------------------------------------------------------
	const TimeSpan TimeSpan::Add(const TimeSpan& ts) const
	{
		const long long result = ticks + ts.ticks;
#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if ((ticks >> 63 == ts.ticks >> 63) && (ticks >> 63 != result >> 63))
		{
			throw ArgumentOutOfRangeException("ticks");
		}
#endif
		return TimeSpan(result);
	}

	//---------------------------------------------------------------------------
	const TimeSpan TimeSpan::Subtract(const TimeSpan& ts) const
	{
		const long long result = ticks - ts.ticks;
#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if ((ticks >> 63 != ts.ticks >> 63) && (ticks >> 63 != result >> 63))
		{
			throw ArgumentOutOfRangeException("ticks");
		}
#endif
		return TimeSpan(result);
	}

	//---------------------------------------------------------------------------
	bool TimeSpan::operator ==(const TimeSpan& ts) const
	{
		return ticks == ts.ticks;
	}

	//---------------------------------------------------------------------------
	bool TimeSpan::operator !=(const TimeSpan& ts) const
	{
		return ticks != ts.ticks;
	}

	//---------------------------------------------------------------------------
	bool TimeSpan::operator <(const TimeSpan& ts) const
	{
		return ticks < ts.ticks;
	}

	//---------------------------------------------------------------------------
	bool TimeSpan::operator >(const TimeSpan& ts) const
	{
		return ticks > ts.ticks;
	}

	//---------------------------------------------------------------------------
	bool TimeSpan::operator <=(const TimeSpan& ts) const
	{
		return ticks <= ts.ticks;
	}

	//---------------------------------------------------------------------------
	bool TimeSpan::operator >=(const TimeSpan& ts) const
	{
		return ticks >= ts.ticks;
	}

	//---------------------------------------------------------------------------
	const TimeSpan TimeSpan::operator-() const
	{
		return TimeSpan(-ticks);
	}

	//---------------------------------------------------------------------------
	const TimeSpan TimeSpan::operator+() const
	{
		return *this;
	}

	//---------------------------------------------------------------------------
	const TimeSpan TimeSpan::operator-(const TimeSpan& ts) const
	{
		return Subtract(ts);
	}

	//---------------------------------------------------------------------------
	const TimeSpan TimeSpan::operator+(const TimeSpan& ts) const
	{
		return Add(ts);
	}

	//---------------------------------------------------------------------------
	TimeSpan TimeSpan::Duration() const
	{
#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (ticks == MinValue.ticks)
		{
			throw ArgumentOutOfRangeException("ticks");
		}
#endif
		return TimeSpan(ticks >= 0 ? ticks : -ticks);
	}

	//---------------------------------------------------------------------------
	TimeSpan TimeSpan::Negate() const
	{
#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (ticks == MinValue.ticks)
		{
			throw ArgumentOutOfRangeException("ticks");
		}
#endif
		return TimeSpan(-ticks);
	}

	//---------------------------------------------------------------------------
	TimeSpan TimeSpan::FromDays(double value)
	{
		return Interval(value, MillisPerDay);
	}

	//---------------------------------------------------------------------------
	TimeSpan TimeSpan::FromHours(double value)
	{
		return Interval(value, MillisPerHour);
	}

	//---------------------------------------------------------------------------
	TimeSpan TimeSpan::FromMinutes(double value)
	{
		return Interval(value, MillisPerMinute);
	}

	//---------------------------------------------------------------------------
	TimeSpan TimeSpan::FromSeconds(double value)
	{
		return Interval(value, MillisPerSecond);
	}

	//---------------------------------------------------------------------------
	TimeSpan TimeSpan::FromMilliseconds(double value)
	{
		return Interval(value, 1);
	}

	//---------------------------------------------------------------------------
	TimeSpan TimeSpan::FromTicks(long long value)
	{
		return TimeSpan(value);
	}

	//---------------------------------------------------------------------------
	TimeSpan TimeSpan::Interval(double value, int scale)
	{
		//if (value == 0.0 / 0.0)
		//{
		//	throw ArgumentException("Invalid argument: value is NAN", __FILE__, __LINE__);
		//}
		const double temp = value * scale;
		const double millis = temp + (value >= 0.0 ? 0.5 : -0.5);
#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if ((millis > MaxMilliSeconds) || (millis < MinMilliSeconds))
		{
			throw ArgumentOutOfRangeException("value");
		}
#endif
		return TimeSpan((long long)millis * TicksPerMillisecond);
	}

	//---------------------------------------------------------------------------
	long long TimeSpan::TimeToTicks(int hour, int minute, int second)
	{
		const long long totalSeconds = (long long)hour * 3600 + (long long)minute * 60 + (long long)second;
#ifndef OSHGUI_DONTUSEEXCEPTIONS
		if (totalSeconds > MaxSeconds || totalSeconds < MinSeconds)
		{
			throw ArgumentOutOfRangeException("totalSeconds");
		}
#endif
		return totalSeconds * TicksPerSecond;
	}

}

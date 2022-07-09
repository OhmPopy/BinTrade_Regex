/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

//Credits Microsoft .NET

#ifndef _UI_RegexsTIMESPAN_H_
#define _UI_RegexsTIMESPAN_H_
#pragma once

#include <TypeDef.h>

namespace Regexs
{
	/**
	 * Represents a time interval.
	 */
	class REGEX_X_API  TimeSpan
	{
		static const double MillisecondsPerTick;
		static const double SecondsPerTick;
		static const double MinutesPerTick;
		static const double HoursPerTick;
		static const double DaysPerTick;

		static const int MillisPerSecond;
		static const int MillisPerMinute;
		static const int MillisPerHour;
		static const int MillisPerDay;

		static const long long MaxSeconds;
		static const long long MinSeconds;
		static const long long MaxMilliSeconds;
		static const long long MinMilliSeconds;

		static const long long TicksPerTenthSecond;

		long long ticks;

	public:
		/**
		 * Represents the number of ticks per day.
		 */
		static const long long TicksPerDay;
		/**
		 * Represents the number of ticks per hour.
		 */
		static const long long TicksPerHour;
		/**
		 * Represents the number of ticks per minute.
		 */
		static const long long TicksPerMinute;
		/**
		 * Represents the number of ticks per second.
		 */
		static const long long TicksPerSecond;
		/**
		 * Represents the number of ticks per millisecond.
		 */
		static const long long TicksPerMillisecond;

		/**
		 * Represents the TimeSpan value for 0.
		 */
		static const TimeSpan Zero;
		/**
		 * Represents the maximum TimeSpan value.
		 */
		static const TimeSpan MaxValue;
		/**
		 * Represents the minimum TimeSpan value.
		 */
		static const TimeSpan MinValue;

		enum class FormatToString
		{
			DayHourMinSecondMiniSecond,
			HourMinSecondMiniSecond,
			MinSecondMiniSecond,
			DayHourMinSecond,
			HourMinSecond,
			MinSecond,
			HourMin,
			/**
			 * \brief Auto. ToString 
			 */
			Auto,
		};

	private:
		static TimeSpan Interval(double value, int scale);
		static long long TimeToTicks(int hour, int minute, int second);

	public:
		/**
		 * Initializes a new TimeSpan (see TimeSpan :: Zero).
		 */
		TimeSpan();
		/**
		 * Initializes a new TimeSpan for the specified number of ticks.
		 *
		 * @param ticks a period of time expressed in units of 100 nanoseconds
		 */
		explicit TimeSpan(long long ticks);
		/**
		 * Initializes a new TimeSpan with the specified number of hours, minutes, and seconds.
		 *
		* @param hours Number of hours
		* @param minutes Number of minutes
		* @param seconds Number of seconds
		 */
		TimeSpan(int hours, int minutes, int seconds);
		/**
		* Initializes a new TimeSpan with the specified number of days, hours,
		* Minutes and seconds.
		*
		* @param days Number of days
		* @param hours Number of hours
		* @param minutes Number of minutes
		* @param seconds Number of seconds
		 */
		TimeSpan(int days, int hours, int minutes, int seconds);
		/**
		* Initializes a new TimeSpan with the specified number of days, hours,
		* Minutes, seconds and milliseconds.
		*
		* @param days Number of days
		* @param hours Number of hours
		* @param minutes Number of minutes
		* @param seconds Number of seconds
		* @param milliseconds Number of milliseconds
		 */
		TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds);

		__declspec( property ( get=GetTicks) ) long long Ticks;
		__declspec( property ( get=GetDays) ) int Days;
		__declspec( property ( get=GetHours) ) int Hours;
		__declspec( property ( get=GetMilliseconds) ) int Milliseconds;
		__declspec( property ( get=GetMinutes) ) int Minutes;
		__declspec( property ( get=GetSeconds) ) int Seconds;
		__declspec( property ( get=GetTotalDays) ) double TotalDays;
		__declspec( property ( get=GetTotalHours) ) double TotalHours;
		__declspec( property ( get=GetTotalMilliseconds) ) double TotalMilliseconds;
		__declspec( property ( get=GetTotalMinutes) ) double TotalMinutes;
		__declspec( property ( get=GetTotalSeconds) ) double TotalSeconds;

		/**
		* Gets the day component of the time interval that is determined by the
		* Current TimeSpan structure is shown.
		*
		* @return the days
		 */
		int GetDays() const;
		/**
		* Gets the value of the current TimeSpan structure in whole days and
		* Fractions of days off.
		*
		* @return all days and fractions
		 */
		double GetTotalDays() const;
		/**
		* Gets the hourly component of the time interval represented by the
		* Current TimeSpan structure is shown.
		*
		* @return the hours
		 */
		int GetHours() const;
		/**
		* Gets the value of the current TimeSpan structure in whole hours and
		* Fractions of hours.
		*
		* @return the whole hours and fractions
		 */
		double GetTotalHours() const;
		/**
		* Gets the minute component of the time interval that the
		* Current TimeSpan structure is shown.
		*
		* @return the minutes
		 */
		int GetMinutes() const;
		/**
		* Gets the value of the current TimeSpan structure in whole minutes and
		* Fractions of minutes.
		*
		* @return all the minutes and fractions
		 */
		double GetTotalMinutes() const;
		/**
		* Gets the seconds component of the time interval elapsed by the
		* Current TimeSpan structure is shown.
		*
		* @return the seconds
		 */
		int GetSeconds() const;
		/**
		* Gets the value of the current TimeSpan structure in whole seconds and
		* Fractions of seconds off.
		*
		* @return the whole seconds and fractions
		 */
		double GetTotalSeconds() const;
		/**
		* Gets the millisecond component of the time interval elapsed by the
		* Current TimeSpan structure is shown.
		*
		* @return the milliseconds
		 */
		int GetMilliseconds() const;
		/**
		* Gets the value of the current TimeSpan structure in whole milliseconds and
		* Fractions of milliseconds.
		*
		* @return the whole milliseconds and fractions
		 */
		double GetTotalMilliseconds() const;
		/**
		* Gets the number of ticks in the time interval that the
		* Current TimeSpan structure is shown.
		*
		* @return the ticks
		*/
		long long GetTicks() const;

		/**
		* Adds the specified TimeSpan to this instance.
		*
		* @return a TimeSpan
		 */
		const TimeSpan Add(const TimeSpan& ts) const;
		/**
		* Subtracts the specified TimeSpan from this instance.
		*
		* @return a TimeSpan
		 */
		const TimeSpan Subtract(const TimeSpan& ts) const;

		bool operator ==(const TimeSpan& ts) const;
		bool operator !=(const TimeSpan& ts) const;
		bool operator <(const TimeSpan& ts) const;
		bool operator >(const TimeSpan& ts) const;
		bool operator <=(const TimeSpan& ts) const;
		bool operator >=(const TimeSpan& ts) const;
		const TimeSpan operator -() const;
		const TimeSpan operator +() const;
		const TimeSpan operator -(const TimeSpan& ts) const;
		const TimeSpan operator +(const TimeSpan& ts) const;

		/**
		* Returns a TimeSpan whose value is the absolute value of this instance.
		*
		* @return the TimeSpan
		 */
		TimeSpan Duration() const;
		/**
		* Returns a TimeSpan whose value is the negated value of this instance.
		*
		* @return the TimeSpan
		 */
		TimeSpan Negate() const;

		/**
		* Returns a TimeSpan representing a specified number of days.
		* The information is rounded to the nearest millisecond.
		*
		* @param value a number of days, rounded to the nearest millisecond
		* @return a TimeSpan
		 */
		static TimeSpan FromDays(double value);
		/**
		* Returns a TimeSpan that is a specified number of hours.
		* The information is rounded to the nearest millisecond.
		*
		* @param value a number of hours, rounded to the nearest millisecond
		* @return a TimeSpan
		 */
		static TimeSpan FromHours(double value);
		/**
		* Returns a TimeSpan that is a specified number of minutes.
		* The information is rounded to the nearest millisecond.
		*
		* @param value is a number of minutes, rounded to the nearest millisecond
		* @return a TimeSpan
		 */
		static TimeSpan FromMinutes(double value);
		/**
		* Returns a TimeSpan that is a specified number of minutes.
		* The information is rounded to the nearest millisecond.
		*
		* @param value is a number of minutes, rounded to the nearest millisecond
		* @return a TimeSpan
		 */
		static TimeSpan FromSeconds(double value);
		/**
		* Returns a TimeSpan that is a specified number of milliseconds.
		*
		* @param value a number of milliseconds
		* @return a TimeSpan
		 */
		static TimeSpan FromMilliseconds(double value);
		/**
		* Returns a TimeSpan that represents a specified time period in ticks.
		*
		* @param value a period of time expressed in units of 100 nanoseconds
		* @return a TimeSpan
		 */
		static TimeSpan FromTicks(long long value);

	};
}


#endif

#pragma once
#include <cstdint>
#include "nn/types.h"

namespace nn::time
{
    struct PosixTime {
        int64_t value = 0; //secondes depuis le 1er janvier 1970
    };

    struct CalendarTime {
        int16_t year = 0;
        int8_t month = 0;
        int8_t day = 0;
        int8_t hour = 0;
        int8_t minute = 0;
        int8_t second = 0;
    };

    struct CalendarAdditionalInfo {
        int dayOfWeek = 0;
        int dayOfYear = 0;
        char timeZoneName[8] = {};
        bool isDaylightSavingTime = false;
        int utcOffsetSeconds = 0;
    };

    struct LocationName {
        char value[36] = {};
    };

    struct TimeZoneRule {
        int dummy = 0;
    };

    struct ClockSnapshot {
        PosixTime time;

        ClockSnapshot();
        static Result CreateWithAdjustableSystemClock(ClockSnapshot* outSnapshot);
    };

    struct StandardUserSystemClock {
        static Result GetCurrentTime(PosixTime* outTime);
    };

    struct AdjustableUserSystemClock {
        static Result GetCurrentTime(PosixTime* outTime);
        static void Adjust();
    };

    Result Initialize();
    void   Finalize();
    Result GetDeviceLocationName(LocationName* outName);
    Result LoadTimeZoneRule(TimeZoneRule* outRule, const LocationName& name);
    Result ToCalendarTime(CalendarTime* outCalendar, CalendarAdditionalInfo* outInfo,
                          const PosixTime& time, const TimeZoneRule& rule);
    TimeSpan CalculateStandardUserSystemClockDifferenceByUser(const ClockSnapshot& a, const ClockSnapshot& b);
}
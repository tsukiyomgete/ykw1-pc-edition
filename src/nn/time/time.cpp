#include "nn/time.h"
#include <ctime>
#include <cstring>

namespace nn::time {

Result Initialize()
{
    return Result{};
}

void Finalize()
{
}

//L'heure officielle : celle du PC
Result StandardUserSystemClock::GetCurrentTime(PosixTime* outTime)
{
    std::time_t now = std::time(nullptr); //secondes depuis le 1er janvier 1970
    outTime->value = now;
    return Result{};
}

//L'heure reglable (evenements du jeu) : pour l'instant, la meme que le PC
Result AdjustableUserSystemClock::GetCurrentTime(PosixTime* outTime)
{
    return StandardUserSystemClock::GetCurrentTime(outTime);
}

void AdjustableUserSystemClock::Adjust()
{
    //Rien a faire sur PC
}

Result GetDeviceLocationName(LocationName* outName)
{
    std::strcpy(outName->value, "Europe/Paris");
    return Result{};
}

Result LoadTimeZoneRule(TimeZoneRule* outRule, const LocationName& name)
{
    //Rien a faire : localtime connait deja le fuseau du PC
    (void)outRule;
    (void)name;
    return Result{};
}

Result ToCalendarTime(CalendarTime* outCalendar, CalendarAdditionalInfo* outInfo,
                      const PosixTime& time, const TimeZoneRule& rule)
{
    (void)rule;
    std::time_t seconds = time.value;
    std::tm* date = std::localtime(&seconds);
    if (date == nullptr)
    {
        return Result{1};
    }

    outCalendar->year = date->tm_year + 1900; //tm compte les annees depuis 1900
    outCalendar->month = date->tm_mon + 1;    //tm compte les mois de 0 a 11
    outCalendar->day = date->tm_mday;
    outCalendar->hour = date->tm_hour;
    outCalendar->minute = date->tm_min;
    outCalendar->second = date->tm_sec;

    if (outInfo != nullptr)
    {
        outInfo->dayOfWeek = date->tm_wday; //0 = dimanche
        outInfo->dayOfYear = date->tm_yday;
    }
    return Result{};
}

ClockSnapshot::ClockSnapshot()
{
    time.value = 0;
}

Result ClockSnapshot::CreateWithAdjustableSystemClock(ClockSnapshot* outSnapshot)
{
    return AdjustableUserSystemClock::GetCurrentTime(&outSnapshot->time);
}

TimeSpan CalculateStandardUserSystemClockDifferenceByUser(const ClockSnapshot& a, const ClockSnapshot& b)
{
    int64_t difference = b.time.value - a.time.value; //en secondes
    TimeSpan result;
    result.nanoSeconds = difference * 1000000000;
    return result;
}

}

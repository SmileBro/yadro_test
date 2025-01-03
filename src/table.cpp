#include "table.hpp"

void Table::freeOccupied(std::tm end, long long price)
{
    if (!occupied)
    {
        return;
    }

    // Increase total time
    int minutes = end.tm_hour * 60 - occupationTm.tm_hour * 60 + end.tm_min - occupationTm.tm_min;
    totalTm.tm_hour += (minutes / 60);
    minutes %= 60;
    if (totalTm.tm_min + minutes > 59)
    {
        totalTm.tm_hour++;
        totalTm.tm_min = (totalTm.tm_min + minutes) % 60;
    } else 
    {
        totalTm.tm_min += minutes;
    }

    // Profit increase
    int duration = end.tm_hour - occupationTm.tm_hour;
    if (end.tm_min - occupationTm.tm_min > 0)
    {
        duration++;
    }
    profit += duration * price;

    // Reset occupation
    occupationTm = std::tm{};
    occupied = false;
}
#ifndef TABLE_H
#define TABLE_H
#include <ctime>
class Table
{
private:
    bool occupied = false;
    long long profit = 0;
    std::tm occupationTm = std::tm{};
    std::tm totalTm = std::tm{};
public:
    void setOccupied(std::tm tm)
    {
        occupied = true;
        occupationTm = tm;
    }

    bool isOccupied()
    {
        return occupied;
    }

    long long getProfit()
    {
        return profit;
    }

    void freeOccupied(std::tm end, long long price);

    std::tm* getTotalTm()
    {
        return &totalTm;
    }
};

#endif
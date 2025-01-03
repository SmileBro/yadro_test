#ifndef TYPES_H
#define TYPES_H
#include <string>
#include <ctime>

struct CompClub
{
    long long tables = 0;
    long long price = 0;
    std::tm start = std::tm{};
    std::tm end = std::tm{};
};

struct Event
{
    long long id = 0;
    long long table = 0;
    std::tm time;
    std::string client;
};

enum InId
{
    CLIENT_CAME = 1,
    CLIENT_SAT = 2,
    CLIENT_WAITING = 3,
    CLIENT_LEFT = 4
};

enum OutId
{
    OUT = 11,
    DEQUEUE = 12,
    ERROR = 13
};

enum ErrNum
{
    YouShallNotPass,
    NotOpenYet,
    PlaceIsBusy,
    ClientUnknown,
    ICanWaitNoLonger,
    AlreadyInQueue,
    HavePlaceAlready
};

#endif
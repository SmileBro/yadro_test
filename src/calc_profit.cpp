#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

#include "check_format.hpp"
#include "table.hpp"
#include "calc_profit.hpp"

void print_error(std::tm tm, ErrNum err)
{
    if (err > HavePlaceAlready)
    {
        return;
    }

    std::cout << std::put_time(&tm, "%H:%M") << " " << ERROR << " ";
    switch (err)
    {
    case YouShallNotPass:
        std::cout << "YouShallNotPass\n";
        break;
    case NotOpenYet:
        std::cout << "NotOpenYet\n";
        break;
    case PlaceIsBusy:
        std::cout << "PlaceIsBusy\n";
        break;
    case ClientUnknown:
        std::cout << "ClientUnknown\n";
        break;
    case ICanWaitNoLonger:
        std::cout << "ICanWaitNoLonger!\n";
        break;
    case AlreadyInQueue:
        std::cout << "AlreadyInQueue\n";
        break;
    case HavePlaceAlready:
        std::cout << "HavePlaceAlready\n";
        break;
    default:
        break;
    }
}

void print_event(std::tm tm, OutId id, std::string client, long long table)
{
    std::cout << std::put_time(&tm, "%H:%M") << ' ' << id << ' ' << client;

    if (table != 0)
    {
        std::cout << ' ' << table << '\n';
    } else 
    {
        std::cout << '\n';
    }
}

void print_event(std::tm tm, OutId id, std::string client)
{
    print_event(tm, id, client, 0);
}

int calc_profit(const CompClub& club_info, const char* file_name)
{
    std::ifstream f(file_name);
    if (!f.is_open()) 
    {
        std::cerr << "Error opening the file!";
        return 1;
    }

    std::string line;
    for (int i = 0; i < 3; i++)
    {
        std::getline(f, line, '\n');
    }

    std::vector<Table> tables(club_info.tables + 1);
    long long free_tables = club_info.tables;
    std::unordered_map<std::string, long long> clients;
    std::queue<std::string> q;
    
    std::cout << std::put_time(&club_info.start, "%H:%M") << '\n';
    while (std::getline(f, line, '\n'))
    {
        std::cout << line << std::endl;

        Event event{};
        check_event(event, line);

        std::string client = event.client;

        switch (event.id)
        { 
        case CLIENT_CAME: 
            if (clients[client] != 0)
            {
                print_error(event.time, YouShallNotPass);
                break;
            }
            if (check_tm_increase(club_info.start, event.time))
            {
                print_error(event.time, NotOpenYet);
                break;
            }
            clients[client] = -1;
            break; 
        case CLIENT_SAT:
            if (clients[client] == 0)
            {
                print_error(event.time, ClientUnknown);
                break;
            }
            if (tables[event.table].isOccupied())
            {
                print_error(event.time, PlaceIsBusy);
                break;
            }
            if (clients[client] > 0)
            {
                tables[clients[client]].freeOccupied(event.time, club_info.price);
                free_tables++;
            }
            tables[event.table].setOccupied(event.time);
            clients[client] = event.table;
            free_tables--;
            break; 
        case CLIENT_WAITING:
            // If already in queue
            if (clients[client] == -2)
            {
                print_error(event.time, AlreadyInQueue);
                break;
            }
            // If already have a place
            if (clients[client] > 0)
            {
                print_error(event.time, HavePlaceAlready);
                break;
            }
            if (clients[client] == 0)
            {
                print_error(event.time, ClientUnknown);
                break;
            }
            if (free_tables > 0)
            {
                print_error(event.time, ICanWaitNoLonger);
                break; 
            }
            if (q.size() + 1 > club_info.tables)
            {
                clients[client] = 0;
                print_event(event.time, OUT, client);
                break;
            }
            q.push(client);
            clients[client] = -2;
            break;
        case CLIENT_LEFT:
            if (clients[client] == 0)
            {
                print_error(event.time, ClientUnknown);
                break;
            }
            if (clients[client] > 0)
            {
                tables[clients[client]].freeOccupied(event.time, club_info.price);
                if (q.size() > 0)
                {
                    std::string new_client = q.front();
                    q.pop();
                    tables[clients[client]].setOccupied(event.time);
                    clients[new_client] = clients[client];
                    print_event(event.time, DEQUEUE, new_client, clients[client]);
                } else 
                {
                    free_tables++;
                }
            }
            clients[client] = 0;
            break;
        default: 
            return 1;
        }
    }

    // Grab all clients that are left in the club
    std::vector<std::string> names;
    for (auto kv : clients)
    {
        if (kv.second != 0)
        {
            names.push_back(kv.first);
        }
    }
    std::sort(names.begin(), names.end());
    
    for (std::string name : names)
    {
        print_event(club_info.end, OUT, name);
        tables[clients[name]].freeOccupied(club_info.end, club_info.price);
    }
    std::cout << std::put_time(&club_info.end, "%H:%M") << '\n';

    for (int i = 1; i <= tables.size() - 1; i++)
    {
        std::cout << i << ' ' << tables[i].getProfit() << ' ';
        std::cout << std::put_time(tables[i].getTotalTm(), "%H:%M") << '\n'; 
    }
    return 0;
}
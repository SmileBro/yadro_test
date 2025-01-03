#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include "check_format.hpp"

bool check_long(long long* dst, std::string in)
{
    long long tmp = 0;
    std::istringstream ss(in);
    
    ss >> tmp;

    if ((ss.fail()) || (!ss.eof()))
    {
        return 1;
    }

    if (tmp < 1)
    {
        return 1;
    }
    *dst = tmp;
    return 0;
}

bool check_long(std::string in)
{
    long long tmp = 0;
    return check_long(&tmp, in);
}

bool check_tm_increase(std::tm less, std::tm more)
{    
    if (less.tm_hour > more.tm_hour)
    {
        return 1;
    }
    if (less.tm_hour < more.tm_hour)
    {
        return 0;
    }
    return less.tm_min > more.tm_min; 
}

bool check_tm(std::tm *old_tm, const std::string in)
{
    std::istringstream ss(in);
    std::tm new_tm{};
    ss >> std::get_time(&new_tm, "%H:%M");
    if ((ss.fail()) || (in.size() != 5))
    {
        return 1;
    }
    if (check_tm_increase(*old_tm, new_tm))
    {
        return 1;
    }
    *old_tm = new_tm;
    return 0;
}

bool check_tm(std::string in)
{
    std::tm tmp{};
    return check_tm(&tmp, in);
}

bool check_client(const std::string client)
{
    if (client.size() < 1)
    {
        return 1;
    }
    for (char ch : client)
    {
        bool hit = false;
        hit |= (ch >= 'a' && ch <= 'z');
        hit |= (ch >= '0' && ch <= '9');
        hit |= (ch == '_') || (ch == '-');
        if (!hit)
        {
            return 1;
        }
    }
    return 0;
}

bool check_event(Event& event, const std::string line)
{
    uint8_t part = 0;
    std::string s = line;
    while (s.size() > 0)
    {
        std::string token = s;
        size_t pos = s.find(' ');
        if (pos != std::string::npos)
        {
            token = s.substr(0, pos);
            s.erase(0, pos + 1);
        } else
        {
            s.clear();
        }
           
        switch (part)
        {
        case 0:
            if (check_tm(&event.time, token))
            {
                event = {};
                return 1;  
            }
            part++;
            break;
        case 1:
            if (check_long(&event.id, token))
            {
                event = {};
                return 1;  
            }
            if (event.id > CLIENT_LEFT)
            {
                event = {};
                return 1;
            }
            part++;
            break;
        case 2:
            if (check_client(token))
            {
                event = {};
                return 1;  
            }
            part++;
            event.client = token;
            break;
        case 3:
            if ((event.id != CLIENT_SAT) || // Only in Event ID 2 we have 4 parts
                (check_long(&event.table, token)))
            {
                event = {};
                return 1; 
            }
            part++;
            break;        
        default:
            event = {};
            return 1;
        }
    }
    if ((part != 3 && event.id != CLIENT_SAT) ||
        (part != 4 && event.id == CLIENT_SAT))
    {
        event = {};
        return 1;
    }
    return 0;
}

bool check_format(CompClub& club_info, const char* file_name)
{
    std::ifstream f(file_name);
    if (!f.is_open()) 
    {
        std::cerr << "Error opening the file!";
        return 1;
    }

    // Check amount of tables
    std::string line;
    std::getline(f, line, '\n');
    if (f.fail() || check_long(&club_info.tables, line))
    {
        std::cout << line << std::endl;
        return 1;        
    }
    
    // Check start and end times
    std::getline(f, line, '\n');
    if (f.fail())
    {
        std::cout << line << std::endl;
        return 1;        
    }
    
    auto pos = line.find(' ');
    if (pos == std::string::npos)
    {
        std::cout << line << std::endl;
        return 1; 
    }

    std::string startStr = line.substr(0, pos);
    std::string endStr = line;
    endStr.erase(0, pos + 1);
    if (check_tm(&club_info.start, startStr) || 
        check_tm(&club_info.end, endStr))
    {
        std::cout << line << std::endl;
        return 1;        
    }
    if (check_tm_increase(club_info.start, club_info.end))
    {
        std::cout << line << std::endl;
        return 1;
    }

    // Check price
    std::getline(f, line, '\n');
    if (f.fail() || check_long(&club_info.price, line))
    {
        std::cout << line << std::endl;
        return 1;        
    }

    // Check records
    std::tm cur_tm{};
    while (std::getline(f, line, '\n'))
    {
        Event event{};
        event.time = cur_tm;
        if (check_event(event, line))
        {
            std::cout << line << std::endl;
            return 1;
        }
        if ((event.table != 0 && event.table > club_info.tables) ||
            (check_tm_increase(event.time, club_info.end)))
        {
            std::cout << line << std::endl;
            return 1;
        }
        cur_tm = event.time;
    }
    return 0;
}
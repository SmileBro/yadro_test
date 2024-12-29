#include <iostream>
#include "check_format.hpp"
#include "calc_profit.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Provide one name of an input file as an argument\n";
        return 1;
    }

    CompClub club_info{};
    if (check_format(club_info, argv[1]))
    {
        return 1;
    }

    return calc_profit(club_info, argv[1]);
}
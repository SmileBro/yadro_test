#ifndef CALC_PROFIT_H
#define CALC_PROFIT_H
#include "types.hpp"

/*
	Returns 1 if error, 0 otherwise.
    @param club_info info about computer club params.
	@param file_name name of the input file
	@return 1 if there is an error
*/
int calc_profit(const CompClub& club_info, const char* file_name);

#endif
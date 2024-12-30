#ifndef CHECK_FORMAT_H
#define CHECK_FORMAT_H
#include "types.hpp"

/*
	Returns 0 if no format error found in the given file, 1 otherwise.
    Also populates fields of club_info struct
	@param file_name name of the input file
	@return True if there is an error
*/
bool check_format(CompClub& club_info, const char* file_name);

/*
    Check that std::tm on the right is more than std::tm on the left
	@param less std::tm that should be less
    @param more std::tm that should be more
	@return True if `less` is more of equal
*/
bool check_tm_increase(std::tm less, std::tm more);

bool check_event(Event& event, std::string line);

bool check_long(long long* dst, std::string in);

bool check_long(std::string in);

bool check_tm(std::string in);

/*
    Check if input string format is "%H:%M"
	also checks if `old_tm` is less than std::tm parsed from `in` 
	@param *old_tm std::tm that should be less
    @param in std::string that should be more and have no format errors
	@return True if there are format errors or `old_tm` is less
*/
bool check_tm(std::tm *old_tm, const std::string in);

bool check_client(const std::string client);

#endif
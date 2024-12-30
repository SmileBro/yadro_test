#include <gtest/gtest.h>
#include "check_format.hpp"
#include "file_fixture.hpp"

TEST_F(FileTest, Example)
{
    RedirectCout();
    CompClub club_info{};
    bool res = check_format(club_info, GetInputFile().c_str());
    ReturnCout();
    ASSERT_FALSE(res);
    CompareOutputs();
}

TEST_F(FileTest, TwoTables)
{
    RedirectCout();
    CompClub club_info{};
    bool res = check_format(club_info, GetInputFile().c_str());
    ReturnCout();
    ASSERT_FALSE(res);
    CompareOutputs();
}

TEST_F(FileTest, ThreeTables)
{
    RedirectCout();
    CompClub club_info{};
    bool res = check_format(club_info, GetInputFile().c_str());
    ReturnCout();
    ASSERT_FALSE(res);
    CompareOutputs();
}

TEST_F(FileTest, AlphabetOrder)
{
    RedirectCout();
    CompClub club_info{};
    bool res = check_format(club_info, GetInputFile().c_str());
    ReturnCout();
    ASSERT_FALSE(res);
    CompareOutputs();
}

TEST_F(FileTest, PerfectDay)
{
    RedirectCout();
    CompClub club_info{};
    bool res = check_format(club_info, GetInputFile().c_str());
    ReturnCout();
    ASSERT_FALSE(res);
    CompareOutputs();
}

TEST_F(FileTest, LonelyDay)
{
    RedirectCout();
    CompClub club_info{};
    bool res = check_format(club_info, GetInputFile().c_str());
    ReturnCout();
    ASSERT_FALSE(res);
    CompareOutputs();
}
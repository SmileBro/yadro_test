#include <gtest/gtest.h>
#include "check_format.hpp"
#include <climits>
#include "file_fixture.hpp"

/*check_long tests*/
TEST(FormatTest, CheckLongBasic)
{
    EXPECT_FALSE(check_long("123"));
    EXPECT_FALSE(check_long("1"));
}

TEST(FormatTest, CheckLongReturn)
{
    long long tmp = 0;
    EXPECT_FALSE(check_long(&tmp, "123"));
    ASSERT_EQ(tmp, 123);
}

TEST(FormatTest, CheckLongNegative)
{
    long long tmp = 0;
    std::string in = "-100";
    EXPECT_TRUE(check_long(in));
    EXPECT_TRUE(check_long(&tmp, in));
    ASSERT_EQ(tmp, 0);
}

TEST(FormatTest, CheckLongZero)
{
    long long tmp = 1;
    std::string in = "0";
    EXPECT_TRUE(check_long(in));
    EXPECT_TRUE(check_long(&tmp, in));
    ASSERT_EQ(tmp, 1);
}

TEST(FormatTest, CheckLongIncorrect)
{
    long long tmp = 0;
    std::string in = "1 ";
    EXPECT_TRUE(check_long(in));
    EXPECT_TRUE(check_long(&tmp, in));
    ASSERT_EQ(tmp, 0);
}

TEST(FormatTest, CheckLongMax)
{
    long long tmp = 0;
    std::string in = std::to_string(LLONG_MAX);
    EXPECT_FALSE(check_long(in));
    EXPECT_FALSE(check_long(&tmp, in));
    ASSERT_EQ(tmp, LLONG_MAX);
}

TEST(FormatTest, CheckLongMin)
{
    long long tmp = 0;
    std::string in = std::to_string(LLONG_MIN);
    EXPECT_TRUE(check_long(in));
    EXPECT_TRUE(check_long(&tmp, in));
    ASSERT_EQ(tmp, 0);
}

/*check_tm tests*/
TEST(FormatTest, CheckTmBasic)
{
    EXPECT_FALSE(check_tm("00:00"));
    EXPECT_FALSE(check_tm("12:30"));
    EXPECT_FALSE(check_tm("08:08"));
    EXPECT_FALSE(check_tm("01:59"));
}

TEST(FormatTest, CheckTmIncorrect)
{
    EXPECT_TRUE(check_tm("24:00"));
    EXPECT_TRUE(check_tm("12:3"));
    EXPECT_TRUE(check_tm(":08"));
    EXPECT_TRUE(check_tm("01:61"));
}

TEST(FormatTest, CheckTmReturn)
{
    std::tm tm = {};
    EXPECT_FALSE(check_tm(&tm, "01:59"));
    EXPECT_EQ(tm.tm_hour, 1);
    EXPECT_EQ(tm.tm_min, 59);
}

TEST(FormatTest, CheckTmReturnNegative)
{
    std::tm tm = {};
    EXPECT_TRUE(check_tm(&tm, "01"));
    EXPECT_EQ(tm.tm_hour, 0);
    EXPECT_EQ(tm.tm_min, 0);
}

TEST(FormatTest, CheckTmReturnNegative2)
{
    std::tm tm = {};
    EXPECT_TRUE(check_tm(&tm, "00123"));
    EXPECT_EQ(tm.tm_hour, 0);
    EXPECT_EQ(tm.tm_min, 0);
}

TEST(FormatTest, CheckTmIncrease)
{
    std::tm tm1 = {};
    EXPECT_FALSE(check_tm(&tm1, "10:00"));
    std::tm tm2 = {};

    EXPECT_FALSE(check_tm(&tm2, "10:30"));
    EXPECT_FALSE(check_tm_increase(tm1, tm2));

    EXPECT_FALSE(check_tm(&tm2, "11:00"));
    EXPECT_FALSE(check_tm_increase(tm1, tm2));

    EXPECT_FALSE(check_tm(&tm2, "11:30"));
    EXPECT_FALSE(check_tm_increase(tm1, tm2));
}

TEST(FormatTest, CheckTmDecrease)
{
    std::tm tm1 = {};
    std::tm tm2 = {};
    EXPECT_FALSE(check_tm(&tm1, "10:30"));
    EXPECT_FALSE(check_tm(&tm2, "10:00"));

    // Expect true because all of those are decreasing
    EXPECT_TRUE(check_tm_increase(tm1, tm2));

    // Expect that if tm2 is more than input than tm2 should not change
    EXPECT_TRUE(check_tm(&tm2, "09:30"));
    EXPECT_EQ(tm2.tm_hour, 10);
    EXPECT_EQ(tm2.tm_min, 0);
    tm2.tm_hour = 9;
    tm2.tm_min = 30;
    EXPECT_TRUE(check_tm_increase(tm1, tm2));

    EXPECT_TRUE(check_tm(&tm2, "08:00"));
    EXPECT_EQ(tm2.tm_hour, 9);
    EXPECT_EQ(tm2.tm_min, 30);
    tm2.tm_hour = 8;
    tm2.tm_min = 0;
    EXPECT_TRUE(check_tm_increase(tm1, tm2));
}

TEST(FormatTest, CheckTmEqual)
{
    std::tm tm1 = {};
    EXPECT_FALSE(check_tm(&tm1, "10:00"));
    std::tm tm2 = {};
    EXPECT_FALSE(check_tm(&tm2, "10:00"));

    EXPECT_FALSE(check_tm_increase(tm1, tm2));
}

/*check_client tests*/
TEST(FormatTest, CheckClientBasic) 
{
    EXPECT_FALSE(check_client("client1"));
    EXPECT_FALSE(check_client("12345"));
    EXPECT_FALSE(check_client("-"));
    EXPECT_FALSE(check_client("_"));
    EXPECT_FALSE(check_client("abcdefghijklmnopqrstuvwxyz"));
    EXPECT_FALSE(check_client("abcdefghijklmnopqrstuvwxyz_-1234567890"));
}

TEST(FormatTest, CheckClientNegative) 
{
    EXPECT_TRUE(check_client("client1 "));
    EXPECT_TRUE(check_client(" 12345"));
    EXPECT_TRUE(check_client("-#"));
    EXPECT_TRUE(check_client("^_"));
    EXPECT_TRUE(check_client("\0"));
    EXPECT_TRUE(check_client("\n"));
}

/*check_event tests*/
TEST(FormatTest, CheckEventBasic) 
{
    Event event = {};
    EXPECT_FALSE(check_event(event, "08:48 1 client1"));
    EXPECT_EQ(event.time.tm_hour, 8);
    EXPECT_EQ(event.time.tm_min, 48);
    EXPECT_EQ(event.id, 1);
    EXPECT_EQ(event.client, "client1");

    EXPECT_FALSE(check_event(event, "09:54 2 client1 11234"));
    EXPECT_EQ(event.time.tm_hour, 9);
    EXPECT_EQ(event.time.tm_min, 54);
    EXPECT_EQ(event.id, 2);
    EXPECT_EQ(event.table, 11234);
    EXPECT_EQ(event.client, "client1");

    event = {};
    EXPECT_FALSE(check_event(event, "00:00 3 123"));
    EXPECT_EQ(event.time.tm_hour, 0);
    EXPECT_EQ(event.time.tm_min, 0);
    EXPECT_EQ(event.id, 3);
    EXPECT_EQ(event.client, "123");

    EXPECT_FALSE(check_event(event, "23:59 4 -_"));
    EXPECT_EQ(event.time.tm_hour, 23);
    EXPECT_EQ(event.time.tm_min, 59);
    EXPECT_EQ(event.id, 4);
    EXPECT_EQ(event.client, "-_");
}

TEST(FormatTest, CheckEventNegative) 
{
    Event event = {};
    EXPECT_TRUE(check_event(event, "08:48 1 client1%"));
    EXPECT_EQ(event.time.tm_hour, 0);
    EXPECT_EQ(event.time.tm_min, 0);
    EXPECT_EQ(event.id, 0);
    EXPECT_EQ(event.client, "");

    EXPECT_TRUE(check_event(event, "09:54 3 client1 11234"));
    EXPECT_EQ(event.time.tm_hour, 0);
    EXPECT_EQ(event.time.tm_min, 0);
    EXPECT_EQ(event.id, 0);
    EXPECT_EQ(event.client, "");

    EXPECT_TRUE(check_event(event, "00:00 3"));
    EXPECT_EQ(event.time.tm_hour, 0);
    EXPECT_EQ(event.time.tm_min, 0);
    EXPECT_EQ(event.id, 0);
    EXPECT_EQ(event.client, "");

    EXPECT_TRUE(check_event(event, "23:59 5 -_"));

    EXPECT_EQ(event.time.tm_hour, 0);
    EXPECT_EQ(event.time.tm_min, 0);
    EXPECT_EQ(event.id, 0);
    EXPECT_EQ(event.client, "");
}

TEST(FormatTest, CheckEventTimeDecrease) 
{
    Event event = {};
    EXPECT_FALSE(check_event(event, "09:54 2 client1 1"));
    EXPECT_FALSE(check_event(event, "10:25 2 client2 2"));
    EXPECT_TRUE(check_event(event, "09:58 1 client3"));
}

/*check_format tests*/
TEST_F(FileTest, WrongStartFormat)
{
    RedirectCout();
    CompClub club_info{};
    bool res = check_format(club_info, GetInputFile().c_str());
    ReturnCout();
    ASSERT_TRUE(res);
    CompareOutputs();
}

TEST_F(FileTest, WrongEndFormat)
{
    RedirectCout();
    CompClub club_info{};
    bool res = check_format(club_info, GetInputFile().c_str());
    ReturnCout();
    ASSERT_TRUE(res);
    CompareOutputs();
}

TEST_F(FileTest, WrongTableInEvent)
{
    RedirectCout();
    CompClub club_info{};
    bool res = check_format(club_info, GetInputFile().c_str());
    ReturnCout();
    ASSERT_TRUE(res);
    CompareOutputs();
}

TEST_F(FileTest, WrongTableInEvent2)
{
    RedirectCout();
    CompClub club_info{};
    bool res = check_format(club_info, GetInputFile().c_str());
    ReturnCout();
    ASSERT_TRUE(res);
    CompareOutputs();
}

TEST_F(FileTest, WrongEventFormat)
{
    RedirectCout();
    CompClub club_info{};
    bool res = check_format(club_info, GetInputFile().c_str());
    ReturnCout();
    ASSERT_TRUE(res);
    CompareOutputs();
}

TEST_F(FileTest, MissingTableCount)
{
    RedirectCout();
    CompClub club_info{};
    bool res = check_format(club_info, GetInputFile().c_str());
    ReturnCout();
    ASSERT_TRUE(res);
    CompareOutputs();
}

TEST_F(FileTest, MissingStartTime)
{
    RedirectCout();
    CompClub club_info{};
    bool res = check_format(club_info, GetInputFile().c_str());
    ReturnCout();
    ASSERT_TRUE(res);
    CompareOutputs();
}

TEST_F(FileTest, MissingEndTime)
{
    RedirectCout();
    CompClub club_info{};
    bool res = check_format(club_info, GetInputFile().c_str());
    ReturnCout();
    ASSERT_TRUE(res);
    CompareOutputs();
}

TEST_F(FileTest, MissingPrice)
{
    RedirectCout();
    CompClub club_info{};
    bool res = check_format(club_info, GetInputFile().c_str());
    ReturnCout();
    ASSERT_TRUE(res);
    CompareOutputs();
}

TEST_F(FileTest, MissingLine)
{
    RedirectCout();
    CompClub club_info{};
    bool res = check_format(club_info, GetInputFile().c_str());
    ReturnCout();
    ASSERT_TRUE(res);
    CompareOutputs();
}

TEST_F(FileTest, TimeDecreased)
{
    RedirectCout();
    CompClub club_info{};
    bool res = check_format(club_info, GetInputFile().c_str());
    ReturnCout();
    ASSERT_TRUE(res);
    CompareOutputs();
}

TEST_F(FileTest, EventAfterClosing)
{
    RedirectCout();
    CompClub club_info{};
    bool res = check_format(club_info, GetInputFile().c_str());
    ReturnCout();
    ASSERT_TRUE(res);
    CompareOutputs();
}

TEST_F(FileTest, ReversedTimestamps)
{
    RedirectCout();
    CompClub club_info{};
    bool res = check_format(club_info, GetInputFile().c_str());
    ReturnCout();
    ASSERT_TRUE(res);
    CompareOutputs();
}
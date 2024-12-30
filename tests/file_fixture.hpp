#ifndef FILE_TEST_FIXTURE_H
#define FILE_TEST_FIXTURE_H
 
#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <filesystem>

class FileTest : public ::testing::Test
{
protected:
    std::string input_file;
    std::string output_file;
    std::string expected_output_file;
    std::streambuf* coutbuf;
    std::ofstream out_file_buf;

    virtual void SetUp()
    {
        std::string test_name = ::testing::UnitTest::GetInstance()->current_test_info()->name();
        // Burn this with fire...
        auto p = std::filesystem::current_path().parent_path();
        input_file = p.string() + "\\tests\\in\\" + test_name + "_in.txt";
        expected_output_file = p.string() + "\\tests\\out\\" + test_name + "_out.txt";
        output_file = "output.txt";
        
        ASSERT_TRUE(std::filesystem::exists(input_file));
        ASSERT_TRUE(std::filesystem::exists(expected_output_file));
    }

    virtual void TearDown()
    {
        if (std::filesystem::exists(output_file))
        {
            std::filesystem::remove(output_file);
        } else 
        {
            std::cout << "Couldn't remove output file!\n";
        }
    }

    void RedirectCout()
    {
        out_file_buf = std::ofstream(output_file);
        coutbuf = std::cout.rdbuf();
        std::cout.rdbuf(out_file_buf.rdbuf());
    }

    void ReturnCout()
    {
        out_file_buf.close();
        std::cout.rdbuf(coutbuf);
    }

    std::string GetInputFile()
    {
        return input_file;
    }

    std::string GetOutputFile()
    {
        return output_file;
    }

    void CompareOutputs()
    {
        std::ifstream output_stream(output_file);
        std::ifstream expected_stream(expected_output_file);
        
        std::string output_line;
        std::string expected_line;
        
        while (std::getline(output_stream, output_line) && 
                std::getline(expected_stream, expected_line))
        {
            ASSERT_EQ(output_line, expected_line) << "DIFF: " + output_line + "\n" + expected_line;
        }

        // No more to read in files
        std::string line;
        ASSERT_FALSE((output_stream >> line) && 
                     (expected_stream >> line)) << "LAST READ: " + output_line + "\n" + expected_line;
        
        output_stream.close();
        expected_stream.close();
    }
};

#endif
#include <palindrome_number.hpp>

#include <gtest/gtest.h>

TEST(Palindrome_number, first)
{
    EXPECT_FALSE(is_palindrome(12));
    EXPECT_FALSE(is_palindrome(123));
    EXPECT_FALSE(is_palindrome(99198));
    EXPECT_FALSE(is_palindrome(5512215));

    EXPECT_TRUE(is_palindrome(1));
    EXPECT_TRUE(is_palindrome(121));
    EXPECT_TRUE(is_palindrome(9223229));
    EXPECT_TRUE(is_palindrome(55122155));
}
#include "pch.h"
#include "..\lib\Either.h"

namespace Tests
{

	TEST(EitherTests, MatchWhenRight)
	{
		constexpr auto errorCode = 35; // left
		auto resultIfLeft = std::to_string(errorCode);

		// assign a left type value
		libmonad::Either<int, std::string> errorOrMessage = errorCode;

		// What to return if its actually a string, i.e a right type
		const std::string result = errorOrMessage.Match(
			[=](int) {return  resultIfLeft; },
			[](std::string errorMessage) {return errorMessage;});
		
		EXPECT_EQ(result, resultIfLeft);

		// What to return when its actually a integer, i.e a left type
		const int result1 = errorOrMessage.Match(
			[=](const int error) {return  error; },
			[](const std::string&) {return -1;});
		
		EXPECT_EQ(result1, errorCode);
	}


	TEST(EitherTests, MatchWhenLeft)
	{
		constexpr auto errorMessage = "35";

		// assign a right type value
		libmonad::Either<int, std::string> errorOrMessage = errorMessage;

		// What to return if its actually a string, i.e a right type
		const std::string result = errorOrMessage.Match(
			[=](int) {return  "Error"; },
			[](std::string errorMessage) {return errorMessage;});
		
		EXPECT_EQ(result, errorMessage);

		// What to return when its actually a integer, i.e a left type
		const int result1 = errorOrMessage.Match(
			[=](const int error) {return  -1; },
			[](const std::string&) {return 35;});
		
		EXPECT_EQ(result1, 35);
	}


	TEST(EitherTests, IfLeft)
	{
		constexpr auto errorCode = 35; // left
		auto resultIfLeft = std::to_string(errorCode);

		// assign a left type value
		libmonad::Either<int, std::string> errorOrMessage = errorCode;

		// What to return if its actually a int, i.e a left type
		const std::string result = errorOrMessage.IfLeft([=](int code) {return resultIfLeft;}); 
		
		EXPECT_EQ(result, resultIfLeft);

		// What to return when its actually a string, i.e a right type
		const int result1 = errorOrMessage.IfRight([](const std::string&) {return -1;});
		
		EXPECT_EQ(result1, errorCode);
	}


	TEST(EitherTests, IfRight)
	{
		constexpr auto errorMessage = "35";

		// assign a right type value
		libmonad::Either<int, std::string> errorOrMessage = errorMessage;

		// What to return if its actually a int, i.e a left type
		const std::string result = errorOrMessage.IfLeft(
			[=](int) {return  "Error"; });
		
		EXPECT_EQ(result, errorMessage);

		// What to return when its actually a string, i.e a right type
		const int result1 = errorOrMessage.IfRight([](const std::string&) {return 35;});
		
		EXPECT_EQ(result1, 35);
	}
}

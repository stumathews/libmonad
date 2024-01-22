#include "pch.h"
#include "..\lib\Either.h"
using namespace libmonad;

namespace Tests
{

	TEST(EitherTests, Match)
	{
		Either<int, const char*> either = "A phrase";

		// To extract the value or determine what it is, you match.
		// Match is important as it forces the caller to deal with either case, i.e when it s an integer or when its a string
		either.Match([](int leftValue)
		{
			std::cout << "Was an integer" << std::endl;
		}, [](const char* rightValue)
		{
			std::cout << "Was an string" << std::endl;
		});
	}

	TEST(EitherTests, MatchWhenRight)
	{
		constexpr auto errorCode = 35; // left
		auto resultIfLeft = std::to_string(errorCode);

		// assign a left type value
		Either<int, std::string> errorOrMessage = errorCode;

		// What to return if its actually a string, i.e a right type
		const std::string result = errorOrMessage.When(
			[=](int) {return  resultIfLeft; },
			[](std::string errorMessage) {return errorMessage;});
		
		EXPECT_EQ(result, resultIfLeft);

		// What to return when its actually a integer, i.e a left type
		const int result1 = errorOrMessage.When(
			[=](const int error) {return  error; },
			[](const std::string&) {return -1;});
		
		EXPECT_EQ(result1, errorCode);
	}


	TEST(EitherTests, MatchWhenLeft)
	{
		constexpr auto errorMessage = "35";

		// assign a right type value
		Either<int, std::string> errorOrMessage = errorMessage;

		// What to return if its actually a string, i.e a right type
		const std::string result = errorOrMessage.When(
			[=](int) {return  "Error"; },
			[](std::string errorMessage) {return errorMessage;});
		
		EXPECT_EQ(result, errorMessage);

		// What to return when its actually a integer, i.e a left type
		const int result1 = errorOrMessage.When(
			[=](const int error) {return  -1; },
			[](const std::string&) {return 35;});
		
		EXPECT_EQ(result1, 35);
	}


	TEST(EitherTests, IfLeft)
	{
		constexpr auto errorCode = 35; // left
		auto resultIfLeft = std::to_string(errorCode);

		// assign a left type value
		Either<int, std::string> errorOrMessage = errorCode;

		// What to return if its actually a int, i.e a left type
		const std::string result = errorOrMessage.WhenLeft([=](int code) {return resultIfLeft;}); 
		
		EXPECT_EQ(result, resultIfLeft);

		// What to return when its actually a string, i.e a right type
		const int result1 = errorOrMessage.WhenRight([](const std::string&) {return -1;});
		
		EXPECT_EQ(result1, errorCode);
	}


	TEST(EitherTests, IfRight)
	{
		constexpr auto errorMessage = "35";

		// assign a right type value
		Either<int, std::string> errorOrMessage = errorMessage;

		// What to return if its actually a int, i.e a left type
		const std::string result = errorOrMessage.WhenLeft(
			[=](int) {return  "Error"; });
		
		EXPECT_EQ(result, errorMessage);

		// What to return when its actually a string, i.e a right type
		const int result1 = errorOrMessage.WhenRight([](const std::string&) {return 35;});
		
		EXPECT_EQ(result1, 35);
	}

	TEST(EitherTests, IsLeftRightBottom)
	{
		Either<int, float> either;
		EXPECT_TRUE(either.IsBottom());
		EXPECT_FALSE(either.IsRight());
		EXPECT_FALSE(either.IsLeft());

		either = 25;
		EXPECT_TRUE(either.IsLeft());
		EXPECT_FALSE(either.IsRight());
		EXPECT_FALSE(either.IsBottom());

		either = 0.5f;

		EXPECT_FALSE(either.IsLeft());
		EXPECT_TRUE(either.IsRight());
		EXPECT_FALSE(either.IsBottom());

	}
}

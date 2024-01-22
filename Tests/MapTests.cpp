#include "pch.h"
#include "..\lib\Either.h"
using namespace libmonad;

namespace Tests
{	
	TEST(EitherTests, TryMapALeftValue)
	{		
		auto string5 = std::string("5");

		// Assign a left type value 
		Either<int, char> either = 5;

		// Map the right type (if it is a right type) to a string
		auto result = either.Map<std::string>([=](char c) { return string5; });

		// if its a right type (it isn't) return 15 - should not work
		const auto result2 = result.WhenRight([](std::string) { return 15;});

		// Ensure it stays at 5 as it was a left value so map should not transform it
		EXPECT_EQ(result2, 5);

	}

	TEST(EitherTests, TryMapARightValue)
	{		
		auto string5 = std::string("Five");

		// Assign a left type value 
		Either<int, char> either = '5';

		// Map the right type (if it is a right type) to a string
		auto result = either.Map<std::string>([=](char c) { return string5; });

		// if its a left type (it isn't) return - should not work
		const auto result2 = result.WhenLeft([](int i) { return std::string("shit");});

		// Ensure it transformed to a string 'five' as it was a right value so map should transform it
		EXPECT_EQ(result2, string5);
	}

	TEST(EitherTests, ChainMap)
	{
		constexpr auto finalTransformation = 0.5f;

		// set as right type value
		Either<std::string, int> either = 5;

		auto result = either.Map<int>([](int i) { return 6;}) // transform int to int
		.Map<char>([](int i) { return '6';}) // transform int to char
		.Map<float>([=](char c) { return finalTransformation;}); // transform char to float

		// extract right value
		const auto result2 = result.WhenLeft([](const std::string&) { return 0.0f;});

		// Ensure right value was the last transformation at the end
		EXPECT_EQ(result2, finalTransformation);

	}

	TEST(EitherTests, ChainMapFail)
	{
		constexpr auto finalTransformation = 0.5f;

		// set as right type value
		Either<std::string, int> either{"Error"};

		auto result = either.Map<int>([](int i) { return 6;}) // not run
		.Map<char>([](int i) { return '6';}) // not run
		.Map<float>([=](char c) { return finalTransformation;}); // not run

		// extract right value
		const auto result2 = result.WhenLeft([](const std::string&) { return 0.0f;});

		// Ensure right value is the right value when its actually a left in the either
		EXPECT_EQ(result2, 0.0f);

	}
}
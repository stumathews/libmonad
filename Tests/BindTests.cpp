#include "pch.h"

#include <string>

#include "..\lib\Either.h"
using namespace libmonad;

namespace Tests
{	
	TEST(EitherTests, BasicBind)
	{
		// int is an error, string is a result of a successful operation:

		auto successfulOperation = []{ return std::string("Value is ABC");};

		Either<int, std::string> result = successfulOperation();

		Either<int, char> result2 = result.Bind<char>([](const Either<int, std::string>&)
		{
			return libmonad::Either<int, char>('c'); // transform to Either<int, char>
		});

		const auto result3 = result2.IfLeft([](int i) { return '0';});
		EXPECT_EQ(result3, 'c');
	}

	TEST(EitherTests, ChainBind)
	{
		// int is an error, string is a result of a successful operation:

		auto successfulOperation = []{ return std::string("Value is ABC");};

		Either<int, std::string> result = successfulOperation();

		Either<int, float> result2 = result.Bind<char>([](const Either<int, std::string>&)
		{
			return libmonad::Either<int, char>('c'); // transform to Either<int, char>
		}).Bind<float>([](Either<int, char> in)
		{
			return libmonad::Either<int, float>(99.0f); // transform to Either<int, float>
		});

		const auto result3 = result2.IfLeft([](int i) { return '0';});
		EXPECT_EQ(result3, 99.0f);
	}

	TEST(EitherTests, ShortCurcuit)
	{
		Either<int, std::string> code = 44;

		const Either<int, std::string> result = code.Map<std::string>([](std::string s){ return std::string("55");})
		.Map<std::string>([](std::string s){ return 22;}) // short circuits because we have a left value now
		.Map<std::string>([](std::string s){ return std::string("fish");});

		EXPECT_TRUE(result.IsLeft());
	}
}
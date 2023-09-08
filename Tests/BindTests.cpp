#include "pch.h"
#include "..\lib\Either.h"

namespace Tests
{	
	TEST(EitherTests, BasicBind)
	{
		// int is an error, string is a result of a successful operation:

		auto successfulOperation = []{ return std::string("Value is ABC");};

		libmonad::Either<int, std::string> result = successfulOperation();

		libmonad::Either<int, char> result2 = result.Bind<char>([](const libmonad::Either<int, std::string>&)
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

		libmonad::Either<int, std::string> result = successfulOperation();

		libmonad::Either<int, float> result2 = result.Bind<char>([](const libmonad::Either<int, std::string>&)
		{
			return libmonad::Either<int, char>('c'); // transform to Either<int, char>
		}).Bind<float>([](libmonad::Either<int, char> in)
		{
			return libmonad::Either<int, float>(99.0f); // transform to Either<int, float>
		});

		const auto result3 = result2.IfLeft([](int i) { return '0';});
		EXPECT_EQ(result3, 99.0f);
	}
}
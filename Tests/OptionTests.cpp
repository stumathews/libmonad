#include "pch.h"
#include "..\lib\Option.h"

using namespace libmonad;
using namespace std;

namespace Tests
{	
	TEST(OptionTests, Basics)
	{
		Option<int> maybeNumber;
		
		EXPECT_TRUE(maybeNumber.IsNone());
		EXPECT_TRUE(maybeNumber.IsNone());

		maybeNumber = 55;

		EXPECT_TRUE(maybeNumber.IsSome());
		EXPECT_FALSE(maybeNumber.IsNone());

		// map it
		auto result = maybeNumber.Map<float>([](int i)
		{
			return 26.75f;
		});

		EXPECT_TRUE(maybeNumber.IsSome());
		EXPECT_FALSE(maybeNumber.IsNone());

		// extract value
		auto floatValue = result.Match<float>(
			[]() { return 0.0f;},
			[](const float f) {return f;});

		EXPECT_EQ(floatValue, 26.75f);

		// nothing
		result = None();

		EXPECT_FALSE(result.IsSome());
		EXPECT_TRUE(result.IsNone());

		// map nothing
		floatValue = result.Match<float>(
			[]() { return 0.0f;},
			[](float f) {return f;});

		EXPECT_FALSE(result.IsSome());
		EXPECT_TRUE(result.IsNone());

		// extract value
		EXPECT_EQ(floatValue, 0.0f);
	}

	TEST(OptionTests, BindBasics)
	{
		Option<bool> success = true;

		auto a = success.Bind<string>([](bool b)
		{
			return Option<string>(string("true"));
		});

		auto result = a.Match<string>(
			[]() { return string("failed"); },
			[](string s){ return s;});

		EXPECT_FALSE(a.IsNone());
		EXPECT_TRUE(a.IsSome());
		EXPECT_EQ(result, "true");

		success = {};

		a = success.Bind<string>([](bool b)
		{
			return Option<string>(string("true"));
		});

		result = a.Match<string>(
			[]() { return string("failed"); },
			[](string s){ return s;});

		EXPECT_EQ(result, "failed");
		EXPECT_TRUE(a.IsNone());
		EXPECT_FALSE(a.IsSome());
	}
}
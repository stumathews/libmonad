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
		auto result = maybeNumber.Map<string>([](int i)
		{
			return string("26.75f");
		});

		EXPECT_TRUE(maybeNumber.IsSome());
		EXPECT_FALSE(maybeNumber.IsNone());

		// extract value
		auto floatValue = result.Match(
			[]() { return "0.0f";},
			[](const string& s) {return s;});

		EXPECT_EQ(floatValue, "26.75f");

		// nothing
		result = None();

		EXPECT_FALSE(result.IsSome());
		EXPECT_TRUE(result.IsNone());

		// map nothing
		floatValue = result.Match(
			[]() { return "0.0f";},
			[](string s) {return s;});

		EXPECT_FALSE(result.IsSome());
		EXPECT_TRUE(result.IsNone());

		// extract value
		EXPECT_EQ(floatValue, "0.0f");

		Either<int, string> maybe = 45;
	}

	TEST(OptionTests, BindBasics)
	{
		Option<bool> success = true;

		auto a = success.Bind<string>([](bool b)
		{
			return Option<string>(string("true"));
		});

		auto result = a.Match(
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

		result = a.Match(
			[]() { return string("failed"); },
			[](string s){ return s;});

		EXPECT_EQ(result, "failed");
		EXPECT_TRUE(a.IsNone());
		EXPECT_FALSE(a.IsSome());	

	}

	TEST(OptionTests, ShortCircuit)
	{
		Option<int> option = 25;

		const auto result = option
		                    .Map<int>([](const int i){ return i * 25;})
		                    .Map<string>([](const int i) { return to_string(i);})
		                    .Map<string>([](const string& s){ return None();})
		                    .Map<string>([](const string& s){ return string("failed");});

		EXPECT_TRUE(result.IsNone());
		EXPECT_FALSE(result.IsSome());

		Option<int> option1 = 25;
		const auto result2 = option1
		                     .Bind<string>([](int i) { return Option<string>("Hello");})
		                     .Bind<int>([](string s){ return None();});

		EXPECT_TRUE(result2.IsNone());
		EXPECT_FALSE(result2.IsSome());

	}
}
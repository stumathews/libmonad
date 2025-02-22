#include "pch.h"
#include "..\lib\Option.h"

using namespace libmonad;
using namespace std;

namespace Tests
{
	TEST(OptionTests, Match)
	{
		Option<int> maybeNumber = 25;
		maybeNumber = None();

		// extract value using match.
		// This is useful as it forces the caller to deal with a None  and a Some (Number) use-case
		maybeNumber.Match([](None none)
		{
			std::cout << "Was None" << std::endl;
		}, [](int number)
		{
			std::cout << "Was an number" << std::endl;
		});
	}
	
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

		// extract value using match
		auto floatValue = result.MatchTo(
			[]() { return "0.0f";},
			[](const string& s) {return s;});

		EXPECT_EQ(floatValue, "26.75f");

		// nothing
		result = None();

		EXPECT_FALSE(result.IsSome());
		EXPECT_TRUE(result.IsNone());

		// map nothing
		floatValue = result.MatchTo(
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

		auto result = a.MatchTo(
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

		result = a.MatchTo(
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
		                    .Map<string>([](const string& s){ return None();}) // short circuit
		                    .Map<string>([](const string& s){ return string("failed");});

		EXPECT_TRUE(result.IsNone());
		EXPECT_FALSE(result.IsSome());

		Option<int> option1 = 25;
		const auto result2 = option1
		                     .Bind<string>([](int i) { return Option<string>("Hello");})
		                     .Bind<int>([](string s){ return None();})
							 .Map<float>([](float f){ return 0.0f;}); // wont be run

		EXPECT_TRUE(result2.IsNone());
		EXPECT_FALSE(result2.IsSome());

	}

	TEST(OptionTests, WhenNone)
	{
		std::string expectedStringWhenNone = "NothingFound";
		const std::string expectedStringWhenNotNone = "IFoundSomethingWonderful!";

		Option<std::string> itemKey = None();

		const auto resultWhenNone = itemKey.WhenNone([&]{ return expectedStringWhenNone;});

		EXPECT_EQ(resultWhenNone, expectedStringWhenNone);

		// negative test...
		itemKey = expectedStringWhenNotNone;

		const auto resultWhenNotNone = itemKey.WhenNone([&]{ return expectedStringWhenNone;});		
		EXPECT_EQ(resultWhenNotNone, expectedStringWhenNotNone);

	}

	TEST(OptionTests, ThrowIfNone)
	{
		Option<std::string> maybeString {"Stuart"};

		// It should not throw as the option is set to a string
		EXPECT_NO_THROW(maybeString.ThrowIfNone());

		// Of course the whole point of ThrowIfNone() is to get the underlying value if its not a none:
		auto theString = maybeString.ThrowIfNone();

		// We got the string, make sure its correct
		EXPECT_STREQ(theString.c_str(), "Stuart");

		// Double check that it not a None (we know its not due to above)
		maybeString.Match(
			[](None none) {  FAIL(); },
			[](const string& str) { EXPECT_STREQ(str.c_str(), "Stuart"); });

		// Now unset it
		maybeString = None();

		// Should throw
		EXPECT_THROW(maybeString.ThrowIfNone(), std::exception);
		std::string message = "Bad juju! Value can't be none here!";

		try
		{
			// pass a message to use in the exception as a variable
			maybeString.ThrowIfNone(message);
			FAIL();
		}
		catch (std::exception& e)
		{
			// check it passed message to thrown exception
			EXPECT_STREQ(e.what(), message.c_str());
		}

		try
		{
			// pass a string literal as 
			maybeString.ThrowIfNone("Bad juju! Value can't be none here!"s);
			FAIL();
		}
		catch (std::exception& e)
		{
			// check it passed message to thrown exception
			EXPECT_STREQ(e.what(), "Bad juju! Value can't be none here!");
		}

	};
}
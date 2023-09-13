#include "pch.h"
#include "..\lib\Either.h"
#include "..\lib\Option.h"
using namespace libmonad;
using namespace std;

namespace Tests
{

	TEST(ExampleTests, Basics)
	{
		Either<int, string> result;
		cout << "IsBottom:" << result.IsBottom() << endl; // true

		result = 25;
		cout << "IsBottom:" << result.IsBottom() << endl; // false
		cout << "IsLeft:" << result.IsLeft() << endl; // true
		result = string("hello");

		cout << "IsBottom:" << result.IsBottom() << endl; // false
		cout << "IsRight:"<< result.IsRight() << endl; // true

		// Extract a number value from either
		const int number = result.Match(
			[](int i) { return i;}, // return the number if it has one
			[](string s) { return -1;}); // return a number if it doesn't contain one

		// Extract a string value from either
		const string str = result.Match(
			[](int i) { return to_string(i);}, // return a string if it doesn't contain one
			[](string s) { return s;}); // return string

		cout << "Number is: " << number << " String is: " << str << endl;

		EXPECT_EQ(number, -1);
		EXPECT_STREQ(str.c_str(), "hello");

	}

	TEST(ExampleTests, SimpleEither)
	{
		// Out either can either by a either with a left type value of int or a right type value of string
		Either<int, string> either = 25; // its an int now, a right type value
		either = string("something wonderful"); // its a string now, a left type value (can comment this line out to see how the rest of the code deals with it)

		// Transform the contents to another form. rule: we only map the right type value if the either contains it...
		Either<int, string> transformed = either.Map<string>([](const string& in)
		{
			// We map/transform the right type, i.e string to another right type, i.e a nother string
			return string("Got this: '") + in + string("', but this is even better!");
		});

		// it could have been a number, in which case we can tell it how to represent that number as a right-value (or string type)
		const string witherWay = transformed.IfLeft([](const int number) { return string("Could not transform correctly as it was a number:  ") + to_string(number) ;});

		// either way, out code now can deal with strings uniformly even if it was a number
		cout << witherWay << endl;
	}

	TEST(ExampleTests, MatchAndIfRight)
	{
		srand(time(nullptr));

		// This is a function that has variable outcomes
		auto impureFunction = []() -> Either<int, string>
		{
			// result can either be a error message or a return code
			Either<int, string> result;

			// What will it be...
			switch(rand() % 2)
			{
			  case 0:
			    // all good, no errors
			    result = 100;
			    break;
			  case 1:
			    // it failed this time with an error
			    result = string("The muli-factor hyper-optimization index exploded!");
			    break;
			  default:
			    // something weird happened, return an error
			    result = string("Unexpected result");
			  }

			return result;
		};

		// Call the function which could result in either an error message or a return code
		Either<int, string> result = impureFunction();

		// our downstream function only deals with error codes,
		auto downStreamFunction = [](const int number) { return number + 2; };
		
		// either way, lets see what actually the result is before interpreting it as a code.
		// Match can transform the result as right type (there is also an overload to turn it into a left type)
		const string resultAsString = result.Match(
			[](const int i){ return std::to_string(i);},  
			[](string s) {return s;}); // Could also use IfLeft does this return line implicitly

		// If it is a error message, we want that to be a code, and will make that a code of -1 (and report the error)
		const int code = result.IfRight([](const string& error)
		{
			cout << "We had an error: " << error << endl;
			return -1;
		});		

		// use a code
		const int done = downStreamFunction(code);

		cout << "result of downstream function was " << done  << " because code was " << code << " because result result was " << resultAsString << endl;

	}

	TEST(ExampleTests, Options)
	{
		// Declare and option of integer
		Option<int> result = 56;	

		auto yourMapFunction = [](int i) { };
		auto yourBindFunction = [](const int i) { return Option<int>(i);};

		// Transformation time:
		Option<string> final = result
			.Map<int>([](const int i)
			{
				return i * 12; // 672
			})
			.Map<int>([=](const int i)
			{
				yourMapFunction(i);
				return i;
			})
			.Bind<int>([=](int i)
			{
				return yourBindFunction(i);
			})
			.Map<string>([](const int i)
			{
				return to_string(i);
			});

		const string expected = final.Match(
			[]{ return string("failed"); }, // ifNone:
			[](string s){ return s;}); // ifSome:

		EXPECT_EQ(expected, "672");

	}
}
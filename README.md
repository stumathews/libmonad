# libmonad
C++ Library of Monads

Currently the following monads are supported:
- Either<L,R>
- Option<T>

To use link to libmonad.lib

## Examples:

### Either

An Either type can contain either one of two types of values. eg, number or string.

We'll interpret numbers are error codes, and we'll interpret strings as successful string transformations.

```cpp
Either<int, string> either = 25; //  its an int now, a right type value 
either = string("I made something wonderful"); //  its a string now, a left type value 
```
We can transform the right value to another right value

#### Map

```cpp
// Transform the contents to another form.
// rule: we only map the right type value if the either contains it...
Either<int, string> = either.Map<string>([](const string& in)
{
  // We map/transform the right type, i.e string to another right type, i.e another string
  return string("Got this: '") + in + string("', but this is even better!");
});
```

#### Bind
```cpp
// Can also use a bind Transform
Either<int, float> transformed2 = transformed.Bind<float>([](Either<int, string> in)
{
  return Either<int, float>(0.0f); // returns new Either with different Right type - float
});
```

#### Full example with Match and IfRight

Match allows you, irrespective of which type of value it contains, to be transformed to always a left type or always the right type. You chose.

The programmer defines what the transformation function that will do, depending on the version of the Match overload called, eg:

1. Match (L, R) -> R // this will allow you to always get a right value out of the either (you need to convert a left value to a right value)

You can also use IfLeft() which is simpler but does not allow you to modify the right value into something  

2. Match( L, R) -> L // this will allow you to always get a left value out of the either (you need to convert a right value to a left value)

   You can also use IfRight() which is simpler but does not allow you to modify the left value into something

Eg:
```cpp
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

// Call the function which could result in
// either an error message or a return code
Either<int, string> result = impureFunction();

// Now our downstream function only deals with error codes,
auto downStreamFunction = [](const int number) { return number + 2; };

// either way, lets see what actually the result is before interpreting it as a code
// Match can transform the result as right type (there is also an overload to turn it into a left type)
const string resultAsString = result.Match(
  [](const int i){ return std::to_string(i);},  
  [](string s) {return s;}); // Could also use IfLeft does this return line implicitly

// If it is a error message, we want that to be a code,
// and will make that a code of -1 (end report the error)
const int code = result.IfRight([](const string&)
{
  cout << "We had an error: " << error << endl;
  return -1;
});		

// use a code - will be either the code
// or -1 if we had and error and we got a string above
const int done = downStreamFunction(code);

cout << "result of downstream function was "
<< done
<< " because code was " << code
<< " because result result was " << resultAsString << endl;
```

#### Short circuiting
```cpp
Either<int, std::string> code = 44;

const Either<int, std::string> result =
code.Map<std::string>([](std::string s){ return std::string("55");});
code.Map<std::string>([](std::string s){ return 22;}); // short circuits because we have a left value now
code.Map<std::string>([](std::string s){ return std::string("fish");});

EXPECT_TRUE(result.IsLeft());
```

### Option<T>

```cpp
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
```

#### Short circuiting
```cpp
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
```

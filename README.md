# libmonad
C++ Library of Monads

Currently the following monads are supported:
- Either<L,R>

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
// Transform the contents to another form. rule: we only map the right type value if the either contains it...
auto transformed = either.Map<string>([](const string& in)
{
  // We map/transform the right type, i.e string to another right type, i.e another string
  return string("Got this: '") + in + string("', but this is even better!");
});
```
But if we couldn't, (because due to an error we got a left value of 25, say), we can still interpret it as a string, and continue our work. 

Why? Because we dont expect numbers in our results, but we can get them when errors occur, so we can cater from them and still continue to only care about strings

#### IfLeft 
```cpp
// the result could have been a number, in which case we can tell it how to represent that number as a right-value (or string type)
// If either did actually contain a transformed string (right type), you get it back, or you get to create a new string if it was a left-type (int).
// Eitherway you can still end up with a string
const auto witherWayAsString = transformed.IfLeft([](const int number)
{
  return string("Could not transform correctly as it was a number:  ") + to_string(number) ;
});

```
You can deal with the strings now in the rest of your code, irrespective if the result was in fact a string or a number to begin with.

```cpp
// either way, out code now can deal with strings uniformly even if it was a number
cout << UseMyString(witherWayAsString) << endl;
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
auto impureFunction = []()
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
auto result = impureFunction();

// Now our downstream function only deals with error codes,
auto downStreamFunction = [](const int number) { return number + 2; };

// either way, lets see what actually the result is before interpreting it as a code
// Match can transform the result as right type (there is also an overload to turn it into a left type)
const string resultAsString = result.Match(
  [](const int i){ return std::to_string(i);},  
  [](string s) {return s;}); // Could also use IfLeft does this return line implicitly

// If it is a error message, we want that to be a code, and will make that a code of -1 (end report the error)
const auto code = result.IfRight([](const string&)
{
  cout << "We had an error: " << error << endl;
  return -1;
});		

// use a code - will be either the code or -1 if we had and error and we got a string above
const auto done = downStreamFunction(code);

cout << "result of downstream function was " << done  << " because code was " << code << " because result result was " << resultAsString << endl;
```

# libmonad
C++ Library of Monads

Currently the following monads are supported:
- Either<L,R>

## Examples:

### Either

```cpp
// Declare an either - it can contain a left type value of int or a right type value of string
Either<int, string> either = 25; //  its an int now, a right type value
either = string("something wonderful"); //  its a string now, a left type value
```
We can transform the right value to another right value

#### Map

```cpp
// Transform the contents to another form. rule: we only map the right type value if the either contains it...
auto transformed = either.Map<string>([](const string& in)
{
  // We map/transform the right type, i.e string to another right type, i.e a nother string
  return string("Got this: '") + in + string("', but this is even better!");
});
```
But if we couldn't, (because it maybe was a left value of 25, say), we can still interpret as a string:

#### IfLeft 
```cpp
// it could have been a number, in which case we can tell it how to represent that number as a right-value (or string type)
// If either originally contained a string (right type), you get it, or you get new string if it was a left-type (int).
const auto witherWayAsString = transformed.IfLeft([](const int number) { return string("Could not transform correctly as it was a number:  ") + to_string(number) ;});

```
You can deal with the strings now in the rest of your code, irrespective if the result was in fact a string or a number to begin with.

```cpp
// either way, out code now can deal with strings uniformly even if it was a number
cout << witherWayAsString << endl;
```
#### Full example with Match and IfRight

Match allows you, irrespective of which type of value it contains, to be transformed to always a left type or always the right type.

The programmer defines the transformation function that will do that depending on the version of the Match overload called:

- Match (L, R) -> R // this will allow you to always get a right value out of the either (you need to convert a left value to a right value)
  - You can also use IfLeft() which is simpler but does not allow you to modify the right value into something  
- Match( L, R) -> L // this will allow you to always get a left value out of the either (you need to convert a right value to a left value)
  - You can also use IfRight() which is simpler but does not allow you to modify the left value into something  
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
      result = string("There was an error");
      break;
    default:
      // something weird happened, return an error
      result = string("Unexpected result");
    }

  return result;
};

// Call the function which could result in either an error message or a return code
auto result = impureFunction();

// our downstream function only deals with error codes,
auto downStreamFunction = [](const int number) { return number + 2; };

// either way, lets see what actually the result is before interpreting it as a code
// Match can transform the result as right type (there is also an overload to turn it into a left type)
const string resultAsString = result.Match(
  [](const int i){ return std::to_string(i);},  
  [](string s) {return s;}); // Could also use IfLeft does this return line implicitly

// If it is a error message, we want that to be a code, and will make that a code of -1
const auto code = result.IfRight([](const string&){ return -1; });		

// use a code
const auto done = downStreamFunction(code);

cout << "result of downstream function was " << done  << " because code was " << code << " because result result was " << resultAsString << endl;
```

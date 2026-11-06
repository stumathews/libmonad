#include <Either.h>
#include <iostream>
#include <string>

int main()
{
    libmonad::Either<int, std::string> result =
        std::string("Hello from libmonad!");

    return 0;
}

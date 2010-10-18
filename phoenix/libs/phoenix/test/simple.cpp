
#include <iostream>

#include <boost/phoenix/core.hpp>

namespace phoenix = boost::phoenix;

int main()
{
    using namespace phoenix::placeholders;

    //phoenix::val(8);
    std::cout << (phoenix::val(8) + 9)() << "\n";
    std::cout << (phoenix::ref(9))() << "\n";

    int j = 9;
    int & i = phoenix::ref(j)();
    std::cout << (arg1)(0) << "\n";

    std::cout << (arg1 + 5 * 10 + arg1)(2) << "\n";
}

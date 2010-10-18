
#include <iostream>

#include <boost/phoenix/core.hpp>

namespace boost { namespace phoenix {

    template <typename Actions>
    struct phoenix_algorithm<proto::tag::plus, Actions>
        : proto::bind<proto::plus<meta_grammar<Actions>, meta_grammar<Actions> >, Actions>
    {};

    template <typename Actions>
    struct phoenix_algorithm<proto::tag::multiplies, Actions>
        : proto::bind<proto::multiplies<meta_grammar<Actions>, meta_grammar<Actions> >, Actions>
    {};
}}

namespace phoenix = boost::phoenix;

int main()
{
    using namespace phoenix::placeholders;

    //phoenix::val(8);
    std::cout << (phoenix::val(8) + 9)(0) << "\n";
    std::cout << (phoenix::val(8) * 9)(0) << "\n";
    std::cout << (phoenix::cref(9))() << "\n";

    int j = 9;
    int & i = phoenix::ref(j)();
    std::cout << (arg1)(0) << "\n";

    std::cout << (arg1 + 5 * 10 + arg1)(2) << "\n";
}

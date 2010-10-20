
#include <iostream>

#include <boost/phoenix/core.hpp>
#include <boost/phoenix/core/expression.hpp>
#include <boost/phoenix/statement/for.hpp>

namespace boost { namespace phoenix {

    template <>
    struct phoenix_rules<proto::tag::plus>
        : detail::rule<proto::plus<meta_grammar<>, meta_grammar<> > >
    {};

    template <>
    struct phoenix_rules<proto::tag::multiplies>
        : detail::rule<proto::multiplies<meta_grammar<>, meta_grammar<> > >
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

    for_(phoenix::val(8), phoenix::val(false), phoenix::val(0))[phoenix::val(9)]();
}

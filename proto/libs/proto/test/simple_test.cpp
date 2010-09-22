/*******************************************************************************
 *               Copyright 2010 Thomas Heller
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/

#include <boost/proto/core.hpp>
#include <boost/proto/visitor/cases.hpp>
#include <boost/proto/visitor/grammar.hpp>
#include <boost/proto/visitor/visit.hpp>

namespace proto = boost::proto;

// visitor specific to a given tag
template <typename Tag> struct visitor;

// grammar specific to a given tag
template <typename Tag> struct grammar;

int main()
{
    typedef proto::terminal<int>::type terminal_type;

    typedef proto::grammar<visitor, grammar> test_grammar;

    BOOST_MPL_ASSERT_MSG((proto::matches<terminal_type, test_grammar>::value), NO_MATCH, (test_grammar, terminal_type));
}
#include <boost/proto/visitor/visitor.hpp>
#include <boost/proto/visitor/detail/invoke_impl.hpp>

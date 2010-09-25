/*******************************************************************************
 *               Copyright 2010 Thomas Heller
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/

#include <boost/proto/core.hpp>
#include <boost/proto/visitor.hpp>

namespace proto = boost::proto;

// visitor specific to a given tag
template <typename Tag> struct visitor
    : proto::_
{};

// grammar specific to a given tag
template <typename Tag> struct grammar
    : proto::_
{};

int main()
{
    typedef proto::terminal<int>::type terminal_type;

    typedef proto::visitor<visitor, grammar> test_grammar;

    BOOST_MPL_ASSERT_MSG((proto::matches<terminal_type, test_grammar>::value), NO_MATCH, (test_grammar, terminal_type));
}

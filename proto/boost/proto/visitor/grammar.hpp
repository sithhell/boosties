/*******************************************************************************
 *               Copyright 2010 Thomas Heller
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/

#ifndef BOOST_PROTO_VISITOR_GRAMMAR_HPP
#define BOOST_PROTO_VISITOR_GRAMMAR_HPP

#include <boost/proto/matches.hpp>
#include <boost/proto/visitor/cases_fwd.hpp>
#include <boost/proto/visitor/grammar_fwd.hpp>

namespace boost { namespace proto {

template <
    template <typename> class Visitor
  , template <typename> class Grammar
  , typename DefaultGrammar   /* = proto::_ */
  , template <typename> class DefaultTransform /*= proto::_default*/
>
struct grammar
    : proto::switch_<
        cases<Visitor, Grammar, DefaultGrammar, DefaultTransform>
    >
{};

}}

#endif

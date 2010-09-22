/*******************************************************************************
 *               Copyright 2010 Thomas Heller
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/

#ifndef BOOST_PROTO_VISITOR_CASES_FWD_HPP
#define BOOST_PROTO_VISITOR_CASES_FWD_HPP

namespace boost { namespace proto {

template <
    template <typename> class Visitor
  , template <typename> class Grammar
  , typename DefaultGrammar
>
struct cases;

}}

#endif

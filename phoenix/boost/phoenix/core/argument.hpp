/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010 Eric Niebler
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef PHOENIX_CORE_ARGUMENT_HPP
#define PHOENIX_CORE_ARGUMENT_HPP

#include <boost/mpl/bool.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/phoenix/core/actor.hpp>
#include <boost/phoenix/core/grammar.hpp>
#include <boost/phoenix/core/terminal.hpp>

namespace boost { namespace phoenix
{
    template <typename I>
    struct argument
    {
        typedef I type;
        typedef typename I::value_type value_type;
        static value_type const value = I::value;
    };
}}
  
namespace boost {
    template <typename I>
    struct is_placeholder<phoenix::argument<I> >
        : mpl::true_
    {};
}

namespace boost { namespace phoenix
{
#define BOOST_PHOENIX_ARGUMENT_N(_, N, name)                                    \
    actor<                                                                      \
        proto::terminal<argument<mpl::int_<N> > >::type const                   \
    > const BOOST_PP_CAT(name, BOOST_PP_INC(N)) = {};

    namespace placeholders
    {
        BOOST_PP_REPEAT(3, BOOST_PHOENIX_ARGUMENT_N, arg)
        BOOST_PP_REPEAT(3, BOOST_PHOENIX_ARGUMENT_N, _)
    }

    namespace arg_names
    {
        BOOST_PP_REPEAT(3, BOOST_PHOENIX_ARGUMENT_N, arg)
        BOOST_PP_REPEAT(3, BOOST_PHOENIX_ARGUMENT_N, _)
    }

}}

namespace boost
{
}

#endif


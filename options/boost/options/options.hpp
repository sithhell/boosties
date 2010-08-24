
/*******************************************************************************
 *               Copyright 2010 Joel Falcou, Thomas Heller, Eric Niebler
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/

#ifndef BOOST_OPTIONS_OPTIONS_HPP_INCLUDED
#define BOOST_OPTIONS_OPTIONS_HPP_INCLUDED

#include <boost/options/option_pack.hpp>
#include <boost/options/option_expr.hpp>

namespace boost { namespace options_impl
{
    struct options_
    {
        template <typename Opts>
        Opts const&
        operator[](Opts const& opts) const
        {
            BOOST_MPL_ASSERT((proto::matches<Opts, option_pack>));
            return opts;
        }
    };
} }

namespace boost
{
  ////////////////////////////////////////////////////////////////////////////////
  // options entry-point
  ////////////////////////////////////////////////////////////////////////////////
    options_impl::options_ const options = {};
}

#define BOOST_REGISTER_PARAMETERS(NAME)                         \
    struct NAME ## _ {};                                        \
    boost::options_impl::option_expr<                           \
        boost::proto::terminal<boost::options_impl::option<NAME ## _> >::type \
    > const NAME = {};                                          \
    /**/

#endif


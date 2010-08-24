
/*******************************************************************************
 *               Copyright 2010 Joel Falcou, Thomas Heller, Eric Niebler
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/

#ifndef BOOST_OPTIONS_OPTION_TERM_HPP
#define BOOST_OPTIONS_OPTION_TERM_HPP

#include <boost/options/option.hpp>

#include <boost/proto/core.hpp>

namespace boost { namespace options_impl
{
    struct option_term
        : proto::terminal<
            option< proto::_ >
        >
    {};
}}

#endif

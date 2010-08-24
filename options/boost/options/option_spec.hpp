
/*******************************************************************************
 *               Copyright 2010 Joel Falcou, Thomas Heller, Eric Niebler
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/

#ifndef BOOST_OPTIONS_OPTION_SPEC_HPP
#define BOOST_OPTIONS_OPTION_SPEC_HPP

#include <boost/options/option_term.hpp>

#include <boost/proto/core.hpp>

namespace boost { namespace options_impl
{
    struct option_spec
        : proto::when<
            proto::assign<
                option_term
              , proto::terminal<proto::_>
            >
          , proto::_value(proto::_right)
        >
    {};
}}

#endif

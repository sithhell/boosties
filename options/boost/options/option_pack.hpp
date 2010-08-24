/*******************************************************************************
 *               Copyright 2010 Joel Falcou, Thomas Heller, Eric Niebler
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/

#ifndef BOOST_OPTIONS_OPTION_PACK_HPP
#define BOOST_OPTIONS_OPTION_PACK_HPP

#include <boost/options/option_spec.hpp>
#include <boost/options/option_not_found.hpp>

#include <boost/proto/core.hpp>
#include <boost/proto/transform.hpp>

namespace boost { namespace options_impl
{
    struct option_pack
        : proto::or_<
            proto::when<
                proto::comma<option_pack, option_spec>
              , proto::if_<
                    proto::matches<proto::_left(proto::_right), proto::_state>()
                  , option_spec(proto::_right)
                  , option_pack(proto::_left)
                >
            >
          , proto::when<
                option_spec
              , proto::if_<
                    proto::matches<proto::_left, proto::_state>()
                  , option_spec
                  , option_not_found()
                >
            >
        >
    {};
}}

#endif

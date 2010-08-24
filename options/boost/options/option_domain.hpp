
/*******************************************************************************
 *               Copyright 2010 Joel Falcou, Thomas Heller, Eric Niebler
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/

#ifndef BOOST_OPTIONS_OPTION_DOMAIN_HPP
#define BOOST_OPTIONS_OPTION_DOMAIN_HPP

#include <boost/options/option_expr_fwd.hpp>

#include <boost/proto/core.hpp>

namespace boost { namespace options_impl
{
    struct option_domain
        : proto::domain<
            proto::pod_generator<
                option_expr
            >
        >
    {};
}}

#endif

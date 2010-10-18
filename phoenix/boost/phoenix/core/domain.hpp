/*=============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Eric Niebler
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_PHOENIX_DOMAIN_HPP
#define BOOST_PHOENIX_DOMAIN_HPP

#include <boost/proto/proto.hpp>

namespace boost { namespace phoenix
{
    template <typename Expr>
    struct actor;
    
    struct phoenix_domain
      : proto::domain<
        proto::pod_generator<actor>, 
        proto::_, proto::default_domain>
    {
        template <typename T>
        struct as_child : as_expr<T>
        {};
    };
}}

#endif


/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010 Eric Niebler
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef PHOENIX_CORE_VALUE_HPP
#define PHOENIX_CORE_VALUE_HPP

#include <boost/phoenix/core/actor.hpp>

namespace boost { namespace phoenix
{
    template <typename T>
    struct value
        : proto::terminal<T>
    {
        typedef actor<typename proto::terminal<T>::type const> type;
    };
    
    template <typename T, int N>
    struct value<T[N]>
        : proto::terminal<T>
    {
        typedef actor<typename proto::terminal<T const* >::type const> type;
    };

    template <typename T>
    actor<T> const &
    val(actor<T> const & t)
    {
        return t;
    }

    template <typename T>
    typename value<T>::type const
    val(T const & t)
    {
        typename value<T>::type const e = {t};
        return e;
    }

    template <typename T>
    typename value<T>::type const
    val(T & t)
    {
        typename value<T>::type const e = {t};
        return e;
    }
}}

#endif

/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010 Eric Niebler
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef PHOENIX_CORE_REFERENCE_HPP
#define PHOENIX_CORE_REFERENCE_HPP

#include <boost/ref.hpp>
#include <boost/phoenix/core/actor.hpp>
#include <boost/phoenix/core/terminal.hpp>

namespace boost { namespace phoenix
{
    template <typename T>
    struct reference
        : proto::terminal<reference_wrapper<T> >
    {
        typedef actor<typename proto::terminal<reference_wrapper<T> >::type const> type;
    };

    template <typename T>
    typename reference<T>::type const
    ref(T & t)
    {
        typename reference<T>::type const e = {boost::ref(t)};
        return e;
    };

    template <typename T>
    typename reference<T const>::type const
    cref(T const & t)
    {
        typename reference<T const>::type const e = {boost::cref(t)};
        return e;
    };

    // Call out boost::reference_wrapper for special handling
    template<typename T>
    struct is_custom_terminal<boost::reference_wrapper<T> >
      : mpl::true_
    {};

    // Special handling for boost::reference_wrapper
    template<typename T>
    struct unwrap_custom_terminal<boost::reference_wrapper<T> >
    {
        typedef T &result_type;

        T &operator()(boost::reference_wrapper<T> r) const
        {
            return r;
        }
    };
}}

#endif

/*******************************************************************************
 *               Copyright 2010 Thomas Heller
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/

#ifndef BOOST_PROTO_VISITOR_DETAIL_CAN_VISIT_HPP
#define BOOST_PROTO_VISITOR_DETAIL_CAN_VISIT_HPP

#include <boost/mpl/bool.hpp>

namespace boost { namespace proto {

namespace detail
{
    // type trait to check wether our visitor accepts the specific tag
    namespace impl
    {
        template<typename T, typename Void = void>
        struct can_visit_
          : mpl::false_
        {};

        template<typename T>
        struct can_visit_<T, typename T::proto_can_visit_>
          : mpl::true_
        {};
    };

    template <typename T>
    struct can_visit : impl::can_visit_<T> {};
}

}}

#endif

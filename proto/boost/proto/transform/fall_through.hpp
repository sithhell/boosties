/*******************************************************************************
 *               Copyright 2010 Thomas Heller
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/

#ifndef BOOST_PROTO_TRANSFORM_FALL_THROUGH_HPP
#define BOOST_PROTO_TRANSFORM_FALL_THROUGH_HPP

#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/proto/transform.hpp>
#include <boost/proto/fusion.hpp>

namespace boost { namespace proto {

namespace detail {
    template <typename Grammar, typename State, typename Data>
    struct traversal
    {
        traversal(State state, Data data) : state(state), data(data) {}
        State state;
        Data data;

        typedef void result_type;

        template <typename T>
        typename boost::enable_if<boost::proto::is_expr<T> >::type
        operator()(T const & t) const
        {
            typename Grammar::template impl<T const&, State, Data> g;
            //Grammar g;
            g(t, state, data);
        }

        template <typename T>
        typename boost::disable_if<boost::proto::is_expr<T> >::type
        operator()(T const & t) const
        {}
    };

}

template <typename Grammar>
struct fall_through
    : boost::proto::transform<fall_through<Grammar> >
{
    template <typename Expr, typename State, typename Data>
    struct impl
        : boost::proto::transform_impl<Expr, State, Data>
    {
        typedef typename impl::expr_param result_type;

        result_type
        operator()(
            typename impl::expr_param expr
          , typename impl::state_param state
          , typename impl::data_param data
        ) const
        {
            boost::fusion::for_each(expr, detail::traversal<Grammar, typename impl::state_param, typename impl::data_param>(state, data));
            return expr;
        }
    };
};

}}

#endif

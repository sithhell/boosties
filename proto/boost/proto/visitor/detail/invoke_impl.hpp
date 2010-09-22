#ifndef BOOST_PP_IS_ITERATING
/*******************************************************************************
 *               Copyright 2010 Thomas Heller
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/

#ifndef BOOST_PROTO_VISITOR_DETAIL_INVOKE_IMPL_HPP
#define BOOST_PROTO_VISITOR_DETAIL_INVOKE_IMPL_HPP

#include <boost/proto/visitor/visitor.hpp>

namespace boost { namespace proto {

namespace detail
{
    // helper class to invoke the visitor with the right number of parameters
    template <
        typename Visitor
      , typename Expr
      , int arity = proto::arity_of<Expr>::value
    >
    struct invoke_impl;

#define BOOST_PROTO_DETAIL_INVOKE_IMPL_RESULT() \
        template <typename Sig> \
        struct result; \
        template <typename This> \
        struct result<This(Expr)> \
            : result_of::visitor<Visitor, Expr> \
        {}; \
        template <typename This, typename State> \
        struct result<This(Expr, State)> \
            : result_of::visitor<Visitor, Expr, State> \
        {}; \
        template <typename This, typename State, typename Data> \
        struct result<This(Expr, State, Data)> \
            : result_of::visitor<Visitor, Expr, State, Data> \
        {};
        /* */

#define BOOST_PROTO_DETAIL_INVOKE_IMPL_OPERATOR(e, N) \
        template <typename State> \
        typename result_of::visitor<Visitor, Expr>::type \
        operator()(Expr expr) const \
        { \
            return Visitor()(BOOST_PP_TUPLE_REM(N)e); \
        } \
        template <typename State> \
        typename result_of::visitor<Visitor, Expr, State&>::type \
        operator()(Expr expr, State& state) const \
        { \
            return Visitor()(BOOST_PP_TUPLE_REM(N)e, state); \
        } \
        template <typename State> \
        typename result_of::visitor<Visitor, Expr, State const&>::type \
        operator()(Expr expr, State const& state) const \
        { \
            return Visitor()(BOOST_PP_TUPLE_REM(N)e, state); \
        } \
        template <typename State, typename Data> \
        typename result_of::visitor<Visitor, Expr, State&, Data&>::type \
        operator()(Expr expr, State& state, Data& data) const \
        { \
            return Visitor()(BOOST_PP_TUPLE_REM(N)e, state, data); \
        } \
        template <typename State, typename Data> \
        typename result_of::visitor<Visitor, Expr, State const&, Data const&>::type \
        operator()(Expr expr, State const& state, Data const& data) const \
        { \
            return Visitor()(BOOST_PP_TUPLE_REM(N)e, state, data); \
        } \
        template <typename State, typename Data> \
        typename result_of::visitor<Visitor, Expr, State&, Data const&>::type \
        operator()(Expr expr, State& state, Data const& data) const \
        { \
            return Visitor()(BOOST_PP_TUPLE_REM(N)e, state, data); \
        } \
        template <typename State, typename Data> \
        typename result_of::visitor<Visitor, Expr, State const&, Data&>::type \
        operator()(Expr expr, State const& state, Data & data) const \
        { \
            return Visitor()(BOOST_PP_TUPLE_REM(N)e, state, data); \
        }
        /* */

    template <typename Visitor, typename Expr>
    struct invoke_impl<Visitor, Expr, 0>
    {
        BOOST_PROTO_DETAIL_INVOKE_IMPL_RESULT()
        BOOST_PROTO_DETAIL_INVOKE_IMPL_OPERATOR((expr), 1)
    };

#define BOOST_PROTO_CHILD_N(Z, N, _)                                            \
        proto::child_c<N>(expr)                   

#define BOOST_PP_ITERATION_PARAMS_1                                             \
        (3, (1, BOOST_PROTO_MAX_ARITY, <boost/proto/visitor/detail/invoke_impl.hpp>))      \

#include BOOST_PP_ITERATE()
}

}}

#endif

#else /* BOOST_PP_IS_ITERATING */

#define N BOOST_PP_ITERATION()
    
    template <typename Visitor, typename Expr>
    struct invoke_impl<Visitor, Expr, N>
    {
        BOOST_PROTO_DETAIL_INVOKE_IMPL_RESULT()
        BOOST_PROTO_DETAIL_INVOKE_IMPL_OPERATOR((BOOST_PP_ENUM(N, BOOST_PROTO_CHILD_N, _)), N)
    };


#endif

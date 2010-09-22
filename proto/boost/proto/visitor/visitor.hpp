#ifndef BOOST_PP_IS_ITERATING
/*******************************************************************************
 *               Copyright 2010 Thomas Heller
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/

#ifndef BOOST_PROTO_VISITOR_VISITOR_HPP
#define BOOST_PROTO_VISITOR_VISITOR_HPP

#include <boost/mpl/bool.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

namespace boost { namespace proto {

    namespace visit
    {
        // type tags to determine what our visitor accepts
        typedef mpl::true_  packed;
        typedef mpl::false_ unpacked;
        typedef mpl::true_  with_state;
        typedef mpl::false_ without_state;
        typedef mpl::true_  with_data;
        typedef mpl::false_ without_data;
    }

#define BOOST_PROTO_VISITOR()               typedef void proto_can_visit_;
#define BOOST_PROTO_VISITOR_PACKEK()        typedef mpl::true_ proto_visit_packed_;
#define BOOST_PROTO_VISITOR_UNPACKED()      typedef mpl::false_ proto_visit_packed_;
#define BOOST_PROTO_VISITOR_WITH_STATE()    typedef mpl::true_ proto_visit_state_;
#define BOOST_PROTO_VISITOR_WITHOUT_STATE() typedef mpl::false_ proto_visit_state_;
#define BOOST_PROTO_VISITOR_WITH_DATA()     typedef mpl::true_ proto_visit_data_;
#define BOOST_PROTO_VISITOR_WITHOUT_DATA()  typedef mpl::false_ proto_visit_data_;

template <typename Pack, typename State, typename Data>
struct visitor
{
    BOOST_PROTO_VISITOR()
    typedef Pack  proto_visit_packed_;
    typedef State proto_visit_state_;
    typedef Data  proto_visit_data_;
};

namespace result_of
{
    // meta functions to calculate the result of our custom visitor,
    // need to specialize on arity here to pass the correct number of parameters
    template <typename Visitor, typename Expr, typename State = void, typename Data = void,
        int arity = proto::arity_of<Expr>::value >
    struct visitor;

#define BOOST_PROTO_RESULT_OF_VISITOR_(Expr, N)                                 \
    mpl::eval_if_c<                                                             \
        mpl::and_<                                                              \
            typename Visitor::proto_visit_state_                                \
          , typename Visitor::proto_visit_data_                                 \
        >::value                                                                \
      , boost::result_of<Visitor( BOOST_PP_TUPLE_REM(N)Expr, State, Data)>      \
      , mpl::eval_if_c<                                                         \
            Visitor::proto_visit_state_::value                                  \
          , boost::result_of<Visitor (BOOST_PP_TUPLE_REM(N)Expr, State)>        \
          , mpl::eval_if_c<                                                     \
                Visitor ::proto_visit_data_::value                              \
              , boost::result_of<Visitor(BOOST_PP_TUPLE_REM(N)Expr, Data)>      \
              , boost::result_of<Visitor(BOOST_PP_TUPLE_REM(N)Expr)>            \
            >                                                                   \
        >                                                                       \
    >
    /**/

#define BOOST_PROTO_CHILD_N_TYPE(Z, N, _)                                       \
    typedef typename proto::result_of::child_c<Expr, N> BOOST_PP_CAT(child,N);
    /*
    typedef typename                                                            \
        mpl::eval_if_c<                                                         \
            Visitor::proto_visit_packed_::value                                 \
          , mpl::identity<void>                                                 \
          , proto::result_of::child_c<Expr, N>                                  \
        >::type                                                                 \
        BOOST_PP_CAT(child,N);                                                  \
    */
    
    template <typename Visitor, typename Expr>
    struct visitor<Visitor, Expr, void, void, 0>
    {
        typedef typename
            mpl::eval_if_c<
                !Visitor::proto_visit_state_::value
              , boost::result_of<Visitor(Expr const&)>
              , mpl::identity<void>
            >::type
            type;
    };
    
    template <typename Visitor, typename Expr, typename State>
    struct visitor<Visitor, Expr, State, void, 0>
    {
        typedef typename
            mpl::eval_if_c<
                Visitor::proto_visit_state_::value
              , boost::result_of<Visitor(Expr, State)>
              , result_of::visitor<Visitor, Expr>
            >::type
            type;
    };

    template <typename Visitor, typename Expr, typename State, typename Data>
    struct visitor<Visitor, Expr, State, Data, 0>
    {
        typedef typename
            mpl::eval_if_c<
                Visitor::proto_visit_state_::value && Visitor::proto_visit_data_::value
              , boost::result_of<Visitor(Expr, State, Data)>
              , result_of::visitor<Visitor, Expr, State>
            >::type
            type;
    };
    
#define BOOST_PP_ITERATION_PARAMS_1                                             \
        (3, (1, BOOST_PROTO_MAX_ARITY, <boost/proto/visitor/visitor.hpp>))      \

#include BOOST_PP_ITERATE()

#undef BOOST_PROTO_RESULT_OF_VISITOR_
#undef BOOST_PROTO_CHILD_N_TYPE

}

}}

#endif

#else /* BOOST_PP_IS_ITERATING */

#define N BOOST_PP_ITERATION()

    template <typename Visitor, typename Expr>
    struct visitor<Visitor, Expr, void, void, N>
    {
        BOOST_PP_REPEAT(N, BOOST_PROTO_CHILD_N_TYPE, _)

        typedef typename
            mpl::eval_if_c<
                Visitor::proto_visit_packed_::value
              , mpl::eval_if_c<
                    !Visitor::proto_visit_state_::value
                  , boost::result_of<Visitor(Expr)>
                  , mpl::identity<void>
                >
              , mpl::eval_if_c<
                    !Visitor::proto_visit_state_::value
                  , boost::result_of<Visitor(BOOST_PP_ENUM_BINARY_PARAMS(N, typename child, ::type BOOST_PP_INTERCEPT))>
                  , mpl::identity<void>
                >
            >::type
            type;
    };

    template <typename Visitor, typename Expr, typename State>
    struct visitor<Visitor, Expr, State, void, N>
    {
        BOOST_PP_REPEAT(N, BOOST_PROTO_CHILD_N_TYPE, _)

        typedef typename
            mpl::eval_if_c<
                Visitor::proto_visit_packed_::value
              , mpl::eval_if_c<
                    Visitor::proto_visit_state_::value
                  , boost::result_of<Visitor(Expr, State)>
                  , result_of::visitor<Visitor, Expr>
                >
              , mpl::eval_if_c<
                    Visitor::proto_visit_state_::value
                  , boost::result_of<Visitor(BOOST_PP_ENUM_BINARY_PARAMS(N, typename child, ::type BOOST_PP_INTERCEPT), State)>
                  , result_of::visitor<Visitor, Expr>
                >
            >::type
            type;
    };

    template <typename Visitor, typename Expr, typename State, typename Data>
    struct visitor<Visitor, Expr, State, Data, N>
    {
        BOOST_PP_REPEAT(N, BOOST_PROTO_CHILD_N_TYPE, _)

        typedef typename
            mpl::eval_if_c<
                Visitor::proto_visit_packed_::value
              , mpl::eval_if_c<
                    Visitor::proto_visit_state_::value && Visitor::proto_visit_data_::value
                  , boost::result_of<Visitor(Expr, State, Data)>
                  , result_of::visitor<Visitor, Expr, State>
                >
              , mpl::eval_if_c<
                    Visitor::proto_visit_state_::value && Visitor::proto_visit_data_::value
                  , boost::result_of<Visitor(BOOST_PP_ENUM_BINARY_PARAMS(N, typename child, ::type BOOST_PP_INTERCEPT), State, Data)>
                  , result_of::visitor<Visitor, Expr, State>
                >
            >::type
            type;
    };

#endif

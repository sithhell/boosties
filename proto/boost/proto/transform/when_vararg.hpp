#ifndef BOOST_PP_IS_ITERATING

#ifndef BOOST_PROTO_TRANSFORM_WHEN_VARARG_HPP
#define BOOST_PROTO_TRANSFORM_WHEN_VARARG_HPP

#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_trailing_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>

namespace boost { namespace proto {

    namespace detail
    {
        template <
            typename Fun
          , typename Expr
          , typename State
          , typename Data
          , long Arity = arity_of<Expr>::value >
        struct when_vararg_impl;
    }

   #define BOOST_PP_ITERATION_PARAMS_1 (3, (0, BOOST_PP_DEC(BOOST_PROTO_MAX_ARITY), <boost/proto/transform/when_vararg.hpp>))
   #include BOOST_PP_ITERATE()

    namespace detail
    {
        template <typename R, typename Fun, typename Expr, typename State, typename Data>
        struct when_vararg_impl<R(vararg<Fun>), Expr, State, Data, 0>
            : proto::transform_impl<Expr, State, Data>
        {
            typedef
                typename mpl::if_c<
                    is_callable<R>::value
                  , proto::call<R(Fun(proto::_child_c<0>))> // "R" is a function to call
                  , proto::make<R(Fun(proto::_child_c<0>))> // "R" is an object to construct
                >::type
            which;
            
            typedef typename which::template impl<Expr, State, Data>::result_type result_type;
            
            result_type operator ()(
                typename when_vararg_impl::expr_param   e
              , typename when_vararg_impl::state_param  s
              , typename when_vararg_impl::data_param   d
            ) const
            {
                return typename which::template impl<Expr, State, Data>()(e, s, d);
            }
        };

        #define BOOST_PROTO_CHILD_M(_, N, __) Fun(proto::_child_c<N>)
        #define BOOST_PROTO_CHILD(N) BOOST_PP_ENUM(N, BOOST_PROTO_CHILD_M, _)

        #define BOOST_PROTO_WHEN_VARARG_IMPL(_, N, ARITY)                       \
        template <                                                              \
            typename R                                                          \
          , typename Fun                                                        \
          , BOOST_PP_ENUM_PARAMS(N, typename A)                                 \
          BOOST_PP_COMMA_IF(N) typename Expr                                    \
          , typename State                                                      \
          , typename Data>                                                      \
        struct when_vararg_impl<                                                \
            R(vararg<Fun> BOOST_PP_ENUM_TRAILING_PARAMS(N, A))                  \
          , Expr                                                                \
          , State                                                               \
          , Data                                                                \
          , ARITY>                                                              \
            : proto::transform_impl<Expr, State, Data>                          \
        {                                                                       \
            typedef                                                             \
                typename mpl::if_c<                                             \
                    is_callable<R>::value                                       \
                  , proto::call< /* "R" is a function to call*/                 \
                        R(                                                      \
                            BOOST_PROTO_CHILD(ARITY)                            \
                            BOOST_PP_ENUM_TRAILING_PARAMS(N, A)                 \
                        )                                                       \
                    >                                                           \
                  , proto::make< /* "R" is an object to construct*/             \
                        R(                                                      \
                            BOOST_PROTO_CHILD(ARITY)                            \
                            BOOST_PP_ENUM_TRAILING_PARAMS(N, A)                 \
                        )                                                       \
                    >                                                           \
                >::type                                                         \
            which;                                                              \
                                                                                \
            typedef                                                             \
                typename which::template impl<Expr, State, Data>::result_type   \
            result_type;                                                        \
                                                                                \
            result_type operator ()(                                            \
                typename when_vararg_impl::expr_param   e                       \
              , typename when_vararg_impl::state_param  s                       \
              , typename when_vararg_impl::data_param   d                       \
            ) const                                                             \
            {                                                                   \
                return                                                          \
                    typename which::template impl<Expr, State, Data>()(e, s, d);\
            }                                                                   \
        };
        /* */

        #define BOOST_PROTO_WHEN_VARARG_IMPL_N(_, N, __) \
        BOOST_PP_REPEAT(BOOST_PP_DEC(BOOST_PROTO_MAX_ARITY), BOOST_PROTO_WHEN_VARARG_IMPL, N)

        BOOST_PP_REPEAT_FROM_TO(1, BOOST_PP_INC(BOOST_PROTO_MAX_ARITY), BOOST_PROTO_WHEN_VARARG_IMPL_N, _)
    }

}}

#endif

#else // BOOST_PP_IS_ITERATING

#define N BOOST_PP_ITERATION()

    template <typename Grammar, typename R, typename Fun BOOST_PP_ENUM_TRAILING_PARAMS(N, typename A)>
    struct when<Grammar, R(vararg<Fun> BOOST_PP_ENUM_TRAILING_PARAMS(N, A))>
        : proto::transform<when<Grammar, R(vararg<Fun> BOOST_PP_ENUM_TRAILING_PARAMS(N, A))> >
    {
        typedef typename Grammar::proto_grammar proto_grammar;

        template <typename Expr, typename State, typename Data>
        struct impl
            : detail::when_vararg_impl<R(vararg<Fun> BOOST_PP_ENUM_TRAILING_PARAMS(N, A)), Expr, State, Data>
        {};
    };

#undef N

#endif

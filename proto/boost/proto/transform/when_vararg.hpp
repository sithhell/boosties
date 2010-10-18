#ifndef BOOST_PP_IS_ITERATING

#ifndef BOOST_PROTO_TRANSFORM_WHEN_VARARG_HPP
#define BOOST_PROTO_TRANSFORM_WHEN_VARARG_HPP

#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/control/if.hpp>
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
        #define BOOST_PROTO_CHILD_M(_, N, __) Fun(proto::_child_c<N>)
        #define BOOST_PROTO_CHILD(N) BOOST_PP_ENUM(N, BOOST_PROTO_CHILD_M, _)

        #define BOOST_PROTO_WHEN_VARARG_IMPL(__, N, ARITY)                      \
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
                        BOOST_PP_IF(                                            \
                            ARITY                                               \
                          , R(                                                  \
                                BOOST_PROTO_CHILD(ARITY)                        \
                                BOOST_PP_ENUM_TRAILING_PARAMS(N, A)             \
                            )                                                   \
                         , R(                                                   \
                                proto::_ BOOST_PP_ENUM_TRAILING_PARAMS(N,A)     \
                           )                                                    \
                        )                                                       \
                    >                                                           \
                  , proto::make< /* "R" is an object to construct*/             \
                        BOOST_PP_IF(                                            \
                            ARITY                                               \
                          , R(                                                  \
                                BOOST_PROTO_CHILD(ARITY)                        \
                                BOOST_PP_ENUM_TRAILING_PARAMS(N, A)             \
                            )                                                   \
                         , R(                                                   \
                                proto::_ BOOST_PP_ENUM_TRAILING_PARAMS(N,A)     \
                           )                                                    \
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

        BOOST_PP_REPEAT(BOOST_PP_INC(BOOST_PROTO_MAX_ARITY), BOOST_PROTO_WHEN_VARARG_IMPL_N, _)
        #undef BOOST_PROTO_WHEN_VARARG_IMPL_N
        #undef BOOST_PROTO_WHEN_VARARG_IMPL
        #undef BOOST_PROTO_CHILD
        #undef BOOST_PROTO_CHILD_M
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

#ifndef BOOST_PP_IS_ITERATING
/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010 Eric Niebler
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef PHOENIX_CORE_EXPRESSION_HPP
#define PHOENIX_CORE_EXPRESSION_HPP

#include <boost/proto/proto.hpp>

namespace boost { namespace phoenix
{
    template <typename Expr>
    struct actor;

    template <template <typename> class Actor, typename Tag, BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(5, typename A, void), typename Dummy = void>
    struct phoenix_expr_ex;

    template <typename Tag, BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(5, typename A, void), typename Dummy = void>
    struct phoenix_expr : phoenix_expr_ex<actor, Tag, BOOST_PP_ENUM_PARAMS(5, A)> {};

    struct default_domain_with_basic_expr
        : proto::domain<proto::use_basic_expr<proto::default_generator> >
    {};

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, 5, <boost/phoenix/core/expression.hpp>))
    #include BOOST_PP_ITERATE()

}}

#endif

#else

#define N BOOST_PP_ITERATION()

    template <template <typename> class Actor, typename Tag, BOOST_PP_ENUM_PARAMS(N, typename A)>
    struct phoenix_expr_ex<Actor, Tag, BOOST_PP_ENUM_PARAMS(N, A)>
        : proto::transform<phoenix_expr_ex<Actor, Tag, BOOST_PP_ENUM_PARAMS(N, A)>, int>
    {
        typedef typename proto::result_of::make_expr<Tag, default_domain_with_basic_expr, BOOST_PP_ENUM_PARAMS(N, A)>::type base_type;
        typedef Actor<base_type> type;
        typedef typename proto::nary_expr<Tag, BOOST_PP_ENUM_PARAMS(N, A)>::proto_grammar proto_grammar;
        
        static type const make(BOOST_PP_ENUM_BINARY_PARAMS(N, A, a))
        {
            type const e = {BOOST_PP_ENUM_PARAMS(N, a)};
            return e;
        }

        template<typename Expr, typename State, typename Data>
        struct impl
          : proto::detail::pass_through_impl<phoenix_expr_ex, Expr, State, Data>
        {};
        
        typedef Tag proto_tag;
#define BOOST_PHOENIX_ENUM_CHILDREN(_, N, __) typedef BOOST_PP_CAT(A, N) BOOST_PP_CAT(proto_child, N);
        BOOST_PP_REPEAT(N, BOOST_PHOENIX_ENUM_CHILDREN, _)
#undef BOOST_PHOENIX_ENUM_CHILDREN
    };

#endif


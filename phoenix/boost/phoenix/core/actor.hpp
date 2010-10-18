/*=============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Eric Niebler
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_PHOENIX_ACTOR_HPP
#define BOOST_PHOENIX_ACTOR_HPP

#include <boost/phoenix/core/domain.hpp>
#include <boost/phoenix/core/grammar.hpp>
#include <boost/phoenix/core/is_nullary.hpp>

namespace boost { namespace phoenix
{
    namespace detail
    {
        struct error_expecting_arguments
        {
            template <typename T>
            error_expecting_arguments(T const&) {}
        };
    }

    namespace result_of
    {
        template <typename Expr, typename A0 = void, typename A1 = void, typename Dummy = void>
        struct actor;

        template <typename Expr>
        struct actor<Expr>
        {
            typedef
                typename mpl::eval_if_c<
                    is_nullary<Expr>::value
                  , boost::result_of<eval_grammar(Expr const&, fusion::vector0<>&)>
                  , mpl::identity<detail::error_expecting_arguments>
                >::type
                type;
        };

        template <typename Expr, typename A0>
        struct actor<Expr, A0>
            : boost::result_of<eval_grammar(Expr const&, fusion::vector1<A0>&)>
        {};

        template <typename Expr, typename A0, typename A1>
        struct actor<Expr, A0, A1>
            : boost::result_of<eval_grammar(Expr const&, fusion::vector2<A0, A1>&)>
        {};
    }

    template <typename Expr>
    struct actor
    {
        BOOST_PROTO_BASIC_EXTENDS(Expr, actor<Expr>, phoenix_domain)
        BOOST_PROTO_EXTENDS_ASSIGN()
        BOOST_PROTO_EXTENDS_SUBSCRIPT()

        template <typename Sig>
        struct result;

        template <typename This, typename A0>
        struct result<This(A0)>
            : result_of::actor<Expr, A0>
        {};

        typename result_of::actor<Expr>::type
        operator()() const
        {
            fusion::vector0<> env;
            return eval(*this, env);
        }

        template <typename A0>
        typename result_of::actor<Expr, A0 const &>::type
        operator()(A0 const& a0) const
        {
            fusion::vector1<A0 const&> env(a0);
            return eval(*this, env);
        }

        template <typename A0, typename A1>
        typename result_of::actor<Expr, A0 const &, A1 const &>::type
        operator()(A0 const& a0, A1 const& a1) const
        {
            fusion::vector2<A0 const&, A1 const&> env(a0, a1);
            return eval(*this, env);
        }

        // add more ...
    };
}}

#endif


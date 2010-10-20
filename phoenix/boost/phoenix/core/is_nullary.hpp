/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010 Eric Niebler
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef PHOENIX_CORE_IS_NULLARY_HPP
#define PHOENIX_CORE_IS_NULALRY_HPP

#include <boost/phoenix/core/grammar.hpp>
#include <boost/phoenix/core/terminal.hpp>

namespace boost { namespace phoenix
{
    namespace detail
    {
        struct is_nullary_
        {
            template <typename Rule, typename Dummy = void>
            struct visit;
        };

        typedef meta_grammar<is_nullary_> is_nullary_eval;

        template <typename Rule, typename Dummy>
        struct is_nullary_::visit
            : proto::fold<
                proto::_
              , mpl::true_()
              , mpl::and_<
                    proto::lazy<detail::is_nullary_eval(proto::_)>
                  , proto::_state
                >()
            >
        {};

        template <typename Dummy>
        struct is_nullary_::visit<proto::terminal<proto::_>, Dummy>
            : proto::make<mpl::true_()>
        {};

        template <typename Dummy>
        struct is_nullary_::visit<custom_terminal_grammar, Dummy>
            : proto::make<mpl::true_()>
        {};

        template <typename Dummy>
        struct is_nullary_::visit<argument_grammar, Dummy>
            : proto::make<mpl::false_()>
        {};
    }

    template <typename Expr>
    struct is_nullary
        : boost::result_of<detail::is_nullary_eval(Expr)>::type
    {};
}}

#endif


/*=============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Eric Niebler
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_PHOENIX_CORE_TERMINAL_HPP
#define BOOST_PHOENIX_CORE_TERMINAL_HPP

#include <boost/phoenix/core/grammar.hpp>

namespace boost
{
    template <typename T>
    struct is_placeholder
        : mpl::false_
    {};
}

namespace boost { namespace phoenix
{
    template <typename T>
    struct is_custom_terminal : mpl::false_ {};

    template <typename T>
    struct unwrap_custom_terminal;

    struct custom_terminal_grammar
        : proto::if_<is_custom_terminal<proto::_value>()>
    {};

    template <typename Dummy>
    struct evaluator::visit<custom_terminal_grammar, Dummy>
        : proto::lazy<unwrap_custom_terminal<proto::_value>(proto::_value)>
    {};

    template <typename I>
    struct get_environment_arg
    {
        BOOST_PROTO_CALLABLE()

        template <typename Sig>
        struct result;

        template <typename This, typename Env>
        struct result<This(Env &)>
            : fusion::result_of::at<Env, I>
        {};

        template <typename Env>
        typename fusion::result_of::at<Env, I>::type
        operator()(Env & env) const
        {
            return fusion::at<I>(env);
        };
    };

    struct argument_grammar
        : proto::if_<boost::is_placeholder<proto::_value>()>
    {};
    
    template <typename Actions>
    struct evaluator::visit<argument_grammar, Actions>
        : proto::lazy<get_environment_arg<proto::_value>(proto::_state)>
    {};

    template <typename Dummy>
    struct phoenix_rules<proto::tag::terminal, Dummy>
        : detail::rules<
            argument_grammar
          , custom_terminal_grammar
          , proto::terminal<proto::_>
        >
    {};
}}

#endif

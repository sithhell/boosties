/*=============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Eric Niebler
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_PHOENIX_CORE_GRAMMAR_HPP
#define BOOST_PHOENIX_CORE_GRAMMAR_HPP

#include <boost/proto/proto.hpp>
    
namespace boost { namespace proto 
{
    template <typename Rule, typename Actions>
    struct bind
        : proto::when<Rule, typename Actions::template action<Rule> >
    {};
}}

namespace boost { namespace phoenix
{

    // forward declare the grammar
    template <typename Actions>
    struct meta_grammar;

    template <typename Actions>
    struct default_rule
        : proto::bind<proto::_, Actions>
    {};

    template <typename Tag, typename Actions>
    struct phoenix_algorithm
        : proto::not_<proto::_>
    {};

    template <typename Actions>
    struct grammar_cases
    {
        template <typename Tag>
        struct case_
            : phoenix_algorithm<Tag, Actions>
        {};
    };

    template <typename Actions>
    struct meta_grammar
        : proto::switch_<grammar_cases<Actions> >
    {};

    // forward declare the evaluator
    struct evaluator;

    typedef meta_grammar<evaluator> eval_grammar;

    struct evaluator
    {
        template <typename Rule, typename Dummy = void>
        struct action
            : proto::_default<eval_grammar>
        {};
    };

    eval_grammar const eval = eval_grammar();
}}

#endif

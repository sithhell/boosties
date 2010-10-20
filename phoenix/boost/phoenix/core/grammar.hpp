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
#include <boost/phoenix/core/detail/algorithm.hpp>
    
namespace boost { namespace phoenix
{
    // forward declare the evaluator
    struct evaluator;

    // forward declare the grammar
    template <typename Visitor = evaluator>
    struct meta_grammar;

    template <typename Tag, typename Dummy = void>
    struct phoenix_rules
        : proto::not_<proto::_>
    {};

    struct grammar_dispatch
    {
        template <typename Tag>
        struct case_
            : phoenix_rules<Tag>
        {};
    };

    template <typename Visitor>
    struct meta_grammar
        : detail::algorithm<grammar_dispatch, Visitor>
    {};

    struct evaluator
    {
        template <typename Rule, typename Actions = evaluator>
        struct visit
            : proto::_default<meta_grammar<Actions> >
        {};
    };

    typedef meta_grammar<evaluator> eval_grammar;
    eval_grammar const eval = eval_grammar();
}}

#endif

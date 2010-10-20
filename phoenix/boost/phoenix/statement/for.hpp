/*==============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2010 Eric Niebler
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_STATEMENT_FOR_HPP
#define PHOENIX_STATEMENT_FOR_HPP

#include <boost/phoenix/core/expression.hpp>
#include <boost/phoenix/core/grammar.hpp>

namespace boost { namespace phoenix
{
    // definition of the phoenix expression
    namespace tag
    {
        struct for_loop {};
    }

    template <
        typename Init
      , typename Cond
      , typename Step
      , typename Do
    >
    struct for_loop
        : phoenix_expr<
            tag::for_loop
          , Init
          , Cond
          , Step
          , Do
        >
    {};

    // Note: this part is currently missing in the old design, and not
    // simply not possible as done here
    struct for_grammar
        : for_loop<
            meta_grammar<> // Init
          , meta_grammar<> // Cond
          , meta_grammar<> // Step
          , meta_grammar<> // Do
        >
    {};

    template <>
    struct phoenix_rules<tag::for_loop>
        : detail::rule<for_grammar>
    {};

    // finally, we define the evaluation behaviour
    template <typename Visitor>
    struct for_eval
        : proto::callable
    {
        typedef void result_type;

        template <
            typename Env
          , typename Init
          , typename Cond
          , typename Step
          , typename Do>
        result_type
        operator()(
            Env& env
          , Init const& init
          , Cond const& cond
          , Step const& step
          , Do const& do_) const
        {
            for(
                meta_grammar<Visitor>()(init, env);
                meta_grammar<Visitor>()(cond, env);
                meta_grammar<Visitor>()(step, env)
            )
            {
                meta_grammar<Visitor>()(do_, env);
            }
        }
    };

    // attach the behaviour to our evaluator
    template <typename Actions>
    struct evaluator::visit<for_grammar, Actions>
        : proto::call<
            for_eval<Actions>(
                proto::_state      // Env
              , proto::_child_c<0> // Init
              , proto::_child_c<1> // Cond
              , proto::_child_c<2> // Step
              , proto::_child_c<3> // Do
            )
        >
    {};

    // Generator of the expression
    template <typename Init, typename Cond, typename Step>
    struct for_gen
    {
        for_gen(Init const& init, Cond const& cond, Step const& step)
            : init(init), cond(cond), step(step) {}

        template <typename Do>
        typename for_loop<Init, Cond, Step, Do>::type const
        operator[](Do const& do_) const
        {
            return for_loop<Init, Cond, Step, Do>::make(init, cond, step, do_);
        }

        Init const& init;
        Cond const& cond;
        Step const& step;
    };

    template <typename Init, typename Cond, typename Step>
    for_gen<Init, Cond, Step> const
    for_(Init const& init, Cond const& cond, Step const& step)
    {
        return for_gen<Init, Cond, Step>(init, cond, step);
    }


}}

#endif


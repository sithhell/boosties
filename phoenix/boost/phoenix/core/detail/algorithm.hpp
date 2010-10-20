#ifndef BOOST_PP_IS_ITERATING
/*=============================================================================
    Copyright (c) 2005-2010 Joel de Guzman
    Copyright (c) 2010 Eric Niebler
    Copyright (c) 2010 Thomas Heller

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_PHOENIX_CORE_DETAIL_ALGORITHM_HPP
#define BOOST_PHOENIX_CORE_DETAIL_ALGORITHM_HPP

#include <boost/proto/proto.hpp>
    
namespace boost { namespace phoenix
{
    namespace detail
    {
        template <typename Grammar, typename Visitor, typename IsRule = void>
        struct algorithm_case
            : Grammar
        {};

        template <typename Rule, typename Visitor, int RulesSize = Rule::size>
        struct algorithm_case_rule;

        template <typename Grammar, typename Visitor>
        struct algorithm_case<Grammar, Visitor, typename Grammar::is_rule>
            : algorithm_case_rule<Grammar, Visitor>
        {};

        template <typename Cases, typename Visitor>
        struct algorithm
            : proto::switch_<algorithm<Cases, Visitor> >
        {
            template <typename Tag>
            struct case_
                : algorithm_case<typename Cases::template case_<Tag>, Visitor>
            {};
        };

        template <typename Grammar>
        struct rule
        {
            typedef void is_rule;

            static int const size = 1;
            typedef Grammar rule0;
        };

        template <BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(5, typename Grammar0, void), typename Dummy = void>
        struct rules;

#define BOOST_PHOENIX_RULE_WITH_ACTION(_, N, __) \
        proto::when<typename Rule::BOOST_PP_CAT(rule, N), typename Visitor::template visit<typename Rule::BOOST_PP_CAT(rule, N)> >

#define BOOST_PHOENIX_RULE_TYPE_N(_, N, __) typedef BOOST_PP_CAT(Grammar, N) BOOST_PP_CAT(rule, N);

        #define BOOST_PP_ITERATION_PARAMS_1 (3, (1, 5, <boost/phoenix/core/detail/algorithm.hpp>))
        #include BOOST_PP_ITERATE()
    }
}}

#endif

#else

#define N BOOST_PP_ITERATION()

        template <typename Rule, typename Visitor>
        struct algorithm_case_rule<Rule, Visitor, N>
            : proto::or_<
                BOOST_PP_ENUM(N, BOOST_PHOENIX_RULE_WITH_ACTION, _)
              >
        {};
        
        template <BOOST_PP_ENUM_PARAMS(N, typename Grammar)>
        struct rules<BOOST_PP_ENUM_PARAMS(N, Grammar)>
        {
            typedef void is_rule;

            static int const size = N;
            BOOST_PP_REPEAT(N, BOOST_PHOENIX_RULE_TYPE_N, _)
        };
#endif


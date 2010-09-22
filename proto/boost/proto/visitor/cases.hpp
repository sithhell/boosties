/*******************************************************************************
 *               Copyright 2010 Thomas Heller
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/

#ifndef BOOST_PROTO_VISITOR_CASES_HPP
#define BOOST_PROTO_VISITOR_CASES_HPP

#include <boost/mpl/identity.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/proto/transform/when.hpp>
#include <boost/proto/visitor/cases_fwd.hpp>
#include <boost/proto/visitor/grammar_fwd.hpp>
#include <boost/proto/visitor/visit.hpp>

namespace boost { namespace proto {

namespace detail
{
    // select based upon if our grammar handles this tag
    template <
        template <typename> class Grammar
      , typename Tag
      , typename DefaultGrammar
    >
    struct select_grammar
    {
        private:
                template <typename C>
                static yes_type test(typename C::proto_grammar*);
                template <typename C>
                static no_type test(...);

        public:
            typedef typename
                mpl::eval_if_c<
                    (sizeof(yes_type) == sizeof(test<Grammar<Tag> >(0)))
                  , mpl::identity<Grammar<Tag> >
                  , mpl::identity<DefaultGrammar>
                >::type
                type;
    };
}

template <
    template <typename> class Visitor
  , template <typename> class Grammar
  , typename DefaultGrammar
>
struct cases
{
    template <typename Tag>
    struct case_
        : proto::when<
            // dispatch again, check wether our user supplied grammar handles this tag
            typename detail::select_grammar<Grammar, Tag, DefaultGrammar>::type
          , _visit<Visitor, Grammar, DefaultGrammar>
        >
    {};
};

}}

#endif

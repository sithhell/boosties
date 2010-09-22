/*******************************************************************************
 *               Copyright 2010 Thomas Heller
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/

#ifndef BOOST_PROTO_VISITOR_VISIT_HPP
#define BOOST_PROTO_VISITOR_VISIT_HPP

#include <boost/utility/result_of.hpp>
#include <boost/proto/transform/pass_through.hpp>
#include <boost/proto/visitor/grammar_fwd.hpp>
#include <boost/proto/visitor/visitor.hpp>
#include <boost/proto/visitor/detail/can_visit.hpp>
#include <boost/proto/visitor/detail/invoke_impl.hpp>

namespace boost { namespace proto {

template <
    template <typename> class Visitor
  , template <typename> class Grammar
  , typename DefaultGrammar
>
struct _visit
    : proto::transform<
        _visit<Visitor, Grammar, DefaultGrammar>
    >
{
        template <typename Expr, typename State, typename Data>
        struct impl
            : proto::transform_impl<Expr, State, Data>
        {
            typedef typename proto::tag_of<Expr>::type tag_type;
            typedef typename detail::can_visit<Visitor<tag_type> >::type can_visit;
            typedef
                proto::pass_through<
                    grammar<Visitor, Grammar, DefaultGrammar>
                > default_transform;

            typedef typename
                mpl::eval_if_c<
                    can_visit::value
                  , typename result_of::visitor<Visitor<tag_type>, typename impl::expr_param,  typename impl::state_param, typename impl::data_param>
                  , typename boost::result_of<default_transform(typename impl::expr_param, typename impl::state_param, typename impl::data_param)>
                >::type
                result_type;
            
            result_type operator()(
                typename impl::expr_param expr
              , typename impl::state_param state
              , typename impl::data_param data
            ) const
            {
                return this->invoke(expr, state, data, can_visit());
            }

            private:
                result_type invoke(
                    typename impl::expr_param expr
                  , typename impl::state_param state
                  , typename impl::data_param data
                  , mpl::false_
                ) const
                {
                    return default_transform()(expr, state, data);
                }

                result_type invoke(
                    typename impl::expr_param expr
                  , typename impl::state_param state
                  , typename impl::data_param data
                  , mpl::true_
                ) const
                {
                    return this->invoke_pack(
                        expr
                      , state
                      , data
                      , typename Visitor<tag_type>::proto_visit_packed_()
                      , typename Visitor<tag_type>::proto_visit_state_()
                      , typename Visitor<tag_type>::proto_visit_data_()
                    );
                }
                
                result_type invoke_pack(
                    typename impl::expr_param expr
                  , typename impl::state_param state
                  , typename impl::data_param data
                  , mpl::false_
                  , mpl::false_
                  , mpl::false_
                ) const
                {
                    return detail::invoke_impl<Visitor<tag_type>, typename impl::expr_param>()(expr);
                }
                
                result_type invoke_pack(
                    typename impl::expr_param expr
                  , typename impl::state_param state
                  , typename impl::data_param data
                  , mpl::false_
                  , mpl::true_
                  , mpl::false_
                ) const
                {
                    return detail::invoke_impl<Visitor<tag_type>, typename impl::expr_param>()(expr, state);
                }
                
                result_type invoke_pack(
                    typename impl::expr_param expr
                  , typename impl::state_param state
                  , typename impl::data_param data
                  , mpl::false_
                  , mpl::true_
                  , mpl::true_
                ) const
                {
                    return detail::invoke_impl<Visitor<tag_type>, typename impl::expr_param>()(expr, state, data);
                }

                result_type invoke_pack(
                    typename impl::expr_param expr
                  , typename impl::state_param state
                  , typename impl::data_param data
                  , mpl::true_
                  , mpl::false_
                  , mpl::false_
                ) const
                {
                    return Visitor<tag_type>()(expr);
                }

                result_type invoke_pack(
                    typename impl::expr_param expr
                  , typename impl::state_param state
                  , typename impl::data_param data
                  , mpl::true_
                  , mpl::true_
                  , mpl::false_
                ) const
                {
                    return Visitor<tag_type>()(expr, state);
                }

                result_type invoke_pack(
                    typename impl::expr_param expr
                  , typename impl::state_param state
                  , typename impl::data_param data
                  , mpl::true_
                  , mpl::true_
                  , mpl::true_
                ) const
                {
                    return Visitor<tag_type>()(expr, state, data);
                }
        };
};

}}

#endif

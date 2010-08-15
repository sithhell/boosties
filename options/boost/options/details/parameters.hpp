/*******************************************************************************
 *               Copyright 2010 Joel Falcou - Thomas Heller
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/
#ifndef BOOST_OPTIONS_DETAILS_PARAMETERS_HPP_INCLUDED
#define BOOST_OPTIONS_DETAILS__PARAMETERS_HPP_INCLUDED

#include <boost/options/details/insert.hpp>
#include <boost/proto/proto.hpp>

namespace  boost { namespace details
{
  struct param_domain;

  ////////////////////////////////////////////////////////////////////////////////
  // Parameters expression
  ////////////////////////////////////////////////////////////////////////////////
  template<typename Expr>
  struct param_expr : boost::proto::extends<Expr, param_expr<Expr>, param_domain>
  {
    typedef boost::proto::extends<Expr, param_expr<Expr>, param_domain> base_type;
    explicit param_expr(Expr const &expr = Expr()) : base_type(expr) {}

    BOOST_PROTO_EXTENDS_USING_ASSIGN(param_expr)
  };

  ////////////////////////////////////////////////////////////////////////////////
  // Parameters Grammar
  ////////////////////////////////////////////////////////////////////////////////
  struct param_grammar
  : boost::proto::or_<
          boost::proto::when< boost::proto::assign< boost::proto::_, boost::proto::_>
                            , _insert ( boost::proto::_value(boost::proto::_left)
                                      , boost::proto::_value(boost::proto::_right)
                                      ,boost::proto::_state
                                      )
                            >
        , boost::proto::when< boost::proto::comma<param_grammar,param_grammar>
                            , param_grammar ( boost::proto::_right
                                            , param_grammar(boost::proto::_left)
                                            )
                            >
                      >
  {};

  ////////////////////////////////////////////////////////////////////////////////
  // Parameters domain: perform copy of the parameters value
  ////////////////////////////////////////////////////////////////////////////////
  struct param_domain : proto::domain<proto::generator<param_expr> >
  {
    template<class T> struct as_child : as_expr<T> {};
  };
} }

////////////////////////////////////////////////////////////////////////////////
// Create a new parameters placeholder
////////////////////////////////////////////////////////////////////////////////
#define BOOST_REGISTER_PARAMETERS(Name)                                 \
struct BOOST_PP_CAT(tag_,Name) {};                                      \
boost::details::param_expr<boost::proto                                 \
                  ::terminal<BOOST_PP_CAT(tag_,Name)>::type> const Name \
/**/

#endif


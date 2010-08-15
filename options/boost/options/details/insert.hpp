/*******************************************************************************
 *               Copyright 2010 Joel Falcou - Thomas Heller
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/
#ifndef BOOST_OPTIONS_DETAILS_INSERT_HPP_INCLUDED
#define BOOST_OPTIONS_DETAILS_INSERT_HPP_INCLUDED

#include <boost/proto/proto.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/fusion/include/push_back.hpp>

namespace boost { namespace details
{
  ////////////////////////////////////////////////////////////////////////////////
  // Add a parameters into the parameters map
  ////////////////////////////////////////////////////////////////////////////////
  struct _insert : proto::callable
  {
    template<class Sig> struct result;

    template<class This, class Left, class Right, class State>
    struct result<This(Left,Right,State)>
    {
      typedef typename remove_const<typename remove_reference<Left>::type>::type left_type;
      typedef typename remove_const<typename remove_reference<Right>::type>::type right_type;
      typedef typename remove_const<typename remove_reference<State>::type>::type state_type;

      typedef fusion::pair<left_type, right_type> value_type;

      typedef typename fusion::result_of::push_back<state_type const, value_type>::type type;
    };

    template<class Left, class Right, class State>
    typename result<_insert(Left,Right,State)>::type
    operator()(Left const& ,Right const& r,State& s) const
    {
      return fusion::push_back(s,boost::fusion::make_pair<Left>(r));
    }
  };
} }

#endif


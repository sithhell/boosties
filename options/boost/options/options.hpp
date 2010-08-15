/*******************************************************************************
 *               Copyright 2010 Joel Falcou - Thomas Heller
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/
#ifndef BOOST_OPTIONS_OPTIONS_HPP_INCLUDED
#define BOOST_OPTIONS_OPTIONS_HPP_INCLUDED

#include <boost/options/details/pack.hpp>

namespace boost { namespace details
{
  ////////////////////////////////////////////////////////////////////////////////
  // Option binder class
  ////////////////////////////////////////////////////////////////////////////////
  struct option_binder
  {
    template<class X> struct result
    {
      typedef typename result_of < param_grammar ( param_expr<X> const&
                                                        , fusion::vector<>&
                                                        )
                                        >::type                     pack;
      typedef typename fusion::result_of::as_map<pack>::type map_type;
      typedef parameters<map_type>                                  type;
    };

    template<class X>
    typename result<X>::type
    operator[]( param_expr<X> const& xpr ) const
    {
      using fusion::as_map;
      fusion::vector<> base;
      typename result<X>::type that(as_map(param_grammar()(xpr,base)));
      return that;
    }
  };
} }

namespace boost
{
  ////////////////////////////////////////////////////////////////////////////////
  // options entry-point
  ////////////////////////////////////////////////////////////////////////////////
  ::boost::details::option_binder const options = {};
}

#endif


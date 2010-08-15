/*******************************************************************************
 *               Copyright 2010 Joel Falcou - Thomas Heller
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/
#ifndef BOOST_OPTIONS_DETAILS_PACK_HPP_INCLUDED
#define BOOST_OPTIONS_DETAILS_PACK_HPP_INCLUDED

#include <boost/options/details/parameters.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/as_map.hpp>
#include <boost/fusion/include/has_key.hpp>
#include <boost/fusion/include/as_vector.hpp>

namespace boost { namespace details
{
  //////////////////////////////////////////////////////////////////////////////
  // Fake type for displaying proper error message when no parameter where found
  //////////////////////////////////////////////////////////////////////////////
  struct no_parameter_found {};

  //////////////////////////////////////////////////////////////////////////////
  // Parameters pack
  //////////////////////////////////////////////////////////////////////////////
  template<class Map> struct parameters
  {
    parameters( Map const& m ) : values(m) {}

    template<class Sig> struct result;

    template<class This, class Name>
    struct result<This(param_expr<Name>)>
    {
      typedef typename  fusion
                      ::result_of::has_key< Map const
                                          , typename  proto::result_of
                                                    ::value<Name>::type
                                          >::type found;

      typedef  fusion::result_of::at_key< Map const
                                        , typename  proto::result_of
                                                  ::value<Name>::type
                                        > base;

      typedef typename mpl::eval_if<found
                                    , base
                                    , mpl::identity<no_parameter_found>
                                    >::type type;
    };

    template<class This, class Name, class Default>
    struct result<This(param_expr<Name>,Default)>
    {
      typedef typename  fusion
                      ::result_of::has_key< Map const
                                          , typename  proto::result_of
                                                    ::value<Name>::type
                                          >::type found;

      typedef  fusion::result_of::at_key< Map const
                                        , typename  proto::result_of
                                                  ::value<Name>::type
                                        > base;

      typedef typename mpl::eval_if<found
                                    , base
                                    , mpl::identity<Default>
                                    >::type type;
    };

    template<class Name>
    typename result<parameters(param_expr<Name>)>::type
    operator()(param_expr<Name> const& p) const
    {
      typedef typename  fusion
                      ::result_of::has_key< Map const
                                          , typename  proto
                                                    ::result_of::value<Name>::type
                                          >::type found;
      return eval(p, found());
    }

    template<class Name, class Default>
    typename result<parameters(param_expr<Name>,Default)>::type
    operator()(param_expr<Name> const& p, Default const& v ) const
    {
      typedef typename  fusion
                      ::result_of::has_key< Map const
                                          , typename  proto
                                                    ::result_of::value<Name>::type
                                          >::type found;
      return eval(p,v, found());
    }

    template<class Name, class Default>
    typename result<parameters(param_expr<Name>,Default)>::type
    eval(param_expr<Name> const&, Default const& , mpl::true_ ) const
    {
      return fusion::at_key<typename proto::result_of::value<Name>::type>(values);
    }

    template<class Name, class Default>
    typename result<parameters(param_expr<Name>,Default)>::type
    eval(param_expr<Name> const&, Default const& v, mpl::false_ ) const
    {
      return v;
    }

    template<class Name>
    typename result<parameters(param_expr<Name>)>::type
    eval(param_expr<Name> const&, mpl::true_) const
    {
      return fusion::at_key<typename proto::result_of::value<Name>::type>(values);
    }

    template<class Name>
    typename result<parameters(param_expr<Name>)>::type
    eval(param_expr<Name> const&, mpl::false_) const
    {
      BOOST_MPL_ASSERT_MSG(false, PARAMETER_NOT_FOUND, (Name));
      return no_parameter_found();
    }

    Map values;
  };
} }

#endif


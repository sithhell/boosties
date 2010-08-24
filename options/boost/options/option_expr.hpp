
/*******************************************************************************
 *               Copyright 2010 Joel Falcou, Thomas Heller, Eric Niebler
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/

#ifndef BOOST_OPTIONS_OPTION_EXPR_HPP
#define BOOST_OPTIONS_OPTION_EXPR_HPP

#include <boost/options/option_expr_fwd.hpp>
#include <boost/options/option_domain.hpp>
#include <boost/options/option_not_found.hpp>
#include <boost/options/option_term.hpp>
#include <boost/options/option_pack.hpp>

#include <boost/mpl/assert.hpp>

#include <boost/proto/core.hpp>

namespace boost { namespace options_impl
{
    template <typename Expr>
    struct option_expr
    {
        BOOST_PROTO_BASIC_EXTENDS(Expr, option_expr, option_domain)
        BOOST_PROTO_EXTENDS_ASSIGN()

        template <typename Sig>
        struct result;

        template <typename Option>
        struct has_option
            : mpl::not_<
                is_same<
                    typename result<option_expr const(Option)>::type
                  , option_not_found
                >
            >
        {};

        template <typename This, typename Option>
        struct result<This(Option)>
            : boost::result_of<option_pack(This, Option)>
        {};

        template <typename This, typename Option, typename Default>
        struct result<This(Option, Default)>
            : mpl::if_<
                has_option<Option>
              , typename result<This(Option)>::type
              , Default
            >
        {};

        template<typename Option>
        typename result<option_expr const(Option const)>::type
        operator()(Option const &opt) const
        {
            BOOST_MPL_ASSERT((proto::matches<Option const, option_term>));
            BOOST_MPL_ASSERT((has_option<Option const>));
            return option_pack()(*this, opt);
        }
        
        template<typename Option, typename Default>
        typename result<option_expr const(Option const, Default const)>::type
        operator()(Option const &opt, Default const &def) const
        {
            BOOST_MPL_ASSERT((proto::matches<Option const, option_term>));
            return this->with_default(opt, def, has_option<Option const>());
        }
        
    private:
        template<typename Option, typename Default>
        typename result<option_expr const(Option const, Default const)>::type
        with_default(Option const &opt, Default const &, mpl::true_) const
        {
            return option_pack()(*this, opt);
        }
        
        template<typename Option, typename Default>
        typename result<option_expr const(Option const, Default const)>::type
        with_default(Option const &, Default const &def, mpl::false_) const
        {
            return def;
        }
    };
}}

#endif

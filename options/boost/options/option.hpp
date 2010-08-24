
/*******************************************************************************
 *               Copyright 2010 Joel Falcou, Thomas Heller, Eric Niebler
 *
 *          Distributed under the Boost Software License, Version 1.0.
 *                 See accompanying file LICENSE.txt or copy at
 *                     http://www.boost.org/LICENSE_1_0.txt
 ******************************************************************************/

#ifndef BOOST_OPTIONS_BOOST_OPTION_HPP
#define BOOST_OPTIONS_BOOST_OPTION_HPP

namespace boost { namespace options_impl
{
    template <typename Tag>
    struct option
    {
        typedef Tag type;
    };
}}

#endif

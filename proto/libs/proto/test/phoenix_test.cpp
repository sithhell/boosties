
#include <boost/call_traits.hpp>
#include <boost/ref.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/proto/proto.hpp>
#include <boost/proto/visitor.hpp>
#include <boost/proto/transform/unpack.hpp>
#include <boost/proto/transform/transform_expr.hpp>

#include <iostream>
#include <typeinfo>

namespace boost { namespace phoenix {
    
    template <typename Tag>
    struct phoenix_grammar;

    template <template <typename> class Visitor>
    struct phoenix_visitor
        : proto::visitor<Visitor, phoenix_grammar>
    {};
    
    template <typename Tag>
    struct generic_evaluator;
    
    ////////////////////////////////////////////////////////////////////////////
    // eval_grammar:
    //
    // We define our evaluation grammar/transform just here by using the meta
    // grammar with our generic_evaluator.
    typedef phoenix_visitor<generic_evaluator> eval_grammar;

    eval_grammar const eval = eval_grammar();

    ////////////////////////////////////////////////////////////////////////////
    // Generic evaluator:
    // Depending on the given tag, the transform is called
    //
    // We dispatch on expression arity in our grammar, that means that
    // expression arity and generic::evaluator<Tag>::operator() arity needs to
    // match
    template <typename Tag>
    struct generic_evaluator
        : proto::_default<eval_grammar>
    {};
    
    template <typename Tag>
    struct phoenix_grammar
        : proto::_
    {};

    template <typename Expr>
    struct actor;
    
    // the domain, same as before
    struct phoenix_domain
      : proto::domain<
        proto::pod_generator<actor>, 
        proto::_, proto::default_domain>
    {
        template <typename T>
        struct as_child : as_expr<T>
        {};
    };

    struct default_domain_with_basic_expr
        : proto::domain<proto::use_basic_expr<proto::default_generator> >
    {};
    
    ////////////////////////////////////////////////////////////////////////////
    // phoenix_expr_ex and phoenix_expr:
    //
    // base class for phoenix expressions, similar to the previous compose, but
    // but can now be used in proto grammars/transform
    // expression creation is now done with the static compose function
    template <typename Expr, typename Tag, template <typename> class Actor>
    struct phoenix_expr_ex;

    template <typename Expr, typename Tag>
    struct phoenix_expr : phoenix_expr_ex<Expr, Tag, actor> {};
    
    template <typename A0, template <typename> class Expr, typename Tag, template <typename> class Actor>
    struct phoenix_expr_ex<Expr<A0>, Tag, Actor>
        : proto::transform<phoenix_expr_ex<Expr<A0>,Tag, Actor>, int>
    {
        typedef typename proto::result_of::make_expr<Tag, default_domain_with_basic_expr, A0>::type base_type;
        typedef Actor<base_type> type;
        typedef typename proto::unary_expr<Tag, A0>::proto_grammar proto_grammar;

        static type const
        compose(typename call_traits<A0>::param_type a0)
        {
            actor<base_type> const e = {{a0}};
            return e;
        }
        
        template<typename E, typename S, typename D>
        struct impl : proto::transform_impl<E, S, D>
        {
            typedef E result_type;
            
            #ifdef BOOST_PROTO_STRICT_RESULT_OF
            result_type
            #else
            typename impl::expr_param
            #endif
            operator ()(
                typename impl::expr_param e
              , typename impl::state_param
              , typename impl::data_param
            ) const
            {
                return e;
            }
        };
        
        typedef Tag proto_tag;
        typedef A0 proto_child0;
    };
    
    template <typename A0, typename A1, template <typename, typename> class Expr, typename Tag, template <typename> class Actor>
    struct phoenix_expr_ex<Expr<A0, A1>, Tag, Actor>
        : proto::transform<phoenix_expr_ex<Expr<A0, A1>,Tag, Actor>, int>
    {
        typedef typename proto::result_of::make_expr<Tag, default_domain_with_basic_expr, A0, A1>::type base_type;
        typedef Actor<base_type> type;
        typedef typename proto::binary_expr<Tag, A0, A1>::proto_grammar proto_grammar;
        
        static type const
        compose(typename call_traits<A0>::param_type a0, typename call_traits<A1>::param_type a1)
        {
            actor<base_type> const e = {{a0, a1}};
            return e;
        }
        
        template<typename E, typename S, typename D>
        struct impl : proto::transform_impl<E, S, D>
        {
            typedef E result_type;
            
            #ifdef BOOST_PROTO_STRICT_RESULT_OF
            result_type
            #else
            typename impl::expr_param
            #endif
            operator ()(
                typename impl::expr_param e
              , typename impl::state_param
              , typename impl::data_param
            ) const
            {
                return e;
            }
        };
        
        typedef Tag proto_tag;
        typedef A0 proto_child0;
        typedef A1 proto_child1;
    };
    
    template <typename A0, typename A1, typename A2, template <typename, typename, typename> class Expr, typename Tag, template <typename> class Actor>
    struct phoenix_expr_ex<Expr<A0, A1, A2>, Tag, Actor>
        : proto::transform<phoenix_expr_ex<Expr<A0, A1, A2>,Tag, Actor>, int>
    {
        typedef typename proto::result_of::make_expr<Tag, default_domain_with_basic_expr, A0, A1, A2>::type base_type;
        typedef Actor<base_type> type;
        typedef typename proto::nary_expr<Tag, A0, A1, A2>::proto_grammar proto_grammar;

        static type const
        compose(
            typename call_traits<A0>::param_type a0
          , typename call_traits<A1>::param_type a1
          , typename call_traits<A2>::param_type a2)
        {
            actor<base_type> const e = {{a0, a1, a2}};
            return e;
        }
        
        template<typename E, typename S, typename D>
        struct impl : proto::transform_impl<E, S, D>
        {
            typedef E result_type;
            
            #ifdef BOOST_PROTO_STRICT_RESULT_OF
            result_type
            #else
            typename impl::expr_param
            #endif
            operator ()(
                typename impl::expr_param e
              , typename impl::state_param
              , typename impl::data_param
            ) const
            {
                return e;
            }
        };
        
        typedef Tag proto_tag;
        typedef A0 proto_child0;
        typedef A1 proto_child1;
        typedef A2 proto_child2;
    };

    // add more ...
    
    ////////////////////////////////////////////////////////////////////////////
    // actor is the same as with the previous design
    template <typename Expr>
    struct actor
    {
        BOOST_PROTO_BASIC_EXTENDS(Expr, actor<Expr>, phoenix_domain)
        BOOST_PROTO_EXTENDS_ASSIGN()
        BOOST_PROTO_EXTENDS_SUBSCRIPT()

        template <typename Sig>
        struct result;

        template <typename This, typename A0>
        struct result<This(A0)>
            : boost::result_of<eval_grammar(actor<Expr> const&, fusion::vector1<A0>&)>
        {};

        template <typename A0>
        typename result<actor<Expr>(A0 const&)>::type
        operator()(A0 const& a0) const
        {
            fusion::vector1<A0 const&> env(a0);
            return eval(*this, env);
        }

        // add more ...
    };

    ////////////////////////////////////////////////////////////////////////////
    //
    // Core ends here
    //
    ////////////////////////////////////////////////////////////////////////////
    
    ////////////////////////////////////////////////////////////////////////////
    // val is implemented as a plain proto terminal
    template <typename T>
    struct value : proto::terminal<T> {};

    template <typename T>
    actor<typename value<T>::type> const
    val(T const& t)
    {
        actor<typename value<T>::type> const e = {t};
        return e;
    }
    
    template <typename T>
    typename value<T>::type const
    val(T & t)
    {
        actor<typename value<T>::type> const e = {t};
        return e;
    }

    ////////////////////////////////////////////////////////////////////////////
    // ref is implemented as a plain proto terminal with boost::ref as wrapper
    template <typename T>
    struct reference : proto::terminal<reference_wrapper<T> > {};

    template <typename T>
    actor<typename reference<T const>::type> const
    cref(T const& t)
    {
        actor<typename reference<T const>::type> const e = {::boost::cref(t)};
        return e;
    }
    
    // overload to handle boost::reference_wrapper directly
    template <typename T>
    actor<typename reference<T const>::type> const
    cref( reference_wrapper<T const> t)
    {
        actor<typename reference<T const>::type> const e = {t};
        return e;
    }
    
    template <typename T>
    actor<typename reference<T>::type> const
    ref(T & t)
    {
        actor<typename reference<T>::type> const e = {::boost::ref(t)};
        return e;
    }
    
    // overload to handle boost::reference_wrapper directly
    template <typename T>
    actor<typename reference<T>::type> const
    ref(reference_wrapper<T> t)
    {
        actor<typename reference<T>::type> const e = {t};
        return e;
    }

    ////////////////////////////////////////////////////////////////////////////
    // Argument implementation
    // Note that we at first only define the phoenix expressions without exposure
    // of proto.
    
    namespace tag { struct argument; }
    
    template <typename N>
    struct argument : phoenix_expr<argument<N>, tag::argument> {};
    
    namespace placeholders {
        argument<mpl::int_<0> >::type const _1 = {};
        // more ...
    }
    
    struct argument_eval;
    
    // define the generic evaluator
    template <>
    struct generic_evaluator<tag::argument>
        : proto::unpack<argument_eval(proto::_value, proto::_state)>
    {};

    struct argument_eval
        : proto::callable
    {
        template <typename Sig>
        struct result;
        
        template <typename This, typename N, typename Env>
        struct result<This(N, Env&)>
            : result<This(N const &, Env&)>
        {};

        template <typename This, typename N, typename Env>
        struct result<This(N &, Env&)>
            : fusion::result_of::at_c<Env, N::value>
        {
        };
        
        template <typename N, typename Env>
        typename fusion::result_of::at_c<
            Env
          , N::value
        >::type
        operator()(N &, Env& env) const
        {
            return fusion::at_c<N::value>(env);
        }
        
        template <typename N, typename Env>
        typename fusion::result_of::at_c<
            Env
          , N::value
        >::type
        operator()(N const &, Env& env) const
        {
            return fusion::at_c<N::value>(env);
        }
    };

    ////////////////////////////////////////////////////////////////////////////
    // if, at first we only declare how our if_/else_ is going to be generated
    // note that no definition of any evaluation scheme is done here
    namespace tag
    {
        struct if_then;
        struct if_then_else;
    }

    template <typename Cond, typename Then, typename Else>
    struct if_then_else : phoenix_expr<if_then_else<Cond, Then, Else>, tag::if_then_else> {};

    template <typename Cond, typename Then>
    struct else_gen
    {
        else_gen(Cond const& cond, Then const& then) : cond(cond), then(then) {}
        Cond const& cond;
        Then const& then;

        template <typename Else>
        typename if_then_else<Cond, Then, Else>::type const
        operator[](Else const& else_) const
        {
            return if_then_else<Cond, Then, Else>::compose(cond, then, else_);
        }
    };
    
    template <typename Expr>
    struct if_actor : actor<Expr>
    {
        typedef actor<Expr> base_type;

        if_actor(base_type const& base)
            : base_type(base)
            , else_(proto::child_c<0>(*this), proto::child_c<1>(*this))
        {}
        
        typedef typename proto::result_of::child_c<Expr, 0>::type cond_type;
        typedef typename proto::result_of::child_c<Expr, 1>::type then_type;

        else_gen<cond_type, then_type> else_;

    };

    template <typename Cond, typename Then>
    struct if_then : phoenix_expr_ex<if_then<Cond, Then>, tag::if_then, if_actor> {};

    template <typename Cond>
    struct if_gen
    {
        if_gen(Cond const& cond) : cond(cond) {}

        Cond const& cond;

        template <typename Then>
        typename if_then<Cond, Then>::type const
        operator[](Then const& then) const
        {
            return if_then<Cond, Then>::compose(cond, then);
        }
    };

    template <typename Cond>
    if_gen<Cond> const
    if_(Cond const& cond)
    {
        return if_gen<Cond>(cond);
    }

    // Expression generation done.
    ////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////
    // finally, define how our generic_evaluator should evaluate our if_
    struct if_then_eval;

    template <>
    struct generic_evaluator<tag::if_then>
        : proto::unpack<if_then_eval(proto::_, proto::_state)>
    {};

    struct if_then_eval
        : proto::callable
    {
        typedef void result_type;

        // just like with the previous design, we get the to be evaluated
        // expressions as function parameters, neat!
        template <typename Cond, typename Then, typename Env>
        result_type operator()(Cond const& cond, Then const& then, Env& env) const
        {
            if(eval(cond, env)) eval(then, env);
        }
    };

    ////////////////////////////////////////////////////////////////////////////
    // finally, define how our generic_evaluator should evaluate our if_
    struct if_then_else_eval;

    template <>
    struct generic_evaluator<tag::if_then_else>
        : proto::unpack<if_then_else_eval(proto::_, proto::_state)>
    {};

    struct if_then_else_eval
        : proto::callable
    {
        typedef void result_type;

        // just like with the previous design, we get the to be evaluated
        // expressions as function parameters, neat!
        template <typename Cond, typename Then, typename Else, typename Env>
        result_type operator()(Cond const& cond, Then const& then, Else const& else_, Env& env) const
        {
            if(eval(cond, env)) eval(then, env);
            else eval(else_, env);
        }
    };
}}

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

namespace phoenix = boost::phoenix;
namespace proto = boost::proto;
namespace mpl = boost::mpl;

// the new design enables proto grammars that can refer to phoenix expressions
// in a somehow natural way, i.e. with they're names instead of proto::nary_expr<Tag, ...>
struct test_grammar
    : phoenix::argument<proto::_>
{};

struct if_test_grammar
    : proto::or_<
        phoenix::if_then_else<proto::_, proto::_, proto::_>
      , phoenix::if_then<proto::_, proto::_>
    >
{};

template <typename Expr>
void foo(Expr const& expr)
{
    foo(expr, proto::matches<Expr, test_grammar>());
}

template <typename Expr>
void foo(Expr const& expr, mpl::true_)
{
    std::cout << "argument!\n";
}

template <typename Expr>
void foo(Expr const& expr, mpl::false_)
{
    std::cout << "no argument!\n";
}

template <typename Expr>
void if_foo(Expr const& expr)
{
    if_foo(expr, proto::matches<Expr, if_test_grammar>());
}

template <typename Expr>
void if_foo(Expr const& expr, mpl::true_)
{
    std::cout << "if statement!\n";
}

template <typename Expr>
void if_foo(Expr const& expr, mpl::false_)
{
    std::cout << "no if statement!\n";
}

template <typename Tag>
struct constant_fold_visitor;

typedef phoenix::phoenix_visitor<constant_fold_visitor> constant_folder;

constant_folder const constant_fold = constant_folder();

template <typename Tag>
struct constant_fold_visitor
    : proto::transform_expr<constant_folder>
{};

template <>
struct constant_fold_visitor<proto::tag::terminal>
    : proto::_
{};

struct constant_fold_extract_value
    : proto::when<proto::_, proto::_value(constant_folder(proto::_))>
{};

template <typename FoldOp>
struct folder
    : proto::if_<
        mpl::and_<
            proto::matches<
                constant_folder(proto::_child0)
              , phoenix::value<proto::_>::type
            >()
          , proto::matches<
                constant_folder(proto::_child1)
              , phoenix::value<proto::_>::type
            >()
        >()
      , proto::unpack<FoldOp(constant_fold_extract_value)>
      , proto::transform_expr<constant_folder>
    >
{};

struct fold_plus;

template <>
struct constant_fold_visitor<proto::tag::plus>
    : folder<fold_plus>
{};

struct fold_plus
    : proto::callable
{
    template <typename Sig>
    struct result;

    template <typename This, typename T1, typename T2>
    struct result<This(T1, T2)>
    {
        typedef typename proto::detail::uncvref<T1>::type value_type;
        typedef phoenix::actor<typename phoenix::value<value_type>::type> const type;
    };

    template <typename T1, typename T2>
    typename result<fold_plus(T1, T2)>::type
    operator()(T1 t1, T2 t2)
    {
        std::cout << "fold: " << t1 << " + " << t2 << "\n";
        return phoenix::val(t1 + t2);
    }
};

struct fold_multiply;

template <>
struct constant_fold_visitor<proto::tag::multiplies>
    : folder<fold_multiply>
{};

struct fold_multiply
    : proto::callable
{
    template <typename Sig>
    struct result;

    template <typename This, typename T1, typename T2>
    struct result<This(T1, T2)>
    {
        typedef typename proto::detail::uncvref<T1>::type value_type;
        typedef phoenix::actor<typename phoenix::value<value_type>::type> const type;
    };

    template <typename T1, typename T2>
    typename result<fold_plus(T1, T2)>::type
    operator()(T1 t1, T2 t2)
    {
        std::cout << "fold: " << t1 << " * " << t2 << "\n";
        return phoenix::val(t1 * t2);
    }
};

template <typename U, typename Q = void>
struct bar
{};

template <typename T, typename U>
struct test_impl
{
    typedef int/*typename mpl::apply<T, U>::type*/ type;
};

//template <typename T, typename Q>
//struct test;
//{typedef int type;};

template <template <typename, typename> class T, typename T>
struct test//<T<mpl::_1, mpl::_2>, int>
{
    typedef int type;
};

/*
template <template <typename> class T, typename Q>
struct test<T<mpl::_1>, Q>
    : test_impl<T<mpl::_1>, Q>
{
};

template <template <typename A0 = mpl::_1, typename A1 = mpl::_2> class T, typename Q>
struct test<T<mpl::_1, mpl::_2>, Q>
    : test_impl<T<mpl::_1, mpl::_2>, Q>
{};
*/

int main()
{
    using phoenix::val;
    using phoenix::ref;
    using phoenix::cref;
    using phoenix::placeholders::_1;

    std::cout << typeid(test<bar, proto::tag::plus>::type).name() << "\n";
    
#if 0
    std::cout << val(1)(0) << "\n";
    std::cout << cref(2)(0) << "\n";
    int i = 3;
    std::cout << ref(i)(0) << "\n";

    std::cout << _1(10) << "\n";
    // check if the expression is an argument
    foo( _1 );
    // check if the expression is an if statement
    if_foo( _1 );
    // check if the expression is an argument
    foo( val(1) );
    // check if the expression is an if statement
    if_foo( val(1) );

    if_(val(8) == 8)[ std::cout << val(std::string("success!\n")) ](0);
    // check if the expression is an if statement
    if_foo(if_(val(8) == 8)[ std::cout << val(std::string("success!\n")) ]);
    foo(if_(val(8) == 8)[ std::cout << val(std::string("success!\n")) ]);

    if_(val(9) == 8)[ std::cout << val(std::string("fail!\n")) ].else_[ std::cout << val(std::string("success!\n")) ](0);
    // check if the expression is an if statement
    if_foo(if_(val(9) == 8)[ std::cout << val(std::string("fail!\n")) ].else_[ std::cout << val(std::string("success!\n")) ]);
    // check if the expression is an argument
    foo(if_(val(9) == 8)[ std::cout << val(std::string("fail!\n")) ].else_[ std::cout << val(std::string("success!\n")) ]);

    std::vector<int> v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);

    std::for_each(v.begin(), v.end(), if_(_1 % 2)[std::cout << _1 << std::string(" is odd\n")].else_[std::cout << _1 << std::string(" is even\n")]);

    std::cout << "\n";
    std::cout << constant_fold(val(8) + val(9))(0) << "\n";
    std::cout << "\n";
    std::cout << constant_fold(val(8) + val(9) + val(10))(0) << "\n";
    std::cout << "\n";
    std::cout << constant_fold(_1 * (val(9) + val(10)))(10) << "\n";
    std::cout << "\n";
    std::cout << constant_fold(_1 * val(9) + val(10))(10) << "\n";
    std::cout << "\n";
    std::cout << constant_fold(val(10) * val(9) + val(10))(10) << "\n";
    std::cout << constant_fold(val(8) + _1)(10) << "\n";
    std::cout << "\n";
    std::cout << typeid(constant_fold( val(8) + val(9))).name() << "\n\n";
    std::cout << typeid(constant_fold( val(8) + val(9) + val(10))).name() << "\n\n";
    std::cout << typeid(constant_fold( _1 * (val(7) + val(8)))).name() << "\n\n";
#endif
}

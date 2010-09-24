
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/algorithm/transformation.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/proto/core.hpp>
#include <boost/proto/transform.hpp>
#include <boost/proto/transform/fall_through.hpp>
#include <boost/proto/fusion.hpp>
#include <boost/proto/visitor/cases.hpp>
#include <boost/proto/visitor/visit.hpp>
#include <boost/proto/visitor/grammar.hpp>

#include <iostream>
#include <typeinfo>

namespace proto  = boost::proto;
namespace fusion = boost::fusion;
namespace mpl = boost::mpl;

namespace tag {
    struct split {};
}

typedef proto::terminal<char>::type terminal;

template <typename Expr>
struct make_split : proto::result_of::make_expr<tag::split, proto::default_domain, Expr> {};

template <typename Expr>
typename make_split<Expr>::type const
split(Expr const& expr)
{
    typename make_split<Expr>::type const e = {expr};

    return e;
}

template <typename> struct splitter_grammar;
template <typename> struct splitter_visitor;

typedef proto::grammar<splitter_visitor, splitter_grammar, proto::_, proto::fall_through> split_grammar;

split_grammar const splitter = split_grammar();


template <>
struct splitter_visitor<proto::tag::terminal>
    : boost::proto::visitor<boost::proto::visit::packed, boost::proto::visit::without_state, boost::proto::visit::without_data>
{
    template <typename Sig>
    struct result;

    template <typename This, typename Expr>
    struct result<This(Expr const&)>
    {
        typedef Expr const& type;
    };

    template <typename Expr>
    Expr const &
    operator()(Expr const& expr) const { return expr; }
};

template <typename T>
struct is_split_expr
{
   typedef typename mpl::eval_if_c<
        proto::is_expr<T>::value
      , boost::is_same<typename proto::tag_of<T>::type, tag::split>
      , mpl::false_
    >::type type;

   static const bool value = type::value;
};

template <typename T>
struct needs_split
    : mpl::not_<
        boost::is_same<
            typename fusion::result_of::find_if<T, is_split_expr<mpl::_> >::type
          , typename fusion::result_of::end<T>::type
        >
    >
{
};

template <int N>
struct make_split_terminal : proto::terminal<mpl::int_<N> > {};

template <int N>
struct replace_split
{
    template <typename Sig>
    struct result;

    template <typename This, typename E>
    struct result<This(E)>
    {
        typedef typename
            mpl::eval_if_c<
                is_split_expr<E>::type::value
              , make_split_terminal<N>
              , mpl::identity<E>
            >::type
            type;
    };
    
    template <typename E>
    typename result<replace_split<N>(E)>::type
    operator()(E const& e ) const
    {
        return this->invoke(e, typename boost::is_same<typename proto::tag_of<E>::type, tag::split>::type());
    }

    template <typename E>
    typename result<replace_split<N>(E const&)>::type
    invoke(E const & e, mpl::true_ ) const // needs splitting
    {
        typename make_split_terminal<N>::type const f = {};
        return f;
    }

    template <typename E>
    typename result<replace_split<N>(E const&)>::type
    invoke(E const & e, mpl::false_ ) const // needs no splitting
    {
        return e;
    }

};

struct eval_children
{
    template <typename Sig>
    struct result;

    template <typename This, typename State, typename Expr>
    struct result<This(State const&, Expr const&)>
    {
        typedef typename proto::detail::uncvref<typename boost::result_of<split_grammar(Expr const&, State const&)>::type>::type eval_result;

        typedef typename
            mpl::eval_if_c<
                is_split_expr<Expr>::value
              , mpl::eval_if_c<
                    fusion::traits::is_sequence<eval_result>::value
                  , fusion::result_of::join<State const, eval_result const>
                  , fusion::result_of::push_back<State const,  eval_result>
                >
              , mpl::identity<State const>
            >::type
            type;
    };
    
    template <typename State, typename Expr>
    typename result<eval_children(State const&, Expr const&)>::type
    operator()(State const& state, Expr const& expr) const
    {
        typedef result<eval_children(State const&, Expr const&)> nested_result;
        return this->invoke(state, expr, typename is_split_expr<Expr>::type(), typename fusion::traits::is_sequence<typename nested_result::eval_result>::type());
    }

    template <typename State, typename Expr, typename IsSequence>
    typename result<eval_children(State const&, Expr const&)>::type
    invoke(State const& state, Expr const& expr, mpl::false_, IsSequence) const
    {
        return state;
    }

    template <typename State, typename Expr>
    typename result<eval_children(State const&, Expr const&)>::type
    invoke(State const& state, Expr const& expr, mpl::true_, mpl::false_) const
    {

        return fusion::push_back(state, splitter(expr, state));
    }

    template <typename State, typename Expr>
    typename result<eval_children(State const&, Expr const&)>::type
    invoke(State const& state, Expr const& expr, mpl::true_, mpl::true_) const
    {

        return fusion::join(state, splitter(expr, state));
    }
};

template <typename Tag>
struct splitter_visitor
    : boost::proto::visitor<boost::proto::visit::packed, boost::proto::visit::with_state, boost::proto::visit::without_data>
{
    typedef splitter_visitor<Tag> this_type;

    template <typename Sig>
    struct result;

    template <typename This, typename Expr, typename State>
    struct result<This(Expr const &, State const&)>
    {
        typedef typename proto::detail::uncvref<typename
            proto::result_of::unpack_expr<
                Tag
              , proto::default_domain
              , typename proto::detail::uncvref<typename fusion::result_of::transform<
                    Expr
                  , replace_split<fusion::result_of::size<State>::type::value>
                >::type>::type
            >::type>::type
            split_expr_type;

        typedef typename proto::detail::uncvref<typename
            mpl::eval_if_c<
                needs_split<Expr>::type::value
              , proto::detail::uncvref< typename fusion::result_of::join<typename fusion::result_of::push_front<State const, split_expr_type>::type const, typename fusion::result_of::fold<Expr, fusion::vector0<>, eval_children>::type const>::type >
              , mpl::identity<fusion::vector1<Expr const &> >//fusion::result_of::join<State const, fusion::vector1<Expr> const >
            >::type>::type
            type;
    };

    template <typename Expr, typename State>
    typename result<this_type(Expr const&, State const&)>::type
    operator()(Expr const& expr, State const&state) const
    {
        return this->invoke(expr, state, typename needs_split<Expr>::type());
    }

    template <typename Expr, typename State>
    typename proto::detail::uncvref<typename result<this_type(Expr const&, State const&)>::type>::type
    invoke(Expr const& expr, State const& state, mpl::true_) const
    {
        return fusion::join(
            fusion::push_front(
                state
              , proto::functional::unpack_expr<Tag>()(
                    fusion::transform(
                        expr
                      , replace_split<fusion::result_of::size<State>::type::value>()
                    )
                )
            )
          , fusion::fold(expr, fusion::vector0<>(), eval_children())
        );
    }

    template <typename Expr, typename State>
    typename result<this_type(Expr const&, State const&)>::type
    invoke(Expr const& expr, State const&state, mpl::false_) const
    {
        return fusion::vector1<Expr const&>(expr);//return fusion::join(state, fusion::vector1<Expr>(expr));
    }
};

template <>
struct splitter_visitor<tag::split>
    : boost::proto::visitor<boost::proto::visit::unpacked, boost::proto::visit::with_state, boost::proto::visit::without_data>
{
    template <typename Sig>
    struct result;
    
    template <typename This, typename Expr, typename State>
    struct result<This(Expr const&, State const&)>
    {
        typedef typename boost::result_of<split_grammar(Expr const &, State const&)>::type type;
    };

    template <typename Expr, typename State>
    typename result<splitter_visitor<tag::split>(Expr const&, State const&)>::type
    operator()(Expr const& expr, State const& state) const
    {
        return splitter(expr, state );
    }
};

int main()
{
    terminal const a = {'a'};
    terminal const b = {'b'};
    terminal const c = {'c'};
    terminal const d = {'d'};
    terminal const e = {'e'};
    terminal const f = {'f'};

    fusion::vector0<> r;
    fusion::vector0<> const &res = r;

    std::cout << typeid(fusion::as_vector(splitter(split(a) + split(b), res))).name() << "\n\n";
    std::cout << typeid(fusion::as_vector(splitter(a + b, res))).name() << "\n\n";
    std::cout << typeid(fusion::as_vector(splitter(split(a) + b, res))).name() << "\n\n";
    std::cout << typeid(fusion::as_vector(splitter(a + split(b), res))).name() << "\n\n";
    std::cout << typeid(fusion::as_vector(splitter(a + split(b + c), res))).name() << "\n\n";
    std::cout << typeid(fusion::as_vector(splitter(a+b+split( c*d + split(e-f)), res))).name() << "\n\n";
    fusion::as_vector(splitter(split(a) + split(b), res));
    fusion::as_vector(splitter(a + b, res));
    fusion::as_vector(splitter(split(a) + b, res));
    fusion::as_vector(splitter(a + split(b), res));
    fusion::as_vector(splitter(a + split(b + c), res));
}


#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/algorithm/transformation.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/proto/core.hpp>
#include <boost/proto/transform.hpp>
#include <boost/proto/transform/fall_through.hpp>
#include <boost/proto/transform/transform_expr.hpp>
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

namespace boost { namespace proto {

namespace functional {
    
    struct join
    {
        BOOST_PROTO_CALLABLE()

        template <typename Sig>
        struct result;

        template <typename This, typename Seq1, typename Seq2>
        struct result<This(Seq1, Seq2)>
            : result<This(Seq1 const&, Seq2 const&)>
        {};

        template <typename This, typename Seq1, typename Seq2>
        struct result<This(Seq1 &, Seq2 &)>
            : fusion::result_of::join<Seq1 const, Seq2 const>
        {};

        template <typename Seq1, typename Seq2>
        typename fusion::result_of::join<Seq1 const, Seq2 const>::type
        operator()(Seq1 const& seq1, Seq2 const& seq2) const
        {
            return fusion::join(seq1, seq2);
        }
    };
    
    struct push_back
    {
        BOOST_PROTO_CALLABLE()

        template <typename Sig>
        struct result;

        template <typename This, typename Seq, typename T>
        struct result<This(Seq, T const &)>
            : result<This(Seq const&, T const&)>
        {};

        template <typename This, typename Seq, typename T>
        struct result<This(Seq &, T const &)>
            : fusion::result_of::push_back<Seq const, T>
        {};

        template <typename Seq, typename T>
        typename fusion::result_of::push_back<Seq const, T>::type
        operator()(Seq const& seq, T const& t) const
        {
            return fusion::push_back(seq, t);
        }
    };
}

template <>
struct is_callable<functional::join> : mpl::true_ {};

template <>
struct is_callable<functional::push_back> : mpl::true_ {};
}}

struct eval_children
    : proto::or_<
        proto::when<
            proto::unary_expr<tag::split, proto::_>
          , proto::if_<
                fusion::traits::is_sequence<split_grammar>()
              , proto::functional::join(proto::_state, split_grammar)
              , proto::functional::push_back(proto::_state, split_grammar)
            >
        >
      , proto::otherwise<proto::_state>
    >
{};

struct eval
    : proto::when<
        proto::_
      , proto::fold<
            proto::_
          , proto::_state
          , eval_children
        >
    >
{};

struct transform_children
    : proto::or_<
        proto::when<
            proto::unary_expr<tag::split, proto::_>
          , make_split_terminal<0>(mpl::int_<0>())
        >
      , proto::otherwise<proto::_>
    >
{};

struct transform
    : proto::when<
        proto::_
      , proto::transform_expr<
            transform_children
        >
    >
{};

template <typename Tag>
struct splitter_visitor
    : boost::proto::visitor<boost::proto::visit::packed, boost::proto::visit::with_state, boost::proto::visit::without_data>
{
    typedef splitter_visitor<Tag> this_type;

    template <typename Sig>
    struct result;

    template <typename This, typename Expr, typename State>
    struct result<This(Expr const &, State)>
        : result<This(Expr const&, State const &)>
    {};

    template <typename This, typename Expr, typename State>
    struct result<This(Expr const &, State &)>
    {
        typedef typename proto::detail::uncvref<typename
            mpl::eval_if_c<
                needs_split<Expr>::type::value
              , proto::detail::uncvref<
                    typename fusion::result_of::join<
                        typename fusion::result_of::push_front<
                            typename boost::remove_const<State>::type const
                          , typename boost::result_of<transform(Expr const&)>::type
                        >::type const
                      , typename boost::result_of<eval(Expr const&, fusion::vector0<> &)>::type const
                    >::type
                >
              , mpl::identity<fusion::vector1<Expr const &> >
            >::type>::type
            type;
    };

    template <typename Expr, typename State>
    typename result<this_type(Expr const&, State &)>::type
    operator()(Expr const& expr, State & state) const
    {
        return this->invoke(expr, state, typename needs_split<Expr>::type());
    }

    template <typename Expr, typename State>
    typename result<this_type(Expr const&, State &)>::type
    invoke(Expr const& expr, State & state, mpl::true_) const
    {
        fusion::vector0<> v;
        return fusion::join(
            fusion::push_front(
                state
              , transform()(expr)
            )
          , eval()(expr, v)
        );
    }

    template <typename Expr, typename State>
    typename result<this_type(Expr const&, State &)>::type
    invoke(Expr const& expr, State & state, mpl::false_) const
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
    struct result<This(Expr const&, State)>
        : result<This(Expr const &, State const&)>
    {};
    
    template <typename This, typename Expr, typename State>
    struct result<This(Expr const&, State &)>
    {
        typedef typename boost::result_of<split_grammar(Expr const &, State &)>::type type;
    };

    template <typename Expr, typename State>
    typename result<splitter_visitor<tag::split>(Expr const&, State &)>::type
    operator()(Expr const& expr, State & state) const
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
    //fusion::as_vector(splitter(a + b, res));
    //fusion::as_vector(splitter(split(a) + b, res));
    //fusion::as_vector(splitter(a + split(b), res));
    //fusion::as_vector(splitter(a + split(b + c), res));
}

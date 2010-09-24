
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/algorithm/transformation.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/proto/core.hpp>
#include <boost/proto/transform.hpp>
#include <boost/proto/transform/fall_through.hpp>
#include <boost/proto/transform/transform_expr.hpp>
#include <boost/proto/fusion.hpp>
#include <boost/proto/fusion/push_back.hpp>
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

template <int N>
struct make_split_terminal : proto::terminal<mpl::int_<N> > {};

struct do_split
    : proto::or_<
        proto::when<
            proto::unary_expr<tag::split, proto::_>
          , do_split(proto::_child0, proto::_state)
        >
      , proto::otherwise<
          proto::if_<
            proto::fold<
                proto::_
              , mpl::false_()
              , proto::or_<
                    proto::when<
                        proto::unary_expr<tag::split, proto::_>
                      , mpl::or_<mpl::true_, proto::_state>()
                    >
                  , proto::otherwise<mpl::or_<mpl::false_, proto::_state>()>
                >
            >
          , proto::functional::join(
                proto::functional::push_front(
                    proto::_state
                  , proto::transform_expr<
                        proto::or_<
                            proto::when<
                                proto::unary_expr<tag::split, proto::_>
                              , make_split_terminal<0>(mpl::int_<0>())
                            >
                          , proto::otherwise<proto::_>
                        >
                    >
                )
              , proto::fold<
                    proto::_
                  , proto::_state
                  /*, proto::functional::push_front(
                        proto::_state
                      , proto::transform_expr<
                            proto::or_<
                                proto::when<
                                    proto::unary_expr<tag::split, proto::_>
                                  , make_split_terminal<0>(mpl::int_<0>())
                                >
                              , proto::otherwise<proto::_>
                            >
                        >
                    )*/
                  , proto::or_<
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
                >
            )
          , fusion::vector1<proto::_>(proto::_)
        >
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
    struct result<This(Expr const &, State &)>
    {
        typedef typename
            boost::result_of<
                do_split(Expr const&, State &)
            >::type
            type;
    };

    template <typename Expr, typename State>
    typename result<this_type(Expr const&, State &)>::type
    operator()(Expr const& expr, State & state) const
    {
        return do_split()(expr, state);
    }
};

template <>
struct splitter_visitor<tag::split>
    : boost::proto::visitor<boost::proto::visit::unpacked, boost::proto::visit::with_state, boost::proto::visit::without_data>
{
    template <typename Sig>
    struct result;
    
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
    //std::cout << typeid(fusion::as_vector(do_split()(a+b+split( c*d + split(e-f)), res))).name() << "\n\n";
    fusion::as_vector(splitter(split(a) + split(b), res));
    fusion::as_vector(splitter(a + b, res));
    fusion::as_vector(splitter(split(a) + b, res));
    fusion::as_vector(splitter(a + split(b), res));
    fusion::as_vector(splitter(a + split(b + c), res));
    fusion::as_vector(do_split()(a + split(b + c), res));
}

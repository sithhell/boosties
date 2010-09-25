#include <boost/fusion/support/is_sequence.hpp>
#include <boost/proto/proto.hpp>
#include <boost/fusion/algorithm/iteration/for_each.hpp>
#include <boost/fusion/algorithm/transformation.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/proto/transform/transform_expr.hpp>
#include <boost/proto/fusion.hpp>
#include <boost/proto/fusion/push_back.hpp>
#include <boost/proto/visitor.hpp>

#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/facilities/identity.hpp>

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

template <int N>
struct make_split_terminal : proto::terminal<mpl::int_<N> > {};

template <typename Tag> struct splitter_grammar;
template <typename Tag> struct splitter_visitor;

typedef proto::visitor<splitter_visitor, splitter_grammar> split_grammar;

split_grammar const splitter = split_grammar();

template <typename Tag>
struct splitter_grammar
    : proto::_
{};

template <typename Tag>
struct splitter_visitor
    : proto::if_<
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
{};

template<>
struct splitter_visitor<proto::tag::terminal>
    : proto::_expr
{};

template<>
struct splitter_visitor<tag::split>
    : proto::when<proto::_, split_grammar(proto::_child0)>
{};

int main()
{
    terminal const a = {'a'};
    terminal const b = {'b'};
    terminal const c = {'c'};
    terminal const d = {'d'};
    terminal const e = {'e'};
    terminal const f = {'f'};

    fusion::vector0<> res;

    /*
    std::cout << typeid(splitter(split(a) + split(b), res)).name() << "\n\n";
    std::cout << typeid(splitter(a + b, res)).name() << "\n\n";
    std::cout << typeid(splitter(split(a) + b, res)).name() << "\n\n";
    std::cout << typeid(splitter(a + split(b), res)).name() << "\n\n";
    std::cout << typeid(splitter(a + split(b + c), res)).name() << "\n\n";
    std::cout << typeid(splitter(a+b+split( c*d + split(e-f)), res)).name() << "\n\n";*/
    //splitter(split(a) + split(b), res);
    splitter(a + b, res);
    splitter(split(a) + b, res);
    splitter(a + split(b), res);
    splitter(a + split(b + c), res);
}

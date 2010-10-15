//#include<boost/proto/proto.hpp>
#include <iostream>
#include<boost/proto/proto.hpp>
#include<boost/proto/transform/when_vararg.hpp>


namespace proto = boost::proto;

struct foo
    : proto::callable
{
    typedef void result_type;

    void operator()() const
    {
        std::cout << "nullary\n\n";
    }

    template <typename A0>
    void operator()(A0 const& a0) const
    {
        std::cout << "unary:\n";
        std::cout << typeid(A0).name() << "\n\n";
    }

    template <typename A0, typename A1>
    void operator()(A0 const& a0, A1 const& a1) const
    {
        std::cout << "binary:\n";
        std::cout << typeid(A0).name() << "\n";
        std::cout << typeid(A1).name() << "\n\n";
    }

    template <typename A0, typename A1, typename A2>
    void operator()(A0 const& a0, A1 const& a1, A2 const& a2) const
    {
        std::cout << "ternary:\n";
        std::cout << typeid(A0).name() << "\n";
        std::cout << typeid(A1).name() << "\n";
        std::cout << typeid(A2).name() << "\n\n";
    }

    template <typename A0, typename A1, typename A2, typename A3>
    void operator()(A0 const& a0, A1 const& a1, A2 const& a2, A3 const& a3) const
    {
        std::cout << "4nary:\n";
        std::cout << typeid(A0).name() << "\n";
        std::cout << typeid(A1).name() << "\n";
        std::cout << typeid(A2).name() << "\n\n";
        std::cout << typeid(A3).name() << "\n\n";
    }
};

struct test
    : proto::when<
        proto::function<proto::vararg<proto::_> >
      , foo(proto::vararg<proto::_>)
    >
{};

struct test2
    : proto::when<
        proto::function<proto::vararg<proto::_> >
      , foo(proto::vararg<proto::_>, proto::_state)
    >
{};

typedef proto::terminal<int>::type const i_t;

typedef proto::function<i_t>::type const f1_t;
typedef proto::function<i_t, i_t>::type const f2_t;
typedef proto::function<i_t, i_t, i_t>::type const f3_t;

int main()
{
    f1_t f1 = {};
    f2_t f2 = {};
    f3_t f3 = {};
    i_t i = {};

    test()(f1);
    test2()(f1, 5);
    test()(f2);
    test2()(f2, 5);
    test()(f3);
    test2()(f3, 5);
    test()(i());
    test2()(i(), 5);
}

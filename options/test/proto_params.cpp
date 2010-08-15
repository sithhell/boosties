#include <iostream>
#include <options.hpp>

using namespace std;
using namespace boost;

///////////////////////////////////////////////////////////////////////////////
// User written code begins here
///////////////////////////////////////////////////////////////////////////////
BOOST_REGISTER_PARAMETERS(speed);
BOOST_REGISTER_PARAMETERS(force);
BOOST_REGISTER_PARAMETERS(bar);

struct foo
{
  template <typename ParamMap>
  foo( ParamMap const& opt )
     : speed_( opt(speed) )
     , force_( opt(force) )
     , bar_( opt(bar, true) )
  {}

  float speed_;
  int force_;
  bool bar_;
};

struct baz
{
  template <typename ParamMap>
  baz( ParamMap const& opt )
     : speed_( opt(speed) )
     , force_( opt(force) )
     , bar_( opt(bar) )
  {}

  float speed_;
  int force_;
  bool bar_;
};

int main()
{
  {
    foo f(options[speed = 8.9f, bar= false, force = 5]);
    cout << f.speed_ << " " << f.force_ << " " << f.bar_ << "\n";
  }

  {
    foo f(options[speed = 8.9f, force = 5]);
	 cout << f.speed_ << " " << f.force_ << " " << f.bar_ << "\n";
  }

  {
	  // uncomment this to get an error
	  //baz b(options[speed = 9.9f, force = 5]);
  }
}

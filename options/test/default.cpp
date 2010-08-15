#include <iostream>
#include <string>
#include <boost/options/options.hpp>

////////////////////////////////////////////////////////////////////////////////
// Register some named parameters
////////////////////////////////////////////////////////////////////////////////
namespace tag
{
  BOOST_REGISTER_PARAMETERS(title);
  BOOST_REGISTER_PARAMETERS(pos_x);
  BOOST_REGISTER_PARAMETERS(pos_y);
  BOOST_REGISTER_PARAMETERS(height);
  BOOST_REGISTER_PARAMETERS(width);
}

////////////////////////////////////////////////////////////////////////////////
// Function taking options pack as parameters
////////////////////////////////////////////////////////////////////////////////
namespace window 
{ 
  template<class Options>
  void create(Options const& opt)
  {
    std::cout << "A " << opt(tag::height) << "x" << opt(tag::width)
              << " window titled \"" << opt(tag::title,std::string("Untitled Window"))
              << "\" have been created at (" 
              << opt(tag::pos_x,0) << "," << opt(tag::pos_y,0) << ")\n";
  }
}

int main()
{
  window::create( boost::options[ tag::width = 120, tag::height = 90                             
                                , tag::title = std::string("[C]hoose a File ...")
                                ] 
                );

  window::create( boost::options[ tag::pos_x = 60, tag::width = 64, tag::height = 64] );
}

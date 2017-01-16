#include <iostream>

#include "page-reader.hpp"

int main (int argc, char *argv[])
{
  pageReader test;
  
  std::cout << "Fetching..." << std::endl;
  test.fetch("http://newgrounds.com");
  std::istringstream test_out; test_out.str( test.getRawData().str() );
  
  std::cout << "Printing HTML ressource of " << test.getURL() << " ..." << std::endl;
  
  std::string tmp;
  while( !test_out.eof() )
  {
    test_out >> tmp; std::cout << tmp << std::endl;
  }
  
  std::cout << std::endl << std::endl << test.getURL() << std::endl;
  
  return 0;
}

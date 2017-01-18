#include <iostream>

#include <ctime>

#include "page-reader.hpp"

int main (int argc, char *argv[])
{
  pageReader test;
  
  std::vector<std::string> urls_to_fetch;
  urls_to_fetch.push_back("http://www.newgrounds.com");
  urls_to_fetch.push_back("http://www.newgrounds.com/audio");
  urls_to_fetch.push_back("http://elfire.newgrounds.com");
  
  
  for( auto var : urls_to_fetch )
  {
    clock_t t = clock();
    test.fetch( var );
    t = clock() - t;
    
    /*
    std::istringstream test_out; test_out.str( test.getRawData().str() );
    std::cout << "Printing HTML ressource of " << test.getURL() << " ..." << std::endl;
    
    std::string tmp;
    while( !test_out.eof() )
    {
      test_out >> tmp; std::cout << tmp << std::endl;
    }
    */
    
    std::cout
    << "Url fetched: " << test.getURL() << std::endl
    << "File size:   " << test.getRawData().str().size() << std::endl
    << "Received in: " << (((float)t)/CLOCKS_PER_SEC) << " seconds" << std::endl << std::endl;
  }
  
  return 0;
}

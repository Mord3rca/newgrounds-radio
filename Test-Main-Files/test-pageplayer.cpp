#include <iostream>

#include "page-player.hpp"

int main( int argc, char* argv[] )
{
  audioPage test;
    test.fetch( "http://www.newgrounds.com/audio/random" ); //Test it, randomly ;)
    test(); //Process it
  
  auto tmp = reinterpret_cast< audioPage::rslt_type >( test.getRslt() );
  
  //If needed, insert clock shit here
  std::cout << "-- Printing Processed Data --" << std::endl;
  tmp.printAll( std::cout );
  std::cout << "-- Done --" << std::endl;
  
  return 0;
}
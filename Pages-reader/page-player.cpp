#include "page-player.hpp"

audioPage::audioPage()
{
}

audioPage::~audioPage(){}

void* audioPage::getRslt() const noexcept
{
  return (void*)data;
}

//Hell is here...
void audioPage::process()
{
  std::string tmp; unsigned int step = 0;
  while( this->getRawData() >> tmp )
  {
    
  }
}
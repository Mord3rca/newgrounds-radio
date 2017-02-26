#include "mpg-sound.hpp"

mpgSound::mpgSound()
{
  init();
}

mpgSound::mpgSound( std::ostream& _out, std::mutex& _mtx) : out(_out), out_mtx(_mtx), verbose(true)
{
  init();
}

mpgSound::~mpgSound()
{
  cleanup();
}

void mpgSound::init()
{
  
}

void mpgSound::cleanup()
{
  
}

void mpgSound::operator()()
{
  
}

bool mpgSound::pause()
{
  
}

bool mpgSound::play()
{
  
}

bool mpgSound::stop()
{
  
}

bool mpgSound::volume_change( double val )
{
  
}

bool mpgSound::volume_dec()
{
  
}

bool mpgSound::volume_inc()
{
  
}

bool mpgSound::soundFile_path( const char* _in )
{
  
}

bool mpgSound::soundFile_path( const std::string& _in )
{
  
}

bool mpgSound::state()
{ return (err == MPG123_OK); }

#ifndef MPG_SOUND_HPP
#define MPG_SOUND_HPP

#include <ostream> //For cout ref passed by main prog

//Includes for Threading
#include <atomic>
#include <mutex>
#include <thread>

//SOUND SHITS !!!
#include <mpg123.h>
#include <out123.h>

class mpgSound
{
public:
  mpgSound( void ); //Init & shits
  mpgSound( std::ostream&, std::mutex& ); //If logging is set
  ~mpgSound( void ); //Cleanup & stuff
  
  void operator()( void ); //Call when threaded
  
  //Some controls
  bool pause( void );
  bool play( void );
  bool stop( void );
  
  bool volume_change( double );
  bool volume_inc( void );
  bool volume_dec( void );
  
  bool soundFile_path( const char* );
  bool soundFile_path( const std::string& );
  
  bool state();
  
private:
  bool verbose = false;
  std::ostream& out;
  std::mutex&   out_mtx; //Locking when writting to an ostream (Debug/Verbose only ?)
  
  //Sound handle :)
  mpg123_handle* mh = nullptr;
  out123_handle* ao = nullptr;
  
  //All MPG123 related vars
  int err = MPG123_OK;
  
  void init();
  void cleanup();
  
  void getErr();
};

#endif //MPG_SOUND_HPP

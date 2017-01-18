#ifndef PAGE_READER_HPP
#define PAGE_READER_HPP

#include <curl/curl.h>
#include <json/json.h>

#include <string>
#include <vector>
#include <sstream>

/* Virtual class, contain access shits + basics method
 * Goal:  Just write the process stuff of the derivated class
 *        All I/O method should be there
*/

size_t receivedDataToISStream( char*, size_t, size_t, void* );

class pageReader
{
public:
  pageReader();
  ~pageReader();
  
  const std::string getURL() const noexcept;
  
  bool fetch( const std::string, CURLcode* = nullptr );
  
  const std::istringstream& getRawData() const noexcept;
  
protected:
  //virtual void process() = 0; //Should be call by the derivated class
  //virtual void* getRslt() = 0; //Result: void* = any structure
  
private:
  std::string url; //because, why not ? Could be usefull in case of forwarding & stuff
  std::istringstream data;
  
  CURL* curl;
  
};

#endif //PAGE_READER_HPP
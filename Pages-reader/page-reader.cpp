#include "page-reader.hpp"

size_t receivedDataToISStream(char* ptr, size_t size, size_t nmemb, void* userp)
{
  std::istringstream *obj = reinterpret_cast< std::istringstream* >( userp );
  size_t rslt = nmemb * size;
  std::string data = obj->str();
  
  data.append(ptr, rslt);
  obj->str( data );
  
  return rslt;
}

pageReader::pageReader()
{
  curl = curl_easy_init();
  
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receivedDataToISStream);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, static_cast<void*>(&data));
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
}

pageReader::~pageReader()
{
  curl_easy_cleanup(curl);
}

const std::istringstream& pageReader::getRawData() const noexcept
{
  return data;
}

const std::string pageReader::getURL() const noexcept
{
  return url;
}

bool pageReader::fetch( const std::string _url, CURLcode* errcode )
{
  if( curl )
  {
    
    if( curl_easy_setopt(curl, CURLOPT_URL, _url.c_str()) == CURLE_OK )
    {
      curl_easy_perform(curl);
      
      char* curl_url = nullptr;
      curl_easy_getinfo( curl, CURLINFO_EFFECTIVE_URL, &curl_url );
      
      url = curl_url;
      
      return true;
    }
  }
  
  return false;
}


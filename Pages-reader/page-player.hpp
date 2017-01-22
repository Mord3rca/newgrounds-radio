#ifndef AUDIOPAGE_HPP
#define AUDIOPAGE_HPP

#include "page-reader.hpp"

/*
 * Shitz that we can read:
 * All itemprop name are unique in the page
 * <h2 class="audio" itemprop="name">Elfire - All Systems Go</h2>
 * <meta itemprop="ratingCount" content="295"/> //Uint ?
 * <meta itemprop="worstRating" content="0"/> -- Well, float.
 * <meta itemprop="bestRating" content="5"/>
 * <meta itemprop="ratingValue" content="4.51"/>
 * 
 * Only the JSON config line is interesting:
 * audioPlayer(590, 32, {"filename":"http:\/\/audio.ngfiles.com\/400000\/400253_All_systems_go_Extended.mp3","name":"Elfire%20-%20All%20Systems%20Go","length":"312","loop":0,"artist":"Elfire","icon":"http:\/\/img.ngfiles.com\/defaults\/icon-audio.png"}, "http:\/\/flash.ngfiles.com\/audio_player\/audioplayer.swf");
 * 
 * Can also get the genre stuff 
 * <dd><a href="/audio/browse/genre/dance">Electronic - Dance</a></dd>
 * 
 * Tags: (None in this exemple but can be useful)
 * <dt class="tags">Tags:</dt>
 * <dd>None</dd>
 */

struct audioPageData
{
  std::string name;
  
  struct Rating
  {
    float worst = 0;
    float best = 0;
    float rate = 0;
    
    unsigned long count = 0;
  } rates;
  
  struct PlayerCfg
  {
    std::string filename;
    std::string name;
    
    unsigned int length = 0;
    bool isLoop = false;
    
    std::string artist;
    std::string iconUri;
  } player;
  
  std::string genre;
  std::string subgenre;
  
  std::vector< std::string > tags;
  
  void print(std::ostream&);
  void printRate(std::ostream&);
  void printPlayerCfg(std::ostream&);
  
  void printAll(std::ostream&);
};

class audioPage : public pageReader
{
public:
  audioPage();
  ~audioPage();
  
  void process();
  void* getRslt() const noexcept;
  
  typedef audioPageData* rslt_type;
  
private:
  audioPageData data;
  
  bool lookForMetaItem(const std::string&);
  bool lookForJSONLine(const std::string&);
  bool lookForGenre   (const std::string&);
  bool lookForTags    (const std::string&, std::istringstream&);
};

#endif // AUDIOPAGE_HPP

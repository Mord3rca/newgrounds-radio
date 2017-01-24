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
 
enum audioPage_Genre
{
  GENRE_EASY_LISTENING=0,
  GENRE_ELECTRONIC=1,
  GENRE_HIPHOP_RAP_RNB,
  GENRE_METAL_ROCK, //Fuck Yeah !
  GENRE_OTHER,
  GENRE_PODCASTS,
  GENRE_SOUTHERN_FLAVOR,
  GENRE_VOICE_ACTING,
  
  GENRE_ERROR
};

enum audioPage_Subgenre
{
  //Classical
  SUBGENRE_CLASSICAL=0, SUBGENRE_JAZZ=1, SUBGENRE_SOLO_INSTRUMENT=2,
  
  //Electrnics
  SUBGENRE_AMBIANT, SUBGENRE_CHIPSTEP, SUBGENRE_DANCE, SUBGENRE_DRUMNBASS, SUBGENRE_DUPSTEP,SUBGENRE_HOUSE,
  SUBGENRE_INDUSTRIAL, SUBGENRE_NEW_WAVE, SUBGENRE_SYNTHWAVE, SUBGENRE_TECHNO, SUBGENRE_TRANCE, SUBGENRE_VIDEO_GAME,
  
  //Hip Hop, Rap, R&B
  SUBGENRE_HIPHOP_MODERN, SUBGENRE_HIPHOP_OLSKOOL, SUBGENRE_NERDCORE, SUBGENRE_RNB,
  
  //Metal, Rock
  SUBGENRE_BRIT_POP, SUBGENRE_CLASSICAL_ROCK, SUBGENRE_GENERAL_ROCK, SUBGENRE_GRUNGE,
  SUBGENRE_HEAVY_METAL, SUBGENRE_INDIE, SUBGENRE_POP, SUBGENRE_PUNK,
  
  //Other
  SUBGENRE_CINEMATIC, SUBGENRE_EXPERIMENTAL, SUBGENRE_FUNK, SUBGENRE_FUSION, SUBGENRE_GOTH,
  SUBGENRE_MISCELLANEOUS, SUBGENRE_SKA, SUBGENRE_WORLD,
  
  //Podcasts
  SUBGENRE_DISCUSSION, SUBGENRE_MUSIC, SUBGENRE_STORYTELLING,
  
  //Southern Flavor
  SUBGENRE_BLUEGRASS, SUBGENRE_BLUES, SUBGENRE_COUNTRY,
  
  //Voice Acting
  SUBGENRE_ACAPELLA, SUBGENRE_COMEDY, SUBGENRE_CREEPYPASTA,SUBGENRE_DRAMA,
  SUBGENRE_INFORMATIONAL, SUBGENRE_SPOKEN_WORLD, SUBGENRE_VOICE_DEMO,
  
  SUBGENRE_ERROR
};

extern const char* audioPage_GenreList[];
extern const char* audioPage_SubgenreList[];

//Used to guess url shits string in lookForGenre
extern const char* audioPage_UrlGenreList[];

struct audioPageData
{
  std::string name;
  
  struct Rating
  {
    float worst = 0;
    float best  = 0;
    float rate  = 0;
    
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
  
  audioPage_Genre     genre     = GENRE_ERROR;
  audioPage_Subgenre  subgenre  = SUBGENRE_ERROR;
  
  std::vector< std::string > tags;
  
  const char* formattedGenre();
  const char* formattedSubgenre();
  
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
  
  void urlSubgenreToEnum( const std::string& );
  
  bool lookForMetaItem(const std::string&);
  bool lookForJSONLine(const std::string&);
  bool lookForGenre   (const std::string&);
  bool lookForTags    (const std::string&, std::istringstream&);
};

#endif // AUDIOPAGE_HPP

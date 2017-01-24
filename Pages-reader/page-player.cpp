#include "page-player.hpp"

const char* audioPage_GenreList[] =
{
  "Easy Listening", "Electronic", "Hip Hop, Rap, R&B",
  "Metal, Rock", "Other", "Podcasts", "Southern Flavor",
  "Voice Acting", "GENRE_ERROR", nullptr
};

const char* audioPage_SubgenreList[] =
{
  //Easy Listening
  "Classical", "Jazz", "Solo Instrument",
  
  //Electronic
  "Ambient", "Chipstep", "Dance", "Dum N Bass",
  "Dubstep", "House", "Industrial", "New Wave",
  "Synthwave", "Techno", "Trance", "Video Game",
  
  //Hip Hop, Rap, R&B
  "Hip Hop - Modern", "Hip Hop - Olskool", "Nerdcore", "R&B",
  
  //Metal, Rock
  "Brit Pop", "Classic Rock", "General Rock", "Grunge",
  "Heavy Metal", "Indie", "Pop", "Punk",
  
  //Other
  "Cinematic", "Experimental", "Funk", "Fusion",
  "Goth", "Miscallaneous", "Ska", "World",
  
  //Podcasts
  "Discussion", "Music", "Storytelling",
  
  //Southern Flavor
  "Bluegrass", "Blues", "Country",
  
  //Voice Acting
  "A Capella", "Comedy", "Creepypasta", "Drama",
  "Informational", "Spoken World", "Voice Demo",
  
  "SUBGENRE_ERROR", nullptr
};

//And the fun can continue here !
const char* audioPage_UrlGenreList[] =
{
  //Easy Listening
  "classical", "jazz", "solo-instrument",
  
  //Electronic
  "ambient", "chipstep", "dance", "drum-n-bass", "dupstep",
  "house", "industrial", "new-wave", "synthwave", "techno",
  "trance", "video-game",
  
  //Hip Hop, Rap, R&B
  "hip-hop---modern", "hip-hop---olskool", "nerdcore", "rb",
  
  //Metal Rock
  "brit-pop", "classic-rock", "general-rock", "grunge",
  "heavy-metal", "indie", "pop", "punk",
  
  //Other
  "cinematic", "experimental", "funk", "fusion",
  "goth", "miscellaneous", "ska", "world",
  
  //Podcasts
  "bluegrass", "blues", "country",
  
  //Voice Acting
  "a-capella", "comedy", "creepypasta", "drama",
  "informational", "spoken-world", "voice-demo",
  
  //One day if a song have no subgenre:
  //"easy-listening", "electronic", "hip-hop-rap-rb", "metal-rock",
  //"other", "podcasts", "southern-flavor", "voice-acting",
  
  nullptr
};

audioPage::audioPage(){}

audioPage::~audioPage(){}

void* audioPage::getRslt() const noexcept
{
  return (void*)&data;
}

//Hell is here...
void audioPage::process()
{
  std::string tmp; unsigned char step = 0;
  unsigned char metaItem = 0;
  //std::istringstream tmp_stream; tmp_stream.str( getRawData().str() );
  
  while( std::getline( getRawData(), tmp ) )
  {
    switch( step )
    {
      case 0:
        if( lookForMetaItem( tmp ) )
        {
          metaItem++;
          //Check if all Items are found
          if( metaItem > 4 )
            step++;
        }
        break;
        
      case 1:
        if( lookForJSONLine( tmp ) )
          step++; //Next step !
        break;
      
      case 2:
        if( lookForGenre( tmp ) )
          step++;
        break;
      
      case 3:
        if( lookForTags( tmp, getRawData() ) )
          step++;
        break;
          
      default:
        break;
    }
    
    if( step > 3 )
      break;
  }
  
  //Cleaning up to free the memory from the song commentary
  //They will be used another time ! In an API maybe ?
  this->getRawData().str("");
}

bool audioPage::lookForMetaItem(const std::string& line)
{
  std::string::size_type itempos = line.find("itemprop");
  if( itempos != std::string::npos )
  {
    std::string::size_type pos1, pos2;
    
    if( line.find("name", itempos) != std::string::npos )
    {
      //Get the name !
      pos1 = line.find(">", itempos) +1; pos2 = line.rfind("<");
      data.name = line.substr( pos1, pos2 - pos1 );
      
      return true;
    }
    else if( line.find("ratingCount", itempos) != std::string::npos )
    {
      //Get the rating count !
      std::string::size_type tmpPos = line.rfind( "content" );
      pos1 = line.find( "\"", tmpPos ) +1; pos2 = line.rfind( "\"" );
      std::string strVal = line.substr( pos1, pos2 - pos1 );
      
      data.rates.count = std::stoul( strVal );
      //data.rates.count = 404;
    }
    else if( line.find("worstRating", itempos) != std::string::npos )
    {
      //Get the Worst Rating !
      std::string::size_type tmpPos = line.rfind( "content" );
      pos1 = line.find( "\"", tmpPos ) +1; pos2 = line.rfind( "\"" );
      std::string strVal = line.substr( pos1, pos2 - pos1 );
      
      data.rates.worst = std::stof( strVal );
      //data.rates.worst = 404;
    }
    else if( line.find("bestRating", itempos) != std::string::npos )
    {
      //Get the Best rating :)
      std::string::size_type tmpPos = line.rfind( "content" );
      pos1 = line.find( "\"", tmpPos ) +1; pos2 = line.rfind( "\"" );
      std::string strVal = line.substr( pos1, pos2 - pos1 );
      
      data.rates.best = std::stof( strVal );
      //data.rates.best = 404;
    }
    else if( line.find("ratingValue", itempos) != std::string::npos )
    {
      //Get the Rating value !
      std::string::size_type tmpPos = line.rfind( "content" );
      pos1 = line.find( "\"", tmpPos ) +1; pos2 = line.rfind( "\"" );
      std::string strVal = line.substr( pos1, pos2 - pos1 );
      
      data.rates.rate = std::stof( strVal );
      //data.rates.rate = 404;
    }
    else // If any of these keywords are present...
      return false;
    
    //If we're here, we found & parse something ;)
    return true;
  }
  
  return false;
}

bool audioPage::lookForJSONLine(const std::string& line)
{
  std::string::size_type itemPos = line.find("audioPlayer");
  if( itemPos != std::string::npos )
  {
    std::string::size_type pos1, pos2;
    pos1 = line.find("{", itemPos); pos2 = line.find( "}", pos1 );
    std::string json_str = line.substr( pos1, pos2 - pos1 + 1);
    
    Json::Reader  reader;
    Json::Value   val;
    
    if( reader.parse( json_str ,val ) )
    {
      data.player.filename  = val.get("filename", "").asString();
      data.player.name      = val.get("name", "").asString();
      
      data.player.length    = std::atoi( val.get("length", "").asString().c_str() );
      data.player.isLoop    = val.get("loop", 0).asInt() != 0;
      
      data.player.artist    = val.get("artist", "").asString();
      data.player.iconUri   = val.get("icon", "").asString();
    }
    else
      data.player.name = "JSON Parsing error...";
    
    return true;
  }
  
  return false;
}

bool audioPage::lookForGenre   (const std::string& line)
{
  const std::string str_const_tmp("/audio/browse/genre/");
  std::string::size_type pos = line.find( str_const_tmp );
  
  if( pos != std::string::npos )
  {
    pos += str_const_tmp.length();
    std::string::size_type pos2 = line.find( "\"", pos);
    
    std::string urlsubgenre = line.substr( pos, pos2 - pos);
    
    urlSubgenreToEnum( urlsubgenre );
    
    return true;
  }
  
  return false;
}

void audioPage::urlSubgenreToEnum( const std::string& in )
{
  enum audioPage_Subgenre rslt = SUBGENRE_ERROR;
  
  for( unsigned int i = 0; i < SUBGENRE_ERROR; i++ )
  {
    if( in.find( audioPage_UrlGenreList[i] ) != std::string::npos )
    {
      rslt = (enum audioPage_Subgenre)i; break;
    }
  }
  
  //This switch will be long ... Just for guessing the genre
  switch( rslt )
  {
    case SUBGENRE_CLASSICAL:
    case SUBGENRE_JAZZ:
    case SUBGENRE_SOLO_INSTRUMENT:
      data.genre = GENRE_EASY_LISTENING;
      break;
    
    case SUBGENRE_AMBIANT:
    case SUBGENRE_CHIPSTEP:
    case SUBGENRE_DANCE:
    case SUBGENRE_DRUMNBASS:
    case SUBGENRE_DUPSTEP:
    case SUBGENRE_HOUSE:
    case SUBGENRE_INDUSTRIAL:
    case SUBGENRE_NEW_WAVE:
    case SUBGENRE_SYNTHWAVE:
    case SUBGENRE_TECHNO:
    case SUBGENRE_TRANCE:
    case SUBGENRE_VIDEO_GAME:
      data.genre = GENRE_ELECTRONIC;
      break;
    
    case SUBGENRE_HIPHOP_MODERN:
    case SUBGENRE_HIPHOP_OLSKOOL:
    case SUBGENRE_NERDCORE:
    case SUBGENRE_RNB:
      data.genre = GENRE_HIPHOP_RAP_RNB;
      break;
    
    case SUBGENRE_BRIT_POP:
    case SUBGENRE_CLASSICAL_ROCK:
    case SUBGENRE_GENERAL_ROCK:
    case SUBGENRE_GRUNGE:
    case SUBGENRE_HEAVY_METAL:
    case SUBGENRE_INDIE:
    case SUBGENRE_POP:
    case SUBGENRE_PUNK:
      data.genre = GENRE_METAL_ROCK;
      break;
    
    case SUBGENRE_CINEMATIC:
    case SUBGENRE_EXPERIMENTAL:
    case SUBGENRE_FUNK:
    case SUBGENRE_FUSION:
    case SUBGENRE_GOTH:
    case SUBGENRE_MISCELLANEOUS:
    case SUBGENRE_SKA:
    case SUBGENRE_WORLD:
      data.genre = GENRE_OTHER;
      break;
    
    case SUBGENRE_DISCUSSION:
    case SUBGENRE_BLUES:
    case SUBGENRE_COUNTRY:
      data.genre = GENRE_PODCASTS;
      break;
    
    case SUBGENRE_ACAPELLA:
    case SUBGENRE_COMEDY:
    case SUBGENRE_CREEPYPASTA:
    case SUBGENRE_DRAMA:
    case SUBGENRE_INFORMATIONAL:
    case SUBGENRE_SPOKEN_WORLD:
    case SUBGENRE_VOICE_DEMO:
      data.genre = GENRE_VOICE_ACTING;
      break;
    
    default:
      data.genre = GENRE_ERROR;
  }
  
  data.subgenre = rslt;
}

//Stream Needed ... Cauz it's multiline
bool audioPage::lookForTags    (const std::string& line, std::istringstream& in)
{
  const std::string str_to_find("class=\"tags\"");
  const std::string str_end("</dl>");
  const std::string str_tag("/audio/browse/tag/");
  
  std::string::size_type pos = line.find( str_to_find );
  if( pos != std::string::npos )
  {
    std::string tmp_line;
    while( std::getline( in, tmp_line ) )
    {
      if( line.find( str_end ) != std::string::npos )
        break;
      
      pos = tmp_line.find( str_tag );
      if( pos != std::string::npos )
      {
        data.tags.push_back(  tmp_line.substr( pos + str_tag.length(),
                              tmp_line.find( "\"", pos + str_tag.length())
                              - (pos + str_tag.length()) ) );
      }
    }
    
    return true;
  }
  
  return false;
}

const char* audioPageData::formattedGenre()
{
  return audioPage_GenreList[ this->genre ];
}

const char* audioPageData::formattedSubgenre()
{
  return audioPage_SubgenreList[ this->subgenre ];
}

//Begin prints function of data structure
void audioPageData::print( std::ostream& out )
{
  out << "Filename: " << name << std::endl
      << "Genre:    " << formattedGenre() << std::endl
      << "SubGenre: " << formattedSubgenre() << std::endl
      << "-- Tags --" << std::endl;
      
      if( tags.size() > 0 )
      {
        for( auto i : tags )
        {
          out << i << " ";
        }
      }
      else
        out << "None";
      
      out << std::endl;
}

void audioPageData::printPlayerCfg( std::ostream& out )
{
  out << "URI:      " << player.filename << std::endl
      << "Name:     " << player.name << std::endl
      << "Length:   " << player.length << std::endl
      << "Loop ?    " << ( player.isLoop ? "Yes":"No" ) << std::endl
      << "Artist:   " << player.artist << std::endl
      << "Icon URI: " << player.iconUri << std::endl;
}

void audioPageData::printRate( std::ostream& out )
{
  out << "Rate:  " << rates.rate << std::endl
      << "Worst: " << rates.worst << std::endl
      << "Best:  " << rates.best << std::endl
      << "Count: " << rates.count << std::endl;
}

void audioPageData::printAll( std::ostream& out )
{
  out << "-- General Information --" << std::endl;
      print( out );
      
  out << "-- Player Information --" << std::endl;
      printPlayerCfg( out );
      
  out << "-- Song Rates --" << std::endl;
    printRate( out );
}

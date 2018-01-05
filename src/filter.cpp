#include "filter.hpp"

const std::array<std::string, 32> ng::audio::genre_list = {{
  "Ambient","Bluegrass","Blues","Brit Pop","Classical",
  "Classical Rock","Country","Dance","Drum N Bass",
  "Dubstep","Funk","General Rock","Goth","Grunge",
  "Heavy Metal","Hip Hop - Modern","Hip Hop - Olskool",
  "House","Indie","Industrial","Jazz","Latin",
  "Miscellaneous","New Wave","Pop","Punk","R&B","Ska",
  "Techno","Trance","Video Game","Voice Acting"
}};

std::vector< enum ng::audio::genre > ng::audio::filter::_default_list = {
  ng::audio::genre::AMBIENT, ng::audio::genre::BLUEGRASS, ng::audio::genre::BLUES,
  ng::audio::genre::BRIT_POP, ng::audio::genre::CLASSICAL, ng::audio::genre::CLASSICAL_ROCK,
  ng::audio::genre::COUNTRY, ng::audio::genre::DANCE, ng::audio::genre::DRUMNBASS,
  ng::audio::genre::DUPSTEP, ng::audio::genre::FUNK, ng::audio::genre::GENERAL_ROCK,
  ng::audio::genre::GOTH, ng::audio::genre::GRUNGE, ng::audio::genre::HEAVY_METAL,
  ng::audio::genre::HIPHOP_MODERN, ng::audio::genre::HIPHOP_OLSKOOL,
  ng::audio::genre::HOUSE, ng::audio::genre::INDIE, ng::audio::genre::INDUSTRIAL,
  ng::audio::genre::JAZZ, ng::audio::genre::LATIN, ng::audio::genre::MISCELLANEOUS,
  ng::audio::genre::NEW_WAVE, ng::audio::genre::POP, ng::audio::genre::PUNK,
  ng::audio::genre::RB, ng::audio::genre::SKA, ng::audio::genre::TECHNO,
  ng::audio::genre::TRANCE, ng::audio::genre::VIDEO_GAME, ng::audio::genre::VOICE_ACTING
};

const std::string ng::audio::getGenreStr( const enum ng::audio::genre e )
{
  if( e != ng::audio::genre::NOT_SET )
    return genre_list[e];
  else
    return "NOT SET";
}

ng::audio::filter::filter() : minScore(0), maxScore(5),
                              minDate("2003/01/01")
{
  time_t rawtime; time(&rawtime);
  struct tm *timeinfo = localtime(&rawtime);
  char buffer[12]; std::strftime(buffer, 12, "%Y/%m/%d", timeinfo);
  maxDate = buffer;
  
  _genres = _default_list;
}

//No PTR, no need to clean up
ng::audio::filter::~filter(){}

void ng::audio::filter::setScoreRange(float min, float max) noexcept
{
  minScore = min;
  maxScore = max;
}

//TODO: Validate by regexp, throw an exception if not.
void ng::audio::filter::setDateRange(const std::string min, const std::string max) noexcept
{
  minDate = min;
  maxDate = max;
}

void ng::audio::filter::setGenre( const std::vector< enum ng::audio::genre > e )
{
  _genres = e;
}

void ng::audio::filter::appendGenre( const enum ng::audio::genre e )
{
  if( e != ng::audio::genre::NOT_SET )
    _genres.push_back(e);
}

void ng::audio::filter::deleteGenre( const enum ng::audio::genre e )
{
  _genres.erase(std::remove(_genres.begin(), _genres.end(), e), _genres.end());
}

void ng::audio::filter::emptyGenre( void ) noexcept
{
  _genres.clear();
}

const float ng::audio::filter::getMinScore( void ) const noexcept
{
  return minScore;
}

const float ng::audio::filter::getMaxScore( void ) const noexcept
{
  return maxScore;
}

const std::string ng::audio::filter::getMinDate( void ) const noexcept
{
  return minDate;
}

const std::string ng::audio::filter::getMaxDate( void ) const noexcept
{
  return maxDate;
}

const std::string ng::audio::filter::serialize( enum ng::audio::filter::serial_type e ) const
{
  std::ostringstream out;
  
  if( e == ng::audio::filter::serial_type::DEFAULT )
    return "Not implemented yet.";
  
  out << "{"
      << "\"maxScore\":" << maxScore << ", \"minScore\":" << minScore
      << ", \"minDate\":\"" << minDate << "\", \"maxDate\":\"" << maxDate
      << "\", \"genres\":[ ";
  
  for( unsigned int i = 0; i < _genres.size(); i++ )
  {
    out << "\"" << getGenreStr( _genres[i] ) << (i == _genres.size() -1 ? "\" " : "\", ");
  }
  
  out << "]}";
  
  return out.str();
}

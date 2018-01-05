#ifndef NG_FILTER_HEADER
#define NG_FILTER_HEADER

#include <string>
#include <algorithm>
#include <vector>
#include <array>
#include <sstream>

#include <ctime> //Needed to get the current date (maxDate from filter)

namespace ng
{
  namespace audio
  {
    extern const std::array< std::string, 32 > genre_list;
    
    enum genre
    {
      AMBIENT = 0, BLUEGRASS = 1, BLUES = 2, BRIT_POP, CLASSICAL,
      CLASSICAL_ROCK, COUNTRY, DANCE, DRUMNBASS, DUPSTEP, FUNK,
      GENERAL_ROCK, GOTH, GRUNGE, HEAVY_METAL, HIPHOP_MODERN,
      HIPHOP_OLSKOOL, HOUSE, INDIE, INDUSTRIAL, JAZZ, LATIN,
      MISCELLANEOUS, NEW_WAVE, POP, PUNK, RB, SKA, TECHNO,
      TRANCE, VIDEO_GAME, VOICE_ACTING,
      NOT_SET = -1
    };
    
    const std::string getGenreStr( const enum genre );
    
    class filter
    {
    public:
      filter();
      ~filter();
      
      // SELECT Default mode or NG AUDIO BRIDGE Compatibility
      enum serial_type
      {
        DEFAULT, COMPAT_AUDIO_BRIGDE
      };
      
      //Date/Score modifiers
      void setScoreRange(float=0, float=5) noexcept;
      void setDateRange(const std::string, const std::string) noexcept;
      
      //Genre modifiers
      void setGenre( const std::vector< enum genre > );
      void appendGenre( const enum genre );
      void deleteGenre( const enum genre );
      void emptyGenre( void ) noexcept;
      
      //Access fonctions
      const float getMinScore( void ) const noexcept;
      const float getMaxScore( void ) const noexcept;
      
      const std::string getMinDate( void ) const noexcept;
      const std::string getMaxDate( void ) const noexcept;
      
      //At the end, all we need is to send a JSON !
      const std::string serialize( enum serial_type = DEFAULT ) const;
      
    private:
      std::vector< enum genre > _genres;
      float minScore, maxScore;
      std::string minDate, maxDate;
      
      static std::vector< enum genre > _default_list;
    };
  }
}

#endif //NG_FILTER_HEADER

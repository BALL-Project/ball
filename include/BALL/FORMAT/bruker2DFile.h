#include <vector>
#include <glib.h>

#include <BALL/SYSTEM/file.h>
#include <BALL/DATATYPE/string.h>

#include <BALL/FORMAT/JCAMPFile.h>

using namespace BALL;

class Bruker2D : public BALL::File
{
 public:
  /**
   * Konstruktor. Wichtig: name ist hier der Name des Bruker-*Verzeichnisses*
   */
  Bruker2D( const String& name, OpenMode open_mode = IN|BINARY );

  /**
   * Copy-Konstruktor.
   */
  Bruker2D( const Bruker2D& file );

  /**
   * Destruktor.
   */
  ~Bruker2D();

  /**
   * Read a spectrum.
   */
  void read( vector<double>& dat );

  /**
   * These classes give access to the parameters used in aqcuiring the
   * spectrum.
   */
  BrukerParameter *parsf1_, *parsf2_;

 protected:
  int minx_, maxx_, miny_, maxy_;
};

#include <vector>
#include <glib.h>

#include <BALL/SYSTEM/file.h>
#include <BALL/DATATYPE/string.h>

#include "BrukerParameter.h"

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
   * Einlesen eines 2D-Spektrums nach dat.
   */
  void read( vector<double>& dat );

 protected:
  BrukerParameter *parsf1_, *parsf2_;
  int minx_, maxx_, miny_, maxy_;
};

#include <vector>
#include <glib.h>

#include <BALL/SYSTEM/file.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/FORMAT/JCAMPFile.h>

using namespace BALL;

class Bruker2D
	: public File
{
 public:

  /**	Constructor.
			@param name important: name of the Bruker-*directory*
  */
  Bruker2D(const String& name, OpenMode open_mode = IN|BINARY);

  /**	Copy-Constructor.
  */
  Bruker2D(const Bruker2D& file);

  /**	Destructor.
  */
  ~Bruker2D();

  /** Read a spectrum.
  */
  void read(vector<double>& dat);

  /**	These classes give access to the parameters used in processing the
			spectrum.
  */
  BrukerParameter* parsf1_;
	BrukerParameter* parsf2_;

 protected:
  int minx_, maxx_, miny_, maxy_;
};

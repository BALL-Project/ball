// $Id: bruker2DFile.h,v 1.5 2000/11/10 17:14:45 anhi Exp $

#ifndef BALL_FORMAT_BRUKER2DFILE_H
#define BALL_FORMAT_BRUKER2DFILE_H

#include <vector>
#include <glib.h>

#include <list.h>

<<<<<<< bruker2DFile.h
#ifndef BALL_SYSTEM_FILE_H
#       include <BALL/SYSTEM/file.h>
#endif
=======
#include <BALL/SYSTEM/file.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/FORMAT/JCAMPFile.h>
>>>>>>> 1.3

#ifndef BALL_DATATYPE_STRING_H
#       include <BALL/DATATYPE/string.h>
#endif

<<<<<<< bruker2DFile.h
#ifndef BALL_FORMAT_JCAMPFILE_H
#       include <BALL/FORMAT/JCAMPFile.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA2D_H
#       include <BALL/DATATYPE/regularData2D.h>
#endif

namespace BALL
=======
class Bruker2D
	: public File
>>>>>>> 1.3
{

class Bruker2D 
      : public File
{
 public:
<<<<<<< bruker2DFile.h
  /**
   * Konstruktor. Wichtig: name ist hier der Name des Bruker-*Verzeichnisses*
   */
  Bruker2D( const String& name, OpenMode open_mode = IN|BINARY );

  /**
   * Copy-Konstruktor.
   */
  Bruker2D( const Bruker2D& file );

  Bruker2D();

  /**
   * Destruktor.
   */
=======

  /**	Constructor.
			@param name important: name of the Bruker-*directory*
  */
  Bruker2D(const String& name, OpenMode open_mode = IN|BINARY);

  /**	Copy-Constructor.
  */
  Bruker2D(const Bruker2D& file);

  /**	Destructor.
  */
>>>>>>> 1.3
  ~Bruker2D();

<<<<<<< bruker2DFile.h
  /**
   * Read a spectrum.
   */
  void read(vector<double>& dat);

  /** Read a spectrum. It will be stored in spectrum_;
   */
  void read();

  /** Return a reference to the spectrum.
   */
  RegularData2D* GetData();

  /**
   * Returns a list of endpoints of a contour-line corresponding to the value "threshold".
   * The coordinates are given in the form of shifts.
   */
  list<pair <double, double> > &GetContourLine(double threshold);

  /**
   * Returns a list of peaks found in the spectrum.
   */
  list< pair<int, int> > &GetPeakList();

  /**
   * Returns the shift corresponding to a position in the bitmap.
   */
  pair<double, double> GetShift(Position x, Position y);

  /**
   * Returns the coordinates of a point in the original data next to the given coordinates.
   */
  pair<Position, Position> GetPosition(double x, double y);

  /**
   * Set the parameters needed for conversion index<->shift.
   */
  void SetShiftRange(double offsetf1, double offsetf2, double swidthf1, double swidthf2, double bfreqf1, double bfreqf2, double spointnumf1, double spointnumf2);

  /**
   * These classes give access to the parameters used in aqcuiring the
   * spectrum.
   */
  JCAMPFile *parsf1_, *parsf2_;
=======
  /** Read a spectrum.
  */
  void read(vector<double>& dat);

  /**	These classes give access to the parameters used in processing the
			spectrum.
  */
  BrukerParameter* parsf1_;
	BrukerParameter* parsf2_;
>>>>>>> 1.3

 protected:
  Size minx_;
  Size maxx_;
  Size miny_;
  Size maxy_;
  // To be replaced by regularData2D
  vector<double> *dat_;
  double soffsetf1_, soffsetf2_;
  double swidthf1_, swidthf2_;
  double bfreqf1_, bfreqf2_;
  Size spointnumf1_, spointnumf2_;
  RegularData2D spectrum_;
};

}

#endif

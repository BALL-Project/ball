
// $Id: bruker2DFile.h,v 1.13 2001/12/30 13:28:37 sturm Exp $

#ifndef BALL_FORMAT_BRUKER2DFILE_H
#define BALL_FORMAT_BRUKER2DFILE_H

#ifndef BALL_SYSTEM_FILE_H
# include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_FORMAT_JCAMPFILE_H
# include <BALL/FORMAT/JCAMPFile.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_FORMAT_JCAMPFILE_H
# include <BALL/FORMAT/JCAMPFile.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA2D_H
# include <BALL/DATATYPE/regularData2D.h>
#endif

#include <vector>
#include <list>


namespace BALL
{

/**	Bruker 2D spectrum format.
		This class....	\\
		{\bf Definition:}\URL{BALL/FORMAT/bruker2DFile.h}
*/
// ?????
class Bruker2D 
  : public File
{
 public:

  /** Default constructor.
   */
  Bruker2D();

  /**	Constructor.
			@param name important: name of the Bruker-*directory*
  */
  Bruker2D(const String& name, OpenMode open_mode = IN|BINARY)
		throw(Exception::FileNotFound);

  /**	Copy-Constructor.
  */
  Bruker2D(const Bruker2D& file)
		throw(Exception::FileNotFound);

  /**	Destructor.
  */
  ~Bruker2D()
		throw();

  /** Read a spectrum. 
			It will be stored in spectrum_;
  */
  void read();

  /** Read a spectrum from "name". 
			It will be stored in spectrum_;
  */
  void read(const String &name);

  /** Return a reference to the spectrum.
  */
  RegularData2D* GetData();

  /**	Returns a list of peaks found in the spectrum.
  */
  std::list<std::pair<int, int> >& GetPeakList();

  /** Returns the shift corresponding to a position in the bitmap.
  */
	std::pair<double, double> GetShift(Position x, Position y);

  /** Returns the coordinates of a point in the original data next to the given coordinates.
  */
  std::pair<Position, Position> GetPosition(double x, double y);

  /** Set the parameters needed for conversion index<->shift.
  */
  void SetShiftRange(double offsetf1, double offsetf2, double swidthf1, double swidthf2, 
										 double bfreqf1, double bfreqf2, double spointnumf1, double spointnumf2);

  /** These classes give access to the parameters used in aqcuiring the spectrum.
  */
  JCAMPFile *parsf1_, *parsf2_;

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

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/bruker2DFile.h>
#include <BALL/SYSTEM/binaryFileAdaptor.h>

using namespace std;

namespace BALL
{
	Bruker2DFile::Bruker2DFile() 
		: File()
	{
		//????
	}

	Bruker2DFile::Bruker2DFile(const String& name, OpenMode open_mode) 
		: File(name + FileSystem::PATH_SEPARATOR + "2rr", open_mode),
			parsf1_(name + FileSystem::PATH_SEPARATOR + "proc2s"),
			parsf2_(name + FileSystem::PATH_SEPARATOR + "procs")
	{
		parsf1_.read();
		parsf2_.read();
		miny_ = (int)parsf1_.getIntValue("YMIN_p");
		maxy_ = (int)parsf1_.getIntValue("YMAX_p");
		minx_ = (int)parsf2_.getIntValue("YMIN_p");
		maxx_ = (int)parsf2_.getIntValue("YMAX_p");
		read();
	}

	Bruker2DFile::~Bruker2DFile()
	{
	}

	void Bruker2DFile::read(const String& name)
	{
		parsf1_.open(name + FileSystem::PATH_SEPARATOR + "proc2s");
		parsf2_.open(name + FileSystem::PATH_SEPARATOR + "procs");
		parsf1_.read();
		parsf2_.read();
		parsf1_.close();
		parsf2_.close();
		miny_ = (int)parsf1_.getIntValue("YMIN_p");
		maxy_ = (int)parsf1_.getIntValue("YMAX_p");
		minx_ = (int)parsf2_.getIntValue("YMIN_p");
		maxx_ = (int)parsf2_.getIntValue("YMAX_p");
		
	  close();
	  open(name + FileSystem::PATH_SEPARATOR + "2rr");
	  read();
	}

	void Bruker2DFile::read()
	{
		// first we will have to find out whether we are using big or little
		// endian on this machine.
		unsigned int endTest = 1;
		BinaryFileAdaptor<BALL_INT32> adapt_int32_t_;
		adapt_int32_t_.setSwapEndian((*(char*)&endTest == 1) != (parsf1_.getDoubleValue("BYTORDP") == 1.0));

	  int SIF1_   = (int) parsf1_.getIntValue( "SI"   ); // Y - spacing
	  int SIF2_   = (int) parsf2_.getIntValue( "SI"   ); // X - spacing
	  int XDIMF1_ = (int) parsf1_.getIntValue( "XDIM" );
	  int XDIMF2_ = (int) parsf2_.getIntValue( "XDIM" );

#ifdef BALL_DEBUG
		Log.info() << "SIF1: " << SIF1_ << "--" << parsf1_.getIntValue("SI") << parsf1_.getDoubleValue("SI")
		           << " SIF2: " << SIF2_ << "--" << parsf2_.getIntValue( "SI") << parsf2_.getDoubleValue( "SI")
		           << " XDIMF1: " <<  XDIMF1_<<"--"<< parsf1_.getIntValue( "XDIM" )  << parsf1_.getDoubleValue( "XDIM" )
		           << " XDIMF2: " << XDIMF2_<<"--"<< parsf2_.getIntValue( "XDIM" ) <<  parsf2_.getDoubleValue( "XDIM" )  << std::endl;
#endif
	  // prepare the regularData
	  //spectrum_.setXSize(SIF2_);
	  //spectrum_.setYSize(SIF1_);
	  //spectrum_.resize(SIF2_, SIF1_);

	  double a = parsf2_.getDoubleValue( "OFFSET" );
	  double b = parsf2_.getDoubleValue( "OFFSET" ) - (parsf2_.getDoubleValue( "SW_p" ) / parsf2_.getDoubleValue( "SF" ));

#ifdef BALL_DEBUG
		Log.info() << "parsf2: Offset: " << a << " b: " << b << " SW_P: " <<parsf2_.getDoubleValue( "SW_p" ) << " SF: " <<  parsf2_.getDoubleValue( "SF" ) << std::endl;
#endif

		double lower_x = (a<b) ? a : b;
		double upper_x = (a>b) ? a : b;
		
	  a = parsf1_.getDoubleValue( "OFFSET" );
	  b = parsf1_.getDoubleValue( "OFFSET" ) - (parsf1_.getDoubleValue( "SW_p" ) / parsf1_.getDoubleValue( "SF" ));

#ifdef BALL_DEBUG
		Log.info() <<"parsf1: Offset: " << a << " SW_P: " << parsf1_.getDoubleValue( "SW_p" ) << " SF: " <<  parsf1_.getDoubleValue("SF") << std::endl;
#endif

		double lower_y = (a<b) ? a : b;
		double upper_y = (a>b) ? a : b;

	  //spectrum_.setLowerBound(parsf1_.getIntValue( "YMIN_p" ));
	  //spectrum_.setUpperBound(parsf1_.getIntValue( "YMAX_p" ));

		spectrum_ = RegularData2D(RegularData2D::IndexType(SIF2_, SIF1_),
															Vector2(lower_x, lower_y), 
															Vector2(upper_x, upper_y) - Vector2(lower_x, lower_y)); 

	  // Back to the beginning of the file.
	  this->reopen( );
	  
	  int matNumF2 = (int) (SIF2_ / XDIMF2_); // Number of matrices in x - direction
	  int matNumF1 = (int) (SIF1_ / XDIMF1_); // Number of matrices in y - direction

		for (int actMat=0; actMat < matNumF2 * matNumF1; actMat++ ) 
		{ // Walk through all submatrices
			for (int f1 = 0; f1 < XDIMF1_; f1++ ) 
			{   // for each matrix: look at every row
				for (int f2 = 0; f2 < XDIMF2_; f2++ ) 
				{ // look at every column
					if (!this->good())
					{
						break;
					}

					(*this) >> adapt_int32_t_;

					// We need to know the number of the matrix we are looking at
					// right now.
					int actMatF2 = (actMat % matNumF2); // x - coordinate of submatrix
					int actMatF1 = (actMat / matNumF2); // y - coordinate of submatrix

					spectrum_[ f2 + XDIMF2_ * actMatF2 + ( ( f1 + XDIMF1_ * actMatF1 ) * SIF2_ ) ] 
						= (float) adapt_int32_t_.getData();
				}
			}
		}

#ifdef BALL_DEBUG
		Log.info() << "end of read" << std::endl;
#endif

	}

  /** Returns the shift corresponding to a position in the bitmap.
  */
  std::pair<double, double> Bruker2DFile::getShift(Position x, Position y)
  {
    std::pair<double, double> res;

    res.first  = soffsetf2_ - (double) x / spointnumf2_ * (double)swidthf2_ / bfreqf2_;
    res.second = soffsetf1_ - (double) y / spointnumf1_ * (double)swidthf1_ / bfreqf1_;

    return res;
  }

  /** Returns the coordinates of a point in the original data next to the given coordinates.
  */
  std::pair<Position, Position> Bruker2DFile::getPosition(double x, double y)
  {
    pair<Position, Position> res;
    res.first  = (Position) (((double)(soffsetf2_ - x)) * spointnumf2_ * bfreqf2_ / swidthf2_);
    res.second = (Position) (((double)(soffsetf1_ - y)) * spointnumf1_ * bfreqf1_ / swidthf1_);

    return res;
  }

  /** Returns a list of peaks found in the spectrum. 
			Peaks at the edge are ignored.
  */
	std::list<std::pair<int, int> > Bruker2DFile::getPeakList() const
  {
		std::list<std::pair<int, int> > retlist;

    Size xdim = (Size)parsf2_.getIntValue("SI");
    Size ydim = (Size)parsf1_.getIntValue("SI");

    for (Position y = 1; y < ydim - 1; y++) // ignore edges -> y=1..ydim-1
    {
      for (Position x = 1; x < xdim - 1; x++)
      {
				double numdum = spectrum_[x + xdim * y];
				if ((numdum > spectrum_[x-1 + (xdim*(y-1))]) &&  // upper left
					  (numdum > spectrum_[x + (xdim*(y-1))])   &&  // upper middle
					  (numdum > spectrum_[x+1 + (xdim*(y-1))]) &&  // upper right
					  (numdum > spectrum_[x-1 + (xdim*y)])     &&  // left
					  (numdum > spectrum_[x+1 + (xdim*y)])     &&  // right
					  (numdum > spectrum_[x-1 + (xdim*(y+1))]) &&  // lower left
					  (numdum > spectrum_[x + (xdim*(y+1))])   &&  // lower middle
					  (numdum > spectrum_[x+1 + (xdim*(y+1))]) )   // lower right
				{
					retlist.push_back(std::pair<int, int>(x, y));
				}
			}
		}

		return retlist;
  }

  void Bruker2DFile::setShiftRange(double offsetf1, double offsetf2, double swidthf1, double swidthf2, 
															 double bfreqf1, double bfreqf2, double spointnumf1, double spointnumf2)
  {
    soffsetf1_ = offsetf1;
    soffsetf2_ = offsetf2;
    swidthf1_ = swidthf1;
    swidthf2_ = swidthf2;
    bfreqf1_ = bfreqf1;
    bfreqf2_ = bfreqf2;
    spointnumf1_ = (Size) spointnumf1;
    spointnumf2_ = (Size) spointnumf2;
		// ?????
		/*
    spectrum_.setXSize(spointnumf2_);
    spectrum_.setYSize(spointnumf1_);
    double dum1 = getShift(0,0).first;
    double dum2 = getShift(spointnumf2_,0).first;
    spectrum_.setXLower((dum1<dum2) ? dum1 : dum2);
    spectrum_.setXUpper((dum1>dum2) ? dum1 : dum2);
    dum1 = getShift(0,0).second;
    dum2 = getShift(0, spointnumf1_).second;
    spectrum_.setYLower((dum1<dum2) ? dum1 : dum2);
    spectrum_.setYUpper((dum1>dum2) ? dum1 : dum2);
		*/
  }

}

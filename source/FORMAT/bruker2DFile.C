// $Id: bruker2DFile.C,v 1.12 2001/02/06 18:12:35 anhi Exp $

#include <BALL/FORMAT/bruker2DFile.h>

using namespace std;

namespace BALL
{
	Bruker2D::Bruker2D() 
		: File()
	{
	}

	Bruker2D::Bruker2D( const String& name, OpenMode open_mode ) 
		: File( name+"/2rr", open_mode )
	{
		parsf1_ = new JCAMPFile( name + "/proc2s" );
		parsf2_ = new JCAMPFile( name + "/procs"  );
		parsf1_->read();
		parsf2_->read();
		miny_ = (int) parsf1_->parameter( "YMIN_p" );
		maxy_ = (int) parsf1_->parameter( "YMAX_p" );
		minx_ = (int) parsf2_->parameter( "YMIN_p" );
		maxx_ = (int) parsf2_->parameter( "YMAX_p" );
	}

	Bruker2D::Bruker2D( const Bruker2D& file ) 
		: File( file )
	{
	}

	Bruker2D::~Bruker2D()
	{
		if (parsf1_)
		{
			delete parsf1_;
		}
		if (parsf2_)
		{
			delete parsf2_;
		}
	}

	void Bruker2D::read(const String& name)
	{
		parsf1_ = new JCAMPFile( name + "/proc2s" );
		parsf2_ = new JCAMPFile( name + "/procs"  );
		parsf1_->read();
		parsf2_->read();
		miny_ = (int) parsf1_->parameter( "YMIN_p" );
		maxy_ = (int) parsf1_->parameter( "YMAX_p" );
		minx_ = (int) parsf2_->parameter( "YMIN_p" );
		maxx_ = (int) parsf2_->parameter( "YMAX_p" );
		
	  close();
	  open(name+"/2rr");
	  read();
	}

	void Bruker2D::read()
	{
	  char c[4];
		signed long int &numdum = *(signed long int*) (&c[0]);
	  int actMat, actMatF1, actMatF2;
	  int matNumF1, matNumF2, f1, f2;
	  File& f = static_cast<File&> (*this);
	  int SIF1_, SIF2_, XDIMF1_, XDIMF2_;
	  bool littleEndian;
	  double a, b;

		// first we will have to find out whether we are using big or little
		// endian on this machine.
	  int endTest = 1;
	  if (*(char *) &endTest == 1)
	  {
	    littleEndian = true;
	  } 	
		else
		{
	    littleEndian = false;
	  };

	  SIF1_   = (int) parsf1_->parameter( "SI"   );
	  SIF2_   = (int) parsf2_->parameter( "SI"   );
	  XDIMF1_ = (int) parsf1_->parameter( "XDIM" );
	  XDIMF2_ = (int) parsf2_->parameter( "XDIM" );

	  // prepare the regularData
	  spectrum_.setXSize(SIF2_);
	  spectrum_.setYSize(SIF1_);
	  spectrum_.resize(SIF2_, SIF1_);

	  a = parsf2_->parameter( "OFFSET" );
	  b = parsf2_->parameter( "OFFSET" ) - (parsf2_->parameter( "SW_p" ) / parsf2_->parameter( "SF" ));
	  
	  spectrum_.setXLower((a<b) ? a : b);
	  spectrum_.setXUpper((a>b) ? a : b);

	  a = parsf1_->parameter( "OFFSET" );
	  b = parsf1_->parameter( "OFFSET" ) - (parsf1_->parameter( "SW_p" ) / parsf1_->parameter( "SF" ));

	  spectrum_.setYLower((a<b) ? a : b);
	  spectrum_.setYUpper((a>b) ? a : b);

	  spectrum_.setLowerBound(parsf1_->parameter( "YMIN_p" ));
	  spectrum_.setUpperBound(parsf1_->parameter( "YMAX_p" ));

	  // Back to the beginning of the file.
	  f.reopen( );
	  
	  matNumF2 = (int) (SIF2_ / XDIMF2_); // Number of matrices in x - direction
	  matNumF1 = (int) (SIF1_ / XDIMF1_); // Number of matrices in y - direction
	  
		for ( actMat=0; actMat < matNumF2 * matNumF1; actMat++ ) 
		{ // Walk through all submatrices
			for ( f1 = 0; f1 < XDIMF1_; f1++ ) 
			{   // for each matrix: look at every row
				for ( f2 = 0; f2 < XDIMF2_; f2++ ) 
				{ // look at every column
					if (!f.good()) 
					{
						break;
					}

					f.read(c, 4);
					if ( parsf1_->parameter( "BYTORDP" ) == 1 ) 
					{
						if (littleEndian == true) // no conversion needed;
						{
						}
						else 
						{ // conversion from little to big
							numdum = (signed long) ( ((numdum & 0x000000FFL) << 24)
									|((numdum & 0x0000FF00L) << 8)
									|((numdum & 0x00FF0000L) >> 8)
									|((numdum & 0xFF000000L) >> 24));
						}
					} 
					else 
					{
						if (littleEndian == true) // conversion from big to little
						{
							numdum = (signed long) ( ((numdum & 0x000000FFL) << 24)
									|((numdum & 0x0000FF00L) << 8)
									|((numdum & 0x00FF0000L) >> 8)
									|((numdum & 0xFF000000L) >> 24));
						} 
						else 
						{ // no conversion needed;
						}
					}

					// We need to know the number of the matrix we are looking at
					// right now.
					actMatF2 = (actMat % matNumF2); // x - coordinate of submatrix
					actMatF1 = (actMat / matNumF2); // y - coordinate of submatrix

					spectrum_[ f2 + XDIMF2_ * actMatF2 + ( ( f1 + XDIMF1_ * actMatF1 ) * SIF2_ ) ] = (float) numdum;
				}
			}
		}
	}

  /** Return a reference to the spectrum.
   */
  RegularData2D* Bruker2D::GetData()
  {
    return(&spectrum_);
  }

  /**
   * Returns the shift corresponding to a position in the bitmap.
   */
  pair<double, double> Bruker2D::GetShift(Position x, Position y)
  {
    pair<double, double> res;

    res.first = soffsetf2_ - (double) x / spointnumf2_ * (double)swidthf2_ / bfreqf2_;
    res.second = soffsetf1_ - (double) y / spointnumf1_ * (double)swidthf1_ / bfreqf1_;

    return res;
  }

  /**
   * Returns the coordinates of a point in the original data next to the given coordinates.
   */
  pair<Position, Position> Bruker2D::GetPosition(double x, double y)
  {
    pair<Position, Position> res;

    res.first = ((double)(soffsetf2_ - x)) * spointnumf2_ * bfreqf2_ / swidthf2_;
    res.second = ((double)(soffsetf1_ - y)) * spointnumf1_ * bfreqf1_ / swidthf1_;

    return res;
  }

  /** Returns a list of peaks found in the spectrum. Peaks at the edge are ignored.
   */
  list< pair<int, int> > &Bruker2D::GetPeakList()
  {
    pair<int, int> *dummy;
    Position x, y;
    Size xdim, ydim;
    double numdum;
    list< pair<int, int> > *retlist = new list< pair<int, int> >;

    xdim = (Size) parsf2_->parameter("SI");
    ydim = (Size) parsf1_->parameter("SI");

    for (y=1; y<ydim-1; y++) // ignore edges -> y=1..ydim-1
    {
      for (x=1; x<xdim-1; x++)
      {
				numdum = spectrum_[x + xdim * y];
				if (numdum > spectrum_[x-1 + (xdim*(y-1))])               // upper left
				{
					if (numdum > spectrum_[x + (xdim*(y-1))])               // upper middle
					{
						if (numdum > spectrum_[x+1 + (xdim*(y-1))])           // upper right
						{
							if (numdum > spectrum_[x-1 + (xdim*y)])             // left
							{
								if (numdum > spectrum_[x+1 + (xdim*y)])           // right
								{
									if (numdum  > spectrum_[x-1 + (xdim*(y+1))])    // lower left
									{
										if (numdum > spectrum_[x + (xdim*(y+1))])     // lower middle
										{
											if (numdum > spectrum_[x+1 + (xdim*(y+1))]) // lower right
											{
												dummy = new pair<int, int>;
												dummy->first  = x;
												dummy->second = y;
												retlist->push_back(*dummy);
											};
										};
									};
								};
							};
						};
					};
				};
			};
		};
		return (*retlist);
  }

  void Bruker2D::SetShiftRange(double offsetf1, double offsetf2, double swidthf1, double swidthf2, double bfreqf1, double bfreqf2, double spointnumf1, double spointnumf2)
  {
    soffsetf1_ = offsetf1;
    soffsetf2_ = offsetf2;
    swidthf1_ = swidthf1;
    swidthf2_ = swidthf2;
    bfreqf1_ = bfreqf1;
    bfreqf2_ = bfreqf2;
    spointnumf1_ = (Size) spointnumf1;
    spointnumf2_ = (Size) spointnumf2;

    double dum1, dum2;
    spectrum_.setXSize(spointnumf2);
    spectrum_.setYSize(spointnumf1);
    dum1 = GetShift(0,0).first;
    dum2 = GetShift(spointnumf2,0).first;
    spectrum_.setXLower((dum1<dum2) ? dum1 : dum2);
    spectrum_.setXUpper((dum1>dum2) ? dum1 : dum2);
    dum1 = GetShift(0,0).second;
    dum2 = GetShift(0, spointnumf1).second;
    spectrum_.setYLower((dum1<dum2) ? dum1 : dum2);
    spectrum_.setYUpper((dum1>dum2) ? dum1 : dum2);
  }

}

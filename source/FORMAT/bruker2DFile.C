// $Id: bruker2DFile.C,v 1.4 2000/11/10 17:17:14 anhi Exp $

#include <BALL/FORMAT/bruker2DFile.h>

namespace BALL
{
  // First I define some macros needed for the marching cube-algorithm. The names come from the number associated with
  // the different corners of the square.
  #define INTERPOL12 { \
            dummy = GetShift(act_cell_x, act_cell_y);\
            d1 = spectrum_[act_cell_x + act_cell_y*(number_of_cells_x+1)];\
            d2 = spectrum_[act_cell_x + 1 + act_cell_y*(number_of_cells_x+1)];\
            slope = (d2 - d1) / (GetShift(act_cell_x + 1, act_cell_y + 1).first - dummy.first);\
            dummy.first += (threshold - d1)/slope;\
            reslist->push_back(dummy);\
      } 

   #define INTERPOL18 { \
             dummy = GetShift(act_cell_x, act_cell_y);\
             d1 = spectrum_[act_cell_x + act_cell_y*(number_of_cells_x+1)];\
   	     d2 = spectrum_[act_cell_x + (act_cell_y+1)*(number_of_cells_x+1)];\
             slope = (d2 - d1) / (GetShift(act_cell_x, act_cell_y+1).second - dummy.second);\
             dummy.second += (threshold - d1)/slope;\
             reslist->push_back(dummy);\
       }

  #define INTERPOL24 {  \
            dummy = GetShift(act_cell_x+1, act_cell_y);\
            d1 = spectrum_[act_cell_x+1 + act_cell_y*(number_of_cells_x+1)];\
            d2 = spectrum_[act_cell_x+1 + (act_cell_y+1)*(number_of_cells_x+1)];\
            slope = (d2 - d1) / (GetShift(act_cell_x+1, act_cell_y+1).second - dummy.second);\
            dummy.second += (threshold - d1)/slope;\
            reslist->push_back(dummy);\
      }

  #define INTERPOL48 {  \
	    dummy = GetShift(act_cell_x+1, act_cell_y+1);\
            d1 = spectrum_[act_cell_x+1 + (act_cell_y+1)*(number_of_cells_x+1)];\
            d2 = spectrum_[act_cell_x   + (act_cell_y+1)*(number_of_cells_x+1)];\
            slope = (d2 - d1) / (GetShift(act_cell_x, act_cell_y+1).first - dummy.first);\
            dummy.second += (threshold - d1)/slope;\
            reslist->push_back(dummy);\
      }

  
        Bruker2D::Bruker2D() : File()
        {
	}

	Bruker2D::Bruker2D( const String& name, OpenMode open_mode ) : File( name+"/2rr", open_mode )
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

	Bruker2D::Bruker2D( const Bruker2D& file ) : File( file )
	{
	}

	Bruker2D::~Bruker2D()
	{
		if (parsf1_)
			delete parsf1_;
		if (parsf2_)
			delete parsf2_;
	}

	void Bruker2D::read()
	{
	  char c[4];
	  signed long int &numdum = *(signed long int*) (&c[0]);
	  int actMat, actMatF1, actMatF2;
	  int matNumF1, matNumF2, f1, f2;
	  File& f = static_cast<File&> (*this);
	  int SIF1_, SIF2_, XDIMF1_, XDIMF2_;
	  
	  SIF1_   = (int) parsf1_->parameter( "SI"   );
	  SIF2_   = (int) parsf2_->parameter( "SI"   );
	  XDIMF1_ = (int) parsf1_->parameter( "XDIM" );
	  XDIMF2_ = (int) parsf2_->parameter( "XDIM" );

	  // prepare the regularData
	  spectrum_.setXSize(SIF2_);
	  spectrum_.setYSize(SIF1_);
	  spectrum_.resize(SIF2_, SIF1_);

	  // Back to the beginning of the file.
	  f.reopen( );
	  
	  matNumF2 = (int) (SIF2_ / XDIMF2_); // Number of matrices in x - direction
	  matNumF1 = (int) (SIF1_ / XDIMF1_); // Number of matrices in y - direction
	  
	  for ( actMat=0; actMat < matNumF2 * matNumF1; actMat++ ) { // Walk through all submatrices
	    for ( f1 = 0; f1 < XDIMF1_; f1++ ) {   // for each matrix: look at every row
	      for ( f2 = 0; f2 < XDIMF2_; f2++ ) { // look at every column
		if (!f.good()) {
		  break;
		};
		
		f.get(c[0]); f.get(c[1]); f.get(c[2]); f.get(c[3]);
		if ( parsf1_->parameter( "BYTORDP" ) == 1 ) {
		  numdum=GINT32_FROM_LE(numdum);
		} else {
		  numdum=GINT32_FROM_BE(numdum);
		};
		
		// We need to know the number of the matrix we are looking at right now.
		actMatF2 = (actMat % matNumF2); // x - coordinate of submatrix
		actMatF1 = (actMat / matNumF2); // y - coordinate of submatrix
		
		spectrum_[ f2 + XDIMF2_ * actMatF2 + ( ( f1 + XDIMF1_ * actMatF1 ) * SIF2_ ) ] = numdum;
	      };
	    };
	  };
	}

  
  /** Read a spectrum.
   */
  void Bruker2D::read(vector<double>& dat)
  {
    dat_ = &dat;
	  
    char c[4];
    signed long int &numdum = *(signed long int*) (&c[0]);
    int actMat, actMatF1, actMatF2;
    int matNumF1, matNumF2, f1, f2;
    File& f = static_cast<File&> (*this);
    int SIF1_, SIF2_, XDIMF1_, XDIMF2_;
	  
    SIF1_   = (int) parsf1_->parameter( "SI"   );
    SIF2_   = (int) parsf2_->parameter( "SI"   );
    XDIMF1_ = (int) parsf1_->parameter( "XDIM" );
    XDIMF2_ = (int) parsf2_->parameter( "XDIM" );

    if ( dat.size() < (SIF1_ * SIF2_ ) )
      { 
	dat.resize( SIF1_ * SIF2_ );
      };
    // Zurück an den Anfang des Files.
    f.reopen( );
	  
    matNumF2 = (int) (SIF2_ / XDIMF2_); // Anzahl Matrizen in F2 - Richtung
    matNumF1 = (int) (SIF1_ / XDIMF1_); // Anzahl Matrizen in F1 - Richtung
	  
    for ( actMat=0; actMat < matNumF2 * matNumF1; actMat++ ) { // Gehe alle Submatrizen durch
      for ( f1 = 0; f1 < XDIMF1_; f1++ ) {   // in jeder Matrix: gehe alle Zeilen durch
	for ( f2 = 0; f2 < XDIMF2_; f2++ ) { // gehe alle Spalten durch
	  if (!f.good()) {
	    break;
	  };
		
	  f.get(c[0]); f.get(c[1]); f.get(c[2]); f.get(c[3]);
	  if ( parsf1_->parameter( "BYTORDP" ) == 1 ) {
	    numdum=GINT32_FROM_LE(numdum);
	  } else {
	    numdum=GINT32_FROM_BE(numdum);
	  };
		
	  // Die wievielte Matrix in der aktuellen Zeile/Spalte haben wir erreicht?
	  actMatF2 = (actMat % matNumF2);
	  actMatF1 = (actMat / matNumF2);
		
	  dat[ f2 + XDIMF2_ * actMatF2 + ( ( f1 + XDIMF1_ * actMatF1 ) * SIF2_ ) ] = numdum;
	};
      };
    };
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

  /**
   * Returns a list of endpoints of a contour-line corresponding to the value "threshold". 
   */
  list< pair<double, double> > &Bruker2D::GetContourLine(double threshold)
  {
    // This function uses a "marching cubes"-style algorithm to determine the contour-lines.
    Size number_of_cells;
    Size number_of_cells_x;
    Size number_of_cells_y;
    Position act_cell;
    Position act_cell_x;
    Position act_cell_y;
    pair<double, double> dummy;
    list< pair<double, double> > *reslist = new list< pair<double, double> >;
    double d1, d2, slope;

    number_of_cells_x = (Size) spointnumf2_-1;
    number_of_cells_y = (Size) spointnumf1_-1;
    number_of_cells   = number_of_cells_x * number_of_cells_y;

    
    for (act_cell_y = 0; act_cell_y < number_of_cells_y; act_cell_y++)
    {
      for (act_cell_x = 0; act_cell_x < number_of_cells_x; act_cell_x++)
	{
	  // First we have to find out the topology of the actual square.
	  int topology = 0;
	  
	  if (spectrum_[act_cell_x + act_cell_y*(number_of_cells_x+1)] > threshold)
	    topology |= 1;
	  if (spectrum_[act_cell_x+1 + act_cell_y*(number_of_cells_x+1)] > threshold)
	    topology |= 2;
	  if (spectrum_[act_cell_x+1 + (act_cell_y+1)*(number_of_cells_x+1)] > threshold)
	    topology |= 4;
	  if (spectrum_[act_cell_x + (act_cell_y+1)*(number_of_cells_x+1)] > threshold)
	    topology |= 8;

	  // now we can use this information to compute the contour-line.
	  switch (topology)
	  {
	    // no cut of contour-line here
	    case 0  :
	    case 15 : break;

	    // Line from upper left to lower right
       	    case 1  : 
	    case 14 : INTERPOL18
		      INTERPOL12
		      break;

	    case 4  :
	    case 11 : INTERPOL48
                      INTERPOL24
		      break;

	    // Line from upper right to lower left
       	    case 2  :
  	    case 13 : INTERPOL12
                      INTERPOL24
     		      break;

	    case 8  :
  	    case 7  : INTERPOL18
                      INTERPOL48
		      break;

	    // Line through the middle (upwards)
     	    case 9  :
	    case 6  : INTERPOL12
                      INTERPOL48
		      break;

	    // Line through the middle (left to right)
	    case 3  :
	    case 12 : INTERPOL18
                      INTERPOL24
		      break;

	    // Two lines from upper right to lower left
	    case 10 : INTERPOL18
                      INTERPOL12
                      INTERPOL48
                      INTERPOL24
		      break;

	    // Two lines from upper left to lower right
	    case 5  : INTERPOL12
                      INTERPOL24
                      INTERPOL18
                      INTERPOL48
		      break;
	  };
	}
    }
    return (*reslist);
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
    spointnumf1_ = spointnumf1;
    spointnumf2_ = spointnumf2;

    double dum1, dum2;
    spectrum_.setXSize(swidthf2);
    spectrum_.setYSize(swidthf1);
    dum1 = GetShift(0,0).first;
    dum2 = GetShift(spointnumf2,0).first;
    spectrum_.setXLower(MIN(dum1, dum2));
    spectrum_.setXUpper(MAX(dum1, dum2));
    dum1 = GetShift(0,0).second;
    dum2 = GetShift(0, spointnumf1).second;
    spectrum_.setYLower(MIN(dum1, dum2));
    spectrum_.setYUpper(MAX(dum1, dum2));
  }

}

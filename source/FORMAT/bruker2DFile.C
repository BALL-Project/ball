// $ID$

#include <BALL/FORMAT/bruker2DFile.h>

namespace BALL
{
	Bruker2D::Bruker2D( const String& name, OpenMode open_mode ) : File( name+"/2rr", open_mode )
	{
		parsf1_ = new BrukerParameter( name + "/proc2s" );
		parsf2_ = new BrukerParameter( name + "/procs"  );
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

	void Bruker2D::read( vector<double>& dat )
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

		if ( dat.size() < (SIF1_ * SIF2_ ) )
		{ 
			dat.resize( SIF1_ * SIF2_ );
		};
		// Zurück an den Anfang des Files.
		f.reopen( );

		matNumF2 = (int) (SIF1_ / XDIMF2_); // Anzahl Matrizen in F2 - Richtung
		matNumF1 = (int) (SIF1_ / XDIMF1_); // Anzahl Matrizen in F1 - Richtung

		for ( actMat=0; actMat < matNumF2 * matNumF1; actMat++ ) { // Gehe alle Submatrizen durch
			for ( f1 = 0; f1 < XDIMF1_; f1++ ) {   // in jeder Matrix: gehe alle Zeilen durch
				for ( f2 = 0; f2 < XDIMF2_; f2++ ) { // gehe alle Spalten durch
		if (!f.good()) {
			break;
		};
		
		f.get(c[0]); f.get(c[1]); f.get(c[2]); f.get(c[3]);
		if ( parsf1_->parameter( "BYTORDP" ) == 1 ) {
			numdum=GINT32_FROM_BE(numdum);
		} else {
			numdum=GINT32_FROM_LE(numdum);
		};

		// Die wievielte Matrix in der aktuellen Zeile/Spalte haben wir erreicht?
		actMatF2 = (actMat % matNumF2);
		actMatF1 = (actMat / matNumF2);

		dat[ f2 + XDIMF2_ * actMatF2 + ( ( f1 + XDIMF1_ * actMatF1 ) * SIF2_ ) ] = numdum;
				};
			};
		};
	}

	// Test!!!
	int main( int argc, char *argv[] )
	{
		Bruker2D t( "/home/andreas/projekte/nmr/hsqc/pdata/1" );

		vector<double> data;

		t.read( data );
		for (int i=0; i<data.size(); i++)
		{
			cout << data[i] << endl;
		};
	}
}

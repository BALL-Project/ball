// $Id: bruker1DFile.C,v 1.1 2000/09/14 12:23:44 oliver Exp $

#include <BALL/SYSTEM/fileSystem.h>
#include <BALL/FORMAT/Bruker1D.h>

namespace BALL 
{

	Bruker1D::Bruker1D( const String& name, OpenMode open_mode ) : File( name + FileSystem::PATH_SEPARATOR + "1r", open_mode )
	{
		pars_ = new BrukerParameter( name + FileSystem::PATH_SEPARATOR + "procs" );
		pars_->read();
		min_ = (Size) pars_->parameter( "YMIN_p" );
		max_ = (Size) pars_->parameter( "YMAX_p" );
	}

	Bruker1D::Bruker1D( const Bruker1D& file ) : File( file )
	{
	}

	Bruker1D::~Bruker1D()
	{
		if (pars_)
			delete pars_;
	}

	void Bruker1D::read( vector<double>& dat )
	{
		char c[4];
		signed long int &numdum = *(signed long int*) (&c[0]);
		Position actpos=0;
		File& f = static_cast<File&> (*this);
		
		dat.resize( pars_->parameter( "SI" ) );
		
		// Zurück an den Anfang des Files.
		f.reopen();

		// Daten einlesen.
		while ( f.good() )
		{
			f.get(c[0]); f.get(c[1]); f.get(c[2]); f.get(c[3]);
			if ( pars_->parameter( "BYTORDP" ) == 1 ) {
				numdum=GINT32_FROM_BE(numdum);
			} else {
				numdum=GINT32_FROM_LE(numdum);
			};
			
			if ((max_ - min_) != 0) {
			dat[actpos] = ((double) (numdum - min_)) / (max_ - min_);
			}
			actpos++;
		}
	}

	// Test!!!
	int main( int argc, char *argv[] )
	{
		Bruker1D t( "/home/andreas/projekte/nmr/parvulin1d/pdata/1" );

		vector<double> data;

		t.read( data );
		for (int i=0; i<data.size(); i++)
		{
			cout << data[i] << endl;
		};
	}

}

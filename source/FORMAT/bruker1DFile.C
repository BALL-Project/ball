// $Id: bruker1DFile.C,v 1.7 2000/12/18 12:18:48 anker Exp $


#include <BALL/FORMAT/bruker1DFile.h>

namespace BALL 
{

	Bruker1D::Bruker1D( const String& name, OpenMode open_mode ) 
		: File( name + FileSystem::PATH_SEPARATOR + "1r", open_mode )
	{
		pars_ = new JCAMPFile( name + FileSystem::PATH_SEPARATOR + "procs" );
		pars_->read();
		min_ = (Size) pars_->parameter( "YMIN_p" );
		max_ = (Size) pars_->parameter( "YMAX_p" );
	}

	Bruker1D::Bruker1D( const Bruker1D& file ) 
		: File( file )
	{
	}

	Bruker1D::~Bruker1D()
	{
		if (pars_)
		{
			delete pars_;
		}
	}

	void Bruker1D::read( vector<double>& dat )
	{
		int i = 0;
		char c[4];
		signed long int &numdum = *(signed long int*) (&c[0]);
		Position actpos=0;
		File& f = static_cast<File&> (*this);
		bool littleEndian;

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


		dat.resize( pars_->parameter( "SI" ) );
				
		// back to beginning of file
		f.reopen();

		// read data
		for (i=0; i < (int)pars_->parameter("SI"); i++)
		{
			if (f.good())
			{
				f.get(c[0]); f.get(c[1]); f.get(c[2]); f.get(c[3]);
				if ( pars_->parameter( "BYTORDP" ) == 1 ) 
				{
					if (littleEndian == true) // no conversion needed;
					{
					} 
					else 
					{ // conversion from little to big
						numdum = ( ((numdum & 0x000000FFL) << 24)
								|((numdum & 0x0000FF00L) << 16)
								|((numdum & 0x00FF0000L) >> 16)
								|((numdum & 0xFF000000L) >> 24));
					}
				} 
				else 
				{
					if (littleEndian == true) // conversion from big to little
					{
						numdum = ( ((numdum & 0x000000FFL) << 24)
								|((numdum & 0x0000FF00L) << 16)
								|((numdum & 0x00FF0000L) >> 16)
								|((numdum & 0xFF000000L) >> 24));
					} 
					else 
					{ // no conversion needed;
					}
				}
			
				if ((max_ - min_) != 0) 
				{
					dat[actpos] = ((double) (numdum - min_)) / (max_ - min_);
				}
				actpos++;
			}
		}
	}

  JCAMPFile* Bruker1D::getParameters()
  {
    return pars_;
  }

}

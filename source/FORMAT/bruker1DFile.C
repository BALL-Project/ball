// $Id: bruker1DFile.C,v 1.13 2001/12/30 13:28:45 sturm Exp $

#include <BALL/FORMAT/bruker1DFile.h>

namespace BALL 
{

  Bruker1D::Bruker1D()
    : File()
  {
  }

	Bruker1D::Bruker1D( const String& name, OpenMode open_mode ) 
		throw(Exception::FileNotFound)
		: File( name + FileSystem::PATH_SEPARATOR + "1r", open_mode )
	{
		pars_ = new JCAMPFile( name + FileSystem::PATH_SEPARATOR + "procs" );
		pars_->read();
		min_ = (Size) pars_->parameter( "YMIN_p" );
		max_ = (Size) pars_->parameter( "YMAX_p" );
	}

	Bruker1D::Bruker1D( const Bruker1D& file ) 
		throw(Exception::FileNotFound)
		: File( file )
	{
	}

	Bruker1D::~Bruker1D()
		throw()
	{
		if (pars_)
		{
			delete pars_;
		}
	}

	void Bruker1D::read(const String &name)
	{
	  pars_ = new JCAMPFile(name + FileSystem::PATH_SEPARATOR + "procs");
	  pars_->read();
	  min_ = (Size) pars_->parameter( "YMIN_p" );
	  max_ = (Size) pars_->parameter( "YMAX_p" );
	  
	  close();
	  open(name + FileSystem::PATH_SEPARATOR + "1r");
	  read();
	}

  void Bruker1D::read()
	{
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
	  }
	  
	  spectrum_.resize( (Size)pars_->parameter( "SI" ) );
	  spectrum_.setBoundaries(pars_->parameter("YMIN_p"), pars_->parameter("YMAX_p"));

	  // back to beginning of file
	  f.reopen();
	  
	  // read data
	  for (int i = 0; i < (int)pars_->parameter("SI"); i++)
		{
		  if (!f.good())
		  {
				// ?????: here should be a warning or exception
				return;
			}
			
		  f.get(c[0]); f.get(c[1]); f.get(c[2]); f.get(c[3]);
		  if (pars_->parameter( "BYTORDP" ) == 1) 
			{
			 	if (littleEndian == false)
			 	{ // conversion from little to big
				 	numdum = (signed long) ( ((numdum & 0x000000FFL) << 24)
					|((numdum & 0x0000FF00L) << 16)
				 	|((numdum & 0x00FF0000L) >> 16)
				 	|((numdum & 0xFF000000L) >> 24));
			 	}
				// else no conversion needed
			} 
		  else 
			{
				if (littleEndian == true) // conversion from big to little
			 	{
			    numdum = (signed long) ( ((numdum & 0x000000FFL) << 24)
					|((numdum & 0x0000FF00L) << 16)
					|((numdum & 0x00FF0000L) >> 16)
					|((numdum & 0xFF000000L) >> 24));
			   } 
			  // else no conversion needed
			}
		      
		  if ((max_ - min_) != 0) 
			{
				spectrum_[actpos] = ((double) (numdum - min_)) / (max_ - min_);
			}
		    
			actpos++;
				
		}
	}
  
  RegularData1D* Bruker1D::getData()
  {
    return (&spectrum_);
  }

  JCAMPFile* Bruker1D::getParameters()
  {
    return pars_;
  }

}

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/bruker1DFile.h>

namespace BALL 
{

  Bruker1DFile::Bruker1DFile()
    : File(),
			min_(0),
			max_(1),
			pars_()
  {
  }

	Bruker1DFile::Bruker1DFile(const String& name, OpenMode open_mode) 
		: File(name + FileSystem::PATH_SEPARATOR + "1r", open_mode),
			min_(0),
			max_(1),
			pars_()
	{
		pars_.open(name + FileSystem::PATH_SEPARATOR + "procs");
		pars_.read();
		min_ = (Size)pars_.getDoubleValue("YMIN_p");
		max_ = (Size)pars_.getDoubleValue("YMAX_p");
		pars_.close();
		read();
	}

	Bruker1DFile::~Bruker1DFile()
	{
	}

	void Bruker1DFile::read(const String &name)
	{
	  pars_.open(name + FileSystem::PATH_SEPARATOR + "procs");
	  pars_.read();
	  min_ = (Size)pars_.getDoubleValue("YMIN_p");
	  max_ = (Size)pars_.getDoubleValue("YMAX_p");
		pars_.close();
	  
	  close();
	  open(name + FileSystem::PATH_SEPARATOR + "1r");
	  read();
	}

  void Bruker1DFile::read()
	{
	  char c[4];
	  signed long int &numdum = *(signed long int*) (&c[0]);
	  Position actpos = 0;
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
	  
	  spectrum_.resize( (Size)pars_.getDoubleValue("SI"));
	  spectrum_.setOrigin(pars_.getDoubleValue("YMIN_p"));
		spectrum_.setDimension(pars_.getDoubleValue("YMAX_p") - pars_.getDoubleValue("YMIN_p"));

	  // back to beginning of file
	  f.reopen();
	  
	  // read data
	  for (Position i = 0; i < (Size)pars_.getDoubleValue("SI"); i++)
		{
		  if (!f.good())
		  {
				// ?????: here should be a warning or exception
				return;
			}
			
		  f.get(c[0]); f.get(c[1]); f.get(c[2]); f.get(c[3]);
		  if (pars_.getDoubleValue("BYTORDP") == 1.0L) 
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
}

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/bruker1DFile.h>
#include <BALL/SYSTEM/binaryFileAdaptor.h>

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
	  // first we will have to find out whether we are using big or little
	  // endian on this machine.
		unsigned int endTest = 1;
		BinaryFileAdaptor<BALL_INT32> adapt_int32_t_;
		adapt_int32_t_.setSwapEndian((*(char*)&endTest == 1) != (pars_.getDoubleValue("BYTORDP") == 1.0));

	  spectrum_.resize( (Size)pars_.getDoubleValue("SI"));
	  spectrum_.setOrigin(pars_.getDoubleValue("YMIN_p"));
		spectrum_.setDimension(pars_.getDoubleValue("YMAX_p") - pars_.getDoubleValue("YMIN_p"));

	  // back to beginning of file
	  this->reopen();
	  
	  // read data
	  for (Position i = 0; i < spectrum_.size(); ++i)
		{
		  if (!this->good())
		  {
				// ?????: here should be a warning or exception
				return;
			}

			(*this) >> adapt_int32_t_;

		  if ((max_ - min_) != 0) 
			{
				spectrum_[i] = ((double) (adapt_int32_t_.getData() - min_)) / (max_ - min_);
			}
		}
	}
}

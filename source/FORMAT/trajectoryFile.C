// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: trajectoryFile.C,v 1.12 2003/08/26 09:17:48 oliver Exp $
//

#include <BALL/FORMAT/trajectoryFile.h>

using namespace std;

namespace BALL
{

	TrajectoryFile::TrajectoryFile()
		throw()
		:	File(),
			number_of_snapshots_(0),
			number_of_atoms_(0)
	{
	}


	TrajectoryFile::TrajectoryFile(const TrajectoryFile& file)
		throw()
		:	File(file),
			number_of_snapshots_(file.number_of_snapshots_),
			number_of_atoms_(file.number_of_atoms_)
	{
	}


	TrajectoryFile::TrajectoryFile(const String& filename,
			File::OpenMode open_mode)
		throw()
		: File(filename, open_mode),
			number_of_snapshots_(0),
			number_of_atoms_(0)
	{
	}


	TrajectoryFile::~TrajectoryFile
		()
		throw()
	{
		clear();
	}


	const TrajectoryFile& TrajectoryFile::operator =
		(const TrajectoryFile& file)
		throw()
	{
		File::operator = (file);

		number_of_snapshots_ = file.number_of_snapshots_;
		number_of_atoms_ = file.number_of_atoms_;

		return *this;
	}


	void TrajectoryFile::clear()
		throw()
	{
		File::clear();

		number_of_snapshots_ = 0;
		number_of_atoms_ = 0;

	}


	bool TrajectoryFile::operator ==
		(const TrajectoryFile& file) const
		throw()
	{
		return((File::operator == (file))
				&& (number_of_snapshots_ == number_of_snapshots_)
				&& (number_of_atoms_ == file.number_of_atoms_));
	}


	bool TrajectoryFile::readHeader()
		throw()
	{
		Log.error() << "TrajectoryFile::readHeader(): " 
			<< "This method should not be called" << endl;
		return false;
	}


	bool TrajectoryFile::writeHeader()
		throw()
	{
		Log.error() << "TrajectoryFile::writeHeader(): " 
			<< "This method should not be called" << endl;
		return false;
	}


	bool TrajectoryFile::append(const SnapShot& /* snapshot */)
		throw()
	{
		Log.error() << "TrajectoryFile::append(): " 
			<< "This method should not be called" << endl;
		return false;
	}


	bool TrajectoryFile::read(SnapShot& /* snapshot */)
		throw()
	{
		Log.error() << "TrajectoryFile::read(): " 
			<< "This method should not be called" << endl;
		return false;
	}


	Size TrajectoryFile::getNumberOfSnapShots() const
		throw()
	{
		return number_of_snapshots_;
	}


	Size TrajectoryFile::getNumberOfAtoms() const
		throw()
	{
		return number_of_atoms_;
	}


	bool TrajectoryFile::flushToDisk(const std::vector<SnapShot>& /* buffer */)
		throw(File::CannotWrite)
	{
		if (!isOpen() || getOpenMode() != File::OUT)
		{
			throw (File::CannotWrite(__FILE__, __LINE__, name_));
		}
		// empty implementation
		return true;
	}
	
} // namespace BALL

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/trajectoryFile.h>

using namespace std;

namespace BALL
{

	TrajectoryFile::TrajectoryFile()
		:	File(),
			number_of_snapshots_(0),
			number_of_atoms_(0)
	{
	}

	TrajectoryFile::TrajectoryFile(const String& filename, File::OpenMode open_mode)
		: File(filename, open_mode),
			number_of_snapshots_(0),
			number_of_atoms_(0)
	{
	}

	TrajectoryFile::~TrajectoryFile()
	{
		clear();
	}

	void TrajectoryFile::clear()
	{
		File::clear();

		number_of_snapshots_ = 0;
		number_of_atoms_ = 0;

	}

	bool TrajectoryFile::operator ==
		(const TrajectoryFile& file) const
	{
		return((File::operator == (file))
				&& (number_of_snapshots_ == number_of_snapshots_)
				&& (number_of_atoms_ == file.number_of_atoms_));
	}

	bool TrajectoryFile::readHeader()
	{
		Log.error() << "TrajectoryFile::readHeader(): " 
			<< "This method should not be called" << endl;
		return false;
	}

	bool TrajectoryFile::writeHeader()
	{
		Log.error() << "TrajectoryFile::writeHeader(): " 
			<< "This method should not be called" << endl;
		return false;
	}

	bool TrajectoryFile::append(const SnapShot& /* snapshot */)
	{
		Log.error() << "TrajectoryFile::append(): " 
			<< "This method should not be called" << endl;
		return false;
	}

	bool TrajectoryFile::read(SnapShot& /* snapshot */)
	{
		Log.error() << "TrajectoryFile::read(): " 
			<< "This method should not be called" << endl;
		return false;
	}

	Size TrajectoryFile::getNumberOfSnapShots()
	{
		return number_of_snapshots_;
	}

	Size TrajectoryFile::getNumberOfAtoms() const
	{
		return number_of_atoms_;
	}

	bool TrajectoryFile::flushToDisk(const std::vector<SnapShot>& /* buffer */)
	{
		if (!isOpen() || getOpenMode() != File::MODE_OUT)
		{
			throw (File::CannotWrite(__FILE__, __LINE__, name_));
		}
		// empty implementation
		return true;
	}
	
} // namespace BALL

// $Id: trajectoryFile.C,v 1.6 2001/03/21 18:14:40 anker Exp $

#include <BALL/FORMAT/trajectoryFile.h>

using namespace std;

namespace BALL
{

	TrajectoryFile::TrajectoryFile
		()
		throw()
		:	File()
	{
	}


	TrajectoryFile::TrajectoryFile
		(const TrajectoryFile& file)
		throw()
		:	File(file)
	{
	}


	TrajectoryFile::TrajectoryFile
		(const String& filename, File::OpenMode open_mode)
		throw()
		: File(filename, open_mode)
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

		return *this;
	}


	void TrajectoryFile::clear()
		throw()
	{
		File::clear();
	}


	bool TrajectoryFile::operator ==
		(const TrajectoryFile& file) const
		throw()
	{
		return File::operator == (file);
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


	bool TrajectoryFile::flushToDisk(const ::std::vector<SnapShot> /* buffer */)
		throw()
	{
		Log.error() << "TrajectoryFile::flushToDisk(): "
			<< "This method should not be called" << endl;
		return false;
	}
	
} // namespace BALL

// $Id: trajectoryFile.C,v 1.3 2000/12/19 15:22:58 anker Exp $

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


	bool TrajectoryFile::updateHeader(const SnapShotManager& /* manager */)
		throw()
	{
		Log.error() << "TrajectoryFile::updateHeader(): " 
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


	bool TrajectoryFile::read(SnapShotManager& /* manager */)
		throw(Exception::NotImplemented)
	{
		Log.error() << "TrajectoryFile::read(): " 
			<< "This method should not be called" << endl;
		return false;
	}
	

	bool TrajectoryFile::write(const SnapShotManager& /* manager */)
		throw(Exception::NotImplemented)
	{
		Log.error() << "TrajectoryFile::write(): " 
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

} // namespace BALL

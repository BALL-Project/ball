// $Id: DCDFile.C,v 1.2 2000/12/19 13:29:01 anker Exp $

#include <BALL/FORMAT/DCDFile.h>
#include <BALL/MOLMEC/COMMON/snapShot.h>

namespace BALL
{

	DCDFile::DCDFile()
		throw()
		: TrajectoryFile(),
			header_(),
			swap_bytes_(false)
	{
		if (sizeof(int) != 4)
		{
			Log.error() << "DCDFile::DCDFile(): "
				<< "Size of int is not equal to 4 on this machine." << endl;
		}

		if (sizeof(double) != 8)
		{
			Log.error() << "DCDFile::DCDFile(): "
				<< "Size of double is not equal to 4 on this machine." << endl;
		}
	}


	DCDFile::DCDFile(const DCDFile& file)
		throw()
		:	TrajectoryFile(),
			header_(file.header_),
			swap_bytes_(file.swap_bytes_)
	{
		if (sizeof(int) != 4)
		{
			Log.error() << "DCDFile::DCDFile(): "
				<< "Size of int is not equal to 4 on this machine." << endl;
		}

		if (sizeof(double) != 8)
		{
			Log.error() << "DCDFile::DCDFile(): "
				<< "Size of double is not equal to 4 on this machine." << endl;
		}
	}


	DCDFile::DCDFile(const String& name, File::OpenMode open_mode)
		throw()
		: TrajectoryFile(name, open_mode),
			header_(),
			swap_bytes_(false)
	{
		if (sizeof(int) != 4)
		{
			Log.error() << "DCDFile::DCDFile(): "
				<< "Size of int is not equal to 4 on this machine." << endl;
		}

		if (sizeof(double) != 8)
		{
			Log.error() << "DCDFile::DCDFile(): "
				<< "Size of double is not equal to 4 on this machine." << endl;
		}
	}


	DCDFile::~DCDFile()
		throw()
	{
		clear();
	}


	const DCDFile& DCDFile::operator = (const DCDFile& file)
		throw()
	{
		TrajectoryFile::operator = (file);
		header_ = file.header_;

		return *this;
	}


	void DCDFile::clear()
		throw()
	{
		header_ = DCDHeader();
		swap_bytes_ = false;
		TrajectoryFile::clear();
	}


	bool DCDFile::operator == (const DCDFile& file)
		throw()
	{
		// BAUSTELLE: Header vergleichen. Was heißt gleich eigentlich in diesem
		// Fall?
		return (TrajectoryFile::operator == (file));
	}


	bool DCDFile::readHeader()
		throw()
	{
		char			fourbytes[4];
		char			eightyfourbytes[84];
		int				tmp_int;

		DCDHeader	tmp_header;

		File::read(fourbytes, 4);
		tmp_int = *((int*)fourbytes);
		if (tmp_int != 84)
		{
			// it might be a problem of endianness, so swap the bytes and test
			// again.
			// tmp_int = swapFourBytes(tmp_int);
			if (tmp_int != 84)
			{
				Log.error() << "DCDFile::readHeader(): "
					<< "file seems corrupted (wrong magic number)" << endl;
				return false;
			}
			else
			{
				// we have to swap bytes...
				// if (verbosity > 1)
				// {
					Log.info() << "DCDFile::readHeader(): enabling byte swap" << endl;
				// }
				swap_bytes_ = true;
			}
		}

		// read this block
		File::read(eightyfourbytes, 84);
		if (swap_bytes_)
		{
			// BAUSTELLE
		}
		else
		{
			// check for "CORD" signature
			if ((eightyfourbytes[0] != 'C') || (eightyfourbytes[1] != 'O')
				|| (eightyfourbytes[2] != 'O') || (eightyfourbytes[3] != 'D'))
			{
				Log.error() << "DCDFile::readHeader() "
					<< "File seems to be corrupted (CORD missing)" << endl;
				return false;
			}
			tmp_header.number_of_coordinate_sets = *((int*)(eightyfourbytes + 4));
			tmp_header.step_number_of_starting_time = *((int*)(eightyfourbytes + 8));
			tmp_header.steps_between_saves = *((int*)(eightyfourbytes + 12));
			tmp_header.time_step_length = *((double*)(eightyfourbytes + 40));

			// check if the block is terminated correctly
			File::read(fourbytes, 4);
			tmp_int = *((int*)fourbytes);
			if (tmp_int != 84)
			{
				Log.error() << "DCDFile::readHeader() "
					<< "File seems to be corrupted (84 bytes incorrectly terninated)"
					<< endl;
				return false;
			}

			// now handle the next block
			File::read(fourbytes, 4);
			tmp_int = *((int*)fourbytes);

			// check if the length of the next block allows n titles of 80 chars
			// length

			int number_of_titles;
			if (((tmp_int - 4) % 80) == 0)
			{
				File::read(fourbytes, 4);
				number_of_titles = *((int*)fourbytes);
				// here we believe that the number stated in the file is correct.
				for (int i = 0; i < number_of_titles; ++i)
				{
					File::read(eightyfourbytes, 80);
					// BAUSTELLE jetzt noch was mit den titeln tun...
				}
			}
			else
			{
				Log.error() << "DCDFile::readHeader() "
					<< "File seems to be corrupted (number of titles wrong)"
					<< endl;
				return false;
			}
			File::read(fourbytes, 4);
			tmp_int = *((int*)fourbytes);

			if (tmp_int != number_of_titles)
			{
				Log.error() << "DCDFile::readHeader(): "
					<< "File seems to be corrupted (title block terminated incorrectly"
					<< endl;
				return false;
			}

			// GROSSBAUSTELLE
		}

		return false;
	}


	bool DCDFile::updateHeader(const SnapShotManager& manager)
		throw()
	{
		// BAUSTELLE
		header_.number_of_coordinate_sets = manager.getNumberOfSnapShots();
		return true;
	}


	bool DCDFile::writeHeader()
		throw()
	{
		*this << BinaryFileAdaptor<struct DCDHeader>(header_);
		return true;
	}


	bool DCDFile::read(SnapShotManager& /* manager */)
		throw(Exception::NotImplemented)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
		return false;
	}


	bool DCDFile::write(const SnapShotManager& /* manager */)
		throw(Exception::NotImplemented)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
		return false;
	}


	bool DCDFile::append(const SnapShot& snapshot)
		throw()
	{
		Size noa = snapshot.getNumberOfAtoms();
		vector<Vector3> positions = snapshot.getAtomPositions();
		if (positions.size() == 0)
		{
			Log.error() << "DCDFile::append(): "
				<< "No atom positions available" << endl;
			return false;
		}

		*this << BinaryFileAdaptor<Size>(4*noa);
		for (Size atom = 0; atom < noa; ++atom)
		{
			*this << BinaryFileAdaptor<DoubleReal>(positions[atom].x);
		}
		*this << BinaryFileAdaptor<Size>(4*noa);
		*this << BinaryFileAdaptor<Size>(4*noa);
		for (Size atom = 0; atom < noa; ++atom)
		{
			*this << BinaryFileAdaptor<DoubleReal>(positions[atom].y);
		}
		*this << BinaryFileAdaptor<Size>(4*noa);
		*this << BinaryFileAdaptor<Size>(4*noa);
		for (Size atom = 0; atom < noa; ++atom)
		{
			*this << BinaryFileAdaptor<DoubleReal>(positions[atom].z);
		}
		*this << BinaryFileAdaptor<Size>(4*noa);

		return true;
	}

} // namespace BALL

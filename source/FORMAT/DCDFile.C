// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: DCDFile.C,v 1.24 2003/07/06 16:23:11 amoll Exp $

#include <BALL/FORMAT/DCDFile.h>
#include <BALL/MOLMEC/COMMON/snapShot.h>

using namespace std;

namespace BALL
{

	DCDFile::DCDFile()
		throw()
		: TrajectoryFile(),
			verbosity_(0),
			swap_bytes_(false),
			has_velocities_(false),
			charmm_extra_block_A_(false),
			charmm_extra_block_B_(false),
			CORD_("CORD"),
			step_number_of_starting_time_(0),
			steps_between_saves_(0),
			time_step_length_(0.0),
			number_of_comments_(0)
	{
		init();
	}


	DCDFile::DCDFile(const DCDFile& file)
		throw(Exception::FileNotFound)
		:	TrajectoryFile(file),
			verbosity_(0),
			swap_bytes_(file.swap_bytes_),
			has_velocities_(file.has_velocities_),
			charmm_extra_block_A_(file.charmm_extra_block_A_),
			charmm_extra_block_B_(file.charmm_extra_block_B_),
			CORD_("CORD"),
			step_number_of_starting_time_(file.step_number_of_starting_time_),
			steps_between_saves_(file.steps_between_saves_),
			time_step_length_(file.time_step_length_),
			number_of_comments_(file.number_of_comments_)
	{
		init();
	}


	DCDFile::DCDFile(const String& name, File::OpenMode open_mode)
		throw()
		: TrajectoryFile(name, open_mode),
			verbosity_(0),
			swap_bytes_(false),
			has_velocities_(false),
			charmm_extra_block_A_(false),
			charmm_extra_block_B_(false),
			CORD_("CORD"),
			step_number_of_starting_time_(0),
			steps_between_saves_(0),
			time_step_length_(0.0),
			number_of_comments_(0)
	{
		init();

		// If we want to open the file for writing, write a default header,
		// else read the header information from the existing file.
		if ((open_mode & std::ios::out) != 0)
		{
			// if this file is to be overwritten, write a default header.
			writeHeader();
		}
		else
		{
			readHeader();
		}
	}


	DCDFile::~DCDFile()
		throw()
	{
		close();
		clear();
	}


	const DCDFile& DCDFile::operator = (const DCDFile& file)
		throw()
	{
		TrajectoryFile::operator = (file);
		verbosity_ = file.verbosity_;
		swap_bytes_ = file.swap_bytes_;
		has_velocities_ = file.has_velocities_;

		return *this;
	}


	void DCDFile::clear()
		throw()
	{
		swap_bytes_ = false;
		has_velocities_ = false;
		TrajectoryFile::clear();
	}


	bool DCDFile::operator == (const DCDFile& file) const
		throw()
	{
		// ?????: Header vergleichen. Was heiﬂt gleich eigentlich in diesem Fall?
		return (TrajectoryFile::operator == (file));
	}


	bool DCDFile::isSwappingBytes() const
		throw()
	{
		return swap_bytes_;
	}


	bool DCDFile::open(const String& name, File::OpenMode open_mode)
		throw(Exception::FileNotFound)
	{
		if (!TrajectoryFile::open(name, open_mode))
		{
			return(false);
		}

		if ((open_mode & std::ios::out) != 0)
		{
			// if this file is to be overwritten, write a default header.
			return writeHeader();
		}
		
		return readHeader();
	}


	bool DCDFile::hasVelocities() const
		throw()
	{
		return has_velocities_;
	}


	bool DCDFile::readHeader()
		throw()
	{
		BinaryFileAdaptor<char> adapt_char;
		BinaryFileAdaptor<Size> adapt_Size;
		BinaryFileAdaptor<double> adapt_double;

		// read the "header" of the 84 byte block. This must contain the number
		// 84 to indicate the size of this block
		*this >> adapt_Size; 
		if (adapt_Size.getData() != 84)
		{
			swapBytes(adapt_Size.getData());
			if (adapt_Size.getData() != 84)
			{
				Log.error() << "DCDFile::readHeader(): wrong header; expected 84, got " 
										<< adapt_Size.getData() << endl;
				return false;
			}
			else
			{
				if (verbosity_ > 0)
				{
					Log.info() << "Swapping bytes." << endl;
				}
				swap_bytes_ = true;
			}
		}

		// read the distinct components of the block

		// first the CORD characters (don't need to be swapped...)
		for (Size i = 0; i < 4; ++i)
		{
			*this >> adapt_char;
			if (adapt_char.getData() != CORD_[i])
			{
				Log.error() << "DCDFile::readHeader(): "
										<< "error in CORD; expected " << CORD_[i] 
										<< ", got " << adapt_char.getData() << endl;
				return false;
			}
		}

		// read the number of coordinate sets contained in this file
		*this >> adapt_Size;
		if (swap_bytes_) swapBytes(adapt_Size.getData());
		number_of_snapshots_ = adapt_Size.getData();
		if (verbosity_ > 0)
		{
			Log.info() << "DCDFile::readHeader(): number of snapshots: " 
								 << number_of_snapshots_ << endl;
		}

		bool count_snapshots = false;

		if (number_of_snapshots_ == 0)
		{
			Log.info() << "DCDFile::readHeader(): "
				<< "Number of snapshots seems to be zero, trying to count" << endl
				<< "snapshots. Please ignore a single error message about an X header." << endl;
			count_snapshots = true;
		}

		// read the number of the first step
		*this >> adapt_Size;
		if (swap_bytes_) swapBytes(adapt_Size.getData());
		step_number_of_starting_time_ = adapt_Size.getData();

		// read the number of steps between saves
		*this >> adapt_Size;
		if (swap_bytes_) swapBytes(adapt_Size.getData());
		steps_between_saves_ = adapt_Size.getData();

		// is this really DCD or is it DC2?
		*this >> adapt_Size;
		if (swap_bytes_) swapBytes(adapt_Size.getData());
		if (adapt_Size.getData() != 0)
		{
			if (adapt_Size.getData() == 1)
			{
				if (verbosity_ > 0)
				{
					Log.info() << "DCDFile::readHeader(): DCDFile contains velocities" << endl;
				}
				has_velocities_ = true;
			}
			else
			{
				Log.error() << "This does not look like the BALL additional flag." << endl;
			}
		}

		// skip unused fields
		for (Size i = 0; i < 5; ++i)
		{
			*this >> adapt_Size;
		}

		// save position in case we are reading a CHARMm file
		Position ts_pos = tellg();

		// read the length of a time step
		*this >> adapt_double;
		if (swap_bytes_) swapBytes(adapt_double.getData());
		time_step_length_ = adapt_double.getData();
		if (verbosity_ > 0)
		{
			Log.info() << "DCDFile::readHeader(): length of a time step: " 
								 << time_step_length_ << endl;
		}

		// skip unused fields
		for (Size i = 0; i < 8; ++i)
		{
			*this >> adapt_Size;
		}

		// read the CHARMm version info, if any
		*this >> adapt_Size;
		if (swap_bytes_) swapBytes(adapt_Size.getData());
		if (adapt_Size.getData() != 0)
		{
			Log.info() << "DCDFile::readHeader(): "
				<< "encountered CHARMm version number " << adapt_Size.getData()
				<< ", rereading several fields" << endl;

			Position here = tellg();
			seekg(ts_pos);

			// read the length of a time step
			BinaryFileAdaptor<float> adapt_float;
			*this >> adapt_float;
			if (swap_bytes_) swapBytes(adapt_float.getData());
			time_step_length_ = adapt_float.getData();
			if (verbosity_ > 0)
			{
				Log.info() << "DCDFile::readHeader(): length of a time step: " 
									 << time_step_length_ << endl;
			}

			*this >> adapt_Size;
			if (adapt_Size.getData() == 1)
			{
				charmm_extra_block_A_ = true;
				if (verbosity_ > 0)
				{
					Log.info() << "DCDFile::readHeader(): CHARMm extra block A present." << endl;
				}
			}


			*this >> adapt_Size;
			if (adapt_Size.getData() == 1)
			{
				charmm_extra_block_B_ = true;
				if (verbosity_ > 0)
				{
					Log.info() << "DCDFile::readHeader(): CHARMm extra block B present." << endl;
				}
			}

			seekg(here);
		}

		// read the "footer" of the 84 byte block. This also must contain the
		// number 84 to indicate the size of this block
		*this >> adapt_Size; 
		if (swap_bytes_) swapBytes(adapt_Size.getData());
		if (adapt_Size.getData() != 84)
		{
			Log.error() << "DCDFile::readHeader(): "
				<< "wrong header; expected 84, got " << adapt_Size.getData() << endl;
			return false;
		}

		// now the comments
		// read the block "header" again. The comment block consist of an
		// integer indicating the number of 80 bytes comment blocks within this
		// block, so the block size minus 4 must be dividable by 80
		*this >> adapt_Size; 
		if (swap_bytes_) swapBytes(adapt_Size.getData());
		Size comment_size = adapt_Size.getData();
		if ((comment_size - 4) % 80 != 0)
		{
			Log.error() << "DCDFile::readHeader(): "
				<< "size of comment block (" << comment_size - 4
				<< ") is no multiple of 80" << endl;
			return false;
		}
		Size number_of_comments = (comment_size - 4) / 80;
		
		*this >> adapt_Size; 
		if (swap_bytes_) swapBytes(adapt_Size.getData());
		if (adapt_Size.getData() != number_of_comments)
		{
			Log.warn() << "DCDFile::readHeader(): "
				<< "comments block size (" << adapt_Size.getData()
				<< ") does not match number of" << endl
				<< "comments (" << number_of_comments << ")" << endl;
		}
		number_of_comments_ = number_of_comments;

		// read the comment blocks
		// ?????: for now just skip them
		for (Size i = 0; i < number_of_comments; i++)
		{
			for (Size j = 0; j < 80; j++)
			{
				*this >> adapt_char;
			}
		}

		// read the "footer" and compare it with the "header"
		*this >> adapt_Size;
		if (swap_bytes_) swapBytes(adapt_Size.getData());
		if (adapt_Size.getData() != comment_size)
		{
			Log.error() << "DCDFile::readHeader(): "
				<< "comment block footer (" << adapt_Size.getData()
				<< ") does not match header ("
				<< comment_size << ")" << endl;
			return false;
		}

		// read the block containing the number of atoms
		*this >> adapt_Size;
		if (swap_bytes_) swapBytes(adapt_Size.getData());
		if (adapt_Size.getData() != 4)
		{
			Log.error() << "DCDFile::readHeader(): "
				<< "atomnumber-block header contains wrong size " 
				<< adapt_Size.getData() << endl;
			return false;
		}

		// now read the actual number of atoms
		*this >> adapt_Size;
		if (swap_bytes_) swapBytes(adapt_Size.getData());
		number_of_atoms_ = adapt_Size.getData();

		// and check the footer
		*this >> adapt_Size;
		if (swap_bytes_) swapBytes(adapt_Size.getData());
		if (adapt_Size.getData() != 4)
		{
			Log.error() << "DCDFile::readHeader(): "
				<< "atomnumber-block footer contains wrong size " 
				<< adapt_Size.getData() << endl;
			return false;
		}


		if (count_snapshots)
		{
			SnapShot dummy;
			Size count = 0;

			Position pos = tellg();

			seekg(0, ios::end);
			int end = tellg();
			seekg(pos);

			do
			{
				if (read(dummy)) count++;
				else break;
			}
			while ((tellg() > 0) && (tellg() < end));

			seekg(pos);

			number_of_snapshots_ = count;
		}

		return true;

	}


	bool DCDFile::writeHeader()
		throw()
	{
		// DEBUG
		// Log.info() << good() << " " << bad() << endl;
		// /DEBUG
		Size i;
		if (!isAccessible() || !isOpen()) return false;

		// write the info block
		*this << BinaryFileAdaptor<Size>(84);
		for (i = 0; i < 4; ++i)
		{
			*this << BinaryFileAdaptor<char>(CORD_[i]);
		}
		// DEBUG
		// Log.info() << "wH: number_of_snapshots_ " << number_of_snapshots_ << endl;
		// /DEBUG
		*this << BinaryFileAdaptor<Size>(number_of_snapshots_);
		*this << BinaryFileAdaptor<Size>(step_number_of_starting_time_);
		*this << BinaryFileAdaptor<Size>(steps_between_saves_);
		*this << BinaryFileAdaptor<Size>((Size)has_velocities_);
		for (i = 0; i < 5; ++i)
		{
			*this << BinaryFileAdaptor<Size>(0);
		}
		*this << BinaryFileAdaptor<double>(time_step_length_);
		for (i = 0; i < 9; ++i)
		{
			*this << BinaryFileAdaptor<Size>(0);
		}
		*this << BinaryFileAdaptor<Size>(84);

		// write the comment block
		*this << BinaryFileAdaptor<Size>((number_of_comments_*80)+4);
		*this << BinaryFileAdaptor<Size>(number_of_comments_);
		for (i = 0; i < (number_of_comments_*80); ++i)
		{
			*this << BinaryFileAdaptor<char>(' ');
		}
		*this << BinaryFileAdaptor<Size>((number_of_comments_*80)+4);

		// write the atom number block
		*this << BinaryFileAdaptor<Size>(4);
		// DEBUG
		// Log.info() << "wH: number_of_atoms_ " << number_of_atoms_ << endl;
		// /DEBUG
		*this << BinaryFileAdaptor<Size>(number_of_atoms_);
		*this << BinaryFileAdaptor<Size>(4);

		return true;
	}


	bool DCDFile::append(const SnapShot& snapshot)
		throw()
	{
		// DEBUG
		// Log.info() << good() << " " << bad() << endl;
		// /DEBUG

		if (number_of_atoms_ != snapshot.getNumberOfAtoms())
		{
			if (number_of_atoms_ != 0)
			{
				Log.error() << "Differnt number of atoms in SnapShot in DCDFile:" 
										<< snapshot.getNumberOfAtoms() << std::endl;
			}
			number_of_atoms_ = snapshot.getNumberOfAtoms();
		}
		
		// DEBUG
		// Log.info() << "append: number_of_atoms_ " << number_of_atoms_ << endl;
		// /DEBUG
		
		// increase the snapshot counter for a correct header
		number_of_snapshots_++;

		const vector<Vector3>& positions = snapshot.getAtomPositions();
		if (positions.size() == 0)
		{
			Log.error() << "DCDFile::append(): No atom positions available" << endl;
			return false;
		}

		*this << BinaryFileAdaptor<Size>(4*number_of_atoms_);
		for (Size atom = 0; atom < number_of_atoms_; ++atom)
		{
			*this << BinaryFileAdaptor<float>((float) positions[atom].x);
		}
		*this << BinaryFileAdaptor<Size>(4*number_of_atoms_);
		*this << BinaryFileAdaptor<Size>(4*number_of_atoms_);
		for (Size atom = 0; atom < number_of_atoms_; ++atom)
		{
			*this << BinaryFileAdaptor<float>((float) positions[atom].y);
		}
		*this << BinaryFileAdaptor<Size>(4*number_of_atoms_);
		*this << BinaryFileAdaptor<Size>(4*number_of_atoms_);
		for (Size atom = 0; atom < number_of_atoms_; ++atom)
		{
			*this << BinaryFileAdaptor<float>((float) positions[atom].z);
		}
		*this << BinaryFileAdaptor<Size>(4*number_of_atoms_);

		if (has_velocities_)
		{
			const vector<Vector3>& velocities = snapshot.getAtomVelocities();
			if (velocities.size() == 0)
			{
				Log.error() << "DC2File::append(): No atom velocities available" << endl;
				return false;
			}

			*this << BinaryFileAdaptor<Size>(4*number_of_atoms_);
			for (Size atom = 0; atom < number_of_atoms_; ++atom)
			{
				*this << BinaryFileAdaptor<float>((float) velocities[atom].x);
			}
			*this << BinaryFileAdaptor<Size>(4*number_of_atoms_);
			*this << BinaryFileAdaptor<Size>(4*number_of_atoms_);
			for (Size atom = 0; atom < number_of_atoms_; ++atom)
			{
				*this << BinaryFileAdaptor<float>((float) velocities[atom].y);
			}
			*this << BinaryFileAdaptor<Size>(4*number_of_atoms_);
			*this << BinaryFileAdaptor<Size>(4*number_of_atoms_);
			for (Size atom = 0; atom < number_of_atoms_; ++atom)
			{
				*this << BinaryFileAdaptor<float>((float) velocities[atom].z);
			}
			*this << BinaryFileAdaptor<Size>(4*number_of_atoms_);
		}

		seekAndWriteHeader();
		return true;
	}


	bool DCDFile::read(SnapShot& snapshot)
		throw()
	{
		// DEBUG
		// Log.info() << "file position at beginning of read(): " << tellg() << endl;
		// /DEBUG

		Size tmp;

		// the number of atoms has to be read from the file header before ever
		// thinking of reading correct information

		Size expected_noa = getNumberOfAtoms();
		if (expected_noa == 0)
		{
			Log.error() << "DCDFile::read(): "
				<< "DCDFile does not contain any atoms. Did you call readHeader()?" << endl;
			return false;
		}
		snapshot.setNumberOfAtoms(expected_noa);
		Size expected_size = 4 * expected_noa;

		vector<Vector3> positions(expected_noa);
		BinaryFileAdaptor<Size> adapt_Size;
		BinaryFileAdaptor<float> adapt_float;

		// ignore the CHARMm extra block A if present
		if (charmm_extra_block_A_)
		{
			*this >> adapt_Size;
			Size count = adapt_Size.getData();
			// ?????
			// maybe we should check whether count is dividable by 4...
			for (Size i = 0; i < (count / 4); ++i) *this >> adapt_Size;
			*this >> adapt_Size;
			if (adapt_Size.getData() != count)
			{
				Log.error() << "DCDFile::read(): CHARMm extra block A corrupt." << endl;
				return false;
			}
		}

		// first read the x coordinates

		// read the block "header"...
		*this >> adapt_Size; 
		if (swap_bytes_) swapBytes(adapt_Size.getData());
		tmp = adapt_Size.getData();
		// ...and compare it to what we would expect. This value stems from the
		// information of the file header.
		if (tmp != expected_size)
		{
			Log.error() << "DCDFile::read(): "
				<< "X block header: expected " << expected_size << " but got " << tmp << endl;
			return false;
		}

		// now read the x positions
		for (Size atom = 0; atom < expected_noa; ++atom)
		{
			*this >> adapt_float; 
			if (swap_bytes_) swapBytes(adapt_float.getData());
			positions[atom].x = adapt_float.getData();
		}
		// and the block "footer"
		*this >> adapt_Size; 
		if (swap_bytes_) swapBytes(adapt_Size.getData());
		tmp = adapt_Size.getData();
		if (tmp != expected_size)
		{
			Log.error() << "DCDFile::read(): X block footer: expected " 
									<< expected_size << " but got " << tmp << endl;
			return false;
		}

		// the same proceedure for y coordinates

		// header
		*this >> adapt_Size; 
		if (swap_bytes_) swapBytes(adapt_Size.getData());
		tmp = adapt_Size.getData();
		// sanity check
		if (tmp != expected_size)
		{
			Log.error() << "DCDFile::read(): Y block header: expected " 
									<< expected_size << " but got " << tmp << endl;
			return false;
		}
		// data
		for (Size atom = 0; atom < expected_noa; ++atom)
		{
			*this >> adapt_float; 
			if (swap_bytes_) swapBytes(adapt_float.getData());
			positions[atom].y = adapt_float.getData();
		}
		// footer
		*this >> adapt_Size; 
		if (swap_bytes_) swapBytes(adapt_Size.getData());
		tmp = adapt_Size.getData();
		// sanity check
		if (tmp != expected_size)
		{
			Log.error() << "DCDFile::read(): " << "Y block footer: expected " 
									<< expected_size << " but got " << tmp << endl;
			return false;
		}

		// and z coordinates

		// header
		*this >> adapt_Size; 
		if (swap_bytes_) swapBytes(adapt_Size.getData());
		tmp = adapt_Size.getData();
		// sanity check
		if (tmp != expected_size)
		{
			Log.error() << "DCDFile::read(): Z block header: expected " 
									<< expected_size << " but got " << tmp << endl;
			return false;
		}
		// data
		for (Size atom = 0; atom < expected_noa; ++atom)
		{
			*this >> adapt_float; 
			if (swap_bytes_) swapBytes(adapt_float.getData());
			positions[atom].z = adapt_float.getData();
		}
		// footer
		*this >> adapt_Size; 
		if (swap_bytes_) swapBytes(adapt_Size.getData());
		tmp = adapt_Size.getData();
		// sanity check
		if (tmp != expected_size)
		{
			Log.error() << "DCDFile::read(): Z block footer: expected " 
									<< expected_size << " but got " << tmp << endl;
			return false;
		}

		snapshot.setAtomPositions(positions);

		if (charmm_extra_block_B_ == true)
		{
			*this >> adapt_Size;
			Size count = adapt_Size.getData();
			for (Size i = 0; i < count; ++i) *this >> adapt_Size;
			*this >> adapt_Size;
			if (adapt_Size.getData() != count)
			{
				Log.error() << "DCDFile::read(): " << "CHARMm extra block B corrupt." << endl;
				return false;
			}
		}

		if (has_velocities_)
		{
			vector<Vector3> velocities(expected_noa);

			// first read the x coordinates

			// read the block "header"...

			// ?????
			// *this >> BinaryFileAdaptor<Size>(tmp);

			*this >> adapt_Size; 
			if (swap_bytes_) swapBytes(adapt_Size.getData());
			tmp = adapt_Size.getData();
			// ...and compare it to what we would expect. This value stems from the
			// information of the file header.
			if (tmp != expected_size)
			{
				Log.error() << "DC2File::read(): X block header: expected " 
										<< expected_size << " but got " << tmp << endl;
				return false;
			}

			// now read the x velocities
			for (Size atom = 0; atom < expected_noa; ++atom)
			{
				*this >> adapt_float; 
				if (swap_bytes_) swapBytes(adapt_float.getData());
				velocities[atom].x = adapt_float.getData();
			}
			// and the block "footer"
			*this >> adapt_Size; 
			if (swap_bytes_) swapBytes(adapt_Size.getData());
			tmp = adapt_Size.getData();
			if (tmp != expected_size)
			{
				Log.error() << "DC2File::read(): X block footer: expected " 
										<< expected_size << " but got " << tmp << endl;
				return false;
			}

			// the same proceedure for y coordinates

			// header
			*this >> adapt_Size; 
			if (swap_bytes_) swapBytes(adapt_Size.getData());
			tmp = adapt_Size.getData();
			// sanity check
			if (tmp != expected_size)
			{
				Log.error() << "DC2File::read(): Y block header: expected " 
										<< expected_size << " but got " << tmp << endl;
				return false;
			}
			// data
			for (Size atom = 0; atom < expected_noa; ++atom)
			{
				*this >> adapt_float; 
				if (swap_bytes_) swapBytes(adapt_float.getData());
				velocities[atom].y = adapt_float.getData();
			}
			// footer
			*this >> adapt_Size; 
			if (swap_bytes_) swapBytes(adapt_Size.getData());
			tmp = adapt_Size.getData();
			// sanity check
			if (tmp != expected_size)
			{
				Log.error() << "DC2File::read(): Y block footer: expected " 
										<< expected_size << " but got " << tmp << endl;
				return false;
			}

			// and z coordinates

			// header
			*this >> adapt_Size; 
			if (swap_bytes_) swapBytes(adapt_Size.getData());
			tmp = adapt_Size.getData();
			// sanity check
			if (tmp != expected_size)
			{
				Log.error() << "DC2File::read(): Z block header: expected " 
										<< expected_size << " but got " << tmp << endl;
				return false;
			}
			// data
			for (Size atom = 0; atom < expected_noa; ++atom)
			{
				*this >> adapt_float; 
				if (swap_bytes_) swapBytes(adapt_float.getData());
				velocities[atom].z = adapt_float.getData();
			}
			// footer
			*this >> adapt_Size; 
			if (swap_bytes_) swapBytes(adapt_Size.getData());
			tmp = adapt_Size.getData();
			// sanity check
			if (tmp != expected_size)
			{
				Log.error() << "DC2File::read(): Z block footer: expected " 
										<< expected_size << " but got " << tmp << endl;
				return false;
			}

			snapshot.setAtomVelocities(velocities);
		}
		return true;
	}


	bool DCDFile::seekAndWriteHeader()
		throw()
	{
		Position here = tellp();
		seekp(0, ios::beg);
		bool return_value = writeHeader();
		seekp(here);
		return(return_value);
	}


	bool DCDFile::flushToDisk(const ::std::vector<SnapShot>& buffer)
		throw()
	{
		if (!isOpen())
		{
			Log.error() << "Could not flushToDisk because file is not open" << std::endl;
			return false;
		}

		if (open_mode_ != File::OUT) 
		{
			Log.error() << "Could not flushToDisk because wrong file mode" << std::endl;
			return false;
		}

		// adjust the number of snapshots for header
		number_of_snapshots_ += buffer.size();
		// ?????:
		// this is not quite the place to do this. it should be done when the
		// snapshot manager is set up or at some similar place. the question is
		// how much information has to be replicated at which place in the code.
		// we should think about something like updateHeader().
		::std::vector<SnapShot>::const_iterator it = buffer.begin();
		number_of_atoms_ = it->getNumberOfAtoms();

		// write the header
		seekp(0, ios::beg);
		if (!writeHeader()) 
		{
			Log.error() << "Could not write header in DCDFile" << std::endl;
			return false;
		}

		// adjust the number of snapshots back, because, append increases this number also
		number_of_snapshots_ -= buffer.size();

		// append the data
		seekp(0, ios::end);
		for(; it != buffer.end(); ++it)
		{
			if (!append(*it))
			{
				Log.error() << "Could not write SnapShot" << std::endl;
				return false;
			}
		}

		return true;
	}


	bool DCDFile::init()
		throw()
	{
		// DEBUG
		// verbosity_ = 1;
		// /DEBUG

		if (sizeof(Size) != 4)
		{
			Log.error() << "DCDFile::DCDFile(): " 
									<< "Size of int is not equal to 4 on this machine." << endl;
			return false;
		}

		if (sizeof(double) != 8)
		{
			Log.error() << "DCDFile::DCDFile(): "
									<< "Size of double is not equal to 4 on this machine." << endl;
			return false;
		}

		return true;
	}


	void DCDFile::enableVelocityStorage()
		throw()
	{
		has_velocities_ = true;
	}

	
	void DCDFile::disableVelocityStorage()
		throw()
	{
		has_velocities_ = false;
	}

} // namespace BALL

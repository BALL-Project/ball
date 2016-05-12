// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/DCDFile.h>
#include <BALL/MOLMEC/COMMON/snapShot.h>

#undef BALL_DEBUG

using namespace std;

namespace BALL
{

	/*
	* general structure of a DCD-file:
	* 
	* <header>
	* first snapshot: (<extra_block_A>) (<extra_block_B>) <snapshot_data>
	* [...]
	* last snapshot: (<extra_block_A>) (<extra_block_B>) <snapshot_data>
	* 
	* for a more detailed definition of <header> see: readHeader() or writeHeader()
	* 
	* all other blocks (<extra_block_A>, <extra_block_B> and <snapshot_data>)
	* are build-up in the same scheme:
	* <block_size in bytes>
	* <block_content>
	* <block_size in bytes>
	*	
	* detailed definition of <snapshot_data>:
	* <block_size in bytes> = # of atoms * 4
	* <x_coords>
	* <block_size in bytes> = # of atoms * 4
	* <block_size in bytes> = # of atoms * 4
	* <y_coords>
	* <block_size in bytes> = # of atoms * 4
	* <block_size in bytes> = # of atoms * 4
	* <z_coords>
	* <block_size in bytes> = # of atoms * 4
	* only if velocities are set:
	* <block_size in bytes> = # of atoms * 4
	* <velocities>
	* <block_size in bytes> = # of atoms * 4
	*
	*/

	DCDFile::DCDFile()
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

	DCDFile::DCDFile(const String& name, File::OpenMode open_mode)
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
		if ((open_mode & std::ios::binary) == 0)
		{
			open_mode_ = (open_mode | std::ios::binary); 
			reopen();
		}
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
	{
		close();
		clear();
	}

	void DCDFile::clear()
	{
		swap_bytes_ = false;
		has_velocities_ = false;
		TrajectoryFile::clear();
	}


	bool DCDFile::operator == (const DCDFile& file) const
	{
		// ?????: Header vergleichen. Was heiﬂt gleich eigentlich in diesem Fall?
		return (TrajectoryFile::operator == (file));
	}


	bool DCDFile::isSwappingBytes() const
	{
		return swap_bytes_;
	}


	bool DCDFile::open(const String& name, File::OpenMode open_mode)
	{
		if (!(open_mode |= std::ios::binary))
		{
			open_mode = open_mode | std::ios::binary;
		}

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
	{
		return has_velocities_;
	}


	bool DCDFile::readHeader()
	{
		/*
		* general structure of a DCD-file header:
		* 
		* it consists of 3 blocks
		*
		* each block is enclosed by its size given as a 4 byte int:
		* <size> <block_content> <size>
		* 
		* where:
		* -> size is given in bytes
		* -> block_content is given in subblocks of 4 bytes each
		*
		* 1. block (general information):
		* <size> = 54 in HEX = 84 in dezimal
		* <block_content (in subblocks) =
		*   CORD
		*   number of snapshots
		*   number of the first snapshot
		*   number of steps between saves
		*   DCD or DC2 (has velocities?)
		*   5 uninteresting subblocks (to be skipped)
		*   time between steps (as double = 2 subblocks)
		*   8 uninteresting subblocks (to be skipped)
		*   CHARMm version info>
		* <size> = 54 in HEX = 84 in dezimal
		*
		* 2. block (eventually comments):
		* each comment consists of 80 bytes
		* <size> = number of comments * 80 + 4
		* <block_content> = comments
		* <size> = number of comments * 80 + 4
		* 
		* 3. block (number of atoms):
		* <size> = 4
		* <block_content> = number of atoms
		* <size> = 4
		*
		*/
		
		current_snapshot_ = 0;

		// read the "header" of the 84 byte block. This must contain the number
		// 84 to indicate the size of this block
		*this >> adapt_size_; 
		if (adapt_size_.getData() != 84)
		{
			swapBytes(adapt_size_.getData());
			if (adapt_size_.getData() != 84)
			{
				Log.error() << "DCDFile::readHeader(): wrong header; expected 84, got " 
										<< adapt_size_.getData() << endl;
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
		BinaryFileAdaptor<char> adapt_char;
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
		number_of_snapshots_ = readSize_();
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
		step_number_of_starting_time_ = readSize_();

		// read the number of steps between saves
		steps_between_saves_ = readSize_();

		// is this really DCD or is it DC2?
		if (readSize_() != 0)
		{
			if (adapt_size_.getData() == 1)
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
			*this >> adapt_size_;
		}

		// save position in case we are reading a CHARMm file
		Position ts_pos = tellg();

		// read the length of a time step
		BinaryFileAdaptor<double> adapt_double;
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
			*this >> adapt_size_;
		}

		// read the CHARMm version info, if any
		if (readSize_() != 0)
		{
			Log.info() << "DCDFile::readHeader(): "
				<< "encountered CHARMm version number " << adapt_size_.getData()
				<< ", rereading several fields" << endl;

			Position here = tellg();
			seekg(ts_pos);

			// read the length of a time step
			time_step_length_ = readFloat_();
			if (verbosity_ > 0)
			{
				Log.info() << "DCDFile::readHeader(): length of a time step: " 
									 << time_step_length_ << endl;
			}

			*this >> adapt_size_;
			if (adapt_size_.getData() == 1)
			{
				charmm_extra_block_A_ = true;
				if (verbosity_ > 0)
				{
					Log.info() << "DCDFile::readHeader(): CHARMm extra block A present." << endl;
				}
			}


			*this >> adapt_size_;
			if (adapt_size_.getData() == 1)
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
		if (readSize_() != 84)
		{
			Log.error() << "DCDFile::readHeader(): "
				<< "wrong header; expected 84, got " << adapt_size_.getData() << endl;
			return false;
		}

		// now the comments
		// read the block "header" again. The comment block consist of an
		// integer indicating the number of 80 bytes comment blocks within this
		// block, so the block size minus 4 must be dividable by 80
		Size comment_size = readSize_();
		if ((comment_size - 4) % 80 != 0)
		{
			Log.error() << "DCDFile::readHeader(): "
				<< "size of comment block (" << comment_size - 4
				<< ") is no multiple of 80" << endl;
			return false;
		}
		Size number_of_comments = (comment_size - 4) / 80;
		
		if (readSize_() != number_of_comments)
		{
			Log.warn() << "DCDFile::readHeader(): "
				<< "comments block size (" << adapt_size_.getData()
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
		if (readSize_() != comment_size)
		{
			Log.error() << "DCDFile::readHeader(): "
				<< "comment block footer (" << adapt_size_.getData()
				<< ") does not match header ("
				<< comment_size << ")" << endl;
			return false;
		}

		// read the block containing the number of atoms
		if (readSize_() != 4)
		{
			Log.error() << "DCDFile::readHeader(): "
				<< "atomnumber-block header contains wrong size " 
				<< adapt_size_.getData() << endl;
			return false;
		}

		// now read the actual number of atoms
		number_of_atoms_ = readSize_();

		// and check the footer
		if (readSize_() != 4)
		{
			Log.error() << "DCDFile::readHeader(): "
				<< "atomnumber-block footer contains wrong size " 
				<< adapt_size_.getData() << endl;
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
	{
		/*
		* general structure of a DCD-file header:
		* 
		* it consists of 3 blocks
		*
		* each block is enclosed by its size given as a 4 byte int:
		* <size> <block_content> <size>
		* 
		* where:
		* -> size is given in bytes
		* -> block_content is given in subblocks of 4 bytes each
		*
		* 1. block (general information):
		* <size> = 54 in HEX = 84 in dezimal
		* <block_content (in subblocks) =
		*   CORD
		*   number of snapshots
		*   number of the first snapshot
		*   number of steps between saves
		*   DCD or DC2 (has velocities?)
		*   5 uninteresting subblocks (to be skipped)
		*   time between steps (as double = 2 subblocks)
		*   8 uninteresting subblocks (to be skipped)
		*   CHARMm version info>
		* <size> = 54 in HEX = 84 in dezimal
		*
		* 2. block (eventually comments):
		* each comment consists of 80 bytes
		* <size> = number of comments * 80 + 4
		* <block_content> = comments
		* <size> = number of comments * 80 + 4
		* 
		* 3. block (number of atoms):
		* <size> = 4
		* <block_content> = number of atoms
		* <size> = 4
		*
		*/

		Size i;
		if (!isAccessible() || !isOpen()) return false;

		// write the info block
		writeSize_(84);
		for (i = 0; i < 4; ++i)
		{
			*this << BinaryFileAdaptor<char>(CORD_[i]);
		}

		writeSize_(number_of_snapshots_);
		writeSize_(step_number_of_starting_time_);
		writeSize_(steps_between_saves_);
		writeSize_((Size)has_velocities_);
		for (i = 0; i < 5; ++i)
		{
			writeSize_(0);
		}
		*this << BinaryFileAdaptor<double>(time_step_length_);
		for (i = 0; i < 9; ++i)
		{
			writeSize_(0);
		}
		writeSize_(84);

		// write the comment block
		writeSize_((number_of_comments_*80)+4);
		writeSize_(number_of_comments_);
		for (i = 0; i < (number_of_comments_*80); ++i)
		{
			*this << BinaryFileAdaptor<char>(' ');
		}
		writeSize_((number_of_comments_*80)+4);

		// write the atom number block
		writeSize_(4);
		writeSize_(number_of_atoms_);
		writeSize_(4);

		return true;
	}


	bool DCDFile::append(const SnapShot& snapshot)
	{
		if (number_of_atoms_ != snapshot.getNumberOfAtoms())
		{
			if (number_of_atoms_ != 0)
			{
				Log.error() << "Differnt number of atoms in SnapShot in DCDFile:" 
										<< snapshot.getNumberOfAtoms() << std::endl;
			}
			number_of_atoms_ = snapshot.getNumberOfAtoms();
		}
		
		// increase the snapshot counter for a correct header
		number_of_snapshots_++;

		if (snapshot.getAtomPositions().size() == 0)
		{
			Log.error() << "DCDFile::append(): No atom positions available" << endl;
			return false;
		}
		writeVector_(snapshot.getAtomPositions());

		if (has_velocities_)
		{
			if (snapshot.getAtomVelocities().size() == 0)
			{
				Log.error() << "DC2File::append(): No atom velocities available" << endl;
				return false;
			}
			writeVector_(snapshot.getAtomVelocities());
		}

		return seekAndWriteHeader();
	}


	void DCDFile::writeVector_(const vector<Vector3>& v)
	{
		writeSize_(4*number_of_atoms_);
		for (Size atom = 0; atom < number_of_atoms_; ++atom)
		{
			writeFloat_(v[atom].x);
		}
		writeSize_(4*number_of_atoms_);
		writeSize_(4*number_of_atoms_);
		for (Size atom = 0; atom < number_of_atoms_; ++atom)
		{
			writeFloat_(v[atom].y);
		}
		writeSize_(4*number_of_atoms_);
		writeSize_(4*number_of_atoms_);
		for (Size atom = 0; atom < number_of_atoms_; ++atom)
		{
			writeFloat_(v[atom].z);
		}
		writeSize_(4*number_of_atoms_);
	}

	
	bool DCDFile::read(SnapShot& snapshot)
	{
		#ifdef BALL_DEBUG
			Log.info() << "file position at beginning of read(): " << tellg() << endl;
		#endif

		Size current = 0;
		if (((int(current_snapshot_) - int(number_of_snapshots_)) > 0) && (int(current_snapshot_) != int(number_of_snapshots_)))
		{
			current = current_snapshot_ - number_of_snapshots_;
			number_of_snapshots_ = int(current_snapshot_) - int(number_of_snapshots_) - 1;
			current_snapshot_ = current - 1;
		}
		else
		{
			current = current_snapshot_;
		}

		if (!good() || ((current > number_of_snapshots_) && (current == current_snapshot_))
				        || ((current_snapshot_ == number_of_snapshots_) && (number_of_snapshots_ != 0)))
			return false;

		// the number of atoms has to be read from the file header before ever
		// thinking of reading correct information
		Size expected_noa = getNumberOfAtoms();
		if (expected_noa == 0)
		{
			Log.error() << "DCDFile::read(): DCDFile does not contain any atoms. Did you call readHeader()?" << endl;
			return false;
		}
		snapshot.setNumberOfAtoms(expected_noa);

		// ignore the CHARMM extra block A if present
		if (charmm_extra_block_A_)
		{
			*this >> adapt_size_;
			Size count = adapt_size_.getData();
			// ?????
			// maybe we should check whether count is dividable by 4...
			for (Size i = 0; i < (count / 4); ++i) *this >> adapt_size_;
			*this >> adapt_size_;
			if (adapt_size_.getData() != count)
			{
				Log.error() << "DCDFile::read(): CHARMm extra block A corrupt." << endl;
				return false;
			}
		}

		// read the atoms positions
		vector<Vector3> positions(expected_noa);
		if (!readVector_(positions))
		{
			Log.error() << "Error while reading the atom positions." << std::endl;
			return false; 
		}
		snapshot.setAtomPositions(positions);


		if (charmm_extra_block_B_)
		{
			*this >> adapt_size_;
			Size count = adapt_size_.getData();
			for (Size i = 0; i < count; ++i) *this >> adapt_size_;
			*this >> adapt_size_;
			if (adapt_size_.getData() != count)
			{
				Log.error() << "DCDFile::read(): " << "CHARMm extra block B corrupt." << endl;
				return false;
			}
		}


		if (has_velocities_)
		{
			// read the atoms velocities
			vector<Vector3> velocities(expected_noa);

			if (!readVector_(velocities))
			{
				Log.error() << "Error while reading the atom velocities." << std::endl;
				return false; 
			}

			snapshot.setAtomVelocities(velocities);
		}

		current_snapshot_++;

		return true;
	}

	Size DCDFile::readSize_()
	{
		*this >> adapt_size_; 
		if (swap_bytes_) swapBytes(adapt_size_.getData());
		return adapt_size_.getData();
	}

	bool DCDFile::readSize_(Size expected_size, const String& what)
	{
		Size tmp = readSize_();
		// sanity check
		if (tmp != expected_size)
		{
			Log.error() << "DC2File::read(): " << what << " block footer: expected " 
									<< expected_size << " but got " << tmp << endl;
			return false;
		}

		return true;
	}

	float DCDFile::readFloat_()
	{
		*this >> adapt_float_; 
		if (swap_bytes_) swapBytes(adapt_float_.getData());
		return adapt_float_.getData();
	}


	bool DCDFile::readVector_(vector<Vector3>& v)
	{
		Size expected_noa = getNumberOfAtoms();
		Size expected_size = expected_noa * 4;

		// x part
		if (!readSize_(expected_size, "X")) return false;
		for (Size atom = 0; atom < expected_noa; ++atom)
		{
			v[atom].x = readFloat_();
		}
		if (!readSize_(expected_size, "X")) return false;

		// now the y part
		if (!readSize_(expected_size, "Y")) return false;
		for (Size atom = 0; atom < expected_noa; ++atom)
		{
			v[atom].y = readFloat_();
		}
		if (!readSize_(expected_size, "Y")) return false;

		// now the z part
		if (!readSize_(expected_size, "Z")) return false;
		for (Size atom = 0; atom < expected_noa; ++atom)
		{
			v[atom].z = readFloat_();
		}

		return readSize_(expected_size, "Z") != 0;
	}


	bool DCDFile::seekAndWriteHeader()
	{
		Position here = tellp();
		seekp(0, ios::beg);
		bool return_value = writeHeader();
		seekp(here);
		return(return_value);
	}


	bool DCDFile::flushToDisk(const ::std::vector<SnapShot>& buffer)
	{
		if (!isOpen() || !(getOpenMode() & File::MODE_OUT))
		{
			throw (File::CannotWrite(__FILE__, __LINE__, name_));
		}

		if (buffer.size() == 0) return true;

		// adjust the number of snapshots for header
		number_of_snapshots_ += buffer.size();
		// ?????:
		// this is not quite the place to do this. it should be done when the
		// snapshot manager is set up or at some similar place. the question is
		// how much information has to be replicated at which place in the code.
		// we should think about something like updateHeader().
		number_of_atoms_ = buffer[0].getNumberOfAtoms();

		// write the header
		seekp(0, ios::beg);
		if (!writeHeader()) 
		{
			Log.error() << "Could not write header in DCDFile" << std::endl;
			return false;
		}

		// adjust the number of snapshots back because append increases this number also
		number_of_snapshots_ -= buffer.size();

		// append the data
		seekp(0, ios::end);

		::std::vector<SnapShot>::const_iterator it = buffer.begin();
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
	{
		#ifdef BALL_DEBUG
		 verbosity_ = 1;
		#endif

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
	{
		has_velocities_ = true;
	}


	void DCDFile::disableVelocityStorage()
	{
		has_velocities_ = false;
	}

} // namespace BALL

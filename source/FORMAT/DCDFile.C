// $Id: DCDFile.C,v 1.10 2001/03/21 18:14:04 anker Exp $

#include <BALL/FORMAT/DCDFile.h>
#include <BALL/MOLMEC/COMMON/snapShot.h>

using namespace std;

namespace BALL
{

	DCDFile::DCDFile()
		throw()
		: TrajectoryFile(),
			header_(),
			swap_bytes_(false)
	{
		init();
	}


	DCDFile::DCDFile(const DCDFile& file)
		throw()
		:	TrajectoryFile(),
			header_(file.header_),
			swap_bytes_(file.swap_bytes_)
	{
		init();
	}


	DCDFile::DCDFile(const String& name, File::OpenMode open_mode)
		throw()
		: TrajectoryFile(name, open_mode),
			header_(),
			swap_bytes_(false)
	{
		init();

		// If we want to open the file for writing, we need a valid header for
		// later use by readHeader(). If we open it for reading, we have to
		// synchronize header and DCDFile by calling init().
		if ((open_mode & File::OUT) == 1)
		{
			// if this file is to be overwritten, write a default header.
			writeHeader();
		}
	}


	/*
	bool DCDFile::open(const String& name, File::OpenMode open_mode)
		throw()
	{
		TrajectoryFile::open(name, open_mode);

		// If we want to open the file for writing, we need a valid header for
		// later use by readHeader(). If we open it for reading, we have to
		// synchronize header and DCDFile by calling readHeader().
		if ((open_mode & File::OUT) == 1)
		{
			// if this file is to be overwritten, write a default header.
			writeHeader();
		}
		else
		{
			readHeader();
		}
	}
	*/


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
		// BAUSTELLE: Header vergleichen. Was heiﬂt gleich eigentlich in diesem
		// Fall?
		return (TrajectoryFile::operator == (file));
	}


	bool DCDFile::readHeader()
		throw()
	{

		BinaryFileAdaptor<char> adapt_char;
		BinaryFileAdaptor<Size> adapt_Size;
		BinaryFileAdaptor<DoubleReal> adapt_DoubleReal;

		// read the "header" of the 84 byte block. This must contain the number
		// 84 to indicate the size of this block
		*this >> adapt_Size; 
		if (adapt_Size.getData() != 84)
		{
			Log.error() << "DCDFile::readHeader(): "
				<< "wrong header; expected 84, got " << adapt_Size.getData() << endl;
			return false;
		}

		// read the distinct components of the block

		// first the CORD characters
		for (Size i = 0; i < 4; ++i)
		{
			*this >> adapt_char;
			if (adapt_char.getData() != header_.CORD[i])
			{
				Log.error() << "DCDFile::readHeader(): "
					<< "error in CORD; expected " << header_.CORD[i] 
					<< ", got " << adapt_char.getData() << endl;
				return false;
			}
		}

		// read the number of coordinate sets contained in this file
		// BAUSTELLE:
		// storing that number in this instance AND in header_ might not be too
		// clever.
		*this >> adapt_Size;
		header_.number_of_coordinate_sets = number_of_snapshots_ 
			= adapt_Size.getData();
		
		// read the number of the first step
		*this >> adapt_Size;
		header_.step_number_of_starting_time = adapt_Size.getData();

		// read the number of steps between saves
		*this >> adapt_Size;
		header_.steps_between_saves = adapt_Size.getData();

		// skip unused fields
		for (Size i = 0; i < 6; ++i)
		{
			*this >> adapt_Size;
		}

		// read the length of a time step
		*this >> adapt_DoubleReal;
		header_.time_step_length = adapt_DoubleReal.getData();

		// skip unused fields
		for (Size i = 0; i < 9; ++i)
		{
			*this >> adapt_Size;
		}

		// read the "footer" of the 84 byte block. This also must contain the
		// number 84 to indicate the size of this block
		*this >> adapt_Size; 
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
		if (adapt_Size.getData() != number_of_comments)
		{
			Log.error() << "DCDFile::readHeader(): "
				<< "comments block size (" << adapt_Size.getData()
				<< ") does not match number of comments ("
				<< number_of_comments << ")" << endl;
			return false;
		}
		header_.number_of_comments = number_of_comments;

		// read the comment blocks
		// BAUSTELLE: for now just skip them
		for (Size i = 0; i < number_of_comments; i++)
		{
			for (Size j = 0; j < 80; j++)
			{
				*this >> adapt_char;
			}
		}

		// read the "footer" and compare it with the "header"
		*this >> adapt_Size;
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
		if (adapt_Size.getData() != 4)
		{
			Log.error() << "DCDFile::readHeader(): "
				<< "atomnumber-block header contains wrong size " 
				<< adapt_Size.getData() << endl;
			return false;
		}

		// now read the actual number of atoms
		*this >> adapt_Size;
		header_.number_of_atoms = number_of_atoms_ = adapt_Size.getData();

		// and check the footer
		*this >> adapt_Size;
		if (adapt_Size.getData() != 4)
		{
			Log.error() << "DCDFile::readHeader(): "
				<< "atomnumber-block footer contains wrong size " 
				<< adapt_Size.getData() << endl;
			return false;
		}

		return true;

	}


	bool DCDFile::writeHeader()
		throw()
	{
		// the following would be nice. Unfortunately the compiler does strange
		// things when storing the struct in memory...
		// *this << BinaryFileAdaptor<struct DCDHeader>(header_);

		Size i;

		*this << BinaryFileAdaptor<Size>(header_.start_info_block);
		for (i = 0; i < 4; ++i)
		{
			*this << BinaryFileAdaptor<char>(header_.CORD[i]);
		}
		*this << BinaryFileAdaptor<Size>(header_.number_of_coordinate_sets);
		*this << BinaryFileAdaptor<Size>(header_.step_number_of_starting_time);
		*this << BinaryFileAdaptor<Size>(header_.steps_between_saves);
		for (i = 0; i < 6; ++i)
		{
			*this << BinaryFileAdaptor<Size>(header_.unused_1[i]);
		}
		*this << BinaryFileAdaptor<DoubleReal>(header_.time_step_length);
		for (i = 0; i < 9; ++i)
		{
			*this << BinaryFileAdaptor<Size>(header_.unused_2[i]);
		}
		*this << BinaryFileAdaptor<Size>(header_.end_info_block);
		*this << BinaryFileAdaptor<Size>(header_.start_title_block);
		*this << BinaryFileAdaptor<Size>(header_.number_of_comments);
		for (i = 0; i < 160; ++i)
		{
			*this << BinaryFileAdaptor<char>(header_.title[i]);
		}
		*this << BinaryFileAdaptor<Size>(header_.end_title_block);
		*this << BinaryFileAdaptor<Size>(header_.start_atomnumber_block);
		*this << BinaryFileAdaptor<Size>(header_.number_of_atoms);
		*this << BinaryFileAdaptor<Size>(header_.end_atomnumber_block);
		return true;
	}


	bool DCDFile::append(const SnapShot& snapshot)
		throw()
	{
		Size noa = snapshot.getNumberOfAtoms();
		const vector<Vector3>& positions = snapshot.getAtomPositions();
		if (positions.size() == 0)
		{
			Log.error() << "DCDFile::append(): "
				<< "No atom positions available" << endl;
			return false;
		}

		*this << BinaryFileAdaptor<Size>(4*noa);
		for (Size atom = 0; atom < noa; ++atom)
		{
			*this << BinaryFileAdaptor<Real>((Real) positions[atom].x);
		}
		*this << BinaryFileAdaptor<Size>(4*noa);
		*this << BinaryFileAdaptor<Size>(4*noa);
		for (Size atom = 0; atom < noa; ++atom)
		{
			*this << BinaryFileAdaptor<Real>((Real) positions[atom].y);
		}
		*this << BinaryFileAdaptor<Size>(4*noa);
		*this << BinaryFileAdaptor<Size>(4*noa);
		for (Size atom = 0; atom < noa; ++atom)
		{
			*this << BinaryFileAdaptor<Real>((Real) positions[atom].z);
		}
		*this << BinaryFileAdaptor<Size>(4*noa);

		return true;
	}


	bool DCDFile::read(SnapShot& snapshot)
		throw()
	{
		Size tmp;

		// the number of atoms has to be read from the file header before ever
		// thinking of reading correct information

		Size expected_noa = getNumberOfAtoms();
		if (expected_noa == 0)
		{
			Log.error() << "DCDFile::read(): "
				<< "DCDFile does not contain any atoms. Did you call readHeader()?" 
				<< endl;
			return false;
		}
		snapshot.setNumberOfAtoms(expected_noa);
		Size expected_size = 4 * expected_noa;

		vector<Vector3> positions(expected_noa);
		BinaryFileAdaptor<Size> adaptSize;
		BinaryFileAdaptor<Real> adaptReal;

		// first read the x coordinates

		// read the block "header"...

		// BAUSTELLE
		// *this >> BinaryFileAdaptor<Size>(tmp);

		*this >> adaptSize; tmp = adaptSize.getData();
		// ...and compare it to what we would expect. This value stems from the
		// information of the file header.
		if (tmp != expected_size)
		{
			Log.error() << "DCDFile::read(): "
				<< "X block header: expected " << expected_size << " but got " 
				<< tmp << endl;
			return false;
		}

		// now read the x positions
		for (Size atom = 0; atom < expected_noa; ++atom)
		{
			*this >> adaptReal; positions[atom].x = adaptReal.getData();
		}
		// and the block "footer"
		*this >> adaptSize; tmp = adaptSize.getData();
		if (tmp != expected_size)
		{
			Log.error() << "DCDFile::read(): "
				<< "X block footer: expected " << expected_size << " but got " 
				<< tmp << endl;
			return false;
		}

		// the same proceedure for y coordinates

		// header
		*this >> adaptSize; tmp = adaptSize.getData();
		// sanity check
		if (tmp != expected_size)
		{
			Log.error() << "DCDFile::read(): "
				<< "Y block header: expected " << expected_size << " but got " 
				<< tmp << endl;
			return false;
		}
		// data
		for (Size atom = 0; atom < expected_noa; ++atom)
		{
			*this >> adaptReal; positions[atom].y = adaptReal.getData();
		}
		// footer
		*this >> adaptSize; tmp = adaptSize.getData();
		// sanity check
		if (tmp != expected_size)
		{
			Log.error() << "DCDFile::read(): "
				<< "Y block footer: expected " << expected_size << " but got " 
				<< tmp << endl;
			return false;
		}

		// and z coordinates

		// header
		*this >> adaptSize; tmp = adaptSize.getData();
		// sanity check
		if (tmp != expected_size)
		{
			Log.error() << "DCDFile::read(): "
				<< "Z block header: expected " << expected_size << " but got " 
				<< tmp << endl;
			return false;
		}
		// data
		for (Size atom = 0; atom < expected_noa; ++atom)
		{
			*this >> adaptReal; positions[atom].z = adaptReal.getData();
		}
		// footer
		*this >> adaptSize; tmp = adaptSize.getData();
		// sanity check
		if (tmp != expected_size)
		{
			Log.error() << "DCDFile::read(): "
				<< "Z block footer: expected " << expected_size << " but got " 
				<< tmp << endl;
			return false;
		}

		snapshot.setAtomPositions(positions);
		return true;
	}


	bool DCDFile::flushToDisk(const ::std::vector<SnapShot> buffer)
		throw()
	{
		::std::vector<SnapShot>::const_iterator it = buffer.begin();

		// adjust the number of snapshots 
		header_.number_of_coordinate_sets += buffer.size();
		// BAUSTELLE:
		// this is not quite the place to do this. it should be done when the
		// snapshot manager is set up or at some similar place. the question is
		// how much information has to be replicated at which place in the
		// code.
		header_.number_of_atoms = it->getNumberOfAtoms();

		// write the header
		reopen(File::IN | File::OUT | File::BINARY);
		writeHeader();

		// append the data
		reopen(File::APP | File::BINARY);
		for(; it != buffer.end(); ++it)
		{
			append(*it);
		}

		// BAUSTELLE
		// no error handling/reporting at the moment
		return true;
	}


	bool DCDFile::init()
		throw()
	{
		if (sizeof(Size) != 4)
		{
			Log.error() << "DCDFile::DCDFile(): "
				<< "Size of int is not equal to 4 on this machine." << endl;
			return false;
		}

		if (sizeof(DoubleReal) != 8)
		{
			Log.error() << "DCDFile::DCDFile(): "
				<< "Size of double is not equal to 4 on this machine." << endl;
			return false;
		}
		// BAUSTELLE
		// return readHeader();
		return true;
	}


} // namespace BALL

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

namespace BALL 
{
	GenericMolFile::GenericMolFile()
		:	LineBasedFile(),
			input_is_temporary_(false),
			compress_output_(false),
			gmf_is_closed_(false)
	{
	}

	GenericMolFile::GenericMolFile(const String& filename, File::OpenMode open_mode)
		:	LineBasedFile(filename, open_mode),
			input_is_temporary_(false),
			compress_output_(false),
			gmf_is_closed_(false)
	{
	}

	GenericMolFile::~GenericMolFile()
	{
		GenericMolFile::close();
	}

	void GenericMolFile::close()
	{
		LineBasedFile::close();

		if (gmf_is_closed_) return;

		if (getOpenMode() == std::ios::in)
		{
			if (input_is_temporary_)
			{
				File::remove(name_);
			}
		}
		else if(compress_output_)
		{
			std::fstream::close();
			std::ifstream unzipped_file(name_.c_str(), ios_base::in);
			boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
			in.push(boost::iostreams::gzip_compressor());
			in.push(unzipped_file);
			std::ofstream zipped_file(zipped_filename_.c_str(), std::ios::out | std::ios_base::binary);
			boost::iostreams::copy(in, zipped_file);
			File::remove(name_);
		}

		gmf_is_closed_ = true;
	}

	void GenericMolFile::defineInputAsTemporary(bool b)
	{
		input_is_temporary_ = b;
	}

	void GenericMolFile::enableOutputCompression(String zipped_filename)
	{
		compress_output_ = true;
		zipped_filename_ = zipped_filename;
	}

	bool GenericMolFile::isCompressedFile()
	{
		if (getOpenMode() == std::ios::in)
		{
			return input_is_temporary_;
		}
		else
		{
			return compress_output_;
		}
	}

	const GenericMolFile& GenericMolFile::operator = (const GenericMolFile& rhs)
	{
		LineBasedFile::operator = (rhs);
		return *this;
	}

	bool GenericMolFile::read(System& system)
	{
		if (!isOpen())
		{
			return false;
		}

		initRead_();

		bool read_anything = false;
		Molecule* molecule = 0;
		while ((molecule = read()) != 0)
		{
			system.append(*molecule);
			read_anything = true;
		}

		return read_anything;
	}

	Molecule* GenericMolFile::read()
	{
		return 0;
	}

	bool GenericMolFile::write(const Molecule& /* molecule */)
	{
		if (!isOpen() || getOpenMode() != std::ios::out)
		{
			throw (File::CannotWrite(__FILE__, __LINE__, name_));
		}
		return true;
	}

	bool GenericMolFile::write(const System& system)
	{
		if (!isOpen() || getOpenMode() != std::ios::out)
		{
			throw (File::CannotWrite(__FILE__, __LINE__, name_));
		}

		initWrite_();
		MoleculeConstIterator molecule = system.beginMolecule();
		for (; +molecule; ++molecule)
		{
			if (!write(*molecule)) return false;
		}
		return true;
	}

	GenericMolFile& GenericMolFile::operator >> (System& system)
	{
		read(system);
		return *this;
	}
 
	GenericMolFile& GenericMolFile::operator << (const System& system)
	{
		write(system);
		return *this;
	}

	GenericMolFile& GenericMolFile::operator >> (Molecule& molecule)
	{
		molecule.clear();
		Molecule* new_mol = read();
		if (new_mol != 0)
		{
			molecule = *new_mol; // copy the Molecule
		}
		
		delete new_mol;
		return *this;
	}
 
	GenericMolFile& GenericMolFile::operator << (const Molecule& molecule)
	{
		write(molecule);
		return *this;
	}

	void GenericMolFile::initRead_()
	{
	}

	void GenericMolFile::initWrite_()
	{
	}

} // namespace BALL

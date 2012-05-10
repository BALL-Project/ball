#include <BALL/FORMAT/molFileFactory.h>

#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/antechamberFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/MOLFile.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/FORMAT/XYZFile.h>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include <BALL/DATATYPE/string.h>

namespace BALL
{

	GenericMolFile* MolFileFactory::open(const String& name, File::OpenMode open_mode)
	{
		String tmp = name;
		bool compression = false;
		String filename = name;
		String zipped_filename = "";

		if (tmp.hasSuffix(".gz"))
		{
			compression = true;
		}
		else if (open_mode == ios::in && !isFileExtensionSupported(filename)) // check whether file is zipped
		{
			ifstream zipped_file(filename.c_str(), ios_base::in | ios_base::binary);
			boost::iostreams::filtering_istream in;
			in.push(boost::iostreams::gzip_decompressor());
			in.push(zipped_file);
			bool ok = false;
			try
			{
				string s;
				ok = std::getline(in,s);
			}
			catch (const boost::iostreams::gzip_error& e)
			{
				compression = false;
			}
			if (ok) compression = true;
		}

		if (compression)
		{
			compression = true;
			zipped_filename = tmp;
			String unzipped_filename;
			if (tmp.hasSuffix(".gz"))
			{
				tmp = tmp.before(".gz");
				String ext = tmp.substr(tmp.find_last_of("."));
				File::createTemporaryFilename(unzipped_filename,ext);
			}
			else // unknown extension
			{
				File::createTemporaryFilename(unzipped_filename);
			}

			if (open_mode == ios::in)
			{
				ifstream zipped_file(zipped_filename.c_str(), ios_base::in | ios_base::binary);
				boost::iostreams::filtering_istream in;
				in.push(boost::iostreams::gzip_decompressor());
				in.push(zipped_file);
				ofstream unzipped_file(unzipped_filename.c_str());
				boost::iostreams::copy(in, unzipped_file);
			}

			filename = unzipped_filename;
			tmp = unzipped_filename.right(5);
		}
		else
		{
			tmp = filename.right(5);
		}
		tmp.toLower(0, 5);

		GenericMolFile* gmf = 0;
		if (tmp.hasSuffix(".ac"))
		{
			gmf = new AntechamberFile(filename, open_mode);
		}
		else if(tmp.hasSuffix(".pdb") || tmp.hasSuffix(".ent") || tmp.hasSuffix(".brk"))
		{
			gmf = new PDBFile(filename, open_mode);
		}
		else if(tmp.hasSuffix(".hin"))
		{
			gmf = new HINFile(filename, open_mode);
		}
		else if(tmp.hasSuffix(".mol"))
		{
			gmf = new MOLFile(filename, open_mode);
		}
		else if(tmp.hasSuffix(".sdf"))
		{
			gmf = new SDFile(filename, open_mode);
		}
		else if(tmp.hasSuffix(".mol2"))
		{
			gmf = new MOL2File(filename, open_mode);
		}
		else if(tmp.hasSuffix(".xyz"))
		{
			gmf = new XYZFile(filename, open_mode);
		}
		/*
		else if(tmp.hasSuffix(".drf"))
		{
			gmf = new Docking::DockResultFile(filename, open_mode);
		}
		*/
		else
		{
			if (open_mode == ios::in)
			{
				gmf = detectFormat(filename);
				// Make sure that temporary input-file is deleted when GenericMolFile is closed.
				if (compression)
				{
					gmf->defineInputAsTemporary();
				}
				return gmf;
			}
			return NULL;
		}

		if (compression)
		{
			if (open_mode == ios::in)
			{
				// Make sure that temporary input-file is deleted when GenericMolFile is closed.
				gmf->defineInputAsTemporary();
			}
			else
			{
				// Make sure that temporary output-file is compressed and then deleted when GenericMolFile is closed.
				gmf->enableOutputCompression(zipped_filename);
			}
		}
		return gmf;

		/*
		String tmp = name.right(5);
		tmp.toLower(0, 5);
		
		if (tmp.hasSuffix(".ac")) {
			return new AntechamberFile(name, open_mode);
		} else if(tmp.hasSuffix(".pdb") || tmp.hasSuffix(".ent") || tmp.hasSuffix(".brk")) {
			return new PDBFile(name, open_mode);
		} else if(tmp.hasSuffix(".hin")) {
			return new HINFile(name, open_mode);
		} else if(tmp.hasSuffix(".mol")) {
			return new MOLFile(name, open_mode);
		} else if(tmp.hasSuffix(".sdf")) {
			return new SDFile(name, open_mode);
		} else if(tmp.hasSuffix(".mol2")) {
			return new MOL2File(name, open_mode);
		} else if(tmp.hasSuffix(".xyz")) {
			return new XYZFile(name, open_mode);
		} else {
			return NULL;
		}
		*/
	}

	bool MolFileFactory::isFileExtensionSupported(String filename)
	{
		vector<String> exts;
		String s = getSupportedFormats();
		s.split(exts,",");
		for (Size i=0; i<exts.size(); i++)
		{
			if (filename.hasSuffix(exts[i]))
			{
				return true;
			}
		}
		return false;
	}

	GenericMolFile* MolFileFactory::open(const String& name, File::OpenMode open_mode, String default_format)
	{
		GenericMolFile* file = open(name, open_mode);
		if (file)
		{
			return file;
		}

		if (open_mode == ios::out)
		{
			bool compression = false;
			String filename = name;
			String zipped_filename = "";
			if (default_format.hasSuffix(".gz"))
			{
				compression = true;
				zipped_filename = filename;
				default_format = default_format.before(".gz");
				String unzipped_filename;
				File::createTemporaryFilename(unzipped_filename, default_format);
				filename = unzipped_filename;
			}

			if (default_format == "ac")
			{
				file = new AntechamberFile(filename, open_mode);
			}
			else if(default_format == "pdb" || default_format == "ent" || default_format == "brk")
			{
				file = new PDBFile(filename, open_mode);
			}
			else if(default_format == "hin")
			{
				file = new HINFile(filename, open_mode);
			}
			else if(default_format == "mol")
			{
				file = new MOLFile(filename, open_mode);
			}
			else if(default_format == "sdf")
			{
				file = new SDFile(filename, open_mode);
			}
			else if(default_format == "mol2")
			{
				file = new MOL2File(filename, open_mode);
			}
			else if(default_format == "xyz")
			{
				file = new XYZFile(filename, open_mode);
			}
			/*
			else if(default_format == "drf")
			{
				file = new Docking::DockResultFile(filename, open_mode);
			}
			*/
			// Make sure that temporary output-file is compressed and then deleted when GenericMolFile is closed.
			if (compression)
			{
				file->enableOutputCompression(zipped_filename);
			}
		}
		return file;
	}

	GenericMolFile* MolFileFactory::open(const String& name, File::OpenMode open_mode, GenericMolFile* default_format_file)
	{
		GenericMolFile* file = open(name, open_mode);
		if (file)
		{
			return file;
		}

		if (open_mode == ios::out)
		{
			bool compression = false;
			String filename = name;
			String zipped_filename = "";
			if (default_format_file->isCompressedFile())
			{
				compression = true;
				zipped_filename = filename;
				String unzipped_filename;
				File::createTemporaryFilename(unzipped_filename);
				filename = unzipped_filename;
			}

			if (dynamic_cast<AntechamberFile*>(default_format_file))
			{
				file = new AntechamberFile(filename, open_mode);
			}
			else if(dynamic_cast<PDBFile*>(default_format_file))
			{
				file = new PDBFile(filename, open_mode);
			}
			else if(dynamic_cast<HINFile*>(default_format_file))
			{
				file = new HINFile(filename, open_mode);
			}
			else if(dynamic_cast<SDFile*>(default_format_file))
			{
				file = new SDFile(filename, open_mode);
			}
			else if(dynamic_cast<MOL2File*>(default_format_file))
			{
				file = new MOL2File(filename, open_mode);
			}
			else if(dynamic_cast<MOLFile*>(default_format_file))
			{
				file = new MOLFile(filename, open_mode);
			}
			else if(dynamic_cast<XYZFile*>(default_format_file))
			{
				file = new XYZFile(filename, open_mode);
			}
			/*
			else if(dynamic_cast<Docking::DockResultFile*>(default_format_file))
			{
				file = new Docking::DockResultFile(filename, open_mode);
			}
			*/
			// Make sure that temporary output-file is compressed and then deleted when GenericMolFile is closed.
			if (compression)
			{
				file->enableOutputCompression(zipped_filename);
			}
		}
		return file;
	}

	String MolFileFactory::getSupportedFormats()
	{
		String formats = "mol2, sdf, drf, pdb, ac, ent, brk, hin, mol, xyz, mol2.gz, sdf.gz, drf.gz, pdb.gz, ac.gz, ent.gz, brk.gz, hin.gz, mol.gz, xyz.gz";
		return formats;
	}

	GenericMolFile* MolFileFactory::detectFormat(const String& name)
	{
		LineBasedFile input(name, ios::in);
		bool empty_file = true;

		// Read at most 400 lines.
		// If no format-indicator is found, return NULL
		for (Size no=1; no < 400 && input.readLine(); no++)
		{
			String& line = input.getLine();
			if (!line.trim().empty())
			{
				empty_file = false;
			}
			if (line.hasPrefix("@<TRIPOS>MOLECULE"))
			{
				return new MOL2File(name, ios::in);
			}
			else if (line.hasPrefix("$$$$") || line.hasPrefix("M  END"))
			{
				return new SDFile(name, ios::in);
			}
			/*
			else if (line.hasPrefix("<dockingfile>"))
			{
				return new Docking::DockResultFile(name, ios::in);
			}
			*/
			else if (line.hasPrefix("HEADER") || line.hasPrefix("ATOM") || line.hasPrefix("USER"))
			{
				return new PDBFile(name, ios::in);
			}
		}

		if (empty_file)
		{
			Log.error() << endl << "[Error:] Specified input file is empty!" << endl << endl;
		}
		else
		{
			Log.error() << endl << "[Error:] Specified input file has unknown extension and its format could not be detected automatically!" << endl << endl;
		}
		return NULL;
	}
}


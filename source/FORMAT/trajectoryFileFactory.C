// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/trajectoryFileFactory.h>

#include <BALL/FORMAT/trajectoryFile.h>
#include <BALL/FORMAT/DCDFile.h>
#include <BALL/FORMAT/TRRFile.h>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include <BALL/DATATYPE/string.h>

// TODO: - this shares a lot of code with MolFileFactory. We should probably put the shared stuff into a base class.
//       - also, we currently do not handle automatic compression/decompression into a temporary. I am not even sure
//         yet if I like the solution used in MolFileFactory
namespace BALL
{
	TrajectoryFile* TrajectoryFileFactory::open(const String& name, File::OpenMode open_mode)
	{
		String tmp = name;
		tmp.toLower();

		TrajectoryFile* tf = 0;
		if (tmp.hasSuffix(".dcd"))
		{
			tf = new DCDFile(name, open_mode);
		}
		else if(tmp.hasSuffix(".trr"))
		{
			tf = new TRRFile(name, open_mode);
		}
		else
		{
			if (open_mode == std::ios::in)
			{
				tf = detectFormat(name);

				return tf;
			}
			return NULL;
		}

		return tf;
	}

	TrajectoryFile* TrajectoryFileFactory::open(const String& name, File::OpenMode open_mode, String default_format)
	{
		TrajectoryFile* file = open(name, open_mode);

		if (file)
		{
			return file;
		}

		if (open_mode == std::ios::out)
		{
			if (default_format == "dcd")
			{
				file = new DCDFile(name, open_mode);
			}
			else if(default_format == "trr")
			{
				file = new TRRFile(name, open_mode);
			}
		}

		return file;
	}

	TrajectoryFile* TrajectoryFileFactory::open(const String& name, File::OpenMode open_mode, TrajectoryFile* default_format_file)
	{
		TrajectoryFile* file = open(name, open_mode);

		if (file)
		{
			return file;
		}

		if (dynamic_cast<DCDFile*>(default_format_file))
		{
			file = new DCDFile(name, open_mode);
		}
		else if(dynamic_cast<TRRFile*>(default_format_file))
		{
			file = new TRRFile(name, open_mode);
		}

		return file;
	}

	String TrajectoryFileFactory::getSupportedFormats()
	{
		String formats = "dcd, trr";

		return formats;
	}

	TrajectoryFile* TrajectoryFileFactory::detectFormat(const String& name)
	{
		Log.error() << std::endl << "[Error:] Format detection not yet implemented for trajectory files!" << std::endl << std::endl;

		return NULL;
	}

	bool TrajectoryFileFactory::isFileExtensionSupported(String filename)
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

}

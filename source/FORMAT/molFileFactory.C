#include <BALL/FORMAT/molFileFactory.h>

#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/antechamberFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/MOLFile.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/FORMAT/XYZFile.h>
#include <BALL/FORMAT/dockResultFile.h>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

#include <BALL/DATATYPE/string.h>

namespace BALL
{


  String MolFileFactory::getSupportedFormats()
  {
    String formats = "mol2,sdf,drf,pdb,ac,ent,brk,hin,mol,xyz,mol2.gz,sdf.gz,drf.gz,pdb.gz,ac.gz,ent.gz,brk.gz,hin.gz,mol.gz,xyz.gz";
    return formats;
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

  GenericMolFile* MolFileFactory::open(const String& name, File::OpenMode open_mode)
  {
    bool compression = false;
    String filename = name;

    if (name.hasSuffix(".gz"))
    {
      compression = true;
    }
    else if (open_mode == std::ios::in && !isFileExtensionSupported(filename)) // check whether file is zipped
    {
      std::ifstream zipped_file(filename.c_str(), std::ios_base::in | std::ios_base::binary);
      boost::iostreams::filtering_istream in;
      in.push(boost::iostreams::gzip_decompressor());
      in.push(zipped_file);
      bool ok = false;
      try
      {
        string s;
        ok = bool(std::getline(in,s));
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
      String unzipped_filename;
      if (name.hasSuffix(".gz"))
      {
        String tmp = name.before(".gz");
        String ext = tmp.substr(tmp.find_last_of("."));
        File::createTemporaryFilename(unzipped_filename, ext);
      }
      else // unknown extension
      {
        File::createTemporaryFilename(unzipped_filename);
      }

      if (open_mode == std::ios::in)
      {
        std::ifstream zipped_file(name.c_str(), std::ios_base::in | std::ios_base::binary);
        boost::iostreams::filtering_istream in;
        in.push(boost::iostreams::gzip_decompressor());
        in.push(zipped_file);
        std::ofstream unzipped_file(unzipped_filename.c_str());
        boost::iostreams::copy(in, unzipped_file);
      }

      filename = unzipped_filename;
    }
    
    GenericMolFile* gmf = 0;
    if (filename.hasSuffix(".ac") || filename.hasSuffix(".AC"))
    {
      gmf = new AntechamberFile(filename, open_mode);
    }
    else if(filename.hasSuffix(".pdb") || filename.hasSuffix(".ent") || filename.hasSuffix(".brk") ||
      filename.hasSuffix(".PDB") || filename.hasSuffix(".ENT") || filename.hasSuffix(".BRK"))
    {
      gmf = new PDBFile(filename, open_mode);
    }
    else if(filename.hasSuffix(".hin") || filename.hasSuffix(".HIN"))
    {
      gmf = new HINFile(filename, open_mode);
    }
    else if(filename.hasSuffix(".mol") || filename.hasSuffix(".MOL"))
    {
      gmf = new MOLFile(filename, open_mode);
    }
    else if(filename.hasSuffix(".sdf") || filename.hasSuffix(".SDF"))
    {
      gmf = new SDFile(filename, open_mode);
    }
    else if(filename.hasSuffix(".mol2") || filename.hasSuffix(".MOL2"))
    {
      gmf = new MOL2File(filename, open_mode);
    }
    else if(filename.hasSuffix(".xyz") || filename.hasSuffix(".XYZ"))
    {
      gmf = new XYZFile(filename, open_mode);
    }
    else if(filename.hasSuffix(".drf") || filename.hasSuffix(".DRF"))
    {
      gmf = new DockResultFile(filename, open_mode);
    }
    else
    {
      if (open_mode == std::ios::in)
      {
        gmf = detectFormat(filename);
        // Make sure that temporary input-file is deleted when GenericMolFile is closed.
        if (gmf && compression)
        {
          gmf->defineInputAsTemporary();
        }
        return gmf;
      }
      return NULL;
    }

    if (compression)
    {
      if (open_mode == std::ios::in)
      {
        // Make sure that temporary input-file is deleted when GenericMolFile is closed.
        gmf->defineInputAsTemporary();
      }
      else
      {
        // Make sure that temporary output-file is compressed and then deleted when GenericMolFile is closed.
        gmf->enableOutputCompression(name);
      }
    }
    
    return gmf;
  }

  GenericMolFile* MolFileFactory::open(const String& name, File::OpenMode open_mode, String default_format, bool forced)
  {
    //If we do not force the default format, we start as in others
    //else skip to avoid unnecessary computation
    if(!forced)
    {
      //try to read/write using filename with suffix (try recognise if necessary)
      GenericMolFile* file = open(name, open_mode);
      if (file)
      {
        return file;
      }
    }
    
    //if forced or if filetype could not be recognised by ending use default format
    GenericMolFile* file = 0;

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
      if (open_mode == std::ios::in)
        {
          std::ifstream zipped_file(zipped_filename.c_str(), std::ios_base::in | std::ios_base::binary);
          boost::iostreams::filtering_istream in;
          in.push(boost::iostreams::gzip_decompressor());
          in.push(zipped_file);
          std::ofstream unzipped_file(unzipped_filename.c_str());
          boost::iostreams::copy(in, unzipped_file);
        }
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
    else if(default_format == "drf")
    {
      file = new DockResultFile(filename, open_mode);
    }


    if (compression)
    {
      if (open_mode == std::ios::in)
      {
        // Make sure that temporary input-file is deleted when GenericMolFile is closed.
        file->defineInputAsTemporary();
      }
      else
      {
        // Make sure that temporary output-file is compressed and then deleted when GenericMolFile is closed.
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

    if (open_mode == std::ios::out)
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
      else if(dynamic_cast<DockResultFile*>(default_format_file))
      {
        file = new DockResultFile(filename, open_mode);
      }
      // Make sure that temporary output-file is compressed and then deleted when GenericMolFile is closed.
      if (compression)
      {
        file->enableOutputCompression(zipped_filename);
      }
    }
    return file;
  }

  GenericMolFile* MolFileFactory::detectFormat(const String& name)
  {
    LineBasedFile input(name, std::ios::in);
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
        input.close();
        return new MOL2File(name, std::ios::in);
      }
      else if (line.hasPrefix("$$$$") || line.hasPrefix("M  END"))
      {
        input.close();
        return new SDFile(name, std::ios::in);
      }
      else if (line.hasPrefix("<dockingfile>"))
      {
        input.close();
        return new DockResultFile(name, std::ios::in);
      }
      else if (line.hasPrefix("HEADER") || line.hasPrefix("ATOM") || line.hasPrefix("USER"))
      {
        input.close();
        return new PDBFile(name, std::ios::in);
      }
    }

    if (empty_file)
    {
      Log.error() << std::endl << "[Error:] Specified input file is empty!" << std::endl << std::endl;
    }
    else
    {
      Log.error() << std::endl << "[Error:] Specified input file has unknown extension and its format could not be detected automatically!" << std::endl << std::endl;
    }
    
    input.close();
    
    return NULL;
  }
}


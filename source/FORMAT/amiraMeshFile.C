// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: amiraMeshFile.C
//

#include <BALL/FORMAT/amiraMeshFile.h>
#include <cstdio>


namespace BALL
{
	AmiraMeshFile::AmiraMeshFile()
		: File(),
			binary_(false),
			idx_start_data_(0),
			num_components_(0)
	{
	}
	
	AmiraMeshFile::AmiraMeshFile(const String& name, File::OpenMode open_mode)
		: File(name, open_mode),
			binary_(false),
			idx_start_data_(0),
			num_components_(0)
	{
	}
	
	AmiraMeshFile::~AmiraMeshFile()
	{
		close();
		clear();
	}
	
	void AmiraMeshFile::clear()
	{
		File::clear();
		binary_= false;
		idx_start_data_ = 0;
		num_components_ = 0;
	}
	
	bool AmiraMeshFile::operator == (const AmiraMeshFile& file) const
	{
		return (   (File::operator == (file))
						 &&(binary_    == file.binary_));
	}
	
	bool AmiraMeshFile::open(const String& name, File::OpenMode open_mode)
	{
		if (!(open_mode |= std::ios::binary))
		{
			open_mode = open_mode | std::ios::binary;
		}

		if (!File::open(name, open_mode))
		{
			return(false);
		}

		return true;
	}
	
	bool AmiraMeshFile::readHeader()
	{
    const size_t MAX_READ_SIZE = 2048;
		//We read the first 2k bytes into memory to parse the header.
    //The fixed buffer size looks a bit like a hack, and it is one, but it gets the job done.
    char buffer[MAX_READ_SIZE + 1];

    std::fstream::read(buffer, MAX_READ_SIZE);
    buffer[std::fstream::gcount()] = '\0'; //The following string routines prefer null-terminated strings

    if (strstr(buffer, "# AmiraMesh BINARY-LITTLE-ENDIAN 2.1"))
    {
        binary_ = true;
    }
    else if (strstr(buffer, "# AmiraMesh ASCII 1.0"))
    {
        binary_ = false;
    }
    else
    {
        return false;
    }
    
		sscanf(findAndJump_(buffer, "define Lattice"), "%g %g %g", &extent_.x, &extent_.y, &extent_.z);
	
		
    sscanf(findAndJump_(buffer, "BoundingBox"), "%g %g %g %g %g %g", &min_.x, &max_.x,
																																		&min_.y, &max_.y,
																																		&min_.z, &max_.z);
		

    //Is it a uniform grid? We need this only for the sanity check below.
    const bool bIsUniform = (strstr(buffer, "CoordType \"uniform\"") != NULL);

    //Type of the field: scalar, vector
    num_components_ = 0;
    if (strstr(buffer, "Lattice { float Data }"))
    {
        //Scalar field
        num_components_ = 1;
    }
    else
    {
        //A field with more than one component, i.e., a vector field
        sscanf(findAndJump_(buffer, "Lattice { float["), "%u", &num_components_);

				
        Log.error() << "Sorry, currently, we do not support Amira files containing" << std::endl;
        Log.error() << "multiple component fields (e.g. vector fields)\n" << std::endl;
				return false;
    }

    //Sanity check
    if (extent_.x <= 0 || extent_.y <= 0 || extent_.z <= 0
        || min_.x > max_.x || min_.y > max_.y || min_.z > max_.z
        || !bIsUniform || num_components_ <= 0)
    {
        Log.error() << "Something went wrong\n" << std::endl;
        return false;
    }
		
		idx_start_data_ = strstr(buffer, "@1") - buffer;
    
		return true;
	}
	
	bool AmiraMeshFile::read(RegularData3D& map)
	{
		// first read the header
		if (!readHeader())
		{
			Log.error() << "AmiraMeshFile::read(): readHeader() failed. Aborting read." << std::endl;
			return false;
		}

		RegularData3D::IndexType size;
		size.x = (Size) extent_.x;
		size.y = (Size) extent_.y;
		size.z = (Size) extent_.z;

		map = RegularData3D(size, min_, max_);

		Size num_to_read = size.x * size.y * size.z * num_components_;

		char file_buffer[2048];

		if (idx_start_data_ > 0)
		{
			size_t idx_actual_read = 0;

			std::fstream::seekg( idx_start_data_);
			
			if (binary_)
			{

				//Consume this line, which is the Lattice definition
				std::fstream::getline(file_buffer, 2048);		
				
				//Consume this line, which is an empty line
				std::fstream::getline(file_buffer, 2048);		

				//Consume this line, which is "# Data section follows"
				std::fstream::getline(file_buffer, 2048);		

				//Consume the next line, which is "@1"
				std::fstream::getline(file_buffer, 2048);		
				
				//char* data = new char[(4 * 192)];
				const Size char_to_read = num_to_read * sizeof(float);

				char* data = new char[char_to_read];
				
				std::fstream::read(data, char_to_read);
				
				while (idx_actual_read < num_to_read)
				{
					map[idx_actual_read] = static_cast<float>(*(data + 4*idx_actual_read));
					idx_actual_read++;

				}

				delete [] data;
			}
			else
			{
				
				//Consume this line, which is the Lattice definition
				std::fstream::getline(file_buffer, 2048);		

				//Consume this line, which is an empty line
				std::fstream::getline(file_buffer, 2048);		

				//Consume the next line, which is "@1"
				std::fstream::getline(file_buffer, 2048);		

				float a, b, c, d, e, f = 0.0;


				//while (!std::fstream::eof())
				while (idx_actual_read < num_to_read)
				{
					std::fstream::getline(file_buffer, 2048);		
					sscanf(file_buffer, "%e %e %e %e %e %e", &a, &b, &c, &d, &e, &f);

					map[idx_actual_read++] = a;
					map[idx_actual_read++] = b;
					map[idx_actual_read++] = c;
					map[idx_actual_read++] = d;
					map[idx_actual_read++] = e;
					map[idx_actual_read++] = f;

				}
			}
		}
		else
		{
			Log.error() << "Error: Corrupted data section.  ";
		}
		return true;
	}
	
	bool AmiraMeshFile::writeHeader()
	{
		// construct a correct header array and write it.
		// TODO: implement
		return false;
	}

	bool AmiraMeshFile::write(RegularData3D& /*map*/)
	{
		// Write the content of a RegularData3D dataset to a CCP4 file.
		// TODO: implement
		return false;
	}
	
	const char* AmiraMeshFile::findAndJump_(const char* buffer, const char* search_string)
	{
    const char* found_loc = strstr(buffer, search_string);
    if (found_loc) return found_loc + strlen(search_string);
    return buffer;
	}
} // namespace BALL

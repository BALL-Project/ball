// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/DSN6File.h>

namespace BALL
{

	DSN6File::DSN6File()
		: File(),
		swap_bytes_(false),
		cell_scaling_(1.0),
		prod_(1.0),
		plus_(0.0)
	{
	}

	DSN6File::DSN6File(const String& name, File::OpenMode open_mode)
		: File(name, open_mode),
		swap_bytes_(false),
		cell_scaling_(1.0),
		prod_(1.0),
		plus_(0.0)
	{
		// DSN6Files are always binary
		if ((open_mode & std::ios::binary) == 0)
		{
			open_mode_ = (open_mode | std::ios::binary); 
			reopen();
		}
	}

	DSN6File::~DSN6File()
	{
		close();
		clear();
	}

	void DSN6File::clear()
	{
		File::clear();
		swap_bytes_ = false;
	}


	bool DSN6File::operator == (const DSN6File& file) const
	{
		return (   (File::operator == (file))
						 &&(swap_bytes_    == file.swap_bytes_));
	}

	bool DSN6File::isSwappingBytes() const
	{
		return swap_bytes_;
	}


	bool DSN6File::open(const String& name, File::OpenMode open_mode)
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

	bool DSN6File::readHeader()
	{
		// first read the complete 512 bytes of header information
		char header[512];
		std::fstream::read(header, 512);

		if (gcount() != 512)
		{
			Log.error() << "DSN6File::readHeader(): File does not contain a proper DSN6 header. Aborting read." << std::endl;

			return false;
		}

		// to determine whether we have to swap bytes in the header (depending on the version of
		// the DSN6 - File and on the byte order on the machine) we try to reproduce the known value
		// of 100 in header[2*18]
		short int header_value = readHeaderValue_(header, 18);
		
		if (header_value != 100)
		{
			// try to change endianness
			swap_bytes_ = true;

			header_value = readHeaderValue_(header, 18);
			if (header_value != 100)
			{
				Log.error() << "DSN6File::readHeader(): Corrupt DSN6 header: header[16] != 100. Aborting read." << std::endl;
				
				return false;
			}
		}

		header_value = readHeaderValue_(header, 0);
		start_.x = (float)header_value;

		header_value = readHeaderValue_(header, 1);
		start_.y = (float)header_value;

		header_value = readHeaderValue_(header, 2);
		start_.z = (float)header_value;

		header_value = readHeaderValue_(header, 3);
		extent_.x = (float)header_value;

		header_value = readHeaderValue_(header, 4);
		extent_.y = (float)header_value;

		header_value = readHeaderValue_(header, 5);
		extent_.z = (float)header_value;

		header_value = readHeaderValue_(header, 6);
		sampling_rate_.x = (float)header_value;

		header_value = readHeaderValue_(header, 7);
		sampling_rate_.y = (float)header_value;

		header_value = readHeaderValue_(header, 8);
		sampling_rate_.z = (float)header_value;

		header_value = readHeaderValue_(header, 17);	
		cell_scaling_ = (float)header_value;

		header_value = readHeaderValue_(header, 9);
		crystal_dimension_.x = (float)header_value / cell_scaling_;  

		header_value = readHeaderValue_(header, 10);
		crystal_dimension_.y = (float)header_value / cell_scaling_;

		header_value = readHeaderValue_(header, 11);
		crystal_dimension_.z = (float)header_value / cell_scaling_;

		header_value = readHeaderValue_(header, 12);
		alpha_ = Angle((float)header_value / cell_scaling_, false);

		header_value = readHeaderValue_(header, 13);
		beta_  = Angle((float)header_value / cell_scaling_, false);

		header_value = readHeaderValue_(header, 14);
		gamma_ = Angle((float)header_value / cell_scaling_, false);

		header_value = readHeaderValue_(header, 15);
		prod_ = (float)header_value / 100.;

		header_value = readHeaderValue_(header, 16);
		plus_ = (float)header_value;

		// convert from grid space to cartesian coordinates (inspired by the VMD code :-) )
		Vector3 scaled_axes(crystal_dimension_.x/sampling_rate_.x,
												crystal_dimension_.y/sampling_rate_.y,
											 	crystal_dimension_.z/sampling_rate_.z);
		
		Vector3 x_tmp(scaled_axes.x, 0., 0.);
		Vector3 y_tmp(cos(gamma_.toRadian()), sin(gamma_.toRadian()), 0.);
		y_tmp *= scaled_axes.y;
		Vector3 z_tmp( cos(beta_.toRadian()), 
									(cos(alpha_.toRadian()) - cos(beta_.toRadian())*cos(gamma_.toRadian())) / sin(gamma_.toRadian()),
									0.);
		z_tmp.z = sqrt(1.0 - z_tmp.x*z_tmp.x - z_tmp.y*z_tmp.y);
		z_tmp *= scaled_axes.z;

		origin_.x = x_tmp.x * start_.x + y_tmp.x * start_.y + z_tmp.x * start_.z;
		origin_.y = y_tmp.y * start_.y + z_tmp.y * start_.z;
		origin_.z = z_tmp.z * start_.z;

		xaxis_.x = x_tmp.x * (extent_.x - 1);
		xaxis_.y = 0.;
		xaxis_.z = 0.;

		yaxis_.x = y_tmp.x * (extent_.y - 1);
		yaxis_.y = y_tmp.y * (extent_.y - 1);
		yaxis_.z = 0.;

		zaxis_.x = z_tmp.x * (extent_.z - 1);
		zaxis_.y = z_tmp.y * (extent_.z - 1);
		zaxis_.z = z_tmp.z * (extent_.z - 1);

		// that's it. we're done
		return true;
	}

	short int DSN6File::readHeaderValue_(char* header, Position pos)
	{
		short int val = *((short int*)(header + 2*pos));

		if (swap_bytes_)
			swapBytes(val);

		return val;
	}
			
	bool DSN6File::writeHeader()
	{
		// construct a correct header array and write it.
//		char header[512];

		// TODO: implement
		return false;
	}

	bool DSN6File::read(RegularData3D& density_map)
	{

		// first read the header
		if (!readHeader())
		{
			Log.error() << "DSN6File::read(): readHeader() failed. Aborting read." << std::endl;
			return false;
		}

		// and then the individual bricks. each brick contains 8^3 grid points, stored as bytes
		// which are converted to floats using the formula 
		// density = (byte_value - plus) / prod
		float factor = 1./prod_;

		// how many bricks do we have?
		Size number_of_bricks_x = (Size) ceil(extent_.x / 8.0);
		Size number_of_bricks_y = (Size) ceil(extent_.y / 8.0);
		Size number_of_bricks_z = (Size) ceil(extent_.z / 8.0);
		
		Size global_index = 0;
		Size brick_index = 0;
		char brick[512];
		unsigned char* brick_pointer;

		RegularData3D::IndexType size;
		size.x = (Size) extent_.x;
		size.y = (Size) extent_.y;
		size.z = (Size) extent_.z;
		
		density_map = RegularData3D(origin_, xaxis_, yaxis_, zaxis_, size);

		// NOTE: this currently only works for orthogonal maps!!!
		// TODO: implement a simple volumetric data type. all we need to do currently
		//       is to set a matrix converting between the given coordinate system of
		//       the volumetric data set and 3D space
		density_map.setDimension(Vector3(xaxis_.x, yaxis_.y, zaxis_.z));

		Size brick_x, brick_y, brick_z;
		// the ordering in both the individual bricks and the whole filw
		// is (fastest to slowest) x - y - z
		for (brick_z = 0; brick_z < number_of_bricks_z; brick_z++)
		{
			for (brick_y = 0; brick_y < number_of_bricks_y; brick_y++)
			{
				for (brick_x = 0; brick_x < number_of_bricks_x; brick_x++)
				{
					brick_index = 0;
					// read the next brick
					std::fstream::read(brick, 512);
					brick_pointer = (unsigned char*)brick;

					if (gcount() != 512)
					{
						Log.error() << "DSN6File::read(): Could not read next brick. Aborting read." << std::endl;
						return false;
					}

					// and swap its bytes
					convertBrick_(brick);

					// code is inspired by the VMD code :-)
					Size x, y, z;
					for (z=0; z<8; z++) // iterate over z of the current brick
					{
						if ((z + brick_z*8) >= extent_.z)
						{
							global_index += (Size)((8-z)*extent_.x*extent_.y);
							break;
						}	
					
						for (y=0; y<8; y++)
						{
							if ((y + brick_y*8) >= extent_.y)
							{
								global_index += (Size)((8-y)*extent_.x);
								brick_index  += (Size)((8-y)*8);
								break;
							}
							
							for (x=0; x<8; x++)
							{
								if ((x + brick_x*8) >= extent_.x)
								{
									global_index += (Size)(8 - x);
									brick_index  += (Size)(8 - x);
									break;
								}
	
								float brick_value = (float)(*(brick_pointer+brick_index));
								density_map[global_index] = factor * (brick_value - plus_);

								brick_index++;
								global_index++;
							} // for x...

							global_index += (Size)(extent_.x - 8);
						} // for y...

						global_index += (Size)(extent_.x*extent_.y - 8*extent_.x);
					} // for z...

					global_index += (Size)(8 - 8*extent_.x*extent_.y);
				} // for brick_x

				global_index += (Size)(8 * (extent_.x - number_of_bricks_x));
			}	// for brick_y	

			global_index += (Size)(8 * (extent_.x*extent_.y - extent_.x*brick_y));
		}	 // for brick_z

		// done.
		return true;
	}

	void DSN6File::convertBrick_(char* brick)
	{
		for (Size i=0; i<512; i+=2)
		{
			std::swap(brick[i], brick[i+1]);
    }
	}
} // namespace BALL

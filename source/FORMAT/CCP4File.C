// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: CCP4File.C,v 1.1.4.1 2007/03/25 22:00:16 oliver Exp $
//

#include <BALL/FORMAT/CCP4File.h>

namespace BALL
{
	CCP4File::CCP4File()
		: File(),
		  swap_bytes_(false),
		  offset_symops_(0),
		  col_axis_(1),
		  row_axis_(2),
		  sec_axis_(3),
		  mean_density_(0),
		  deviation_sigma_(0),
		  space_group_(-1)
	{
	}

	CCP4File::CCP4File(const String& name, File::OpenMode open_mode)
		: File(name, open_mode),
			swap_bytes_(false),
			offset_symops_(0),
			col_axis_(1),
			row_axis_(2),
			sec_axis_(3),
		  mean_density_(0),
		  deviation_sigma_(0),
		  space_group_(-1)
	{
		// CCP4Files are always binary
		if ((open_mode & std::ios::binary) == 0)
		{
			open_mode_ = (open_mode | std::ios::binary); 
			reopen();
		}
	}

	CCP4File::~CCP4File()
	{
		close();
		clear();
	}

	void CCP4File::clear()
	{
		File::clear();
		swap_bytes_= false;
		offset_symops_= 0;
		col_axis_ = 0;
		row_axis_ = 1;
		sec_axis_ = 2;
	}


	bool CCP4File::operator == (const CCP4File& file) const
	{
		return (   (File::operator == (file))
						 &&(swap_bytes_    == file.swap_bytes_)
						 &&(offset_symops_== file.offset_symops_)
						 &&(col_axis_== file.col_axis_)
						 &&(row_axis_== file.row_axis_)
						 &&(sec_axis_== file.sec_axis_));
	}



	bool CCP4File::open(const String& name, File::OpenMode open_mode)
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

	bool CCP4File::readHeader()
	{
		// first read the complete 1024 bytes of header information
		char header[1024];
		std::fstream::read(header, 1024);

		if (gcount() != 1024)
		{
			Log.error() << "CCP4File::readHeader(): File does not contain a proper CCP4 header. Aborting read." << std::endl;

			return false;
		}
		// Currently only data_mode=2 is allowed, which stores density values as 4-byte float values	
		Index data_mode = readBinValueasInt_(header, 3);
		
		if (data_mode != 2)
		{
			// try to change endianness
			swap_bytes_= true;

			data_mode = readBinValueasInt_(header, 3);
			if (data_mode != 2)
			{
				Log.error() << "CCP4File::readHeader(): Corrupt CCP4 header: data mode not supported, only 32-bit float supported" << std::endl;
				return false;
			}
		}
		
		//check if file claims to have symmetry reocrds stored	
		Size size_of_symops = readBinValueasInt_(header, 23);
		if (size_of_symops != 0)
		{
			offset_symops_ = size_of_symops;
		}
		
		// check internal ordering of coordinate axis
		col_axis_ = readBinValueasInt_(header, 16)-1;
		row_axis_ = readBinValueasInt_(header, 17)-1;
		sec_axis_ = readBinValueasInt_(header, 18)-1;

		extent_.x = (float)readBinValueasInt_(header, 0+col_axis_);
		extent_.y = (float)readBinValueasInt_(header, 0+row_axis_);
		extent_.z = (float)readBinValueasInt_(header, 0+sec_axis_);
		
		start_.x = (float)readBinValueasInt_(header, 4+col_axis_);
		start_.y = (float)readBinValueasInt_(header, 4+row_axis_);
		start_.z = (float)readBinValueasInt_(header, 4+sec_axis_);
		
		sampling_rate_.x = (float)readBinValueasInt_(header, 7);
		sampling_rate_.y = (float)readBinValueasInt_(header, 8);
		sampling_rate_.z = (float)readBinValueasInt_(header, 9);
		
		cell_dimension_.x = readBinValueasFloat_(header, 10);
		cell_dimension_.y = readBinValueasFloat_(header, 11);
		cell_dimension_.z = readBinValueasFloat_(header, 12);
		
		// Angle values of 0 don't make sense, set the Angles to 90 deg
		if (		readBinValueasFloat_(header, 13) == 0
				||	readBinValueasFloat_(header, 14) == 0
				||	readBinValueasFloat_(header, 15) == 0)
		{
			alpha_ = Angle(90.,false);
			beta_ = Angle(90.,false);
			gamma_ = Angle(90.,false);
		}
		else
		{
			alpha_ = Angle(readBinValueasFloat_(header, 13),false);
			beta_ = Angle(readBinValueasFloat_(header, 14),false);
			gamma_ = Angle(readBinValueasFloat_(header, 15),false);
		}	
		
		mean_density_ = readBinValueasFloat_(header, 21);
		space_group_ = readBinValueasInt_(header, 22);
		deviation_sigma_ = readBinValueasFloat_(header, 54);

		Log.info() << "Mean from file: " << mean_density_ << std::endl;
		Log.info() << "Sigma from file: " << deviation_sigma_ << std::endl;
		
		// convert from grid space to cartesian coordinates
		Vector3 scaled_axes(cell_dimension_.x/sampling_rate_.x,
												cell_dimension_.y/sampling_rate_.y,
												cell_dimension_.z/sampling_rate_.z);
		
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

	bool CCP4File::readSymmetryRecords()
	{
		//check if file has special symmetry records
		Distance offset = getSize() - int(4*(extent_.x*extent_.y*extent_.z));
		if (offset != offset_symops_)
		{
			if(offset == 0) 
			{
				Log.info() << "CCP4File::readSymmetryRecords(): Warning: File claims to have symmetry record, but in fact has not" << std::endl;
				offset_symops_ = 0;
			}
			else if (offset < 0)
			{

				Log.error() << "CCP4File::readSymmetryRecords(): Error: File seems to be truncated" << std::endl;
				return false;
			}
			else if ((offset - offset_symops_)< 0)
			{
				Log.info() << "CCP4File::readSymmetryRecords(): Error: Symmetry Record smaller than expected" << std::endl;
				return false;
			}
		}
	
		if(offset_symops_ != 0)
		{
			char sym_record[80];
			Size sym_lines = 0;
			Log.info() << "CCP4File::readSymmetryRecords(): File has the following symmetry information stored:" << std::endl;
			while (sym_lines < offset_symops_/80)
			{
				std::fstream::read(sym_record, 80);
				Log.info() << "--> " << sym_record << std::endl;
				sym_lines++;
			}
		}
		else
		{
			Log.info() << "CCP4File::readSymmetryRecords(): File has no symmetry records" << std::endl;
		}
		return true;
	}
	
	int CCP4File::readBinValueasInt_(char* data, Position pos)
	{
		int int_value = *((int*)(data + 4*pos));

		if (swap_bytes_)
			swapBytes(int_value);

		return int_value;
	}
	
	float CCP4File::readBinValueasFloat_(char* data, Position pos)
	{
		float float_value = *((float*)(data + 4*pos));

		if (swap_bytes_)
			swapBytes(float_value);

		return float_value;
	}
			
	bool CCP4File::writeHeader()
	{
		// construct a correct header array and write it.
		// TODO: implement
		return false;
	}

	bool CCP4File::write(RegularData3D& /*map*/)
	{
		// Write the content of a RegularData3D dataset to a CCP4 file.
		// TODO: implement
		return false;
	}

	bool CCP4File::read(RegularData3D& density_map)
	{

		// first read the header
		if (!readHeader())
		{
			Log.error() << "CCP4File::read(): readHeader() failed. Aborting read." << std::endl;
			return false;
		}

		// then try to read symmetry records if present
		if (!readSymmetryRecords())
		{
			Log.error() << "CCP4File::read(): readSymmteryRecords() failed. Aborting read." << std::endl;
			return false;
		}
		
		if (int(getSize()) > int(4*(extent_.x*extent_.y*extent_.z)))
		{
			Log.info() << "CCP4File::read(): Warning: datablock bigger than expected. But continuing anyway." << std::endl;
		}
		
		Size global_index = 0;

		RegularData3D::IndexType size_crs;
		size_crs.x = (Size) extent_[(Position) col_axis_];
		size_crs.y = (Size) extent_[(Position) row_axis_];
		size_crs.z = (Size) extent_[(Position) sec_axis_];
		
		RegularData3D::IndexType size;
		size.x = (Size) extent_.x;
		size.y = (Size) extent_.y;
		size.z = (Size) extent_.z;
		
		density_map = RegularData3D(origin_, xaxis_, yaxis_, zaxis_, size);
		density_map.setDimension(Vector3(xaxis_.x, yaxis_.y, zaxis_.z));
		char* rowblock = new char[4*int(size_crs.x)];	
		
		Size crs[3];
		for(crs[2]=0; crs[2] < size_crs.z; crs[2]++)
		{
			for(crs[1]=0; crs[1] < size_crs.y; crs[1]++)
			{
				std::fstream::read(rowblock, 4*(int(size_crs.x)));
				for(crs[0]=0; crs[0] < size_crs.x; crs[0]++)
				{
					global_index = (Size)(crs[col_axis_]+ crs[row_axis_]*size.x + crs[sec_axis_]*size.x*size.y);
					density_map[global_index] = readBinValueasFloat_(rowblock,crs[0]); 
				}
			}
		}

		delete [] rowblock;

		return true;
	}

} // namespace BALL

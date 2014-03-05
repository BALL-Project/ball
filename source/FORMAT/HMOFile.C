// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/HMOFile.h>

namespace BALL
{
	HMOFile::HMOFile()
		: LineBasedFile(),
			charges_(),
			comments_()
	{
	}

	HMOFile::HMOFile(const String& filename, File::OpenMode open_mode)
		: LineBasedFile(filename, open_mode),
		  charges_(),
			comments_()
	{
	}

	HMOFile::~HMOFile()
	{
	}

	void HMOFile::clear()
	{
		LineBasedFile::clear();

		charges_.clear();
		comments_.clear();
	}

	bool HMOFile::operator == (const HMOFile& file)
	{
		// ignore the comments for comparison
		return (    (LineBasedFile::operator == (file)
		         && (charges_ == file.charges_)));
	}

	bool HMOFile::open(const String& name, File::OpenMode open_mode)
	{
		return LineBasedFile::open(name, open_mode);
	}
	
	bool HMOFile::read(Surface& surface)
	{
		// we can only read something if the file has been opened correctly
		if (!isOpen() || getOpenMode() != MODE_IN)
		{
			return false;
		}

		// now make sure that there is no old crap lying around
		charges_.clear();
		comments_.clear();

		// find the first block of interest (NODL_DATA)
		if (!readUntil_("BEG_NODL_DATA"))
		{
			throw(Exception::ParseError(__FILE__, __LINE__, "HMOFile::read", "file does not contain node data"));	
		}

		readNodeData_(surface);

		// now, find the geometric elements (ELEM_DATA)
		if (!readUntil_("BEG_ELEM_DATA"))
		{
			throw(Exception::ParseError(__FILE__, __LINE__, "HMOFile::read", "file does not contain element data"));	
		}

		readElementData_(surface);

		// now, see if there is additional charge data (CHARGE_DATA); if not, we are done
		if (readUntil_("BEG_CHARGE_DATA"))
		{
			readChargeData_();	
		}

		return true;
	}

	bool HMOFile::write(Surface const& surface)
	{
		// we can only write something if the file has been opened correctly
		if (!isOpen() || !(getOpenMode() & MODE_OUT))
		{
			throw File::CannotWrite(__FILE__, __LINE__, name_);
		}

		// write a comment as a header replacement
		if (comments_.size() == 0)
		{
			getFileStream() << "# HYPERMESH file written by BALL::HMOFile" << std::endl << std::endl;
		}
		else
		{
			for (Position i=0; i<comments_.size(); ++i)
			{
				getFileStream() << comments_[i] << std::endl;
			}
			getFileStream() << std::endl;
		}

		writeNodes_(surface);
		writeElements_(surface);

		return true;
	}

	bool HMOFile::write(Surface const& surface, AtomContainer const& ac)
	{
		if (!isOpen() || !(getOpenMode() & MODE_OUT))
		{
			throw File::CannotWrite(__FILE__, __LINE__, name_);
		}

		// write a comment as a header replacement
		if (comments_.size() == 0)
		{
			getFileStream() << "# HYPERMESH file for " << ac.getName() << ", written by BALL::HMOFile" << std::endl << std::endl;
		}
		else
		{
			for (Position i=0; i<comments_.size(); ++i)
			{
				getFileStream() << comments_[i] << std::endl;
			}
			getFileStream() << std::endl;
		}

		writeNodes_(surface);
		writeElements_(surface);
		writeCharges_(ac);

		return true;
	}

	void HMOFile::readNodeData_(Surface& surface)
	{
		// at this point, we know that we are at the BEG_NODL_DATA line
		bool parsing_done = false;

		// the next line will contain the number of nodes
		if (!readLine())
		{
			throw Exception::ParseError(__FILE__, __LINE__, "HMOFile::readNodeData_", "node data does not contain number of entries");
		}

		Size num_nodes = 0;
		try 
		{
			num_nodes = line_.toInt();	
		} 
		catch (...) 
		{
			throw Exception::ParseError(__FILE__, __LINE__, "HMOFile::readNodeData_", "node data does not contain number of entries");
		}

		Size current_node = 0;
		std::vector<String> split;

		while (!parsing_done && readLine())
		{
			line_.split(split);

			if (line_.hasSubstring("END_NODL_DATA"))
			{
				parsing_done = true;
				break;
			}

			try 
			{
				surface.vertex.push_back(Vector3(split[1].toFloat(), split[2].toFloat(), split[3].toFloat()));
			} 
			catch (...) 
			{
				throw Exception::ParseError(__FILE__, __LINE__, "HMOFile::readNodeData_", "invalid node entry");
			}

			++current_node;
		}

		if (current_node != num_nodes)
		{
			Log.warn() << "HMOFile::readNodeData_: number of nodes found (" 
			           << current_node 
								 << ") does not match number of nodes expected ("
								 << num_nodes
								 << ")!" << std::endl;
		}

		if (!parsing_done)
		{
			throw Exception::ParseError(__FILE__, __LINE__, "HMOFile::readNodeData_", "node data not correctly terminated");
		}	
	}

	void HMOFile::readElementData_(Surface& surface)
	{
		// at this point, we know that we are at the BEG_ELEM_DATA line
		bool parsing_done = false;

		// the next line will contain the number of elements of the different supported kinds
		if (!readLine())
		{
			throw Exception::ParseError(__FILE__, __LINE__, "HMOFile::readElementData_", "element data does not contain number of entries");
		}

		std::vector<String> elem_numbers;
		line_.split(elem_numbers);

		if (elem_numbers.size() != 13)
		{
			throw Exception::ParseError(__FILE__, __LINE__, "HMOFile::readElementData_", "format error in first line of elements block (expected 13 numbers)");
		}

		Size total_num_elements = 0;
		try 
		{
			total_num_elements = elem_numbers[0].toInt();

			Size sum_elements = elem_numbers[1].toInt();
			for (Position i=2; i<13; ++i)
				sum_elements += elem_numbers[i].toInt();

			if (total_num_elements != sum_elements)
			{
				Log.warn() << "HMOFile::readElementData_: number of total elements (" 
									 << total_num_elements 
									 << ") does not match sum of elements by type ("
									 << sum_elements
									 << ")!" << std::endl;
			}
		} 
		catch (...) 
		{
			throw Exception::ParseError(__FILE__, __LINE__, "HMOFile::readElementData_", "format error in number of elements");
		}

		Size current_elem = 0;
		std::vector<String> split;

		while (!parsing_done && readLine())
		{
			line_.split(split);

			if (line_.hasSubstring("END_ELEM_DATA"))
			{
				parsing_done = true;
				break;
			}

			try 
			{
				// TODO: generate different surfaces for different components. so far, we just ignore it
				Size component_number = split[1].toInt();

				if (component_number != 1)
				{
					Log.warn() << "HMOFile::readElementData_: support for multiple components is not implemented yet" << std::endl;
				}

				Size element_type = split[2].toInt();

				if (element_type != T3)
				{
					Log.warn() << "HMOFile::readElementData_: support for non-triangular elements not implemented yet; ignoring element" << std::endl;
					continue;
				}

				Surface::Triangle t;
				t.v1 = split[3].toInt();
				t.v2 = split[4].toInt();
				t.v3 = split[5].toInt();

				surface.triangle.push_back(t);
			} 
			catch (...) 
			{
				throw Exception::ParseError(__FILE__, __LINE__, "HMOFile::readNodeData_", "invalid element entry");
			}

			++current_elem;
		}

		if (current_elem != total_num_elements)
		{
			Log.warn() << "HMOFile::readElementData_: number of elements found (" 
			           << current_elem 
								 << ") does not match number of elements expected ("
								 << total_num_elements
								 << ")!" << std::endl;
		}

		if (!parsing_done)
		{
			throw Exception::ParseError(__FILE__, __LINE__, "HMOFile::readElementData_", "element data not correctly terminated");
		}	
	}

	void HMOFile::readChargeData_()
	{
		// at this point, we know that we are at the BEG_CHARGE_DATA line
		bool parsing_done = false;

		// the next line will contain the number of charges
		if (!readLine())
		{
			throw Exception::ParseError(__FILE__, __LINE__, "HMOFile::readChargeData_", "charge data does not contain number of entries");
		}

		Size num_charges = 0;
		try 
		{
			num_charges = line_.toInt();	
		} 
		catch (...) 
		{
			throw Exception::ParseError(__FILE__, __LINE__, "HMOFile::readChargeData_", "charge data does not contain number of entries");
		}

		Size current_charge = 0;
		std::vector<String> split;

		while (!parsing_done && readLine())
		{
			line_.split(split);

			if (line_.hasSubstring("END_CHARGE_DATA"))
			{
				parsing_done = true;
				break;
			}

			try 
			{
				HMOCharge c;

				c.position.x = split[1].toFloat();
				c.position.y = split[2].toFloat();
				c.position.z = split[3].toFloat();

				c.value = split[4].toFloat();

				charges_.push_back(c);
			} 
			catch (...) 
			{
				throw Exception::ParseError(__FILE__, __LINE__, "HMOFile::readChargeData_", "invalid charge entry");
			}

			++current_charge;
		}

		if (current_charge != num_charges)
		{
			Log.warn() << "HMOFile::readChargeData_: number of charges found (" 
			           << current_charge 
								 << ") does not match number of charges expected ("
								 << num_charges
								 << ")!" << std::endl;
		}

		if (!parsing_done)
		{
			throw Exception::ParseError(__FILE__, __LINE__, "HMOFile::readChargeData_", "charge data not correctly terminated");
		}	
	}

	bool HMOFile::readUntil_(String const& pattern)
	{
		bool pattern_found = false;

		while (!pattern_found && readLine())
		{
			// is this a comment?
			if (line_.hasPrefix("#"))
			{
				comments_.push_back(line_);

				continue;
			}

			// nope. does it match the pattern?
			if (line_.hasSubstring(pattern))
			{
				pattern_found = true;
			}
		}

		return pattern_found;
	}

	void HMOFile::writeNodes_(Surface const& surface)
	{
		getFileStream() << "BEG_NODL_DATA" << std::endl;
		getFileStream() << "\t" << surface.vertex.size() << std::endl;

		for (Position i=0; i<surface.vertex.size(); ++i)
		{
			Vector3 const& v = surface.vertex[i];

			getFileStream() << "\t" << i+1 << " " << v.x << " " << v.y << " " << v.z << std::endl;
		}

		getFileStream() << "END_NODL_DATA" << std::endl << std::endl;
	}

	void HMOFile::writeElements_(Surface const& surface)
	{
		getFileStream() << "BEG_ELEM_DATA" << std::endl;

		// NOTE: so far, we only support elements of type T3, and single components
		Size num_triangles = surface.triangle.size();
		getFileStream() << "\t" << num_triangles << " 0 0 " << num_triangles << " 0 0 0 0 0 0 0 0 0" << std::endl;

		for (Position i=0; i<num_triangles; ++i)
		{
			Surface::Triangle const& t = surface.triangle[i];

			getFileStream() << "\t" << i+1 << "\t" << 1 << " " << T3 << "\t";
			getFileStream() << t.v1 << " " << t.v2 << " " << t.v3 << std::endl;
		}

		getFileStream() << "END_ELEM_DATA" << std::endl << std::endl;
	}

	void HMOFile::writeCharges_(AtomContainer const& ac)
	{
		getFileStream() << "BEG_CHARGE_DATA" << std::endl;

		Size num_charges = ac.countAtoms();
		getFileStream() << "\t" << num_charges << std::endl;

		Size current_atom = 1;
		for (AtomConstIterator at_it = ac.beginAtom(); +at_it; ++at_it)
		{
			Vector3 const& pos = at_it->getPosition();
			getFileStream() << "\t" << current_atom << " " << pos.x << " " << pos.y << " " << pos.z << " " << at_it->getCharge() << std::endl;
			++current_atom;
		}

		getFileStream() << "END_CHARGE_DATA" << std::endl;
	}
}

// $Id: defaultProcessors.C,v 1.12 2001/04/29 23:57:55 oliver Exp $

#include <BALL/STRUCTURE/defaultProcessors.h>

#include <BALL/COMMON/limits.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/residue.h>
#include <stdio.h>
#include <math.h>

using namespace std;

namespace BALL 
{

	Processor::Result ClearChargeProcessor::operator () (Atom& atom)
	{
		atom.setCharge(0);

		return Processor::CONTINUE;
	}

	Processor::Result ClearRadiusProcessor::operator () (Atom& atom)
	{
		atom.setRadius(0);

		return Processor::CONTINUE;
	}


	// AssignRadiusProcessor

	AssignRadiusProcessor::AssignRadiusProcessor()
		:	number_of_errors_(0),
			number_of_assignments_(0)
	{
	}

	AssignRadiusProcessor::AssignRadiusProcessor(const String& filename)
		:	number_of_errors_(0),
			number_of_assignments_(0)
	{
		// find the file in the data path
		Path path;
		filename_ = path.find(filename);
		
		if (filename_ == "")
		{
			// throw FileNotFound if the file could not be found in the DATA_PATH
			throw Exception::FileNotFound(__FILE__, __LINE__, filename);
		}
	}

	bool AssignRadiusProcessor::start()
	{
		number_of_errors_ = 0;
		number_of_assignments_ = 0;
		return buildTable_();
	}

	bool AssignRadiusProcessor::finish()
	{
		return true;
	}

	Processor::Result AssignRadiusProcessor::operator () (Atom& atom)
	{
		String		name;
		String		atom_name;
		String		res_name;
		float			radius;

		Fragment* frag = atom.getFragment();
		if (frag != 0)
		{
			res_name = frag->getName().trim();
		} 
		else 
		{
			res_name = "";
		}
		atom_name = atom.getName().trim();
		String original_res_name = res_name;

		if (RTTI::isKindOf<Residue>(*atom.getFragment()))
		{
			Residue* residue;
			residue = RTTI::castTo<Residue>(*atom.getFragment());
				
			String suffix("-");
			if (residue->isNTerminal())
			{
				suffix = "-N";
			}

			if (residue->isCTerminal())
			{
				suffix = "-C";
			}

			if (residue->hasProperty(Residue::PROPERTY__HAS_SSBOND))
			{
				suffix += "S";
			}

			if (suffix != "-")
			{
				res_name += suffix;				
			}
		}

		name = res_name + ":" + atom_name;

		if (table_.has(name.c_str()))
		{
			number_of_assignments_++;
			radius = (*table_.find(name.c_str())).second;
			atom.setRadius(radius);
		} 
		else 
		{
			// first try the unmodified residue
			name = original_res_name + ":" + atom_name;
			if (table_.has(name.c_str()))
			{
				number_of_assignments_++;
				radius = (*table_.find(name.c_str())).second;
				atom.setRadius(radius);
			} 
			else 
			{
				// try wildcard matching
				name = "*:" + atom_name;
				if (table_.has(name.c_str()))
				{
					number_of_assignments_++;
					radius = (*table_.find(name.c_str())).second;
					atom.setRadius(radius);
				} 
				else 
				{
					Log.level(LogStream::WARNING) << "Cannot assign radius for " << res_name << ":" << atom_name << endl;
					number_of_errors_++;
				}
			}
		}

					
		return Processor::CONTINUE;
	}

	bool AssignRadiusProcessor::buildTable_()
	{
		ifstream	infile(filename_.c_str());

		if (!infile)
		{
			infile.close();
			throw Exception::FileNotFound(__FILE__, __LINE__, filename_);
		}

		table_.destroy();
					
		String line;
		String fields[2];
		while (infile)
		{
			line.getline(infile);
			line.split(fields, 2);
			table_[fields[0]] = fields[1].toFloat();
		}
					
		infile.close();

		return true;
	}

	unsigned long AssignRadiusProcessor::getNumberOfErrors()
	{
		return number_of_errors_;
	}

	unsigned long AssignRadiusProcessor::getNumberOfAssignments()
	{
		return number_of_assignments_;
	}

	void AssignRadiusProcessor::setFilename(const String& filename)
	{
		Path path;
		filename_ = path.find(filename);

		if (filename_ == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename);
		}
	}

	 
	String& AssignRadiusProcessor::getFilename()
	{
		return filename_;
	}

	 
	// AssignChargeProcessor
	AssignChargeProcessor::AssignChargeProcessor()
		:	AssignRadiusProcessor()
	{
	}

	AssignChargeProcessor::AssignChargeProcessor(const String& filename)
		: AssignRadiusProcessor(filename)
	{
	}

	bool AssignChargeProcessor::start()
	{
		number_of_errors_ = 0;
		number_of_assignments_ = 0;
		total_charge_ = 0;

		return buildTable_();
	}

	Processor::Result AssignChargeProcessor::operator () (Atom& atom)
	{
		String		name;
		String		atom_name;
		String		res_name;
		float			charge;

		Fragment* frag = atom.getFragment();
		if (frag != 0)
		{
			res_name = frag->getName().trim();
		} 
		else 
		{
			res_name = "";
		}
		atom_name = atom.getName().trim();
		String original_res_name = res_name;

		if (RTTI::isKindOf<Residue>(*atom.getFragment()))
		{
			Residue* residue;
			residue = RTTI::castTo<Residue>(*atom.getFragment());
				
			String suffix("-");
			if (residue->isNTerminal())
			{
				suffix = "-N";
			}

			if (residue->isCTerminal())
			{
				suffix = "-C";
			}

			if (residue->hasProperty(Residue::PROPERTY__HAS_SSBOND))
			{
				suffix += "S";
			}

			if (suffix != "-")
			{
				res_name += suffix;				
			}
		}

		name = res_name + ":" + atom_name;


		if (table_.has(name))
		{
			number_of_assignments_++;
			charge = (*table_.find(name)).second;
			atom.setCharge(charge);
			total_charge_ += charge;
		} 
		else 
		{
			// first try the unmodified residue
			name = original_res_name + ":" + atom_name;
			
			if (table_.has(name))
			{
				number_of_assignments_++;
				charge = (*table_.find(name)).second;
				atom.setCharge(charge);
				total_charge_ += charge;
			} 
			else 
			{
				// try wildcard matching
				name = "*:" + atom_name;
				if (table_.has(name))
				{
					number_of_assignments_++;
					charge = (*table_.find(name)).second;
					atom.setCharge(charge);
					total_charge_ += charge;
				} 
				else 
				{
					Log.level(LogStream::WARNING) << "Cannot assign charge for " << res_name << ":" << atom_name << endl;
					number_of_errors_++;
				}
			}
		}
				
		return Processor::CONTINUE;
	}

	float AssignChargeProcessor::getTotalCharge()
	{
		return total_charge_;
	}

} // namespace BALL

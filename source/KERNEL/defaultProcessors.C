// $Id: defaultProcessors.C,v 1.2 1999/09/07 13:48:29 len Exp $

#include <BALL/KERNEL/defaultProcessors.h>

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PSE.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/residue.h>
#include <stdio.h>
#include <math.h>
#include <values.h>

namespace BALL 
{

	Processor::Result ClearChargeProcessor::operator()(Atom& atom)
	{
		atom.setCharge(0);

		return Processor::CONTINUE;
	}

	Processor::Result ClearRadiusProcessor::operator()(Atom& atom){
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
		:	filename_(filename),
			number_of_errors_(0),
			number_of_assignments_(0)
	{
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
		String		name(":");
		String		atom_name;
		String		res_name;
		float			radius;


		if (RTTI<Residue>::isKindOf(*atom.getFragment()))
		{
			Residue* residue;
			residue = RTTI<Residue>::castTo(*atom.getFragment());
				
			if (residue->isNTerminal())
			{
				name = "-N:";
			}

			if (residue->isCTerminal())
			{
				name = "-C:";
			}
		}

		Fragment* frag = atom.getFragment();
		if (frag != 0)
		{
			res_name = frag->getName().trim();
		} else {
			res_name = "";
		}
		atom_name = atom.getName().trim();
		name.insert(0, res_name);
		name.append(atom_name);

		if (table_.has(name.c_str())){
			number_of_assignments_++;
			radius = (*table_.find(name.c_str())).second;
			atom.setRadius(radius);
		} else {
			// first try the unmodified residue
			name = res_name + ":" + atom_name;
			if (table_.has(name.c_str())){
				number_of_assignments_++;
				radius = (*table_.find(name.c_str())).second;
				atom.setRadius(radius);
			} else {
				// try wildcard matching
				name = "*:" + atom_name;
				if (table_.has(name.c_str())){
					number_of_assignments_++;
					radius = (*table_.find(name.c_str())).second;
					atom.setRadius(radius);
				} else {
					Log.level(LogStream::WARNING) << "Cannot assign radius for " << res_name << ":" << atom_name << endl;
					number_of_errors_++;
				}
			}
		}

					
		return Processor::CONTINUE;
	}

	bool AssignRadiusProcessor::buildTable_()
	{
		FILE*					infile;
		char					residue_name[1024], atom_name[1024], key[2048];
		float					value;

		infile = fopen(filename_.c_str(), "r");

		if (infile == NULL) 
		{
			Log.level(LogStream::ERROR) << "Cannot open file: " << filename_ << endl;
			return false;
		}

		table_.destroy();
					
		while (!feof(infile))
		{
			fscanf(infile, "%s %s %f", &residue_name[0], &atom_name[0], &value);
			sprintf(&key[0], "%s:%s", residue_name, atom_name);
			table_[key] = value;
		}
					
		fclose(infile);

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
		filename_ = filename;
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
		String		name(":");
		String		atom_name;
		String		res_name;
		float			charge;

		if (RTTI<Residue>::isKindOf(*atom.getFragment()))
		{
			Residue*	residue;
			residue = RTTI<Residue>::castTo(*atom.getFragment());
				
			if (residue->isNTerminal())
			{
				name = "-N:";
			}

			if (residue->isCTerminal())
			{
				name = "-C:";
			}
		}

		Fragment* frag = atom.getFragment();
		if (frag != 0)
		{
			res_name = frag->getName().trim();
		} else {
			res_name = "";
		}
		atom_name = atom.getName().trim();
		name.insert(0, res_name);
		name.append(atom_name);

		if (table_.has(name.c_str())){
			number_of_assignments_++;
			charge = (*table_.find(name.c_str())).second;
			atom.setCharge(charge);
			total_charge_ += charge;
		} else {
			// first try the unmodified residue
			name = res_name;
			name.append(":");
			name.append(atom_name);
			
			if (table_.has(name.c_str())){
				number_of_assignments_++;
				charge = (*table_.find(name.c_str())).second;
				atom.setCharge(charge);
				total_charge_ += charge;
			} else {
				// try wildcard matching
				name = "*:" + atom_name;
				if (table_.has(name.c_str())){
					number_of_assignments_++;
					charge = (*table_.find(name.c_str())).second;
					atom.setCharge(charge);
					total_charge_ += charge;
				} else {
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

	ElementSelector::ElementSelector()
		:	element_(&Element::UNKNOWN)
	{
	}

	ElementSelector::ElementSelector(const Element& element)
		:	element_(&element)
	{
	}

	void ElementSelector::setElement(const Element& element)
	{
		element_ = &element;
	}

	Processor::Result ElementSelector::operator () (Atom& atom)
	{
		if (atom.getElement() == *element_)
		{
			atom.select();
		}
		
		return Processor::CONTINUE;
	}
	
} // namespace BALL

// $Id: createSpectrumProcessor.C,v 1.10 2000/12/08 09:25:43 oliver Exp $

#include <BALL/NMR/createSpectrumProcessor.h>
#include <BALL/NMR/shiftModule.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/atom.h>

using namespace std;

namespace BALL
{

	CreateSpectrumProcessor::CreateSpectrumProcessor()
		:	width_(1.0),
			use_averaging_(true),
			use_ignore_table_(true),
			expression_("element(H)")
	{
	}

	CreateSpectrumProcessor::~CreateSpectrumProcessor ()
		throw()
	{
	}

	bool CreateSpectrumProcessor::start()
		throw()
	{
		// clear the contents of the old peak list
		peaklist_.clear();
		
		return valid_;
	}

	void CreateSpectrumProcessor::init()
		throw()
	{
		valid_ = false;

		// read the contents of the ignore set
		// this hash set contains all quickly exchanging protons
		// -- thos protons are not seen in the spectrum and do
		// thus not produce a paek in the peak list.
		ignore_atoms_.clear();
		Path path;
		String filename = path.find("NMR/ignore_atoms.dat");
		if (filename == "")
		{
			Log.warn() << "CreateSpectrumProcessor::start: could not open atom ignore file NMR/ignore_atoms.dat" << endl;
			return;
		}

		ifstream infile(filename.c_str());
		String name;
		while (infile.good())
		{
			infile >> name;
			if (name != "")
			{
				ignore_atoms_.insert(name);
			}
		}
		infile.close();
		infile.clear();
		
		// read the list of equivalent protons
		filename = path.find("NMR/equivalent_atoms.dat");
		if (filename == "")
		{
			Log.warn() << "CreateSpectrumProcessor::start: could not open atom equivalency file NMR/equivalent_atoms.dat" << endl;
			return;
		}

		// clear old contents
		equivalency_residues_.clear();
		equivalency_atoms_.clear();
		
		ifstream eqfile(filename.c_str());
		while (eqfile.good())
		{
			String residue, atoms;
			vector<String> equivalencies;
			eqfile >> residue >> atoms;
			if ((residue != "") && (atoms != ""))
			{
				atoms.split(equivalencies, ",");
				equivalency_residues_.push_back(residue);
				equivalency_atoms_.push_back(equivalencies);
			}
		}
		eqfile.close();

		// initialization successful
		valid_ = true;

		return;
	}

	Processor::Result CreateSpectrumProcessor::operator () (Composite& composite)
		throw()
	{		
		// Collect all atoms with assigned chemical shifts
		Atom* atom = dynamic_cast<Atom*>(&composite);
		if (atom != 0)
		{
			if (atom->hasProperty(ShiftModule::PROPERTY__SHIFT))
			{
				float shift = atom->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat();
				// if the atom is in the ignore table, skip it
				if ((!(ignore_atoms_.has(atom->getFullName())
							 || ignore_atoms_.has("*:" + atom->getName()))
						 || !use_ignore_table_)
						&& (expression_(*atom) == true)
						&& (shift != 0.0))
				{
					Peak1D peak;
					peak.setAtom(atom);
					peak.setValue(shift);
					peak.setWidth(width_);
					peak.setHeight(1.0);
					peaklist_.push_back(peak);	
				}
			}	
		}
		

		if (use_averaging_ == true)
		{
			// average the shifts of chemically equivalent nuclei
			// in a residue
			Residue* residue = dynamic_cast<Residue*>(&composite);
			if (residue != 0)
			{
				String residue_name = residue->getName();

				for (Position i = 0; i < equivalency_residues_.size(); i++)
				{
					if (residue_name == equivalency_residues_[i])
					{
						float shift = 0.0;
						list<Atom*> atoms;
						for (AtomIterator it = residue->beginAtom(); +it; ++it)
						{
							for (Position j = 0; j < equivalency_atoms_[i].size(); j++)
							{
								if (equivalency_atoms_[i][j] == it->getName())
								{
									atoms.push_back(&*it);
									shift += it->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat();
									break;
								}
							}
						}
						if (atoms.size() != 0)
						{
							shift /= (float)atoms.size();
							list<Atom*>::const_iterator list_it = atoms.begin();
							for (; list_it != atoms.end(); ++list_it)
							{
								(*list_it)->setProperty(ShiftModule::PROPERTY__SHIFT, shift);
							}
						}
					}
				}
			}
		}

		return Processor::CONTINUE;
	}

	const list<Peak1D>& CreateSpectrumProcessor::getPeakList() const
	{
		return peaklist_;
	}

	void CreateSpectrumProcessor::setWidth(float width)
		throw()
	{
		width_ = width;
	}
	
	float CreateSpectrumProcessor::getWidth() const
		throw()
	{
		return width_;
	}

	void CreateSpectrumProcessor::enableAveraging(bool flag)
		throw()
	{
		use_averaging_ = flag;
	}

	void CreateSpectrumProcessor::enableIgnoreTable(bool flag)
		throw()
	{
		use_ignore_table_ = flag;
	}
	
	void CreateSpectrumProcessor::setExpression(const String& expression)
		throw()
	{
		expression_.setExpression(expression);
	}
}	// namespace BALL

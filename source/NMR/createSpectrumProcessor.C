// $Id: createSpectrumProcessor.C,v 1.15 2001/12/30 13:28:52 sturm Exp $

#include <BALL/NMR/createSpectrumProcessor.h>
#include <BALL/NMR/shiftModule.h>
#include <BALL/DATATYPE/regularData1D.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/atom.h>

using namespace std;

namespace BALL
{

	const String CreateSpectrumProcessor::IGNORE_SECTION_NAME =  "IgnoreAtoms";
	const String CreateSpectrumProcessor::AVERAGE_SECTION_NAME =  "AverageAtoms";

	CreateSpectrumProcessor::CreateSpectrumProcessor()
		throw(Exception::FileNotFound, Exception::ParseError)
		:	width_(1.0),
			use_averaging_(true),
			use_ignore_table_(true),
			expression_("element(H)")
	{
		// call init to read the default config file
		init();
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

	void CreateSpectrumProcessor::init(const String& filename)
		throw()
	{
		valid_ = false;

		Path path;
		String expanded_filename = path.find(filename);
		if (expanded_filename == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename);
		}

		INIFile infile(expanded_filename);
		infile.read();

		if (!infile.isValid())
		{
			throw Exception::ParseError(__FILE__, __LINE__, "Error while reading INIFile ", expanded_filename);
		}

		
		String name;
		

		if (!infile.hasSection(IGNORE_SECTION_NAME))
		{
			throw Exception::ParseError(__FILE__, __LINE__, "Section not found in INIFile:", String("section ") + IGNORE_SECTION_NAME);
		}
		// Read the contents of the ignore set.
		// This hash set contains all quickly exchanging protons
		// -- those protons are not seen in the spectrum and do
		// thus not produce a peak in the peak list.
		ignore_atoms_.clear();
		INIFile::LineIterator line = infile.getSectionFirstLine(IGNORE_SECTION_NAME);
		INIFile::LineIterator last_line = infile.getSectionLastLine(IGNORE_SECTION_NAME);
		for (; line != last_line; ++line)
		{
			ignore_atoms_.insert(*line);
		}

		// read the list of equivalent protons
		// clear old contents
		equivalency_residues_.clear();
		equivalency_atoms_.clear();
		line = infile.getSectionFirstLine(AVERAGE_SECTION_NAME);
		last_line = infile.getSectionLastLine(AVERAGE_SECTION_NAME);
		for (; +line; ++line)
		{
			String atoms = (*line).after(" ");
			String residue = (*line).before(" ");
			if ((residue != "") && (atoms != ""))
			{
				vector<String> equivalencies;
				atoms.split(equivalencies, ",");
				equivalency_residues_.push_back(residue);
				equivalency_atoms_.push_back();
			}
		}

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
					peak.setPosition(shift);
					peak.setWidth(width_);
					peak.setIntensity(1.0);
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

	const PeakList1D& CreateSpectrumProcessor::getPeakList() const
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

	void CreateSpectrumProcessor::setAtomAveraging(bool flag)
		throw()
	{
		use_averaging_ = flag;
	}

	bool CreateSpectrumProcessor::getAtomAveraging() const
		throw()
	{
		return use_averaging_;
	}

	void CreateSpectrumProcessor::setAtomIgnoring(bool flag)
		throw()
	{
		use_ignore_table_ = flag;
	}
	
	bool CreateSpectrumProcessor::getAtomIgnoring() const
		throw()
	{
		return use_ignore_table_;
	}

	void CreateSpectrumProcessor::setExpression(const String& expression)
		throw()
	{
		expression_.setExpression(expression);
	}

	const String& CreateSpectrumProcessor::getExpression() const
		throw()
	{
		return expression_.getExpressionString();
	}

	const RegularData1D& operator << (RegularData1D& data, const PeakList1D& peak_list)
		throw()
	{
		// ?????

		return data;
	}
}	// namespace BALL

// $Id: rotamerLibrary.C,v 1.3 1999/08/27 14:01:41 oliver Exp $

#include <BALL/STRUCTURE/rotamerLibrary.h>
#include <BALL/SYSTEM/file.h>

namespace BALL
{

	/// Rotamer

	// default constructor
	Rotamer::Rotamer()
		: P(0.0),
			chi1(0.0),
			chi2(0.0),
			chi3(0.0),
			chi4(0.0)
	{
	}

	//	Copy constructor
	Rotamer::Rotamer(const Rotamer& rotamer)
		:	P(rotamer.P),
			chi1(rotamer.chi1),
			chi2(rotamer.chi2),
			chi3(rotamer.chi3),
			chi4(rotamer.chi4)
	{
	}

	//	Detailled constructor
	Rotamer::Rotamer(float new_P, float new_chi1, float new_chi2, float new_chi3, float new_chi4)
		: P(new_P),
			chi1(new_chi1),
			chi2(new_chi2),
			chi3(new_chi3),
			chi4(new_chi4)
	{
	}


	bool RotamerLibrary::readSQWRLLibraryFile(const String& filename, const FragmentDB& fragment_db)
	{
		// clear old stuff
		variants_.clear();
		valid_ = false;

		File f(filename);
		
		// abort if the file could not be opened
		if (!f)
		{	
			return false;
		}

		// read the file into a vector of Strings to reparse
		// it faster. Since the SQWRL format is a pain in the ass
		// we have to identify relevant lines by a regular expression
		HashSet<String> amino_acids;
		vector<String>	lines;
		String line;
		RegularExpression regexp("[A-Z][A-Z][A-Z] [0-9] [0-9] [0-9] [0-9] *[0-9]* *[0-9]* *[0-9\\.]*");
		while (f)
		{
			line.getline(f);
			if (regexp.match(line))
			{	
				lines.push_back(line);
				if (!amino_acids.has(line.getField(0)))
				{
					amino_acids.insert(line.getField(0));
				}
			}
		}
		
		// build list of variant names from template DB
		// for each amino acid from the library file
		vector<String> variant_names;
		
		
		HashSet<String>::const_iterator set_it = amino_acids.begin();
		list<String>::const_iterator list_it;
		String s[18];
		String name, variant_name;
		for (; set_it != amino_acids.end() ; set_it++)
		{
			name = *set_it;
			list<String> variant_names = template_db.getVariantNames(name);
			for (list_it = variant_names.begin(); list_it != variant_names.end(); ++list_it)
			{
				ResidueRotamerSet rs(template_db.getResidue(*list_it));
				for (Size i = 0; i < lines.size(); i++)
				{
					if (lines[i](0, 3) == name)
					{
						Size number_of_fields = lines[i].split(s, 18);
						float P = line.getField(7).toFloat() / 100.0;
						Angle chi1, chi2, chi3, chi4;
						chi1.set(line.getField(11).toFloat(), false);
						if (number_of_fields > 13)
						{
							chi2.set(line.getField(13).toFloat(), false);
						}
						if (number_of_fields > 15)
						{
							chi3.set(line.getField(15).toFloat(), false);
						}
						if (number_of_fields > 17)
						{
							chi4.set(line.getField(17).toFloat(), false);
						}
						rs.addRotamer(Rotamer(P, chi1, chi2, chi3, chi4));
					}
				}
				
				variants_.push_back(rs);
			}
		}

		valid_ = true;
			
		return true;
	}

#ifdef BALL_NO_INLINE_FUNCTIONS
# include <BALL/STRUCTURE/rotamerLibrary.h>
#endif
 
} // namespace BALL

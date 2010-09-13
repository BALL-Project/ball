// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/peptides.h>
#include <BALL/KERNEL/protein.h>

#include <list>
#include <vector>
#include <algorithm>

#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/SYSTEM/path.h>
	
using namespace std;

namespace BALL 
{
	namespace Peptides
	{

		const String one_letter_codes = "ARNDCQEGHILKMFPSTWYV";
		const String three_letter_codes = "ALA@ARG@ASN@ASP@CYS@GLN@GLU@GLY@HIS@ILE@LEU@LYS@MET@PHE@PRO@SER@THR@TRP@TYR@VAL@";

		char OneLetterCode(const String& aa)
		{
			if (aa.size() == 3)
			{
				String tmp = aa + "@";
				tmp.toUpper();
				Position idx = three_letter_codes.find(tmp);
				if (idx != String::EndPos)
				{
					return one_letter_codes[idx / 4];
				}
			}
			
			return '?';
		}

		String ThreeLetterCode(char aa)
		{
			aa = toupper(aa);
			Position idx = one_letter_codes.find(aa);
			return ((idx == String::EndPos) ? String("UNK") : String(three_letter_codes, idx * 4, 3));
		}
	 	
		bool IsOneLetterCode(char aa)
		{
			aa = toupper(aa);
			Position idx = one_letter_codes.find(aa);
			return ((idx == String::EndPos) ? false : true); 
		}
 		
		bool IsThreeLetterCode(const String& aa)
		{
			if (aa.size() == 3)
			{
				String tmp = aa + "@";
				tmp.toUpper();
				Position idx = three_letter_codes.find(tmp);
				return ((idx == String::EndPos) ? false : true);
			}
			return false;
		}

		
		OneLetterAASequence ThreeLetterToOneLetter(const ThreeLetterAASequence& sequence)
		{
			OneLetterAASequence tmp;
			ThreeLetterAASequence::const_iterator it(sequence.begin());
			for (; it != sequence.end(); ++it)
			{
				tmp += OneLetterCode(*it);
			}
			return tmp;
		}

		ThreeLetterAASequence OneLetterToThreeLetter(const OneLetterAASequence& sequence)
		{
			ThreeLetterAASequence tmp;
			for (Position i = 0; i < (Size)sequence.size(); i++)
			{
				tmp.push_back(ThreeLetterCode(sequence[i]));
			}
			return tmp;
		}

		OneLetterAASequence GetSequence(const Protein& protein)
		{
			// iterate over all residues in the protein
			OneLetterAASequence tmp;
			ResidueConstIterator it(protein.beginResidue());
			for (; +it; ++it)
			{
				//...and add the corresponding one-letter code to the sequence
				tmp += OneLetterCode(it->getName());
			}

			// return the sequence
			return tmp;
		}
		
		OneLetterAASequence GetSequence(const Chain& chain)
		{
			// iterate over all residues in the protein
			OneLetterAASequence tmp;
			ResidueConstIterator it(chain.beginResidue());
			for (; +it; ++it)
			{
				//...and add the corresponding one-letter code to the sequence
				tmp += OneLetterCode(it->getName());
			}

			// return the sequence
			return tmp;
		}
			

		/***************************************
		 *          Name Converter class
		 ***************************************/	

		const char*  NameConverter::Option::INIFile = "iniFile";
		const String NameConverter::Default::INIFile = "NMR/nameConversions.map";
		
		NameConverter::NameConverter()
			: options(),
				conventions_(),
				conversion_table_()
		{
			setDefaultOptions();
			readConversionTable_();
		}

		void NameConverter::setDefaultOptions()
		{		
			options.setDefault(NameConverter::Option::INIFile,
		                     NameConverter::Default::INIFile);		
		}
		
		void NameConverter::readConversionTable_()
			throw (Exception::FileNotFound())
		{
			// Open parameter file
			Path path;
			String filename = path.find(options[Option::INIFile]);
			if (filename == "") 
			{
				throw Exception::FileNotFound(__FILE__, __LINE__, options[Option::INIFile]);
			}

			String key   = "";
			String value = "";

			LineBasedFile file(filename);
			while (file.readLine())
			{
				String line = file.getLine();

				// do not read comments
				if ((line[0]=='#') || (line.trim() == ""))
				{
					continue;
				}

				vector<String> fields;
				line.split(fields);

				if (fields[0] == "A.A.") // this is the line containing the names of the mapping
				{
					for (Position i=1; i<fields.size(); ++i)
					{
						fields[i].toUpper();
						conventions_.push_back(fields[i]);
					}
				}
				else if (fields.size() != (conventions_.size()+1))
				{
					Log.error() << "Error in " << filename << " expected " << conventions_.size() 
											<< " entries, but got " << fields.size() << " in " << std::endl;
					Log.error() << file.getLine() << std::endl;
				}
				else
				{
					String amino_acid = fields[0];

					if (IsThreeLetterCode(amino_acid))
						amino_acid = OneLetterCode(amino_acid);

					conversion_table_.push_back(vector<String>());
					
					for (Position i=1; i<fields.size(); ++i)
					{
						String complete_name = amino_acid+":"+fields[i];
						conversion_table_.back().push_back(complete_name);
					}
				}	
			}
/*	
			for (Size j=0; j<conversion_table_.size(); j++)
			{
					std::cout << conversion_table_[j][0] << " " << conversion_table_[j][2] << std::endl;
			}
*/
			file.close();
		}

		String NameConverter::convertName(const String& amino_acid, const String& old_atom_name,
				                              const String& old_naming_scheme, const String& new_naming_scheme) const
		{
			// first, find the naming schemes
			String old_naming_scheme_upper = old_naming_scheme;
			old_naming_scheme_upper.toUpper();

			String new_naming_scheme_upper = new_naming_scheme;
			new_naming_scheme_upper.toUpper();

			Index old_scheme_index = -1, new_scheme_index = -1;
			for (Position i=0; i<conventions_.size(); ++i)
			{
				if (conventions_[i] == old_naming_scheme_upper)
					old_scheme_index = i;
				if (conventions_[i] == new_naming_scheme_upper)
					new_scheme_index = i;
			}

			if (old_scheme_index*new_scheme_index < 0)
			{
				return old_atom_name;
			}
			
			String amino_acid_one_letter = amino_acid;
			if (IsThreeLetterCode(amino_acid_one_letter))
				amino_acid_one_letter = OneLetterCode(amino_acid_one_letter);
			else
				amino_acid_one_letter.toUpper();

			String old_atom_name_upper = old_atom_name;
			old_atom_name_upper.toUpper();

			// try to find the old atom name in the table
			String to_find = amino_acid_one_letter+":"+old_atom_name_upper;

			Index row_index = -1;
			for (Position i=0; i<conversion_table_.size(); ++i)
			{
				if (conversion_table_[i][old_scheme_index] == to_find)
				{
					row_index = i;
					break;
				}
			}

			if (row_index < 0)
			{
				return old_atom_name;
			}
				
			String ret_string = conversion_table_[row_index][new_scheme_index];
			ret_string = ret_string.getSubstring(2, ret_string.size()-2);

			return ret_string; 
		}


		bool NameConverter::supportsNamingScheme(const String& scheme_name) const
		{
			bool ret_value = false;
			String naming_scheme_upper = scheme_name;
			naming_scheme_upper.toUpper();
			
			for (Position i=0; i<conventions_.size(); ++i)
			{
				if (conventions_[i] == naming_scheme_upper)
					ret_value = true;
			}
			return ret_value;
		}


	} // namespace Peptides

} // namespace BALL


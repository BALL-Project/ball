// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/SCWRLRotamerFile.h>
#include <BALL/STRUCTURE/rotamerLibrary.h>

using namespace std;

namespace BALL
{
		SCWRLRotamerFile::SCWRLRotamerFile() 
			:	LineBasedFile()
		{
		}

		SCWRLRotamerFile::SCWRLRotamerFile(const String& name, File::OpenMode open_mode) 
			:	LineBasedFile(name, open_mode)
		{
		}

		SCWRLRotamerFile::~SCWRLRotamerFile() 
		{
		}

		const SCWRLRotamerFile& SCWRLRotamerFile::operator = (const SCWRLRotamerFile& file)
		{
			LineBasedFile::operator = (file);
			return *this;
		}

		void SCWRLRotamerFile::operator >> (RotamerLibrary& rotamer_library)
		{
			// Ok, this is stupid, but at least better and faster than what we used to do.
			// We first try to read the file as a backbone-dependent version. If this fails,
			// we try the backbone-independent variant before we barf.

			try 
			{
				readSCWRLBackboneDependentLibraryFile(rotamer_library);
			} catch (...) { // ok, it was not a backbone dependent library. But is it indep?
				rewind();
				readSCWRLBackboneIndependentLibraryFile(rotamer_library);
			}

			if (!rotamer_library.validate())
			{
				throw(Exception::ParseError(__FILE__, __LINE__, "The discretization of the backbone torsions are not correct!", ""));
			}
			clear();
			return;
		}

		void SCWRLRotamerFile::readSCWRLBackboneDependentLibraryFile(RotamerLibrary& rotamer_library)
		{
			rotamer_library.setBackboneDependent(true);

			String aa_name;
			Index phi(0);
			Index psi(0);
			double probability(0);
		
			std::vector<String> split;
			while (readLine()) 
			{
				line_.split(split);
				if (split.size() != 17)
				{
					rotamer_library.clear();		
					throw(Exception::ParseError(__FILE__, __LINE__, "Invalid Format in backbone dependent SCWRL rotamer file!", ""));
				}

				phi = split[1].toInt();
				psi = split[2].toInt();
				probability = split[8].toFloat();

				Size number_of_torsions(0);
				for (Size i = 4; i != 8; ++i)
				{
					if (split[i].toInt() != 0)
					{
						number_of_torsions++;
					}
					else
					{
						break;
					}
				}

				//Angle chi1, chi2, chi3, chi4;
	
				//chi1.set(split[9].toFloat(), false);
				//chi2.set(split[10].toFloat(), false);
				//chi3.set(split[11].toFloat(), false);
				//chi4.set(split[12].toFloat(), false);
	
				aa_name = split[0];

				rotamer_library.addRotamer(aa_name, Rotamer(probability, split[9].toFloat(), 
																																 split[10].toFloat(), 
																																 split[11].toFloat(), 
																																 split[12].toFloat()), number_of_torsions, phi, psi);
			}
	
			return;
		}

		void SCWRLRotamerFile::readSCWRLBackboneIndependentLibraryFile(RotamerLibrary& rotamer_library)
		{
			rotamer_library.setBackboneDependent(false);

			// read the file into a vector of Strings to reparse
			// it faster. Since the SQWRL format is a pain in the ass
			// we have to identify relevant lines by a regular expression
			RegularExpression regexp("[A-Z][A-Z][A-Z] [0-9] [0-9] [0-9] [0-9] *[0-9]* *[0-9]* *[0-9\\.]*");
			String split[18];
			while (readLine())
			{
				if (regexp.match(line_))
				{
					String aa_name = line_(0, 3);	
					Size number_of_fields = line_.split(split, 18);

					float prob = split[7].toFloat() / 100.0;

					//Angle chi1, chi2, chi3, chi4;
					float chi1(0), chi2(0), chi3(0), chi4(0);
					//chi1.set(split[11].toFloat());
					chi1 = split[11].toFloat();
          Size number_of_torsions = 1;
          if (number_of_fields > 13)
          {
            //chi2.set(split[13].toFloat());
						chi2 = split[13].toFloat();
            number_of_torsions = 2;
          }
          if (number_of_fields > 15)
          {
            //chi3.set(split[15].toFloat());
						chi3 = split[15].toFloat();
            number_of_torsions = 3;
          }
          if (number_of_fields > 17)
          {
            //chi4.set(split[17].toFloat());
						chi4 = split[17].toFloat();
            number_of_torsions = 4;
          }

					rotamer_library.addRotamer(aa_name, Rotamer(prob, chi1, chi2, chi3, chi4), number_of_torsions);
				}
			}	
		}

} // namespace BALL

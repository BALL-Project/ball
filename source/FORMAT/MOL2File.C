// $Id: MOL2File.C,v 1.3 2000/05/15 19:16:27 oliver Exp $

#include <BALL/FORMAT/MOL2File.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>

#include <stack>

using namespace std;

namespace BALL 
{
	MOL2File::MOL2File()
	{
	}

	MOL2File::MOL2File(const String& name, File::OpenMode open_mode)
		: File(name, open_mode)
	{
	}

	MOL2File::~MOL2File()
	{
	}
	
	void MOL2File::write(const System& /* system */)
	{
	}


	// the Tripos record type identifier: RTI
	const String MOL2File::TRIPOS = "@<TRIPOS>";
	const Size MOL2File::MAX_LENGTH_ = 4096;

	void MOL2File::read(System& system)
	{
		// remove old rubbish from the system
		system.destroy();

		// remember the line number for error messages
		number_of_lines_ = 0;
		String line;
		
    while (getline(buffer_, MAX_LENGTH_))
    {
      number_of_lines_++;
      line = buffer_;
			line.toUpper();
			
			while (line.hasPrefix(TRIPOS))
			{
				// we found a "Record Type Identifier" (RTI)
				String RTI = line.after(TRIPOS);
				RTI.trim();
				
				#ifdef BALL_DEBUG
					// BAUSTELLE: debug code
					Log.info() << "MOL2File::read: reading record " << RTI << " in line " << number_of_lines_ << endl;
				#endif
				
				// interpret the RTI (at least the known ones)
				if (RTI == "ATOM")
				{
					line = readAtomSection_();
				} 
				else if (RTI == "BOND") 
				{
					line = readBondSection_();
				}	
				else if (RTI == "MOLECULE") 
				{
					line = readMoleculeSection_();
				}	
				else if (RTI == "SET") 
				{
					line = readSetSection_();
				}	
				else if (RTI == "SUBSTRUCTURE") 
				{
					line = readSubstructureSection_();
				} 
				else 
				{	
					// we found an unknown MOL2 section: print a warning message and ignore it!
					Log.warn() << "MOL2File::read: section ignored: " << line << endl;
				}
			}
		}
	}
				
	String MOL2File::readAtomSection_()
	{
		String line;
		Size number_of_fields = 1;
		while (getline(buffer_, MAX_LENGTH_) && (number_of_fields > 0) && !line.hasPrefix(TRIPOS))
		{
			number_of_lines_++;
			line = buffer_;
			line.trim();
			Size number_of_fields = line.countFields();
			if (number_of_fields > 0)
			{
				if (number_of_fields < 6)
				{
					Log.error() << "MOL2File::readAtomSection_: too few fields for an atom entry in line " 
											<< number_of_lines_ << endl;
				} 
				else 
				{	
					// split the line into fields
					String	fields[10];
					line.split(fields, 10);

					// create an atom and assign the fields of the line
					AtomStruct	atom;
					atom.name = fields[1];
					atom.position.x = fields[2].toFloat();
					atom.position.y = fields[3].toFloat();
					atom.position.z = fields[4].toFloat();
					atom.type = fields[5];
					atom.substructure = fields[6].toInt();
					atom.substructure_name = fields[7];
					atom.charge = fields[8].toFloat();
					
					// remember this atom
					atoms_.push_back(atom);
				}
			}
		}	

		number_of_lines_++;
		
		return line;
	}
			
	String MOL2File::readBondSection_()
	{
		String line;
		Size number_of_fields = 1;
		while (getline(buffer_, MAX_LENGTH_) && (number_of_fields > 0) && !line.hasPrefix(TRIPOS))
		{
			number_of_lines_++;
			line = buffer_;
			line.trim();
			Size number_of_fields = line.countFields();
			if (number_of_fields > 0)
			{
				if (number_of_fields < 4)
				{
					Log.error() << "MOL2File::readBondSection_: too few fields for a bond entry in line " 
											<< number_of_lines_ << endl;
				} 
				else 
				{
					// split the line into fields
					String	fields[4];
					line.split(fields, 4);

					// create an atom and assign the fields of the line
					BondStruct	bond;
					bond.atom1 = fields[1].toInt();
					bond.atom1 = fields[2].toInt();
					bond.type = fields[3];
					
					// remember this bond
					bonds_.push_back(bond);
				}
			}
		}	

		number_of_lines_++;
			
		return "";
	}
			
	String MOL2File::readSetSection_()
	{
		String line;
		Size number_of_fields = 1;
		while (getline(buffer_, MAX_LENGTH_) && (number_of_fields > 0) && !line.hasPrefix(TRIPOS))
		{
			number_of_lines_++;
			line = buffer_;
			line.trim();
			Size number_of_fields = line.countFields();
			if (number_of_fields > 0)
			{
				if (number_of_fields < 3)
				{
					Log.error() << "MOL2File::readBondSection_: too few fields for a bond entry in line " 
											<< number_of_lines_ << endl;
				} 
				else 
				{
					// split the line into fields
					String	fields[6];
					line.split(fields, 6);

					// create an atom and assign the fields of the line
					SetStruct	set;
					set.name = fields[1].toInt();
					set.type = fields[3].toInt();
					set.subtype = fields[4];
					set.comment = fields[6];
					if (fields[2] == "static")
					{
						getline(buffer_, MAX_LENGTH_);
						number_of_lines_++;
						line = buffer_;
						line.trim();
						Size number_of_fields = line.countFields();

						for (Size i = 1; (i <= (Size)line.getField(0).toInt()) && (i < number_of_fields); i++)
						{
							set.members.push_back(line.getField(i).toInt());
						}

						// remember this set
						sets_.push_back(set);

					} 
					else 
					{	
						// we cannot read dynamic sets. What is the syntax of these rules?
						Log.warn() << "MOL2File::readSetSection: unsupported set type: " << fields[2] << ". Ignored." << endl;
					}					
				}
			}
		}	

		number_of_lines_++;
				
		return "";
	}
			
	String MOL2File::readMoleculeSection_()
	{
		String line;
		Size number_of_fields = 1;
		while (getline(buffer_, MAX_LENGTH_) && (number_of_fields > 0) && !line.hasPrefix(TRIPOS))
		{
			number_of_lines_++;
			line = buffer_;
			line.trim();
			Size number_of_fields = line.countFields();
			if (number_of_fields > 0)
			{
				if (number_of_fields < 3)
				{
					Log.error() << "MOL2File::readBondSection_: too few fields for a bond entry in line " 
						<< number_of_lines_ << endl;
				} 
				else 
				{
					// split the line into fields
					String	fields[6];
					line.split(fields, 6);

					// create an atom and assign the fields of the line
					SetStruct	set;
					set.name = fields[1].toInt();
					set.type = fields[3].toInt();
					set.subtype = fields[4];
					set.comment = fields[6];
					if (fields[2] == "static")
					{
						getline(buffer_, MAX_LENGTH_);
						number_of_lines_++;
						line = buffer_;
						line.trim();
						Size number_of_fields = line.countFields();

						for (Size i = 1; (i <= (Size)line.getField(0).toInt()) && (i < number_of_fields); i++)
						{
							set.members.push_back(line.getField(i).toInt());
						}

						// remember this set
						sets_.push_back(set);

					} 
					else 
					{
						// we cannot read dynamic sets. What is the syntax of these rules?
						Log.warn() << "MOL2File::readSetSection: unsupported set type: " << fields[2] << ". Ignored." << endl;
					}					
				}
			}
		}	

		number_of_lines_++;
				
		return "";
	}
			
	String MOL2File::readSubstructureSection_()
	{
		return "";
	}

} // namespace BALL

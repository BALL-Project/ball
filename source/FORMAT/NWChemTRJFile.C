// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: NWChemTRJFile.C,v 1.1.2.2 2006/04/01 15:27:13 anhi Exp $
//

#include <BALL/FORMAT/NWChemTRJFile.h>
#include <BALL/MOLMEC/COMMON/snapShot.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PTE.h>

#undef BALL_DEBUG

using namespace std;

namespace BALL
{

	NWChemTRJFile::NWChemTRJFile()
		: TrajectoryFile(),
			has_solvent_coordinates_(true),
			has_solute_coordinates_(true),
			has_solvent_velocities_(false),
			has_solute_velocities_(false)
	{
		init();
	}


	NWChemTRJFile::NWChemTRJFile(const NWChemTRJFile& file)
		:	TrajectoryFile(file),
			has_solvent_coordinates_(file.has_solvent_coordinates_),
			has_solute_coordinates_(file.has_solute_coordinates_),
			has_solvent_velocities_(file.has_solvent_velocities_),
			has_solute_velocities_(file.has_solute_velocities_)
	{
		init();
	}


	NWChemTRJFile::NWChemTRJFile(const String& name, File::OpenMode open_mode)
		: TrajectoryFile(name, open_mode),
			has_solvent_coordinates_(true),
			has_solute_coordinates_(true),
			has_solvent_velocities_(false),
			has_solute_velocities_(false)
	{
		if ((open_mode & std::ios::binary) == 0)
		{
			open_mode_ = (open_mode | std::ios::binary); 
			reopen();
		}
		init();

		// If we want to open the file for writing, write a default header,
		// else read the header information from the existing file.
		if ((open_mode & std::ios::out) != 0)
		{
			// if this file is to be overwritten, write a default header.
			writeHeader();
		}
		else
		{
			readHeader();
		}
	}


	NWChemTRJFile::~NWChemTRJFile()
	{
		close();
		clear();
	}


	const NWChemTRJFile& NWChemTRJFile::operator = (const NWChemTRJFile& file)
	{
		TrajectoryFile::operator = (file);
		has_solvent_coordinates_ = file.has_solvent_coordinates_;	
		has_solute_coordinates_  = file.has_solute_coordinates_;	
		has_solvent_velocities_  = file.has_solvent_velocities_;	
		has_solute_velocities_   = file.has_solute_velocities_;	

		return *this;
	}


	void NWChemTRJFile::clear()
	{
		has_solvent_coordinates_ = true;
		has_solute_coordinates_  = true; 
		has_solvent_velocities_  = false;
		has_solute_velocities_   = false;

		TrajectoryFile::clear();
	}


	bool NWChemTRJFile::operator == (const NWChemTRJFile& file) const
	{
		return (TrajectoryFile::operator == (file));
	}


	bool NWChemTRJFile::open(const String& name, File::OpenMode open_mode)
	{
		if (!TrajectoryFile::open(name, open_mode))
			return(false);

		if ((open_mode & std::ios::out) != 0)
		{
			// if this file is to be overwritten, write a default header.
			return writeHeader();
		}
		
		return readHeader();
	}


	bool NWChemTRJFile::hasSolventVelocities() const
	{
		return has_solvent_velocities_;
	}

	bool NWChemTRJFile::hasSoluteVelocities() const
	{
		return has_solvent_velocities_;
	}


	bool NWChemTRJFile::readHeader()
	{
		// read the "header" keyword
		current_line_.getline(*this,  '\n');

		if (current_line_.find("header") != 0)
		{
			Log.error() << "NWChemTRJFile::readHeader(): wrong header; expected \"header\", got " 
									<< current_line_ << endl;
			return false;
		}

		current_line_.getline(*this, '\n');
		std::vector<String> fields;
		current_line_.split(fields);

		try {
			number_of_atoms_per_solvent_ = fields[0].toUnsignedInt();
			number_of_solute_atoms_      = fields[1].toUnsignedInt();
			number_of_solute_bonds_      = fields[2].toUnsignedInt();
			number_of_bonds_per_solvent_ = fields[3].toUnsignedInt(); // is this correct?
			if (fields.size() > 4)
				number_of_solvent_molecules_ = fields[4].toUnsignedInt();
		} catch (...) {
			Log.error() << "NWChemTRJFile::readHeader(): invalid format!\n" << std::endl;
			return false;
		}

		// read the solvent information. currently, we only handle
		// water correctly
		solvent_ = new Molecule;
		for (Size i=0; i<number_of_atoms_per_solvent_; i++)
		{
			current_line_.getline(*this, '\n');
			
			if (i==0)
				solvent_name_ = current_line_.getSubstring(0, 5);

			// try to guess the correct atom types
			Atom* a;

			String atom_name = current_line_.getSubstring(5, 5);
			if (atom_name[0] == 'O')
			{
				a	= new Atom(PTE[Element::O], atom_name);
			} 
			else if (atom_name[0] == 'H') 
			{
				a	= new Atom(PTE[Element::H], atom_name);
			} 
			else if (atom_name[1] == 'O') 
			{
				a	= new Atom(PTE[Element::O], atom_name);
			} 
			else if (atom_name[1] == 'H') 
			{
				a	= new Atom(PTE[Element::H], atom_name);
			}
			else
			{
				Log.info() << "Unknown atom type" << std::endl;
				a = new Atom;
			}
			solvent_->insert(*a);
		}	

		// read the solute. the identification of atoms and
		// Residue types is very rudimentary at the moment.
		Residue* r = 0;
		solute_ = new Protein;

		int current_residue = -1;

		for (Size current_atom = 0; current_atom < number_of_solute_atoms_; current_atom++)
		{
			current_line_.getline(*this, '\n');
			int new_residue_id = (int) String(current_line_.getSubstring(10,6)).trim().toUnsignedInt();

			if (new_residue_id != current_residue)
			{
				if (r!=0)
					solute_->insert(*r);
				r = new Residue;
				current_residue = new_residue_id;
			}
				
			r->setName(current_line_.getSubstring(0,5));
			PDBAtom* a = new PDBAtom(current_line_.getSubstring(5,5)); // is this a good idea?
			r->insert(*a);
		}
		solute_->insert(*r);
		
		// try to insert the solvent bonds... this does not seem to
		// work as expected. Water has _three_ bonds???
		for (int current_bond = 0; current_bond < (int)number_of_bonds_per_solvent_; current_bond++)
		{
			current_line_.getline(*this, '\n');
			current_line_.split(fields);

			int from = fields[0].toInt();
			int to   = fields[1].toInt();
			
			Atom& a_from = *(solvent_->getAtom(from-1));
			Atom& a_to   = *(solvent_->getAtom(to-1));

			a_from.createBond(a_to);
		}

		// now the solute bonds
		for (Size current_bond = 0; current_bond < number_of_solute_bonds_; current_bond++)
		{
			current_line_.getline(*this, '\n');
			current_line_.split(fields);

			int from = fields[0].toInt();
			int to   = fields[1].toInt();
			
			Atom& a_from = *(solute_->getAtom(from-1));
			Atom& a_to   = *(solute_->getAtom(to-1));

			a_from.createBond(a_to);
		}

		// now insert the correct number of solvent atoms
		for (Size current_solvent = 0; current_solvent < number_of_solvent_molecules_; current_solvent++)
		{
			Molecule *m = new Molecule(*solvent_);
			system_.insert(*m);
		}

		// and finally the solute
		system_.insert(*solute_);

		// done.
		return true;
	}


	bool NWChemTRJFile::writeHeader()
	{
		if (!isAccessible() || !isOpen()) return false;

		// TODO: implement
		return true;
	}

	bool NWChemTRJFile::read(SnapShot& snapshot)
	{
		#ifdef BALL_DEBUG
			Log.info() << "file position at beginning of read(): " << tellg() << endl;
 		#endif

		if (!good()) return false;

		// read the "frame" marker
		current_line_.getline(*this, '\n');
		if (current_line_.find("frame") != 0)
		{
			Log.error() << "NWChemTRJFile::read(): expected \"frame\" keyword, got " 
									<< current_line_ 
									<< "... bailing out." 
									<< std::endl;

			return false;
		}
		
		// the next line contains the time of the current snapshot, the temperature and the pressure
		// we currently ignore this information
		current_line_.getline(*this, '\n');

		// the next three lines contain the box dimensions (in nm). we ignore these as well
		current_line_.getline(*this, '\n');
		current_line_.getline(*this, '\n');
		current_line_.getline(*this, '\n');

		// now there's again a problem with the definition of the file format...
		// according to http://www.emsl.pnl.gov/docs/nwchem/nwchem.html, the next
		// line should contain four boolean flags: solvent coordinates, solvent velocities
		// solute coordinates, and solute velocities. In all the files I have at hand,
		// it contains _eight_ boolean flags... sigh...
		// We use the assumption that the first flag denotes solvent coordinates, the
		// second solvent velocities, the fifth solute coordinates and the sixth solute
		// velocities, but that might be terribly wrong...
		current_line_.getline(*this, '\n');
		String flags = current_line_.getSubstring(0,8);

		has_solvent_coordinates_ = ((flags[0] == 't') || (flags[0] == 'T')) ? true : false;
		has_solvent_velocities_  = ((flags[1] == 't') || (flags[1] == 'T')) ? true : false;

		has_solute_coordinates_ = ((flags[4] == 't') || (flags[4] == 'T')) ? true : false;
		has_solute_velocities_  = ((flags[5] == 't') || (flags[5] == 'T')) ? true : false;

		// the remainder of the line contains the number of solvent molecules, the
		// number of atoms per solvent, and the number of solute atoms to read. if
		// these don't match the ones from the header we bail out.

		Size solvent_mol_number  = String(current_line_.getSubstring(9,10)).trim().toUnsignedInt();
		Size solvent_atom_number = String(current_line_.getSubstring(19,10)).trim().toUnsignedInt();
		Size solute_atom_number  = String(current_line_.getSubstring(29, String::EndPos)).trim().toUnsignedInt();

		if (!(   (solvent_mol_number  == number_of_solvent_molecules_)
					 &&(solvent_atom_number == number_of_atoms_per_solvent_)
					 &&(solute_atom_number  == number_of_solute_atoms_)))
		{
			Log.error() << "NWChemTRJFile::read(): Atom counts do not match! Aborting." << std::endl;
			return false;
		}
			
		Size total_atom_number = solvent_mol_number * solvent_atom_number + solute_atom_number;
		std::vector<Vector3> positions(total_atom_number);
		std::vector<Vector3> velocities;

		if (has_solvent_velocities_ || has_solute_velocities_)
		{
			velocities.resize(total_atom_number);
		}

		// now read the solvent coordinates and velocities
		String x, y, z;
		Vector3 pos;
		Size solvent_atoms = solvent_mol_number*solvent_atom_number;
		Size current_atom = 0;

		for (; current_atom<solvent_atoms; current_atom++)
		{
			current_line_.getline(*this, '\n');

			x = current_line_.getSubstring(0,8); y = current_line_.getSubstring(8,8); z = current_line_.getSubstring(16,8);
			pos.x = x.trim().toFloat()*10; pos.y = y.trim().toFloat()*10; pos.z = z.trim().toFloat()*10;
			
			// convert to angstrom and set the position
			positions[current_atom] = pos; 

			if (has_solvent_velocities_)
			{
				x = current_line_.getSubstring(24,8); y = current_line_.getSubstring(32,8); z = current_line_.getSubstring(40,8);
				pos.x = x.trim().toFloat()*10; pos.y = y.trim().toFloat()*10; pos.z = z.trim().toFloat()*10;
				
				// convert to angstrom and set the velocity
				velocities[current_atom] = pos; 
			}
		}	

		// now read the solute positions and velocities
		for (; current_atom < solute_atom_number + solvent_atoms; current_atom++)
		{
			current_line_.getline(*this, '\n');

			x = current_line_.getSubstring(0,8); y = current_line_.getSubstring(8,8); z = current_line_.getSubstring(16,8);
			pos.x = x.trim().toFloat()*10; pos.y = y.trim().toFloat()*10; pos.z = z.trim().toFloat()*10;
			
			// convert to angstrom and set the position
			positions[current_atom] = pos; 

			if (has_solute_velocities_)
			{
				x = current_line_.getSubstring(24,8); y = current_line_.getSubstring(32,8); z = current_line_.getSubstring(40,8);
				pos.x = x.trim().toFloat()*10; pos.y = y.trim().toFloat()*10; pos.z = z.trim().toFloat()*10;
				
				// convert to angstrom and set the velocity
				velocities[current_atom] = pos; 
			}
		}
		
		// save the stuff in a snapshot
		snapshot.setNumberOfAtoms(total_atom_number);
		snapshot.setAtomPositions(positions);

		if (has_solute_velocities_ || has_solvent_velocities_)
			snapshot.setAtomVelocities(velocities);

		// done
		return true;
	}

	bool NWChemTRJFile::init()
	{
		#ifdef BALL_DEBUG
		 verbosity_ = 1;
		#endif

		return true;
	}

/**
	bool NWChemTRJFile::append(const SnapShot& snapshot)
	{
		if (number_of_atoms_ != snapshot.getNumberOfAtoms())
		{
			if (number_of_atoms_ != 0)
			{
				Log.error() << "Different number of atoms in SnapShot in NWChemTRJFile:" 
										<< snapshot.getNumberOfAtoms() << std::endl;
			}
			number_of_atoms_ = snapshot.getNumberOfAtoms();
		}
		
		// increase the snapshot counter for a correct header
		number_of_snapshots_++;

		if (snapshot.getAtomPositions().size() == 0)
		{
			Log.error() << "NWChemTRJFile::append(): No atom positions available" << endl;
			return false;
		}
		writeVector_(snapshot.getAtomPositions());

		if (has_velocities_)
		{
			if (snapshot.getAtomVelocities().size() == 0)
			{
				Log.error() << "DC2File::append(): No atom velocities available" << endl;
				return false;
			}
			writeVector_(snapshot.getAtomVelocities());
		}

		return seekAndWriteHeader();
	}


	void DCDFile::writeVector_(const vector<Vector3>& v)
	{
		writeSize_(4*number_of_atoms_);
		for (Size atom = 0; atom < number_of_atoms_; ++atom)
		{
			writeFloat_(v[atom].x);
		}
		writeSize_(4*number_of_atoms_);
		writeSize_(4*number_of_atoms_);
		for (Size atom = 0; atom < number_of_atoms_; ++atom)
		{
			writeFloat_(v[atom].y);
		}
		writeSize_(4*number_of_atoms_);
		writeSize_(4*number_of_atoms_);
		for (Size atom = 0; atom < number_of_atoms_; ++atom)
		{
			writeFloat_(v[atom].z);
		}
		writeSize_(4*number_of_atoms_);
	}


	bool DCDFile::seekAndWriteHeader()
	{
		Position here = tellp();
		seekp(0, ios::beg);
		bool return_value = writeHeader();
		seekp(here);
		return(return_value);
	}


	bool DCDFile::flushToDisk(const ::std::vector<SnapShot>& buffer)
	{
		if (!isOpen() || !(getOpenMode() & File::MODE_OUT))
		{
			throw (File::CannotWrite(__FILE__, __LINE__, name_));
		}

		if (buffer.size() == 0) return true;

		// adjust the number of snapshots for header
		number_of_snapshots_ += buffer.size();
		// ?????:
		// this is not quite the place to do this. it should be done when the
		// snapshot manager is set up or at some similar place. the question is
		// how much information has to be replicated at which place in the code.
		// we should think about something like updateHeader().
		number_of_atoms_ = buffer[0].getNumberOfAtoms();

		// write the header
		seekp(0, ios::beg);
		if (!writeHeader()) 
		{
			Log.error() << "Could not write header in DCDFile" << std::endl;
			return false;
		}

		// adjust the number of snapshots back because append increases this number also
		number_of_snapshots_ -= buffer.size();

		// append the data
		seekp(0, ios::end);

		::std::vector<SnapShot>::const_iterator it = buffer.begin();
		for(; it != buffer.end(); ++it)
		{
			if (!append(*it))
			{
				Log.error() << "Could not write SnapShot" << std::endl;
				return false;
			}
		}

		return true;
	}

**/



	void NWChemTRJFile::enableVelocityStorage()
	{
		has_solvent_velocities_ = true;
		has_solute_velocities_  = true;
	}

	
	void NWChemTRJFile::disableVelocityStorage()
	{
		has_solvent_velocities_ = false;
		has_solute_velocities_  = false;
	}

	System& NWChemTRJFile::getSystem()
	{
		return system_;
	}
} // namespace BALL

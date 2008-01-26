#include <BALL/MOLMEC/AMBER/GAFFTypeProcessor.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/QSAR/aromaticityProcessor.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/SYSTEM/path.h>

namespace BALL
{
	const String GAFFTypeProcessor::Option::ATOMTYPE_FILENAME = "atomtype_filename";
//	const String GAFFTypeProcessor::Default::ATOMTYPE_FILENAME = "Amber/GAFFTypes.dat";
	const String GAFFTypeProcessor::Default::ATOMTYPE_FILENAME = "Amber/AMBERTypes.dat";

	GAFFTypeProcessor::GAFFTypeProcessor()
		: UnaryProcessor<Composite>()
	{
		options.setDefault(Option::ATOMTYPE_FILENAME, Default::ATOMTYPE_FILENAME);
		parseAtomtypeTableFile_();
	}

	GAFFTypeProcessor::~GAFFTypeProcessor()
	{
		StringHashMap<GAFFCESParser*>::Iterator parser_it = ces_parsers_.begin();
		for (; parser_it != ces_parsers_.end(); ++parser_it)
			delete(parser_it->second);
	}	
	
	Processor::Result GAFFTypeProcessor::operator() (Composite &composite)
	{
		// TODO: think whether application to other things than molecules would make sense...
		bool value = false;
		if (RTTI::isKindOf<Molecule>(composite))
		{
			Molecule *mol = RTTI::castTo<Molecule>(composite);

			precomputeAtomProperties_(mol);

			AtomIterator atom_it = mol->beginAtom();
			for(;+atom_it;++atom_it)
			{
				value = assignAtomtype_(*atom_it);
			}	
		}

		return Processor::CONTINUE;
	}

	//read File with table of atomtypes and push atomtypes 
	//and their TypeDefinition in corresponding vector
	//and store a GAFFCESParser for every CESstring
	void GAFFTypeProcessor::parseAtomtypeTableFile_()
		throw(Exception::FileNotFound)
	{	
		ces_parsers_.clear();

		ifstream atomfile;
		Path path;
		String filename = path.find(options.get(Option::ATOMTYPE_FILENAME));

		atomfile.open(filename.c_str());
		if(!atomfile)
		{
			Log.error() << "atomtype table could not be read!" << endl;
			throw(Exception::FileNotFound(__FILE__, __LINE__, filename));
		}
	
		String table_line;
		while(getline(atomfile,table_line))
		{
			TypeDefinition typeDefinition;
			typeDefinition.atom_type = table_line.getField(0);
			typeDefinition.atomic_number = table_line.getField(1).toInt();
			typeDefinition.connectivity = table_line.getField(2).toInt();	
			typeDefinition.attached_hydrogens = table_line.getField(3);
			typeDefinition.electron_withdrawal_atoms = table_line.getField(4);
			typeDefinition.atomic_property = table_line.getField(5);
			typeDefinition.chemical_environment = table_line.getField(6);
			
			// create new parser to parse the given CESstring and store it
			String to_parse = "";
			if (typeDefinition.atomic_property != "*")
				to_parse = typeDefinition.atomic_property;
			to_parse += typeDefinition.chemical_environment;

			ces_parsers_.insert(to_parse, new GAFFCESParser(to_parse));

			// insert the type definition at the corresponding position
			if (atom_types_.find(typeDefinition.atomic_number) == atom_types_.end())
			{
				std::vector<TypeDefinition> v;
				atom_types_[typeDefinition.atomic_number] = v;
			}
			atom_types_[typeDefinition.atomic_number].push_back(typeDefinition);
		}
	}

	// store connectivity, number of attached H-atoms and 
	// number of attached N,O,F,Cl and Br-atoms for every atom in molecule
	void GAFFTypeProcessor::precomputeAtomProperties_(Molecule* molecule)
	{
		current_molecule_ = molecule;
		RingPerceptionProcessor rpp;
		rpp.calculateSSSR(sssr_, *molecule);

		AromaticityProcessor arp;
		arp.options.setBool(AromaticityProcessor::Option::OVERWRITE_BOND_ORDERS, false);
		arp.aromatize(sssr_, *molecule);
	
		annotateRingSizes_();
		annotateAliphaticAndAromaticRingAtoms_();
		annotatePlanarRingAtoms_();

		AtomIterator atom_it = molecule->beginAtom();
		
		for(;+atom_it;++atom_it)
		{
			int connectivity = (atom_it->countBonds());
 			atom_it->setProperty("connectivity", connectivity);

			int attached_hydrogens_int = 0;
			int electron_withdrawal_atoms_int = 0;
			String attached_hydrogens;
			String electron_withdrawal_atoms; 

			Atom::BondIterator bond_it = atom_it->beginBond();
			for(;+bond_it;++bond_it)
			{

				const Atom& partner_atom = *(bond_it->getBoundAtom(*atom_it));
				if(partner_atom.getElement() == PTE[Element::H])
				{
					++attached_hydrogens_int;
				}
				else 
				{
					for (Atom::BondConstIterator constBond_it = partner_atom.beginBond(); +constBond_it; ++constBond_it)
					{
						//not to consider the atom we started at
						if (constBond_it->getBoundAtom(partner_atom) == &(*atom_it))
							continue;

						const Atom& partner_partner_atom = *(constBond_it->getBoundAtom(partner_atom));

						if(   (partner_partner_atom.getElement()== PTE[Element::N])
								||(partner_partner_atom.getElement()== PTE[Element::O]) 
								||(partner_partner_atom.getElement()== PTE[Element::F]) 
								||(partner_partner_atom.getElement()== PTE[Element::Cl]) 
								||(partner_partner_atom.getElement()== PTE[Element::Br]))
						{
							++electron_withdrawal_atoms_int;
						}
					}
				}
			} 

			attached_hydrogens.set(attached_hydrogens_int);
			electron_withdrawal_atoms.set(electron_withdrawal_atoms_int);

 			atom_it->setProperty("attached hydrogens", attached_hydrogens);
 			atom_it->setProperty("electron withdrawal atoms", electron_withdrawal_atoms);
		}
	}
	
	void GAFFTypeProcessor::annotateRingSizes_()
	{
		std::vector<std::vector<Atom* > >::iterator ring_it = sssr_.begin();
		for(;ring_it != sssr_.end();++ring_it)
		{
			String property;
			bool   in_ring;
			
			switch (ring_it->size())
			{
				case 3: property = "In3Ring";
								in_ring  = true;
								break;
				case 4: property = "In4Ring";
								in_ring  = true;
								break;
				case 5: property = "In5Ring";
								in_ring  = true;
								break;
				case 6: property = "In6Ring";
								in_ring  = true;
								break;
				case 7: property = "In7Ring";
								in_ring  = true;
								break;
				case 8: property = "In8Ring";
								in_ring  = true;
								break;
				case 9: property = "In9Ring";
								in_ring  = true;
								break;
				default: in_ring = false;
			}

			std::vector<Atom*>::iterator atom_it = ring_it->begin();
			for(;atom_it != ring_it->end();++atom_it)
			{
				if (in_ring)
					(*atom_it)->setProperty(property, true);
			}
		}
	}

	// checks if current atom is in an aliphatic ringsystem,
	// which is made of sp3 Carbon or a purely aromatic ring
	void GAFFTypeProcessor::annotateAliphaticAndAromaticRingAtoms_()	
	{
		std::vector<std::vector<Atom* > >::iterator ring_it = sssr_.begin();
		for(;ring_it != sssr_.end();++ring_it)
		{
			bool purely_aliphatic = true;
			bool purely_aromatic  = true;

			vector<Atom*>::iterator atom_it = ring_it->begin();
			for(;atom_it != ring_it->end();++atom_it)
			{
				// if one ring member is not sp3 carbon, the whole thing isn't	
				if( ((*atom_it)->getElement() != PTE[Element::C]) || ((*atom_it)->countBonds() != 4))
				{
					purely_aliphatic = false;
				}

				if ( !(*atom_it)->getProperty("IsAromatic").getBool() )
				{
					purely_aromatic = false;
				}
			}

			if (purely_aliphatic)
			{
				for(;atom_it != ring_it->end();++atom_it)
					(*atom_it)->setProperty("IsPureAliphatic", true);
			}
			if (purely_aromatic)
			{
				for(;atom_it != ring_it->end();++atom_it)
					(*atom_it)->setProperty("IsPureAromatic", true);
			}
		}
	}

	// checks if current atom is in a planar ringsystem
	void GAFFTypeProcessor::annotatePlanarRingAtoms_()
	{
		std::vector<std::vector<Atom* > >::iterator ring_it = sssr_.begin();
		for(;ring_it != sssr_.end();++ring_it)
		{
			bool is_planar = true;
			bool has_db_to_non_ring = false;

			vector<Atom*>::iterator atom_it = ring_it->begin();
			for(;atom_it != ring_it->end();++atom_it)
			{
				// if one atom isn't planar, the whole ring isn't.
				if( !planarAtom_(**atom_it) )
				{
					is_planar = false;
					break;
				}

				// is there one atom with a double bond to a non-ring?
				if (!has_db_to_non_ring) // otherwise, we don't need to look for another one!
				{
					Atom::BondConstIterator constBond_it = (*atom_it)->beginBond();
					for(;+constBond_it;++constBond_it)
					{
						if(   ((constBond_it->getProperty("GAFFBondType").getString())== "DB")
								||((constBond_it->getProperty("GAFFBondType").getString())== "db")) 
						{
							const Atom* partner_atom = constBond_it->getBoundAtom(**atom_it);
							if (!partner_atom->getProperty("InRing").getBool())
								has_db_to_non_ring = true;
						}
					}
				}
			}

			// if the ring is planar 
			if (is_planar)
			{
				for(;atom_it != ring_it->end();++atom_it)
				{
					(*atom_it)->setProperty("IsPlanarRingAtom", true);
					if (has_db_to_non_ring)
						(*atom_it)->setProperty("IsPlanarWithDBtoNR", true);
				}
			}
		}
	}

	// check if current atom could be in a planar ring
	// s.t. atom is a C(X3),N(X2),N(X3),O(X2),S(X2),P(X2),P(X3)
	bool GAFFTypeProcessor::planarAtom_(const Atom& atom)
	{
		Element element = atom.getElement();
		int num_bonds   = atom.countBonds();
		// C(X2) or C(X3)
		if(		 (element == PTE[Element::C]) &&((num_bonds == 2) ||(num_bonds == 3)))
			return true;
		// N(X2) or N(X3)
		if(	(element == PTE[Element::N]) &&((num_bonds == 2) || (num_bonds == 3)))
			return true;
		// P(X2) or P(X3)
		if(	(element == PTE[Element::P]) &&((num_bonds == 2) || (num_bonds == 3)))
			return true;
		// O(X2)
		if((element == PTE[Element::O]) && (num_bonds == 2))
			return true;
		// S(X2)
		if((element == PTE[Element::S]) && (num_bonds == 2))
			return true;		

		return false;
	}	

	bool GAFFTypeProcessor::assignAtomtype_(Atom& atom)
	{
		// do we have a chance to match this atom at all?
		if (atom_types_.find(atom.getElement().getAtomicNumber()) == atom_types_.end())
		{
			Log.error() << "GAFFTypeProcessor: Could not assign atom type for " << atom.getFullName() << std::endl;
			Log.error() << "GAFFTypeProcessor: Reason: no type definition for atomic number " << atom.getElement().getAtomicNumber() << " available!" << std::endl;
			
			return false;
		}

		std::vector<TypeDefinition>& type_defs = atom_types_[atom.getElement().getAtomicNumber()];
		for (Position i=0; i<type_defs.size(); i++)
		{
			TypeDefinition& typeDefinition = type_defs[i];
			//all fields with "*" are invalid and therefore considered as True
			if(atom.getProperty("connectivity").getInt() == typeDefinition.connectivity)
			{
				if(		(atom.getProperty("attached hydrogens").getString() == typeDefinition.attached_hydrogens) 
						||(typeDefinition.attached_hydrogens == "*"))
				{
					if(		(atom.getProperty("electron withdrawal atoms").getString() == typeDefinition.electron_withdrawal_atoms) 
							||(typeDefinition.electron_withdrawal_atoms == "*"))
					{
						String atomic_property = typeDefinition.atomic_property;

						String to_match = "";
						if (typeDefinition.atomic_property != "*")
							to_match = typeDefinition.atomic_property;
						to_match += typeDefinition.chemical_environment;
						
						if(	(ces_parsers_[to_match]->match(atom)))
						{
							atom.setProperty("atomtype", typeDefinition.atom_type );

							cout << "atom name: " << atom.getName() << " atomtype:" << typeDefinition.atom_type << endl;

							return true;	
						}
					}
				}	
			}
		}
		return false;
	}

}

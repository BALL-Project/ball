#include <BALL/MOLMEC/AMBER/GAFFTypeProcessor.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/QSAR/aromaticityProcessor.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/selector.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/STRUCTURE/assignBondOrderProcessor.h>

#include <queue>

//#define DEBUG
#undef DEBUG

namespace BALL
{
	const String GAFFTypeProcessor::Option::ATOMTYPE_FILENAME  = "atomtype_filename";
	const String GAFFTypeProcessor::Default::ATOMTYPE_FILENAME = "Amber/GAFFTypes.dat";
//	const String GAFFTypeProcessor::Default::ATOMTYPE_FILENAME = "Amber/AMBERTypes.dat";

	const String GAFFTypeProcessor::Option::GAFF_ATOMTYPE_POSTPROCESSING  = "gaff_atomtype_postprocessing";
	const bool   GAFFTypeProcessor::Default::GAFF_ATOMTYPE_POSTPROCESSING = true;

	GAFFTypeProcessor::GAFFTypeProcessor()
		: UnaryProcessor<Composite>()
	{
		options.setDefault(Option::ATOMTYPE_FILENAME, Default::ATOMTYPE_FILENAME);
		options.setDefault(Option::GAFF_ATOMTYPE_POSTPROCESSING, Default::GAFF_ATOMTYPE_POSTPROCESSING);
		parseAtomtypeTableFile_();
	}

	GAFFTypeProcessor::GAFFTypeProcessor(const Options& new_options)
		: UnaryProcessor<Composite>(),
			options(new_options)
	{
		options.setDefault(Option::ATOMTYPE_FILENAME, Default::ATOMTYPE_FILENAME);
		options.setDefault(Option::GAFF_ATOMTYPE_POSTPROCESSING, Default::GAFF_ATOMTYPE_POSTPROCESSING);
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
        if (RTTI::isKindOf<Molecule>(&composite))
		{
			Molecule *mol = RTTI::castTo<Molecule>(composite);

			precomputeBondProperties_(mol);

			precomputeAtomProperties_(mol);

			AtomIterator atom_it = mol->beginAtom();
			for ( ;+atom_it; ++atom_it)
			{
				try
				{
					assignAtomtype_(*atom_it);
				}
				catch(...)
				{
					Log.error()<< "GAFFTypeProcessor: Atom type could not be assigned!" << std::endl;
				}
			}	

			// decide whether we want post-processing of atom types
			if (options.getBool(Option::GAFF_ATOMTYPE_POSTPROCESSING))
			{
				postProcessAtomTypes_(mol);
			}
		}

		return Processor::CONTINUE;
	}

  std::set<String> GAFFTypeProcessor::getTypeNames() const
  {
    std::set<String> result;
    for (std::map<Position, std::vector<TypeDefinition> >::const_iterator type_it = atom_types_.begin();
         type_it != atom_types_.end();
         ++type_it)
    {
      std::vector<TypeDefinition> const& types = type_it->second;
      for (Position i=0; i<types.size(); ++i)
      {
        result.insert(types[i].atom_type);
      }
    }

    return result;
  }

	//read File with table of atomtypes and push atomtypes 
	//and their TypeDefinition in corresponding vector
	//and store a GAFFCESParser for every CESstring
	void GAFFTypeProcessor::parseAtomtypeTableFile_()
		throw(Exception::FileNotFound)
	{
		StringHashMap<GAFFCESParser*>::Iterator parser_it = ces_parsers_.begin();
		for (; parser_it != ces_parsers_.end(); ++parser_it)
			delete(parser_it->second);

		ces_parsers_.clear();

		std::ifstream atomfile;
		Path path;
		String filename = path.find(options.get(Option::ATOMTYPE_FILENAME));

		atomfile.open(filename.c_str());
		if (!atomfile)
		{
			Log.error() << "atomtype table could not be read!" << std::endl;
			throw(Exception::FileNotFound(__FILE__, __LINE__, filename));
		}
	
		String table_line;
		while (getline(atomfile,table_line))
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
			else
				// for our parser to work, we need to convert the GAFF 
				// wildcard * to a grammatically correct APS
				to_parse = "[*]";
			to_parse += typeDefinition.chemical_environment;

			parser_it = ces_parsers_.find(to_parse);
			if(parser_it == ces_parsers_.end()) {
				ces_parsers_.insert(to_parse, new GAFFCESParser(to_parse));
			}

			// insert the type definition at the corresponding position
			if (atom_types_.find(typeDefinition.atomic_number) == atom_types_.end())
			{
				std::vector<TypeDefinition> v;
				atom_types_[typeDefinition.atomic_number] = v;
			}
			atom_types_[typeDefinition.atomic_number].push_back(typeDefinition);
		}
	}

	// compute aromaticity, ring memberships, GAFF bond typization, ...
	void GAFFTypeProcessor::precomputeBondProperties_(Molecule* molecule)
	{
		current_molecule_ = molecule;
		RingPerceptionProcessor rpp;
		rpp.calculateSSSR(sssr_, *molecule);

		AromaticityProcessor arp;
		arp.options.setBool(AromaticityProcessor::Option::OVERWRITE_BOND_ORDERS, false);
		arp.aromatize(sssr_, *molecule);
	
		annotateBondTypes_();
	}

	// store connectivity, number of attached H-atoms and 
	// number of attached N,O,F,Cl and Br-atoms for every atom in molecule
	// NOTE: this function requires that precomputeBondProperties_ has been
	//       called previously
	void GAFFTypeProcessor::precomputeAtomProperties_(Molecule* molecule)
	{
		annotateRingSizes_();
		annotateAliphaticAndAromaticRingAtoms_();
		annotatePlanarRingAtoms_();

		AtomIterator atom_it = molecule->beginAtom();
		for ( ; +atom_it; ++atom_it)
		{
			int connectivity = (atom_it->countBonds());
 			atom_it->setProperty("connectivity", connectivity);

			int attached_hydrogens_int = 0;
			int electron_withdrawal_atoms_int = 0;
			String attached_hydrogens;
			String electron_withdrawal_atoms; 

			Atom::BondIterator bond_it = atom_it->beginBond();
			for ( ; +bond_it; ++bond_it)
			{

				const Atom& partner_atom = *(bond_it->getBoundAtom(*atom_it));
				if (partner_atom.getElement() == PTE[Element::H])
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

						if (  (partner_partner_atom.getElement()== PTE[Element::N])
								||(partner_partner_atom.getElement()== PTE[Element::O]) 
								||(partner_partner_atom.getElement()== PTE[Element::F]) 
								||(partner_partner_atom.getElement()== PTE[Element::Cl]) 
								||(partner_partner_atom.getElement()== PTE[Element::Br])
								||(partner_partner_atom.getElement()== PTE[Element::S]))
						{
							++electron_withdrawal_atoms_int;
						}
					}
				}
			} 

			String attached(attached_hydrogens_int);
			String electron(electron_withdrawal_atoms_int);

			attached_hydrogens = attached;
			electron_withdrawal_atoms = electron;

 			atom_it->setProperty("attached hydrogens", (String) attached_hydrogens);
 			atom_it->setProperty("electron withdrawal atoms", (String) electron_withdrawal_atoms);
		}
	}
	
	void GAFFTypeProcessor::annotateBondTypes_()
	{
		// store the old selection
		std::list<Atom*> old_atom_selection;
		std::list<Bond*> old_bond_selection;

		// NOTE: we store each bond twice, but this does not really matter...
		for (AtomIterator a_it = current_molecule_->beginAtom(); +a_it; ++a_it)
		{
			if (a_it->isSelected())
			{
				old_atom_selection.push_back(&*a_it);
			}
			for (Atom::BondIterator b_it = a_it->beginBond(); +b_it; ++b_it)
			{
				if (b_it->isSelected())
				{
					old_bond_selection.push_back(&*b_it);
				}
			}
		}

		current_molecule_->deselect();
		
		// find conjugated atoms:
		// select all carboxyl anions and nitro groups for delocalized bond types 
		Selector select("SMARTS([#16D1,#8D1]) AND SMARTS([#16D1,#8D1]~[*D3]~[#16D1,#8D1])");
		//						Selector select("SMARTS([#16D1,#8D1]) AND SMARTS([#16D1,#8D1]~*~[#16D1,#8D1]) AND (SMARTS(a) OR SMARTS(*=,#*-,=*=,#*) OR SMARTS([N,P,O,S]=,#*-[*;!H0]) OR SMARTS(*=,#*-[F,Cl,Br,I]) OR SMARTS(*=,#*-[N,P,O,S;!H0]))");
		current_molecule_->apply(select);

		// we know that the selected atoms only have one bond each. so we only need to make sure it really is a double bond
		list<Atom*> selected_atoms = select.getSelectedAtoms();
		list<Atom*>::iterator it = selected_atoms.begin();
		for ( ; it != selected_atoms.end(); ++it)
		{
			Atom::BondIterator bond_it = (*it)->beginBond();
			for( ; +bond_it; ++bond_it)
			{
				if (bond_it->getOrder() == Bond::ORDER__DOUBLE)
					bond_it->setProperty("GAFFBondType", DL);
			}
		}

		// restore the old selection
		current_molecule_->deselect();

		for (list<Atom*>::iterator al_it = old_atom_selection.begin(); al_it != old_atom_selection.end(); ++al_it)
			(*al_it)->select();
		for (list<Bond*>::iterator bl_it = old_bond_selection.begin(); bl_it != old_bond_selection.end(); ++bl_it)
			(*bl_it)->select();

		AtomIterator a_it = current_molecule_->beginAtom();
		Atom::BondIterator b_it;
		BALL_FOREACH_BOND(*current_molecule_, a_it, b_it)
		{
			// b_it is no delocalized bond 
			if (    !b_it->hasProperty("GAFFBondType") 
					 || (b_it->getProperty("GAFFBondType").getInt() != DL))
			{
				switch(b_it->getOrder())
				{
					case Bond::ORDER__SINGLE:
						if (b_it->isAromatic())
						{
							b_it->setProperty("GAFFBondType", sb);
						}
						else
						{
							b_it->setProperty("GAFFBondType",  SB);
						}
						break;
					case Bond::ORDER__DOUBLE:
						if (b_it->isAromatic())
						{
							b_it->setProperty("GAFFBondType",  db);
						}
						else
						{
							b_it->setProperty("GAFFBondType",  DB);
						}
						break;
					case Bond::ORDER__TRIPLE:
						b_it->setProperty("GAFFBondType", TB);
						break;
					case Bond::ORDER__AROMATIC:
						b_it->setProperty("GAFFBondType", AB);
						break;
				}
			}
		}
	}

	void GAFFTypeProcessor::annotateRingSizes_()
	{
		std::vector<std::vector<Atom* > >::iterator ring_it = sssr_.begin();
		for ( ; ring_it != sssr_.end(); ++ring_it)
		{
			String in_ring_property;
			String num_rings_property;

			// mark the number of occurence for the given feature
			int occurence = 0;
			bool in_ring = false;
			
			switch (ring_it->size())
			{
				case 3: in_ring_property = "In3Ring";
								num_rings_property = "NumberOf3Rings";
								occurence = 1;
								in_ring = true;
								break;
				case 4: in_ring_property = "In4Ring";
								num_rings_property = "NumberOf4Rings";
								occurence = 1;
								in_ring = true;
								break;
				case 5: in_ring_property = "In5Ring";
								num_rings_property = "NumberOf5Rings";
								occurence = 1;
								in_ring = true;
								break;
				case 6: in_ring_property = "In6Ring";
								num_rings_property = "NumberOf6Rings";
								occurence = 1;
								in_ring = true;
								break;
				case 7: in_ring_property = "In7Ring";
								num_rings_property = "NumberOf7Rings";
								occurence = 1;
								in_ring = true;
								break;
				case 8: in_ring_property = "In8Ring";
								num_rings_property = "NumberOf8Rings";
								occurence = 1;
								in_ring = true;
								break;
				case 9: in_ring_property = "In9Ring";
								num_rings_property = "NumberOf9Rings";
								occurence = 1;
								in_ring = true;
								break;
				default: in_ring = false;
			}

			// set property to current in ring for every atom
			// note: we count the occurence of the property within an atom
			std::vector<Atom*>::iterator atom_it = ring_it->begin();
			for ( ; atom_it != ring_it->end(); ++atom_it)
			{
				 occurence = ((*atom_it)->getProperty(num_rings_property).getInt()) + occurence;
				(*atom_it)->setProperty(in_ring_property, (bool) in_ring);
				(*atom_it)->setProperty(num_rings_property, (int) occurence);
			}
		}
	}

	// checks if current atom is in an aliphatic ringsystem,
	// which is made of sp3 Carbon or a purely aromatic six-membered ring
	void GAFFTypeProcessor::annotateAliphaticAndAromaticRingAtoms_()	
	{
		std::vector<std::vector<Atom* > >::iterator ring_it = sssr_.begin();
		for ( ; ring_it != sssr_.end(); ++ring_it)
		{
			bool purely_aliphatic = true;
			bool purely_aromatic  = true;
// 			bool has_sp3_carbon   = false;

			vector<Atom*>::iterator atom_it = ring_it->begin();
			for ( ; atom_it != ring_it->end(); ++atom_it)
			{
				//TODO why needed? has_sp3_carbon is just local and used nowhere else...
// 				if( ((*atom_it)->getElement() == PTE[Element::C]) && ((*atom_it)->countBonds() == 4))
// 					has_sp3_carbon = true;

				// if one ring member is not sp3 carbon, the whole thing isn't	
				if( ((*atom_it)->getElement() != PTE[Element::C]) || ((*atom_it)->countBonds() != 4))
				{
					purely_aliphatic = false;
				}

				// NOTE: antechamber defines purely aromatic only for 6-membered rings!
				if ( !(*atom_it)->getProperty("IsAromatic").getBool() || (ring_it->size() != 6))
				{
					purely_aromatic = false;
				}
			}

			for (atom_it = ring_it->begin(); atom_it != ring_it->end(); ++atom_it)
			{
				// do not overwrite the property if we have already detected a suitable ring!
				if (  !(*atom_it)->hasProperty("IsPureAliphatic")
            ||!(*atom_it)->getProperty("IsPureAliphatic").getBool())
					(*atom_it)->setProperty("IsPureAliphatic",(bool) purely_aliphatic);
				if (  !(*atom_it)->hasProperty("IsPureAromatic")
						||!(*atom_it)->getProperty("IsPureAromatic").getBool())
					(*atom_it)->setProperty("IsPureAromatic",(bool) purely_aromatic);
			}
		}
	}

	// checks if current atom is in a planar ringsystem
	void GAFFTypeProcessor::annotatePlanarRingAtoms_()
	{
		std::vector<std::vector<Atom* > >::iterator ring_it = sssr_.begin();
		for( ; ring_it != sssr_.end(); ++ring_it)
		{
			bool is_planar = true;
			bool has_db_to_non_ring = false;
			vector<Atom*>::iterator atom_it = ring_it->begin();
			for ( ; atom_it != ring_it->end(); ++atom_it)
			{
				// if one atom isn't planar, the whole ring isn't.
				if ( !planarAtom_(**atom_it) )
				{
					is_planar = false;
					break;
				}

				// is there one atom with a double bond to a non-ring?
				if (!has_db_to_non_ring) // otherwise, we don't need to look for another one!
				{
					Atom::BondConstIterator constBond_it = (*atom_it)->beginBond();
					for ( ; +constBond_it; ++constBond_it)
					{
						if (    (constBond_it->hasProperty("GAFFBondType")) 
								 && (    (constBond_it->getProperty("GAFFBondType").getInt() == DB)
								      || (constBond_it->getProperty("GAFFBondType").getInt() == db))
								)
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
				for (atom_it = ring_it->begin(); atom_it != ring_it->end(); ++atom_it)
				{
					(*atom_it)->setProperty("IsPlanarRingAtom", true);
					if (has_db_to_non_ring)
						(*atom_it)->setProperty("IsPlanarWithDBtoNR", true);
					else
						(*atom_it)->setProperty("IsPlanarWithDBtoNR", false);
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
		if (   (element == PTE[Element::C]) 
				&& ((num_bonds == 2) || (num_bonds == 3)))
			return true;
		// N(X2) or N(X3)
		if (   (element == PTE[Element::N]) 
				&& ((num_bonds == 2) || (num_bonds == 3)))
			return true;
		// P(X2) or P(X3)
		if (   (element == PTE[Element::P]) 
				&& ((num_bonds == 2) || (num_bonds == 3)))
			return true;
		// O(X2)
		if (   (element == PTE[Element::O])
			 	&& (num_bonds == 2))
			return true;
		// S(X2)
		if (   (element == PTE[Element::S]) 
				&& (num_bonds == 2))
			return true;		

		return false;
	}	

	bool GAFFTypeProcessor::assignAtomtype_(Atom& atom)
	{
		// can we match this atom at all?
		if (atom_types_.find(atom.getElement().getAtomicNumber()) == atom_types_.end())
		{
			Log.error() << "GAFFTypeProcessor: could not assign atom type for " << atom.getFullName() << std::endl;
			Log.error() << "                   Reason: no type definition for atomic number " << atom.getElement().getAtomicNumber() << " available!" << std::endl;
			
			return false;
		}

		std::vector<TypeDefinition>& type_defs = atom_types_[atom.getElement().getAtomicNumber()];
		for (Position i=0; i<type_defs.size(); i++)
		{
			TypeDefinition& typeDefinition = type_defs[i];
#ifdef DEBUG
			Log.info() << "GAFFTypeProcessor: match atom " << atom.getFullName() << " against type " << typeDefinition.atom_type << std::endl;
			Log.info() << "GAFFTypeProcessor: connectivity is " << atom.getProperty("connectivity").getInt() << " but should be " << typeDefinition.connectivity
								 << std::endl;
#endif

			// all fields with "*" are invalid and therefore considered as True
			if (   (typeDefinition.connectivity < 0) 
					 ||(atom.getProperty("connectivity").getInt() == typeDefinition.connectivity))
			{
#ifdef DEBUG
				Log.info() << "GAFFTypeProcessor: number of attached hydrogens is " << atom.getProperty("attached hydrogens").getString() 
									 << " but should be " << typeDefinition.attached_hydrogens
									 << std::endl;
#endif
				if (	 (atom.getProperty("attached hydrogens").getString() == typeDefinition.attached_hydrogens) 
						 ||(typeDefinition.attached_hydrogens == "*"))
				{
#ifdef DEBUG
				Log.info() << "GAFFTypeProcessor: number of electron withdrawal atoms is " << atom.getProperty("electron withdrawal atoms").getString() 
									 << " should be " << typeDefinition.electron_withdrawal_atoms
									 << std::endl;
#endif
					if (	 (atom.getProperty("electron withdrawal atoms").getString() == typeDefinition.electron_withdrawal_atoms) 
							 ||(typeDefinition.electron_withdrawal_atoms == "*"))
					{
						String atomic_property = typeDefinition.atomic_property;

						// add aps string to ces string for parsing
						String to_match = "";
						if (typeDefinition.atomic_property != "*")
						{	
							to_match = typeDefinition.atomic_property;
						}
						else 
						{
							// for our parser to work, we need to convert the GAFF 
							// wildcard * to a grammatically correct APS
							to_match = "[*]";
						}	
						to_match += typeDefinition.chemical_environment;

#ifdef DEBUG
						Log.info() << "GAFFTypeProcessor: combined APS/CES to match is " << to_match << std::endl;
#endif

						if (	 (ces_parsers_.find(to_match) != ces_parsers_.end())
								&& (ces_parsers_[to_match]->match(atom)))
						{
#ifdef DEBUG
							Log.info() << "atom name: " << atom.getName() << " atomtype:" << typeDefinition.atom_type << endl;
#endif
							atom.setProperty("atomtype", typeDefinition.atom_type);
							return true;	
						}
					}
				}	
			}
		}

		// if no type could be assigned return false
		Log.error() << "GAFFTypeProcessor: could not assing a type for atom " << atom.getFullName() 
								<< "! Setting type to DU" << std::endl;
		atom.setProperty("atomtype", String("DU")); //ANNE 

		return false;
	}

	void GAFFTypeProcessor::postProcessAtomTypes_(Molecule* molecule)
	{
		// first the code that corresponds to atadjust() in antechamber/atomtype.c

		// what we try to do is the following: for each pair of conjugated atoms of particular atom types,
		// try to achieve that the types differ along double bonds and are identical along single bonds.
		//
		// To this end, we will do a breadth-first search
		std::queue<Atom*> search_queue;
		std::map<Atom*, Index> new_type;

		for (AtomIterator at_it = molecule->beginAtom(); +at_it; ++at_it)
		{
			for (Atom::BondIterator bond_it = at_it->beginBond(); +bond_it; ++bond_it)
			{
				const String& atomtype_first  = at_it->getProperty("atomtype").getString();
				const String& atomtype_second = bond_it->getPartner(*at_it)->getProperty("atomtype").getString();
				if (	(  	 (atomtype_first == "cc") || (atomtype_first == "ce") || (atomtype_first == "cg") 
						 		|| (atomtype_first == "pc") || (atomtype_first == "pe") || (atomtype_first == "nc") 
						 		|| (atomtype_first == "ne") )
					  &&(  	 (atomtype_second == "cc") || (atomtype_second == "ce") || (atomtype_second == "cg") 
						 		|| (atomtype_second == "pc") || (atomtype_second == "pe") || (atomtype_second == "nc") 
						 		|| (atomtype_second == "ne") ) )
				{
					new_type[&*at_it] = 0;
					new_type[bond_it->getPartner(*at_it)] = 0;
				}
			}
		}

	
		Size number_to_cleanup = new_type.size();

		// do we still have atom types to fix?
		while (number_to_cleanup > 0)
		{
			// start the search at the first non-fixed atom
			std::map<Atom*, Index>::iterator map_it = new_type.begin();
			while ((map_it != new_type.end() && (map_it->second != 0)))
				++map_it;

			search_queue.push(map_it->first);

			// and set its value fixed to 1
			map_it->second = 1;
			// one atom has now been fixed
			--number_to_cleanup;

			while (!search_queue.empty())
			{
				Atom* current_atom = search_queue.front();
				search_queue.pop();

				// iterate over current_atom's children
				for (Atom::BondIterator bond_it = current_atom->beginBond(); +bond_it; ++bond_it)
				{
					// is this one of the interesting atoms?
					Atom* child = bond_it->getPartner(*current_atom);
					if (new_type.find(child) != new_type.end())
					{
						// have we seen it before?
						if (new_type[child] == 0)
						{
							// nope => set its type and insert it into the queue
							if (!bond_it->hasProperty("GAFFBondType"))
							{
								Log.error() << "GAFFTypeProcessor::postProcessAtomTypes_: missing bond type information! aborting!" << std::endl;
								return;
							}

							Index bond_type = bond_it->getProperty("GAFFBondType").getInt();

							if ( (bond_type == SB) || (bond_type == sb) )
							{
								// this is a single or aromatic single bond => propagate the same type as the parent atom
								new_type[child] = new_type[current_atom];
								--number_to_cleanup;
							}
							else if ( 	 (bond_type == DB) 
												|| (bond_type == db) 
												|| (bond_type == TB) )
							{
								// this is a double, aromatic double, or triple bond => invert the type
								new_type[child] = -new_type[current_atom];
								--number_to_cleanup;
							}
							else
							{
								Log.error() << "GAFFTypeProcessor::postProcessAtomTypes_: delocalized or conjugated bond not allowed for this atom type! aborting!" << std::endl;
								return;
							}

							search_queue.push(child);
						}
					}
				}
			}
		}

		// now compute the new types for our atoms
		std::map<Atom*, Index>::iterator map_it = new_type.begin();
		for (; map_it != new_type.end(); ++map_it)
		{
			if (map_it->second == -1)
			{
				// we compute the name of the new type by increasing the second letter by one position in the alphabet (e.g. cc => cd)
				String new_atom_type = map_it->first->getProperty("atomtype").getString();
				new_atom_type[1]+=1;

				map_it->first->setProperty("atomtype", new_atom_type);
			}
		}

		// now the code that corresponds to cpadjust() in antechamber/atomtype.c

		// This is nearly equivalent to atadjust, just for cp only and with slightly different propagation rules
		new_type.clear();

		for (AtomIterator at_it = molecule->beginAtom(); +at_it; ++at_it)
		{
			for (Atom::BondIterator bond_it = at_it->beginBond(); +bond_it; ++bond_it)
			{
				const String& atomtype_first  = at_it->getProperty("atomtype").getString();
				const String& atomtype_second = bond_it->getPartner(*at_it)->getProperty("atomtype").getString();

				if (   (atomtype_first  == "cp")
					  && (atomtype_second == "cp") )
				{
					new_type[&*at_it] = 0;
					new_type[bond_it->getPartner(*at_it)] = 0;
				}
			}
		}

	
		number_to_cleanup = new_type.size();

		// do we still have atom types to fix?
		while (number_to_cleanup > 0)
		{
			// start the search at the first non-fixed atom
			std::map<Atom*, Index>::iterator map_it = new_type.begin();
			while ((map_it != new_type.end() && (map_it->second != 0)))
				++map_it;

			search_queue.push(map_it->first);

			// and set its value fixed to 1
			map_it->second = 1;
			// one atom has now been fixed
			--number_to_cleanup;

			while (!search_queue.empty())
			{
				Atom* current_atom = search_queue.front();
				search_queue.pop();

				// iterate over current_atom's children
				for (Atom::BondIterator bond_it = current_atom->beginBond(); +bond_it; ++bond_it)
				{
					// is this one of the interesting atoms?
					Atom* child = bond_it->getPartner(*current_atom);
					if (new_type.find(child) != new_type.end())
					{
						// have we seen it before?
						if (new_type[child] == 0)
						{
							// nope => set its type and insert it into the queue
							if (!bond_it->hasProperty("GAFFBondType"))
							{
								Log.error() << "GAFFTypeProcessor::postProcessAtomTypes_: missing bond type information! aborting!" << std::endl;
								return;
							}

							Index bond_type = bond_it->getProperty("GAFFBondType").getInt();

							if (bond_type == SB)
							{
								// this is a single bond => propagate the same type as the parent atom
								new_type[child] = new_type[current_atom];
								--number_to_cleanup;
							}
							else 
							{
								// this is anything but a single bond => invert the type
								new_type[child] = -new_type[current_atom];
								--number_to_cleanup;
							}

							search_queue.push(child);
						}
					}
				}
			}
		}

		// now compute the new types for our atoms
		map_it = new_type.begin();
		for (; map_it != new_type.end(); ++map_it)
		{
			if (map_it->second == -1)
			{
				// we compute the name of the new type by increasing the second letter by one position in the alphabet (e.g. cc => cd)
				String new_atom_type = map_it->first->getProperty("atomtype").getString();
				new_atom_type[1]+=1;

				map_it->first->setProperty("atomtype", new_atom_type);
			}
		}

	}
}

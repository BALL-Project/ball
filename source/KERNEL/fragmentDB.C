// $Id: fragmentDB.C,v 1.3 1999/09/17 06:41:17 oliver Exp $

#include <BALL/KERNEL/fragmentDB.h>


#include <list>
#include <vector>

#include <BALL/KERNEL/PSE.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
	
/*			Things still missing (among others)
				===================================
				- check for unique atom names
				- add hydrogen processor modifications: removal of (addh ..) entries from database
				- create bond processor
				- saving of fragment databases
				- dynamic import of databases
				- check for completeness of a residue
				- hydrogen optimization using a simple force field
				- standard-path for fragment databases
*/

#define FRAGMENT_DB_INCLUDE_TAG "#include:"

namespace BALL 
{

	FragmentDB::NoFragmentNode::NoFragmentNode
		(const char* file, int line, const string& filename)
		: Exception::GeneralException(file, line, "NoFragmentNode", 
											 "the resource database does not contain a valid Fragment entry"),
			filename_(filename)
	{
	}

	void FragmentDB::expandTree_(ResourceEntry& root_entry, int depth)
	{
		while (expandFirst_(root_entry, depth));
	}

	bool FragmentDB::expandFirst_(ResourceEntry& root_entry, int depth)
	{
		if (!root_entry.getKey().hasPrefix(FRAGMENT_DB_INCLUDE_TAG))
		{
			ResourceEntry::Iterator	entry_iterator;

			bool expanded_one = false;

			for (entry_iterator = ++root_entry.begin(); +entry_iterator && !expanded_one; ++entry_iterator)
			{
				if (depth > 0)
				{
					if (expandFirst_(*entry_iterator, depth - 1))
					{
						return true;
					}
				}
			}
					
			return false;

		} else {

			String	key = root_entry.getKey();
			String	value = root_entry.getValue();
			String	key_fields[2], value_fields[2];

			if (key.countFields(":") != 2)
			{
				// if the include directive is invalid,
				// remove the entry
				Log.level(LogStream::ERROR) << "illegal #include directive: " << key << endl;
				root_entry.getParent()->removeChild(key, 0);

				return false;

			} else {

				key.split(key_fields, 2, ":");
				value.split(value_fields, 2, ":");
					
				ResourceEntry*	parent = root_entry.getParent();
				ResourceEntry* entry;
				parent->removeChild(key, 0);

				ResourceFile*	file;
				ResourceEntry* tree;
					
				file = new ResourceFile(value_fields[0]);
				if ((file == 0) || !file->isValid())
				{
					Log.level(LogStream::ERROR) << "error opening include file: " << value_fields[0] << endl;
					delete file;
					return false;
				} else {
					tree = file->getRoot().getEntry(value_fields[1]);
					if (tree == 0){
						Log.level(LogStream::ERROR) << "cannot find node " << value_fields[1] << " in file " << value_fields[0] << endl;
					} else {
						entry = parent->insertChild(key_fields[1], tree->getValue());
						entry->mergeChildrenOf(*tree);
					}
					delete file;
					
				}
			}
		}

		return true;
	}

	// default constructor
	FragmentDB::FragmentDB()
	{
		// search for the standard fragment DB file
		Path path;
		filename_ = path.find("fragments/Fragment.db");
		
		if (filename_ == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, "fragments/Fragment.db");
		}
		valid_ = true;

		normalizeNames = new NormalizeNamesProcessor(*this);
		addHydrogens = new AddHydrogensProcessor(*this);
		buildBonds = new BuildBondsProcessor(*this);

		tree = 0;
	}


	FragmentDB::FragmentDB(const String& filename)
	{
		setFilename(filename);

		init();

		normalizeNames = new NormalizeNamesProcessor(*this);
		addHydrogens = new AddHydrogensProcessor(*this);
		buildBonds = new BuildBondsProcessor(*this);
	}

	FragmentDB::FragmentDB(const FragmentDB& db, bool /* deep */)
	{
		destroy();
		filename_ = db.getFilename();
		valid_ = db.isValid();
		init();
	}

	FragmentDB::~FragmentDB()
	{
		destroy();
	}

	void FragmentDB::destroy()
	{
		valid_ = false;
		filename_ = "";

		delete tree;

		name_to_path_.destroy();
		name_to_frag_pointer_.destroy();
		name_to_variants_.destroy();
		fragments_.clear();
		standards_.destroy();
	}

	void FragmentDB::setFilename(const String& filename)
	{
		// search for the standard fragment DB file
		Path path;
		filename_ = path.find(filename);
		
		if (filename_ == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename);
		}
		filename_ = filename;
	}

	const String& FragmentDB::getFilename() const 
	{
		return filename_;
	}

	bool FragmentDB::isValid() const 
	{
		return valid_;
	}

	bool FragmentDB::has(const String& fragment_name) const 
	{
		if (!isValid())
			return false;

		return name_to_path_.has(fragment_name);
	}

	FragmentDB::Type FragmentDB::getFragmentType(const String& fragment_name) const 
	{
		ResourceEntry*	entry;
		String					path;
					
		if (!isValid() || !tree->isValid())
		{
			return FragmentDB::TYPE__UNKNOWN;
		}

		if (!has(fragment_name))
		{
			return FragmentDB::TYPE__UNKNOWN;
		}
		
		path = (*name_to_path_.find(fragment_name)).second;
		path += "/Type";
		entry = tree->findChild(path);
		entry = tree->findChild("");
			
		if (entry != 0)
		{
			if (entry->getValue() == "residue")
			{
				return FragmentDB::TYPE__RESIDUE;
			}

			if (entry->getValue() == "molecule")
			{
				return FragmentDB::TYPE__MOLECULE;
			}

			if (entry->getValue() == "fragment")
			{
				return FragmentDB::TYPE__MOLECULE;
			}
		}

		return FragmentDB::TYPE__UNKNOWN;
	}

	void FragmentDB::parseAtoms_(ResourceEntry& entry, Fragment& fragment)
	{

		ResourceEntry::Iterator	entry_iterator;

		for (entry_iterator = ++entry.begin(); +entry_iterator; ++entry_iterator)
		{
			if ((*entry_iterator).getDepth() == entry.getDepth() + 1)
			{
				if ((*entry_iterator).getValue().countFields(" ") != 4)
				{
					Log.level(LogStream::ERROR) << "Wrong entry for atom " << (*entry_iterator).getKey() 
							 << ": " << (*entry_iterator).getValue() << endl;
				} else {
					// create a new atom...
					Atom*	 atom;
					atom = new Atom;
								
					// ...set its name and insert it into the fragment.
					atom->setName((*entry_iterator).getKey());
					fragment.insert(*atom);
		
					// now extract element and position (x, y, z) from the string
					String		s[4];
					(*entry_iterator).getValue().split(s, BALL_SIZEOF_ARRAY(s), " ");
		
					Vector3				r;										
					r.set(s[1].toFloat(), s[2].toFloat(), s[3].toFloat());
		
					// and assign its values to the atom
					atom->setPosition(r);
					atom->setElement(PSE.getElement(s[0]));
				}
			}
		}
	}

	void FragmentDB::parseBonds_(ResourceEntry& entry, Fragment& fragment)
	{

		ResourceEntry::Iterator	entry_iterator;

		for (entry_iterator = ++entry.begin(); +entry_iterator; ++entry_iterator)
		{
			if ((*entry_iterator).getDepth() == entry.getDepth() + 1)
			{
				// check whether the fragment contains both bonds
				Atom*	atom1 = 0;
				Atom*	atom2 = 0;
				AtomIterator	atom_iterator;
				
				// first field contains a serial number, field 2 the first atom, field 3 the second atom
				// and the third field (optional) the bond type (s[ingle], d[ouble], a[romatic])
				String fields[3];
				entry_iterator->getValue().split(fields, 3);
		
				for (atom_iterator = fragment.beginAtom(); +atom_iterator; ++atom_iterator)
				{
					if (atom_iterator->getName() == fields[0])
					{
						atom1 = &(*atom_iterator);
					}
					if (atom_iterator->getName() == fields[1])
					{
						atom2 = &(*atom_iterator);
					}
				}
		
				if ((atom1 == 0) || (atom2 == 0))
				{
					// if at least on of the atoms doesn`t exist: complain about it
					Log.level(LogStream::ERROR) << "Bond to a non-existing atom: " 
																			<< fields[0] << "-"
																			<< fields[1] << endl;
				} else {
					// otherwise create the bond, if valences free
					if ((atom1->countBonds() > 7) || (atom2->countBonds() > 7))
					{
						Log.level(LogStream::ERROR) << "Too many bonds - cannot create bond: " 
																				<< atom1->getName() << "-" << atom2->getName()
																				<< " in fragment " << fragment.getName() << endl;
					} else {
						atom1->createBond(*atom2);
					}
				}
			}
		}
	}


	void FragmentDB::parseDelete_(ResourceEntry& entry, Fragment& fragment)
	{

		ResourceEntry::Iterator	entry_iterator;

		for (entry_iterator = ++entry.begin(); +entry_iterator; ++entry_iterator)
		{
			if ((*entry_iterator).getDepth() == entry.getDepth() + 1)
			{
				// check whether the fragment contains both bonds
				AtomIterator	atom_iterator;
				Atom*				atom = 0;
		
				for (atom_iterator = fragment.beginAtom(); +atom_iterator; ++atom_iterator)
				{
					if ((*atom_iterator).getName() == (*entry_iterator).getKey())
						atom = &(*atom_iterator);
				}
		
				if (atom == 0)
				{
					// if the atom to be deleted doesn`t exist - complain about it!
					Log.level(LogStream::ERROR) << "Cannot delete non-existing atom: "
																			<< (*entry_iterator).getKey() << endl;
				} else {
					// otherwise delete the atom
					fragment.remove(*atom);
					delete atom;
				}
			}
		}
	}


	void FragmentDB::parseRename_(ResourceEntry& entry, Fragment& fragment)
	{

		ResourceEntry::Iterator	entry_iterator;

		for (entry_iterator = ++entry.begin(); +entry_iterator; ++entry_iterator)
		{
			if ((*entry_iterator).getDepth() == entry.getDepth() + 1)
			{
				// check whether the fragment contains both bonds
				AtomIterator	atom_iterator;
				Atom*				atom = 0;
		
				for (atom_iterator = fragment.beginAtom(); +atom_iterator; ++atom_iterator)
				{
					if ((*atom_iterator).getName() == (*entry_iterator).getKey())
						atom = &(*atom_iterator);
				}
		
				if (atom == 0)
				{
					// if the atom to be renamed doesn`t exist - complain about it!
					Log.level(LogStream::ERROR) << "Cannot rename non-existing atom: "
																			<< (*entry_iterator).getKey() << endl;
				} else {
					// otherwise rename the atom
					atom->setName((*entry_iterator).getValue());
				}
			}
		}
	}

	void FragmentDB::parseProperties_(ResourceEntry&  entry, PropertyManager& property_man)
	{
		ResourceEntry::Iterator	entry_iterator;
		String property;
		bool invert = false;

		for (entry_iterator = ++entry.begin(); +entry_iterator; ++entry_iterator)
		{
			if ((*entry_iterator).getDepth() == entry.getDepth() + 1)
			{
				// check for the most important properties: all those defined
				// in Residue::properties
				property = (*entry_iterator).getKey();
				property.toUpper();
				if (property[0] == '!')
				{
					property.erase(0, 1);
					invert = true;
				} else {
					invert = false;
				}
				

				Property prop = -1;
				if (property == "NON_STANDARD")
				{
					prop = Residue::PROPERTY__NON_STANDARD;
				}
				else if (property == "AMINO_ACID")
				{
					prop = Residue::PROPERTY__AMINO_ACID;
				}
				else if (property == "WATER")
				{
					prop = Residue::PROPERTY__WATER;
				}
				else if (property == "HAS_SSBOND")
				{
					prop = Residue::PROPERTY__HAS_SSBOND;
				}
				else if (property == "C_TERMINAL")
				{
					prop = Residue::PROPERTY__C_TERMINAL;
				}
				else if (property == "N_TERMINAL")
				{
					prop = Residue::PROPERTY__N_TERMINAL;
				}
				else 
				{
				}

				if (prop == -1)
				{
					// if the property was not recognized,
					// store it as a name-value pair
					if (invert)
					{
						property_man.clearProperty(property.c_str());
					} else {
						property_man.setProperty(property.c_str());
					}
				} else {
					if (invert)
					{
						property_man.clearProperty(prop);
					} else {
						property_man.setProperty(prop);
					}
				}
			}
		}
		
	}

		
	void FragmentDB::init()
	{
		// we are invalid until we're sure we're not...
		valid_ = false;

		// try to open the main resource file
		ResourceFile* resource_db = new ResourceFile(filename_);

		// check for success and terminate on failure
		if (!resource_db->isValid())
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename_);
		}


		// copy the contents of the resource file into a tree
		tree = new ResourceEntry();			
					

		tree->mergeChildrenOf(resource_db->getRoot());
					

		// close the main resource file
		resource_db->close();
		delete resource_db;

		expandTree_(*tree);

		// search for the "Fragments" entry in the main resource file
		ResourceEntry*	entry;
		entry = tree->getRoot().findChild("Fragments");
		if (entry == 0)
		{
			valid_ = false;
			delete tree;

			// terminate with an exception
			throw NoFragmentNode(__FILE__, __LINE__, filename_);
		}
		
		ResourceEntry::Iterator	frag_entry_iterator;
		ResourceEntry::Iterator	entry_iterator;
		for (frag_entry_iterator = ++(tree->getEntry("/Fragments")->begin()); 
				 +frag_entry_iterator; ++frag_entry_iterator)
		{
			if ((*frag_entry_iterator).getDepth() == 2)
			{
				// create a new fragment and assign its name
				// 
				Residue*				fragment;

				fragment = new Residue;
				fragment->setName((*frag_entry_iterator).getKey());

				String fragment_name = (*frag_entry_iterator).getKey();
						
				// insert the fragment name into the corresponding lists
				fragments_.push_back(fragment);
				name_to_path_[fragment_name] = "/Fragments/" + fragment_name;
		
				// if there are no atoms in the database, something went wrong
				entry = (*frag_entry_iterator).getEntry("Atoms");
				if (entry == 0)
				{
					Log.level(LogStream::ERROR) << "Cannot find Atoms entry for " 
						<< fragment_name << endl;
					return;
				} else {
					parseAtoms_(*entry, *fragment);
				}

				// now find all the bonds for the fragment and create them
				// Fragments without bonds are legal, so we don`t complain but
				// continue
				entry = (*frag_entry_iterator).getEntry("Bonds");
				if (entry != 0)
				{
					parseBonds_(*entry, *fragment);
				}

				// now check for properties common to all variants of this
				// fragment (usually AMINO_ACID)
				// Each variant entry may also contain additional properties
				// or reset properties by specifying a "!" in front of the property
				// name
				entry = (*frag_entry_iterator).getEntry("Properties");
				if (entry != 0)
				{
					parseProperties_(*entry, *dynamic_cast<PropertyManager*>(fragment));
				}

				// check for all aliases (given in the Names section of the db-file)
				// and insert them into the corresponding hash maps

				ResourceEntry::Iterator			entry_iterator;

				entry = (*frag_entry_iterator).getEntry("Names");
				if (entry != 0)
				{
					String path = "/Fragments/" + fragment_name;
					for (entry_iterator = ++entry->begin(); +entry_iterator; ++entry_iterator)
					{
						name_to_path_[(*entry_iterator).getKey()] = path;
						name_to_frag_pointer_[(*entry_iterator).getKey()] = fragment;
					}
				}

				// check for possible variants of this residue type
				// (keyword Variants)
				entry = (*frag_entry_iterator).getEntry("Variants");
				if (entry != 0)
				{
					ResourceEntry::Iterator variant_iterator;

					Residue* original_fragment = new Residue(*fragment);

					for (variant_iterator = ++entry->begin(); +variant_iterator; ++variant_iterator)
					{	
						if ((*variant_iterator).getDepth() == entry->getDepth() + 1)
						{
							String variant_name = variant_iterator->getKey();
							Residue*	variant;
							if (variant_name == "Default")
							{
								variant = new Residue(*original_fragment);
									
								// make sure that the default fragment is the first in the list
								// just to ensure it is selected, if other fragments fit
								// equally well from their properties
								name_to_variants_[fragment_name].push_front(variant);
								name_to_frag_pointer_[fragment_name] = variant;
								name_to_path_[fragment_name] = "/Fragments/" 
									+ fragment_name + "/Variants/" + variant_name;
							} else {
								variant = new Residue(*original_fragment);
								variant->setName(variant_name);

								fragments_.push_back(variant);
								name_to_frag_pointer_[variant_name] = variant;
								name_to_variants_[fragment_name].push_back(variant);
								name_to_path_[variant_name] = "/Fragments/" 
									+ fragment_name + "/Variants/" + variant_name;
							}

							// remember all variants of a certain fragment in a list
							// This list is accessed via a hash map. It is needed to 
							// determine the correct variant from given properties
							// (see getReferenceFragment(Fragment&), parseProperties_)
							for (entry_iterator = (*variant_iterator).begin(); +entry_iterator; ++entry_iterator)
							{
								if ((*entry_iterator).getDepth() == entry->getDepth() + 2)
								{
									if ((*entry_iterator).getKey() == "Atoms")
									{
										parseAtoms_(*entry_iterator, *variant);
									}
									if ((*entry_iterator).getKey() == "Bonds")
									{
										parseBonds_(*entry_iterator, *variant);
									}
									if ((*entry_iterator).getKey() == "Rename")
									{
										parseRename_(*entry_iterator, *variant);
									}
									if ((*entry_iterator).getKey() == "Delete")
									{
										parseDelete_(*entry_iterator, *variant);
									}
									if ((*entry_iterator).getKey() == "Properties")
									{
										parseProperties_(*entry_iterator, *dynamic_cast<PropertyManager*>(variant));
									}
								}
							}
						}
					}
				}
			}
		}

		// check for entries concerning naming standards
		entry = tree->getEntry("/Names");
		if (entry != 0){
			for (entry_iterator = ++entry->begin(); +entry_iterator; ++entry_iterator){
				if ((*entry_iterator).getDepth() == 2){

						StringHashMap<String>*	name_map_to;
						StringHashMap<String>*	name_map_from;
						name_map_to = new StringHashMap<String>;
						name_map_from = new StringHashMap<String>;
						standards_[(*entry_iterator).getKey() + "-" + (*entry_iterator).getValue()] = name_map_to;
						standards_[(*entry_iterator).getValue() + "-" + (*entry_iterator).getKey()] = name_map_from;
						
						ResourceEntry::Iterator	alias_iterator;
						for (alias_iterator = ++(*entry_iterator).begin(); +alias_iterator; ++alias_iterator)
						{
							(*name_map_to)[(*alias_iterator).getKey()] = (*alias_iterator).getValue();
							(*name_map_from)[(*alias_iterator).getValue()] = (*alias_iterator).getKey();
						}
				}
			}
		}
				

		// check for a default naming standard
		entry = tree->getEntry("/Defaults/Naming");
		if (entry == 0){
			default_standard_ = "PDB";
		} else {
			default_standard_ = entry->getValue();
		}
		
		// OK. Everything went well, so we might consider ourselves as valid.
		valid_ = true;
		return;
	}

	const String& FragmentDB::getDefaultNamingStandard() const 
	{
		return default_standard_;
	}
					
	const Fragment* FragmentDB::getFragment(const String& fragment_name) const
	{
		if (name_to_frag_pointer_.has(fragment_name)){
			return (*name_to_frag_pointer_.find(fragment_name)).second;
		} else {
			return 0;		
		}
	}

	const Fragment* FragmentDB::getReferenceFragment(const Fragment& fragment) const
	{
		// if there are no variants, return the default fragment
		String s(fragment.getName());
		if (!name_to_variants_.has(s))
		{
			return 0;
		}
		if (name_to_variants_[fragment.getName()].size() < 2)
		{
			return getFragment(s);
		}

		// now find the variant that best matches the fragment
		// This returns N/C terminal variants for fragments
		// that have the corresponding properties set or 
		// cystein variants without thiol hydrogen if the
		// disulphide bond property is set
		
		// first, check for two special properties of amino acids:
		// C_TERMINAL and N_TERMINAL 
		// They are usually not set, so set them here
		// As the fragment should be const, we store the properties
		// in a bit vector and OR them later with the fragment's properties
		BitVector	additional_properties;
		if (RTTI<Residue>::isKindOf(fragment))
		{
			const Residue*	residue = RTTI<const Residue>::castTo(fragment);
			if (residue->isCTerminal())
			{
				additional_properties.setBit(Residue::PROPERTY__C_TERMINAL);
			}
			if (residue->isNTerminal())
			{
				additional_properties.setBit(Residue::PROPERTY__N_TERMINAL);
			}
		}

		Fragment* variant = 0;
		// the number of properties that matched.
		// the fragment with the largest number of matched
		// properties is returned
		int number_of_properties = -1;
		int property_difference = -1;
		int best_number_of_properties = -1;
		int best_property_difference = 10000;

		// iterate over all variants and compare properties
		list<Residue*> variant_list = name_to_variants_[fragment.getName()];
		list<Residue*>::const_iterator it = variant_list.begin();
		for (; it != variant_list.end(); ++it)
		{
			// determine how many properties both have in common
			// by ANDing both bitvectors and counting ones
			BitVector props = fragment.getBitVector();
			props |= additional_properties;
			property_difference = (int)abs((int)props.countValue(true) - (int)(*it)->getBitVector().countValue(true));
			props &= (*it)->getBitVector();
			number_of_properties = (int)props.countValue(true);

			if ((number_of_properties > best_number_of_properties)
					|| ((number_of_properties == best_number_of_properties) && (property_difference < best_property_difference)))
			{
				variant = (*it);
				best_number_of_properties = number_of_properties;
				best_property_difference = property_difference;
			}
		}

		return variant;
	}

	const Residue* FragmentDB::getResidue(const String& fragment_name) const 
	{
		if (name_to_frag_pointer_.has(fragment_name)){
			return (*name_to_frag_pointer_.find(fragment_name)).second;
		} else {
			return 0;		
		}
	}

	FragmentDB::NormalizeNamesProcessor::NormalizeNamesProcessor() 
		: UnaryProcessor<Fragment>() 
	{
		fragment_db_ = 0;
		naming_standard_ = "";
	}

	FragmentDB::NormalizeNamesProcessor::NormalizeNamesProcessor(FragmentDB& db)
	{
		setFragmentDB(db);
	}

	FragmentDB::NormalizeNamesProcessor::~NormalizeNamesProcessor()
	{
	}

	void FragmentDB::NormalizeNamesProcessor::setFragmentDB(FragmentDB& db)
	{
		fragment_db_ = &db;
		setNamingStandard(db.getDefaultNamingStandard());
	}

	void FragmentDB::NormalizeNamesProcessor::setNamingStandard(const String& naming_standard) 
	{
		naming_standard_ = naming_standard;
	}

	const String& FragmentDB::NormalizeNamesProcessor::getNamingStandard() 
	{
		return naming_standard_;
	}

	StringHashMap<StringHashMap<String>*>& FragmentDB::getNamingStandards() 
	{
		return standards_;
	}

	Processor::Result FragmentDB::NormalizeNamesProcessor::operator () (Fragment& fragment) 
	{
		fragments_.push_back(&fragment);

		return Processor::CONTINUE;
	}

	bool FragmentDB::NormalizeNamesProcessor::start() 
	{
		fragments_.clear();
		return true;
	}



	// match an RES/ATOM pair in a map
	bool FragmentDB::NormalizeNamesProcessor::matchName(String&	res_name, 
																											String&	atom_name,
																											const StringHashMap<String>*	map) const
	{

		String	match_name;	
		String	s[2];
		String	r_name;	// residue name (non const)
		String	a_name; // atom name (non const)
		bool		hit = false;

		r_name = res_name;
		r_name.trim();
		match_name = r_name + ":*";
		if (map->has(match_name)) 
		{
			(*map->find(match_name)).second.split(s, 2, ":");
			r_name = s[0];
		}

		a_name = atom_name;
		a_name.trim();

		// first, try to match exactly
		match_name = r_name + ":" + a_name;
		if (map->has(match_name))
		{
			(*map->find(match_name)).second.split(s, 2, ":");
			a_name = s[1];
			r_name = s[0];
			hit = true;
		} else {
			// second, try wildcard match for residue names
			match_name = "*:" + a_name;
			if (map->has(match_name)){
				(*map->find(match_name)).second.split(s, 2, ":");
				a_name = s[1];
				hit = true;
			}
		}


		res_name = r_name;
		atom_name = a_name;
		return hit;
	}

	bool FragmentDB::NormalizeNamesProcessor::finish() 
	{
		String		map_name = "-" + naming_standard_;

		StringHashMap<StringHashMap<String>*>	table;
		table = fragment_db_->getNamingStandards();

		StringHashMap<StringHashMap<String>*>::iterator		it;

		StringHashMap<Index>																usable_maps;

		int number_of_tables = 0;
		for (it = table.begin(); !(it == table.end()); ++it)
		{
			if ((*it).first.hasSubstring(map_name))
			{
				number_of_tables++;
				usable_maps[(*it).first] = 0;
			}
		}
				
		std::list<Fragment*>::iterator	frag_it;				
		AtomIterator										atom_it;
		StringHashMap<String>*					map;
		StringHashMap<Index>::iterator	map_iterator;
		String													match_name;
		String													atom_name;
		String													res_name;
		String													res_name_suffix;
		int															hit_counter;

		for (map_iterator = usable_maps.begin(); !(map_iterator == usable_maps.end()); ++map_iterator)
		{
			hit_counter = 0;
			map = (*table.find((*map_iterator).first)).second;
			for (frag_it = fragments_.begin(); frag_it != fragments_.end(); ++frag_it)
			{
				// determine the fragment name
				res_name = (*frag_it)->getName();

				// determine whether the fragment is an amino acid
				// if it is: determine the correct name for N-,C-terminal AA
				if (RTTI<Residue>::isKindOf(*(*frag_it))) 
				{
					Residue*	residue;	
					residue = RTTI<Residue>::castTo(*(*frag_it));
					if (residue->isCTerminal())
					{
						res_name_suffix = "-C";
					} else {
						if (residue->isNTerminal())
						{
							res_name_suffix = "-N";
						}				
					}
				} else {
					res_name_suffix = "";
				}

				for (atom_it = (*frag_it)->beginAtom(); +atom_it; ++atom_it){
					atom_name = (*atom_it).getName();

					// first, try to match exactly
					match_name = res_name + res_name_suffix;
					if (matchName(match_name, atom_name, map)){
						hit_counter++;
					} else {
						// try to match non-terminal residues
						if (matchName(res_name, atom_name, map)){
							hit_counter++;
						} else {
							match_name = "*" + res_name_suffix;
							if (matchName(match_name, atom_name, map)){
								hit_counter++;
							} else {
								match_name = "*";
								if (matchName(match_name, atom_name, map)){
									hit_counter++;
								}
							}
						}
					}
				}
			}

			// update hit_count for each map
			(*map_iterator).second = hit_counter;
		}

		// these two variables are needed to store the best map
		Index max_hits = -1;
		map_name = "";
		
		// look for the best map
		for (map_iterator = usable_maps.begin(); !(map_iterator == usable_maps.end()); ++map_iterator)
		{
			if ((*map_iterator).second > max_hits)
			{
				max_hits = (*map_iterator).second;
				map_name = (*map_iterator).first;
			}
		}


		// after having identified the map, use it to replace the names
		// first, get the map
		
		
		if (table.find(map_name) != table.end())
		{
			map = (*table.find(map_name)).second;
		} else {
			map = 0;
		}

			
		// we found an appropriate map, so use it
		if ((max_hits > 0) && (map != 0))
		{
			// now iterate over all fragments we collected
			for (frag_it = fragments_.begin(); frag_it != fragments_.end(); ++frag_it)
			{
				// extract the residue name
				res_name = (*frag_it)->getName();

				// determine whether the fragment is an amino acid
				// if it is: determine the correct name for N-,C-terminal AA
				if (RTTI<Residue>::isKindOf(*(*frag_it))) 
				{
					Residue*	residue = RTTI<Residue>::castTo(*(*frag_it));
					if (residue->isCTerminal())
					{
						res_name_suffix = "-C";
					} else {
						if (residue->isNTerminal())
						{
							res_name_suffix = "-N";
						}				
					}
				} else {
					res_name_suffix = "";
				}

				// now, iterate over the fragment`s atoms
				for (atom_it = (*frag_it)->beginAtom(); +atom_it; ++atom_it){
					// get the atom name
					atom_name = (*atom_it).getName();

					// first, try to match exactly
					match_name = res_name + res_name_suffix;
					if (matchName(match_name, atom_name, map))
					{
						// OK, got it. Change the names
						(*atom_it).setName(atom_name);
						(*atom_it).getFragment()->setName(res_name);
					} else {
						// second, try to match non-terminal residues exactly
						match_name = res_name;
						if (matchName(match_name, atom_name, map))
						{
							// OK, got it. Change the names
							(*atom_it).setName(atom_name);
							(*atom_it).getFragment()->setName(res_name);
						} else {
							// try wildcard match for residue names
							match_name = "*" + res_name_suffix;
							if (matchName(match_name, atom_name, map))
							{
								// OK, got it. Change names
								(*atom_it).setName(atom_name);
								(*atom_it).getFragment()->setName(res_name);
							} else {
								// last alternative: try wildcard with unmodified name
								match_name = "*";
								if (matchName(match_name, atom_name, map))
								{
									(*atom_it).setName(atom_name);
									(*atom_it).getFragment()->setName(res_name);
								}
							}
						}
					}
				}
			}
		}

		// if we couldn't find an appropriate table, complain about it!
		if ((number_of_tables == 0) || (max_hits < 0)){
			Log.level(LogStream::ERROR) << "cannot locate an appropriate name conversion table!" << endl;
			return false;
		}

		// else:
		return true;
	}



	/////////////////////////////////////////////////////////////////
	//		FragmentDB::AddHydrogensProcessor												 //
	/////////////////////////////////////////////////////////////////	

	// dreht Vektor um die x1 Achse
	void FragmentDB::AddHydrogensProcessor::turn_x1_(Vector3& v,const float winkel) 
	{
		float hilf_y = v.y * cos(winkel) + v.z * -sin(winkel);
		float hilf_z = v.y * sin(winkel) + v.z * cos(winkel);
		v.y = hilf_y;
		v.z = hilf_z;
	}

	// dreht Vektor um die x2 Achse
	void FragmentDB::AddHydrogensProcessor::turn_x2_(Vector3& v,const float winkel)
	{
		float hilf_x = v.x * cos(winkel) + v.z * -sin(winkel);
		float hilf_z = v.x * sin(winkel) + v.z * cos(winkel);
		v.x = hilf_x;
		v.z = hilf_z;
	}

	// dreht Vektor um die x3 Achse
	void FragmentDB::AddHydrogensProcessor::turn_x3_(Vector3& v,const float winkel) 
	{
		float hilf_x = v.x * cos(winkel) + v.y * -sin(winkel);
		float hilf_y = v.x * sin(winkel) + v.y * cos(winkel);
		v.x = hilf_x;
		v.y = hilf_y;
	}

	// berechnet Winkel zur x1x2 Ebene
	float FragmentDB::AddHydrogensProcessor::winkelx1_x2_(Vector3& v) 
	{
		float hilf = v.x;
		float alpha;
		v.x = 0;
		alpha = asin(v.z / v.getLength());
		v.x = hilf;
		return alpha;
	}

	// berechnet Winkel zur x1x3 Ebene
	float FragmentDB::AddHydrogensProcessor::winkelx1_x3_(Vector3& v) 
	{
		float hilf = v.z;
		float beta;
		v.z = 0;
		beta = asin(v.y / v.getLength());
		v.z = hilf;

		return beta;
	}

	// berechnet Wasserstoff Koordinaten
	void FragmentDB::AddHydrogensProcessor::calculate_	
		(String atom_name, Atom* bindungs_atom, Vector3& a, Vector3& b,Vector3& c, Vector3& d,
		 Vector3& xa, Vector3& xb,Vector3& xc, Vector3& xd, Vector3& xtarget)
	{
		// Setzen der lokalen Variablen    
		// Variablen mit beginnendem x beziehen sich auf die             
		// Prototypen
		int spiegel1 = 0;	
		int spiegel2 = 0;
		int xspiegel1 = 0;
		int xspiegel2 = 0;
		int xxspiegel = 0;	

		// bezieht sich auf beide Objekte, gesetzt,wenn am Ende 
		// noch Spiegelung zur x1x2 Ebene noetig ist

		// Verschieben des ersten Punktes in den Ursprung
		// Protein 
		Vector3 b_a = b-a;	
		Vector3 c_a = c-a;
		Vector3 d_a = d-a;
		// Prototyp      
		Vector3 xb_a = xb-xa;
		Vector3 xc_a = xc-xa;
		Vector3 xd_a = xd-xa;
		Vector3 xtarget_a = xtarget-xa; // einzufuegendes H-Atom
		//        


		// Drehen des ersten Punktes auf die x1_x2 Ebene
		// Protein
		float alpha = winkelx1_x2_(b_a);
		// Ueberpruefen der Drehrichtung
		if (b_a.y > 0)
		{
			alpha =-alpha; 
		}
		
		turn_x1_(b_a,alpha);	
		// Prototyp
		float xalpha = winkelx1_x2_(xb_a);
		// Ueberpruefen der Drehrichtung
		if (xb_a.y>0) 
		{
			xalpha=-xalpha; 
		}
			

		turn_x1_(xb_a,xalpha);	
	 
		// Drehen des ersten Punktes auf die x1 Achse
		// Protein
		float beta = winkelx1_x3_(b_a);
		if (b_a.x>0)
		{
			beta=-beta; 
		}
		turn_x3_(b_a,beta);	

		// Prototyp      
		float xbeta = winkelx1_x3_(xb_a);
		if (xb_a.x > 0)
		{
			xbeta=-xbeta; 
		}
		turn_x3_(xb_a,xbeta);	
					
		// Drehen des zweiten Punktes um alpha und beta
		// Drehen des zeiten Punktes auf die x1_x2 Ebene
		// Protein
		turn_x1_(c_a, alpha);
		turn_x3_(c_a, beta);	
		float gamma = winkelx1_x2_(c_a);
		if (c_a.y>0) 
		{
			gamma=-gamma; 
		}
		turn_x1_(c_a,gamma);

		// Prototyp 
		turn_x1_(xc_a,xalpha);
		turn_x3_(xc_a,xbeta);	       

		float xgamma = winkelx1_x2_(xc_a);
		if(xc_a.y>0) 
		{
			xgamma=-xgamma; 
		}
		turn_x1_(xc_a,xgamma);	

		// Setzen der Spiegel	
		// der zweite Punkt liegt dann im 1.Quadranten der x1x2-Ebene
		// Protein
		if (c_a.x<0) 
		{
			spiegel1=1;
		}
	
		if (c_a.y<0)
		{
			spiegel2=1;
		}

		// Prototyp
		if (xc_a.x<0)
		{
			xspiegel1=1;
		}

		if (xc_a.y<0)
		{
			xspiegel2=1;
		}
			
		// Anwenden der Spiegel
		// Protein
		if (spiegel1)
		{
			b_a.x=-b_a.x;
			c_a.x=-c_a.x;
		}
		if (spiegel2)
		{
			b_a.y=-b_a.y;
			c_a.y=-c_a.y;
		}
		// Prototyp
		if (xspiegel1)
		{
			xb_a.x=-xb_a.x;
			xc_a.x=-xc_a.x;
		}
		if (xspiegel2)
		{
			xb_a.y=-xb_a.y;
			xc_a.y=-xc_a.y;
		}

		// Winkeltest : korrigiert den Fehlerabstand der beiden Punkte in der Ebene
		float w;
		float n,z;
		float abstand1, abstand2;
		Vector3 test1, test2;
					
		z = c_a*xc_a;
		n = c_a.getLength() * xc_a.getLength();
				
		if (fabs( z - n ) > 0.0001)
		{
			//cerr <<endl<<"Winkel wird korrigiert";
			w = -(acos(z / n)) / 2;
											 
			test1=c_a - xc_a;
			abstand1 = test1.getLength();

			turn_x3_(xb_a, w);                     
			test2 = c_a - xc_a;
			abstand2 = test2.getLength();
 
			if (abstand2 > abstand1)
			{
				w = -w;
				turn_x3_(xc_a, 2 * w);
				turn_x2_(xb_a, 2 * w);
			}
		} else 
		{
			w = 0;
		}
			
		//

		// Drehen und Spiegeln des dritten Punktes um die
		// errechneten Werte
		// Ueberprueft ob beide Objekte noch an der x1x2-Ebene gespiegelt sind
		// Protein
		turn_x1_(d_a,alpha);
		turn_x3_(d_a,beta);
		turn_x1_(d_a,gamma);
		if (spiegel1)
		{
			d_a.x = -d_a.x;
		}
		if (spiegel2)
		{
			d_a.y = -d_a.y;
		}
		// Prototyp
		turn_x1_(xd_a, xalpha);
		turn_x3_(xd_a, xbeta);
		turn_x1_(xd_a, xgamma);
		turn_x3_(xd_a, w);		       
		if (xspiegel1)
		{
			xd_a.x = -xd_a.x;
		}
		if (xspiegel2)
		{
			xd_a.y = -xd_a.y;
		}
						
		// ueberprueft den Spiegel
		if (d_a.z * xd_a.z < 0) 
		{
			if ((d_a.z + xd_a.z < 0.2) && (d_a.z + xd_a.z > -0.2)) 
			{
				xxspiegel=1;
			}
		}
	 
		//
					
		// Berechnen von xtarget ::	// die Koordinaten des H-Atoms werden berechnet
					
		turn_x1_(xtarget_a, xalpha);
		turn_x3_(xtarget_a, xbeta);
		turn_x1_(xtarget_a, xgamma);
		turn_x3_(xtarget_a, w);		
		//Winkeltest
		if (xspiegel1)
		{
			xtarget_a.x = -xtarget_a.x;
		}
		if (xspiegel2)
		{
			xtarget_a.y = -xtarget_a.y; 
		}
		if (xxspiegel)
		{
			xtarget_a.z = -xtarget_a.z;
		}
		if (spiegel1)
		{
			xtarget_a.x = -xtarget_a.x;
		}
		if (spiegel2)
		{
			xtarget_a.y = -xtarget_a.y;  
		}
		turn_x1_(xtarget_a, -gamma);
		turn_x3_(xtarget_a, -beta);
		turn_x1_(xtarget_a, -alpha);
		xtarget = xtarget_a + a;

		// extract the atom name
		String name(atom_name.before(":"));

		PDBAtom& atom = *new PDBAtom;
		atom.setElement(PSE["H"]);
		atom.setPosition(xtarget);
		atom.setName(name);

		try 
		{
			atom.createBond(*bindungs_atom);
		} 
		catch (Bond::TooManyBonds)
		{
			Log.level(LogStream::ERROR) << "Cannot create bond between "
				<< atom_name << " and " << bindungs_atom->getName() << ": too many bonds!" << endl;
		}

		residue_->insert(atom);

		count_h_++;
	}
		
	// liest die Datei as.ini ein
	void FragmentDB::AddHydrogensProcessor::readTemplates_()
	{
		String h_name;
		String atom_name;

		ResourceEntry::Iterator iter;

		Size as_zaehler;
		Size zaehler1 = 0;
		Size zaehler2 = 0;
		Size h_anzahl = 0;
		const Size anzahl = 4;
		
		String s[5];
	 
		// this variable contains the number of fragment
		// types (excluding variants of fragments)
		Size number_of_raw_templates = template_names_.size();

		as_zaehler = 0;
		for (; as_zaehler < number_of_raw_templates; ++as_zaehler)
		{
			if (!fragment_db_->name_to_variants_.has(template_names_[as_zaehler]))
			{
				continue;
			}

			list<Residue*> variant_list = fragment_db_->name_to_variants_[template_names_[as_zaehler]];
			list<Residue*>::iterator list_it = variant_list.begin();
			for (; list_it != variant_list.end(); ++list_it)
			{
				String name = (*list_it)->getName();

				// check whether the path exists
				ResourceEntry* tmp = fragment_db_->tree->getEntry(fragment_db_->name_to_path_[name] + "/addh");
				if (tmp == 0)
				{
					Log.warn() << "Didn't find addh entry for " << name 
						<< " in path " << fragment_db_->name_to_path_[name] << "/addh" << endl;
					continue;
				}

				iter = tmp->begin();
				zaehler1 = 0;
				for (++iter; +iter; ++iter) 
				{
					zaehler1++;
				}
				// if the addh entry is empty, continue the main loop
				if (zaehler1 == 0)
				{
					Log.level(LogStream::WARNING) << "Cannot find addh entries for " << name << endl;
					continue;
				}
				
				h_anzahl = zaehler1;
					
				// if the variant is not the default variant, store its name
				if (name != template_names_[as_zaehler])
				{
					template_names_.push_back(name);
				}
				
								
				h_atoms_ = new String[h_anzahl + 1];
									
				iter = tmp->begin();
					
				// lese fuer jedes H-Atom ein
				for (zaehler1 = 0; zaehler1 < h_anzahl; zaehler1++) 
				{
					++iter;
					// Eindeutiger Name des einzufuegenden H-Atoms
					h_name = (*iter).getKey() + ":" + name;
			
					// einfuegen des Namens
					h_atoms_[zaehler1] = h_name; 
													
					atoms_ = new String[anzahl + 1]; // anlegen des Feldes der Referenzatome
					(*iter).getValue().split(s, 5);
			
					for (zaehler2 = 0; zaehler2 < anzahl; zaehler2++)
					{
						atom_name = s[zaehler2];
						
						if (atom_name != "x") 
						{
							atom_name.append(":");
							atom_name.append(name);
						}
						atoms_[zaehler2] = atom_name;
						
					}
					// terminierender String fuer H-Feld
					atoms_[zaehler2] = "NULL"; 
					ini_table_[h_name] = atoms_;
				}
				// terminierender String fuer H-Feld
				h_atoms_[zaehler1] = "NULL"; 
				h_table_[name] = h_atoms_;
			}
		}
	}    

	// liest die Prototypen ein
	// und speichert die Struktur in prot_table_
	void FragmentDB::AddHydrogensProcessor::buildProtonTable_()  
	{
		// Setzen der lokalen Variablen        
		Size as_zaehler = 0;
		String residue_name;
		String atom_name;
		const Fragment* tplate;
		AtomIterator atom_iter;
			
		for (as_zaehler = 0; as_zaehler < template_names_.size(); as_zaehler++) 
		{
			residue_name = template_names_[as_zaehler];
			tplate = fragment_db_->getFragment(residue_name);

			for(atom_iter = tplate->beginAtom(); +atom_iter; ++atom_iter) 
			{
				atom_name = (*atom_iter).getName();
				atom_name.append(":");
				atom_name.append(residue_name);
				prot_table_[atom_name] = *atom_iter;
			}
		}			
			 
	}

	// fuegt ein einzelnes H-Atom ein.
	// wird aufgerufen , wenn das Residue nicht N-Terminal ist
	void FragmentDB::AddHydrogensProcessor::addSingleH_(Atom* bindungs_atom, Vector3& n,Vector3& ca,Vector3& old_c)
	{
		// Setzen der lokalen Variablen

		int spiegel1=0;
		int spiegel2=0;
		Vector3 old_c_n=old_c-n; // letzte C-Atom des vorangehenden Residues
		Vector3 ca_n=ca-n;
		Vector3 h;
		const float length=1; // ungefaehr die Länge der H-Verbindung
		const float winkel=2.094395; //Winkel von 120 Grad
		h.set(length,0,0); // setze Positionsvektor des H-Atoms
		//	
		
		// Berechne Winkel zur x1x2-Ebene und drehe      
		float alpha = winkelx1_x2_(old_c_n);
		if (old_c_n.y>0) alpha=-alpha;
		turn_x1_(old_c_n,alpha);

		// Berechne Winkel zur x1x3-Ebene und drehe      
		float beta=winkelx1_x3_(old_c_n);
		if (old_c_n.x>0) beta=-beta;
		turn_x3_(old_c_n,beta);
		
		//drehe den zweiten Punkt um berechnete Winkel und drehe in dann auf x1x2 Ebene      
		turn_x1_(ca_n,alpha);
		turn_x3_(ca_n,beta);	
		float gamma=winkelx1_x2_(ca_n);
		if (ca_n.y>0) gamma=-gamma;	
		turn_x1_(ca_n,gamma);
					
		if (ca_n.x<0) spiegel1=1;
		if (ca_n.y<0) spiegel2=1;
		
		turn_x3_(h,-winkel/2); // H-Atom wird um 60 Grad gedreht
		
		if (spiegel1) h.x=-h.x;
		if (spiegel2) h.y=-h.y;
					
		turn_x1_(h,-gamma); // drehe H-Atom in Bezugssystem des Proteins
		turn_x3_(h,-beta);
		turn_x1_(h,-alpha);
		
		h=h+n;
		// lege neues Atom an und fuege es ein
		PDBAtom& atom = *new PDBAtom; 
		atom.setPosition(h);
		atom.setElement(PSE["H"]);
		atom.setName("H");
		atom.createBond(*bindungs_atom);
		residue_->insert(atom);
			
		count_h_++;
	}
		
					
	void FragmentDB::AddHydrogensProcessor::buildTetraeder_(Atom* bindungs_atom, Vector3& n, Vector3& c)
	{
		// wird aufgerufen wenn Residue N-Terminal ist und baut ein Tetraeder aus H-Atomen auf
		// Setzen der lokalen Variablen
		int spiegel=0;  // BAUSTELLE!!!
		const float length=1; // Laenge der H-Verbindung
		const float winkel1=1.911135; // winkel von 109.5 Grad
		const float winkel2=2.094395; // winkel von 120 Grad
		Vector3 c_n=c-n;
		Vector3 h1,h2,h3;
		h1.set(length,0,0);
		h2.set(length,0,0);
		h3.set(length,0,0); // setzen der Vektoren


		String residue_name,name;
		residue_name = residue_->getName();
		AtomIterator atom_iter;
		int h1_set = 0;
		int h2_set = 0;
		int h3_set = 0;
		int set = 0;
		float w1;
		Vector3 hilf1, hilf2, hilf3;


		//iteriere ueber residue;
		for(atom_iter=residue_->beginAtom();+atom_iter;++atom_iter)
		{
			name=(*atom_iter).getName();
			if (name=="1H") {h1=(*atom_iter).getPosition();h1_set=1;}
			if (name=="2H") {h2=(*atom_iter).getPosition();h2_set=1;}
			if (name=="3H") {h3=(*atom_iter).getPosition();h3_set=1;}
		}
		
		// drehen auf x1x2Ebene
		float alpha=winkelx1_x2_(c_n);
		if (c_n.y>0) alpha = -alpha;
		turn_x1_(c_n,alpha);
									
		// drehen auf x1 Achse
		float beta=winkelx1_x3_(c_n);
		if (c_n.x>0) beta=-beta;
		turn_x3_(c_n,beta);
		
		if (c_n.x<0) spiegel=1;
		//
		if (h1_set+h2_set+h3_set==0) // alles ist ungesetzt
		{	  	
			turn_x3_(h1,winkel1);
			
			turn_x3_(h2,winkel1);
			turn_x1_(h2,winkel2);
			
			turn_x3_(h3,winkel1);
			turn_x1_(h3,2*winkel2); // baue Tetraeder auf
			
			if (spiegel)
			{
				h1.x=-h1.x;
				h2.x=-h2.x;
				h3.x=-h3.x;
			}
			
			// drehen in das Bezugssystem des Proteins   
			turn_x3_(h1,-beta);
			turn_x3_(h2,-beta);
			turn_x3_(h3,-beta);
		
			turn_x1_(h1,-alpha);
			turn_x1_(h2,-alpha);
			turn_x1_(h3,-alpha); 
		
			h1 = h1 + n;
			h2 = h2 + n;
			h3 = h3 + n;
		
			// einfuegen der berechneten Atome
			PDBAtom& atom = *new PDBAtom;	
			atom.setElement(PSE["H"]);
			atom.setPosition(h1);
			atom.setName("1H");
			atom.createBond(*bindungs_atom);
			residue_->insert(atom);

			atom = *new PDBAtom;
			atom.setElement(PSE["H"]);
			atom.setPosition(h2);
			atom.setName("2H");
			atom.createBond(*bindungs_atom);
			residue_->insert(atom);

			atom = *new PDBAtom;
			atom.setElement(PSE["H"]);
			atom.setPosition(h3);
			atom.setName("3H");
			atom.createBond(*bindungs_atom);
			residue_->insert(atom);
			
			count_h_ += 3;
			h1_set = 1;
			h2_set = 1;
			h3_set = 1;
		} // Ende if alles ungesetzt
		
		if (h1_set+h2_set+h3_set==1) // nur ein Proton ist gesetzt
		{
			if (h1_set==1) 
			{
				hilf1 = h1 - n;
				set=1;
			}
			if (h2_set==1) 
			{
				hilf1 = h2 - n;
				set=2;
			}
			if (h3_set==1) 
			{
				hilf1 = h3 - n;
				set=3;
			}
			
			// hilf1 ist schon gesetzt, drehe in auf die positive x1_2 Ebene
			
			// zuerst nachdrehen
			turn_x1_(hilf1,alpha);
			turn_x3_(hilf1,beta);
			if (spiegel) hilf1.x=-hilf1.x;
			
			w1=winkelx1_x2_(hilf1);
			if (hilf1.y>0) w1=-w1;
			turn_x1_(hilf1,w1);
			
			//setze hilf2
			hilf2=hilf1;
			turn_x1_(hilf2,winkel2);
			
			//setze hilf3
			hilf3=hilf2;
			turn_x1_(hilf3,winkel2);
			
			
			// drehe in das Bezugssystem
			
			turn_x1_(hilf1,-w1);
			turn_x1_(hilf2,-w1);
			turn_x1_(hilf3,-w1);
					
			if (spiegel)
			{
				hilf1.x=-hilf1.x;
				hilf2.x=-hilf2.x;
				hilf3.x=-hilf3.x;
			}
			
			turn_x3_(hilf1,-beta);
			turn_x3_(hilf2,-beta);
			turn_x3_(hilf3,-beta);
		
			turn_x1_(hilf1,-alpha);
			turn_x1_(hilf2,-alpha);
			turn_x1_(hilf3,-alpha); // drehen in das Bezugssystem des Proteins   
		
			hilf1=hilf1+n;
			hilf2=hilf2+n;
			hilf3=hilf3+n;
		
			PDBAtom& atom = *new PDBAtom;	// einfuegen der berechneten Atome
			atom.setPosition(hilf2);
			atom.setElement(PSE["H"]);
			if (set==3) atom.setName("1H");
			if (set==2) atom.setName("3H");
			if (set==1) atom.setName("2H");
			atom.createBond(*bindungs_atom);
			residue_->insert(atom);

			atom = *new PDBAtom;
			atom.setElement(PSE["H"]);
			atom.setPosition(hilf3);
			if (set==3) atom.setName("2H");
			if (set==2) atom.setName("1H");
			if (set==1) atom.setName("3H");
			atom.createBond(*bindungs_atom);
			residue_->insert(atom);
			
			count_h_ += 2;
			h1_set = 1;
			h2_set = 1;
			h3_set = 1;
		} //Ende if nur ein Proton gesetzt

		if (h1_set+h2_set+h3_set==2) // 2 Protonen sind gesetzt
		{
			if (h1_set==1) set+=1;
			if (h2_set==1) set+=2;
			if (h3_set==1) set+=3;
			if (set==4) {set=2;hilf1=h1-n;}
			if (set==5) {set=1;hilf1=h3-n;}
			if (set==3) {set=3;hilf1=h2-n;}
			
			// hilf ist schon gesetzt, drehe in auf die positive x1_2 Ebene
			
			// zuerst nachdrehen
			turn_x1_(hilf1,alpha);
			turn_x3_(hilf1,beta);
			if (spiegel) {hilf1.x=-hilf1.x;}
			
			w1=winkelx1_x2_(hilf1);
			if (hilf1.y>0) w1=-w1;
			turn_x1_(hilf1,w1);
			
			//setze hilf2
			hilf2=hilf1;
			turn_x1_(hilf2,winkel2);
			
			// drehe in das Bezugssystem
			
			turn_x1_(hilf1,-w1);
					turn_x1_(hilf2,-w1);
					
			if (spiegel)
			{
				hilf1.x=-hilf1.x;
				hilf2.x=-hilf2.x;
			}
			
			turn_x3_(hilf1,-beta);
			turn_x3_(hilf2,-beta);
		
			turn_x1_(hilf1,-alpha);
			turn_x1_(hilf2,-alpha); // drehen in das Bezugssystem des Proteins   
			
			hilf1=hilf1+n;
			hilf2=hilf2+n;
			
			PDBAtom& atom = *new PDBAtom;	// einfuegen der berechneten Atome
			atom.setElement(PSE["H"]);
			atom.setPosition(hilf2);
			if (set==3) atom.setName("3H");
			if (set==2) atom.setName("2H");
			if (set==1) atom.setName("1H");
			atom.createBond(*bindungs_atom);
			residue_->insert(atom);

			count_h_ += 1;
			h1_set = 1;
			h2_set = 1;
			h3_set = 1;
		} //Ende if nur 2 Protonen gesetzt
	}
		


	bool FragmentDB::AddHydrogensProcessor::start()
	{
		count_h_ = 0;
						
		// einlesen von as.ini und Aufbau von h_table_ und ini_table_
		readTemplates_(); 

		// einlesen der Prototypen und Aufbau von prot_table__
		buildProtonTable_(); 
		
		return true;
	}

	bool FragmentDB::AddHydrogensProcessor::finish()
	{
		return true;
	}
		
	Processor::Result FragmentDB::AddHydrogensProcessor::operator () (Fragment& object)
	{
		// applyObject Operator
		// Definition der lokalen Variablen
		
		// check Variable fuer bereits eingefuegte Wasserstoffe
		bool inserted;
		// lopp variables
		int i = 0;
		int k = 0;
		Vector3 x[4]; //Vektoren des Proteins
		Vector3 xx[4]; //Vektoren der Prototypen
		Vector3 xtarget;
		static Vector3 old_C;
		AtomIterator atom_iter;
		Atom bindungs_atom;
		PDBAtom* atom;


		// Setzen des aktuellen Residues 
		
		if (RTTI<Residue>::isKindOf(object))
		{
			residue_ = RTTI<Residue>::castTo(object);
			
			String atom_name;
	
			const Fragment* tmp = fragment_db_->getReferenceFragment(*residue_);
			if (tmp == 0)
			{
				Log.level(LogStream::WARNING) << "couldn't find reference fragment for " << residue_->getName() << endl;
				return Processor::CONTINUE;
			} else {
				String variant_name = tmp->getName();
				cout << "reference variant for " << residue_->getName() << " is: " << variant_name << endl;
			
				if (h_table_.has(variant_name)) 
				{
					h_atoms_ = h_table_[variant_name];
				} else {
					Log.level(LogStream::WARNING) << "No information on hydrogens for residue "<< variant_name << endl;	
					return Processor::CONTINUE;
				}
			
				// Iterieren ueber die Wasserstoffe		
				while(h_atoms_[k] != "NULL")	
				{
					inserted = false;
					// teste ob Wasserstoff schon enthalten ist
					for (atom_iter = residue_->beginAtom(); +atom_iter; ++atom_iter)
					{
						atom_name = (*atom_iter).getName();
						atom_name.append(":");
						atom_name.append(variant_name);

						if (atom_name == h_atoms_[k]) 
						{ 
							inserted = true;		
							break;
						}
					}
			
					if (inserted) 
					{
						k++;
					} else {
						if(ini_table_.has(h_atoms_[k]))	
						{
							// identify the atoms needed to get the new atom`s coordinates
							atoms_ = ini_table_[h_atoms_[k]]; 
						} else {
							Log.level(LogStream::WARNING) << "Did not find entry for " << h_atoms_[k] << endl;
							continue;
						}

						if (atoms_[3] != "x") 
						{
							for (i = 0; i <= 3; i++)
							{	
								if (prot_table_.has(atoms_[i]))
								{
									atom = (PDBAtom*)&prot_table_[atoms_[i]];
									xx[i] = atom->getPosition();
								} else {
									Log.level(LogStream::ERROR) << "prot_table_ Eintrag nicht gefunden :"<< atoms_[i] << endl;
									continue;
								}
							}
							if (prot_table_.has(h_atoms_[k]))
							{
								atom = (PDBAtom*)&prot_table_[h_atoms_[k]];
								// Koordinaten der Prototypatome
								xtarget = atom->getPosition(); 
							} else {
								Log.level(LogStream::ERROR) << "prot_table_ Eintrag nicht gefunden :"<< h_atoms_[k] << endl;
								continue;
							}
						
							for (i = 0; i <= 3; i++)
							{
								for (atom_iter = residue_->beginAtom(); +atom_iter; ++atom_iter)
								{
									atom_name = (*atom_iter).getName();
									atom_name.append(":");
									atom_name.append(variant_name);
									if (atom_name == atoms_[i])
									{
										x[i] = (*atom_iter).getPosition();
										if (!i)
										{
											bindungs_atom = (*atom_iter);	
										}
									}
								}
							}
		

							// Berechne xtarget
							calculate_(h_atoms_[k], &bindungs_atom,
												 x[0], x[1], x[2], x[3],
												 xx[0], xx[1], xx[2], xx[3], 
												 xtarget); 
		
							k++;
		
						}else {

							//Setzen der N - Wasserstoffe
		
							for(i = 0; i <= 2; i++) // lauft 3 mal durch
							{
								for(atom_iter = residue_->beginAtom(); +atom_iter; ++atom_iter)
								{
									atom_name = (*atom_iter).getName();
									atom_name.append(":");
									atom_name.append(variant_name);
									if (atom_name == atoms_[i]) 
									{
										x[i] = (*atom_iter).getPosition();
										if (!i) 
										{
											bindungs_atom=(*atom_iter);
										}
									}
								}
							}
							if (residue_->isNTerminal())
							{
								buildTetraeder_(&bindungs_atom, x[0], x[1]);
							} else {
								addSingleH_(&bindungs_atom, x[0], x[1], old_C);
							}
							
							old_C = x[2];
							k++;
						}
					}
				}
			}
		}

		return Processor::CONTINUE;
	}

		
	FragmentDB::AddHydrogensProcessor::AddHydrogensProcessor(const FragmentDB& db)
	{
		fragment_db_ = &db;
		
		// extract the fragment names from the fragment DB
		const ResourceEntry* tmp = fragment_db_->tree->getEntry("/Fragments");
		ResourceEntry::ConstIterator it = tmp->begin();
		for (++it; +it; ++it)
		{
      if ((*it).getDepth() == tmp->getDepth() + 1)
			{
				template_names_.push_back((*it).getKey());
			}
		}
	}
		
	FragmentDB::AddHydrogensProcessor::AddHydrogensProcessor()
	{
		fragment_db_ = 0;
	}
		
	FragmentDB::AddHydrogensProcessor::~AddHydrogensProcessor()
	{
		// BAUSTELLE: Da fehlt ja wohl noch alles!
		// erase the array containing the amino acid names
		template_names_.clear();
		
		fragment_db_ = 0;
	}

	unsigned long FragmentDB::AddHydrogensProcessor::getNumberOfInsertedH()
	{
		return count_h_;
	}



	//	BuildBondsProcessor

	FragmentDB::BuildBondsProcessor::BuildBondsProcessor()
		:	fragment_db_(0)
	{
	}

	FragmentDB::BuildBondsProcessor::BuildBondsProcessor(const FragmentDB& db)
		: fragment_db_(const_cast<FragmentDB*>(&db))
	{
	}

	FragmentDB::BuildBondsProcessor::~BuildBondsProcessor()
	{
		fragment_db_ = 0;
	}

	bool FragmentDB::BuildBondsProcessor::finish()
	{
		// if there are no inter-fragment bonds, return
		if (fragment_list_.size() >= 2)
		{

			// iterate over all pairs of consecutive fragments 
			// in the list and try to construct bonds between them
			list<Fragment*>::iterator it1 = fragment_list_.begin();
			list<Fragment*>::iterator it2 = it1;
			for (++it2 ; it2 != fragment_list_.end(); ++it1, ++it2)
			{
				buildInterFragmentBonds(**it1, **it2);
			}
		}

		// clear the fragment list
		fragment_list_.clear();

		return true;
	}

	Processor::Result FragmentDB::BuildBondsProcessor::operator () (Fragment& fragment)
	{
		// build all bonds in the fragment
		buildFragmentBonds(fragment);
		
		// store a pointer to the fragment in a list
		// to build all inter-fragment bonds in the finish method
		fragment_list_.push_back(&fragment);

		return Processor::CONTINUE;
	}

	void FragmentDB::BuildBondsProcessor::buildFragmentBonds(Fragment& fragment) const
	{
		// abort immediately if no fragment DB is known
		if (fragment_db_ == 0)
			return;

		// get the fragment`s name
		String	name = fragment.getName();

		
		
		// check whether our DB knows the fragment and retrieve the template
		const Fragment* tplate = fragment_db_->getReferenceFragment(fragment);
		if (tplate == 0) 
		{
			return;
		}

		Size bond_count = 0;
		AtomIterator				tmp_it1;
		Atom::BondIterator	tmp_it2;
		BALL_FOREACH_BOND(*tplate, tmp_it1, tmp_it2)
		{
			bond_count++;
		}
		// iterate over all atoms in the tplate
		AtomIterator				tplate_atom_it;
		AtomIterator				frag_atom_it;
		Atom::BondIterator	tplate_bond_it;
		Atom*								partner;

		// iterate over all template atoms 
		for (tplate_atom_it = tplate->beginAtom(); +tplate_atom_it; ++tplate_atom_it) 
		{
			// determing the corresponding fragment atom
			for (frag_atom_it = fragment.beginAtom(); +frag_atom_it; ++frag_atom_it) 
			{
				if ((*frag_atom_it).getName().trim() == (*tplate_atom_it).getName()) 
				{
					// we found two matching atoms. Great! Now check their bonds..
					// iterate over all bonds of the template
					for (tplate_bond_it = (*tplate_atom_it).beginBond(); +tplate_bond_it; ++tplate_bond_it) 
					{
						partner = (*tplate_bond_it).getPartner(*tplate_atom_it);
						// if we found the bond partner, create the new bond
						if (partner != 0) 
						{
							name = partner->getName();
							AtomIterator	second_frag_it;
							for (second_frag_it = fragment.beginAtom(); +second_frag_it; ++second_frag_it) 
							{
								if ((*second_frag_it).getName().trim() == name) 
								{
									(*frag_atom_it).createBond(*second_frag_it);
									break;
								}
							}
						}

					}
				}
			}
		}
	}

	void FragmentDB::BuildBondsProcessor::buildInterFragmentBonds(Fragment& first, Fragment& second) const
	{
		if (fragment_db_ == 0)
		{
			return;
		}

		String first_name(first.getName());
		String second_name(second.getName());

		ResourceEntry*	first_entry = fragment_db_->tree->getEntry("/Fragments/" + first_name + "/Connections");
		if (first_entry == 0)
		{
			return;
		}

		ResourceEntry* second_entry = fragment_db_->tree->getEntry("/Fragments/" + second_name + "/Connections");
		if (second_entry == 0)
		{
			return;
		}


		String	s1[5];
		String	s2[5];
		ResourceEntry::Iterator	it1 = first_entry->begin();
		ResourceEntry::Iterator	it2;
		for (++it1; +it1; ++it1)
		{
			// split the fields of the "Connections" entry.
			// It should have the following format:
			//   (<name> <atom_name> <match_name> <distance> <tolerance>)
			//	<name>:				Name of the connection type (eg C-term)
			//	<atom_name>:	Name of the atom that might create the connection
			//	<match_name>:	Name of a matching connection type: this connection is 
			//								created if the two names match
			//	<distance>:		Distance of the connection in Angstrom
			//	<tolerance>:	Tolerance: connection will be built only if the distance
			//								of the two atoms within <tolerance> of <distance>
			//	Example entry:
			//		(C-term C N-term 1.33 0.5):
			//			This will build a connection to a fragment with a N-term connection
			//			if the two atoms are 1.33+/-0.5 Angstrom apart.

			it1->getValue().split(s1, 5);

			// check if the connection of the first fragment
			// matches any connection type of the second fragment
			for (it2 = second_entry->begin(), ++it2; +it2; ++it2)
			{
				// if the two connection types match,
				// check for distance condition and the two atoms
				if (it2->getKey() == s1[1])
				{
					it2->getValue().split(s2, 5);
					Atom* a1 = first.getAtom(s1[0]);
					Atom* a2 = second.getAtom(s2[0]);
					if ((a1 != 0) && (a2 != 0))
					{
						// check the distance conditions for both Connection data sets
						float distance = a1->getPosition().getDistance(a2->getPosition());
						if ((fabs(distance - s1[2].toFloat()) < s1[3].toFloat())
								&& (fabs(distance - s2[2].toFloat()) < s2[3].toFloat()))
						{
							// create the bond
							a1->createBond(*a2);
						}
					}
				}
			}
		}
	}

	list<String> FragmentDB::getVariantNames(const String& name) const
	{
		list<String> names;

		if (name_to_variants_.has(name))
		{
			list<Residue*>::const_iterator it = name_to_variants_[name].begin();
			list<Residue*>::const_iterator end_it = name_to_variants_[name].end();
			for (; it != end_it; it++)
			{
				names.push_back((*it)->getName());
			}
		}

		return names;
	}
 

} // namespace BALL

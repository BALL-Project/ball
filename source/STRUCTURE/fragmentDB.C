// $Id: fragmentDB.C,v 1.33 2001/06/24 14:22:53 oliver Exp $

#include <BALL/STRUCTURE/fragmentDB.h>

#include <list>
#include <vector>

#include <BALL/KERNEL/PTE.h>
#include <BALL/COMMON/limits.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
	
/*			Things still missing (among others)
				===================================
				- check for unique atom names
				- saving of fragment databases
				- dynamic import of databases
*/

#define FRAGMENT_DB_INCLUDE_TAG "#include:"

using namespace std;

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

		} 
		else 
		{

			String	key = root_entry.getKey();
			String	value = root_entry.getValue();
			String	key_fields[2], value_fields[2];

			if (key.countFields(":") != 2)
			{
				// if the include directive is invalid,
				// remove the entry
				Log.error() << "FragmentDB: illegal #include directive: " << key << endl;
				root_entry.getParent()->removeChild(key, 0);

				return false;

			} 
			else 
			{
				key.split(key_fields, 2, ":");
				value.split(value_fields, 2, ":");
					
				ResourceEntry*	parent = root_entry.getParent();
				ResourceEntry* entry;
				parent->removeChild(key, 0);

				ResourceFile*	file;
				ResourceEntry* tree_entry;

				// search in the standard fragment DB file
				Path path;
				String filename = path.find(value_fields[0]);
				if (filename == "")
				{
					throw Exception::FileNotFound(__FILE__, __LINE__, value_fields[0]);
				}

				file = new ResourceFile(filename);
				if (!file->isValid())
				{
					Log.error() << "FragmentDB: cannot open include file " << value_fields[0] << endl;
					delete file;
					return false;
				}
					
				tree_entry = file->getRoot().getEntry(value_fields[1]);
				if (tree_entry == 0)
				{
					Log.error() << "FragmentDB: cannot find node " << value_fields[1] << " in file " << value_fields[0] << endl;
				} 
				else 
				{
					entry = parent->insertChild(key_fields[1], tree_entry->getValue());
					entry->mergeChildrenOf(*tree_entry);
				}
				delete file;
			}
		}

		return true;
	}

	// default constructor
	FragmentDB::FragmentDB()
	{
		setFilename("fragments/Fragments.db");

		init();

		normalize_names.setFragmentDB(*this);
		add_hydrogens.setFragmentDB(*this);
		build_bonds.setFragmentDB(*this);
	}


	FragmentDB::FragmentDB(const String& filename)
	{
		setFilename(filename);

		init();

		normalize_names.setFragmentDB(*this);
		add_hydrogens.setFragmentDB(*this);
		build_bonds.setFragmentDB(*this);
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
					Log.error() << "FragmentDB: wrong entry for atom " << (*entry_iterator).getKey() 
							 << ": " << (*entry_iterator).getValue() << endl;
				} 
				else 
				{
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
					atom->setElement(PTE.getElement(s[0]));
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
					Log.error() << "FragmentDB: Bond to a non-existing atom: " 
											<< fields[0] << "-" << fields[1] 
											<< " (in " << entry_iterator->getPath() << ")" << endl;
				} 
				else	
				{
					// otherwise create the bond, if valences free
					if ((atom1->countBonds() > Atom::MAX_NUMBER_OF_BONDS) || (atom2->countBonds() > Atom::MAX_NUMBER_OF_BONDS))
					{
						Log.error() << "FragmentDB: too many bonds - cannot create bond: " 
												<< atom1->getName() << "-" << atom2->getName()
												<< " in fragment " << fragment.getName() 
												<< " (in " << entry_iterator->getPath() << ")" << endl;
					} 
					else 
					{
						// create the bond
						Bond* bond = atom1->createBond(*atom2);

						if (bond != 0)
						{
							// by default, we create single bonds
							bond->setOrder(Bond::ORDER__SINGLE);

							// if the bond order is specified, set it
							// s == single, a == aromatic, d = double, t = triple
							if (fields[2] != "")
							{
								switch (fields[2][0])
								{
									case 'a':
										bond->setOrder(Bond::ORDER__AROMATIC); break;
									case 'd':
										bond->setOrder(Bond::ORDER__DOUBLE); break;
									case 't':
										bond->setOrder(Bond::ORDER__TRIPLE); break;
									case 's':
										bond->setOrder(Bond::ORDER__SINGLE); break;
									default:
										Log.error() << "FragmentDB::parseBonds_: unknown bond type " 
																<< fields[2] << " (in " << entry_iterator->getPath() << ")" << endl;
								}
							}
						}
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
					Log.error() << "FragmentDB: cannot delete non-existing atom: "
																			<< (*entry_iterator).getKey() << endl;
				} 
				else 
				{
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
					Log.error() << "FragmentDB: cannot rename non-existing atom: "
																			<< (*entry_iterator).getKey() << endl;
				} 
				else 
				{
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
				} 
				else 
				{
					invert = false;
				}
				

				Property prop = Limits<Property>::max();
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

				if (prop == Limits<Property>::max())
				{
					// if the property was not recognized,
					// store it as a name-value pair
					if (invert)
					{
						property_man.clearProperty(property.c_str());
					} 
					else	
					{
						property_man.setProperty(property.c_str());
					}
				} 
				else 
				{
					if (invert)
					{
						property_man.clearProperty(prop);
					} 
					else 
					{
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
					Log.error() << "FragmentDB: cannot find Atoms entry for " 
											<< fragment_name << endl;
					return;
				} 
				else	
				{
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
							} 
							else 
							{
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
		if (entry != 0)
		{
			for (entry_iterator = ++entry->begin(); +entry_iterator; ++entry_iterator)
			{
				if ((*entry_iterator).getDepth() == 2)
				{
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
		if (entry == 0)
		{
			default_standard_ = "PDB";
		} 
		else 
		{
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
		if (name_to_frag_pointer_.has(fragment_name))
		{
			return (*name_to_frag_pointer_.find(fragment_name)).second;
		} 
		else	
		{
			return 0;		
		}
	}

	Fragment* FragmentDB::getFragmentCopy(const String& fragment_name) const
	{
		const Fragment* ref_fragment = getFragment(fragment_name);
		Fragment* copy = 0;

		// copy the reference fragment if we found a reference fragment
		// (pointer != 0). Otherwise, return the NULL pointer.
		if (ref_fragment !=	0)
		{
			copy = new Fragment(*ref_fragment);
		}

		return copy;
	}

	Residue* FragmentDB::getResidueCopy(const String& fragment_name) const
	{
		const Residue* ref_residue = getResidue(fragment_name);
		Residue* copy = 0;

		// copy the reference residue if we found a reference residue
		// (pointer != 0). Otherwise, return the NULL pointer.
		if (ref_residue !=	0)
		{
			copy = new Residue(*ref_residue);
		}

		return copy;
	}

	Molecule* FragmentDB::getMoleculeCopy(const String& fragment_name) const
	{
		const Fragment* ref_fragment = getFragment(fragment_name);
		Molecule* copy = 0;

		// copy the reference fragment if we found a reference fragment
		// (pointer != 0) and insert it into a new molecule.
		// Otherwise, return the NULL pointer.
		if (ref_fragment !=	0)
		{
			copy = new Molecule;
			copy->insert(*new Fragment(*ref_fragment));
		}

		return copy;
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
		if (RTTI::isKindOf<Residue>(fragment))
		{
			const Residue*	residue = RTTI::castTo<const Residue>(fragment);
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
			property_difference = (int)abs((int)props.countValue(true) 
													- (int)(*it)->getBitVector().countValue(true));
			props &= (*it)->getBitVector();
			number_of_properties = (int)props.countValue(true);

			if ((number_of_properties > best_number_of_properties)
					|| ((number_of_properties == best_number_of_properties) 
							&& (property_difference < best_property_difference)))
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
		if (name_to_frag_pointer_.has(fragment_name))
		{
			return (*name_to_frag_pointer_.find(fragment_name)).second;
		} 
		else 
		{
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
	bool FragmentDB::NormalizeNamesProcessor::matchName
		(String&	res_name, String&	atom_name,
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
		} 
		else 
		{
			// second, try wildcard match for residue names
			match_name = "*:" + a_name;
			if (map->has(match_name))
			{
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

		StringHashMap<StringHashMap<String>*>::Iterator		it;

		StringHashMap<Index> usable_maps;

		int number_of_tables = 0;
		for (it = table.begin(); !(it == table.end()); ++it)
		{
			if ((*it).first.hasSubstring(map_name))
			{
				number_of_tables++;
				usable_maps[(*it).first] = 0;
			}
		}
				
		list<Fragment*>::iterator				frag_it;				
		AtomIterator										atom_it;
		StringHashMap<String>*					map;
		StringHashMap<Index>::Iterator	map_iterator;
		String													match_name;
		String													atom_name;
		String													res_name;
		String													res_name_suffix;
		Size														hit_counter;

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
				if (RTTI::isKindOf<Residue>(*(*frag_it))) 
				{
					Residue*	residue;	
					residue = RTTI::castTo<Residue>(*(*frag_it));
					if (residue->isCTerminal())
					{
						res_name_suffix = "-C";
					} 
					else 
					{
						if (residue->isNTerminal())
						{
							res_name_suffix = "-N";
						}				
					}
				} 
				else 
				{
					res_name_suffix = "";
				}

				for (atom_it = (*frag_it)->beginAtom(); +atom_it; ++atom_it)
				{
					atom_name = (*atom_it).getName();

					// first, try to match exactly
					match_name = res_name + res_name_suffix;
					if (matchName(match_name, atom_name, map))
					{
						hit_counter++;
					} 
					else 
					{
						// try to match non-terminal residues
						if (matchName(res_name, atom_name, map))
						{
							hit_counter++;
						} 
						else 
						{
							match_name = "*" + res_name_suffix;
							if (matchName(match_name, atom_name, map))
							{
								hit_counter++;
							} 
							else 
							{
								match_name = "*";
								if (matchName(match_name, atom_name, map))
								{
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
		} 
		else 
		{
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
				if (RTTI::isKindOf<Residue>(*(*frag_it))) 
				{
					Residue*	residue = RTTI::castTo<Residue>(*(*frag_it));
					if (residue->isCTerminal())
					{
						res_name_suffix = "-C";
					}		
					else 
					{
						if (residue->isNTerminal())
						{
							res_name_suffix = "-N";
						}				
					}
				} 
				else 
				{
					res_name_suffix = "";
				}

				// now, iterate over the fragment`s atoms
				for (atom_it = (*frag_it)->beginAtom(); +atom_it; ++atom_it)
				{
					// get the atom name
					atom_name = (*atom_it).getName();

					// first, try to match exactly
					match_name = res_name + res_name_suffix;
					if (matchName(match_name, atom_name, map))
					{
						// OK, got it. Change the names
						(*atom_it).setName(atom_name);
						(*atom_it).getFragment()->setName(res_name);
					} 
					else 
					{
						// second, try to match non-terminal residues exactly
						match_name = res_name;
						if (matchName(match_name, atom_name, map))
						{
							// OK, got it. Change the names
							(*atom_it).setName(atom_name);
							(*atom_it).getFragment()->setName(res_name);
						}
						else 
						{
							// try wildcard match for residue names
							match_name = "*" + res_name_suffix;
							if (matchName(match_name, atom_name, map))
							{
								// OK, got it. Change names
								(*atom_it).setName(atom_name);
								(*atom_it).getFragment()->setName(res_name);
							} 
							else 
							{
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
		if ((number_of_tables == 0) || (max_hits < 0))
		{
			Log.error() << "FragmentDB: cannot locate an appropriate name conversion table!" << endl;
			return false;
		}

		// else:
		return true;
	}


	/////////////////////////////////////////////////////////////////
	//		FragmentDB::AddHydrogensProcessor												 //
	/////////////////////////////////////////////////////////////////	

	void FragmentDB::AddHydrogensProcessor::setFragmentDB(const FragmentDB& db)
	{
		fragment_db_ = &const_cast<FragmentDB&>(db);
	}

	// turns vector around x1-axis
	void FragmentDB::AddHydrogensProcessor::turn_x1_
		(Vector3& v, const float winkel) 
	{
		float hilf_y = v.y * cos(winkel) + v.z * -sin(winkel);
		float hilf_z = v.y * sin(winkel) + v.z * cos(winkel);
		v.y = hilf_y;
		v.z = hilf_z;
	}

	// turns vector around x2-axis
	void FragmentDB::AddHydrogensProcessor::turn_x2_(Vector3& v,const float winkel)
	{
		float hilf_x = v.x * cos(winkel) + v.z * -sin(winkel);
		float hilf_z = v.x * sin(winkel) + v.z * cos(winkel);
		v.x = hilf_x;
		v.z = hilf_z;
	}

	// turns vector around x3-axis
	void FragmentDB::AddHydrogensProcessor::turn_x3_(Vector3& v,const float winkel) 
	{
		float hilf_x = v.x * cos(winkel) + v.y * -sin(winkel);
		float hilf_y = v.x * sin(winkel) + v.y * cos(winkel);
		v.x = hilf_x;
		v.y = hilf_y;
	}

	// calculates angle to x1x2 plane
	float FragmentDB::AddHydrogensProcessor::winkelx1_x2_(Vector3& v) 
	{
		float hilf = v.x;
		float alpha;
		v.x = 0;
		alpha = asin(v.z / v.getLength());
		v.x = hilf;
		return alpha;
	}

	// calculates angle to x1x3 plane
	float FragmentDB::AddHydrogensProcessor::winkelx1_x3_(Vector3& v) 
	{
		float hilf = v.z;
		float beta;
		v.z = 0;
		beta = asin(v.y / v.getLength());
		v.z = hilf;

		return beta;
	}

	// calculates position of hydrogen to be added
	PDBAtom* FragmentDB::AddHydrogensProcessor::createNewHydrogen_	
		(const Atom& ref_hydrogen,
		 const Atom& center_atom, const Atom& atom_1, const Atom& atom_2, const Atom& atom_3,
		 const Atom& ref_center_atom, const Atom& ref_atom_1, const Atom& ref_atom_2, const Atom& ref_atom_3)
		throw()
	{
		// setting of local variables    
		// variables beginning with x, are related to             
		// the prototypes of the fragment database
		int mirror1 = 0;	
		int mirror2 = 0;
		int xmirror1 = 0;
		int xmirror2 = 0;
		int xxmirror = 0;	

		// moving first point to coordinate center
		// actual residue 
		Vector3 b_a = atom_1.getPosition() - center_atom.getPosition();	
		Vector3 c_a = atom_2.getPosition() - center_atom.getPosition();
		Vector3 d_a = atom_3.getPosition() - center_atom.getPosition();

		// residue prototype    
		Vector3 xb_a = ref_atom_1.getPosition() - ref_center_atom.getPosition();
		Vector3 xc_a = ref_atom_2.getPosition() - ref_center_atom.getPosition();
		Vector3 xd_a = ref_atom_3.getPosition() - ref_center_atom.getPosition();
		// hydrogen to be added
		Vector3 xtarget = ref_hydrogen.getPosition();
		Vector3 xtarget_a = xtarget - ref_center_atom.getPosition();

		// turning the second point onto x1_x2 plane
		// actual residue
		float alpha = winkelx1_x2_(b_a);
		// checking turn direction
		if (b_a.y > 0)
		{
			alpha =-alpha; 
		}
		
		turn_x1_(b_a,alpha);	
		// residue prototype
		float xalpha = winkelx1_x2_(xb_a);
		// checking turn direction
		if (xb_a.y>0) 
		{
			xalpha=-xalpha; 
		}
			

		turn_x1_(xb_a,xalpha);	
	 
	 	// turning second point onto x-axis
		// actual residue
		float beta = winkelx1_x3_(b_a);
		if (b_a.x>0)
		{
			beta=-beta; 
		}
		turn_x3_(b_a, beta);	

		// residue prototype    
		float xbeta = winkelx1_x3_(xb_a);
		if (xb_a.x > 0)
		{
			xbeta = -xbeta; 
		}
		turn_x3_(xb_a, xbeta);	
		
		// turning third point with alpha and beta			
		// turning third point onto x1_x2 plane
		// actual residue
		turn_x1_(c_a, alpha);
		turn_x3_(c_a, beta);	
		float gamma = winkelx1_x2_(c_a);
		if (c_a.y > 0) 
		{
			gamma=-gamma; 
		}
		turn_x1_(c_a, gamma);

		// residue prototype
		turn_x1_(xc_a, xalpha);
		turn_x3_(xc_a, xbeta);	       

		float xgamma = winkelx1_x2_(xc_a);
		if(xc_a.y > 0) 
		{
			xgamma=-xgamma; 
		}
		turn_x1_(xc_a, xgamma);	

		// setting the mirrors
		// third point lies in first quadrant of x1x2-plane	
		// actual residue
		if (c_a.x < 0) 
		{
			mirror1 = 1;
		}
	
		if (c_a.y < 0)
		{
			mirror2 = 1;
		}

		// residue prototype
		if (xc_a.x < 0)
		{
			xmirror1 = 1;
		}

		if (xc_a.y < 0)
		{
			xmirror2 = 1;
		}
			
		// using mirrors
		// actual residue
		if (mirror1)
		{
			b_a.x = -b_a.x;
			c_a.x = -c_a.x;
		}
		if (mirror2)
		{
			b_a.y = -b_a.y;
			c_a.y = -c_a.y;
		}
		// residue prototype
		if (xmirror1)
		{
			xb_a.x = -xb_a.x;
			xc_a.x = -xc_a.x;
		}
		if (xmirror2)
		{
			xb_a.y = -xb_a.y;
			xc_a.y = -xc_a.y;
		}

		// error distance of both points on plane is corrected
		float w;
		float n,z;
		float abstand1, abstand2;
		Vector3 test1, test2;
					
		z = c_a * xc_a;
		n = c_a.getLength() * xc_a.getLength();
				
		if (fabs(z - n) > 0.0001)
		{
			w = -(acos(z / n)) / 2;
											 
			test1 = c_a - xc_a;
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
		} 
		else 
		{
			w = 0;
		}
			
		
		// turning and using mirrors with forth point
		// check wether there is need of another mirror at x1x2 plane
		
		// actual residue
		turn_x1_(d_a, alpha);
		turn_x3_(d_a, beta);
		turn_x1_(d_a, gamma);
		if (mirror1)
		{
			d_a.x = -d_a.x;
		}
		if (mirror2)
		{
			d_a.y = -d_a.y;
		}
		// residue prototype
		turn_x1_(xd_a, xalpha);
		turn_x3_(xd_a, xbeta);
		turn_x1_(xd_a, xgamma);
		turn_x3_(xd_a, w);		       
		if (xmirror1)
		{
			xd_a.x = -xd_a.x;
		}
		if (xmirror2)
		{
			xd_a.y = -xd_a.y;
		}
						
		// check for mirror
		if (d_a.z * xd_a.z < 0) 
		{
			if ((d_a.z + xd_a.z < 0.2) && (d_a.z + xd_a.z > -0.2)) 
			{
				xxmirror=1;
			}
		}
	 
		//
		
		// calculating of xtarget : coordinates of hydrogen to be added are calculated			
					
		turn_x1_(xtarget_a, xalpha);
		turn_x3_(xtarget_a, xbeta);
		turn_x1_(xtarget_a, xgamma);
		turn_x3_(xtarget_a, w);		
		//checking the mirrors
		if (xmirror1)
		{
			xtarget_a.x = -xtarget_a.x;
		}
		if (xmirror2)
		{
			xtarget_a.y = -xtarget_a.y; 
		}
		if (xxmirror)
		{
			xtarget_a.z = -xtarget_a.z;
		}
		if (mirror1)
		{
			xtarget_a.x = -xtarget_a.x;
		}
		if (mirror2)
		{
			xtarget_a.y = -xtarget_a.y;  
		}
		turn_x1_(xtarget_a, -gamma);
		turn_x3_(xtarget_a, -beta);
		turn_x1_(xtarget_a, -alpha);
		xtarget = xtarget_a + center_atom.getPosition();

		// creating new atom and inserting it 
		PDBAtom& new_hydrogen = *new PDBAtom;
		((Atom&)new_hydrogen).set(ref_hydrogen);
		new_hydrogen.setPosition(xtarget);

		return &new_hydrogen;
	}

	Quadruple<bool, const Atom*, const Atom*, const Atom*> 
	FragmentDB::AddHydrogensProcessor::getThreeReferenceAtoms_
		(const Atom& ref_center_atom)
		throw()
	{
		Quadruple<bool, const Atom*, const Atom*, const Atom*> result(false, 0, 0, 0);
		
		// a hash set to remember all those atoms we have already visited
		list<const Atom*> atom_list;
		atom_list.push_back(&ref_center_atom);
		
		// abort if we found the three first atoms (beyond the center atom)
		// or we are running out of fresh atoms
		list<const Atom*>::iterator current(atom_list.begin());
		while ((atom_list.size() < 4) && (current != atom_list.end()))
		{
			Atom::BondConstIterator bond((*current)->beginBond());
			for (; +bond; ++bond)
			{
				const Atom* next_atom = bond->getPartner(**current);
				if ((next_atom->getElement() != PTE[Element::H]) 
						&& (find(atom_list.begin(), atom_list.end(), next_atom) == atom_list.end()))
				{
					atom_list.push_back(next_atom);
					if (atom_list.size() > 3)
					{
						bond = (*current)->endBond();
						break;
					}
				}
			}
			
			// try the bonds of the next atom in the list
			current++;
		}
		
		// copy the three reference atoms to the result 
		// (omit the first atom, which is the center atom!)
		result.first = (atom_list.size() == 4);
		current = atom_list.begin();
		current++;
		if (current != atom_list.end())
		{
			result.second = *current;
			current++;
		}
		if (current != atom_list.end())
		{
			result.third  = *current;
			current++;
		}
		if (current != atom_list.end())
		{
			result.fourth = *current;
		}

		return result;
	}
	
	// start function of AddHydrogensProcessor
	// nothing important is done here
	bool FragmentDB::AddHydrogensProcessor::start()
	{
		number_of_inserted_h_ = 0;
		
		if (fragment_db_ == 0)
		{
			Log.error() << "FragmentDB::AddHydrogensProcessor: no FragmentDB defined. "
									<< "Use setFragmentDB() to associate a fragment database." << std::endl;
			return false;
		}
		
		return true;
	}
	
	// Processor finish method
	bool FragmentDB::AddHydrogensProcessor::finish()
	{
		return true;
	}
	
	// Processor application method
	Processor::Result FragmentDB::AddHydrogensProcessor::operator () (Fragment& object)
	{
		// abort if the object is not a residue (only residues are 
		// contained in the fragment DB)																				
		if (!RTTI::isKindOf<Residue>(object))
		{
			return Processor::CONTINUE;
		}

		// cast the object to a residue
		Residue& residue = dynamic_cast<Residue&>(object);

		// get the reference fragment from the fragment DB
		const Fragment* reference_fragment = fragment_db_->getReferenceFragment(residue);

		// complain if no reference fragment could be found
		if (reference_fragment == 0)
		{
			Log.warn() << "FragmentDB::AddHydrogensProcessor: no reference fragment found for " 
							   << residue.getName() << ":" << residue.getID() << std::endl;
		}

		// check whether this reference fragment is already known
		Handle handle = reference_fragment->getHandle();
		if (!reference_fragment_h_names_.has(handle))
		{
			// this is the first time we encounter this fragment, extract all hydrogen names
			// create a new name list for the current reference fragment
			reference_fragment_h_names_.insert(pair<Handle, StringHashSet>(handle, StringHashSet()));
			StringHashSet& names = reference_fragment_h_names_[handle];
			for (AtomConstIterator atom_it = reference_fragment->beginAtom();
					+atom_it; ++atom_it)
			{
				if (atom_it->getElement() == PTE[Element::H])
				{
					// add this hydrogen name to the name list for the current reference fragment
					names.insert(atom_it->getName());
				}
			}
		}

		// get a copy of the hydrogen names occurring in the current reference fragment....
		StringHashSet names = reference_fragment_h_names_[handle];

		// ...and remove the names of existing hydrogens.
		// At the same time, we construct a hash map relating the atom names
		// of the residues with the corresponding atom pointers.
		StringHashMap<Atom*> atom_name_map;
		for (AtomIterator atom_it = residue.beginAtom(); +atom_it; ++atom_it)
		{
			if (names.has(atom_it->getName()))
			{
				names.erase(atom_it->getName());
			}

			atom_name_map.insert(atom_it->getName(), &*atom_it);
		}


		// the hash set now contains the names of the missing hydrogen atoms
		// iterate over these atom names and identify the reference atoms
		StringHashSet::Iterator missing_h_it = names.begin();
		for (; missing_h_it != names.end(); ++missing_h_it)
		{
			// identify the reference hydrogen and the atom it is bound to
				AtomConstIterator atom_it = reference_fragment->beginAtom();
			for (; +atom_it; ++atom_it)
			{
				if (atom_it->getName() == *missing_h_it)
				{
					break;
				}
			}
			const Atom& ref_hydrogen = *atom_it;
			if ((ref_hydrogen.countBonds() != 1) 
					|| (ref_hydrogen.getBond(0)->getPartner(ref_hydrogen)->getElement() == PTE[Element::H]))
			{
				Log.error() << "FragmentDB::AddHydrogensProcessor: unbound hydrogen atom "
									  << ref_hydrogen.getName() << " in template of " << reference_fragment->getName() << std::endl;
				continue;
			}
			const Atom& ref_center_atom = *ref_hydrogen.getBond(0)->getPartner(ref_hydrogen);

			// determine the equivalent atom in the current residue
			if (!atom_name_map.has(ref_center_atom.getName()))
			{
				Log.error() << "FragmentDB::AddHydrogensProcessor: could not identify a central atom for "
									  << ref_hydrogen.getName() << " in " << residue.getName() << ":" << residue.getID() << std::endl;
				continue;
			}
			Atom& center_atom = *atom_name_map[ref_center_atom.getName()];

			// Go, BFS, go!
			Quadruple<bool, const Atom*, const Atom*, const Atom*> result(getThreeReferenceAtoms_(ref_center_atom));

			const Atom* ref_atom_1 = result.second;
			const Atom* atom_1 = 0;
			static Atom dummy_ref_atom_1;
			static Atom dummy_atom_1;
			if (result.second != 0)
			{
				if (atom_name_map.has(ref_atom_1->getName()))
				{
					atom_1 = atom_name_map[ref_atom_1->getName()];					
				}
				else
				{
					Log.error() << "AddHydrogenProcessor: cannot identify reference atom for " << ref_hydrogen.getName() << endl;
					continue;
				}
			} 
			else
			{
				ref_atom_1 = &dummy_ref_atom_1;
				dummy_ref_atom_1.setPosition(ref_center_atom.getPosition() + Vector3(1.0, 0.0, 0.0));
				atom_1 = &dummy_atom_1;
				dummy_atom_1.setPosition(center_atom.getPosition() + Vector3(1.0, 0.0, 0.0));
			}
			

			const Atom* ref_atom_2 = result.third;
			const Atom* atom_2 = 0;
			static Atom dummy_ref_atom_2;
			static Atom dummy_atom_2;
			if (result.third != 0)
			{
				if (atom_name_map.has(ref_atom_2->getName()))
				{
					atom_2 = atom_name_map[ref_atom_2->getName()];
				}
				else
				{
					Log.error() << "AddHydrogenProcessor: cannot identify reference atom for " << ref_hydrogen.getName() << endl;
					continue;
				}
			} 
			else
			{
				ref_atom_2 = &dummy_ref_atom_2;
				dummy_ref_atom_2.setPosition(ref_center_atom.getPosition() + Vector3(0.0, 1.0, 0.0));
				atom_2 = &dummy_atom_2;
				dummy_atom_2.setPosition(center_atom.getPosition() + Vector3(0.0, 1.0, 0.0));
			}


			const Atom* ref_atom_3 = result.fourth;
			const Atom* atom_3 = 0;
			static Atom dummy_ref_atom_3;
			static Atom dummy_atom_3;
			if (result.fourth != 0)
			{
				if (atom_name_map.has(ref_atom_3->getName()))
				{
					atom_3 = atom_name_map[ref_atom_3->getName()];
				}
				else
				{
					Log.error() << "AddHydrogenProcessor: cannot identify reference atom for " << ref_hydrogen.getName() << endl;
					continue;
				}
			} 
			else
			{
				ref_atom_3 = &dummy_ref_atom_3;
				dummy_ref_atom_3.setPosition(ref_center_atom.getPosition() + Vector3(0.0, 0.0, 1.0));
				atom_3 = &dummy_atom_3;
				dummy_atom_3.setPosition(center_atom.getPosition() + Vector3(0.0, 0.0, 1.0));
			}


			// create a new hydrogen at an appropriate position
			PDBAtom* new_hydrogen = createNewHydrogen_(ref_hydrogen, center_atom, *atom_1, *atom_2, *atom_3,
																							ref_center_atom, *ref_atom_1, *ref_atom_2, *ref_atom_3);
			
			// check whether we could create the new hydrogen
			if (new_hydrogen != 0)
			{
				// insert the hydrogen atom
				residue.insert(*new_hydrogen);
				number_of_inserted_h_++;
				try 
				{
					new_hydrogen->createBond(center_atom);
				} 
				catch (Bond::TooManyBonds)
				{
					Log.error() << "FragmentDB::AddHydrogensProcessor: cannot create bond between "
											<< residue.getName() << ":" << new_hydrogen->getName() << " and " 
											<< center_atom.getName() << ": too many bonds!" << endl;
				}
			}
			else
			{
				// complain about it
				Log.warn() << "FragmentDB::AddHydrogensProcessor: could not create new hydrogen atom " 
									 << ref_hydrogen.getName() << std::endl;
			}
		}	
		
		return Processor::CONTINUE;

	}

	// copy constructor	
	FragmentDB::AddHydrogensProcessor::AddHydrogensProcessor(const FragmentDB& db)
	{
		fragment_db_ = &db;
	}
	
	// constructor	
	FragmentDB::AddHydrogensProcessor::AddHydrogensProcessor()
	{
		fragment_db_ = 0;
	}
	
	// destructor	
	FragmentDB::AddHydrogensProcessor::~AddHydrogensProcessor()
	{
		fragment_db_ = 0;
	}
	// returning numbers of inserted hydrogens
	Size FragmentDB::AddHydrogensProcessor::getNumberOfInsertedHydrogens()
	{
		return 0;
	}
	//	BuildBondsProcessor

	FragmentDB::BuildBondsProcessor::BuildBondsProcessor()
		:	fragment_db_(0),
			bonds_built_(0)
	{
	}

	FragmentDB::BuildBondsProcessor::BuildBondsProcessor(const FragmentDB& db)
		: fragment_db_(const_cast<FragmentDB*>(&db)),
			bonds_built_(0)
	{
	}

	FragmentDB::BuildBondsProcessor::~BuildBondsProcessor()
	{
		fragment_db_ = 0;
	}

	void FragmentDB::BuildBondsProcessor::setFragmentDB(const FragmentDB& db)
	{
		fragment_db_ = &const_cast<FragmentDB&>(db);
	}

	bool FragmentDB::BuildBondsProcessor::start()
	{
		bonds_built_ = 0;
		return true;
	}

	Size FragmentDB::BuildBondsProcessor::getNumberOfBondsBuilt()
	{
		return bonds_built_;
	}

	bool FragmentDB::BuildBondsProcessor::finish()
	{
		// if there are no inter-fragment bonds, return
		if (fragment_list_.size() >= 2)
		{

			// iterate over all pairs of fragments 
			// in the list and try to construct bonds between them
			list<Fragment*>::iterator it1 = fragment_list_.begin();
			list<Fragment*>::iterator it2 = it1;
			for (; it1 != fragment_list_.end(); ++it1)
			{
				for (it2 = it1, ++it2; it2 != fragment_list_.end(); ++it2)
				{
					Size inter_fragment_bonds = buildInterFragmentBonds(**it1, **it2);
					bonds_built_ += inter_fragment_bonds;

					// check for the special case of cyclic structures (peptides)
					Residue* res1 = dynamic_cast<Residue*>(&**it1);
					Residue* res2 = dynamic_cast<Residue*>(&**it2);
					if ((inter_fragment_bonds > 0) && (res1 != 0) && (res2 != 0)
							&& res1->isTerminal() && res2->isTerminal())
					{
						// BAUSTELLE: wie sieht es bei terminal CYS-verbrueckten 
						//   Peptiden aus?
						// assign them the CYCLIC property and thus prevent
						// getNTerminal/getCTerminal to recognize them
						// as terminal from now on
						(**it1).setProperty(Residue::PROPERTY__CYCLIC);
						(**it2).setProperty(Residue::PROPERTY__CYCLIC);
					}
				}
			}
		}

		// clear the fragment list
		fragment_list_.clear();

		return true;
	}

	Processor::Result FragmentDB::BuildBondsProcessor::operator () (Fragment& fragment)
	{
		// build all bonds in the fragment
		bonds_built_ += buildFragmentBonds(fragment);
		
		// store a pointer to the fragment in a list
		// to build all inter-fragment bonds in the finish method
		fragment_list_.push_back(&fragment);

		return Processor::CONTINUE;
	}

	Size FragmentDB::BuildBondsProcessor::buildFragmentBonds(Fragment& fragment) const
	{
		// abort immediately if no fragment DB is known
		if (fragment_db_ == 0)
		{
			return 0;
		}

		// get the fragment`s name
		String	name = fragment.getName();
		
		// check whether our DB knows the fragment and retrieve the template
		const Fragment* tplate = fragment_db_->getReferenceFragment(fragment);
		if (tplate == 0) 
		{
			return 0;
		}

		Size bond_count = 0;
		AtomConstIterator				tmp_it1;
		Atom::BondConstIterator	tmp_it2;
		BALL_FOREACH_BOND(*tplate, tmp_it1, tmp_it2)
		{
			bond_count++;
		}

		// iterate over all atoms in the tplate
		AtomConstIterator				tplate_atom_it;
		AtomIterator				frag_atom_it;
		Atom::BondConstIterator	tplate_bond_it;
		Atom*								partner;

		// count the counds we build...
		Size bonds_built = 0;

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
								if (second_frag_it->getName().trim() == name) 
								{
									Bond* bond = second_frag_it->getBond(*frag_atom_it);
									if (bond == 0)
									{
										// if the bond did not yet exist, create it											
										bond = frag_atom_it->createBond(*second_frag_it);
									}

									// assign the correct bond order, name, and type
									// (even if the bond exists -- too correct PDB CONECT entries)
									if (bond != 0)
									{
										// assign the bond type and order
										bond->setOrder(tplate_bond_it->getOrder());
										bond->setType(tplate_bond_it->getType());
										bond->setName(tplate_bond_it->getName());

										// count this bond 
										bonds_built++;
									}
									break;
								}
							}
						}
					}
				}
			}
		}

		return bonds_built;
	}

	Size FragmentDB::BuildBondsProcessor::buildInterFragmentBonds
		(Fragment& first, Fragment& second) const
	{
		if (fragment_db_ == 0)
		{
			return 0;
		}

		String first_name(first.getName());
		String second_name(second.getName());

		ResourceEntry*	first_entry = fragment_db_->tree->getEntry("/Fragments/" + first_name + "/Connections");
		if (first_entry == 0)
		{
			return 0;
		}

		ResourceEntry* second_entry = fragment_db_->tree->getEntry("/Fragments/" + second_name + "/Connections");
		if (second_entry == 0)
		{
			return 0;
		}

		// count the bonds we build
		Size bonds_built = 0;

		String	s1[6];
		String	s2[6];
		ResourceEntry::Iterator	it1 = first_entry->begin();
		ResourceEntry::Iterator	it2;
		for (++it1; +it1; ++it1)
		{
			// split the fields of the "Connections" entry.
			// It should have the following format:
			//   (<name> <atom_name> <match_name> <distance> <tolerance>)
			//	<name>:				Name of the connection type (eg C-term)
			//	<atom_name>:	Name of the atom that might create the connection
			//  <bond_order>: s/d/t/a (single/double/triple/aromatic)
			//	<match_name>:	Name of a matching connection type: this connection is 
			//								created if the two names match
			//	<distance>:		Distance of the connection in Angstrom
			//	<tolerance>:	Tolerance: connection will be built only if the distance
			//								of the two atoms within <tolerance> of <distance>
			//	Example entry:
			//		(C-term C s N-term 1.33 0.5):
			//			This will build a connection to a fragment with a N-term connection
			//			if the two atoms are 1.33+/-0.5 Angstrom apart. The bond is a single bond.
			it1->getValue().split(s1, 6);

			// check if the connection of the first fragment
			// matches any connection type of the second fragment
			for (it2 = second_entry->begin(), ++it2; +it2; ++it2)
			{
				// if the two connection types match,
				// check for distance condition and the two atoms
				if (it2->getKey() == s1[1])
				{
					it2->getValue().split(s2, 6);
					Atom* a1 = first.getAtom(s1[0]);
					Atom* a2 = second.getAtom(s2[0]);
					if ((a1 != 0) && (a2 != 0))
					{
						// check the distance conditions for both Connection data sets
						float distance = a1->getPosition().getDistance(a2->getPosition());
						if ((fabs(distance - s1[3].toFloat()) < s1[4].toFloat())
								&& (fabs(distance - s2[3].toFloat()) < s2[4].toFloat()))
						{
							// create the bond only if it does not exist
							if (!a1->isBoundTo(*a2))
							{
								Bond* bond = a1->createBond(*a2);
								// create the bond
								if (bond != 0)
								{
									// count the bond
									bonds_built++;

									// set the bond order
									switch (s1[2][0])
									{
										case 's':
											bond->setOrder(Bond::ORDER__SINGLE); break;
										case 'd':
											bond->setOrder(Bond::ORDER__DOUBLE); break;
										case 't':
											bond->setOrder(Bond::ORDER__TRIPLE); break;
										case 'a':
											bond->setOrder(Bond::ORDER__AROMATIC); break;
										default:
											Log.warn() << "FragmentDB::BuildBondsProcessor: unknown bond order " 
																 << s1[2] << " (in " << first_entry->getPath() << ")" << endl;
									}
								}
							}
						}
					}
				}
			}
		}
		
		return bonds_built;
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

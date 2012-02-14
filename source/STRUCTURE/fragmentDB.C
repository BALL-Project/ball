// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/fragmentDB.h>

#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/nucleotide.h>
#include <BALL/KERNEL/nucleicAcid.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/FORMAT/resourceFile.h>
	
/*			Things still missing (among others)
				===================================
				- check for unique atom names
				- dynamic import of databases
*/

//#define BALL_DEBUG_FRAGMENTDB

#ifdef BALL_DEBUG_FRAGMENTDB
# define DEBUG(a) Log.info() << a << std::endl;
#else
# define DEBUG(a)
#endif

#define FRAGMENT_DB_INCLUDE_TAG "#include:"

using namespace std;

namespace BALL 
{

	FragmentDB::NoFragmentNode::NoFragmentNode(const char* file, int line, const string& filename)
		: Exception::GeneralException(file, line, "NoFragmentNode", 
											 string("the resource database does not contain a valid Fragment entry: ") + filename),
			filename_(filename)
	{
	}

	void FragmentDB::expandTree_(ResourceEntry& root_entry)
	{
		bool expanded_one = true;
		while (expanded_one)
		{
			expanded_one = false;
			ResourceEntry::Iterator entry_it;
			for (entry_it = ++root_entry.begin(); +entry_it && !expanded_one; ++entry_it)
			{
				if (entry_it->getKey().hasPrefix(FRAGMENT_DB_INCLUDE_TAG))
				{
					expandFirst_(*entry_it);
					expanded_one = true;
					break;
				}
			}	
		}
	}

	bool FragmentDB::expandFirst_(ResourceEntry& root_entry)
	{
		String key = root_entry.getKey();
		vector<String> key_fields;

		if (key.split(key_fields, ":") != 2)
		{
			// if the include directive is invalid, remove the entry
			Log.error() << "FragmentDB: illegal #include directive: " << key << endl;
			root_entry.getParent()->removeChild(key, 0);
			return false;
		} 
		else 
		{
			String value_fields[2];
			String value = root_entry.getValue();
			value.split(value_fields, 2, ":");
				
			ResourceEntry*	parent = root_entry.getParent();
			parent->removeChild(key, 0);

			// search in the standard fragment DB file
			Path path;
			String filename = path.find(value_fields[0]);
			if (filename == "")
			{
				throw Exception::FileNotFound(__FILE__, __LINE__, value_fields[0]);
			}

			ResourceFile file(filename);
			if (!file.isValid())
			{
				Log.error() << "FragmentDB: cannot open include file " << value_fields[0] << endl;
				return false;
			}
				
			ResourceEntry* tree_entry = file.getRoot().getEntry(value_fields[1]);
			if (tree_entry == 0)
			{
				Log.error() << "FragmentDB: cannot find node " << value_fields[1] << " in file " << value_fields[0] << endl;
			} 
			else 
			{
				ResourceEntry* entry = parent->insertChild(key_fields[1], tree_entry->getValue());
				entry->mergeChildrenOf(*tree_entry);
			}
		}

		return true;
	}

	Position FragmentDB::addNewFragment_(Residue* fragment)
	{
		fragments_.push_back(fragment);
		return fragments_.size() - 1;
	}

	// default constructor
	FragmentDB::FragmentDB()
		: tree(0),
			valid_(false),
			filename_("")
	{
	}


	FragmentDB::FragmentDB(const String& filename)
		: tree(0),
			valid_(false),
			filename_("")
	{
		if (filename == "")
		{
			setFilename("fragments/Fragments.db");
		}
		else
		{
			setFilename(filename);
		}

		init();
	}

	FragmentDB::FragmentDB(const FragmentDB& db, bool /* deep */)
		: tree(0),
			valid_(false),
			filename_("")
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

		if (tree != 0) 
		{
			delete tree;
			tree = 0;
		}

		// Delete all hash maps.
		name_to_path_.destroy();
		name_to_frag_index_.destroy();
		name_to_variants_.destroy();
		standards_.clear();
		// Delete all fragments.
		for (std::vector<Residue*>::iterator it = fragments_.begin();
				 it != fragments_.end(); ++it)
		{
			delete *it;
			*it = 0;
		}
		fragments_.clear();		
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
		return valid_ && tree != 0;
	}

	bool FragmentDB::has(const String& fragment_name) const 
	{
		if (!isValid()) return false;

		return name_to_path_.has(fragment_name);
	}

	FragmentDB::Type FragmentDB::getFragmentType(const String& fragment_name) const 
	{
		if (!isValid() || 
				!tree->isValid() ||
				!has(fragment_name))
		{
			return FragmentDB::TYPE__UNKNOWN;
		}

		String path = (*name_to_path_.find(fragment_name)).second;
		path += "/Type";
		ResourceEntry* entry = tree->findChild(path);
		entry = tree->findChild("");
			
		if (entry!= 0)
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
		ResourceEntry::Iterator	entry_it;

		for (entry_it = ++entry.begin(); +entry_it; ++entry_it)
		{
			if (entry_it->getDepth() == entry.getDepth() + 1)
			{
				if (entry_it->getValue().countFields(" ") != 4)
				{
					Log.error() << "FragmentDB: wrong entry for atom " << entry_it->getKey() 
							 << ": " << entry_it->getValue() << endl;
				} 
				else 
				{
					// create a new atom...
					Atom*	atom = new Atom;
								
					// ...set its name and insert it into the fragment.
					atom->setName(entry_it->getKey());
					fragment.insert(*atom);
		
					// Now extract element and position (x, y, z) from the string.
					String		s[4];
					entry_it->getValue().split(s, BALL_SIZEOF_ARRAY(s), " ");
					Vector3	r(s[1].toFloat(), s[2].toFloat(), s[3].toFloat());
		
					// and assign its values to the atom
					atom->setPosition(r);
					atom->setElement(PTE.getElement(s[0]));
				}
			}
		}
	}

	void FragmentDB::parseBonds_(ResourceEntry& entry, Fragment& fragment)
	{

		ResourceEntry::Iterator	entry_it;

		for (entry_it = ++entry.begin(); +entry_it; ++entry_it)
		{
			if (entry_it->getDepth() == entry.getDepth() + 1)
			{
				// check whether the fragment contains both bonds
				Atom*	atom1 = 0;
				Atom*	atom2 = 0;
				AtomIterator	atom_it;
				
				// first field contains a serial number, field 2 the first atom, field 3 the second atom
				// and the third field (optional) the bond type (s[ingle], d[ouble], a[romatic])
				String fields[3];
				entry_it->getValue().split(fields, 3);
		
				for (atom_it = fragment.beginAtom(); +atom_it; ++atom_it)
				{
					if (atom_it->getName() == fields[0])
					{
						atom1 = &*atom_it;
					}
					if (atom_it->getName() == fields[1])
					{
						atom2 = &*atom_it;
					}
				}
		
				if ((atom1 == 0) || (atom2 == 0))
				{
					// if at least on of the atoms doesn`t exist: complain about it
					Log.error() << "FragmentDB: Bond to a non-existing atom: " 
											<< fields[0] << "-" << fields[1] 
											<< " (in " << entry_it->getPath() << ")" << endl;
				} 
				else	
				{
					// otherwise create the bond, if valences free
					if ((atom1->countBonds() > Atom::MAX_NUMBER_OF_BONDS) || (atom2->countBonds() > Atom::MAX_NUMBER_OF_BONDS))
					{
						Log.error() << "FragmentDB: too many bonds - cannot create bond: " 
												<< atom1->getName() << "-" << atom2->getName()
												<< " in fragment " << fragment.getName() 
												<< " (in " << entry_it->getPath() << ")" << endl;
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
																<< fields[2] << " (in " << entry_it->getPath() << ")" << endl;
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

		ResourceEntry::Iterator	entry_it;

		for (entry_it = ++entry.begin(); +entry_it; ++entry_it)
		{
			if (entry_it->getDepth() == entry.getDepth() + 1)
			{
				// check whether the fragment contains both bonds
				AtomIterator	atom_it;
				Atom*				atom = 0;
		
				for (atom_it = fragment.beginAtom(); +atom_it; ++atom_it)
				{
					if (atom_it->getName() == entry_it->getKey())
					{
						atom = &*atom_it;
					}
				}
		
				if (atom == 0)
				{
					// if the atom to be deleted doesn`t exist - complain about it!
					Log.error() << "FragmentDB: cannot delete non-existing atom: "
																			<< entry_it->getKey() << endl;
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

		ResourceEntry::Iterator	entry_it;

		for (entry_it = ++entry.begin(); +entry_it; ++entry_it)
		{
			if (entry_it->getDepth() == entry.getDepth() + 1)
			{
				// check whether the fragment contains both bonds
				AtomIterator	atom_it;
				Atom*				atom = 0;
		
				for (atom_it = fragment.beginAtom(); +atom_it; ++atom_it)
				{
					if (atom_it->getName() == entry_it->getKey())
					{
						atom = &*atom_it;
					}
				}
		
				if (atom == 0)
				{
					// if the atom to be renamed doesn`t exist - complain about it!
					Log.error() << "FragmentDB: cannot rename non-existing atom: "
																			<< entry_it->getKey() << endl;
				} 
				else 
				{
					// otherwise rename the atom
					atom->setName(entry_it->getValue());
				}
			}
		}
	}

	FragmentDB& FragmentDB::operator = (const FragmentDB& db)
	{
		destroy();
		filename_ = db.filename_;
		init();
		return *this;
	}

	void FragmentDB::parseProperties_(ResourceEntry&  entry, PropertyManager& property_man)
	{
		ResourceEntry::Iterator	entry_it;
		String property;
		bool invert = false;

		for (entry_it = ++entry.begin(); +entry_it; ++entry_it)
		{
			if (entry_it->getDepth() == entry.getDepth() + 1)
			{
				// Check for the most important properties: all those defined
				// in Residue::PROPERTIES
				property = entry_it->getKey();
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
				

				Property prop = std::numeric_limits<Property>::max();
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
				else if (property == "NUCLEOTIDE")
				{
					prop = Nucleotide::PROPERTY__NUCLEOTIDE;
				}

				if (prop == std::numeric_limits<Property>::max())
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
			delete resource_db;
			throw Exception::FileNotFound(__FILE__, __LINE__, filename_);
		}


		// copy the contents of the resource file into a tree
		if (tree != 0) delete tree;
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
		
		ResourceEntry::Iterator	frag_entry_it;
		ResourceEntry::Iterator	entry_it;
		for (frag_entry_it = ++(tree->getEntry("/Fragments")->begin()); 
				 +frag_entry_it; ++frag_entry_it)
		{
			if (frag_entry_it->getDepth() == 2)
			{
				// create a new fragment and assign its name
				// 
				Residue* fragment = new Residue;
				fragment->setName(frag_entry_it->getKey());

				String fragment_name = (*frag_entry_it).getKey();
						
				// insert the fragment name into the corresponding lists
				Position fragment_index = addNewFragment_(fragment);
				name_to_path_[fragment_name] = "/Fragments/" + fragment_name;
		
				// if there are no atoms in the database, something went wrong
				entry = frag_entry_it->getEntry("Atoms");
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
				entry = frag_entry_it->getEntry("Bonds");
				if (entry != 0)
				{
					parseBonds_(*entry, *fragment);
				}

				// now check for properties common to all variants of this
				// fragment (usually AMINO_ACID)
				// Each variant entry may also contain additional properties
				// or reset properties by specifying a "!" in front of the property
				// name
				entry = frag_entry_it->getEntry("Properties");
				if (entry != 0)
				{
					parseProperties_(*entry, *dynamic_cast<PropertyManager*>(fragment));
				}

				// check for all aliases (given in the Names section of the db-file)
				// and insert them into the corresponding hash maps
				ResourceEntry::Iterator entry_it;
				entry = frag_entry_it->getEntry("Names");
				if (entry != 0)
				{
					String path = "/Fragments/" + fragment_name;
					for (entry_it = ++entry->begin(); +entry_it; ++entry_it)
					{
						name_to_path_[entry_it->getKey()] = path;
						name_to_frag_index_[entry_it->getKey()] = fragment_index;
					}
				}

				// check for possible variants of this residue type
				// (keyword Variants)
				entry = frag_entry_it->getEntry("Variants");
				if (entry != 0)
				{
					ResourceEntry::Iterator variant_it;
					Residue& original_fragment(*fragment);

					bool has_default_variant = false;

					for (variant_it = ++entry->begin(); +variant_it; ++variant_it)
					{	
						if (variant_it->getDepth() == entry->getDepth() + 1)
						{
							String variant_name = variant_it->getKey();
							Residue*	variant;
							if (variant_name == "Default")
							{
								has_default_variant = true;
								variant = new Residue(original_fragment);

								//If a default variant exists, it should take the place of the
								//basis fragment. The basis fragment itself is no longer required
								fragments_[fragment_index] = variant;
								name_to_variants_[fragment_name].push_back(fragment_index);
								name_to_frag_index_[fragment_name] = fragment_index;
								name_to_path_[fragment_name] = "/Fragments/" + fragment_name + "/Variants/" + variant_name;
							} 
							else 
							{
								variant = new Residue(original_fragment);
								variant->setName(variant_name);
								Position index = addNewFragment_(variant);

								name_to_frag_index_[variant_name] = index;
								name_to_variants_[fragment_name].push_back(index);
								name_to_path_[variant_name] = "/Fragments/" + fragment_name + "/Variants/" + variant_name;
							}

							// Remember all variants of a certain fragment in a list.
							// This list is accessed via a hash map. It is required to 
							// determine the correct variant from given properties
							// (see getReferenceFragment(Fragment&), parseProperties_).
							for (entry_it = variant_it->begin(); +entry_it; ++entry_it)
							{
								if (entry_it->getDepth() == entry->getDepth() + 2)
								{
									const String& key = entry_it->getKey();
									if (key == "Atoms")
									{
										parseAtoms_(*entry_it, *variant);
									}
									else if (key == "Bonds")
									{
										parseBonds_(*entry_it, *variant);
									}
									else if (key == "Rename")
									{
										parseRename_(*entry_it, *variant);
									}
									else if (key == "Delete")
									{
										parseDelete_(*entry_it, *variant);
									}
									else if (key == "Properties")
									{
										parseProperties_(*entry_it, *dynamic_cast<PropertyManager*>(variant));
									}
								}
							}
						}
					}

					if(has_default_variant) {
						delete fragment;
					}
				}
			}
		}

		// check for entries concerning naming standards
		entry = tree->getEntry("/Names");
		if (entry != 0)
		{
			for (entry_it = ++entry->begin(); +entry_it; ++entry_it)
			{
				if (entry_it->getDepth() != 2) continue;
				
				// Create empty hash maps for both directions (to and from the standard).
				StringHashMap<String> map;
				standards_[entry_it->getKey() + "-" + entry_it->getValue()] = StringHashMap<String>();
				standards_[entry_it->getValue() + "-" + entry_it->getKey()] = StringHashMap<String>();
				StringHashMap<String>& name_map_to = standards_[entry_it->getKey() + "-" + entry_it->getValue()];
				StringHashMap<String>& name_map_from = standards_[entry_it->getValue() + "-" + entry_it->getKey()];
				
				// Fill those maps.
				ResourceEntry::Iterator	alias_iterator(++entry_it->begin());
				for (; +alias_iterator; ++alias_iterator)
				{
					name_map_to[alias_iterator->getKey()] = alias_iterator->getValue();
					name_map_from[alias_iterator->getValue()] = alias_iterator->getKey();
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
		
		normalize_names.setFragmentDB(*this);
		add_hydrogens.setFragmentDB(*this);
		build_bonds.setFragmentDB(*this);

		return;
	}


	const String& FragmentDB::getDefaultNamingStandard() const 
	{
		return default_standard_;
	}
	
	
	const Fragment* FragmentDB::getFragment(const String& fragment_name) const
	{
		const StringHashMap<Position>::ConstIterator to_find = name_to_frag_index_.find(fragment_name);
		if (to_find == name_to_frag_index_.end()) 
		{
			return 0;	
		}
		
		return fragments_[to_find->second];
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
		if (!name_to_variants_.has(s)) return 0;

		if (name_to_variants_[fragment.getName()].size() == 1)
		{
			return getFragment(s);
		}

		// now find the variant that best matches the fragment
		// This returns N/C terminal variants for fragments
		// that have the corresponding properties set or 
		// cystein variants without thiol hydrogen if the
		// disulphide bond property is set
		
		// First, check for two special properties of amino acids:
		// C_TERMINAL and N_TERMINAL 
		// They are usually not set, so set them here
		// As the fragment should be const, we store the properties
		// in a bit vector and OR them later with the fragment's properties
		BitVector	additional_properties;
		const Residue* residue = dynamic_cast<const Residue*>(&fragment);
		if (residue != 0)
		{
			if (residue->isCTerminal())
			{
				additional_properties.setBit(Residue::PROPERTY__C_TERMINAL);
			}
			if (residue->isNTerminal())
			{
				additional_properties.setBit(Residue::PROPERTY__N_TERMINAL);
			}
		}
		else
		{
			const Nucleotide* nucleotide = dynamic_cast<const Nucleotide*>(&fragment);
			if (nucleotide != 0)
			{
				if (nucleotide->is3Prime())
				{
					additional_properties.setBit(Nucleotide::PROPERTY__3_PRIME);
				}
				if (nucleotide->is5Prime())
				{
					additional_properties.setBit(Nucleotide::PROPERTY__5_PRIME);
				}
			}
			else
			{
				DEBUG(" neither residue nor nucleotide!")
			}
		}
	
		Fragment* variant = 0;
		// the number of properties that matched.
		// the fragment with the largest number of matched
		// properties is returned
		Index number_of_properties = -1;
		Index property_difference = -1;
		Index best_number_of_properties = -1;
		Index best_property_difference = 10000;

		// Iterate over all variants of the fragment and compare the properties.
		const std::list<Position>& variant_list = name_to_variants_[fragment.getName()];
		std::list<Position>::const_iterator it = variant_list.begin();
		for (; it != variant_list.end(); ++it)
		{
			// determine how many properties both have in common
			// by ANDing both bitvectors and counting ones
			BitVector props = fragment.getBitVector();
			props |= additional_properties;
			property_difference = (int)abs((int)props.countValue(true) - (int)fragments_[*it]->getBitVector().countValue(true));
			DEBUG(" props = " << props << "  bv = " << (*it)->getBitVector() << "   add = " << additional_properties)

			props &= fragments_[*it]->getBitVector();
			number_of_properties = (int)props.countValue(true);
			DEBUG(" considering variant " << (*it)->getName() << ". # properties: " << number_of_properties)

			if ((number_of_properties > best_number_of_properties)
					|| ((number_of_properties == best_number_of_properties) 
							&& (property_difference < best_property_difference)))
			{
				variant = fragments_[*it];
				best_number_of_properties = number_of_properties;
				best_property_difference = property_difference;
			}
		}

		return variant;
	}


	const Residue* FragmentDB::getResidue(const String& fragment_name) const 
	{
		const StringHashMap<Position>::ConstIterator to_find = name_to_frag_index_.find(fragment_name);
		if (to_find == name_to_frag_index_.end())
		{
			return 0;
		}
		
		return fragments_[(*to_find).second];
	}

	
	list<String> FragmentDB::getVariantNames(const String& name) const
	{
		list<String> names;

		StringHashMap<list<Position> >::ConstIterator to_find = name_to_variants_.find(name);
		if (to_find == name_to_variants_.end())  
		{
			return names;	
		}
		
		list<Position>::const_iterator it = (*to_find).second.begin();
		const list<Position>::const_iterator end_it = (*to_find).second.end();

		for (; it != end_it; it++)
		{
			names.push_back(fragments_[*it]->getName());
		}

		return names;
	}
 
	/////////////////////////////////////////////////////////////////////
	// NormalizeNamesProcessor	
	/////////////////////////////////////////////////////////////////////
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

	StringHashMap<StringHashMap<String> >& FragmentDB::getNamingStandards() 
	{
		return standards_;
	}

	const StringHashMap<String>& FragmentDB::getNamingStandard(const String& std) const
	{
		return standards_[std];
	}

	std::vector<String> FragmentDB::getAvailableNamingStandards() const
	{
		std::vector<String> result(standards_.size());

		int i = 0;
		for(StringHashMap<StringHashMap<String> >::const_iterator it = standards_.begin();
		    it != standards_.end();
		    ++it, ++i)
		{
			result[i] = it->first;
		}

		return result;
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
	bool FragmentDB::NormalizeNamesProcessor::matchName(String& res_name, String&	atom_name, const FragmentDB::NameMap&	map) const
	{
		// residue name (non const)
		res_name.trim();
		String	s[2];

		NameMap::ConstIterator it;
		it = map.find(res_name + ":*");
		if (it != map.end())
		{
			it->second.split(s, 2, ":");
			res_name = s[0];
		}
		
		// atom name (non const)
		atom_name.trim();

		bool hit = false;

		// first, try to match exactly
		it = map.find(res_name + ":" + atom_name);
		if (it != map.end())
		{
			it->second.split(s, 2, ":");
			atom_name = s[1];
			res_name = s[0];
			hit = true;
		} 
		else 
		{
			// second, try wildcard match for residue names
			it = map.find("*:" + atom_name);
			if (it != map.end())
			{
				it->second.split(s, 2, ":");
				atom_name = s[1];
				hit = true;
			}
		}

		return hit;
	}

	bool FragmentDB::NormalizeNamesProcessor::finish() 
	{
		if (fragment_db_ == 0)
		{
			return false;
		}

		const char* error_msg = "FragmentDB: cannot locate an appropriate name conversion table!";
		const String map_name = "-" + naming_standard_;

		StringHashMap<NameMap>& table = fragment_db_->getNamingStandards();

		HashMap<NameMap*, Index> usable_maps;

		for (StringHashMap<NameMap>::Iterator it = table.begin(); it != table.end(); ++it)
		{
			if (it->first.hasSubstring(map_name))
			{
				usable_maps[&it->second] = 0;
			}
		}

		if (usable_maps.size() == 0) {
			Log.error() << error_msg << endl;
			return false;
		}

		//We now sort the fragments into parent containers if available. The rational
		//is that we should get a more stable estimate of the applicable naming scheme
		//if we iterate over a set of fragments than applying naming schemes to a
		//single Fragment, as there might by errors which could lead to the selection
		//of a wrong naming scheme.
		map<AtomContainer*, list<Fragment*> > parent_containers;

		list<Fragment*>::iterator frag_it = fragments_.begin();
		while(frag_it != fragments_.end()) {
			Residue* residue = 0;
			Nucleotide* nacid = 0;

			if((residue = RTTI::castTo<Residue>(**frag_it)) && residue->getChain())
			{
				parent_containers[static_cast<AtomContainer*>(residue->getChain())].push_back(*frag_it);
				frag_it = fragments_.erase(frag_it);
			}
			else if ((nacid = RTTI::castTo<Nucleotide>(**frag_it)) && nacid->getNucleicAcid())
			{
				parent_containers[static_cast<AtomContainer*>(nacid->getNucleicAcid())].push_back(*frag_it);
				frag_it = fragments_.erase(frag_it);
			}
			else
			{
				++frag_it;
			}
		}

		const NameMap* map;

		//First deal with the remaining fragments
		for(list<Fragment*>::iterator frag_it = fragments_.begin(); frag_it != fragments_.end(); ++frag_it) {
			countHits_(usable_maps, *frag_it, OVERWRITE);
			// we found an appropriate map, so use it
			if ((map = getBestMap_(usable_maps)) != 0)
			{
				normalizeFragment_(map, *frag_it);
			}
			else
			{
				// if we couldn't find an appropriate table, complain about it!
				Log.error() << error_msg << endl;
			}
		}

		//Now look at all fragments that could be assigned to a chain
		std::map<AtomContainer*, list<Fragment*> >::iterator chain_it;
		for (chain_it = parent_containers.begin(); chain_it != parent_containers.end(); ++chain_it)
		{
			countHits_(usable_maps, chain_it->second);
			// we found an appropriate map, so use it
			if ((map = getBestMap_(usable_maps)) != 0)
			{
				normalizeFragments_(map, chain_it->second);
			}
			else
			{
				// if we couldn't find an appropriate table, complain about it!
				Log.error() << error_msg << endl;
			}

		}

		return true;
	}

	String FragmentDB::NormalizeNamesProcessor::getSuffix_(const Fragment* frag) const
	{
		// determine whether the fragment is an amino acid
		// if it is: determine the correct name for N-,C-terminal AA
		const Residue* const residue = RTTI::castTo<Residue>(*frag);
		if (residue != 0)
		{
			if (residue->isCTerminal()) return String("-C");
			if (residue->isNTerminal()) return String("-N");
		}

		return String();
	}

	bool FragmentDB::NormalizeNamesProcessor::doMatch_(String& res_name, const String& res_name_suffix, String& atom_name, const NameMap& name_map) const
	{
		// first, try to match exactly
		String match_name = res_name + res_name_suffix;
		if (res_name_suffix.isEmpty() || !matchName(match_name, atom_name, name_map))
		{
			// try to match non-terminal residues
			if (!matchName(res_name, atom_name, name_map))
			{
				match_name = "*" + res_name_suffix;
				if (res_name_suffix.isEmpty() || !matchName(match_name, atom_name, name_map))
				{
					match_name = "*";
					if (!matchName(match_name, atom_name, name_map))
					{
						return false;
					}
				}
			}
		}

		return true;
	}

	void FragmentDB::NormalizeNamesProcessor::countHits_(HashMap<NameMap*, Index>& maps, const std::list<Fragment*>& frags)
	{
		if(frags.size() == 0) {
			return;
		}

		list<Fragment*>::const_iterator it = frags.begin();
		countHits_(maps, *it, OVERWRITE);

		for (++it; it != frags.end(); ++it)
		{
			countHits_(maps, *it, ADD);
		}
	}

	void FragmentDB::NormalizeNamesProcessor::countHits_(HashMap<NameMap*, Index>& maps, const Fragment* frag, CountingMode mode)
	{
		String atom_name;
		String res_name = frag->getName();
		AtomConstIterator atom_it;
		HashMap<NameMap*, Index>::Iterator map_iterator;

		for (map_iterator = maps.begin(); map_iterator != maps.end(); ++map_iterator)
		{
			Size hit_counter = 0;
			String res_name_suffix = getSuffix_(frag);

			for (atom_it = frag->beginAtom(); +atom_it; ++atom_it)
			{
				atom_name = atom_it->getName();

				if(doMatch_(res_name, res_name_suffix, atom_name, *map_iterator->first)) {
					hit_counter++;
				}
			}

			// update hit_count for each map
			if(mode == ADD) {
				map_iterator->second += hit_counter;
			} else {
				map_iterator->second = hit_counter;
			}
		}
	}

	const FragmentDB::NameMap* FragmentDB::NormalizeNamesProcessor::getBestMap_(const HashMap<NameMap*, Index>& maps) const
	{
		// these two variables are needed to store the best map
		Index max_hits = 0;
		NameMap* result = 0;

		// look for the best map
		HashMap<NameMap*, Index>::ConstIterator map_iterator;
		for (map_iterator = maps.begin(); map_iterator != maps.end(); ++map_iterator)
		{
			if (map_iterator->second > max_hits)
			{
				max_hits = (*map_iterator).second;
				result   = (*map_iterator).first;
			}
		}

		return result;
	}

	void FragmentDB::NormalizeNamesProcessor::normalizeFragments_(const NameMap* name_map, const std::list<Fragment*>& frags)
	{
		for (list<Fragment*>::const_iterator frag_it = frags.begin(); frag_it != frags.end(); ++frag_it)
		{
			normalizeFragment_(name_map, *frag_it);
		}
	}

	void FragmentDB::NormalizeNamesProcessor::normalizeFragment_(const NameMap* name_map, Fragment* frag)
	{
		String atom_name;
		// extract the residue name
		String res_name = frag->getName();
		String res_name_suffix = getSuffix_(frag);

		// now, iterate over the fragment`s atoms
		for (AtomIterator atom_it = frag->beginAtom(); +atom_it; ++atom_it)
		{
			// get the atom name
			atom_name = atom_it->getName();

			if(doMatch_(res_name, res_name_suffix, atom_name, *name_map)) {
				atom_it->setName(atom_name);
				atom_it->getFragment()->setName(res_name);
			}
		}
	}
	/////////////////////////////////////////////////////////////////////
	//	BuildBondsProcessor
	/////////////////////////////////////////////////////////////////////

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
		bool ok = true;

		try
		{
			// if there are no inter-fragment bonds, return
			if (connections_.size() >= 2)
			{
				ConnectionList::iterator it1(connections_.begin());
				ConnectionList::iterator it2;
				for (; it1 != connections_.end(); ++it1)
				{
					for (it2 = it1, ++it2; it2 != connections_.end(); ++it2)
					{
						if ((it1->atom != 0) && (it2->atom != 0))
						{
							if (buildConnection_(*it1, *it2))
							{
								// Remember we built a bond
								bonds_built_++;
								// Remove the connection we made from the list of connections
								it1->atom = 0;	
								it2->atom = 0;
							}
						}
					}
				}
			}
		}
		catch(Exception::TooManyBonds&)
		{
			ok = false;
		}

		// Clear the connection list
		connections_.clear();
		return ok;
	}


	bool FragmentDB::BuildBondsProcessor::buildConnection_(FragmentDB::BuildBondsProcessor::Connection& con1, 
																												 FragmentDB::BuildBondsProcessor::Connection& con2)
	{
		if (con1.type_name != con2.connect_to || 
				con1.connect_to != con2.type_name)
		{
			return false;
		}

		// if the two connection types match,
		// check for distance condition and the two atoms
		const float distance = con1.atom->getPosition().getDistance(con2.atom->getPosition());
		if (fabs(con1.dist - distance) > con1.delta || 
			  fabs(con2.dist - distance) > con2.delta)
		{
			return false;
		}

		// create the bond only if it does not exist
		if (con1.atom->isBoundTo(*con2.atom)) return false;
		
		// create the bond
		Bond* const bond = con1.atom->createBond(*con2.atom);

		if (bond != 0)
		{
			bond->setOrder(con1.order);
			if (con1.order != con2.order)
			{
				Log.warn() << "FragmentDB::BuildBondsProcessor: inconsistent bond orders for connection between " 
									 << con1.atom->getFullName() << " and " << con2.atom->getFullName() << std::endl;
			}
			return true;
		}
		
		return false;
	}


	Processor::Result FragmentDB::BuildBondsProcessor::operator () (Fragment& fragment)
	{
		try
		{
			// build all bonds in the fragment
			bonds_built_ += buildFragmentBonds(fragment);
		}
		catch(Exception::TooManyBonds&)
		{
			return Processor::ABORT;
		}
			
		// store a pointer to the fragment in a list
		// to build all inter-fragment bonds in the finish method
		storeConnections_(fragment);

		return Processor::CONTINUE;
	}


	Size FragmentDB::BuildBondsProcessor::buildFragmentBonds(Fragment& fragment, const Fragment& tplate) const
	{
		// abort immediately if no fragment DB is known
		if (fragment_db_ == 0) return 0;

		DEBUG("FragmentDB::BuildBondsProcessor: building bonds for " 
							 << fragment.getName() << " from template " << tplate.getName())

		StringHashMap<const Atom*> template_names;

		for (AtomConstIterator catom_it = tplate.beginAtom(); +catom_it; ++catom_it)
		{
			const String atom_name = catom_it->getName().trim();
#ifdef BALL_DEBUG_FRAGMENTDB
			if (template_names.has(atom_name))
			{
				DEBUG("FragmentDB::BuildBondsProcessor: duplicate atom name in template " << tplate.getName())
			}
#endif
			template_names.insert(atom_name, &*catom_it);
		}
		
		// count the counds we build...
		Size bonds_built = 0;
		
		// iterate over all atoms in the tplate
		Atom::BondConstIterator	tplate_bond_it;

		// iterate over all fragment atoms 
		for (AtomIterator frag_atom_it = fragment.beginAtom(); +frag_atom_it; ++frag_atom_it) 
		{
			const String atom_name = frag_atom_it->getName().trim();
			const StringHashMap<const Atom*>::Iterator to_find = template_names.find(atom_name);
			if (to_find == template_names.end()) 
			{
				continue;	
			}
			
			const Atom* tplate_atom = to_find->second;

			// we found two matching atoms. Great! Now check their bonds..
			// iterate over all bonds of the template
			for (tplate_bond_it = tplate_atom->beginBond(); +tplate_bond_it; ++tplate_bond_it) 
			{
				const Atom* partner = tplate_bond_it->getPartner(*tplate_atom);
				// if we found the bond partner, create the new bond
				if (partner == 0) continue;
			
				const String name = partner->getName();

				// look in the fragment for the correct partner of the current bond in the template
				AtomIterator second_frag_it(fragment.beginAtom());
				for (; +second_frag_it; ++second_frag_it) 
				{
					if (second_frag_it->getName().trim() != name) continue;
				
					// ok, we found the correct partner atom
					// does the bond already exists?
					Bond* bond = second_frag_it->getBond(*frag_atom_it);
					if (bond == 0)
					{
						// no, so create it											
						bond = frag_atom_it->createBond(*second_frag_it);
					}

					// assign the correct bond order, name, and type
					// (even if the bond exists -- to correct PDB CONECT entries)
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

		return bonds_built;
	}


	Size FragmentDB::BuildBondsProcessor::buildFragmentBonds(Fragment& fragment) const
	{
		// abort immediately if no fragment DB is known
		if (fragment_db_ == 0) return 0;

		// check whether our DB knows the fragment and retrieve the template
		const Fragment* const tplate = fragment_db_->getReferenceFragment(fragment);
		if (tplate == 0) return 0;
		
		return buildFragmentBonds(fragment, *tplate);
	}

	void FragmentDB::BuildBondsProcessor::storeConnections_(Fragment& fragment)
	{
		if (fragment_db_ == 0) return;

		const ResourceEntry* first_entry = 
			fragment_db_->tree->getEntry("/Fragments/" + fragment.getName() + "/Connections");

		if (first_entry == 0) return;

		ResourceEntry::ConstIterator	it1 = first_entry->begin();
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
			
			String	s[6];
			it1->getValue().split(s, 6);
			Connection conn;
			conn.atom = 0;
			for (AtomIterator ai = fragment.beginAtom(); +ai; ++ai)
			{
				if (ai->getName() == s[0])
				{
					conn.atom = &*ai;
					break;
				}
			}
			// If there is a matching atom, store the connection.
			if (conn.atom != 0)
			{
				conn.type_name = it1->getKey();
				conn.connect_to = s[1];
				
				conn.dist = s[3].toFloat();
				conn.delta = s[4].toFloat();
				// set the bond order
				switch (s[2][0])
				{
					case 's': conn.order = Bond::ORDER__SINGLE; break;
					case 'd': conn.order = Bond::ORDER__DOUBLE; break;
					case 't': conn.order = Bond::ORDER__TRIPLE; break;
					case 'a': conn.order = Bond::ORDER__AROMATIC; break;
					default:
						Log.warn() << "FragmentDB::BuildBondsProcessor: unknown bond order " 
											 << s[2] << " (in " << first_entry->getPath() << ")" << std::endl;
				}
				
				connections_.push_back(conn);	
			}
		}
	}


	Size FragmentDB::BuildBondsProcessor::buildInterFragmentBonds(Fragment& first, Fragment& second) const
	{
		if (fragment_db_ == 0) return 0;

		const ResourceEntry* const first_entry = fragment_db_->tree->getEntry("/Fragments/" + first.getName() + "/Connections");
		if (first_entry == 0) return 0;

		const ResourceEntry* const second_entry = fragment_db_->tree->getEntry("/Fragments/" + second.getName() + "/Connections");
		if (second_entry == 0) return 0;

		// count the bonds we build
		Size bonds_built = 0;

		String s1[6], s2[6];
		ResourceEntry::ConstIterator	it1 = first_entry->begin();
		ResourceEntry::ConstIterator	it2;
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
				// do the two connection types match?
				if (it2->getKey() != s1[1]) continue;
			
				it2->getValue().split(s2, 6);
				Atom* const a1 = first.getAtom(s1[0]);
				Atom* const a2 = second.getAtom(s2[0]);
				// break if not the two atoms were found
				if (a1 == 0 || a2 == 0) continue;
			
				// check the distance conditions for both Connection data sets
				const double distance = a1->getPosition().getDistance(a2->getPosition());
				if ((fabs(distance - s1[3].toFloat()) >= s1[4].toFloat()) ||
						(fabs(distance - s2[3].toFloat()) >= s2[4].toFloat()))
				{
					continue;
				}

				// create the bond only if it does not exist
				if (a1->isBoundTo(*a2)) continue;
			
				// create the bond
				Bond* const bond = a1->createBond(*a2);
				if (bond == 0) continue;
			
				// count the bond
				bonds_built++;

				// set the bond order
				switch (s1[2][0])
				{
					case 's': bond->setOrder(Bond::ORDER__SINGLE); break;
					case 'd': bond->setOrder(Bond::ORDER__DOUBLE); break;
					case 't': bond->setOrder(Bond::ORDER__TRIPLE); break;
					case 'a': bond->setOrder(Bond::ORDER__AROMATIC); break;
					default:
						Log.warn() << "FragmentDB::BuildBondsProcessor: unknown bond order " 
											 << s1[2] << " (in " << first_entry->getPath() << ")" << endl;
				}
			}
		}
		
		return bonds_built;
	}



} // namespace BALL

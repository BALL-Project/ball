// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: buildBondsProcessor.C,v 1.5 2005/02/25 18:23:01 bertsch Exp $
//

#include <BALL/STRUCTURE/buildBondsProcessor.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/DATATYPE/hashSet.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/COMMON/limits.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/MATHS/common.h>
#include <BALL/FORMAT/resourceFile.h>
#include <BALL/STRUCTURE/geometricProperties.h>

#include <cmath>

using namespace std;

namespace BALL 
{
	const char* BuildBondsProcessor::Option::BONDLENGTHS_FILENAME = "bondlengths_filename";
	const char* BuildBondsProcessor::Option::DELETE_EXISTING_BONDS = "delete_existing_bonds";
	const char* BuildBondsProcessor::Option::REESTIMATE_BONDORDERS_RINGS = "reestimate_bondorders_rings";
	const char* BuildBondsProcessor::Default::BONDLENGTHS_FILENAME = "bond_lengths/bond_lengths.db";
	const bool  BuildBondsProcessor::Default::DELETE_EXISTING_BONDS = false;
	const bool  BuildBondsProcessor::Default::REESTIMATE_BONDORDERS_RINGS = false;
	
	BuildBondsProcessor::BuildBondsProcessor()
		: UnaryProcessor<AtomContainer>(),
			options(),
			num_bonds_(0),
			max_length_(0.0f)
	{
		readBondLengthsFromFile_();
	}

	BuildBondsProcessor::BuildBondsProcessor(const BuildBondsProcessor& bbp)
		:	UnaryProcessor<AtomContainer>(bbp),
			options(),
			num_bonds_(0),
			max_length_(bbp.max_length_)
	{
		bond_lengths_ = bbp.bond_lengths_;
		max_bond_lengths_ = bbp.max_bond_lengths_;
		min_bond_lengths_ = bbp.min_bond_lengths_;
	}

	BuildBondsProcessor::BuildBondsProcessor(const String& file_name)	throw(Exception::FileNotFound)
		:	UnaryProcessor<AtomContainer>(),
			options(),
			num_bonds_(0),
			max_length_(0.0f)
	{
		readBondLengthsFromFile_(file_name);
	}

	BuildBondsProcessor::~BuildBondsProcessor()
	{
	}

	BuildBondsProcessor& BuildBondsProcessor::operator = (const BuildBondsProcessor& bbp)
	{
		max_length_ = bbp.max_length_;
		bond_lengths_ = bbp.bond_lengths_;
		max_bond_lengths_ = bbp.max_bond_lengths_;
		min_bond_lengths_ = bbp.min_bond_lengths_;
		return *this;
	}

	bool BuildBondsProcessor::start()
	{
		num_bonds_ = 0;
		return true;
	}

	Size BuildBondsProcessor::getNumberOfBondsBuilt()
	{
		return num_bonds_;
	}

	bool BuildBondsProcessor::finish()
	{
		return true;
	}

	Processor::Result BuildBondsProcessor::operator () (AtomContainer& ac)
	{
		if (options.getBool(BuildBondsProcessor::Option::DELETE_EXISTING_BONDS))
		{
			while (ac.countBonds() != 0)
			{
				AtomIterator ait;
				Atom::BondIterator bit;
				BALL_FOREACH_BOND(ac, ait, bit)
				{
					bit->destroy();
				}
			}
		}
		
		num_bonds_ += buildBondsHashGrid3_(ac);

		estimateBondOrders_(ac);

		if (options.getBool(BuildBondsProcessor::Option::REESTIMATE_BONDORDERS_RINGS))
		{
			// TODO
		}
		
		return Processor::CONTINUE;
	}

	Size BuildBondsProcessor::buildBondsHashGrid3_(AtomContainer& ac)
	{
		Size num_bonds(0);

		// get the bounding box of ac
		BoundingBoxProcessor bbox;
		ac.apply(bbox);		
		Vector3 size = bbox.getUpper() - bbox.getLower();

		// check if size is nan
		if (Maths::isNan(size.x) || Maths::isNan(size.y) || Maths::isNan(size.z))
		{
			// now we try to find bonds with a simple method, iterating over all atom pairs
			// which is quite expensive if the atomcontainer contains a lot of atoms
			AtomIterator ait1, ait2;
			BALL_FOREACH_ATOM_PAIR(ac, ait1, ait2)
			{
				float dist = ait1->getPosition().getDistance(ait2->getPosition());
				float max_dist(0), min_dist(0);
				Size an1(ait1->getElement().getAtomicNumber());
				Size an2(ait2->getElement().getAtomicNumber());

				if (getMaxBondLength_(max_dist, an1, an2) &&
						getMinBondLength_(min_dist, an1, an2) &&
						max_dist != 0 && min_dist != 0)
				{
					if (dist <= max_dist && dist >= min_dist)
					{
						if (!ait1->isBoundTo(*ait2))
						{
							Bond * b = ait1->createBond(*ait2);
							b->setOrder(Bond::ORDER__UNKNOWN);
							num_bonds++;
						}
					}
				}
			}
			return num_bonds;
		}
		
		// build HashGrid
		HashGrid3<Atom*> grid(bbox.getLower() - Vector3(max_length_+1.0), size + Vector3((max_length_+1.0)*2), max_length_);
	
		// fill in the atom pointers into the grid
		AtomIterator a_it(ac.beginAtom());
		for (; +a_it; ++a_it)
		{
			grid.insert(a_it->getPosition(), &*a_it);
		}
	
		Atom * atom1 = 0;
		// iterate over all boxes
		HashGrid3<Atom*>::BoxIterator box_it1(grid.beginBox());
		for (; +box_it1; ++box_it1)
		{
			HashGridBox3<Atom*>::DataIterator data_it1;
			HashGridBox3<Atom*>::BoxIterator box_it2;
			HashGridBox3<Atom*>::DataIterator data_it2;
			
			// over all items of the box
			for (data_it1 = box_it1->beginData(); +data_it1; ++data_it1)
			{
				atom1 = *data_it1;
				// over all neighbor boxes
				for (box_it2 = box_it1->beginBox(); +box_it2; ++box_it2)
				{
					// over all items 
					for (data_it2 = box_it2->beginData(); +data_it2; ++data_it2)
					{
						// test every pair only once
						if (atom1->getHandle() < (*data_it2)->getHandle())
						{
							// test the distance criterion for the specific element pair
							float dist = atom1->getPosition().getDistance((*data_it2)->getPosition());
							float max_dist(0), min_dist(0);
							Size an1(atom1->getElement().getAtomicNumber());
							Size an2((*data_it2)->getElement().getAtomicNumber());

							if (getMaxBondLength_(max_dist, an1, an2) &&
									getMinBondLength_(min_dist, an1, an2) &&
									max_dist != 0 && min_dist != 0)
							{
								if (dist <= max_dist && dist >= min_dist)
								{
									if (!atom1->isBoundTo(**data_it2))
									{
										Bond * b = atom1->createBond(**data_it2);
										b->setOrder(Bond::ORDER__UNKNOWN);
										num_bonds++;
									}
								}
							}
						}
					}
				}
			}
		}
		return num_bonds;
	}

	void BuildBondsProcessor::estimateBondOrders_(AtomContainer& ac)
	{
		// iterate over all bonds
		AtomIterator ait;
		Atom::BondIterator bit;
		BALL_FOREACH_BOND(ac, ait, bit)
		{
			// set best bond order found
			if (bit->getOrder() == Bond::ORDER__UNKNOWN)
			{
				float length = bit->getLength();
				Size an1(bit->getFirstAtom()->getElement().getAtomicNumber());
				Size an2(bit->getSecondAtom()->getElement().getAtomicNumber());
				
				bit->setOrder(getNearestBondOrder_(length, an1, an2));
			}
		}
	}

	bool BuildBondsProcessor::getMinBondLength_(float& length, Size an1, Size an2)
	{
		if (min_bond_lengths_.has(an1) && min_bond_lengths_[an1].has(an2))
		{
			length = min_bond_lengths_[an1][an2];
			return true;
		}
		else
		{
			if (min_bond_lengths_.has(an2) && min_bond_lengths_[an2].has(an1))
			{
				length = min_bond_lengths_[an2][an1];
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	bool BuildBondsProcessor::getMaxBondLength_(float& length, Size an1, Size an2)
	{
		if (max_bond_lengths_.has(an1) && max_bond_lengths_[an1].has(an2))
		{
			length = max_bond_lengths_[an1][an2];
			return true;
		}
		else
		{
			if (max_bond_lengths_.has(an2) && max_bond_lengths_[an2].has(an1))
			{
				length = max_bond_lengths_[an2][an1];
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	Bond::BondOrder BuildBondsProcessor::getNearestBondOrder_(float length, Size elem1, Size elem2)
	{
		Size e1, e2;
		if (bond_lengths_.has(elem1) && bond_lengths_[elem1].has(elem2))
		{
			e1 = elem1;
			e2 = elem2;
		}
		else
		{
			if (bond_lengths_.has(elem2) && bond_lengths_[elem2].has(elem1))
			{
				e1 = elem2;
				e2 = elem1;
			}
			else
			{
				// should never occur, otherwise the input paramters file is corrupted
				Log.error() << "cannot find right bond order: " << elem1 << " " << elem2 << " " << length << endl;
				return Bond::ORDER__UNKNOWN;
			}
		}
	
		Bond::BondOrder order = Bond::ORDER__UNKNOWN;
		float min_dist(Limits<float>::max());
		HashMap<Bond::BondOrder, float> bonds = bond_lengths_[e1][e2];
		HashMap<Bond::BondOrder, float>::ConstIterator it=bonds.begin();
		for (; +it; ++it)
		{
			if (min_dist > abs(it->second-length))
			{
				min_dist = abs(it->second-length);
				order = it->first;
			}
		}
		return order;
	}

	void BuildBondsProcessor::setBondLengths(const String& file_name) throw(Exception::FileNotFound)
	{
		bond_lengths_.clear();
		min_bond_lengths_.clear();
		max_bond_lengths_.clear();
		max_length_ = 0;
		readBondLengthsFromFile_(file_name);
	}

	void BuildBondsProcessor::readBondLengthsFromFile_(const String& file_name) throw(Exception::FileNotFound)
	{

		options.setDefault(BuildBondsProcessor::Option::BONDLENGTHS_FILENAME,
													 BuildBondsProcessor::Default::BONDLENGTHS_FILENAME);
		options.setDefaultBool(BuildBondsProcessor::Option::REESTIMATE_BONDORDERS_RINGS,
													 BuildBondsProcessor::Default::REESTIMATE_BONDORDERS_RINGS);
		options.setDefaultBool(BuildBondsProcessor::Option::DELETE_EXISTING_BONDS,
													 BuildBondsProcessor::Default::DELETE_EXISTING_BONDS);

		// test file or set default file
		String filename(file_name);
		if (file_name == "")
		{
			filename = String(options.get(BuildBondsProcessor::Option::BONDLENGTHS_FILENAME));
		}
		Path path;
		String filepath = path.find(filename);
		if (filepath == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename);
		}
	
		// read the resource file
		ResourceFile * resource_db = new ResourceFile(filepath);
		if (!resource_db->isValid())
		{
			delete resource_db;
			throw Exception::FileNotFound(__FILE__, __LINE__, filepath);
		}

		// put content of file into ResourceEntry
		ResourceEntry * tree = new ResourceEntry();
		tree->mergeChildrenOf(resource_db->getRoot());
		resource_db->close();
		delete resource_db;

		// get content and test validity of root entry
		ResourceEntry* entry1;
		entry1 = tree->getRoot().findChild("BondLengthsDB");

		if (entry1 == 0)
		{
			throw Exception::ParseError(__FILE__, __LINE__, filename, "BondLengthsDB");			
		}

		// bond orders and its names in the resource file
		HashMap<String, Bond::BondOrder> name_to_order;
		name_to_order["single"] = Bond::ORDER__SINGLE;
		name_to_order["double"] = Bond::ORDER__DOUBLE;
		name_to_order["triple"] = Bond::ORDER__TRIPLE;
		name_to_order["aromatic"] = Bond::ORDER__AROMATIC;

		// iterator over the whole BondLengthDB section of the tree
		ResourceEntry::Iterator it(tree->getEntry("/BondLengthsDB")->begin());
		++it;
		for (; +it; ++it)
		{
			// test if we are in the bond partner depth
			if (it->getDepth() == 2)
			{
				Size an1 = it->getKey().toUnsignedInt();
				entry1 = it->getEntry("Partners");

				// iterate over all bond partners
				ResourceEntry::Iterator entry_it;
				for (entry_it = ++entry1->begin(); +entry_it; ++entry_it)
				{
					// test if we are in the bond length section depth
					if (entry_it->getDepth() == entry1->getDepth() + 1)
					{
						Size an2 = entry_it->getKey().toUnsignedInt();
						ResourceEntry * entry2;
						entry2 = entry_it->getEntry("BondLengths");

						// iterator over all bond lengths and orders
						ResourceEntry::Iterator bond_it;
						for (bond_it = ++entry2->begin(); +bond_it; ++bond_it)
						{
							if (bond_it->getDepth() == entry2->getDepth() + 1)
							{
								// now fill the bond_lengths, min and, max structs
								String key = bond_it->getKey();
								Bond::BondOrder order = Bond::ORDER__UNKNOWN;
								float value = bond_it->getValue().toFloat();
								if (name_to_order.has(key))
								{
									order = name_to_order[key];
									bond_lengths_[an1][an2][order] = value;
								}
								else
								{
									if (key == "max")
									{
										max_bond_lengths_[an1][an2] = value;
										if (value > max_length_)
										{
											max_length_ = value;
										}
									}
									else
									{
										if (key == "min")
										{
											min_bond_lengths_[an1][an2] = value;
										}
										else
										{
											Log.error() << "Read unknown entry: " << key << endl;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
} // namespace BALL

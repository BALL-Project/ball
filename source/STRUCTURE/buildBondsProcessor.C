// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: buildBondsProcessor.C,v 1.2 2005/02/17 15:04:25 bertsch Exp $
//

#include <BALL/STRUCTURE/buildBondsProcessor.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/DATATYPE/hashSet.h>
#include <BALL/COMMON/limits.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/FORMAT/resourceFile.h>

#include <cmath>

#define BALL_STRUCTURE_BUILDBONDSPROCESSOR_DEBUG
#undef  BALL_STRUCTURE_BUILDBONDSPROCESSOR_DEBUG

#define MAX_LENGTH_DEFECT 0.4

using namespace std;

namespace BALL 
{

	BuildBondsProcessor::BuildBondsProcessor()
		: num_bonds_(0)
	{
	}

	BuildBondsProcessor::~BuildBondsProcessor()
	{
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
		// TODO 
		// hashgrid for bigger molecules such as proteins

		readBondLengthsFromFile_();

		num_bonds_ += buildBonds_(ac);

		estimateBondOrders_(ac);
		
		return Processor::CONTINUE;
	}

	Size BuildBondsProcessor::buildBonds_(AtomContainer& ac)
	{	
		Size num_bonds(0), count(0);
		for(AtomIterator a_it1 = ac.beginAtom(); +a_it1; ++a_it1, ++count)
		{
			AtomIterator a_it2 = a_it1;
			a_it2++;
			for (; a_it2!=ac.endAtom(); ++a_it2)
			{
				float dist(a_it1->getPosition().getDistance(a_it2->getPosition()));
				
				Size an1(a_it1->getElement().getAtomicNumber());
				Size an2(a_it2->getElement().getAtomicNumber());
				
				float max_dist(0), min_dist(0);
			
				if (getMaxBondLength_(max_dist, an1, an2) && 
						getMinBondLength_(min_dist, an1, an2) &&
						max_dist != 0 && min_dist != 0)
				{
					if (dist <= (max_dist+MAX_LENGTH_DEFECT) && dist >= min_dist)
					{
						a_it1->createBond(*a_it2);
						num_bonds++;
					}
				}
			}
		}
		
		return num_bonds;
	}

	void BuildBondsProcessor::estimateBondOrders_(AtomContainer& ac)
	{
		AtomIterator ait;
		Atom::BondIterator bit;
		BALL_FOREACH_BOND(ac, ait, bit)
		{
			float length = bit->getLength();
			Size an1(bit->getFirstAtom()->getElement().getAtomicNumber());
			Size an2(bit->getSecondAtom()->getElement().getAtomicNumber());
			
			bit->setOrder(getNearestBondOrder_(length, an1, an2));
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
		readBondLengthsFromFile_(file_name);
	}

	void BuildBondsProcessor::readBondLengthsFromFile_(const String& file_name) throw(Exception::FileNotFound)
	{
		#ifdef BALL_STRUCTURE_BUILDBONDSPROCESSOR_DEBUG
		cerr << "'void BuildBondsProcessor::readBondLengthsFromFile_(const String& file_name)'" << endl;
		#endif 
		
		// TODO code cleanups, exception handling, comments

		// test file or set default file
		String filename(file_name);
		if (file_name == "")
		{
			filename = String(BALL_STRUCTURE_BUILDBONDSPROCESSOR_DEFAULT_FILE);
		}
		Path path;
		String filepath = path.find(filename);
		if (filepath == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename);
		}
		
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

		ResourceEntry* entry1;
		entry1 = tree->getRoot().findChild("BondLengthsDB");

		if (entry1 == 0)
		{
			throw Exception::ParseError(__FILE__, __LINE__, filename, "BondLengthsDB");			
		}


		// bond orders and its names int the resource file
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
				#ifdef BALL_STRUCTURE_BUILDBONDSPROCESSOR_DEBUG
				cerr << "> 1:key of depth " << it->getDepth() << ": " << it->getKey() << endl;
				#endif
				
				Size an1 = it->getKey().toUnsignedInt();
				entry1 = it->getEntry("Partners");

				// iterate over all bond partners
				ResourceEntry::Iterator entry_it;
				for (entry_it = ++entry1->begin(); +entry_it; ++entry_it)
				{
					// test if we are in the bond length section depth
					if (entry_it->getDepth() == entry1->getDepth() + 1)
					{
						#ifdef BALL_STRUCTURE_BUILDBONDSPROCESSOR_DEBUG
						cerr << "> 2:key of depth " << entry_it->getDepth() << ": " << entry_it->getKey() << endl;
						#endif 
						Size an2 = entry_it->getKey().toUnsignedInt();
						ResourceEntry * entry2;
						entry2 = entry_it->getEntry("BondLengths");

						// iterator over all bond lengths and orders
						ResourceEntry::Iterator bond_it;
						for (bond_it = ++entry2->begin(); +bond_it; ++bond_it)
						{
							if (bond_it->getDepth() == entry2->getDepth() + 1)
							{
								#ifdef BALL_STRUCTURE_BUILDBONDSPROCESSOR_DEBUG
								cerr << "> 3:key of depth " << bond_it->getDepth() << ": " << 
									bond_it->getKey() << ", value: " << 
									bond_it->getValue() << endl;
								#endif
								
								// now fill the bond_lengths, min and, max structs
								String key = bond_it->getKey();
								Bond::BondOrder order = Bond::ORDER__UNKNOWN;
								if (name_to_order.has(key))
								{
									order = name_to_order[key];
									bond_lengths_[an1][an2][order] = bond_it->getValue().toFloat();
								}
								else
								{
									if (key == "max")
									{
										max_bond_lengths_[an1][an2] = bond_it->getValue().toFloat();
									}
									else
									{
										if (key == "min")
										{
											min_bond_lengths_[an1][an2] = bond_it->getValue().toFloat();
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

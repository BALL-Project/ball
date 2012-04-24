// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <iostream>

#include <BALL/STRUCTURE/hybridisationProcessor.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/expression.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/FORMAT/parameters.h>
#include <BALL/FORMAT/parameterSection.h>
#include <BALL/MATHS/common.h>

// Qt
#include <QtXml/QtXml>
#include <QtXml/qdom.h>

//#define DEBUG 1
#undef DEBUG

using namespace std;

namespace BALL 
{
	const String HybridisationProcessor::Method::SMART_MATCHING = "smart_matching";
	const String HybridisationProcessor::Method::STRUCTURE_BASED = "structure_based";
	const String HybridisationProcessor::Method::FF_BASED = "FF_based";

	const char* HybridisationProcessor::Option::ATOM_TYPE_SMARTS_FILENAME = "atom_type_smarts_filename";
	const char* HybridisationProcessor::Default::ATOM_TYPE_SMARTS_FILENAME = "bondtyping/atomtypes.xml";
	
	const char* HybridisationProcessor::Option::ATOM_TYPE_FF_FILENAME = "atom_type_gaff_filename";
	const char* HybridisationProcessor::Default::ATOM_TYPE_FF_FILENAME = "bondtyping/GAFFbondangles.ini";
	
	const String  HybridisationProcessor::Option::METHOD = "method";
	const String  HybridisationProcessor::Default::METHOD = HybridisationProcessor::Method::SMART_MATCHING;

	HybridisationProcessor::HybridisationProcessor()
		: UnaryProcessor<AtomContainer>(),
			options(),
			num_hybridisation_states_(),
			atom_type_smarts_(),
			bond_angles_(),
			elements_()
	{
		setDefaultOptions();
		valid_ = readAtomTypeSmartsFromFile_();
		valid_ &= readAndInitBondAnglesFromFile_();
	}

	HybridisationProcessor::HybridisationProcessor(const HybridisationProcessor& hp)
		:	UnaryProcessor<AtomContainer>(hp),
			options(),
			num_hybridisation_states_(hp.num_hybridisation_states_), //TODO?
			atom_type_smarts_(hp.atom_type_smarts_),
			bond_angles_(hp.bond_angles_),
			elements_(hp.elements_),
			valid_(hp.valid_)
	{
	}

	HybridisationProcessor::HybridisationProcessor(const String& smarts_file_name, const String& gaff_angle_file_name)	throw(Exception::FileNotFound)
		:	UnaryProcessor<AtomContainer>(),
			options(),
			num_hybridisation_states_(0)
	{
		valid_ = readAtomTypeSmartsFromFile_(smarts_file_name);
		valid_ &= readAndInitBondAnglesFromFile_(gaff_angle_file_name);
	}

	HybridisationProcessor::~HybridisationProcessor()
	{
		setDefaultOptions();  // TODO: Why??
	}

	HybridisationProcessor& HybridisationProcessor::operator = (const HybridisationProcessor& hp)
	{
		valid_ = hp.valid_;
		atom_type_smarts_ = hp.atom_type_smarts_;
		num_hybridisation_states_ = hp.num_hybridisation_states_; // TODO?
		bond_angles_ = hp.bond_angles_;
		elements_ = hp.elements_;
		return *this;
	}

	bool HybridisationProcessor::start()
	{
		num_hybridisation_states_ = 0;
		return true;
	}

	Size HybridisationProcessor::getNumberOfHybridisationStatesSet()
	{
		return num_hybridisation_states_;
	}

	double HybridisationProcessor::AverageBondAngle_(Atom* a)
	{
		Atom* atom1;
		Atom* atom2;
	
		double angleSum = 0.0;
		Vector3 v1, v2;
		Size n = 0; 						// number of bonds

		Atom::BondIterator b_it1;
		Atom::BondIterator b_it2;
		for (b_it1 = a->beginBond(); +b_it1; ++b_it1)
		{
			b_it2 = b_it1;
			++b_it2;
			for (; +b_it2; ++b_it2)
			{
				atom1 = b_it1->getPartner(*a);
				atom2 = b_it2->getPartner(*a);

				v1 = atom1->getPosition() - a->getPosition();
				v2 = atom2->getPosition() - a->getPosition();

				angleSum += v1.getAngle(v2);
				++n;
			}
		}

		if (n >= 1)
			return angleSum / n * 180.0 / M_PI;
		else
			return 0.0;
	}


	Processor::Result HybridisationProcessor::operator () (AtomContainer& ac)
	{
#ifdef DEBUG				
		Log.info() << "HybridisationProcessor::operator() : \n\tmethod: " 
							 << options.get(Option::METHOD)<< "\n"
							<< "\tnum of smarts read from file: " << atom_type_smarts_.size()  << "\n" << endl; 
#endif 		

		if (options.get(Option::METHOD) == Method::SMART_MATCHING)
		{
			AtomIterator ait;
			BALL_FOREACH_ATOM(ac, ait)
			{
				// initialize with 0
				ait->setProperty("HybridisationState", 0);

#ifdef DEBUG					
		if (ait->getResidue())
		{
		 Log.info() << ait->getResidue()->getFullName();
		}
		Log.info() << " " << ait->getName() << " " << ait->getPosition() << " --> " << endl;
#endif 	

				bool found = false;
				// find the __first__ matching atom type smarts
				for (Size j = 0; !found && (j < atom_type_smarts_.size()); j++)
				{	
					Expression exp(atom_type_smarts_[j].first);
					if (exp(*ait))
					{
						ait->setProperty("HybridisationState", int(atom_type_smarts_[j].second));
						found = true;

#ifdef DEBUG		
	Log.info() 	<< "    smarts num "<< j+1 << " " << (atom_type_smarts_[j].first)<< " : " 
							<<  ait->getProperty("HybridisationState").getInt()	<< endl;
	found = false;
#endif 
					}
				}
				
				if (!found)
				{
					Log.info() << "HybridisationProcessor: No hybridisation state found for atom "; 	
					if (ait->getResidue())
					{
						Log.info() 	<< (ait->getResidue())->getFullName() << " : ";
					}
					Log.info() << ait->getFullName() << endl;
				}

			} // end of FOREACH_ATOM
		}
		else if (options.get(Option::METHOD) == Method::STRUCTURE_BASED) 
		{
			// This method is a reimplementation of the openbabel code 
			// for determination of hybridisation states as denoted 
			// in the file mol.cpp
			// 
			// openbabel divides into several passes:
			// Pass 1: Assign estimated hybridization based on avg. angles 
			// Pass 2: look for 5-member rings with torsions <= 7.5 degrees
    	//         and 6-member rings with torsions <= 12 degrees
    	//         (set all atoms with at least two bonds to sp2)
			// Pass 3: "Antialiasing" If an atom marked as sp hybrid isn't
    	//          bonded to another or an sp2 hybrid isn't bonded
    	//          to another (or terminal atoms in both cases)
    	//          mark them to a lower hybridization for now 
			//         //TODO: not sure what lower means: smaller hyb 
			//                 number or lower energy (higher hyb num)

			// Pass 1: Assign estimated hybridization based on avg. angles 
			//		angle > 155         --> 1
			//		angle e [115, 155]  --> 2
			//		angle < 115 && !H 	--> 3  all other cases :-)
			//		hydrogens:    --> 0
			
			AtomIterator ait;
			float angle;

			for (ait = ac.beginAtom(); +ait; ++ait)
			{

#ifdef DEBUG
	cout << "\n*** " << ait->getFullName() << "*******************"  << endl;
#endif
				angle = AverageBondAngle_(&*ait);

#ifdef DEBUG
	cout << " aver bond angle: " << angle ;
#endif
				
				if (angle > 155.0)
				{	
					ait->setProperty("HybridisationState", 1);
				}
				else if ( (angle <= 155.0) && (angle > 115.))
				{	
					ait->setProperty("HybridisationState", 2);
				}
				else if (ait->getElement().getName() != "Hydrogen")
				{		
					ait->setProperty("HybridisationState", 3);
				}
				else if (ait->getElement().getName() == "Hydrogen")
				{
					ait->setProperty("HybridisationState", 0);
				}
#ifdef DEBUG
	cout << " --> hyb: " << 	ait->getProperty("HybridisationState").getInt() << endl;
#endif
			}

			// Pass 2:
			// Check the rings 
			// 	 	5-rings: averaged_ring_torsion < 7.5 --> "HybridisationState" 2
			//		6-rings: averaged_ring_torsion < 12  --> "HybridisationState" 2
			//
			vector<vector<Atom*> > rings;
			RingPerceptionProcessor rpp;
			ac.apply(rpp);
			rpp.calculateSSSR(rings, ac);

			double averaged_ring_torsion; // torsion 
			Atom* b;

			for(Size i=0; i<rings.size(); ++i)
			{
				// 5-rings
				if (rings[i].size() == 5)
				{
#ifdef DEBUG
	cout << "\t 5-ring " << i << ": " << (rings[i][0])->getFullName() << " " << (rings[i][1])->getFullName()
				<< (rings[i][2])->getFullName() << " "	<< rings[i][3]->getFullName() << " "
				<< (rings[i][4])->getFullName() << endl;
#endif
					averaged_ring_torsion = 
					 (fabs(calculateTorsionAngle(*(rings[i][0]),*(rings[i][1]),*(rings[i][2]),
																			 *(rings[i][3])).toDegree()) +
						fabs(calculateTorsionAngle(*(rings[i][1]),*(rings[i][2]),*(rings[i][3]),
																			 *(rings[i][4])).toDegree()) +	
						fabs(calculateTorsionAngle(*(rings[i][2]),*(rings[i][3]),*(rings[i][4]),
																			 *(rings[i][0])).toDegree()) +	
						fabs(calculateTorsionAngle(*(rings[i][3]),*(rings[i][4]),*(rings[i][0]),
																			 *(rings[i][1])).toDegree()) +	
						fabs(calculateTorsionAngle(*(rings[i][4]),*(rings[i][0]),*(rings[i][1]),
																			 *(rings[i][2])).toDegree())
					 ) / 5.0;	

#ifdef DEBUG
	cout << "\t\t aver tor angle: " << averaged_ring_torsion;  
#endif
					if (averaged_ring_torsion <= 7.5)
					{
						for (Size j=0; j<rings[i].size(); ++j)
						{
							b = rings[i][j];
							// if an aromatic ring atom has valence 3, it is already set
              // to sp2 because the average angles should be 120 anyway
              // so only look for valence 2
							if (b->countBonds() == 2)
							{
								b->setProperty("HybridisationState", 2);
#ifdef DEBUG
	cout << "\t\t\t " <<  b->getFullName() << " --> hyb: " <<  b->getProperty("HybridisationState").getInt() << endl;  
#endif
							}
						}
					}
				} // now check the 6 rings
				else 	if (rings[i].size() == 6)
				{
#ifdef DEBUG
	cout << "\t 6-ring " << i << ": " << rings[i][0]->getFullName() << " " << rings[i][1]->getFullName()
				<< rings[i][2]->getFullName() << " "	<< rings[i][3]->getFullName() << " "
				<< rings[i][4]->getFullName() << " "	<< rings[i][5]->getFullName() <<  endl;
#endif

					averaged_ring_torsion = 
					( fabs(calculateTorsionAngle(*(rings[i][0]),*(rings[i][1]),*(rings[i][2]),
																			 *(rings[i][3])).toDegree()) +
						fabs(calculateTorsionAngle(*(rings[i][1]),*(rings[i][2]),*(rings[i][3]),
																			 *(rings[i][4])).toDegree()) +	
						fabs(calculateTorsionAngle(*(rings[i][2]),*(rings[i][3]),*(rings[i][4]),
																			 *(rings[i][5])).toDegree()) +	
						fabs(calculateTorsionAngle(*(rings[i][3]),*(rings[i][4]),*(rings[i][5]),
																			 *(rings[i][0])).toDegree()) +	
						fabs(calculateTorsionAngle(*(rings[i][4]),*(rings[i][5]),*(rings[i][0]),
																			 *(rings[i][1])).toDegree()) +
						fabs(calculateTorsionAngle(*(rings[i][5]),*(rings[i][0]),*(rings[i][1]),
																			 *(rings[i][2])).toDegree())
				  ) / 6.0; 

#ifdef DEBUG
	cout << "\t\t aver tor angle: " << averaged_ring_torsion;  
#endif

					if (averaged_ring_torsion <= 12.0)
					{
						for (Size j=0; j<rings[i].size(); ++j)
						{
							b = rings[i][j];
							if (   (b->countBonds() == 2) 
									|| (b->countBonds() == 3))
							{	
								b->setProperty("HybridisationState", 2);
#ifdef DEBUG
	cout << "\t\t\t " <<  b->getFullName() << " --> hyb: " <<  b->getProperty("HybridisationState").getInt() << endl;  
#endif
							}
						}
					}

				} 
			}// end of all rings

			// Pass 3: "Antialiasing" If an atom marked as sp hybrid isn't
    	//          bonded to another or an sp2 hybrid isn't bonded
    	//          to another (or terminal atoms in both cases)
    	//          mark them to a lower hybridization for now
			
			//
			// Make sure, that neighboring atoms have same hybridization (sp or sp2)
			//  NOTE: the default case is hybridisation 3 !!!
			//
			bool openNbr = false; // denotes whether _all_ neighbours are still free
			Atom::BondIterator b_it;
			BALL::Atom* a;
			for (ait = ac.beginAtom(); +ait; ++ait)
			{
				if (   (ait->getProperty("HybridisationState").getInt() == 2) 
						|| (ait->getProperty("HybridisationState").getInt() == 1))
				{
					openNbr = false;
					for (b_it = ait->beginBond(); +b_it; ++b_it)
					{
						// get the neighbouring atom
						a = b_it->getPartner(*ait);
						if (    (a->getProperty("HybridisationState").getInt() < 3)
								 || (a->countBonds() == 1))
						{
							openNbr = true;
							break;
						}
					}
#ifdef DEBUG	
if (!openNbr)
{
	cout << " openNbr-Correction for" <<  ait->getFullName() << " --> hyb: "; 
}
#endif
					if ((!openNbr) && (ait->getProperty("HybridisationState").getInt() == 2))
					{	
						ait->setProperty("HybridisationState", 3);

#ifdef DEBUG	
					cout	<<  ait->getProperty("HybridisationState").getInt() << endl;  
#endif
					}
					else if ((!openNbr) && (ait->getProperty("HybridisationState").getInt() == 1))
					{
						ait->setProperty("HybridisationState", 2);
#ifdef DEBUG	
					cout	<<  ait->getProperty("HybridisationState").getInt() << endl;  
#endif
					}	
				}
			}
		} // End of the structure-based method
		else if (options.get(Option::METHOD) == Method::FF_BASED) 
		{
			AtomIterator ait;
			// initialize all hybridization states with 0
			BALL_FOREACH_ATOM(ac, ait)
			{
				ait->setProperty("HybridisationState", 0);
			}
#ifdef DEBUG
			cout << "HybProc: ============================== " << endl;
#endif
			
			BALL_FOREACH_ATOM(ac, ait)
			{
				if (ait->getProperty("HybridisationState").getInt() == 0)
				{
					// Check only "free" states
					
					// Calculate the average of all estimations for a certain atom
					double av_hyb = 0.;
					Size num = 0;
					Atom::BondIterator bit1 = ait->beginBond();
					for(; +bit1; ++bit1)
					{
						Atom::BondIterator bit2 = bit1;
						++bit2;
						for(; +bit2; ++bit2)
						{
							Atom* a1 = bit1->getPartner(*ait);
							Atom* a3 = bit2->getPartner(*ait);
							float angle = calculateBondAngle(*a1, *ait, *a3);
							
							
							// Find the corresponding element combination
							String a1_sym = a1->getElement().getSymbol();
							String a2_sym = ait->getElement().getSymbol();
							String a3_sym = a3->getElement().getSymbol();
							
							multimap<float, AtomNames_> &names = (a1_sym < a3_sym) ? bond_angles_[a1_sym][a2_sym][a3_sym] : bond_angles_[a3_sym][a2_sym][a1_sym];
							
							if (names.empty())
							{
								// We have an atom combination which is not in the database.
								continue;
							}
							
							// Calculate the bond angle in the data base which fits best the real angle
							multimap<float, AtomNames_>::iterator mit = names.lower_bound(angle);
							if (mit != names.begin())
							{
								multimap<float, AtomNames_>::iterator mit2 = mit;
								--mit2;
								if (mit != names.end())
								{
									if (angle - mit2->first < mit->first - angle)
									{
										// mit2 fits best
										mit = mit2;
									}
								}
								else
								{
									mit = mit2;
								}
							}
							
#ifdef DEBUG
							cout << "HybProc**: " << Angle(angle).toDegree() << " " << a1_sym << " " << a2_sym << " " << a3_sym << " " << (int)elements_[mit->second.a2].hyb << endl;
#endif
							
							// Take the atom types and assign the hybridization
							av_hyb += (double)elements_[mit->second.a2].hyb;
							++num;
							
							if (a1->countBonds() == 1)
							{
								if (a1_sym < a3_sym)
								{
									a1->setProperty("HybridisationState", elements_[mit->second.a1].hyb);
								}
								else
								{
									a1->setProperty("HybridisationState", elements_[mit->second.a3].hyb);
								}
							}
							
							if (a3->countBonds() == 1)
							{
								if (a1_sym < a3_sym)
								{
									a3->setProperty("HybridisationState", elements_[mit->second.a3].hyb);
								}
								else
								{
									a3->setProperty("HybridisationState", elements_[mit->second.a1].hyb);
								}
							}
							
						}
					}
					
					// Estimated (averaged) hybridization
					if (num != 0)
					{
						av_hyb /= (double)num;
					}
					
#ifdef DEBUG
					cout << "HybProc: " << ait->getName() << " " << av_hyb << endl;
#endif
					
					// Round the state
					int hyb = (int)Maths::round(av_hyb);
					ait->setProperty("HybridisationState", hyb);
				}
			}
		}
		return Processor::BREAK;
	}

	void HybridisationProcessor::setAtomTypeSmarts(const String& file_name) 
		throw(Exception::FileNotFound)
	{
		atom_type_smarts_.clear();
		valid_ = readAtomTypeSmartsFromFile_(file_name);
	}
	
	bool HybridisationProcessor::readAndInitBondAnglesFromFile_(const String& file_name) 
		throw (Exception::FileNotFound)
	{
		// test file or set default file
		String filename(file_name);
		if (file_name == "")
		{
			filename = String(options.get(HybridisationProcessor::Option::ATOM_TYPE_FF_FILENAME));
		}
		
		Path path;

		String filepath = path.find(filename);
		if (filepath == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename);
		}
		
		// Read the contents from the ini file
		Parameters parameters(filepath);
		parameters.init();
		ParameterSection ele_types;
		if (!ele_types.extractSection(parameters, "AtomTypes"))
		{
			Log.error() << "HybridisationProcessor::extractSection: didn't find section for AtomTypes" << endl;
			return false;
		}
    // Read the atom types and corresponding elements
		Size index_el = ele_types.getColumnIndex("element");
		Size index_hyb = ele_types.getColumnIndex("hybridization");
		for(Size i = 0; i < ele_types.getNumberOfKeys(); ++i)
		{
			Elements_ el;
			el.type = ele_types.getValue(i, index_el);
			el.hyb = (unsigned char)atoi(ele_types.getValue(i,index_hyb).c_str());
			elements_[ele_types.getKey(i)] = el;
		}

		// Read the combinations of bend angles
		ParameterSection angles;
		if (!angles.extractSection(parameters, "BondAngles"))
		{
			Log.error() << "HybridisationProcessor::extractSection: didn't find section for BondAngles" << endl;
			return false;
		}
		
		String    fields[4];
		Size index_r = angles.getColumnIndex("theta");
		for(Size i = 0; i < angles.getNumberOfKeys(); ++i)
		{
			String key = angles.getKey(i);
			// Split the key into its three parts
			if (key.split(fields, 3) == 3)
			{
				StringHashMap<Elements_>::Iterator a1 = elements_.find(fields[0]);
				if (a1 == elements_.end())
				{
					Log.error() << "Could not find atom type " << a1->first << endl;
					return false;
				}
				StringHashMap<Elements_>::Iterator a2 = elements_.find(fields[1]);
				if (a2 == elements_.end())
				{
					Log.error() << "Could not find atom type " << a2->first << endl;
					return false;
				}
				StringHashMap<Elements_>::Iterator a3 = elements_.find(fields[2]);
				if (a3 == elements_.end())
				{
					Log.error() << "Could not find atom type " << a3->first << endl;
					return false;
				}
				AtomNames_ atom;
				atom.a2 = a2->first;
				if (a1->first < a3->first)
				{
					atom.a1 = a1->first;
					atom.a3 = a3->first;
					bond_angles_[elements_[atom.a1].type][elements_[atom.a2].type][elements_[atom.a3].type].insert(make_pair((float)atof(angles.getValue(i, index_r).c_str()), atom));
				}
				else
				{
					atom.a3 = a1->first;
					atom.a1 = a3->first;
					bond_angles_[elements_[atom.a3].type][elements_[atom.a2].type][elements_[atom.a1].type].insert(make_pair((float)atof(angles.getValue(i, index_r).c_str()), atom));
				}
			}
		}
		return true;
	}
	

	bool HybridisationProcessor::readAtomTypeSmartsFromFile_(const String& file_name) 
		throw(Exception::FileNotFound)
	{
		// test file or set default file
		String filename(file_name);
		if (file_name == "")
		{
			filename = String(options.get(HybridisationProcessor::Option::ATOM_TYPE_SMARTS_FILENAME));
		}

		Path path;

		String filepath = path.find(filename);
		if (filepath == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename);
		}
		
		QString errorStr;
		int errorLine;
		int errorColumn;

		QFile file((filepath.c_str()));
		if (!file.open(QFile::ReadOnly | QFile::Text)) 
		{
			Log.error() << "HybridisationProcessor: cannot read file " << filename << std::endl;
			Log.error() << "Reason was: " << file.errorString().toAscii().constData() << std::endl;
			return 1;
		}

		// read the document
		QDomDocument domDocument;
		if (!domDocument.setContent(&file, true, &errorStr, &errorLine,
					&errorColumn)) 
		{
			Log.error() << "Parse error in line " << errorLine << " column " << errorColumn 
									<<  " of file " << filename << endl;
			Log.error() << "Reason was: " << errorStr.toAscii().constData() << std::endl;
			return 1;
		}
		// get the root element...
		QDomElement root = domDocument.documentElement();	
		
		// ... and get all entries
		QDomNodeList entries = domDocument.elementsByTagName("entry");

		for (unsigned int i= 0; i < entries.length(); i++)
		{
			pair<String, Size> tmp;

			// get the smart expression (tag smartstring) 
			// NOTE: each entry should have just ONE smart expression)
			QDomNodeList smartstrings = entries.item(i).toElement().elementsByTagName("smartstring");
			
			if (smartstrings.length() == 1)
			{
				tmp.first = (smartstrings.item(0).toElement().firstChild().nodeValue()).toAscii().constData();	
			} 
			else if (smartstrings.length() == 0)
			{
					Log.warn() << "HybridisationProcessor: Parse error in file " << filename 
										<< " : no SMARTS-string found in entry " 
										<< i << endl;
			}  
			else
			{
					Log.error() <<  "HybridisationProcessor: Parse error in file " << filename 
											<< " : more than one item in entry " << i << endl;
			}

			// now read the corresponding hybridisation state
			QDomNodeList hyb_state =  entries.item(i).toElement().elementsByTagName("hybridisationstate");
			if (hyb_state.length() == 1)
			{	
				tmp.second = (hyb_state.item(0).toElement().firstChild().nodeValue()).toInt();
			}
			else
			{
					Log.error() << "HybridisationProcessor: Parse error in file " << filename 
											<< " : no hybridisation state found for entry " 
											<< i << " : " << tmp.first << endl;
			}
			
			atom_type_smarts_.push_back(tmp);

		} // next entry	

		return true;
	}
	
	void HybridisationProcessor::setDefaultOptions()
	{
		options.setDefault(HybridisationProcessor::Option::ATOM_TYPE_SMARTS_FILENAME,
											 HybridisationProcessor::Default::ATOM_TYPE_SMARTS_FILENAME);
		options.setDefault(HybridisationProcessor::Option::ATOM_TYPE_FF_FILENAME,
											 HybridisationProcessor::Default::ATOM_TYPE_FF_FILENAME);
		options.setDefault(HybridisationProcessor::Option::METHOD,
											 HybridisationProcessor::Default::METHOD);
	}

} // namespace BALL

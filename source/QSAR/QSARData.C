// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#include <BALL/QSAR/QSARData.h>

#include <BALL/STRUCTURE/molecularSimilarity.h>

#include <set>
#include <algorithm>

#include <boost/random/mersenne_twister.hpp>

using namespace std;

namespace BALL
{
	namespace QSAR
			{

		QSARData::QSARData()
		{
			data_folder_ = "QSAR/";
		}

		QSARData::~QSARData()
		{
			
		}

		bool QSARData::isDataCentered() const
		{
			return !descriptor_transformations_.empty();
		}

		bool QSARData::isResponseCentered() const
		{
			return !y_transformations_.empty();
		}


		void QSARData::setDataFolder(const char* folder)
		{
			data_folder_ = folder;
		}

		void QSARData::readSDFile(const char* file)
		{
			string f = file;
			string a = f.substr(0, f.size()-4);
			a = a+".txt";
			
			descriptor_matrix_.clear();
			column_names_.clear();
			substance_names_.clear();
			Y_.clear();
			descriptor_transformations_.clear(); 
			y_transformations_.clear(); 
			class_names_.clear();
			descriptor_matrix_.resize(60);
			column_names_.resize(60, "");
			Y_.resize(1);
			int no_properties = 0;
			SDFile input(file);

			ifstream activity(a.c_str()); 
			

			for (int n = 0; input && activity; n++)
			{
				Molecule m;
				try
				{
					input>>m;
				}
				catch(BALL::Exception::ParseError e)
				{
					throw Exception::WrongFileFormat(__FILE__, __LINE__, file);
				}
				
				String d;
				//activity >> d;
				getline(activity, d);
				if (d.compare("") == 0) { break; }
				if (n == 0)
				{
					no_properties = m.countNamedProperties();
					std::multiset<int> l;
					setDescriptorNames(m, l);
					descriptor_matrix_.resize(60+no_properties);
				}
				substance_names_.push_back(m.getName());
				
				if ((int)m.countNamedProperties() != no_properties)
				{
					String e="properties are missing for"; 
					e = e+m.getName()+"; "+String(m.countNamedProperties())+" properties given, but "+String(no_properties)+" needed!";
					throw Exception::PropertyError(__FILE__, __LINE__, file, n, e.c_str());
				}
				
				for (int i = 0; i < no_properties; i++) // add all external descriptors for current molecule
				{
					descriptor_matrix_[60+i].push_back(String(m.getNamedProperty(i).getString()).toDouble());
				}	
				
				calculateBALLDescriptors(m);
				
				//cout<<d.getField(0, "\t")<<endl;
				Y_[0].push_back(atof(d.getField(0, "\t").c_str()));
			}
		}


		void QSARData::checkActivityIDs(std::multiset<int>& act, int no_properties)
		{	
			std::multiset<int>::iterator a_it = act.begin();
			// No response variable in case of data set whose activity is to be predicted
			if (act.empty() || ((*a_it == -1) && (act.size() == 1)))
			{									
				return;
				//throw Exception::InvalidActivityID(__FILE__, __LINE__);
			}
			
			for (; a_it != act.end(); ++a_it)
			{
				if ((*a_it) < 0 || (*a_it) >= no_properties)
				{
					throw Exception::InvalidActivityID(__FILE__, __LINE__, (*a_it), no_properties);
				}
			}
		}



		vector<BALL::String>* QSARData::readPropertyNames(String sd_file)
		{
			SDFile input(sd_file);
			Molecule m;
			input >> m;
			int no = m.countNamedProperties();
			
			vector<String>* names = new vector<String>;
			names->resize(no);
			
			for (int i = 0; i < no; i++)
			{
				(*names)[i] = m.getNamedProperty(i).getName();
			}
			input.close();
			return names;
		}


		void QSARData::readSDFile(const char* file, set<String>& activity_names, bool useExDesc, bool append, bool translate_class_labels, bool calc_phychem_properties, bool calc_topological_properties)
		{
			int no_phychem_descr = 60*calc_phychem_properties;
			int no_topologiocal_descr = 0;
			MolecularSimilarity* molsim = 0;
			if(calc_topological_properties)
			{
				molsim = new MolecularSimilarity("fragments/functionalGroups.smarts");
				no_topologiocal_descr = molsim->getFunctionalGroupNames().size();
			}
			int no_internal_descriptors = no_phychem_descr+no_topologiocal_descr;

			if(!append)
			{
				descriptor_matrix_.clear();
				column_names_.clear();
				substance_names_.clear();
				Y_.clear();
				descriptor_transformations_.clear();
				y_transformations_.clear();
				descriptor_matrix_.resize(no_internal_descriptors);
				column_names_.resize(no_internal_descriptors,"");
				class_names_.clear();
			}

			string f=file;
			SDFile input(f);

			int no_properties=0; // no of descriptors as determined by the first substance in the sd-file
			int no_descriptors=0;
			std::multiset<int> newInvalidDescriptors; // invalid descriptors of the current input file only; save no. of columns
			std::multiset<int> newInvalidSubstances;
			std::multiset<int> tmp; // invalid descriptors of the current input file; save no. of external descriptor

			map<String,int> descriptor_map; // map descriptor-names to their column-index in descriptor_matrix_
			map<String,int> activity_map; // map activity-names to their column-index in Y_

			// read all molecules in the sd-file
			for(int n=0; input.getSize()!=0; n++)
			{
				Molecule* mol;

				try
				{
					mol=input.read();
				}
				catch(BALL::Exception::ParseError e)
				{
					throw Exception::WrongFileFormat(__FILE__,__LINE__,file);
				}

				int no=mol->countNamedProperties();

				if (n==0) // for the first substance
				{
					no_descriptors=no-activity_names.size();
					no_properties=no;

					if(append)
					{
						if(no_internal_descriptors+useExDesc*no_descriptors!=(int)(descriptor_matrix_.size()+invalidDescriptors_.size()))
						{
							throw Exception::PropertyError(__FILE__,__LINE__, file, n, "substances to append must have the same number of descriptors than the rest of the data!!");
						}
						if(activity_names.size()!=Y_.size())
						{
							throw Exception::PropertyError(__FILE__,__LINE__, file, n, "substances to append must have the same number of activities than the rest of the data!!");
						}
					}
					else
					{
						std::multiset<int> dummy;
						if(calc_phychem_properties) setDescriptorNames(*mol,dummy,false,false);
						if(calc_topological_properties)
						{
							const vector<String>& group_names = molsim->getFunctionalGroupNames();
							for(Size i=0; i<group_names.size(); i++)
							{
								column_names_[no_phychem_descr+i] = group_names[i];
							}
						}

						if(useExDesc)
						{
							for(int i=0; i<no_properties;i++)
							{
								String name = mol->getNamedProperty(i).getName();
								if(activity_names.find(name)!=activity_names.end()) continue;
								Column c;
								descriptor_matrix_.push_back(c);
								descriptor_map.insert(make_pair(name,no_internal_descriptors+i));
								column_names_.push_back(name);
							}
						}
						Y_.resize(activity_names.size());
					}

					Size i=0;
					for(set<String>::iterator it=activity_names.begin(); it!=activity_names.end(); it++, i++)
					{
						activity_map.insert(make_pair(*it,i));
					}

					for(Size i=0; i<column_names_.size(); i++)
					{
						descriptor_map.insert(make_pair(column_names_[i],i));
					}
				}

				substance_names_.push_back(mol->getName());

				for(int i=0; i<no;i++) // create descriptors and activities for current molecule
				{
					String name = mol->getNamedProperty(i).getName();

					/// add response value
					if(activity_names.find(name)!=activity_names.end())
					{
						map<String,int>::iterator it = activity_map.find(name);
						try
						{
							double response_value = String(mol->getProperty(name).getString()).toDouble();

							if(translate_class_labels)
							{
								map<String,int>::iterator it2=class_names_.find(name);
								if(it2!=class_names_.end()) response_value = it2->second;
								else // assign ID for new class label
								{
									int id = class_names_.size();
									class_names_.insert(make_pair(name,id));
									response_value = id;
								}
							}

							Y_[it->second].push_back(response_value);
						}
						catch(BALL::Exception::InvalidFormat g)
						{
							Y_[it->second].push_back(0);
							if(invalidSubstances_.find(n) == invalidSubstances_.end())
							{
								newInvalidSubstances.insert(n);
							}
						}
					}

					/// add descriptor value
					else if(useExDesc)
					{
						map<String,int>::iterator it = descriptor_map.find(name);
						if(it!=descriptor_map.end()) // features that appear only for some molecules, but not for the first one, are ignored!
						{
							try
							{
								descriptor_matrix_[it->second].push_back(String(mol->getProperty(name).getString()).toDouble());
							}
							// descriptors with invalid entries will be removed ...
							catch(BALL::Exception::InvalidFormat g)
							{
								descriptor_matrix_[it->second].push_back(0);
								if(newInvalidDescriptors.find(it->second) == newInvalidDescriptors.end())
								{
									newInvalidDescriptors.insert(it->second);
								}
							}
						}
					}
				}

				/// If desired, calculate descriptors
				if(calc_phychem_properties) calculateBALLDescriptors(*mol); // calculate BALL-descriptors
				if(calc_topological_properties) calculateTopologicalDescriptors(*mol,*molsim,descriptor_map);

				/// Mark descriptors as invalid if it appeared for the first molecule but not for the current one
				for(int p=no_internal_descriptors; p<descriptor_matrix_.size(); p++)
				{
					if(descriptor_matrix_[p].size()<n+1)
					{
						descriptor_matrix_[p].push_back(0);
						if(newInvalidDescriptors.find(p) == newInvalidDescriptors.end())
						{
							newInvalidDescriptors.insert(p);
						}
					}
				}

				/// Mark current compound as invalid if activity values are missing
				for(int act=0; act<Y_.size(); act++)
				{
					if(Y_[act].size()<n+1)
					{
						Y_[act].push_back(0);
						if(invalidSubstances_.find(n) == invalidSubstances_.end())
						{
							newInvalidSubstances.insert(n);
						}
					}
				}

				delete mol; // delete the completly processed molecule!
			}

			removeInvalidDescriptors(newInvalidDescriptors);
			removeInvalidSubstances(newInvalidSubstances);
			invalidSubstances_=newInvalidSubstances;
			delete molsim;
		}


		void QSARData::readSDFile(const char* file, std::multiset<int>& activity_IDs, bool useExDesc, bool append, bool translate_class_labels)
		{
			if (!append)
			{
				descriptor_matrix_.clear();
				column_names_.clear();
				substance_names_.clear();
				Y_.clear();
				descriptor_transformations_.clear(); 
				y_transformations_.clear(); 
				descriptor_matrix_.resize(60);
				column_names_.resize(60, "");
				class_names_.clear();
			}

			string f = file;
			SDFile input(f);
			
			int no_properties = 0; // no of descriptors as determined by the first substance in the sd-file
			int no_descriptors = 0;
			//int no_substances = (int)input.getSize();
			std::multiset<int> newInvalidDescriptors; // invalid descriptors of the current input file only; save no. of columns
			std::multiset<int> newInvalidSubstances; 
			std::multiset<int> tmp; // invalid descriptors of the current input file; save no. of external descriptor

			// read all molecules in the sd-file
			for (int n = 0; input.getSize() != 0; n++) 
			{	
				Molecule* m;
				
				try
				{
					m = input.read();
				}
				catch(BALL::Exception::ParseError e)
				{
					throw Exception::WrongFileFormat(__FILE__, __LINE__, file);
				}
						
				int no = m->countNamedProperties();

				if (n == 0) // for the first substance
				{	
					no_descriptors = no-activity_IDs.size();
					no_properties = no;
					checkActivityIDs(activity_IDs, no_properties);
					
					if (append)
					{	
						if (60+useExDesc*no_descriptors != (int)(descriptor_matrix_.size()+invalidDescriptors_.size()))
						{
							throw Exception::PropertyError(__FILE__, __LINE__, file, n, "substances to append must have the same number of descriptors than the rest of the data!!");
						}	
						if (activity_IDs.size() != Y_.size())
						{	
							throw Exception::PropertyError(__FILE__, __LINE__, file, n, "substances to append must have the same number of activities than the rest of the data!!");
						}	
					}	
					else
					{				
						setDescriptorNames(*m, activity_IDs, useExDesc);  // set names of all descriptors
						// resize descriptor_matrix_ for first substance
						for (int i = 0; useExDesc && i < no_descriptors; i++)
						{
							Column c;
							descriptor_matrix_.push_back(c);
						}
						Y_.resize(activity_IDs.size());
					}
				}

				substance_names_.push_back(m->getName());

				// if some substance has not the same number of properties as the first substance
				if (useExDesc && no != no_properties)
				{
					throw Exception::PropertyError(__FILE__, __LINE__, file, n, "properties are missing!");
				}
				
				int act = 0; // no of current activity
				int des = 60; // no of column for current external descriptors within descriptor_matrix_
				std::multiset<int>::iterator act_it = activity_IDs.begin();
				std::multiset<int>::iterator inv_it = invalidDescriptors_.begin();
				
				for (int i = 0; i < no && i < no_properties; i++) // create descriptors and activities for current molecule
				{	 
					if (useExDesc && (act_it == activity_IDs.end() || *act_it != i)) // read external descriptors from molecule
					{
						if (invalidDescriptors_.empty() || inv_it == invalidDescriptors_.end() || *inv_it != i) // do not add values of descriptors indentified as invalid by the *last* run of getData()
						{	
							try				
							{	
								descriptor_matrix_[des].push_back(String(m->getNamedProperty(i).getString()).toDouble());
							}
							// descriptors with invalid entries will be removed ...
							catch(BALL::Exception::InvalidFormat g)
							{
								descriptor_matrix_[des].push_back(0);
								if (tmp.find(i) == tmp.end())
								{
									tmp.insert(i);
								}
								if (newInvalidDescriptors.find(des) == newInvalidDescriptors.end())
								{
									newInvalidDescriptors.insert(des);
								}
							}
							des++;
						}
						else if (inv_it != invalidDescriptors_.end() && *inv_it == i)
						{
							inv_it++;
						}
					}
					else if (act_it != activity_IDs.end() && *act_it == i) // read activities
					{
						if (!translate_class_labels)
						{
							try
							{
								Y_[act].push_back(String(m->getNamedProperty(i).getString()).toDouble());
								act++; act_it++;
							}
							catch(BALL::Exception::InvalidFormat g) 
							{
								//String a="property '";
								//a = a + m.getNamedProperty(i).getString() + "' is no numerical value!";
								//throw Exception::PropertyError(__FILE__, __LINE__, file, n, a.c_str());
								Y_[act].push_back(0);
								if (invalidSubstances_.find(n) == invalidSubstances_.end())
								{
									newInvalidSubstances.insert(n);
								}
							}
						}
						else
						{
							String value = String(m->getNamedProperty(i).getString());
							map<String, int>::iterator it = class_names_.find(value);
							if (it != class_names_.end())
							{
								Y_[act].push_back(it->second);
							}
							else
							{
								// assign ID for new class label
								int id = class_names_.size();
								Y_[act].push_back(id);
								class_names_.insert(make_pair(value, id));
							}
						}
					}
				}
				calculateBALLDescriptors(*m); // calculate BALL-descriptors
				delete m; // delete the completly processed molecule!
			}

			removeInvalidDescriptors(newInvalidDescriptors);
			removeInvalidSubstances(newInvalidSubstances);
			invalidDescriptors_ = tmp;
			invalidSubstances_ = newInvalidSubstances;
		}


		void QSARData::removeInvalidDescriptors(std::multiset<int>& inv)
		{
			std::multiset<int>::iterator inv_it = inv.begin();
			
			bool rm_names = 0;
			if (column_names_.size() == descriptor_matrix_.size()) rm_names = 1; 
			else cout<<column_names_.size()<<"  "<<descriptor_matrix_.size()<<endl;
			
			for (int i = 0; inv_it != inv.end(); ++i, ++inv_it)
			{
				int pos = *inv_it-i; // i already deleted columns --> descriptor_matrix_ is i columns shorter
				vector<vector<double> >::iterator del = descriptor_matrix_.begin()+pos;
				descriptor_matrix_.erase(del);
				
				string n="";
				if (rm_names) // do only if names of descriptors have been set
				{
					vector<string>::iterator sdel = column_names_.begin()+pos;
					n = *sdel;
					column_names_.erase(sdel);
				}
				cout << "deleted descriptor #"<<pos+i<<", \""<<n<<"\", due to invalid values!"<<endl;
			}
		}


		void QSARData::removeInvalidSubstances(std::multiset<int>& inv)
		{    
			std::multiset<int>::iterator inv_it = inv.begin();
			for (int i = 0; inv_it != inv.end(); ++i, ++inv_it)
			{
				int pos = *inv_it - i; // i already deleted rows --> descriptor_matrix_ is i rows shorter
			
				for (unsigned int j = 0; j < descriptor_matrix_.size(); j++)
				{
					vector<double>::iterator del = descriptor_matrix_[j].begin()+pos;
					descriptor_matrix_[j].erase(del);
				}
				
				for (unsigned int j = 0; j < Y_.size(); j++)
				{
					vector<double>::iterator del = Y_[j].begin()+pos;
					Y_[j].erase(del);
				}
				cout << "deleted substance "<<pos+i<<" due to invalid values!"<<endl;
			}
		}


		void QSARData::setDescriptorNames(const Molecule& m, std::multiset<int>& activity_IDs, bool useExDesc, bool resize)
		{
			if (useExDesc)
			{
				int n = m.countNamedProperties();
				int cols = 60+n-activity_IDs.size();
				if (cols < 60)
				{
					cols = 60;
				}
				column_names_.resize(cols, "");  //60 BALL-descriptors + m-1 descriptors read from the sd-file
				
				
				// set the names of the descriptor read from the sd-file
				int p = 0;
				for (int i = 60; i < cols; )
				{
					if (activity_IDs.find(p) != activity_IDs.end())
					{ 
						p++;
						continue;
					}
					column_names_[i] = m.getNamedProperty(p).getName();
					p++;
					i++;
				}
			}
			else if(resize)
			{
				column_names_.resize(60);
			}

			// set names of BALL-descriptors
			column_names_[0]="AtomicPolarizabilities";
			column_names_[1]="AtomInformationContent"; 
			column_names_[2]="BondPolarizabilities";
			column_names_[3]="FormalCharge";
			column_names_[4]="MeanAtomInformationContent"; 
			column_names_[5]="MolecularWeight";
			column_names_[6]="NumberOfAromaticAtoms";
			column_names_[7]="NumberOfAromaticBonds";
			column_names_[8]="NumberOfAtoms";
			column_names_[9]="NumberOfBonds";
			column_names_[10]="NumberOfBoron";
			column_names_[11]="NumberOfBromine";
			column_names_[12]="NumberOfCarbon";
			column_names_[13]="NumberOfChlorine";
			column_names_[14]="NumberOfDoubleBond";
			column_names_[15]="NumberOfFlourine";
			column_names_[16]="NumberOfHeavyAtoms";
			column_names_[17]="NumberOfHeavyBonds";
			column_names_[18]="NumberOfHydrogen";
			column_names_[19]="NumberOfHydrogenBondAcceptors";
			column_names_[20]="NumberOfHydrogenBondDonors";
			column_names_[21]="NumberOfHydrophobicAtoms";
			column_names_[22]="NumberOfIodine";
			column_names_[23]="NumberOfNitrogen";
			column_names_[24]="NumberOfOxygen";
			column_names_[25]="NumberOfPhosphorus";
			column_names_[26]="NumberOfRotatableBonds";
			column_names_[27]="NumberOfRotatableSingleBonds";
			column_names_[28]="NumberOfSingleBonds";
			column_names_[29]="NumberOfSulfur";
			column_names_[30]="NumberOfTripleBonds";
			column_names_[31]="PrincipalMomentOfInertia";
			column_names_[32]="PrincipalMomentOfInertiaX";
			column_names_[33]="PrincipalMomentOfInertiaY";
			column_names_[34]="PrincipalMomentOfInertiaZ";
			column_names_[35]="RelNumberOfRotatableBonds";
			column_names_[36]="RelNumberOfRotatableSingleBonds";
			column_names_[37]="SizeOfSSSR";
			column_names_[38]="VertexAdjacency";
			column_names_[39]="VertexAdjacencyEquality";
			column_names_[40]="BalabanIndexJ";
			column_names_[41]="ZagrebIndex";
			column_names_[42]="RelNegativePartialCharge";
			column_names_[43]="RelPositivePartialCharge";
			column_names_[44]="TotalNegativePartialCharge";
			column_names_[45]="TotalPositivePartialCharge";
			column_names_[46]="Density";
			column_names_[47]="HydrophobicVdWSurface";
			column_names_[48]="NegativePolarVdWSurface";
			column_names_[49]="PolarVdWSurface";
			column_names_[50]="PositivePolarVdWSurface";
			column_names_[51]="PositiveVdWSurface";
			column_names_[52]="RelHydrophobicVdWSurface";
			column_names_[53]="RelNegativePolarVdWSurface";
			column_names_[54]="RelNegativeVdWSurface";	
			column_names_[55]="RelPolarVdWSurface";	
			column_names_[56]="RelPositivePolarVdWSurface";
			column_names_[57]="RelPositiveVdWSurface";
			column_names_[58]="VdWSurface";
			column_names_[59]="VdWVolume";
		}


		unsigned int QSARData::getNoSubstances() const
		{
			if (descriptor_matrix_.empty())
			{
				return 0;
			}
			return descriptor_matrix_[0].size();
		}

		unsigned int QSARData::getNoDescriptors() const
		{
			return descriptor_matrix_.size();
		}

		unsigned int QSARData::getNoResponseVariables() const
		{
			return Y_.size();
		}


		void QSARData::displayMatrix()
		{
			for (unsigned int i = 0; i < 22 && i < descriptor_matrix_[0].size(); i++) // i = no of substance
			{
				cout << substance_names_[i]<<"  |  "; 
				for (unsigned int j = 0; j < 63 && j < descriptor_matrix_.size(); j++) //j = no of descriptor
				{
					cout << descriptor_matrix_[j][i] <<"  ";
				}
				cout << endl;
			}
		}


		void QSARData::centerData(bool center_Y)
		{
			for (unsigned int i = 0; i < descriptor_matrix_.size(); i++)
			{
				Column c(2);
				Statistics::centering(descriptor_matrix_[i], c[0], c[1]);
				descriptor_transformations_.push_back(c); 
			}
			if (center_Y)
			{
					for (unsigned int i = 0; i < Y_.size(); i++)
				{
					Column c(2);
					Statistics::centering(Y_[i], c[0], c[1]);
					y_transformations_.push_back(c); 
				}
			}
		}


		void QSARData::scaleAllDescriptors()
		{
			Statistics stat;
			stat.scaling(descriptor_matrix_);
		}


		void QSARData::calculateTopologicalDescriptors(Molecule& mol, MolecularSimilarity& molsim, const map<String,int>& descriptor_map)
		{
			vector<Size> fingerprint;
			molsim.generateFingerprint(mol,fingerprint);
			const vector<String>& group_names = molsim.getFunctionalGroupNames();

			for(Size i=0; i<fingerprint.size(); i++)
			{
				int index = descriptor_map.find(group_names[i])->second;
				descriptor_matrix_[index].push_back(fingerprint[i]);
			}
		}


		void QSARData::calculateBALLDescriptors(Molecule& m)
		{
		/*	unsigned int index = m.countNamedProperties();
			
			SimpleBase b0;
			b0.computeAllDescriptors(m);
			
			ConnectivityBase b1;
			b1.computeAllDescriptors(m);
			
			PartialChargeBase b2;
			b2.computeAllDescriptors(m);
			
			SurfaceBase b3;
			b3.computeAllDescriptors(m);
			
			unsigned int i = 0;
			for (; index < m.countNamedProperties(); index++)
			{
				descriptor_matrix_[i].push_back(((String)m.getNamedProperty(i).getString()).toDouble());
				i++;
			}
			
		*/	
			//simple descriptors:
			
			AtomicPolarizabilities simple0; simple0.setDataFolder(data_folder_.c_str());
 			descriptor_matrix_[0].push_back(simple0.compute(m));

			AtomInformationContent simple1; simple1.setDataFolder(data_folder_.c_str()); 
			descriptor_matrix_[1].push_back(simple1.compute(m));
		 
			BondPolarizabilities simple2; simple2.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[2].push_back(simple2.compute(m));

			FormalCharge simple3; simple3.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[3].push_back(simple3.compute(m));

			MeanAtomInformationContent simple4; simple4.setDataFolder(data_folder_.c_str()); 
			descriptor_matrix_[4].push_back(simple4.compute(m));

			MolecularWeight simple5; simple5.setDataFolder(data_folder_.c_str());
 			descriptor_matrix_[5].push_back(simple5.compute(m));

			NumberOfAromaticAtoms simple6; simple6.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[6].push_back(simple6.compute(m));
			
			NumberOfAromaticBonds simple7; simple7.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[7].push_back(simple7.compute(m));

			NumberOfAtoms simple8; simple8.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[8].push_back(simple8.compute(m));

			NumberOfAtoms simple9; simple9.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[9].push_back(simple9.compute(m));

			NumberOfAtoms simple10; simple10.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[10].push_back(simple10.compute(m));

			NumberOfBonds simple11; simple11.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[11].push_back(simple11.compute(m));
			
			NumberOfBoron simple12; simple12.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[12].push_back(simple12.compute(m));

			NumberOfBromine simple13; simple13.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[13].push_back(simple13.compute(m));
			
			NumberOfCarbon simple14; simple14.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[14].push_back(simple14.compute(m));
			
			NumberOfChlorine simple15; simple15.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[15].push_back(simple15.compute(m));
			
			NumberOfDoubleBonds simple16; simple16.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[16].push_back(simple16.compute(m));
			
			NumberOfFlourine simple17; simple17.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[17].push_back(simple17.compute(m));
			
			NumberOfHeavyAtoms simple18; simple18.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[18].push_back(simple18.compute(m));
			
			NumberOfHeavyBonds simple19; simple19.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[19].push_back(simple19.compute(m));
			
			NumberOfHydrogen simple20; simple20.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[20].push_back(simple20.compute(m));
			
			NumberOfHydrogenBondAcceptors simple21; simple21.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[21].push_back(simple21.compute(m));
			
			NumberOfHydrogenBondDonors simple22; simple22.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[22].push_back(simple22.compute(m));

			NumberOfHydrophobicAtoms simple23; simple23.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[23].push_back(simple23.compute(m));
			
			NumberOfIodine simple24; simple24.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[24].push_back(simple24.compute(m));
			
			NumberOfNitrogen simple25; simple25.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[25].push_back(simple25.compute(m));

			NumberOfOxygen simple26; simple26.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[26].push_back(simple26.compute(m));

			NumberOfPhosphorus simple27; simple27.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[27].push_back(simple27.compute(m));
			
			NumberOfRotatableBonds simple28; simple28.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[28].push_back(simple28.compute(m));
			
			NumberOfSingleBonds simple29; simple29.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[29].push_back(simple29.compute(m));
			
			NumberOfSulfur simple30; simple30.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[30].push_back(simple30.compute(m));
				
			NumberOfTripleBonds simple31; simple31.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[31].push_back(simple31.compute(m));
			
			PrincipalMomentOfInertiaX simple32; simple32.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[32].push_back(simple32.compute(m));
			
			PrincipalMomentOfInertiaY simple33; simple33.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[33].push_back(simple33.compute(m));
			
			PrincipalMomentOfInertiaZ simple34; simple34.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[34].push_back(simple34.compute(m));
			
			RelNumberOfRotatableBonds simple35; simple35.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[35].push_back(simple35.compute(m));

			RelNumberOfRotatableSingleBonds simple36; simple36.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[36].push_back(simple36.compute(m));
			
			SizeOfSSSR simple37; simple37.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[37].push_back(simple37.compute(m));
			
			VertexAdjacency simple38; simple38.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[38].push_back(simple38.compute(m));

			VertexAdjacencyEquality simple39; simple39.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[39].push_back(simple39.compute(m));


			// connectivity descriptors:

			BalabanIndexJ connectivity0;
			descriptor_matrix_[40].push_back(connectivity0.compute(m));

			ZagrebIndex connectivity1;
			descriptor_matrix_[41].push_back(connectivity1.compute(m));
			

			// partial charge descriptors:

			RelNegativePartialCharge partial_charge0; 
			partial_charge0.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[42].push_back(partial_charge0.compute(m));
			
			RelPositivePartialCharge partial_charge1;
			partial_charge1.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[43].push_back(partial_charge1.compute(m));

			TotalNegativePartialCharge partial_charge2;
			partial_charge2.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[44].push_back(partial_charge2.compute(m));

			TotalPositivePartialCharge partial_charge3;
			partial_charge3.setDataFolder(data_folder_.c_str());
			descriptor_matrix_[45].push_back(partial_charge3.compute(m));


			// surface descriptors:
			
			Density surface0;
			descriptor_matrix_[46].push_back(surface0.compute(m));

			HydrophobicVdWSurface surface1;
			descriptor_matrix_[47].push_back(surface1.compute(m));

			NegativePolarVdWSurface surface2;
			descriptor_matrix_[48].push_back(surface2.compute(m));

			PolarVdWSurface surface3;
			descriptor_matrix_[49].push_back(surface3.compute(m));

			PositivePolarVdWSurface surface4;
			descriptor_matrix_[50].push_back(surface4.compute(m));

			PositiveVdWSurface surface5;
			descriptor_matrix_[51].push_back(surface5.compute(m));

			RelHydrophobicVdWSurface surface6;
			descriptor_matrix_[52].push_back(surface6.compute(m));

			RelNegativePolarVdWSurface surface7;
			descriptor_matrix_[53].push_back(surface7.compute(m));

			RelNegativeVdWSurface surface8;
			descriptor_matrix_[54].push_back(surface8.compute(m));

			RelPolarVdWSurface surface9;
			descriptor_matrix_[55].push_back(surface9.compute(m));

			RelPositivePolarVdWSurface surface10;
			descriptor_matrix_[56].push_back(surface10.compute(m));

			RelPositiveVdWSurface surface11;
			descriptor_matrix_[57].push_back(surface11.compute(m));

			VdWSurface surface12;
			descriptor_matrix_[58].push_back(surface12.compute(m));

			VdWVolume surface13;
			descriptor_matrix_[59].push_back(surface13.compute(m));
			
		}


		void QSARData::readCSVFile(const char* file, int no_y, bool xlabels, bool ylabels, const char* sep, bool appendDescriptors, bool translate_class_labels)
		{
			ifstream input0(file); 
			String s;
			getline(input0, s);
			int prop = s.countFields(sep);
			
			int nec_min_size = 1;
			if (no_y > 0) nec_min_size = no_y; 
			if (ylabels) nec_min_size++; 
			if (prop < nec_min_size)
			{
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, "CSV-file reading error", "Too few columns found in file. Most likely, the wrong seperator symbol was specified!!"); 
			}
			
			int old_size = descriptor_matrix_.size(); 
			int old_no_y = Y_.size();
				
			if (!appendDescriptors)
			{
				descriptor_matrix_.clear();
				Y_.clear();
				descriptor_matrix_.resize(prop-no_y-ylabels);
				Y_.resize(no_y);
				column_names_.clear();
				substance_names_.clear();
				descriptor_transformations_.clear(); 
				y_transformations_.clear(); 
			}
			else
			{
				descriptor_matrix_.resize(descriptor_matrix_.size()+prop-no_y-ylabels);
				Y_.resize(Y_.size()+no_y);
			}
				
			ifstream input(file); 
			if (!input)
			{
				throw BALL::Exception::FileNotFound(__FILE__, __LINE__, file);
			}
			
			int line = 1;
			std::multiset<int> newInvalidDescriptors; 
			std::multiset<int> tmp;
					
			for (int i = 0; !input.eof(); i++)
			{
				String s;
				getline(input, s);
				if (s == "")
				{
					break;
				}
				istringstream line_stream; // stream that contains the current line
				line_stream.str(s);

				int compound_id = i;
				if (xlabels) compound_id--; 
				if (invalidSubstances_.find(compound_id) != invalidSubstances_.end())
				{
					continue;
				}
				
				if (appendDescriptors && line-xlabels > (int)descriptor_matrix_[0].size())
				{
					throw Exception::PropertyError(__FILE__, __LINE__, file, line, "There are more lines containing additional descriptors than substances already read!");
				}
			
				if (line == 1 && xlabels)   // read first line consisting of descriptor names
				{
					for (int i = 0; i < prop-no_y; i++) 
					{
						String value; getline(line_stream, value, sep[0]);
						
						// if labels for compounds are given (located in first column)
						// then do not read name of first column
						if (i > 0 || !ylabels) column_names_.push_back(value); 
					}
					line++;
					continue;
				}
				
				else if (line == 1 && !xlabels)
				{
					for (int i = 0; i < prop-no_y; i++) 
					{
						column_names_.push_back("unknown");
					}
				}
				
						
				for (int i = 0; i < prop; i++) //read current line consisting of descriptor and activity values
				{	
					if (i == 0 && ylabels) // read first cell containing compound name
					{
						String value; getline(line_stream, value, sep[0]);
						if (appendDescriptors)
						{
							continue;
						}
						substance_names_.push_back(value);
						continue;
					}
					else if (i == 0 && !ylabels && !appendDescriptors)
					{
						substance_names_.push_back("unknown");
					}
						
					
					if (i < prop-no_y)
					{	
						int no = i; // number of current descriptor
						if (ylabels)
						{
							no--;
						}
						if (appendDescriptors)
						{
							no += old_size;
						}
										
						try
						{
							String s; getline(line_stream, s, sep[0]);
							descriptor_matrix_[no].push_back(s.toDouble());
						}	
						catch(BALL::Exception::InvalidFormat g) 
						{
							descriptor_matrix_[no].push_back(0);
							if (tmp.find(i) == tmp.end())
							{
								tmp.insert(i);
							}
							if (newInvalidDescriptors.find(no) == newInvalidDescriptors.end())
							{
								newInvalidDescriptors.insert(no);
							}
						}
					}
					else
					{
						String value; getline(line_stream, value, sep[0]);
						if (!translate_class_labels)
						{
							try
							{
								Y_[old_no_y+i-(prop-no_y)].push_back(value.toDouble());
							}
							catch(BALL::Exception::InvalidFormat g) 
							{
								throw Exception::PropertyError(__FILE__, __LINE__, file, line, "Some properties for activities are not numerical values!"); 
							}
						}
						else
						{
							map<String, int>::iterator it = class_names_.find(value);
							if (it != class_names_.end())
							{
								Y_[old_no_y+i-(prop-no_y)].push_back(it->second);
							}
							else
							{
								// assign ID for new class label
								int id = class_names_.size();
								Y_[old_no_y+i-(prop-no_y)].push_back(id);
								class_names_.insert(make_pair(value, id));
							}
						}
					}
				} 
				line++;
			}
			
			if (appendDescriptors && line-xlabels < (int)descriptor_matrix_[0].size())
			{
				throw Exception::PropertyError(__FILE__, __LINE__, file, line, "There are less lines containing additional descriptors than substances already read!");
			}
			
			invalidDescriptors_ = tmp;
			removeInvalidDescriptors(newInvalidDescriptors);	
		}


		void QSARData::manipulateY(String v)
		{
			vector<String> vs;
			vs.push_back(v);
			manipulateY(vs);
		}


		void QSARData::manipulateY(vector<String> v)
		{
			vector<double> vn(descriptor_matrix_[0].size());
			Y_.resize(v.size(), vn);
			
			// set simulated activities for each substance
			for (unsigned int n = 0; n < descriptor_matrix_[0].size(); n++)
			{
				String var="";
				// replace all x-values for the current substance
				for (unsigned int m = 0; m < descriptor_matrix_.size(); m++)
				{
					var = var+"x"+String(m)+"="+String(descriptor_matrix_[m][n])+";";
				}
				
				//calculate values for Y_
				for (unsigned int i = 0; i < v.size(); i++)
				{
					ParsedFunction<float> f = var+v[i];
					double d = f(0);
					Y_[i][n] = d;
					//cout<<d<<endl;
				}
			}
		}


		void QSARData::discretizeY(vector<double> thresholds)
		{
			// make sure that the thresholds are sorted ascendingly
			sort(thresholds.begin(), thresholds.end());
			
			// if response variable(s) were not normalized, use given thresholds directly
			if (y_transformations_.empty())
			{	
				for (unsigned int i = 0; i < Y_.size(); i++)
				{
					for (unsigned int j = 0; j < Y_[0].size(); j++)
					{
						if (Y_[i][j] < thresholds[0])
						{
							Y_[i][j] = 0; // lowest class-label == 0
						}
						else if (Y_[i][j] >= thresholds[thresholds.size()-1])
						{
							Y_[i][j] = thresholds.size(); // highest class-label
						}
						else
						{
							for (unsigned int k = 0; k < thresholds.size()-1; k++)
							{
								if (Y_[i][j] >= thresholds[k] && Y_[i][j] < thresholds[k+1])
								{
									Y_[i][j] = k+1;
								}
							}
						}
					}
				}
			}
			
			// if response variable(s) were normalized, normalize the given thresholds first
			else
			{
				unsigned int no_thresholds = thresholds.size();
				
				vector<vector<double> > norm_thresholds(Y_.size());
				for (unsigned int act = 0; act < Y_.size(); act++)
				{
					norm_thresholds[act].resize(no_thresholds);
					for (unsigned int i = 0; i < no_thresholds; i++)
					{
						norm_thresholds[act][i] = (thresholds[i]-y_transformations_[act][0])/y_transformations_[act][1]; 
					}
				}
				
				for (unsigned int act = 0; act < Y_.size(); act++)
				{
					for (unsigned int j = 0; j < Y_[0].size(); j++)
					{
						if (Y_[act][j] < norm_thresholds[act][0])
						{
							Y_[act][j] = 0; // lowest class-label == 0
						}
						else if (Y_[act][j] >= norm_thresholds[act][no_thresholds-1])
						{
							Y_[act][j] = no_thresholds; // highest class-label
						}
						else
						{
							for (unsigned int k = 0; k < no_thresholds-1; k++)
							{
								if (Y_[act][j] >= norm_thresholds[act][k] && Y_[act][j] < norm_thresholds[act][k+1])
								{
									Y_[act][j] = k+1;
								}
							}
						}
					}
				}
				
				// now we have discretized response variables, so do not do any response normalization any more
				y_transformations_.clear(); 
				y_transformations_.resize(0); 
			}
		}


		void QSARData::transformX(vector < String > v)
		{
			if (v.size() != descriptor_matrix_.size())
			{
				cout << "ERROR: wrong number of equations"<<endl; return;
			}
			
			// set all descriptor values
			for (unsigned int m = 0; m < descriptor_matrix_.size(); m++)
			{
				for (unsigned int n = 0; n < descriptor_matrix_[0].size(); n++)
				{
					String var="";
					var = var+"x="+String(descriptor_matrix_[m][n])+";";
					ParsedFunction<float> f = var+v[m];
					double d = f(0);
					descriptor_matrix_[m][n] = d;
				}
			}	

		}


		vector<QSARData*> QSARData::partitionInputData(int p)
		{
			vector<QSARData*> v(p);
			for (int i = 0; i < p; i++)
			{
				v[i] = new QSARData;
				v[i]->column_names_ = column_names_;
				v[i]->class_names_ = class_names_;
				v[i]->descriptor_transformations_.clear(); 
				v[i]->y_transformations_.clear(); 
				v[i]->descriptor_matrix_.resize(descriptor_matrix_.size());
				v[i]->Y_.resize(Y_.size());
			}
			
			for (unsigned int i = 0; i < descriptor_matrix_[0].size(); i++)
			{
				//insert substance i into the (i%p)'th QSARData object
				v[i%p]->insertSubstance(this, i, 1);  // features are backtransformated to original space
			}	
			
			return v;	
		}

		vector<QSARData*> QSARData::evenSplit(int no_test_splits, int current_test_split_id, int response_id) const
		{
			if (current_test_split_id < 0 || current_test_split_id >= no_test_splits)
			{
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, "QSARData::evenSplit() error", "Make sure that 0<=current_test_split_id<no_test_splits !");
			}

			vector<QSARData*> v(2);
			v[0] = new QSARData;  // training set
			v[1] = new QSARData;  // external validation set

			v[0]->descriptor_transformations_.clear(); 
			v[0]->y_transformations_.clear(); 
			v[0]->column_names_ = column_names_;
			v[0]->descriptor_matrix_.resize(descriptor_matrix_.size());
			v[0]->Y_.resize(Y_.size());
			v[0]->class_names_ = class_names_;
			v[1]->descriptor_transformations_.clear(); 
			v[1]->y_transformations_.clear(); 
			v[1]->column_names_ = column_names_;
			v[1]->descriptor_matrix_.resize(descriptor_matrix_.size());
			v[1]->Y_.resize(Y_.size());
			v[1]->class_names_ = class_names_;

			std::multiset<unsigned int> val;
			//unsigned int no_val = static_cast<unsigned int>(descriptor_matrix_[0].size()*fraction);

			/// Sort reponse values
			multimap<double, Size> response_map;
			for (Size i = 0; i < Y_[0].size(); i++)
			{
				response_map.insert(make_pair(Y_[response_id][i], i));
			}

			/// Create the test partition
			Size i = 0;
			for (multimap < double, Size > ::iterator it = response_map.begin(); it != response_map.end(); it++, i++)
			{
				if (i%no_test_splits == current_test_split_id)
				{
					v[1]->insertSubstance(this, it->second, 1); // features are backtransformated to original space
					val.insert(it->second);
				}
			}

			/// All compounds not drawn before make up the training partition
			for (unsigned int i = 0; i < descriptor_matrix_[0].size(); i++)
			{
				if (val.find(i) == val.end())
				{
					v[0]->insertSubstance(this, i, 1);  // features are backtransformated to original space
				}
			}

			return v;
		}


		vector<QSARData*> QSARData::generateExternalSet(double fraction) const
		{
			vector<QSARData*> v(2);	
			v[0] = new QSARData;  // training set 
			v[1] = new QSARData;  // external validation set
			
			v[0]->descriptor_transformations_.clear(); 
			v[0]->y_transformations_.clear(); 
			v[0]->column_names_ = column_names_;
			v[0]->descriptor_matrix_.resize(descriptor_matrix_.size());
			v[0]->Y_.resize(Y_.size());
			v[0]->class_names_ = class_names_;
			v[1]->descriptor_transformations_.clear(); 
			v[1]->y_transformations_.clear(); 
			v[1]->column_names_ = column_names_;
			v[1]->descriptor_matrix_.resize(descriptor_matrix_.size());
			v[1]->Y_.resize(Y_.size());
			v[1]->class_names_ = class_names_;
			
			std::multiset<unsigned int> val;
			set<unsigned int> map_val;
			unsigned int no_val = static_cast<unsigned int>(descriptor_matrix_[0].size()*fraction);
			
			PreciseTime pt;
			boost::mt19937 rng(pt.now().getMicroSeconds());
			
			
			/// randomly draw without replacement the desired number of external validation compounds
			for (unsigned int i = 0; i < no_val; i++)
			{
				int pos = rng() % (descriptor_matrix_[0].size()-1);
				if (map_val.find(pos) != map_val.end())
				{
					i--; // no increase, since no new validation compound was selected
					continue;
				}
				v[1]->insertSubstance(this, pos, 1); // features are backtransformated to original space
				val.insert(pos);
				map_val.insert(pos);
			}
			
			/// all compounds not drawn before make up the training set
			std::multiset<unsigned int>::iterator it = val.begin();
			for (unsigned int i = 0; i < descriptor_matrix_[0].size(); i++)
			{
				if (*it != i)
				{
					v[0]->insertSubstance(this, i, 1);  // features are backtransformated to original space
				}
				else
				{
					it++;
				}
			}
			
			return v;
		}


		void QSARData::insertSubstance(const QSARData* source, int s, bool backtransformation)
		{
			substance_names_.push_back(source->substance_names_[s]);
			
			bool backtransf_descr = 0;
			bool backtransf_y = 0;
			if (backtransformation)
			{
				if (source->descriptor_transformations_.size() > 0) backtransf_descr = 1; 
				if (source->y_transformations_.size() > 0) backtransf_y = 1; 
			}
		 	
			for (unsigned int i = 0; i < source->descriptor_matrix_.size(); i++)
			{ 
				double value = source->descriptor_matrix_[i][s];
				if (backtransf_descr)
				{
					// value = (value*stddev)+mean
					value = (value*source->descriptor_transformations_[i][1])+source->descriptor_transformations_[i][0]; 
				}
				descriptor_matrix_[i].push_back(value);
			}
			
			for (unsigned int j = 0; j < source->Y_.size(); j++)
			{
				double value = source->Y_[j][s];
				if (backtransf_y)
				{
					// value = (value*stddev)+mean
					value = (value*source->y_transformations_[j][1])+source->y_transformations_[j][0]; 
				}
				Y_[j].push_back(value);
			}
		}


		void QSARData::printMatrix(const VMatrix& mat, std::ostream& out) const
		{
			if (mat.empty())
			{
				return;
			}
			
			for (unsigned int i = 0; i < mat[0].size(); i++)
			{
				for (unsigned int j = 0; j < mat.size(); j++)
				{
					out << mat[j][i] <<"\t";
				}
				out<<std::endl;
			}
			out<<std::endl;
		}

		void QSARData::saveToFile(string filename) const
		{
			std::ofstream out(filename.c_str());
			
			bool center_data = 0;
			bool center_y = 0;
			if (!descriptor_transformations_.empty())
			{
				center_data = 1;
				if (!y_transformations_.empty())
				{
					center_y = 1;
				}
			}
			bool translated_class_labels = !class_names_.empty();
			
			out << descriptor_matrix_[0].size()<<"\t"<<descriptor_matrix_.size()<<"\t"<<Y_.size()<<"\t"<<center_data<<"\t"<<center_y<<"\t"<<translated_class_labels<<endl<<endl;
			
			printMatrix(descriptor_matrix_, out);
			printMatrix(Y_, out);
			
			for (unsigned int i = 0; i < column_names_.size(); i++)
			{ 
				out<<column_names_[i]<<"\t";
			}
			out<<endl<<endl;
			for (unsigned int i = 0; i < substance_names_.size(); i++)
			{
				out<<substance_names_[i]<<"\t";
			}
			out<<endl<<endl;
			
			printMatrix(descriptor_transformations_, out); 
			printMatrix(y_transformations_, out); 
			
			if (translated_class_labels) 
			{
				vector<String> ordered_names(class_names_.size(), "");
				for (map < String, int > ::const_iterator it = class_names_.begin(); it != class_names_.end(); it++)
				{
					ordered_names[it->second] = it->first;
				}
				for (unsigned int i = 0; i < ordered_names.size(); i++)
				{
					out<<ordered_names[i]<<"\t";
				}
				out<<endl<<endl;	
			}
		}

		void QSARData::readMatrix(VMatrix& mat, ifstream& in, char seperator, unsigned int lines, unsigned int col)
		{
			Column c(lines, 0);
			mat.resize(col, c);
			String line;
			
			for (unsigned int i = 0; i < lines; i++)
			{
				//getline(in, line);
				for (unsigned int j = 0; j < col; j++)
				{
					String s;
					getline(in, s, seperator); 
					mat[j][i] = s.toDouble(); // = line.getField(j, "\t").toDouble();
				}
			}
			getline(in, line); // read until the end of the last matrix-line
		}

		void QSARData::readFromFile(string filename)
		{
			ifstream in(filename.c_str()); 
			if (!in)
			{
				throw BALL::Exception::FileNotFound(__FILE__, __LINE__, filename);
			}
			
			String line;
			getline(in, line);
			int no_fields = line.countFields("\t");
			unsigned int no_subst = (unsigned int) line.getField(0, "\t").toInt();
			unsigned int no_desc = (unsigned int) line.getField(1, "\t").toInt();
			unsigned int no_y = (unsigned int) line.getField(2, "\t").toInt();
			bool center_data = (bool) line.getField(3, "\t").toInt();
			bool center_y = (bool) line.getField(4, "\t").toInt();
			bool translated_class_labels = 0;
			if (no_fields > 5)
			{
				translated_class_labels = (bool) line.getField(5, "\t").toBool();
			}
			column_names_.resize(no_desc);
			substance_names_.resize(no_subst);
			getline(in, line); // skip empty line
			
			descriptor_matrix_.clear();
			Y_.clear();
			
			readMatrix(descriptor_matrix_, in, '\t', no_subst, no_desc); /// read descriptor matrix
			getline(in, line); // skip empty line

			if (no_y > 0)
			{
				readMatrix(Y_, in, '\t', no_subst, no_y);  /// read response values
				getline(in, line); // skip empty line
			}
			
			getline(in, line);
			for (unsigned int i = 0; i < no_desc; i++) /// read names of descriptors
			{ 
				column_names_[i] = line.getField(i, "\t");
			}
			getline(in, line); // skip empty line
			
			getline(in, line);
			for (unsigned int i = 0; i < no_subst; i++) /// read names of substances
			{
				substance_names_[i] = line.getField(i, "\t");
			}
			getline(in, line); // skip empty line
			
			if (center_data)     /// read information about centering of data
			{
				readMatrix(descriptor_transformations_, in, '\t', 2, no_desc); 
				if (center_y)
				{	
					getline(in, line); // skip empty line
					readMatrix(y_transformations_, in, '\t', 2, no_y); 
				}
			}
			else		/// delete all centering information if no centering was done on current input data
			{
				descriptor_transformations_.clear(); 
				y_transformations_.clear(); 
			}
			
			class_names_.clear();
			if (translated_class_labels)
			{
				getline(in, line); // skip empty line
				unsigned int no_labels = line.countFields("\t");
				for (unsigned int i = 0; i < no_labels; i++)
				{
					class_names_.insert(make_pair(line.getField(i), i));
				}
			}
		}


		vector<double>* QSARData::getSubstance(int s) const
		{
			vector<double>* v = new vector<double>(descriptor_matrix_.size(), 0);
			for (unsigned int i = 0; i < descriptor_matrix_.size(); i++)
			{
				(*v)[i] = descriptor_matrix_[i][s];
			}
			
			for (unsigned int i = 0; i < descriptor_transformations_.size(); i++)
			{
				double stddev = descriptor_transformations_[i][1]; 
				(*v)[i] = (*v)[i]*stddev+descriptor_transformations_[i][0]; 
			}
			
			return v;
		}


		vector<double>* QSARData::getActivity(int s) const
		{
			vector<double>* v = new vector<double>(Y_.size(), 0);
			for (unsigned int i = 0; i < Y_.size(); i++)
			{
				(*v)[i] = Y_[i][s];
			}
			
			for (unsigned int i = 0; i < y_transformations_.size(); i++)
			{
				double stddev = y_transformations_[i][1]; 
				(*v)[i] = (*v)[i]*stddev+y_transformations_[i][0]; 
			}	
			return v;
		}


		const vector<string>* QSARData::getSubstanceNames() const
		{
			return &substance_names_;
		}


		bool QSARData::checkforDiscreteY() const
		{
			for (unsigned int i = 0; i < Y_.size(); i++)
			{
				for (unsigned int j = 0; j < Y_[0].size(); j++)
				{
					int label = static_cast<int>(Y_[i][j]);
					if (label != Y_[i][j])
					{
						return false;
					}
				}
			}
			return true;
		}



		bool QSARData::checkforDiscreteY(const char* file, std::multiset < int > & activity_IDs) const
		{
			SDFile sd(file);
			sd.disableAtoms();
			
			while (sd)
			{
				Molecule m;
				sd >> m;
				std::multiset<int>::iterator a_it = activity_IDs.begin();
				for (; a_it != activity_IDs.end(); ++a_it)
				{
					double y = String(m.getNamedProperty(*a_it).getString()).toDouble();
				
					if (y != (int)y)
					{
						return false;
					}
				}
			}
			return true;
		}


		void QSARData::removeHighlyCorrelatedCompounds(double& compound_cor_threshold, double& feature_cor_threshold)
		{
			if (descriptor_matrix_.empty())
			{
				throw Exception::InconsistentUsage(__FILE__, __LINE__, "Data must be read before highly correlated compounds can be removed!"); 
			}
			
			/// use only those features that do not have identical values for all compounds
			std::multiset<int> features_to_use;
			for (unsigned int i = 0; i < descriptor_matrix_.size(); i++) // descriptors
			{
				bool identical_values = 1;
				for (unsigned int j = 1; j < descriptor_matrix_[0].size(); j++) // compounds
				{
					if (descriptor_matrix_[i][j] != descriptor_matrix_[i][0])
					{
						identical_values = 0;
						break;
					}
				}
				if (!identical_values)
				{
					features_to_use.insert(i);
				}
			}
			/// check remaining features for correlaton to each other !
			std::multiset<int>::iterator f_it = features_to_use.begin();
			for (; f_it != features_to_use.end(); ++f_it)
			{
				list<pair<unsigned int, String> > similar_descriptor_IDs;
				getSimilarDescriptors(*f_it, feature_cor_threshold, similar_descriptor_IDs);
				for (list < pair < unsigned int, String > > ::iterator it = similar_descriptor_IDs.begin(); it != similar_descriptor_IDs.end(); it++)
				{
					features_to_use.erase(it->first);
				}
			}	
			
			vector<double> stddev(descriptor_matrix_[0].size(), 0);
			vector<double> mean(descriptor_matrix_[0].size(), 0);

			for (unsigned int i = 0; i < mean.size(); i++)
			{
				mean[i] = Statistics::getRowMean(descriptor_matrix_, i, &features_to_use);
			}		
			for (unsigned int i = 0; i < stddev.size(); i++)
			{
				stddev[i] = Statistics::getRowStddev(descriptor_matrix_, i, mean[i], &features_to_use);
			}
				
			double abs_cor_threshold = abs(compound_cor_threshold);
			bool discrete_response = checkforDiscreteY(); 
			std::multiset<int> to_be_deleted;
			
			/// find highly correlated compounds
			for (unsigned int i = 0; i < descriptor_matrix_[0].size(); i++)
			{	
				if (to_be_deleted.find(i) != to_be_deleted.end()) continue; 
				int no = 1;
				
				for (unsigned int j = 0; j < descriptor_matrix_[0].size(); j++)
				{
					if (i == j) continue; 
					if (to_be_deleted.find(j) != to_be_deleted.end()) continue; 
					
					double covar = Statistics::getRowCovariance(descriptor_matrix_, i, j, mean[i], mean[j], &features_to_use);
					
					double abs_cor = abs(covar/(stddev[i]*stddev[j]));
					
					if (abs_cor > abs_cor_threshold)
					{
						if (!discrete_response)
						{
							// add up response values in order to calculate the mean later
							for (unsigned int c = 0; c < Y_.size(); c++)
							{
								Y_[c][i] += Y_[c][j];
							}
							cout<<i<<" "<<j<<" : "<<abs_cor<<endl;
							to_be_deleted.insert(j);
						}
						else
						{
							bool identical_labels = 1;
							// delete compound only if all class-labels are identical!
							for (unsigned int c = 0; c < Y_.size(); c++)
							{
								if ((int)Y_[c][i] != (int)Y_[c][j]) 
								{
									identical_labels = false;
									break;
								}
							}
							if (identical_labels)
							{
								cout<<i<<" "<<j<<" : "<<abs_cor<<endl;
								to_be_deleted.insert(j);
							}					
						}
					}
				}
				
				if (discrete_response && no > 1)
				{
					for (unsigned int c = 0; c < Y_.size(); c++)
					{
						Y_[c][i] /= no;
					}
				}		
			}
			
			removeInvalidSubstances(to_be_deleted);
			cout<<"Removed "<<to_be_deleted.size()<<" highly correlated compounds."<<endl;
			cout<<"Used "<<features_to_use.size()<<" out of "<<descriptor_matrix_.size()<<" descriptors."<<endl;
		}


		void QSARData::getSimilarDescriptors(int descriptor_ID, double correlation, list<pair<unsigned int, String> >& similar_descriptor_IDs) const
		{
			if (descriptor_ID < 0 || descriptor_ID >= (int)descriptor_matrix_.size())
			{
				String mess = "Specified descriptor ID '"+String(descriptor_ID)+"' is out ouf range"; 
				if (descriptor_ID >= (int)descriptor_matrix_.size())
				{
					mess += "; max. index="+String(descriptor_matrix_.size()-1);
				}
				
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, "getSimilarDescriptors() error", mess);
			}
			
			similar_descriptor_IDs.clear();
			
			vector<double> stddev(getNoDescriptors(), 1);
			vector<double> mean(getNoDescriptors(), 0);
			
			// if data has not been centered, calculate mean and stddev of each feature
			if (descriptor_transformations_.empty())
			{
				for (unsigned int i = 0; i < mean.size(); i++)
				{
					mean[i] = Statistics::getMean(descriptor_matrix_[i]);
				}		
				for (unsigned int i = 0; i < stddev.size(); i++)
				{
					stddev[i] = Statistics::getStddev(descriptor_matrix_[i], mean[i]);
				}
			}
			
			unsigned int size = (unsigned int)descriptor_matrix_.size();
			for (unsigned int i = 0; i < size; i++)
			{
				if ((unsigned int)descriptor_ID == i) continue; 
					
				double covar = Statistics::getCovariance(descriptor_matrix_[descriptor_ID], descriptor_matrix_[i], mean[descriptor_ID], mean[i]);
					
				double abs_cor = abs(covar/(stddev[descriptor_ID]*stddev[i]));
					
				if (abs_cor > correlation)
				{
					similar_descriptor_IDs.push_back(make_pair(i, column_names_[i]));
				}
			}
		}
	}
}

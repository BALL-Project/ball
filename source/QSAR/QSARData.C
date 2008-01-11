// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 

#include <BALL/QSAR/QSARData.h>

using namespace BALL::QSAR;


void QSARData::readFiles(char* directory)
{
	string call="cd ";
	call=call+directory+"; ls *.mol *.hin *.pdb > files.txt";
	system(call.c_str());
	string files=directory;
	files=files+"/files.txt";
	ifstream filenames(files.c_str());
	
	descriptor_matrix_.clear();
	column_names_.clear();
	substance_names_.clear();
	Y_.clear();
	descriptor_transformations_.clear();
	y_transformations_.clear();
	descriptor_matrix_.resize(60);
	column_names_.resize(60,"");
	Y_.resize(1);
	
	for(int n=0; filenames; n++)
	{
		String file;
		filenames >> file;
		if(file.compare("")==0) { break;}
		String type = file.substr(file.size()-3);
		substance_names_.push_back(file);
		string f=directory;
		f=f+"/"+file;
		Molecule m;
		if(type=="hin") 
		{
			HINFile input(f);
			input >> m;
		}
		else if(type=="sdf") 
		{
			SDFile input(f);
			input >> m;
		}
		else if(type=="mol") 
		{
			MOLFile input(f);
			input >> m;
		}
		else if(type=="pdb") 
		{
			PDBFile input(f);
			input >> m;
		}
		if(n==0)
		{
			SortedList<int> l;
			setDescriptorNames(m, l);
		}
	 	calculateBALLDescriptors(m);
	}
	string f=directory;
	f=f+"/activity.txt";
	ifstream activity(f.c_str());
	if(!activity)
	{
		throw BALL::Exception::FileNotFound(__FILE__,__LINE__,f);
	}
	
	while(activity)
	{	
		string d;
		activity >> d;
		if(d.compare("")==0) { break;}
		Y_[0].push_back(atof(d.c_str()));
	}
}


void QSARData::readSDFile(const char* file)
{
	string f=file;
	string a=f.substr(0,f.size()-4);
	a=a+".txt";
	
	descriptor_matrix_.clear();
	column_names_.clear();
	substance_names_.clear();
	Y_.clear();
	descriptor_transformations_.clear();
	y_transformations_.clear();
	descriptor_matrix_.resize(60);
	column_names_.resize(60,"");
	Y_.resize(1);
	int no_properties=0;
	SDFile input(file);

	ifstream activity(a.c_str());
	

	for(int n=0;input && activity; n++)
	{
		Molecule m;
		try
		{
			input>>m;
		}
		catch(BALL::Exception::ParseError e)
		{
			throw Exception::WrongFileFormat(__FILE__,__LINE__,file);
		}
		
		String d;
		//activity >> d;
		getline(activity,d);
		if(d.compare("")==0) { break;}
		if(n==0)
		{
			no_properties = m.countNamedProperties();
			SortedList<int> l;
			setDescriptorNames(m, l);
			descriptor_matrix_.resize(60+no_properties);
		}
		substance_names_.push_back(m.getName());
		
		if((int)m.countNamedProperties()!=no_properties)
		{
			String e="properties are missing for";
			e = e+m.getName()+"; "+String(m.countNamedProperties())+" properties given, but "+String(no_properties)+" needed!";
			throw Exception::PropertyError(__FILE__,__LINE__, file, n, e.c_str());
		}
		
		for(int i=0;i<no_properties;i++) // add all external descriptors for current molecule
		{
			descriptor_matrix_[60+i].push_back(String(m.getNamedProperty(i).getString()).toDouble());
		}	
		
		calculateBALLDescriptors(m);
		
		//cout<<d.getField(0,"\t")<<endl;
		Y_[0].push_back(atof(d.getField(0,"\t").c_str()));
	}
}


void QSARData::checkActivityIDs(SortedList<int>& act, int no_properties)
{	
	act.front();
	// No response variable in case of data set whose activity is to be predicted
	if(act.size()==0 || act.front()==-1) 
	{									
		return;
		//throw Exception::InvalidActivityID(__FILE__,__LINE__);
	}
	
	while(act.hasNext())
	{
		int a = act.next();
		if(a<0 || a>no_properties)
		{
			throw Exception::InvalidActivityID(__FILE__,__LINE__,a,no_properties);
		}
	}
}


void QSARData::readSDFile(const char* file, SortedList<int>& activity_IDs, bool useExDesc, bool append)
{
	if(!append)
	{
		descriptor_matrix_.clear();
		column_names_.clear();
		substance_names_.clear();
		Y_.clear();
		descriptor_transformations_.clear();
		y_transformations_.clear();
		descriptor_matrix_.resize(60);
		column_names_.resize(60,"");
	}

	string f=file;
	SDFile input(f);
	
	int no_properties=0; // no of descriptors as determined by the first substance in the sd-file
	int no_descriptors=0;
	//int no_substances=(int)input.getSize();
	SortedList<int> newInvalidDescriptors; // invalid descriptors of the current input file only; save no. of columns
	SortedList<int> newInvalidSubstances; 
	SortedList<int> tmp; // invalid descriptors of the current input file; save no. of external descriptor
	
	// read all molecules in the sd-file
	for(int n=0; input.getSize()!=0; n++) 
	{	
		Molecule m;
		
		try
		{
			input>>m;
		}
		catch(BALL::Exception::ParseError e)
		{
			throw Exception::WrongFileFormat(__FILE__,__LINE__,file);
		}
				
		int no=m.countNamedProperties();

		if (n==0) // for the first substance
		{	
			no_descriptors=no-activity_IDs.size();
			no_properties=no;
			checkActivityIDs(activity_IDs,no_properties);
			
			if(append)
			{	
				if(60+useExDesc*no_descriptors!=(int)(descriptor_matrix_.size()+invalidDescriptors_.size()))
				{
					throw Exception::PropertyError(__FILE__,__LINE__, file, n, "substances to append must have the same number of descriptors than the rest of the data!!");
				}	
				if(activity_IDs.size()!=Y_.size())
				{	
					throw Exception::PropertyError(__FILE__,__LINE__, file, n, "substances to append must have the same number of activities than the rest of the data!!");
				}	
			}	
			else
			{				
				setDescriptorNames(m, activity_IDs, useExDesc);  // set names of all descriptors
				// resize descriptor_matrix_ for first substance
				for(int i=0; useExDesc && i<no_descriptors;i++)
				{
					Column c;
					descriptor_matrix_.push_back(c);
				}
				Y_.resize(activity_IDs.size());
			}
		}
		
		substance_names_.push_back(m.getName());

		// if some substance has not the same number of properties as the first substance
		if(useExDesc && no!=no_properties)
		{
			throw Exception::PropertyError(__FILE__,__LINE__, file, n, "properties are missing!");
		}
		
		int act=0; // no of current activity
		int des=60; // no of column for current external descriptors within descriptor_matrix_
		SortedList<int>::Iterator act_it = activity_IDs.begin();
		SortedList<int>::Iterator inv_it = invalidDescriptors_.begin();
		
		for(int i=0; i<no && i<no_properties;i++) // create descriptors and activities for current molecule
		{	 
			if(useExDesc && *act_it!=i) // read external descriptors from molecule
			{
				if(invalidDescriptors_.empty() || *inv_it!=i) // do not add values of descriptors indentified as invalid by the *last* run of getData()
				{	
					try				
					{	
						descriptor_matrix_[des].push_back(String(m.getNamedProperty(i).getString()).toDouble());
					}
					// descriptors with invalid entries will be removed ...
					catch(BALL::Exception::InvalidFormat g)
					{
						descriptor_matrix_[des].push_back(0);
						if(!tmp.contains(i))
						{
							tmp.insert(i);
						}
						if(!newInvalidDescriptors.contains(des))
						{
							newInvalidDescriptors.insert(des);
						}
					}
					des++;
				}
				else if(*inv_it==i)
				{
					inv_it++;
				}
			}
			else if(*act_it==i) // read activities
			{
				try
				{
					Y_[act].push_back(String(m.getNamedProperty(i).getString()).toDouble());
					act++; act_it++;
				}
				catch(BALL::Exception::InvalidFormat g) 
				{
					//String a="property '";
					//a = a + m.getNamedProperty(i).getString() + "' is no numerical value!";
					//throw Exception::PropertyError(__FILE__,__LINE__, file, n, a.c_str());
					Y_[act].push_back(0);
					if(!invalidSubstances_.contains(n))
					{
						newInvalidSubstances.insert(n);
					}
				}
			}
		}
		calculateBALLDescriptors(m); // calculate BALL-descriptors
	}

	removeInvalidDescriptors(newInvalidDescriptors);
	removeInvalidSubstances(newInvalidSubstances);
	invalidDescriptors_=tmp;
	invalidSubstances_=newInvalidSubstances;
}


void QSARData::removeInvalidDescriptors(SortedList<int>& inv)
{
	inv.front();
	for(int i=0;inv.hasNext();i++)
	{
		int pos=inv.next()-i; // i already deleted columns --> descriptor_matrix_ is i columns shorter
		vector<vector<double> >::iterator del = static_cast<vector<vector<double> >::iterator> (&descriptor_matrix_[pos]);
		descriptor_matrix_.erase(del);
		
		if(column_names_.size()==descriptor_matrix_.size()) // do only if names of descriptors have been set
		{
			vector<string>::iterator sdel=static_cast<vector<string>::iterator> (&column_names_[pos]);
			column_names_.erase(sdel);
		}
		cout << "deleted descriptor "<<pos+i<<" due to invalid values!"<<endl;
	}
}


void QSARData::removeInvalidSubstances(SortedList<int>& inv)
{    
	inv.front();
	for(int i=0;inv.hasNext();i++)
	{
		int pos=inv.next()-i; // i already deleted rows --> descriptor_matrix_ is i rows shorter
	
		for(unsigned int j=0; j<descriptor_matrix_.size();j++)
		{
			vector<double>::iterator del = static_cast<vector<double> ::iterator> (&descriptor_matrix_[j][pos]);
			descriptor_matrix_[j].erase(del);
		}
		
		for(unsigned int j=0; j<Y_.size();j++)
		{
			vector<double>::iterator del = static_cast<vector<double> ::iterator> (&Y_[j][pos]);
			Y_[j].erase(del);
		}
		cout << "deleted substance "<<pos+i<<" due to invalid values!"<<endl;
	}
}


void QSARData::setDescriptorNames(const Molecule& m, SortedList<int>& activity_IDs, bool useExDesc)
{
	if(useExDesc)
	{
		int n=m.countNamedProperties();
		int cols=60+n-activity_IDs.size();
		if(cols<60)
		{
			cols=60;
		}
		column_names_.resize(cols,"");  //60 BALL-descriptors + m-1 descriptors read from the sd-file
		
		
		// set the names of the descriptor read from the sd-file
		int p=0;
		for(int i=60; i<cols;)
		{
			if(activity_IDs.contains(p))
			{ 
				p++;
				continue;
			}
			column_names_[i]=m.getNamedProperty(p).getName();
			p++;
			i++;
		}
	}
	else
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
	if(descriptor_matrix_.size()==0)
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
	for (unsigned int i=0; i<22 &&i<descriptor_matrix_[0].size();i++) // i=no of substance
	{
		cout << substance_names_[i]<<"  |  "; 
		for(unsigned int j=0; j<63 && j<descriptor_matrix_.size(); j++) //j= no of descriptor
		{
			cout << descriptor_matrix_[j][i] <<"  ";
		}
		cout << endl;
	}
}


void QSARData::centerData(bool center_Y)
{
	for (unsigned int i=0; i<descriptor_matrix_.size();i++)
	{
		Column c(2);
		Statistics::centering(descriptor_matrix_[i],c[0],c[1]);
		descriptor_transformations_.push_back(c);
	}
	if(center_Y)
	{
			for(unsigned int i=0;i<Y_.size();i++)
		{
			Column c(2);
			Statistics::centering(Y_[i],c[0],c[1]);
			y_transformations_.push_back(c);
		}
	}
}


void QSARData::scaleAllDescriptors()
{
	Statistics stat;
	stat.scaling(descriptor_matrix_);
}


void QSARData::calculateBALLDescriptors(Molecule& m)
{
/*	uint index=m.countNamedProperties();
	
	SimpleBase b0;
	b0.computeAllDescriptors(m);
	
	ConnectivityBase b1;
	b1.computeAllDescriptors(m);
	
	PartialChargeBase b2;
	b2.computeAllDescriptors(m);
	
	SurfaceBase b3;
	b3.computeAllDescriptors(m);
	
	uint i=0;
	for(; index<m.countNamedProperties();index++)
	{
		descriptor_matrix_[i].push_back(((String)m.getNamedProperty(i).getString()).toDouble());
		i++;
	}
	
*/	
	//simple descriptors:
	
	AtomicPolarizabilities simple0;
 	descriptor_matrix_[0].push_back(simple0.compute(m));

	AtomInformationContent simple1;
	descriptor_matrix_[1].push_back(simple1.compute(m));
 
	BondPolarizabilities simple2;
	descriptor_matrix_[2].push_back(simple2.compute(m));

	FormalCharge simple3;
	descriptor_matrix_[3].push_back(simple3.compute(m));

	MeanAtomInformationContent simple4;
	descriptor_matrix_[4].push_back(simple4.compute(m));

 	MolecularWeight simple5;
 	descriptor_matrix_[5].push_back(simple5.compute(m));

	NumberOfAromaticAtoms simple6;
	descriptor_matrix_[6].push_back(simple6.compute(m));
	
	NumberOfAromaticBonds simple7;
	descriptor_matrix_[7].push_back(simple7.compute(m));

	NumberOfAtoms simple8;
	descriptor_matrix_[8].push_back(simple8.compute(m));

	NumberOfAtoms simple9;
	descriptor_matrix_[9].push_back(simple9.compute(m));

	NumberOfAtoms simple10;
	descriptor_matrix_[10].push_back(simple10.compute(m));

	NumberOfBonds simple11;
	descriptor_matrix_[11].push_back(simple11.compute(m));
	
	NumberOfBoron simple12;
	descriptor_matrix_[12].push_back(simple12.compute(m));

	NumberOfBromine simple13;
	descriptor_matrix_[13].push_back(simple13.compute(m));
	
	NumberOfCarbon simple14;
	descriptor_matrix_[14].push_back(simple14.compute(m));
	
	NumberOfChlorine simple15;
	descriptor_matrix_[15].push_back(simple15.compute(m));
	
	NumberOfDoubleBonds simple16;
	descriptor_matrix_[16].push_back(simple16.compute(m));
	
	NumberOfFlourine simple17;
	descriptor_matrix_[17].push_back(simple17.compute(m));
	
	NumberOfHeavyAtoms simple18;
	descriptor_matrix_[18].push_back(simple18.compute(m));
	
	NumberOfHeavyBonds simple19;
	descriptor_matrix_[19].push_back(simple19.compute(m));
	
	NumberOfHydrogen simple20;
	descriptor_matrix_[20].push_back(simple20.compute(m));
	
	NumberOfHydrogenBondAcceptors simple21;
	descriptor_matrix_[21].push_back(simple21.compute(m));
	
	NumberOfHydrogenBondDonors simple22;
	descriptor_matrix_[22].push_back(simple22.compute(m));

	NumberOfHydrophobicAtoms simple23;
	descriptor_matrix_[23].push_back(simple23.compute(m));
	
	NumberOfIodine simple24;
	descriptor_matrix_[24].push_back(simple24.compute(m));
	
	NumberOfNitrogen simple25;
	descriptor_matrix_[25].push_back(simple25.compute(m));

	NumberOfOxygen simple26;
	descriptor_matrix_[26].push_back(simple26.compute(m));

	NumberOfPhosphorus simple27;
	descriptor_matrix_[27].push_back(simple27.compute(m));
	
	NumberOfRotatableBonds simple28;
	descriptor_matrix_[28].push_back(simple28.compute(m));
	
	NumberOfSingleBonds simple29;
	descriptor_matrix_[29].push_back(simple29.compute(m));
	
	NumberOfSulfur simple30;
	descriptor_matrix_[30].push_back(simple30.compute(m));
		
	NumberOfTripleBonds simple31;
	descriptor_matrix_[31].push_back(simple31.compute(m));
	
	PrincipalMomentOfInertiaX simple32;
	descriptor_matrix_[32].push_back(simple32.compute(m));
	
	PrincipalMomentOfInertiaY simple33;
	descriptor_matrix_[33].push_back(simple33.compute(m));
	
	PrincipalMomentOfInertiaZ simple34;
	descriptor_matrix_[34].push_back(simple34.compute(m));
	
	RelNumberOfRotatableBonds simple35;
	descriptor_matrix_[35].push_back(simple35.compute(m));

	RelNumberOfRotatableSingleBonds simple36;
	descriptor_matrix_[36].push_back(simple36.compute(m));
	
	SizeOfSSSR simple37;
	descriptor_matrix_[37].push_back(simple37.compute(m));
	
	VertexAdjacency simple38;
	descriptor_matrix_[38].push_back(simple38.compute(m));
	
	VertexAdjacencyEquality simple39;
	descriptor_matrix_[39].push_back(simple39.compute(m));


	// connectivity descriptors:

	BalabanIndexJ connectivity0;
	descriptor_matrix_[40].push_back(connectivity0.compute(m));

	ZagrebIndex connectivity1;
	descriptor_matrix_[41].push_back(connectivity1.compute(m));
	

	// partial charge descriptors:

	RelNegativePartialCharge partial_charge0;
	descriptor_matrix_[42].push_back(partial_charge0.compute(m));
	
	RelPositivePartialCharge partial_charge1;
	descriptor_matrix_[43].push_back(partial_charge1.compute(m));

	TotalNegativePartialCharge partial_charge2;
	descriptor_matrix_[44].push_back(partial_charge2.compute(m));

	TotalPositivePartialCharge partial_charge3;
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


void QSARData::readCSVFile(const char* file, int no_y, bool xlabels, bool ylabels, const char* sep, bool appendDescriptors)
{
	ifstream input0(file);
	String s;
	getline(input0,s);
	int prop=s.countFields(sep);
	int old_size=descriptor_matrix_.size(); 
		
	if(!appendDescriptors)
	{
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
	}
		
	ifstream input(file);
	if(!input)
	{
		throw BALL::Exception::FileNotFound(__FILE__,__LINE__,file);
	}
	
	int line=1;
	SortedList<int> newInvalidDescriptors; 
	SortedList<int> tmp; 
			
	for(int i=0; !input.eof(); i++)
	{
		String s;
		getline(input,s);
		if(s=="")
		{
			break;
		}
		if(invalidSubstances_.contains(i))
		{
			continue;
		}
		
		if(appendDescriptors && line-xlabels>(int)descriptor_matrix_[0].size())
		{
			throw Exception::PropertyError(__FILE__,__LINE__, file, line, "There are more lines containing additional descriptors than substances already read!");
		}
	
		if(line==1 && xlabels)   // read first line consisting of descriptor names
		{
			for(int i=0;i<prop-no_y;i++) 
			{
				column_names_.push_back(s.getField(i,sep));
			}
			line++;
			continue;
		}
		
		else if(line==1 && !xlabels)
		{
			for(int i=0;i<prop-no_y;i++) 
			{
				column_names_.push_back("unknown");
			}
		}
		
				
		for(int i=0;i<prop;i++) //read current line consisting of descriptor and activity values
		{	
			if(i==0 && ylabels)
			{
				if(appendDescriptors)
				{
					continue;
				}
				substance_names_.push_back(s.getField(i,sep));
				continue;
			}
			else if(i==0 && !ylabels && !appendDescriptors)
			{
				substance_names_.push_back("unknown");
			}
				
			
			if(i<prop-no_y)
			{	
				int no=i; // number of current descriptor
				if(ylabels)
				{
					no--;
				}
				if(appendDescriptors)
				{
					no += old_size;
				}
								
				try
				{
					descriptor_matrix_[no].push_back(s.getField(i,sep).toDouble());
				}	
				catch(BALL::Exception::InvalidFormat g) 
				{
					
					descriptor_matrix_[no].push_back(0);
					if(!tmp.contains(i))
					{
						tmp.insert(i);
					}
					if(!newInvalidDescriptors.contains(no))
					{
						newInvalidDescriptors.insert(no);
					}
					
					
// 					throw Exception::PropertyError(__FILE__,__LINE__, file, line, "Some descriptor properties are not numerical values!");
				}
			}
			else if(!appendDescriptors)
			{
				try
				{
					Y_[i-(prop-no_y)].push_back(s.getField(i,sep).toDouble());
				}
				catch(BALL::Exception::InvalidFormat g) 
				{
					throw Exception::PropertyError(__FILE__,__LINE__, file, line, "Some properties for activities are not numerical values!");
				}
			}
		} 
		line++;
	}
	
	if(appendDescriptors && line-xlabels<(int)descriptor_matrix_[0].size())
	{
		throw Exception::PropertyError(__FILE__,__LINE__, file, line, "There are less lines containing additional descriptors than substances already read!");
	}
	
	invalidDescriptors_=tmp;
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
	for(unsigned int n=0;n<descriptor_matrix_[0].size();n++)
	{
		String var="";
		// replace all x-values for the current substance
		for(unsigned int m=0; m<descriptor_matrix_.size();m++)
		{
			var=var+"x"+String(m)+"="+String(descriptor_matrix_[m][n])+";";
		}
		
		//calculate values for Y_
		for(unsigned int i=0; i<v.size();i++)
		{
			ParsedFunction<float> f = var+v[i];
			double d = f(0);
			Y_[i][n]=d;
			//cout<<d<<endl;
		}
	}
}

void QSARData::discretizeY(vector<double> thresholds)
{
	for(unsigned int i=0; i<Y_.size();i++)
	{
		for(unsigned int j=0; j<Y_[0].size(); j++)
		{
			if(Y_[i][j]<thresholds[0])
			{
				Y_[i][j]=0;
			}
			else if(Y_[i][j]>=thresholds[thresholds.size()-1])
			{
				Y_[i][j]=thresholds.size();
			}
			else
			{
				for(unsigned int k=0; k<thresholds.size()-1;k++)
				{
					if(Y_[i][j]>=thresholds[k] && Y_[i][j]<thresholds[k+1])
					{
						Y_[i][j]=k+1;
					}
				}
			}
		}
	}
}

void QSARData::transformX(vector<String> v)
{
	if(v.size()!=descriptor_matrix_.size())
	{
		cout << "ERROR: wrong number of equations"<<endl; return;
	}
	
	// set all descriptor values
	for(unsigned int m=0;m<descriptor_matrix_.size();m++)
	{
		for(unsigned int n=0;n<descriptor_matrix_[0].size();n++)
		{
			String var="";
			var=var+"x="+String(descriptor_matrix_[m][n])+";";
			ParsedFunction<float> f = var+v[m];
			double d = f(0);
			descriptor_matrix_[m][n]=d;
		}
	}	

}


vector<QSARData*> QSARData::partitionInputData(int p)
{
	vector<QSARData*> v(p);
	for(int i=0; i<p;i++)
	{
		v[i] = new QSARData;
		v[i]->column_names_ = column_names_;
		v[i]->descriptor_transformations_ = descriptor_transformations_;
		v[i]->y_transformations_ = y_transformations_;
		v[i]->descriptor_matrix_.resize(descriptor_matrix_.size());
		v[i]->Y_.resize(Y_.size());
	}
	
	for(unsigned int i=0; i<descriptor_matrix_[0].size();i++)
	{
		//insert substance i into the (i%p)'th QSARData object
		v[i%p]->insertSubstance(this, i);
	}	
	
	return v;	
}


vector<QSARData*> QSARData::generateExternalSet(double fraction)
{
	vector<QSARData*> v(2);	
	v[0] = new QSARData;  // training set 
	v[1] = new QSARData;  // external validation set
	
	v[0]->descriptor_transformations_ = descriptor_transformations_;
	v[0]->y_transformations_ = y_transformations_;
	v[0]->column_names_ = column_names_;
	v[0]->descriptor_matrix_.resize(descriptor_matrix_.size());
	v[0]->Y_.resize(Y_.size());
	v[1]->descriptor_transformations_ = descriptor_transformations_;
	v[1]->y_transformations_ = y_transformations_;
	v[1]->column_names_ = column_names_;
	v[1]->descriptor_matrix_.resize(descriptor_matrix_.size());
	v[1]->Y_.resize(Y_.size());
	
	gsl_rng * r = gsl_rng_alloc (gsl_rng_ranlxd2);
	
	SortedList<unsigned int> val;
	unsigned int no_val = static_cast<unsigned int>(descriptor_matrix_[0].size()*fraction);
	
	PreciseTime pt;
	gsl_rng_set(r,pt.now().getSeconds());
	
	/// randomly draw the desired number of external validation compounds
	for(unsigned int i=0;i<no_val;i++)
	{
		int pos = gsl_rng_uniform_int(r,descriptor_matrix_[0].size()-1);
		v[1]->insertSubstance(this,pos);
		val.insert(pos);
	}
	
	/// all compounds not drawn before make up the training set
	SortedList<unsigned int>::Iterator it = val.begin();
	for(unsigned int i=0;i<descriptor_matrix_[0].size();i++)
	{
		if(*it!=i)
		{
			v[0]->insertSubstance(this,i);
		}
		else
		{
			it++;
		}
	}
	
	return v;
}


void QSARData::insertSubstance(QSARData* source, int s)
{
	substance_names_.push_back(source->substance_names_[s]);
 	
	for(unsigned int i=0; i<source->descriptor_matrix_.size();i++)
	{ 
		descriptor_matrix_[i].push_back(source->descriptor_matrix_[i][s]);
	}
	
	for(unsigned int j=0; j<source->Y_.size();j++)
	{
		Y_[j].push_back(source->Y_[j][s]);
	}
}


void QSARData::printMatrix(VMatrix& mat, ostream& out)
{
	if(mat.size()==0)
	{
		return;
	}
	
	for(unsigned int i=0;i<mat[0].size();i++)
	{
		for(unsigned int j=0; j<mat.size();j++)
		{
			out << mat[j][i] <<"\t";
		}
		out<<endl;
	}
	out<<endl;
}

void QSARData::saveToFile(string filename)
{
	ofstream out(filename.c_str());
	
	bool center_data = 0;
	bool center_y = 0;
	if(descriptor_transformations_.size()!=0)
	{
		center_data = 1;
		if(y_transformations_.size()!=0)
		{
			center_y = 1;
		}
	}
	
	out << descriptor_matrix_[0].size()<<"\t"<<descriptor_matrix_.size()<<"\t"<<Y_.size()<<"\t"<<center_data<<"\t"<<center_y<<endl<<endl;
	
	printMatrix(descriptor_matrix_,out);
	printMatrix(Y_,out);
	
	for(unsigned int i=0; i<column_names_.size();i++)
	{ 
		out<<column_names_[i]<<"\t";
	}
	out<<endl<<endl;
	for(unsigned int i=0; i<substance_names_.size();i++)
	{
		out<<substance_names_[i]<<"\t";
	}
	out<<endl<<endl;
	
	printMatrix(descriptor_transformations_,out);
	printMatrix(y_transformations_,out);
}

void QSARData::readMatrix(VMatrix& mat, ifstream& in, unsigned int lines, unsigned int col)
{
	Column c(lines,0);
	mat.resize(col,c);
	String line;
	
	for(unsigned int i=0;i<lines;i++)
	{
		getline(in,line);
		for(unsigned int j=0; j<col;j++)
		{
			mat[j][i] = line.getField(j,"\t").toDouble();
		}
	}
}

void QSARData::readFromFile(string filename)
{
	ifstream in(filename.c_str());
	if(!in)
	{
		throw BALL::Exception::FileNotFound(__FILE__,__LINE__,filename);
	}
	
	String line;
	getline(in,line);
	unsigned int no_subst = (unsigned int) line.getField(0,"\t").toInt();
	unsigned int no_desc = (unsigned int) line.getField(1,"\t").toInt();
	unsigned int no_y = (unsigned int) line.getField(2,"\t").toInt();
	bool center_data = (bool) line.getField(3,"\t").toInt();
	bool center_y = (bool) line.getField(3,"\t").toInt();
	column_names_.resize(no_desc);
	substance_names_.resize(no_subst);
	getline(in,line); // skip empty line
	
	descriptor_matrix_.clear();
	Y_.clear();
	
	readMatrix(descriptor_matrix_, in, no_subst, no_desc); /// read descriptor matrix
	getline(in,line); // skip empty line
	
	readMatrix(Y_, in, no_subst, no_y);  /// read response values
	getline(in,line); // skip empty line
	
	getline(in,line);
	for(unsigned int i=0; i<no_desc;i++) /// read names of descriptors
	{ 
		column_names_[i] = line.getField(i,"\t");
	}
	getline(in,line); // skip empty line
	
	getline(in,line);
	for(unsigned int i=0; i<no_subst;i++) /// read names of substances
	{
		substance_names_[i] = line.getField(i,"\t");
	}
	getline(in,line); // skip empty line
	
	if(center_data)     /// read information about centering of data
	{
		readMatrix(descriptor_transformations_,in,2,no_desc);
		if(center_y)
		{	
			getline(in,line); // skip empty line
			readMatrix(y_transformations_,in,2,no_y);
		}
	}
	else		/// delete all centering information if no centering was done on current input data
	{
		descriptor_transformations_.clear();
		y_transformations_.clear();
	}
}


vector<double>* QSARData::getSubstance(int s)
{
	vector<double>* v = new vector<double>(descriptor_matrix_.size(),0);
	for(unsigned int i=0; i<descriptor_matrix_.size();i++)
	{
		(*v)[i] = descriptor_matrix_[i][s];
	}
	
	for(unsigned int i=0; i<descriptor_transformations_.size();i++)
	{
		double stddev=descriptor_transformations_[i][1];
		(*v)[i]=(*v)[i]*stddev+descriptor_transformations_[i][0];
	}
	
	return v;
}


vector<double>* QSARData::getActivity(int s)
{
	vector<double>* v = new vector<double>(Y_.size(),0);
	for(uint i=0; i<Y_.size();i++)
	{
		(*v)[i] = Y_[i][s];
	}
	
	for(unsigned int i=0; i<y_transformations_.size();i++)
	{
		double stddev=y_transformations_[i][1];
		(*v)[i]=(*v)[i]*stddev+y_transformations_[i][0];
	}	
	return v;
}


const vector<string>* QSARData::getSubstanceNames()
{
	return &substance_names_;
}

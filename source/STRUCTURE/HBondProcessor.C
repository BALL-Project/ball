#include <BALL/STRUCTURE/HBondProcessor.h>

namespace BALL
{

	HBondProcessor::HBondProcessor()
		: BOND_LENGTH_N_H(1.020),
			BOND_LENGTH_C_O(1.240),
			MAX_LENGTH(5.2+4.2),
			vec_()
	{
	}

	HBondProcessor::~HBondProcessor()
	{
	}

	void HBondProcessor::preComputeBonds(ResidueIterator& data)
	{
		//vector<POS> vec;      // includes the atom positions per residue     // class member!!!
		//	ResidueIterator i;
		POS pos;
		int j=0;                // index to serially the residues TOTHINK:maybe it is possible 
		                        //   to identify the residue by residue ID 

		// iteration over all residues of the protein
		// to find the C,N,O atoms
		for(; +data; ++data)
		{
			Residue res=*data;

			PDBAtomIterator ai;
			for(ai=res.beginPDBAtom();+ai;++ai)
			{
				if(ai->getName() == "N")
				{
					pos.posN=ai->getPosition();
				}
				else if(ai->getName() == "C")
				{
					pos.posC=ai->getPosition();
				}
				else if(ai->getName() == "O")
				{
					pos.posO=ai->getPosition();
				}
			}

			// evaluate the position of H 
			pos.posH=pos.posN-((pos.posO-pos.posC)*BOND_LENGTH_N_H/BOND_LENGTH_C_O);

			//set identification number of the residue
			// pos.number=res.getID().toInt();
			pos.number=j;
			j++;
			
			// point pres at data
			pos.pres = &(*data);
			
			vec_.push_back(pos);  // insert the residue in position vector
		}
	}

	bool HBondProcessor::finish()
	{
		// matrix to save the existence of a HBond
		HBondBool_.resize(vec_.size() * vec_.size(), false); //!!!!!!!!!!!TODO: substitute by secondaryStructure

		//create an Hashgrid
		//BoundingBoxProcessor box_processor; 
		// create a mantel box (bounding box)  which should include the protein
		//system.apply(box_processor);

		// identify the edges of the mantel box
		//Vector3 lower = box_processor.getLower();
		//Vector3 upper = box_processor.getUpper();

		//create a grid inside the bounding box
		HashGrid3<POS> atom_grid(lower_, upper_-lower_, MAX_LENGTH);

		// insert all protein-residues in respect of N-atom
		// the last residue does not _have_ an O. => don't use it
		for(Size i=0; i<vec_.size()-1; i++) 
		{
			Vector3 r(vec_[i].posN);
			//!!!!!!!!!!!!!!!!!!!!!!!!!!
			//    Position x, y, z;
			//    atom_grid.getIndices(*(atom_grid.getBox(r)), x, y, z);
			//    std::cout << i << " " <<  x << " " << y << " " << z << " " << r << std::endl;
			//!!!!!!!!!!!!!!!!!!!!!!!!!!
			atom_grid.insert(r, vec_[i]);
		}

		HashGridBox3<POS>::DataIterator data_it;       // iterate over residues of neighbouring boxes
		HashGridBox3<POS>* box;                        // iterate over all boxes of the grid
		HashGridBox3<POS>::BoxIterator box_it;         // iterator
		float energy, dist_ON, dist_CH, dist_OH, dist_CN;

		// now compute the energies and see whether we have a hydrogen bond
		for (Size i=0; i<vec_.size(); i++)
		{
			box = atom_grid.getBox(vec_[i].posN);

			// We iterate over the grid ourselves, since the beginBox()... didn't work... :-(
			Position x, y, z;                            // indices of the actual box
			int size_x = atom_grid.getSizeX();           // size of x dimension
			int size_y = atom_grid.getSizeY();           // size of y dimension
			int size_z = atom_grid.getSizeZ();           // size of z dimension

			atom_grid.getIndices(*box, x, y, z);         // compute the indices of the actual box

			// Iterate over all the neighbouring boxes
			// for (box_it=box->beginBox(); +box_it; ++box_it) //as mentioned beginBox doesn't work
			for (int nx=x-1; (nx < size_x) && (nx < (int)x+2); nx++)
			{
				for (int ny=y-1; (ny < size_y) && (ny < (int)y+2); ny++)
				{
					for (int nz=z-1; (nz < size_z) && (nz < (int)z+2); nz++)
					{
						if ( (nx>=0) && (ny>=0) && (nz>=0) )  // we shouldn'd run outside the box
						{
							// compute the neighbour box
							HashGridBox3<POS> *nb = atom_grid.getBox(nx, ny, nz); 

							//iterate over all residues of the neighbouring box
							for (data_it = nb->beginData(); +data_it; ++data_it)
							{
								// TODO: We don't want H-bonds between neighboring residues!
								//       Does this criterion always work? We should check for
								//       an existing bond between data_it and vec[i] instead!

								if ((int)abs((int)(data_it->number - vec_[i].number)) > 1)
								{
									// compute the distances between the relevant atoms
									dist_ON = (vec_[i].posO - data_it->posN).getLength();
									dist_CH = (vec_[i].posC - data_it->posH).getLength();
									dist_OH = (vec_[i].posO - data_it->posH).getLength();
									dist_CN = (vec_[i].posC - data_it->posN).getLength();

									// compute the electrostatic energy of the bond-building groups
									energy  = 0.42*0.20 * 332.;
									energy *= (1./dist_ON + 1./dist_CH - 1./dist_OH - 1./dist_CN);

									//			HBondBool_[vec_[i].number + vec_.size()*(data_it->number)] = 
									//	(energy < -0.5) ? true : false;
									
									if (energy < -0.5)
									{
										HBondBool_[vec_[i].number + vec_.size()*(data_it->number)] = true;
										AtomIterator ai;
										Atom *pacceptor = 0;
										Atom* donor = 0; 
										for(ai=(vec_[i].pres)->beginAtom(); +ai; ++ai)
										{
											if(ai->getName() == "O")
											{
												pacceptor=&(*ai);
											}
										}	
										
										for(ai=(data_it->pres)->beginAtom(); +ai; ++ai)
										{
											if(ai->getName() == "N")
											{
												donor=&*ai;
											}
										}		
										if (!donor || !pacceptor) continue;
										donor->setProperty("HBOND_DONOR", *pacceptor);
									}
									else
									{
										HBondBool_[vec_[i].number + vec_.size()*(data_it->number)] = false;
									}
									
								}
							} 
						}
					}
				}
			}
		}

		for (Size i=0; i<vec_.size(); i++)
		{
			for (Size j=0; j<vec_.size(); j++)
			{
				if (HBondBool_[i+j*vec_.size()])
				{
					//		std::cout << "HBond between " << i << " O and " << j << " N" << std::endl;
				}
			}
		}

		return true;
	}

	Processor::Result HBondProcessor::operator() (Composite &composite)
	{
		ResidueIterator ri;
	 // create a mantel box (bounding box)  which should include the composit
		BoundingBoxProcessor bp;

		// do we have a system?
		if (RTTI::isKindOf<System>(composite))
		{
			System *s = RTTI::castTo<System>(composite);
			s->apply(bp);
			upper_ = bp.getUpper();
			lower_ = bp.getLower();

			ri = s->beginResidue();
		}
		if (RTTI::isKindOf<Protein>(composite))
		{
			Protein *s = RTTI::castTo<Protein>(composite);
			s->apply(bp);
			upper_ = bp.getUpper();
			lower_ = bp.getLower();

			ri = s->beginResidue();
		}
		if (RTTI::isKindOf<Chain>(composite))
		{
			Chain *s = RTTI::castTo<Chain>(composite);
			s->apply(bp);
			upper_ = bp.getUpper();
			lower_ = bp.getLower();

			ri = s->beginResidue();
		}

		if (!(+ri))
		{
			return Processor::CONTINUE;
		}

		// compute the H-Bonds
		preComputeBonds(ri);
		return Processor::BREAK;

	}

	std::ostream& operator << (std::ostream& s, const BALL::HBondProcessor::POS& p)
	{
		return s << p.posC << " " << p.posN << " " << p.posH << " " << p.posO << " " << p.number << std::endl;
	}

}

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HBondProcessor.C,v 1.9 2005/02/11 15:27:13 oliver Exp $
//

#include <BALL/STRUCTURE/HBondProcessor.h>
#include <BALL/STRUCTURE/peptides.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/system.h>
#include <BALL/STRUCTURE/geometricProperties.h>

namespace BALL
{

	HBondProcessor::HBondProcessor()
		:	MAX_LENGTH(5.2 + 4.2),
			BOND_LENGTH_N_H(1.020),
			BOND_LENGTH_C_O(1.240),
			vec_()
	{
	}

	HBondProcessor::~HBondProcessor()
	{
	}

	void HBondProcessor::preComputeBonds_(ResidueIterator& data)
	{
		POS pos;
		int j = 0;                //   index to serially the residues TOTHINK:maybe it is possible 
		                        //   to identify the residue by residue ID 
		bool haveO = false;
		bool haveN = false;
		bool haveC = false;
		bool found_N_term = false;
		
		// iteration over all residues of the protein
		// to find the C,N,O atoms

		// the N-terminus is special

		PDBAtomIterator ai;
		Residue res = *data;
  
		for(; +data && (!found_N_term) ; ++data)
		{	
			res = *data;
		
			for(ai = res.beginPDBAtom();+ai;++ai)
			{
				if(ai->getName() ==  "C")
				{
					pos.pos_C = ai->getPosition();
					haveC = true;
				}
				else if(ai->getName() ==  "O")
				{
					pos.pos_O = ai->getPosition();
					haveO = true;
				}
				else if(ai->getName() ==  "N")
				{
					pos.pos_N = ai->getPosition();
          haveN = true;
				}
			}	

			if(haveN && haveO && haveC)
			{ // we have to overread incomplete residues					
				pos.is_complete = true;
    	}
			else
			{
				pos.is_complete = false;
			}

			pos.number = j;
			j++;
			pos.res = &(*data);
			++data;
			found_N_term = true;
			vec_.push_back(pos);  
		}	
		
		//iterate over the following residues
		for(; +data; ++data)
		{	
			res = *data;

			//initialize the bool
			haveO = false;
			haveN = false;
			haveC = false;
			
			if (!res.isAminoAcid())
			{
				continue;
			}

			for(ai = res.beginPDBAtom();+ai;++ai)
			{
				if(ai->getName() ==  "N")
				{
					pos.pos_N = ai->getPosition();
					haveN = true;
				}
				else if(ai->getName() ==  "C")
				{
					pos.pos_C = ai->getPosition();
					haveC = true;
				}
				else if(ai->getName() ==  "O")
				{
					pos.pos_O = ai->getPosition();
					haveO = true;
				}
			}
			
			if(haveO && haveN && haveC)
			{ // we have to overread the incomplete residues					
				pos.is_complete = true;
			}
			else
			{
				pos.is_complete = false;
			}
			 	
			// evaluate the position of H 
			Vector3 O = vec_[j-1].pos_O;
			Vector3 C = vec_[j-1].pos_C;

			//TODO: Division durch 0!!!
			if ((O - C).getLength() !=  0.)
			{
				pos.pos_H = pos.pos_N - ((O-C) * BOND_LENGTH_N_H)/(O-C).getLength();
			}
			else
			{
				pos.is_complete = false;
			}

			//set identification number of the residue
			// pos.number = res.getID().toInt();
			pos.number = j;
			j++;
			
			// point pres at data
			pos.res = &(*data);
			
			vec_.push_back(pos);  // insert the residue in position vector
		}
	}



	bool HBondProcessor::finish()
	{
		//      TODO  :  if vec_.size() =  = 0 there was no composit 

		// matrix to save the existence of a HBond
		h_bond_pairs_.resize(vec_.size()); 

		//create a grid inside the bounding box
		HashGrid3<POS> atom_grid(lower_, upper_ - lower_, MAX_LENGTH);


		// insert all protein-residues in respect of N-atom
		// the last residue does not _have_ an O.  = > don't use it
//		for(Size i = 0; i<(vec_.size()-1); i++) 
		for(Size i = 0; i<(vec_.size()); i++) 
		{
			if (vec_[i].is_complete)
			{
				Vector3 r(vec_[i].pos_N);
				atom_grid.insert(r, vec_[i]);
			}
		}                   

		HashGridBox3<POS>::DataIterator data_it;       // iterate over residues of neighbouring boxes
		HashGridBox3<POS>* box;                        // iterate over all boxes of the grid
		HashGridBox3<POS>::BoxIterator box_it;         // iterator
		float energy, dist_ON, dist_CH, dist_OH, dist_CN;

 		// now compute the energies and see whether we have a hydrogen bond
		for (Size i = 0; i<vec_.size(); i++)
		{
			if (vec_[i].is_complete)
			{
				box = atom_grid.getBox(vec_[i].pos_N);

				// We iterate over the grid ourselves, since the beginBox()... didn't work... :-(
				Position x, y, z;                            // indices of the actual box
				int size_x = atom_grid.getSizeX();           // size of x dimension
				int size_y = atom_grid.getSizeY();           // size of y dimension
				int size_z = atom_grid.getSizeZ();           // size of z dimension

				atom_grid.getIndices(*box, x, y, z);         // compute the indices of the actual box

				// Iterate over all the neighbouring boxes
				// for (box_it = box->beginBox(); +box_it; ++box_it) //as mentioned beginBox doesn't work
				for (int nx = x-1; (nx < size_x) && (nx < (int)x+2); nx++)
				{
					for (int ny = y-1; (ny < size_y) && (ny < (int)y+2); ny++)
					{
						for (int nz = z-1; (nz < size_z) && (nz < (int)z+2); nz++)
						{
							if ( (nx >= 0) && (ny >= 0) && (nz >= 0) )  // we shouldn't run outside the box
							{
								// compute the neighbour box
								HashGridBox3<POS> *nb = atom_grid.getBox(nx, ny, nz); 

								//iterate over all residues of the neighbouring box
								for (data_it = nb->beginData(); +data_it; ++data_it)
								{
									// TODO: We don't want H-bonds between neighboring residues!
									//       Does this criterion always work? We should check for
									//       an existing bond between data_it and vec[i] instead!

									if (((int)abs((int)(data_it->number - vec_[i].number)) > 1) && (data_it->number!= 0))
									{
										// compute the distances between the relevant atoms
										dist_ON = (vec_[i].pos_O - data_it->pos_N).getLength();
										dist_CH = (vec_[i].pos_C - data_it->pos_H).getLength();
										dist_OH = (vec_[i].pos_O - data_it->pos_H).getLength();
										dist_CN = (vec_[i].pos_C - data_it->pos_N).getLength();

										// compute the electrostatic energy of the bond-building groups
										energy  = 0.42 * 0.20 * 332.;
										energy *=  (1./dist_ON + 1./dist_CH - 1./dist_OH - 1./dist_CN);

										if (energy < -0.5)
										{
											h_bond_pairs_[vec_[i].number].push_back(data_it->number);
											AtomIterator ai;
											Atom *acceptor = 0;
											Atom* donor = 0; 
											for(ai = (vec_[i].res)->beginAtom(); +ai; ++ai)
											{
												if(ai->getName() ==  "O")
												{
													acceptor = &(*ai);
												}
											}	

											for(ai = (data_it->res)->beginAtom(); +ai; ++ai)
											{
												if(ai->getName() ==  "N")
												{
													donor = &*ai;
												}
											}		

											if (!donor || !acceptor) continue;

											Bond* bond = donor->createBond(*acceptor);
											bond->setType(Bond::TYPE__HYDROGEN);
											bond->setOrder(1);
											bond->setName("calculated H-Bond");

											donor->setProperty("HBOND_DONOR", *acceptor);
										}
									}
								} 
							}
						}
					}
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
		}else
		if (RTTI::isKindOf<Protein>(composite))
		{
			Protein *s = RTTI::castTo<Protein>(composite);
			s->apply(bp);
			upper_ = bp.getUpper();
			lower_ = bp.getLower();
		
      ri = s->beginResidue();
		
		}else
		if (RTTI::isKindOf<Chain>(composite))
		{
			Chain *s = RTTI::castTo<Chain>(composite);
			s->apply(bp);
			upper_ = bp.getUpper();
			lower_ = bp.getLower();
	
			ri = s->beginResidue();
		
		}
		
		if (!(+ri))// ri doesn't seem to exist
		{
			return Processor::CONTINUE;
		}

		// compute the H-Bonds
		
		preComputeBonds_(ri);
	  	
		return Processor::BREAK;

	}

	std::ostream& operator << (std::ostream& s, const BALL::HBondProcessor::POS& p)
	{
		return s << p.pos_C << " " << p.pos_N << " " << p.pos_H << " " << p.pos_O << " " << p.number << std::endl;
	}

	const std::vector< std::vector<int> >& HBondProcessor::getHBondPairs() const
	{
		return h_bond_pairs_;
	}

	const std::vector<HBondProcessor::POS>& HBondProcessor::getPosVec() const
	{
		return vec_;
	}


} //Namespace BALL


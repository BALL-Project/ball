// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/HBondProcessor.h>
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
			residue_data_()
	{
	}

	HBondProcessor::~HBondProcessor()
	{
	}

	/*********************************************************
  *  preComputeBonds identifies characteristic amino acid
	*  atoms (C, N, O), and stores them in residue_data_ 
  *********************************************************/
	void HBondProcessor::preComputeBonds_(ResidueIterator& resit)
	{
		// index to enumerate the residues. Unfortunately, we cannot use
		// the residueID, which is not monotonous
		Position j = 0;  
	
		// iteration over all residues of the protein
		// to find the C,N,O atoms
		//
		// since the N-terminus is special, we treat it differently
		// we also have to take care of residues with missing atoms 
		// we find the first complete residue and use it instead 
		for (; +resit ; ++resit)
		{	
// ??????????????? had to remove these lines to prevent segfaults (A.Moll)
//   			if (!resit->isAminoAcid())
//   			{
//   				continue;
//   			}

			bool haveO = false;
			bool haveN = false;
			bool haveC = false;

			ResidueData pos;
	
			Size found = 0;
			for (AtomIterator ai = resit->beginAtom(); +ai; ++ai)
			{
				if (ai->getName() ==  "C")
				{
					pos.pos_C = ai->getPosition();
					haveC = true;
				}
				else if (ai->getName() ==  "O")
				{
					pos.pos_O = ai->getPosition();
					haveO = true;
				}
				else if (ai->getName() ==  "N")
				{
					pos.pos_N = ai->getPosition();
          haveN = true;
				}
				else
				{
					continue;
				}

				found++;
				if (found == 3) break;
			}	

			// we have to overread incomplete residues					
			pos.is_complete = (haveN && haveO && haveC);

			pos.number = j;
			pos.res = &(*resit);

			// the N-terminus is special
			if (pos.is_complete && !(pos.res->isNTerminal()) && (j>0))
			{
				// evaluate the position of H 
				const Vector3 OC(residue_data_[j-1].pos_O - residue_data_[j-1].pos_C);
				const float length = OC.getLength();

				if (!Maths::isZero(length))
				{
					pos.pos_H = pos.pos_N - (OC * BOND_LENGTH_N_H) / length;
				}
				else
				{
					pos.is_complete = false;
				}
			}

			residue_data_.push_back(pos);  
			j++;
		}	
	}



  /*************************************************** 
	 * Finish computes all hbonds of the composite 
	 * and stores them in h_bond_pairs_.
   ***************************************************/
	bool HBondProcessor::finish()
	{
		if (residue_data_.size() == 0) return true;

		// matrix to save the existence of a HBond
		h_bond_pairs_.resize(residue_data_.size()); 

		//create a grid inside the bounding box
		HashGrid3<ResidueData*> atom_grid(lower_, upper_ - lower_, MAX_LENGTH);

		// insert all protein-residues at the position of their N-atom
		for (Size i = 0; i < residue_data_.size(); i++) 
		{
			if (residue_data_[i].is_complete)
			{
				atom_grid.insert(residue_data_[i].pos_N, &residue_data_[i]);
			}
		}                   

		const Index size_x = atom_grid.getSizeX();
		const Index size_y = atom_grid.getSizeY();
		const Index size_z = atom_grid.getSizeZ();

 		// now compute the energies and see whether we have a hydrogen bond
		for (Size i = 0; i < residue_data_.size(); i++)
		{
			const ResidueData& current_res = residue_data_[i];
			if (!current_res.is_complete) continue;

			HashGridBox3<ResidueData*>* const box = atom_grid.getBox(current_res.pos_N);

			// We iterate over the grid ourselves, since the beginBox()... didn't work... :-(
			Position x, y, z;                            // indices of the actual box
			atom_grid.getIndices(*box, x, y, z);         // compute the indices of the actual box

			// Iterate over all the neighbouring boxes
			// for (box_it = box->beginBox(); +box_it; ++box_it) //as mentioned beginBox doesn't work
			for (int nx = x-1; (nx < size_x) && (nx < (int)x+2); nx++)
			{
				if (nx < 0) continue;

				for (int ny = y-1; (ny < size_y) && (ny < (int)y+2); ny++)
				{
					if (ny < 0) continue;

					for (int nz = z-1; (nz < size_z) && (nz < (int)z+2); nz++)
					{
						if (nz < 0) continue;

						// compute the neighbour box
						HashGridBox3<ResidueData*>* const nb = atom_grid.getBox(nx, ny, nz); 

						//iterate over all residues of the neighbouring box
						HashGridBox3<ResidueData*>::DataIterator data_it;
						for (data_it = nb->beginData(); +data_it; ++data_it)
						{
							// TODO: We don't want H-bonds between neighboring residues!
							//       Does this criterion always work? We should check for
							//       an existing bond between data_it and residue_data_[i] instead!

							// data from neighbouring residue
							const ResidueData& ndata = **data_it;

							if (abs((Index)ndata.number - (Index)(current_res.number)) <= 1 || 
									ndata.number == 0)
							{
								continue;
							}

							// compute the distances between the relevant atoms
							const float dist_ON = (current_res.pos_O - ndata.pos_N).getLength();
							const float dist_CH = (current_res.pos_C - ndata.pos_H).getLength();
							const float dist_OH = (current_res.pos_O - ndata.pos_H).getLength();
							const float dist_CN = (current_res.pos_C - ndata.pos_N).getLength();

							// compute the electrostatic energy of the bond-building groups
							float energy  = 0.42 * 0.20 * 332.;
							energy *=  (1./dist_ON + 1./dist_CH - 1./dist_OH - 1./dist_CN);

							if (energy >= -0.5) continue;

							Atom *acceptor = 0;
							for (AtomIterator ai = current_res.res->beginAtom(); +ai; ++ai)
							{
								if (ai->getName() == "O") 
								{
									acceptor = &(*ai);
									break;
								}
							}	

							Atom* donor = 0; 
							for (AtomIterator ai = ndata.res->beginAtom(); +ai; ++ai)
							{
								if (ai->getName() == "N") 
								{
									donor = &*ai;
									break;
								}
							}		

							if (!donor || !acceptor) continue;

							h_bond_pairs_[current_res.number].push_back(ndata.number);

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

		return true;
	}

	/*********************************************************
	 *  Compute all HBonds for a given composite 
	 *********************************************************/
	Processor::Result HBondProcessor::operator() (Composite &composite)
	{
		ResidueIterator ri;
		
	  // create a bounding box which should include the composite
		BoundingBoxProcessor bp;

		// do we have a system?
		if (RTTI::isKindOf<System>(composite))
		{
			System *s = RTTI::castTo<System>(composite);
			s->apply(bp);
      ri = s->beginResidue();
		}
		else if (RTTI::isKindOf<Protein>(composite))
		{
			Protein *s = RTTI::castTo<Protein>(composite);
			s->apply(bp);
      ri = s->beginResidue();
		}
		else if (RTTI::isKindOf<Chain>(composite))
		{
			Chain *s = RTTI::castTo<Chain>(composite);
			s->apply(bp);
			ri = s->beginResidue();
		}
		
		// ri doesn't seem to exist
		if (!(+ri)) 
		{
			return Processor::CONTINUE;
		}

		upper_ = bp.getUpper();
		lower_ = bp.getLower();

		// compute the H-Bonds
		preComputeBonds_(ri);
	  	
		return Processor::BREAK;

	}

	std::ostream& operator << (std::ostream& s, const BALL::HBondProcessor::ResidueData& p)
	{
		return s << p.pos_C << " " << p.pos_N << " " << p.pos_H << " " << p.pos_O << " " << p.number << std::endl;
	}

	const std::vector< std::vector<Position> >& HBondProcessor::getHBondPairs() const
	{
		return h_bond_pairs_;
	}

	const std::vector<HBondProcessor::ResidueData>& HBondProcessor::getResidueData() const
	{
		return residue_data_;
	}


} //Namespace BALL


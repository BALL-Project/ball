// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HBondProcessor.C,v 1.11 2005/03/02 14:55:04 amoll Exp $
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

	void HBondProcessor::preComputeBonds_(ResidueIterator& resit)
	{
		Position j = 0;  //   index to serially the residues TOTHINK:maybe it is possible 
		                 //   to identify the residue by residue ID 
	
		// iteration over all residues of the protein
		// to find the C,N,O atoms

		for (; +resit ; ++resit)
		{	
			if (!resit->isAminoAcid()) continue;

			bool haveO = false;
			bool haveN = false;
			bool haveC = false;

			POS pos;
	
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
			if (pos.is_complete && j != 0)
			{
				// evaluate the position of H 
				const Vector3 OC(vec_[j-1].pos_O - vec_[j-1].pos_C);
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

			vec_.push_back(pos);  
			j++;
		}	
	}



	bool HBondProcessor::finish()
	{
		if (vec_.size() == 0) return true;

		// matrix to save the existence of a HBond
		h_bond_pairs_.resize(vec_.size()); 

		//create a grid inside the bounding box
		HashGrid3<POS> atom_grid(lower_, upper_ - lower_, MAX_LENGTH);

		// insert all protein-residues in respect of N-atom
		// the last residue does not _have_ an O.  = > don't use it
		for (Size i = 0; i < vec_.size(); i++) 
		{
			if (vec_[i].is_complete)
			{
				atom_grid.insert(vec_[i].pos_N, vec_[i]);
			}
		}                   

		const Index size_x = atom_grid.getSizeX();
		const Index size_y = atom_grid.getSizeY();
		const Index size_z = atom_grid.getSizeZ();

 		// now compute the energies and see whether we have a hydrogen bond
		for (Size i = 0; i < vec_.size(); i++)
		{
			if (!vec_[i].is_complete) continue;

			HashGridBox3<POS>* const box = atom_grid.getBox(vec_[i].pos_N);

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
						HashGridBox3<POS>* const nb = atom_grid.getBox(nx, ny, nz); 

						//iterate over all residues of the neighbouring box
						HashGridBox3<POS>::DataIterator data_it;
						for (data_it = nb->beginData(); +data_it; ++data_it)
						{
							// TODO: We don't want H-bonds between neighboring residues!
							//       Does this criterion always work? We should check for
							//       an existing bond between data_it and vec[i] instead!

							if (abs((Index)data_it->number - (Index)vec_[i].number) <= 1 || 
									data_it->number == 0)
							{
								continue;
							}
							
							// compute the distances between the relevant atoms
							const float dist_ON = (vec_[i].pos_O - data_it->pos_N).getLength();
							const float dist_CH = (vec_[i].pos_C - data_it->pos_H).getLength();
							const float dist_OH = (vec_[i].pos_O - data_it->pos_H).getLength();
							const float dist_CN = (vec_[i].pos_C - data_it->pos_N).getLength();

							// compute the electrostatic energy of the bond-building groups
							float energy  = 0.42 * 0.20 * 332.;
							energy *=  (1./dist_ON + 1./dist_CH - 1./dist_OH - 1./dist_CN);

							if (energy >= -0.5) continue;

							Atom *acceptor = 0;
							for (AtomIterator ai = vec_[i].res->beginAtom(); +ai; ++ai)
							{
								if (ai->getName() == "O") 
								{
									acceptor = &(*ai);
									break;
								}
							}	

							Atom* donor = 0; 
							for (AtomIterator ai = data_it->res->beginAtom(); +ai; ++ai)
							{
								if (ai->getName() == "N") 
								{
									donor = &*ai;
									break;
								}
							}		

							if (!donor || !acceptor) continue;

							h_bond_pairs_[vec_[i].number].push_back(data_it->number);

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



	Processor::Result HBondProcessor::operator() (Composite &composite)
	{
		ResidueIterator ri;
		
	  // create a mantel box (bounding box)  which should include the composite
		BoundingBoxProcessor bp;

		// do we have a system?
		if (RTTI::isKindOf<System>(composite))
		{
			System *s = RTTI::castTo<System>(composite);
			s->apply(bp);
      ri = s->beginResidue();
		}else
		if (RTTI::isKindOf<Protein>(composite))
		{
			Protein *s = RTTI::castTo<Protein>(composite);
			s->apply(bp);
      ri = s->beginResidue();
		}else
		if (RTTI::isKindOf<Chain>(composite))
		{
			Chain *s = RTTI::castTo<Chain>(composite);
			s->apply(bp);
			ri = s->beginResidue();
		}
		
		if (!(+ri))// ri doesn't seem to exist
		{
			return Processor::CONTINUE;
		}

		upper_ = bp.getUpper();
		lower_ = bp.getLower();

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


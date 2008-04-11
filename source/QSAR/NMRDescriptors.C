#include <BALL/QSAR/NMRDescriptors.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/MATHS/analyticalGeometry.h>

#define FLOAT_VALUE_NA 3600. //TODO

namespace BALL
{

	void calculateResidueChiAngles
		(const Protein& protein, HashMap<const Residue*, float>& residue_angles)
	{
		for (ChainConstIterator chain_it = protein.beginChain(); +chain_it; ++chain_it)
		{
			calculateResidueChiAngles(*chain_it, residue_angles);
		}
	}

	void calculateResidueChiAngles
		(const Chain& fragment, HashMap<const Residue*,float>& residue_angles)
	{
		// some variables
		Atom const* N = 0;
		Atom const* CA = 0;
		Atom const* CB = 0;
		Atom const* X = 0;

		int num_of_atoms = 0;
		float angle =  FLOAT_VALUE_NA;
		
		// extract all residues: iterate over all composites and
		// check whether they are Residues
		ResidueConstIterator	it = fragment.beginResidue();
		for (; +it; ++it)
		{
			N = 0;
			CA = 0;
			CB = 0;
			X = 0;

			num_of_atoms = 0;

			angle = FLOAT_VALUE_NA;

			// we assume that GLY has no typical CHI_1 - angle
			if (it->getName() == "GLY" )
			{	
				residue_angles.insert(std::pair<const Residue*, float>(&*it, angle));
				continue;
			}

			AtomConstIterator r_it = it->beginAtom();
			for (; r_it != it->endAtom(); ++r_it)
			{	
				String name = r_it->getName();
				if (name == "N")
				{
					N = &(*r_it);
					num_of_atoms += 1;
				}
				else if (name == "CA")
				{
					CA = &(*r_it);
					num_of_atoms += 1;
				}
				else if (name == "CB")
				{ 
					CB = &(*r_it);
					num_of_atoms += 1;
				}
			}

			if (num_of_atoms != 3)
			{
				Log.info() << "Torsion angle of " << it->getName() << " could not be computed!" << std::endl;
				residue_angles.insert(std::pair<const Residue*, float>(&*it,  FLOAT_VALUE_NA));
				continue;
			}

			// determine the missing atom X
			/* Sidechain dihedral angle chi1 is defined
				 (for non-Gly residues) as follows:

					Chi1:	N(i)-CA(i)-CB(i)-X(i)

					where X is the following atom for the following
					residue types:

					X				Residue type
					----		----------------------------
					HB1			Ala (for models with protons,labelled 1HB in PDB files).
					SG			Cys
					CG			Asp, Glu, Phe, His, Lys, Leu, Met,
					Asn, Pro, Gln, Arg, Trp, Tyr
					CG1			Ile, Val
					OG			Ser
					OG1			Thr

			In order to be conform with ShiftX we leave out GLY and ALA
			*/
			String residue_name = it->getName();
			if (residue_name == "GLY")
			{ 
				Log.info() << "Torsion angle of a Glycine could not be computed!" << std::endl;
				residue_angles.insert(std::pair<const Residue*, float>(&*it,  FLOAT_VALUE_NA));
				continue;
			}
			else if (residue_name == "ALA")
			{
				r_it = it->beginAtom();
				for (;r_it != it->endAtom(); ++r_it)
				{	
					if (r_it->getName() == "HB1" || r_it->getName() == "1HB")
					{
						X = &(*r_it);
						num_of_atoms += 1;
						break;
					}
				}
			}
			else if (residue_name == "CYS")
			{
				r_it = it->beginAtom();
				for (;r_it != it->endAtom(); ++r_it)
				{	
					if (r_it->getName() == "SG")
					{
						X = &(*r_it);	
						num_of_atoms += 1;
						break;
					}
				}
			}
			else if (residue_name == "ASP" || residue_name ==  "GLU" || 
					residue_name == "PHE" || residue_name ==  "HIS" || 
					residue_name == "LYS" || residue_name ==  "LEU" ||
					residue_name == "MET" || residue_name ==  "ASN" ||
					residue_name == "PRO" || residue_name ==  "GLN" ||
					residue_name == "ARG" || residue_name ==  "TRP" ||  residue_name == "TYR")
			{
				r_it = it->beginAtom();
				for (;r_it != it->endAtom(); ++r_it)
				{	
					if (r_it->getName() == "CG")
					{
						X = &(*r_it);	
						num_of_atoms += 1;
						break;
					}
				}
			}
			else if (residue_name == "VAL" || residue_name ==  "ILE")
			{									
				r_it = it->beginAtom();
				for (;r_it != it->endAtom(); ++r_it)
				{	
					if (r_it->getName() == "CG1")
					{
						X = &(*r_it); 	
						num_of_atoms += 1;
						break;
					}
				}
			}
			else if (residue_name == "SER") 
			{									
				r_it = it->beginAtom();
				for (;r_it != it->endAtom(); ++r_it)
				{	
					if (r_it->getName() == "OG")
					{
						X = &(*r_it);	
						num_of_atoms += 1;
						break;
					}
				}
			}
			else if (residue_name == "THR") 
			{									
				r_it = it->beginAtom();
				for (;r_it != it->endAtom(); ++r_it)
				{	
					if (r_it->getName() == "OG1")
					{
						X = &(*r_it);
						num_of_atoms += 1;
						break;
					}
				}
			}

			if (num_of_atoms != 4)
			{
				Log.info() << "Chi torsion angle of " << it->getID() << "-"<<  it->getName() << " could not be computed!" << std::endl;
				residue_angles.insert(std::pair<const Residue*, float>(&*it,  FLOAT_VALUE_NA));
				continue;
			}

			Vector3 a = N->getPosition();
			Vector3 b = CA->getPosition();
			Vector3 c = CB->getPosition();
			Vector3 d = X->getPosition();

			//angle = getTorsionAngle(a.x, a.y, a.z, b.x, b.y, b.z, 
			//											 c.x, c.y, c.z, d.x, d.y, d.z)*180./M_PI;

			angle = calculateTorsionAngle(*N,*CA,*CB,*X).toRadian();
			while (angle < 0.)
			{
				angle = angle + 2*BALL::Constants::PI;
			}

			residue_angles.insert(std::pair<const Residue*, float>(&*it, angle));
		}

		return;
	}


	void calculateResidueChi2Angles
		(const Protein& protein, HashMap<const Residue*, float>& residue_angles)
	{
		for (ChainConstIterator chain_it = protein.beginChain(); +chain_it; ++chain_it)
		{
			calculateResidueChi2Angles(*chain_it, residue_angles);
		}
	}


	void calculateResidueChi2Angles
		(const Chain& fragment, HashMap<const Residue*,float>& residue_angles)
	{
		// some variables	
		Atom const* CA = 0;
		Atom const* CB = 0;
		Atom const* CG = 0;
		Atom const* XG = 0;

		int num_of_atoms = 0;
		float angle =  FLOAT_VALUE_NA;
		String residue_name = "";

		// extract all residues: iterate over all composites and
		// check whether they are Residues
		ResidueConstIterator	it = fragment.beginResidue();
		for (; +it; ++it)
		{
			CA = 0;
			CB = 0;
			CG = 0;
			XG = 0;
			num_of_atoms = 0;

			residue_name = it->getName();

			angle = FLOAT_VALUE_NA;

			// GLY, ALA, SER and CYS have no typical CHI2 - angle
			if ( 	(it->getName() == "ALA") 
					|| (it->getName() == "GLY") 
					|| (it->getName() == "SER") 
					|| (it->getName() == "CYS"))
			{
				residue_angles.insert(std::pair<const Residue*, float>(&*it, angle));
				continue;
			}

			//  Sidechain dihedral angle chi2 is defined as follows:
			//  
			// 	Chi2:	CA(i)-CB(i)-CG(i)-XG(i)
			//  where XG is the following atom for the following
			//	residue types:
			//
			//	CG     XG     residue 
			//	-------------------------------------
			// 	CG     CD     PRO, GLN,  GLU, LYS, ARG
			// 		     CD1    LEU,TRP,PHE,TYR, 
			//  		   OD1    ASN, ASP
			//  		   ND1		HIS	 
			//         SD     MET 
			//  CG1    CD1    ILE
			//         1HG1   VAL
			//	CG2	   (1HG2   VAL)
			//				 1HG2		THR
			//	
			//  Note: in some amino acids the atom names can be switched, i.e.
			// 				for chi2 in amino acids PHE:  CD1 <-> CD2
			//          				              TYR:  CD1 <-> CD2
			//        	        			 	      ASP:  OD1 <-> OD2


			AtomConstIterator r_it = it->beginAtom();
			for (; r_it != it->endAtom(); ++r_it)
			{	
				String name = r_it->getName();
				if (name == "CA")
				{
					CA = &(*r_it);
					num_of_atoms += 1;
				}
				else if (name == "CB")
				{ 
					CB = &(*r_it);
					num_of_atoms += 1;	
				}
				if (name == "CG")
				{
					CG = &(*r_it);
					num_of_atoms += 1;
				}
			}

			//look for XG
			if (   (residue_name == "ARG") || (residue_name == "GLN") 
					|| (residue_name == "GLU") || (residue_name == "LYS") 
					|| (residue_name == "PRO"))
			{
				r_it = it->beginAtom();
				for (;r_it != it->endAtom(); ++r_it)
				{	
					if (r_it->getName() == "CD")
					{
						XG = &(*r_it);
						num_of_atoms += 1;
						break;
					}
				}
			}
			else if ( (residue_name == "ASN") || (residue_name == "ASP") )
			{
				r_it = it->beginAtom();
				for (;r_it != it->endAtom(); ++r_it)
				{	
					if (r_it->getName() == "OD1")
					{
						XG = &(*r_it);
						num_of_atoms += 1;
						break;
					}
				}
			}
			else if ( (residue_name == "LEU") || (residue_name == "TRP") 
					||(residue_name == "PHE") || (residue_name == "TYR") )
			{
				r_it = it->beginAtom();
				for (;r_it != it->endAtom(); ++r_it)
				{	
					if (r_it->getName() == "CD1")
					{
						XG = &(*r_it);
						num_of_atoms += 1;
						break;
					}
				}
			}	
			else if (residue_name == "HIS") 
			{
				r_it = it->beginAtom();
				for (;r_it != it->endAtom(); ++r_it)
				{	
					if (r_it->getName() == "ND1")
					{
						XG = &(*r_it);
						num_of_atoms += 1;
						break;
					}
				}
			}
			else if(residue_name == "MET") 	
			{
				r_it = it->beginAtom();
				for (;r_it != it->endAtom(); ++r_it)
				{	
					if (r_it->getName() == "SD")
					{
						XG = &(*r_it);
						num_of_atoms += 1;
						break;
					}
				}
			}
			// we have to take special care about ILE, VAL and THR
			else if(residue_name == "ILE") 	
			{
				r_it = it->beginAtom();
				for (;r_it != it->endAtom(); ++r_it)
				{	
					if (r_it->getName() == "CG1")
					{
						CG = &(*r_it);
						num_of_atoms += 1;
					}
					else if (r_it->getName() == "CD1")
					{
						XG = &(*r_it);
						num_of_atoms += 1;
					}
				}
			}
			else if(residue_name == "VAL") 	
			{
				r_it = it->beginAtom();
				for (;r_it != it->endAtom(); ++r_it)
				{	
					if (r_it->getName() == "1HG1")
					{
						XG = &(*r_it);
						num_of_atoms += 1;
					}
					else if (r_it->getName() == "CG1")
					{
						CG = &(*r_it);
						num_of_atoms += 1;
					}
				}
			}
			else if(residue_name == "THR") 	
			{
				r_it = it->beginAtom();
				for (;r_it != it->endAtom(); ++r_it)
				{	
					if (r_it->getName() == "1HG2")
					{
						XG = &(*r_it);
						num_of_atoms += 1;
					}
					else if (r_it->getName() == "CG2")
					{
						CG = &(*r_it);
						num_of_atoms += 1;
					}
				}
			} 

			if (num_of_atoms != 4)
			{
				Log.info() << "Chi2 torsion angle of " << it->getID() << "-"  << it->getName() << " could not be computed!" << std::endl;	
				residue_angles.insert(std::pair<const Residue*, float>(&*it,  FLOAT_VALUE_NA));
				continue;
			}

			Vector3 a = CA->getPosition();
			Vector3 b = CB->getPosition();
			Vector3 c = CG->getPosition();
			Vector3 d = XG->getPosition();

			angle = calculateTorsionAngle(*CA,*CB,*CG,*XG).toRadian();
			while (angle < 0.)
			{
				angle = angle + 2*BALL::Constants::PI;
			}

			residue_angles.insert(std::pair<const Residue*, float>(&*it, angle));

		}
		return;
	}
}


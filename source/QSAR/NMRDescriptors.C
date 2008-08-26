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
		float angle = 0;// FLOAT_VALUE_NA;
		
		// extract all residues: iterate over all composites and
		// check whether they are Residues
		ResidueConstIterator	it = fragment.beginResidue();
		for (; +it; ++it)
		{
			angle = calculateResidueChiAngles(*it);
			residue_angles.insert(std::pair<const Residue*, float>(&*it, angle));
		}
	}

	//TODO switch to real Angle
	float calculateResidueChiAngles(const Residue& residue)
	{
		// some variables
		Atom const* N = 0;
		Atom const* CA = 0;
		Atom const* CB = 0;
		Atom const* X = 0;

		int num_of_atoms = 0;
		float angle =  0;//FLOAT_VALUE_NA;
	
		String residue_name = residue.getName();

		// we assume that GLY has no typical CHI_1 - angle
		if (residue_name == "GLY" )
		{	
			return angle;
		}

		AtomConstIterator r_it = residue.beginAtom();
		for (; r_it != residue.endAtom(); ++r_it)
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
			Log.info() << "calculateResidueChiAngles: Torsion angle of " << residue_name<< std::endl;
			return angle;
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
		
		if (residue_name == "GLY")
		{ 
			Log.info() << "calculateResidueChiAngles: Torsion angle of a Glycine could not be computed!" << std::endl;
			return angle;
		}
		else if (residue_name == "ALA")
		{
			r_it = residue.beginAtom();
			for (;r_it != residue.endAtom(); ++r_it)
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
			r_it = residue.beginAtom();
			for (;r_it != residue.endAtom(); ++r_it)
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
			r_it = residue.beginAtom();
			for (;r_it != residue.endAtom(); ++r_it)
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
			r_it = residue.beginAtom();
			for (;r_it != residue.endAtom(); ++r_it)
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
			r_it = residue.beginAtom();
			for (;r_it != residue.endAtom(); ++r_it)
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
			r_it = residue.beginAtom();
			for (;r_it != residue.endAtom(); ++r_it)
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
			Log.info() << "calculateResidueChiAngles: Chi torsion angle of " << residue.getID() << "-"<<  residue.getName() << " could not be computed!" << std::endl;
			return angle;
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

		return angle; // TODO return real angle
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
		float angle = 0; //FLOAT_VALUE_NA;
		
		// extract all residues: iterate over all composites and
		// check whether they are Residues
		ResidueConstIterator	it = fragment.beginResidue();
		for (; +it; ++it)
		{
			angle =  calculateResidueChi2Angles(*it);
			residue_angles.insert(std::pair<const Residue*, float>(&*it, angle));
		}
	}


	//TODO switch to real Angle
	float calculateResidueChi2Angles(const Residue& residue) 
	{
		// some variables	
		Atom const* CA = 0;
		Atom const* CB = 0;
		Atom const* CG = 0;
		Atom const* XG = 0;

		int num_of_atoms = 0;
		float angle = 0; // FLOAT_VALUE_NA;
		String residue_name = residue.getName();

		// GLY, ALA, SER and CYS have no typical CHI2 - angle
		if ( 	 (residue_name == "ALA") 
				|| (residue_name == "GLY") 
				|| (residue_name == "SER") 
				|| (residue_name == "CYS"))
		{
			Log.info() << "calculateResidueChi2Angles: Chi torsion angle of " << residue.getID() << "-"<< residue.getName() << " could not be computed!" << std::endl;
			return angle;
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


		AtomConstIterator r_it = residue.beginAtom();
		for (; r_it != residue.endAtom(); ++r_it)
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
			r_it = residue.beginAtom();
			for (;r_it != residue.endAtom(); ++r_it)
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
			r_it = residue.beginAtom();
			for (;r_it != residue.endAtom(); ++r_it)
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
			r_it = residue.beginAtom();
			for (;r_it != residue.endAtom(); ++r_it)
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
			r_it = residue.beginAtom();
			for (;r_it != residue.endAtom(); ++r_it)
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
			r_it = residue.beginAtom();
			for (;r_it != residue.endAtom(); ++r_it)
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
			r_it = residue.beginAtom();
			for (;r_it != residue.endAtom(); ++r_it)
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
			r_it = residue.beginAtom();
			for (;r_it != residue.endAtom(); ++r_it)
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
			r_it = residue.beginAtom();
			for (;r_it != residue.endAtom(); ++r_it)
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
			Log.info() << "calculateResidueChi2Angles: Chi torsion angle of " << residue.getID() << "-"<<  residue.getName() << " could not be computed!" << std::endl;
			return angle;
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
		return angle; // TODO return real angle
	}
}


#ifndef BALL_QSAR_NMRDESCRIPTORS_H
#define BALL_QSAR_NMRDESCRIPTORS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_H
# include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
# include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
# include <BALL/KERNEL/protein.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
# include <BALL/KERNEL/atomContainer.h>
#endif

namespace BALL
{

	/**	Calculate the Chi Angles for each residue.
			This method computes the chi angle of each residue and stores it in the given hash map.  
			All angles are in radian. Note, that the hash map is NOT cleaned up, but the results are
			appended to it.
			@param  residue_angles a hash map containing the chi angles of the residues (returned)
			@param	fragment the kernel object containing the residues
	*/
	BALL_EXPORT void calculateResidueChiAngles(const Protein& protein, HashMap<const Residue*, float>& residue_angles);

		/**	Calculate the Chi Angles for each residue.
			This method computes the chi angle of each residue and stores it in the given hash map.  
			All angles are in radian. Note, that the hash map is NOT cleaned up, but the results are
			appended to it.
			@param  residue_angles a hash map containing the chi angles of the residues (returned)
			@param	fragment the kernel object containing the residues
	*/
	BALL_EXPORT void calculateResidueChiAngles(	const Chain& fragment, 
																							HashMap<const Residue*,float>& residue_angles);
	
	/**	Calculate the Chi Angles for a given residue in radian. 
	 * If the given residue has no chi angle, zero is returned.
	*/
	BALL_EXPORT float calculateResidueChiAngles(const Residue& residue);


	/**	Calculate the Chi2 Angles for each residue.
			This method computes the chi2 angle of each residue and stores it in the given hash map.    
			All angles are in radian. Note, that the hash map is NOT cleaned up, but the results are
			appended to it.
			@param  residue_angles a hash map containing the chi2 angles the residues (returned)
			@param	fragment the kernel object containing the residues
	*/
	BALL_EXPORT void calculateResidueChi2Angles(const Protein& protein, HashMap<const Residue*, float>& residue_angles);

	/**	Calculate the Chi2 Angles for each residue.
			This method computes the chi2 angle of each residue and stores it in the given hash map.  
			All angles are in radian.  Note, that the hash map is NOT cleaned up, but the results are
			appended to it.
			@param  residue_angles a hash map containing the chi2 angles of the residues (returned)
			@param	fragment the kernel object containing the residues
	*/
	BALL_EXPORT void calculateResidueChi2Angles(	const Chain& fragment, 
																							HashMap<const Residue*,float>& residue_angles);
	
	
	/**	Calculate the Chi2 Angles for a given residue in radian. 
	 * If the given residue has no chi angle, zero is returned.
	*/
	BALL_EXPORT float calculateResidueChi2Angles(const Residue& residue);

}

#endif

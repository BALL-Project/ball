// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_MOLECULARSIMILARITY_H
#define BALL_STRUCTURE_MOLECULARSIMILARITY_H

#ifndef BALL_COMMON_H
	   #include <BALL/common.h>
#endif

#ifndef BALL_CONFIG_CONFIG_H
		#include <BALL/CONFIG/config.h>
#endif

#include <BALL/STRUCTURE/smartsMatcher.h>
#include <BALL/KERNEL/system.h>

#ifdef BALL_HAS_OPENBABEL
	#include <openbabel/mol.h>
#endif


namespace BALL
{
	class BALL_EXPORT MolecularSimilarity
	{
		public:

			MolecularSimilarity(String smarts_file);

			void generateFingerprints(System& molecules, vector<vector<Size> >& fingerprints);

			void generateFingerprints(const list<Molecule*>& molecules, vector<vector<Size> >& fingerprints);

			void generateFingerprint(Molecule& molecule, vector<Size>& fingerprint);


			#ifdef BALL_HAS_OPENBABEL
				/** Create an Openbabel-molecule from a given BALL::Molecule
				 * @param suppress_warning if set to true, warning about aromatic bonds
				 * of carboxyl- and guanidinium-groups being de-aromatized (which OpenBabel requires) will not be shown.
				*/
				static OpenBabel::OBMol* createOBMol(const Molecule& mol, bool ignore_hydrogen=0, bool suppress_warning=0);

				/** Create a BALL::Molecule from a given OpenBabel-molecule
				*/
				static Molecule* createMolecule(OpenBabel::OBMol& obmol, bool ignore_hydrogen=0);

				void generateCanSmile(const Molecule& mol, String& cansmile, OpenBabel::OBMol** output_obmol=0, bool ignore_hydrogen=0);

				/** Match the given SMARTS pattern to the supplied smile and return the number of matches.
				 * @param max_matches the maximal number of SMART matches to be made; can be used as a speed-up.
				 * If this number of matches has been found, the SMARTS-matching algorithm will abort.
				 * Specifying zero will not set any such constraint.*/
				void matchSmarts(const String& usmile, const String& smarts, Size& no_matches, Size max_matches=0);
			#endif

			void generatePathFingerprint(Molecule& mol, vector<bool>& fingerprint);

			/** Calculate Tanimoto coefficient for two given binary fingerprints. */
			float calculateSimilarity(vector<bool>& fingerprint1, vector<bool>& fingerprint2);

			void filterRedundantMolecules(const list<Molecule*>& molecules, float similarity_threshold);

			void filterRedundantMolecules(System& molecules, float similarity_threshold);

			/** Calculate similarity between two fingerprints.\n
			If stddev for function-group counts are specified, then the similarity of position i in the fingerprint-vectors is defined as zero if their absolute difference is larger than the the standard deviation, else as 1-abs(difference_i/stddev_i).
			If no stddev for function-group counts are given, the calculated similarity-value is equal to Tanimoto. */
			float calculateSimilarity(vector<Size>& fingerprint1, vector<Size>& fingerprint2, vector<float>* stddev);

			/** Returns the names of the functional groups that have been read from the SMARTS-file */
			const vector<String>& getFunctionalGroupNames();


		protected:

			vector<vector<Size> > fingerprints_;
			vector<String> smarts_;
			vector<String> smart_names_;

			SmartsMatcher matcher_;

			/** Generate a hash-ID for a given molecule-path. \n
			This function was adapted from OpenBabel (finger2.cpp). */
			void generatePathHash_(vector<Size>& path, Size& hash);

			bool generatePathFingerprint_(const Atom* atom, std::vector<Size>& path, std::set<const Bond*>& path_bonds, 
			                              std::vector<bool>& fingerprint);

	};
}

#endif

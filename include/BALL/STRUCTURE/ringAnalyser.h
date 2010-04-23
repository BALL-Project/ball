// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_RINGANALYSER_H
#define BALL_STRUCTURE_RINGANALYSER_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/common.h>
#endif

#include <vector>

namespace BALL
{
	class Atom;

	/**
	 * \brief Methods for the analysis of the structure of ringsystems
	 */
	class BALL_EXPORT RingAnalyser
	{
		public:

			enum RingType 
			{
				NONE,
				TEMPLATE,
				FUSED,
				BRIDGED,
				SPIRO,
				CORE
			};


			struct RingInfo
			{
				vector<Atom*> ring;
				RingType assignment;
				bool neg_angle;
			};


			/**
			 * \brief Default-Constructor
			 */
			RingAnalyser();

			/**
			 * \brief Destructor
			 */
			virtual ~RingAnalyser();

			/**
			 * \brief performs an anylsis of the structure of a ringsystem
			 * @param ringsystem the ringsystem
			 * @return a vector of struct RingInfo, containing information about the way each ring is connected to the ringsystem
			 */
			std::vector<RingInfo> analyseRings(std::vector<std::vector<Atom*> >& ringsystem);

			/**
			 * \brief helper-function; determines, whether an atom is part of a certain ring
			 * @param atom the atom
			 * @param ring the ring
			 * @return true, if the atom is part of the ring, otherwise returns false
			 */
			bool isInRing(const Atom* atom, std::vector<Atom*>& ring);

			/**
			 * \brief performs an agglomerative clustering of single rings to ringsystems
			 * @param rings the smallest set of smallest rings of the input molecule
			 * @return all ringsystems of the input molecule
			 */
			std::vector<std::vector<std::vector<Atom*> > > clusterRings(std::vector<std::vector<Atom*> >& rings);

		private:

			/**
			 * \brief interface for a Ring Template Database (RTD), no functionality yet
			 * @param ring the ring, which is to be checked
			 * @return always returns false by now
			 */
			bool checkRTD(std::vector<Atom*>& ring);

			/**
			 * \brief ckecks, if a ring is connected to the ringsystem in fused way, or bridged way
			 * @param ring the ring, which is to be checked
			 * @param ringset the ringsystem, which is to be checked
			 * @param num_shared_bonds the number of bonds shared with other rings of the system
			 * @param first_bridged_to_fused flag, determines whether the first (if there is one) bridged-ring of a system has already been found
			 * @return assignment (0 = none, 1 = template, 2 = fused, 3 = bridged, 4 = spiro, 5 = core)
			 */
			RingType checkFusedOrBridged(vector<Atom*>& ring, vector<RingInfo>& ringset, Size num_shared_bonds, bool& first_bridged_to_fused);

			/**
			 * \brief ckecks, if a ring is connected to the ringsystem in spiro way
			 * @param ring the ring, which is to be checked
			 * @param ringset the ringsystem, which is to be checked
			 * @return assignment (0 = none, 1 = template, 2 = fused, 3 = bridged, 4 = spiro, 5 = core
			 */
			RingType checkSpiro(vector<Atom*>& ring, vector<RingInfo>& ringset);


			/** Count the shared bonds for each ring.
			 */
			std::vector<Size> countSharedBonds(std::vector<std::vector<Atom*> >& ring_system);
	};

} // namespace BALL

#endif // BALL_STRUCTURE_RINGANALYSER_H

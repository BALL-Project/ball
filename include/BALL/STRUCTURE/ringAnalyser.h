// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ringAnalyser.h,v 1.3 2006/06/29 20:45:31 bertsch Exp $
//
// Author:
//   Holger Franken
//


#ifndef BALL_STRUCTURE_RINGANALYSER_H
#define BALL_STRUCTURE_RINGANALYSER_H

#include <vector>
#include <BALL/KERNEL/atom.h>

namespace BALL
{

        /**
                * \brief class, providing methods for the analysis of the structure of ringsystems
                *
                */
        class RingAnalyser
        {
                public:

                struct RingInfo
                {
                        std::vector<Atom*> ring;
                        Size assignment;
                        bool neg_angle;
                };

                /**
                * \brief Default-Constructor
                */
                RingAnalyser();

                /**
                * \brief Destructor
                */
                ~RingAnalyser();

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
                * @param first_bridged_to_fused flag, determines whether the first (if there is one) bridged-ring of a system has already been found
                * @return assignment (0 = none, 1 = template, 2 = fused, 3 = bridged, 4 = spiro, 5 = core)
                */
                Size checkFusedOrBridged(std::vector<Atom*>& ring, std::vector<RingInfo>& ringset, bool& first_bridged_to_fused);

                /**
                * \brief ckecks, if a ring is connected to the ringsystem in spiro way
                * @param ring the ring, which is to be checked
                * @param ringset the ringsystem, which is to be checked
                * @return assignment (0 = none, 1 = template, 2 = fused, 3 = bridged, 4 = spiro, 5 = core
                */
                Size checkSpiro(std::vector<Atom*>& ring, std::vector<RingInfo>& ringset);


        };

} // namespace BALL

#endif // BALL_STRUCTURE_RINGANALYSER_H

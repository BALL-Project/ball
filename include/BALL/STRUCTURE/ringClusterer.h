// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ringClusterer.h,v 1.3.10.2 2007/04/03 13:29:42 bertsch Exp $
//
// Author:
//   Holger Franken
//


#ifndef BALL_STRUCTURE_RINGCLUSTERER_H
#define BALL_STRUCTURE_RINGCLUSTERER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <vector>


namespace BALL
{

        class Atom;

        /**
                * \brief class, providing a method for agglomerative clustering of single rings to ringsystems
                *
                */
        class BALL_EXPORT RingClusterer
        {
        public:

                /**
                * \brief Default-Constructor
                */
                RingClusterer();

                /**
                * \brief Destructor
                */
                virtual ~RingClusterer();

                /**
                * \brief performs an agglomerative clustering of single rings to ringsystems
                * @param rings the smallest set of smallest rings of the input molecule
                * @return all ringsystems of the input molecule
                */
                std::vector<std::vector<std::vector<Atom*> > > clusterRings(std::vector<std::vector<Atom*> >& rings);
        };

} // namespace BALL

#endif // BALL_STRUCTURE_RINGCLUSTERER_H

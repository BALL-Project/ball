// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ringClusterer.h,v 1.2 2006/06/08 07:30:24 oliver Exp $
//
// Author:
//   Holger Franken
//


#ifndef BALL_STRUCTURE_RINGCLUSTERER_H
#define BALL_STRUCTURE_RINGCLUSTERER_H

#include <vector>


namespace BALL
{

        class Atom;

        /**
                * \brief class, providing a method for agglomerative clustering of single rings to ringsystems
                *
                */
        class RingClusterer
        {
        public:

                /**
                * \brief Default-Constructor
                */
                RingClusterer();

                /**
                * \brief Destructor
                */
                ~RingClusterer();

                /**
                * \brief performs an agglomerative clustering of single rings to ringsystems
                * @param rings the smallest set of smallest rings of the input molecule
                * @return all ringsystems of the input molecule
                */
                std::vector<std::vector<std::vector<Atom*> > > clusterRings(std::vector<std::vector<Atom*> >& rings);
        };

} // namespace BALL

#endif // BALL_STRUCTURE_RINGCLUSTERER_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_RSCONSTRUCTOR_H
#define BALL_STRUCTURE_RSCONSTRUCTOR_H

#include <vector>
#include <BALL/STRUCTURE/ringAnalyser.h>

namespace BALL
{
	class Atom;

/**
        * \brief class, performing the prefabication of ringsystems (i.e. providing them with relative 2D-coordinates)
        *
        */
class BALL_EXPORT RSConstructor
{
  public:

  typedef RingAnalyser::RingInfo RingInfo;

	/**
  * \brief Default-Constructor
  */
  RSConstructor();

  /**
  * \brief Destructor
  */
  virtual ~RSConstructor();

  /**
  * \brief constructs a ringsystem, providing the atoms with relative 2D-coordinates, starting in the point of origin
  * @param analysed_rings the ringsystem to be constructed
  * @param i consecutive numbering of the molecule's ringsystems
  */
  void constructRS(std::vector<RingInfo>& analysed_rings, Size& i);

private:

  /**
  * \brief construct the core-ring as a regular polygon
  * @param ring the core ring
  * @param z consecutive numbering of the molecule's ringsystems
  */
  void buildRegularPolygon(std::vector<Atom*>& ring, Size& z);

  /**
  * \brief attach a fused ring to a (partially) constructed ringsystem
  * @param ring_info the fused ring
  * @param ringsystem the whole ringsystem
  */
  void attachFused(RingInfo& ring_info, std::vector<RingInfo>& ringsystem);

  /**
  * \brief attach a bridged ring to a (partially) constructed ringsystem
  * @param ring_info the bridged ring
  * @param ringsystem the whole ringsystem
  */
  void attachBridged(std::vector<Atom*>& ring, std::vector<RingInfo>& ringsystem);

  /**
  * \brief attach a spiro ring to a (partially) constructed ringsystem
  * @param ring_info the spiro ring
  * @param ringsystem the whole ringsystem
  */
  void attachSpiro(RingInfo& ring_info, std::vector<RingInfo>& ringsystem);

  /**
  * \brief attach a ring template to a (partially) constructed ringsystem (no functionality yet)
  * @param ring_info the template ring
  * @param ringsystem the whole ringsystem
  */
  void attachTemplate(std::vector<Atom*>& ring);

  /**
  * \brief checks, whether an atom has been positioned inside the area of another ring
  * @param test_a the atom to be checked
  * @param ring the ring to be checked
  * @return true, if the atom lies inside the ring area, false, if it doesn't
  */
  bool inside(Atom*& test_a, vector<Atom*>& ring);

};
}
#endif

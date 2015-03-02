// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef CLASHBASE_H
#define CLASHBASE_H

#include "base.h"

#include <BALL/MATHS/angle.h>
#include <BALL/DATATYPE/hashSet.h>
#include <BALL/STRUCTURE/geometricTransformations.h>

/**
 * @brief The ClashDetector class - Simply provides methods to detect clashes
 * within and between molecules. It simply counts all clashes, where a clash
 * is defined as two atoms having a distance that is less than their added
 * vdw-radii minus a tolerance. The standard tolerance used here is 
 * 1.3 Angsroem.
 */
class ClashDetector
{
public:
	/**
	 * @brief ClashDetector - here it is possible to specify how large the 
	 * tolereance shall be.
	 * @param tolerance
	 */
	ClashDetector( float tolerance = 1.2 );
	~ClashDetector();
	
	/**
	 * @brief detectInMolecule - checks each atom of 'ac' against all 
	 * remaining atoms in 'ac' if a clash exists. Each clash is counted once and
	 * the total count is returned.
	 * 
	 * @param ac
	 * @return 
	 */
	int detectInMolecule(BALL::AtomContainer& ac);
	
	/**
	 * @brief detectBetweenMolecules - Detect all clashes between 'ac1' and 'ac2'
	 * but ignores clashes occuring within each fragment
	 * 
	 * @param ac1
	 * @param ac2
	 * @return 
	 */
	int detectBetweenMolecules(BALL::AtomContainer& ac1, BALL::AtomContainer& ac2);
	
private:
	/*
	 * @brief atom3Away - check if two atoms are speparated by at least 3 bonds
	 * 
	 * @param at1
	 * @param at2
	 * @return 
	 */
	bool atom3Away(BALL::Atom& at1, BALL::Atom& at2);
	
	/*
	 * @brief doClash - check if actual dist+tolerance is grater than the 
	 * theoretical vdw-dist
	 * 
	 * @param atm1
	 * @param atm2
	 * @return 
	 */
	bool doClash(BALL::Atom& atm1, BALL::Atom& atm2);
	
	float _tolerance;
};






/**
 * @brief The Rotator class - The general idea is taken from Jan Fuhrmann and
 * Marcel Schumanns implementation of 'rotate' found in 
 * BALL_DOCKING_GENETICDOCK_ROTATE_BOND. The class abstracts all steps that are
 * necessary to rotate along an bond given by two atoms.
 */
class Rotator
{
public:
	Rotator();
	~Rotator();
	
	/**
	 * @brief rotate - rotate along a previously specified axis about 'angle'
	 * degree/radiant. If no axis was set before, nothing will happen.
	 * 
	 * @param angle
	 */
	void rotate(const BALL::Angle &angle);
	
	/**
	 * @brief setAxis - set the rotation axis to the positions given by 'atm1' and
	 * 'atm2' and set atoms that follow after 'atm2' to be rotated. If parent is
	 * specified, the atoms that belong to 'parent' are to be rotated.
	 * 
	 * @param atm1
	 * @param atm2
	 * @param parent
	 * @param angle
	 */
	void setAxis(BALL::Atom& atm1, BALL::Atom& atm2, BALL::Composite* parent = 0);
	
private:
	
	/*
	 * @brief init - set axis and translation from the coordinates of the input
	 * atoms
	 * 
	 * @param atm1
	 * @param atm2
	 */
	void init(BALL::Atom& atm1, BALL::Atom& atm2);
	
	/*
	 * @brief setAtomsToRotate - select the atoms that are to be rotated. First
	 * call should have 'start' and 'probe' to be the atom in the direction
	 * where we want to add atoms to '_to_rotate', whereas 'block' is the atom
	 * that marks the side which is not added.
	 * @param start
	 * @param probe
	 * @param block
	 */
	void setAtomsToRotate(BALL::Atom &start, BALL::Atom &probe, BALL::Atom &block);
	
	/*
	 * @brief findRotateDirection - check whether all atoms in 'direction' are
	 * cildren of 'parent'. If so return true.
	 * 
	 * @param block
	 * @param direction
	 * @param parent
	 * @return 
	 */
	bool findRotateDirection(BALL::HashSet<BALL::Atom *> &tested, 
													 BALL::Atom& direction, BALL::Composite* parent);
	
	BALL::HashSet<BALL::Atom *> _to_rotate; // atoms to be rotated
	
	// these define the rotation axis:
	BALL::Matrix4x4 _translation;
	BALL::Vector3 _axis;
	BALL::Vector3 _basis;
	
};
#endif // CLASHBASE_H

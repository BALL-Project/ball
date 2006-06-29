// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ringAnalyser.C,v 1.3 2006/06/29 20:45:32 bertsch Exp $
//
// Author:
//   Holger Franken
//

#include <BALL/CONCEPT/property.h>
#include <BALL/KERNEL/bond.h>

#include <BALL/STRUCTURE/ringAnalyser.h>
#include <BALL/STRUCTURE/sdGenerator.h>

using namespace std;

namespace BALL
{

        RingAnalyser::RingAnalyser()
        {}


        RingAnalyser::~RingAnalyser()
        {}

        //      check wether the ring appears in the Ring-Template-Database
        bool RingAnalyser::checkRTD(vector<Atom*>& /* ring */)
        {
   /// ???? TODO: add ring template database
                return false;
        }

        //      check, whether an atom is part of a certain ring
        bool RingAnalyser::isInRing(const Atom* atom, vector<Atom*>& ring)
        {
                for (vector<Atom*>::size_type i = 0; i != ring.size(); i++)
                {
                        if (ring[i] == atom)
                        {
                                return true;
                        }
                }

                return false;
        }

        //      check wether a ring is connected in FUSED or BRIDGED type to the ringsystem or neither of them
        Size RingAnalyser::checkFusedOrBridged(vector<Atom*>& ring, vector<RingInfo>& ringset, bool& first_bridged_to_fused)
        {
                //      before each call of this procedure, the Property "found" must be cleared from all bonds in the current ringsystem
                for (vector<RingInfo>::size_type i = 0; i != ringset.size(); i++)
                {
                        {
                                for (vector<Atom*>::size_type j = 0; j != ringset[i].ring.size(); j++)
                                {
                                        for (Atom::BondIterator bond_it = ringset[i].ring[j] -> beginBond(); bond_it != ringset[i].ring[j] -> endBond(); bond_it++)
                                        {
                                                bond_it -> clearProperty(SDGenerator::found);
                                        }
                                }
                        }
                }

                //      find out the number of bonds, that the current ring shares with rest of the ringsystem

                //      iterating over all rings in the ringsystem

                vector<Bond*> shared_bonds_vec;

                Size shared_bonds = 0;
                for (vector<RingInfo>::size_type k = 0; k != ringset.size(); k++)
                {
                        //cout << "------NEW RING!!-------" << endl;
                        if (ringset[k].ring != ring)
                        {
                                //      iterating over all atoms of the current ring
                                for (vector<Atom*>::size_type j = 0; j != ringset[k].ring.size(); j++)
                                {
                                        //      iterating over all bonds of the current atom
                                        for (Atom::BondIterator bond_it2 = ringset[k].ring[j] -> beginBond(); bond_it2 != ringset[k].ring[j] -> endBond(); bond_it2++)
                                        {
                                                //      if the current bond is part of the ring
                                                RingAnalyser ra;
                                                if (ra.isInRing(bond_it2 -> getFirstAtom(), ringset[k].ring) && ra.isInRing(bond_it2 -> getSecondAtom(), ringset[k].ring))
                                                {
                                                        //      iterating over all atoms in the ring, that is to be checked
                                                        for (vector<Atom*>::size_type i = 0; i != ring.size(); i++)
                                                        {
                                                                //      iterating over all bonds of each atom in the ring that is to be checked
                                                                for (Atom::BondIterator bond_it = ring[i] -> beginBond(); bond_it != ring[i] -> endBond(); bond_it++)
                                                                {
                                                                        //      if this bond is part of the ring that is to be checked, too
                                                                        if (ra.isInRing(bond_it -> getFirstAtom(), ring) && ra.isInRing(bond_it -> getSecondAtom(), ring) )
                                                                        {
                                                                                //cout << "--BOND1 = [" << bond_it -> getFirstAtom() -> getName() << "]--[" << bond_it -> getSecondAtom() -> getName() << "]" << endl;
                                                                                //      if the two bonds are the same and neither of them has been found before
                                                                                if ((*bond_it == *bond_it2)  && (!(bond_it -> hasProperty(SDGenerator::found))) && (!(bond_it2 -> hasProperty(SDGenerator::found))))
                                                                                {
                                                                                        //      declare them as found
                                                                                        bond_it -> setProperty(SDGenerator::found);
                                                                                        bond_it2 -> setProperty(SDGenerator::found);

                                                                                        //cout << "shared bond found!!" << endl;

                                                                                        //      increment the number of shared bonds
                                                                                        shared_bonds += 1;
                                                                                        shared_bonds_vec.push_back(&*bond_it);
                                                                                }
                                                                        }
                                                                }
                                                        }
                                                }
                                        }
                                }
                        }
                }

                //      if more than one shared bond is found, check, whether the bonds are neighbours
                //      if they are, its a bridged ring, otherwise the ring is fused to more than one other ring
                bool neighboured_bonds = false;
                if (shared_bonds_vec.size() > 1)
                {
                        for (Size i = 0; i != shared_bonds_vec.size(); i++)
                        {
                                for (Size j = 0; j != shared_bonds_vec.size(); j++)
                                {
                                        if (i != j)
                                        {
                                                if ((shared_bonds_vec[i] -> getFirstAtom() == shared_bonds_vec[j] -> getFirstAtom()) ||
                                                                shared_bonds_vec[i] -> getFirstAtom() == shared_bonds_vec[j] -> getSecondAtom() ||
                                                                shared_bonds_vec[i] -> getSecondAtom() == shared_bonds_vec[j] -> getFirstAtom() ||
                                                                shared_bonds_vec[i] -> getSecondAtom() == shared_bonds_vec[j] -> getSecondAtom()   )
                                                {
                                                        neighboured_bonds = true;
                                                }
                                        }
                                }
                        }
                }

                if (shared_bonds > 1)
                {
                        if (ringset.size() > 2 && !first_bridged_to_fused)
                        {
                                first_bridged_to_fused = true;
                                return 2;               //      for construction reasons, the first bridged-ring must be assigned FUSED
                        }
                        else if (!neighboured_bonds)
                        {
                                return 2;       // if the ring shares more than 1 non-neighboured bond, it is assigned FUSED
                        }
                        else
                        {
                                return 3;               // if the ring shares more than 1 neighboured bond, it is assigned BRIDGED
                        }
                }

                if (shared_bonds == 1)
                {
                        return 2;       // if the ring shares exactly 1 bond with the ringsystem, it is assigned FUSED
                }
                return 0;       //      otherwise it is assigned neither BRIDGED nore FUSED
        }


        //      check wether the ring is connected to ring ringsystem in SPIRO way
        Size RingAnalyser::checkSpiro(vector<Atom*>& ring, vector<RingInfo>& ringset)
        {
                //      count the number of atoms the ring shares with the rest of the ringsystem

                //      iterate over every atom in the ring
                for (vector<Atom*>::size_type i = 0; i != ring.size(); i++)
                {
                        //      iterate over every ring in the ringsystem
                        for (vector<RingInfo>::size_type j = 0; j != ringset.size(); j++)
                        {
                                Size shared_atoms = 0;
                                if (ring != ringset[j].ring)    //      don't compare the ring with itself
                                {
                                        //      iterate over every atom
                                        for (vector<Atom*>::size_type k = 0; k != ringset[j].ring.size(); k++)
                                        {
                                                //      if an atom appears in the ring that is to be checked and in another ring of the ringsystem
                                                if (ring[i] == ringset[j].ring[k])
                                                {
                                                        shared_atoms++; //      increment the number o shared atoms
                                                }
                                        }
                                }
                                if (shared_atoms == 1)
                                {
                                        return 4;       //      if the ring shares exactly      1 atom with the rest of the ringsystem, it is assigned SPIRO
                                }
                        }
                }
                return 0;
        }

        vector<RingAnalyser::RingInfo> RingAnalyser::analyseRings(vector<vector<Atom*> >& ringsystem)
        {
                //              [Procedure A]: Assignment Engine

                vector<RingInfo> ringset;
                bool first_bridged_to_fused = 0;

                //      build a vector, that contains all rings of the current ringsystem and an assignmant, that indicates the way in which the ring is conected to the rest of the system

                for (vector<vector<Atom*> >::size_type i = 0; i != ringsystem.size(); i++)
                {
                        RingInfo record;
                        record.ring = ringsystem[i];
                        record.assignment = 0;                  // "0" means that that the ring has not been given any assignment yet
                        ringset.push_back(record);
                }

                //      count the number of rings, that are not yet assigned
                for (vector<RingInfo>::size_type i = 0; i != ringset.size(); i++)
                {
                        Size unassigned_rings = 0;

                        for (vector<RingInfo>::size_type j = 0; j != ringset.size(); j++)
                        {
                                if (ringset[j].assignment == 0)
                                {
                                        unassigned_rings ++;
                                }
                        }

                        //              1.      If only one ring is left, assign it CORE. Then exit.

                        // ringset[0].assignment = 5;

                        if (unassigned_rings > 1)
                        {

                                //              2.      If the remainig rings constitute a RS (RingSystem) present in the RTD (RingTemplateDatabase),
                                //                      get their coordinates, assign the ring type TEMPLATE and exit


                                if (checkRTD(ringset[i].ring))
                                {
                                        ringset[i].assignment = 1;      //      "1" means "Template"
                                }

                                //              3.      If there is a fused, bridged or spiro ring as determined by [checkFusedOrBridged] or [checkSpiro] :
                                //                      a)      Assign it type FUSED, BRIDGED or SPIRO and remove it from further consideration

                                else
                                {
                                        if (ringset[i].assignment == 0)
                                        {
                                                ringset[i].assignment = checkFusedOrBridged(ringset[i].ring, ringset, first_bridged_to_fused); //       "2" means FUSED and "3" means BRIDGED
                                        }

                                        if (ringset[i].assignment == 0)
                                        {
                                                ringset[i].assignment = checkSpiro(ringset[i].ring, ringset);   //      "4" means SPIRO
                                        }
                                }

                                //                      b)      If using an RTD or only one ring remains, go to step 1 of this procedure

                                //                      c)      Go to beginning of step 3 of this procedure

                        }
                        else
                        {
                                ringset[i].assignment = 5;      //      ...     If only one ring is left, assign it CORE. Then exit
                        }


                        cout << "\t-*-[RingAnalyser]:\t Ring No. " << i << " (" <<  flush;

                        for (Size h = 0; h != ringset[i].ring.size(); h++)
                        {
                                cout << "[" << ringset[i].ring[h] -> getName() << "] " << flush;
                        }

                        cout << ") assigned " << flush;
                        switch (ringset[i].assignment)
                        {
                        case 0:
                                {
                                        cout << "NONE." << endl;

                                        break;
                                }

                        case 1:
                                {
                                        cout << "TEMPLATE." << endl;

                                        break;
                                }

                        case 2:
                                {
                                        cout << "FUSED." << endl;

                                        break;
                                }
                        case 3:
                                {
                                        cout << "BRIDGED." << endl;

                                        break;
                                }
                        case 4:
                                {
                                        cout << "SPIRO." << endl;

                                        break;
                                }
                        case 5:
                                {
                                        cout << "CORE." << endl;

                                        break;
                                }
                        case 6:
                                {
                                        cout << "TOUGH_CORE." << endl;

                                        break;
                                }
                        }

                        cout << endl;
                }
                return ringset;
        }

} // namespace BALL

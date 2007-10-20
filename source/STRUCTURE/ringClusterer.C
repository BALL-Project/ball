// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ringClusterer.C,v 1.4.10.1 2007/03/22 11:48:20 oliver Exp $
//
// Author:
//   Holger Franken
//

#include <BALL/STRUCTURE/ringClusterer.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/STRUCTURE/sdGenerator.h>

#include <vector>

#ifdef BALL_DEBUG_RINGCLUSTERER
# define DEBUG(a) Log.info() << a << std::endl;
#else
# define DEBUG(a)
#endif

namespace BALL
{


	RingClusterer::RingClusterer()
	{}


	RingClusterer::~RingClusterer()
	{}


	//      clustering rings Sizeo connected ringsystems

	vector<vector<vector<Atom*> > > RingClusterer::clusterRings(vector<vector<Atom*> >& rings)
	{
		vector<vector<vector<Atom*> > > clustorator;    //      vector that initially contains all rings, each in its own "ringsystem"
		vector<vector<vector<Atom*> > >::size_type j = 0;

		//      putting all the rings Sizeo the initial ringsystems-vector
		for (vector<vector<Atom*> >::size_type i = 0; i != rings.size(); i++)
		{
			vector<vector<Atom*> > temp;

			temp.push_back(rings[i]);
			clustorator.push_back(temp);
			j++;
		}

		bool change = true; //  flag, that indicates, whether the composition of the set of ringsystems has changed during the last run through the loop

		while(change)
		{
			for (vector<vector<vector<Atom*> > >::size_type i = 0; i != clustorator.size(); i++)
			{
				//      if all the atoms in the current ringsystem have been set to invalid, proceed to the next ringsystem
				bool invalid_ringsys = true;

				for (vector<vector<Atom*> >::size_type p = 0; p != clustorator[i].size(); p++)
				{
					for (vector<Atom*>::size_type q = 0; q != clustorator[i][p].size(); q++)
					{
						if (!(clustorator[i][p][q] -> hasProperty(SDGenerator::INVALID)))
						{
							invalid_ringsys = false;
							break;
						}
					}
				}

				//      otherwise, for every two ringsystems in the initial vector, check, whether they share atoms and if they do put them together into one ringsystem vector

				if (!(invalid_ringsys))  //      only check the remaining valid ringsystems
				{
					for (vector<vector<Atom*> >::size_type j = 0; j != clustorator[i].size(); j++) //        check all rings in the ringsystem
					{
						for (vector<Atom*>::size_type k = 0; k != clustorator[i][j].size(); k++) //      check all atoms in each ring
						{
							for (vector<vector<vector<Atom*> > >::size_type l = 0; l != clustorator.size(); l++)     // and compare them to all other atoms of all other ringsystms
							{
								if (clustorator[i] != clustorator[l]) // don't compare a ringsystem with itself
								{
									for (vector<vector<Atom*> >::size_type m = 0; m != clustorator[l].size(); m++)
									{
										if (clustorator[i][j] != clustorator[l][m]) //   don't compare a ring with itself
										{
											for (vector<Atom*>::size_type n = 0; n != clustorator[l][m].size(); n++)
											{
												bool check_invalid = true;                      // do not check invalid atoms
												for (Size d = 0; d != clustorator[l][m].size(); d++)//
												{//
													if (!(clustorator[l][m][d] -> hasProperty(SDGenerator::INVALID)))//
													{//
														check_invalid = false;//
													}
												}
												if (!check_invalid)
												{
													//      if the same atom appears in rings of two different ringsystems
													if (clustorator[i][j][k] == clustorator[l][m][n])
													{
														//      append the ring to the first ringsystem
														clustorator[i].push_back(clustorator[l][m]);

														//      and set all its atoms to "invalid" in the other one, so that it is not taken Sizeo account any longer
														for (Size p = 0; p != clustorator[l][m].size(); p++)
														{
															clustorator[l][m][p] -> setProperty(SDGenerator::INVALID);
														}
														break;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
			change = false;
		}


		// new return-vector, that will contain only the valid ringsystems
		vector<vector<vector<Atom*> > > ringsystems;

		for (vector<vector<vector<Atom*> > >::size_type i = 0; i != clustorator.size(); i++)
		{
			//      if any atom in the current ringsystem has not been set to invalid, the whole ringsystem is valid and put into the return-vector
			bool invalid_ringsys = true;
			for (vector<vector<Atom*> >::size_type p = 0; p != clustorator[i].size(); p++)
			{
				for (vector<Atom*>::size_type q = 0; q != clustorator[i][p].size(); q++)
				{
					if (!(clustorator[i][p][q] -> hasProperty(SDGenerator::INVALID)))
					{
						invalid_ringsys = false;
						ringsystems.push_back(clustorator[i]);
						break;
					}
				}
			}
		}

		DEBUG("\t-*-[RingClusterer]:\t" << ringsystems.size() << " ringsystems found. ")

		#ifdef BALL_DEBUG_RINGCLLUSTERER
		//      print the ringsystems that were found

		for (vector<vector<vector<Atom*> > >::size_type i = 0; i != ringsystems.size(); i++)
		{
			Log.info() << "Ringsystem No. " << i+1 << " :" << endl;
			for (vector<vector<Atom*> >::size_type j = 0; j != ringsystems[i].size(); j++)
			{
				for (vector<Atom*>::size_type k = 0; k != ringsystems[i][j].size(); k++)
				{
					Log.info() << "[" << ringsystems[i][j][k] -> getName() << "] " << flush;
				}
				Log.info() << endl << "-------------------------" << endl;
			}
		}
		#endif

		return ringsystems;
	}

} // namespace BALL

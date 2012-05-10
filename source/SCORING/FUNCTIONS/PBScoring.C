/* PBScoring.C
 *
 * Copyright (C) 2011 Marcel Schumann
 *
 * This program free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

 // ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/FUNCTIONS/PBScoring.h>
#include <BALL/SCORING/COMPONENTS/PB.h>

using namespace BALL;


PBScoring::PBScoring(AtomContainer& receptor, AtomContainer& ligand, Options& options)
	: ScoringFunction(receptor, ligand, options)
{
	setup();
}


PBScoring::PBScoring(AtomContainer& receptor, Vector3& hashgrid_origin, Options& options)
	: ScoringFunction(receptor, hashgrid_origin, options)
{
	setup();
}


void PBScoring::setup()
{
	ScoringFunction::setName("PBScoring");
	PB* pb = new PB(*this);
	pb->setName("PB");
	pb->setScoringFunction(*this);
	pb->setLigandIntraMolecular(0);
	scoring_components_.push_back(pb);
}


double PBScoring::getES()
{
	double es = 0;
	return es;
}

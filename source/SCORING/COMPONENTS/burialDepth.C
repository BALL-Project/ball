/* burialDepth.C
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

#include <BALL/SCORING/COMPONENTS/burialDepth.h>
#include <BALL/KERNEL/atom.h>

using namespace BALL;


BurialDepth::BurialDepth(ScoringFunction& sf)
	: ScoringComponent(sf)
{
	gridable_ = false;
	setName("BurialDepth");
}

BurialDepth::~BurialDepth()
{

}

void BurialDepth::update(const vector<std::pair<Atom*, Atom*> >& /*pair_vector*/)
{
	// no need to do anything here
}


double BurialDepth::updateScore()
{
	score_ = scoring_function_->getNoNeighboringReceptorAtoms();

	scaleScore();
	return score_;
}

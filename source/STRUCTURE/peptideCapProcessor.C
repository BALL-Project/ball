// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/peptideCapProcessor.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atomHandle.h>     // v2.2 H3c Pattern B: D-H3.8 opt-in
#include <BALL/KERNEL/moleculeStore.h>
//#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/peptides.h>
#include <BALL/STRUCTURE/geometricTransformations.h>


namespace BALL
{
	PeptideCapProcessor::PeptideCapProcessor()
	{
	}

	float PeptideCapProcessor::computeDistance(MoleculeStore* store,
	                                           const std::vector<MoleculeStore::StableId>& a,
	                                           const std::vector<MoleculeStore::StableId>& b)
	{
		// Codex CR finding 1 fix: resolve LIVE atom positions per call.
		// The cap atoms rotate between calls in optimizeCapPosition's
		// torsion loop, so we cannot snapshot positions.
		if (store == nullptr) return 0.0f;

		auto pos_of = [store](MoleculeStore::StableId sid) -> Vector3
		{
			MoleculeStore::Index idx = store->atom_idx_by_stable_id(sid);
			if (idx == MoleculeStore::UNKNOWN_STABLE_ID) return Vector3(0,0,0);
			return store->position(idx);
		};

		float d = 0.0;
		for (Position i = 0; i < a.size(); ++i)
		{
			const Vector3 ai = pos_of(a[i]);
			for (Position j = 0; j < b.size(); ++j)
			{
				d += (ai - pos_of(b[j])).getSquareLength();
			}
		}
		return d;
	}

	void PeptideCapProcessor::optimizeCapPosition(Chain& chain, bool start)
	{
		Vector3 translation;
		Atom* axis   = NULL;
		Residue* cap = NULL;
		// v2.2 H3c Pattern B (D-H3.8) + Codex CR finding 1+2 fixes: sid
		// vectors with single-store discipline. computeDistance resolves
		// to live atom positions at each torsion step.
		std::vector<MoleculeStore::StableId> a;
		std::vector<MoleculeStore::StableId> b;
		MoleculeStore* store = nullptr;   // captured at first push; rejects foreign-store inputs

		Size nr = chain.countResidues();

		// cap at the beginning of a peptide
		if (start)
		{
			// put ACE-C to the center
			for (AtomIterator it = chain.getResidue(1)->beginAtom(); +it; ++it)
			{
				if (it->getName() == "N")
				{
					translation = it->getPosition();
				}

				{
					MoleculeStore* s = it->getStore();
					if (s != nullptr && (store == nullptr || store == s))
					{
						if (store == nullptr) store = s;
						b.push_back(s->stable_id(it->getStoreIndex()));
					}
				}
			}

			cap = chain.getResidue(0);
			for (AtomIterator it = cap->beginAtom(); +it; ++it)
			{
				{
					MoleculeStore* s = it->getStore();
					if (s != nullptr && (store == nullptr || store == s))
					{
						if (store == nullptr) store = s;
						a.push_back(s->stable_id(it->getStoreIndex()));
					}
				}
				if (it->getName() == "C")
				{
					axis = &*it;
				}
			}
		}
		//cap at the end of a peptide
		else
		{
			for (AtomIterator it = chain.getResidue(nr-2)->beginAtom(); +it; ++it)
			{
				if (it->getName() == "C")
				{
					translation = it->getPosition();
				}

				{
					MoleculeStore* s = it->getStore();
					if (s != nullptr && (store == nullptr || store == s))
					{
						if (store == nullptr) store = s;
						b.push_back(s->stable_id(it->getStoreIndex()));
					}
				}
			}

			cap = chain.getResidue(nr-1);
			for (AtomIterator it = cap->beginAtom(); +it; ++it)
			{
				{
					MoleculeStore* s = it->getStore();
					if (s != nullptr && (store == nullptr || store == s))
					{
						if (store == nullptr) store = s;
						a.push_back(s->stable_id(it->getStoreIndex()));
					}
				}
				if (it->getName() == "N")
				{
					axis = &*it;
				}
			}
		}

		//translate the anchor to origin
		TranslationProcessor tlp;
		tlp.setTranslation(translation*-1.0);
		chain.apply(tlp);

		//try all torsions
		float largest_distance = 0.0;
		float tmp_distance = 0.0;
		float torsion      = 0.0;
		float step         = 2.0;

		TransformationProcessor tfp;
		Matrix4x4 m;
		m.setRotation( Angle(step, false), axis->getPosition());
		tfp.setTransformation(m);

		for (Position r = step; r <= 360; r+=step)
		{
			cap->apply(tfp);

			tmp_distance = computeDistance(store, a, b);

			if (largest_distance < tmp_distance)
			{
				largest_distance = tmp_distance;
				torsion = r;
			}
		}

		//apply best rotation angle
		m.setRotation( Angle(torsion, false), axis->getPosition());
		tfp.setTransformation(m);
		cap->apply(tfp);

		//now translate the protein back
		tlp.setTranslation(translation);
		chain.apply(tlp);
	}


	Processor::Result PeptideCapProcessor::operator() (Chain& chain)
	{
		TranslationProcessor    tlp;
		TransformationProcessor tfp;
		Matrix4x4 m;

		FragmentDB fragment_db("");
		ReconstructFragmentProcessor reconstruct(fragment_db);

		Residue* ace_residue = NULL;
		Residue* nme_residue = NULL;

		// check if a ACE cap was already added
		bool add_cap = (chain.getResidue(0)->getName() != "ACE");

		if (add_cap)
		{
			// create ACE-Cap
			ace_residue = new Residue("ACE");
			ace_residue->setProperty(Residue::PROPERTY__AMINO_ACID);
			ace_residue->apply(reconstruct);
			ace_residue->apply(fragment_db.normalize_names);
			ace_residue->apply(fragment_db.build_bonds);

			AtomIterator ace_atom = ace_residue->beginAtom();

			//get all atoms needed to compute the transformation of the cap
			Atom* cAtom   = NULL;
			Atom* ch3Atom = NULL;
			Atom* oAtom   = NULL;
			Vector3 h2Atom(0.0,0.0,0.0);
			Vector3 h3Atom(0.0,0.0,0.0);
			Vector3 nAtom(0.0,0.0,0.0);

			while(+ace_atom)
			{
				if (ace_atom->getName() == "C")
					cAtom = &*ace_atom;
				else if (ace_atom->getName() == "CH3")
					ch3Atom = &*ace_atom;
				else if (ace_atom->getName() == "O")
					oAtom = &*ace_atom;
				++ace_atom;
			}

			// v2.2 H3c Pattern B (D-H3.8): the deferred-delete set is keyed
			// on StableId. Stored Atom* keys would violate the gate; sids
			// survive the intervening transforms / fragment-DB calls that
			// might recycle slots elsewhere. Resolve back via the H3c
			// Phase 0 reverse map at delete time.
			std::vector<MoleculeStore::StableId> to_remove;
			MoleculeStore* to_remove_store = nullptr;   // captured at first push

			AtomIterator n_atom = chain.getResidue(0)->beginAtom();

			while (+n_atom)
			{
				if (n_atom->getName() == "1H")
				{
					n_atom->setName("H");

					if (chain.getResidue(0)->getName() == "PRO")
					{
						MoleculeStore* s = n_atom->getStore();
						// Codex final-CR finding 1 fix: reject foreign-store
						// pushes (parallel guard in the 2H/3H sites below).
						if (s != nullptr && (to_remove_store == nullptr || s == to_remove_store))
						{
							if (to_remove_store == nullptr) to_remove_store = s;
							to_remove.push_back(s->stable_id(n_atom->getStoreIndex()));
						}
					}
				}
				else if (n_atom->getName() == "N")
				{
					nAtom = n_atom->getPosition();
					n_atom->apply(fragment_db.add_hydrogens);
				}
				++n_atom;
			}

			//compute position of OXT... map N of the last residue to it later
			tlp.setTranslation(ch3Atom->getPosition()*-1.0);
			ace_residue->apply(tlp);

			m.setRotation( Angle(180.0 * Constants::PI / 180.0), cAtom->getPosition());

			// map oxt and N of the last residue to the origin
			tlp.setTranslation(m*oAtom->getPosition()*-1.0);
			ace_residue->apply(tlp);

			tlp.setTranslation(nAtom*-1.0);
			chain.apply(tlp);

			n_atom = chain.getResidue(0)->beginAtom();
			while (+n_atom)
			{
				if (n_atom->getName() == "2H")
				{
					h2Atom = n_atom->getPosition();
					MoleculeStore* s = n_atom->getStore();
					// Codex final-CR finding 1 fix: reject foreign-store pushes
					// so delete-time resolution via to_remove_store can't
					// false-match on coincidental sid collision.
					if (s != nullptr && (to_remove_store == nullptr || s == to_remove_store))
					{
						if (to_remove_store == nullptr) to_remove_store = s;
						to_remove.push_back(s->stable_id(n_atom->getStoreIndex()));
					}
				}
				else if (n_atom->getName() == "3H")
				{
					h3Atom = n_atom->getPosition();
					MoleculeStore* s = n_atom->getStore();
					// Codex final-CR finding 1 fix: reject foreign-store pushes
					// so delete-time resolution via to_remove_store can't
					// false-match on coincidental sid collision.
					if (s != nullptr && (to_remove_store == nullptr || s == to_remove_store))
					{
						if (to_remove_store == nullptr) to_remove_store = s;
						to_remove.push_back(s->stable_id(n_atom->getStoreIndex()));
					}
				}
				++n_atom;
			}

			//rotate CH3 atom to 2H position
			Angle   angle    = cAtom->getPosition().getAngle(h2Atom+h3Atom);
			Vector3 rot_axis = (cAtom->getPosition()%(h2Atom+h3Atom)).normalize();

			m.setRotation(angle,rot_axis);
			tfp.setTransformation(m);
			ace_residue->apply(tfp);

			//insert ACE-Cap into chain
			chain.insertBefore(*ace_residue,*chain.getResidue(0));

			//Add Bond between ACE-Cap and Helix
			n_atom = chain.getResidue(1)->beginAtom();
			while (+n_atom)
			{
				if (n_atom->getName() == "N")
				{
					Bond* new_bond = cAtom->createBond(*n_atom);
					new_bond->setOrder(Bond::ORDER__SINGLE);
					break;
				}
				++n_atom;
			}

			//back translation
			tlp.setTranslation(nAtom);
			chain.apply(tlp);

			//remove old hydrogens. Resolve sids via the H3c Phase 0 reverse
			// map; if a slot got recycled since collection, skip (defense-
			// in-depth -- under the single-thread-per-System contract this
			// branch is unreachable within one operator() call).
			if (to_remove_store != nullptr)
			{
				for (Position a = 0; a < to_remove.size(); ++a)
				{
					MoleculeStore::Index idx = to_remove_store->atom_idx_by_stable_id(to_remove[a]);
					if (idx == MoleculeStore::UNKNOWN_STABLE_ID) continue;
					Atom* p = to_remove_store->back_ptr(idx);
					if (p == nullptr) continue;
					delete p;
				}
			}

			//torsional optimzation of ACE
			optimizeCapPosition(chain, true);
		}

		//##################################################################

		add_cap = (chain.getResidue(chain.countResidues()-1)->getName() != "NME");

		if (add_cap)
		{
			//create NME-Cap
			nme_residue = new Residue("NME");
			nme_residue->setProperty(Residue::PROPERTY__AMINO_ACID);
			nme_residue->apply(reconstruct);
			nme_residue->apply(fragment_db.normalize_names);
			nme_residue->apply(fragment_db.build_bonds);

			Residue* last_residue = chain.getResidue(chain.countResidues()-1);

			//####################################################

			Atom* nAtom = NULL;
			Atom* ch3Atom = NULL;
			Atom* hAtom = NULL;

			AtomIterator nme_atom = nme_residue->beginAtom();
			while (+nme_atom)
			{
				if (nme_atom->getName() == "N")
					nAtom = &*nme_atom;
				else if (nme_atom->getName() == "CH3")
					ch3Atom = &*nme_atom;
				else if (nme_atom->getName() == "H")
					hAtom = &*nme_atom;
				++nme_atom;
			}

			Vector3 anchor((  (hAtom->getPosition()-nAtom->getPosition()).normalize()
						+ (ch3Atom->getPosition()-nAtom->getPosition()).normalize()).normalize()*-1.335);
			tlp.setTranslation((anchor + nAtom->getPosition())*-1.0);
			nme_residue->apply(tlp);

			Atom* oxt = NULL;
			Atom* cAtom = NULL;

			AtomIterator c_atom = last_residue->beginAtom();

			while (+c_atom)
			{
				if (c_atom->getName() == "OXT")
					oxt = &*c_atom;
				else if (c_atom->getName() == "C")
				{
					anchor = c_atom->getPosition();
					cAtom  = &*c_atom;
				}
				++c_atom;
			}

			tlp.setTranslation(anchor*-1.0);
			chain.apply(tlp);

			//transform cap to the old OXT position
			Angle angle = nAtom->getPosition().getAngle(oxt->getPosition());
			Vector3 rot_axis = (nAtom->getPosition()%(oxt->getPosition())).normalize();
			m.setRotation(angle,rot_axis);
			tfp.setTransformation(m);
			nme_residue->apply(tfp);

			//insert NME-Cap into chain
			chain.insertAfter(*nme_residue, *last_residue);

			//Add Bond between NME-Cap and Helix
			Bond* new_bond = cAtom->createBond(*nAtom);
			new_bond->setOrder(Bond::ORDER__SINGLE);

			tlp.setTranslation(anchor);
			chain.apply(tlp);

			delete oxt;

			//torsional optimzation of NME
			optimizeCapPosition(chain, false);
		}
		return Processor::CONTINUE;
	}

} // namespace BALL

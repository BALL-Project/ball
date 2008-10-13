#include <BALL/STRUCTURE/DNAMutator.h>

#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/geometricTransformations.h>

#include <list>
#include <set>

#include <BALL/FORMAT/HINFile.h>

namespace BALL
{
	DNAMutator::DNAMutator(FragmentDB* frag)
		: db_(frag), keep_db_(true)
	{
		if(!db_) {
			keep_db_ = false;
			db_ = new FragmentDB("");
		}
	}

	void DNAMutator::mutate(Residue* res, const String& base) throw(Exception::InvalidOption)
	{
#warning "TODO: Add capabilities into the FragmentDB that allows the retrival of Molecule type information"
		if(base != "A" && base != "T" && base != "G" &&
			 base != "C" && base != "U" )
		{
			throw Exception::InvalidOption(__FILE__, __LINE__,
                       "Expected A, T, G, C or U, got " + base + "!");
		}

		Fragment* frag = db_->getFragmentCopy("Adenine");

		Atom* res_at = selectBaseAtoms(res);

		if(!res_at) {
			throw Exception::InvalidOption(__FILE__, __LINE__, "Could not select the base. Did you specify a valid nucleotide?");
		}

		Atom* frag_at = selectBaseAtoms(frag);

		if(!frag_at) {
			throw Exception::InvalidOption(__FILE__, __LINE__, "Could not select the space in the new nucleotide.");
		}

		for(AtomIterator it = frag->beginAtom(); +it; ++it) {
			if(!it->isSelected()) {
				frag->remove(*it);
			}
		}

		rotateBases(frag, frag_at, res, res_at);
		HINFile f("sdfsdf.hin", std::ios::out);
		System s;
		Molecule m;
		m.insert(*frag);
		s.insert(m);
		f << s;
		f.close();


	}

	Atom* DNAMutator::getAttachmentAtom(AtomContainer* res)
	{
		for(AtomIterator it = res->beginAtom(); +it;  ++it) {
			if(it->getElement().getSymbol() != "N") {
				continue;
			}


			/**
 			 * Exploit that the nitrogen connecting the sugar backbone and the
			 * base has 3 bonds to carbon atoms.
			 * This of course must be changed if there are fancier bases
			 */
			const Size num_bonds = it->countBonds();
			if(num_bonds == 3) {
				for(int i = 0; i < 3; ++i) {
					if(it->getPartnerAtom(i)->getElement().getSymbol() != "C") {
						break;
					}

					return &*it;
				}
			}
		}

		return NULL;
	}

	Atom* DNAMutator::selectBaseAtoms(AtomContainer* res)
	{
		res->deselect();
		Atom* n = getAttachmentAtom(res);

		if(!n) {
			throw Exception::InvalidOption(__FILE__, __LINE__, "Invalid residue specified");
		}

		std::list<Atom*> queue;
/*
 * The code below is safer, but much more complicated. Use this if the assumption
 * should not hold that the sugar backbone does not contain a nitrogen!
 */
#if 0
		std::list<Atom*> path;
		queue.push_front(n);

		bool found_nitro = false;

		std::set<Atom*> visited;
		while(queue.size() > 0) {
			Atom* current = queue.front();
			queue.pop_front();

			if(visited.find(current) != visited.end()) {
				continue;
			}

      // If necessary backtrack along the path
			while((path.size() > 0) && !path.front()->isBoundTo(*current)) {
				path.pop_front();
			}

			bool bound_to_nitrogen = current->isBoundTo(*n);

      /*
 			 * If 5 atoms have already been investigated and the nitrogen is not
 			 * found, we are not on the ring and should backtrack
 			 */
			if((path.size() == 5) && !bound_to_nitrogen) {
				continue;
			}

			path.push_front(current);

			if((path.size() > 2) && bound_to_nitrogen) {
				found_nitro = true;
				break;
			}

			visited.insert(current);

			int num_bonds = current->countBonds();
			for(int i = 0; i < num_bonds; ++i) {
				Atom* partner = current->getPartnerAtom(i);

				if(visited.find(partner) == visited.end()) {
					queue.push_front(partner);
				}
			}
		}

		//We should now have a path with length > 4. If not something
		//has gone wrong.
		if(path.size() <= 4) {
			return NULL;
		}

		//Now lets do a simple BFS to select the base's atoms
		queue.clear();
		queue.push_front(path.front());
#endif

		n->select();

		/*
		 * The sugar backbone should not contain a nitrogen. So lets simply
		 * select a nitrogen != n and do a BFS to select the remaining base atoms
		 */
		for(AtomIterator it = res->beginAtom(); +it; ++it) {
			if((it->getElement().getSymbol() == "N") && (&*it != n)) {
				queue.push_back(&*it);
				break;
			}
		}

		if(queue.size() == 0) {
			return NULL;
		}

		while(queue.size() > 0) {
			Atom* current = queue.front();
			queue.pop_front();

			current->select();

			int num_bonds = current->countBonds();
			for(int i = 0; i < num_bonds; ++i) {
				Atom* partner = current->getPartnerAtom(i);
				if(!partner->isSelected()) {
					queue.push_back(partner);
				}
			}
		}

		return n;
	}

	Vector3 DNAMutator::getNormalVector(Atom* at)
	{
		Vector3 dists[2];

		int i = 0;
		AtomBondIterator it = at->beginBond();

		while((i < 2) && (it != at->endBond())) {
			Atom* partner = it->getBoundAtom(*at);
			if(partner->isSelected()) {
				dists[i] = partner->getPosition() - at->getPosition();
				++i;
			}
			++it;
		}

		return (dists[0] % dists[1]).normalize();
	}

	Vector3 DNAMutator::getConnectionVector(Atom* at)
	{
		Vector3 result;
		for(AtomBondIterator it = at->beginBond(); +it; ++it) {
			Atom* partner = it->getBoundAtom(*at);
			if(!partner->isSelected()) {
				result = partner->getPosition() - at->getPosition();
			}
		}

		return result;
	}

	void DNAMutator::rotateBases(AtomContainer* from, Atom* from_at,
	                             AtomContainer* to,   Atom* to_at)
	{
		
		//First we have to align the bases with each other.
		Vector3 from_connection = getConnectionVector(from_at).normalize();
		Vector3 to_connection = getConnectionVector(to_at).normalize();

		Vector3 rot = from_connection % to_connection;

		Matrix4x4 trans;
		trans.setIdentity();
		trans.setTranslation(-from_at->getPosition());

		Matrix4x4 rotation;
		rotation.setIdentity();
		rotation.rotate(to_connection.getAngle(from_connection), rot);

		TransformationProcessor p(rotation*trans);
		from->apply(p);	

		/*
 		 * Now all that is left to do is to rotate around to_connection
		 * Here the rotation that minimizes the distance between the bases
		 * has to be chosen.
		 */
		Vector3 from_norm = getNormalVector(from_at);
		Vector3 to_norm   = getNormalVector(to_at);

		trans.setIdentity();
		trans.translate(to_at->getPosition());

		rotation.setIdentity();
		rotation.rotate(-from_norm.getAngle(to_norm), to_connection);
		
		p.setTransformation(trans*rotation);
		from->apply(p);
	}


}


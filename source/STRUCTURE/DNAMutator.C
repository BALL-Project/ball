/*
 * File: BALL/STRUCTURE/DNAMutator.C
 * Created: 23.10.2008
 * 
 * Author: Daniel Stoeckel
 */

#include <BALL/STRUCTURE/DNAMutator.h>

#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/forEach.h>

#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/STRUCTURE/RMSDMinimizer.h>

#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>

#include <list>
#include <set>
#include <limits>

#include <BALL/FORMAT/HINFile.h>

namespace BALL
{
	const char* DNAMutator::bases_[] = {
		"Adenine", "Thymine", "Guanine", "Cytosine", "Uracile"
	};

	const Size DNAMutator::default_num_steps_ = 50;

	DNAMutator::DNAMutator(EnergyMinimizer* mini, ForceField* ff, FragmentDB* frag)
		: keep_db_(true), keep_ff_(true), db_(frag), ff_(ff), minimizer_(mini), num_steps_(default_num_steps_)
	{
	}

	DNAMutator::~DNAMutator()
	{
		freeDB_();
		freeFF_();
	}

	void DNAMutator::freeDB_()
	{
		if(!keep_db_ && db_) {
			delete db_;
		}
	}

	void DNAMutator::freeFF_()
	{
		if(!keep_ff_ && ff_) {
			delete ff_;
		}
	}

	void DNAMutator::setup()
	{
		if(!db_) {
			keep_db_ = false;
			db_ = new FragmentDB("");
		}

		if(minimizer_ && !ff_) {
			keep_ff_ = false;
			ff_ = new AmberFF();
		}
	}

	void DNAMutator::setMinimizer(EnergyMinimizer* mini)
	{
		minimizer_ = mini;
	}

	void DNAMutator::setForceField(ForceField* ff)
	{
		freeFF_();
		ff_ = ff;
		keep_ff_ = true;
	}

	void DNAMutator::setFragmentDB(FragmentDB* frag)
	{
		freeDB_();
		db_ = frag;
		keep_db_ = true;
	}

	void DNAMutator::setMaxOptimizationSteps(Size steps)
	{
		num_steps_ = steps;
	}

	void DNAMutator::mutate(Fragment* res, Base base, bool optimize) throw(Exception::InvalidOption)
	{
		setup();

		Fragment* frag = db_->getFragmentCopy(bases_[static_cast<int>(base)]);

		//If we did not get a vaild fragment it is not present in the Fragment DB.
		//Time to bail out.
		if(!frag) {
			throw Exception::InvalidOption(__FILE__, __LINE__, "Could not find the specified base, please check your FragmentDB");
		}

		//Get everything needed from the input residue
		Atom* res_at = selectBaseAtoms(res);
		if(!res_at) {
			throw Exception::InvalidOption(__FILE__, __LINE__, "Could not select the base. Did you specify a valid nucleotide?");
		}

		Atom* res_connection_at = getConnectionAtom(res_at);
		if(!res_connection_at) {
			throw Exception::InvalidOption(__FILE__, __LINE__, "Could not find the C1 carbon of the specified residue");
		}

		res_at->destroyBond(*res_connection_at);
		Vector3 res_connection = res_connection_at->getPosition() - res_at->getPosition();

		//Get everything needed from the output fragment
		Atom* frag_at = selectBaseAtoms(frag);
		if(!frag_at) {
			throw Exception::InvalidOption(__FILE__, __LINE__, "Could not select the base in the new nucleotide.");
		}

		const Atom* frag_connection_at = getConnectionAtom(frag_at);
		if(!frag_connection_at) {
			throw Exception::InvalidOption(__FILE__, __LINE__, "Could not find the C1 carbon of the new base. Check your FragmentDB");
		}

		frag_at->destroyBond(*frag_connection_at);
		Vector3 frag_connection = frag_connection_at->getPosition() - frag_at->getPosition();

		//We do not need the atoms of the sugar backbone any longer, this is important
		//for the RMSDMinimizer to work

		frag->removeUnselected();

		if(isPurine(*frag_at) == isPurine(*res_at)) {
			rotateSameBases(frag, res);
		} else {
			rotateBases(frag, frag_at, res_at, frag_connection, res_connection);
		}

		//Now it is save to delete the base atoms of the input residue
		res->removeSelected();
		res->setName(frag->getName());

		res->splice(*frag);
		delete frag;

		frag_at->createBond(*res_connection_at);

		tryFlip_(res, res_connection_at->getPosition(), res_connection);

		if(minimizer_) {
			if(!optimize_(res)) {
				Log.error() << "Could not optimize the generated base. Check that your minimizer is set up correctly!\n";
			}
		}

		res->deselect();
	}

	bool DNAMutator::optimize_(Fragment* frag)
	{
		frag->select();

		ff_->setup(*frag->getAtom(0)->getMolecule()->getSystem());
		minimizer_->setup(*ff_);

		if(!minimizer_->isValid()) {
			return false;
		}

		if(!minimizer_->minimize(num_steps_)) {
			Log.warn() << "Optimization did not converge. Try a larger number of steps\n";
		}

		frag->deselect();

		return true;
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

		n->select();

		/*
		 * The sugar backbone should not contain a nitrogen. So lets simply
		 * select a nitrogen != n and do a BFS to select the remaining base atoms
		 */
		std::list<Atom*> queue;
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

	Vector3 DNAMutator::getNormalVector(const Atom* at)
	{
		Vector3 dists[2];

		int i = 0;
		AtomBondConstIterator it = at->beginBond();

		while((i < 2) && (it != at->endBond())) {
			const Atom* partner = it->getBoundAtom(*at);
			if(partner->isSelected()) {
				dists[i] = partner->getPosition() - at->getPosition();
				++i;
			}
			++it;
		}

		return (dists[0] % dists[1]).normalize();
	}

	Atom* DNAMutator::getConnectionAtom(Atom* at)
	{
		for(AtomBondIterator it = at->beginBond(); +it; ++it) {
			Atom* partner = it->getBoundAtom(*at);
			if(!partner->isSelected()) {
				return partner;
			}
		}

		return NULL;
	}

	void DNAMutator::rotateSameBases(AtomContainer* from, AtomContainer* to)
	{
		AtomBijection bij;
		bij.assignByName(*from, *to);
		Matrix4x4 trafo = RMSDMinimizer::computeTransformation(bij).first;
		TransformationProcessor tr;
		tr.setTransformation(trafo);
		from->apply(tr);
	}

	/*
	 * This function is not a member function as it is not possible to create a Matrix4x4 forward declaration
	 * It is needed as the TransformationProcessor applies its transformation to all atoms in an atom container
	 * and not only the selected ones.
	 */
	void applyTrafoToSelection_(const Matrix4x4& trafo, AtomContainer* cont)
	{
		AtomIterator it;
		BALL_FOREACH_ATOM(*cont, it) {
			if(it->isSelected()) {
				it->setPosition(trafo * it->getPosition());
			}
		}
	}

	void DNAMutator::tryFlip_(Fragment* res, const Vector3& connect_atom, const Vector3& axis) const
	{
		if(!ff_) {
			return;
		}

		ff_->setup(*res->getAtom(0)->getMolecule()->getSystem());
		double e1 = ff_->updateEnergy();

		Matrix4x4 trans_fwd = Matrix4x4::getIdentity();
		Matrix4x4 trans_bwd = Matrix4x4::getIdentity();
		Matrix4x4 rotate = Matrix4x4::getIdentity();

		rotate.rotate(Angle(180, false), axis);
		trans_fwd.translate(-connect_atom);
		trans_bwd.translate(connect_atom);

		rotate = trans_bwd * rotate * trans_fwd;

		applyTrafoToSelection_(rotate, res);

		double e2 = ff_->updateEnergy();

		Log.warn() << "Energies: " << e1 << " " << e2 << "\n";

		if(e1 < e2) {
			applyTrafoToSelection_(rotate, res);
		}

	}

	void DNAMutator::rotateBases(AtomContainer* from, const Atom* from_at, const Atom* to_at,
	                             const Vector3& from_connection, const Vector3& to_connection)
	{
		//First we have to align the bases with each other.
		Vector3 rot = from_connection % to_connection;

		Matrix4x4 trans = Matrix4x4::getIdentity();
		trans.setTranslation(-from_at->getPosition());

		Matrix4x4 rotation = Matrix4x4::getIdentity();
		rotation.rotate(to_connection.getAngle(from_connection), rot);

		TransformationProcessor p(rotation*trans);
		from->apply(p);

		/*
		 * Now all that is left to do is to rotate around to_connection
		 * Here the rotation that minimizes the distance between the bases
		 * has to be chosen.
		 */
		trans.setIdentity();
		trans.translate(to_at->getPosition());

		const Vector3 from_norm = getNormalVector(from_at);
		const Vector3 to_norm   = getNormalVector(to_at);

		/*
		 * Setup the rotation around to_connection. The problem here is, that
		 * it is unknown which vector has to be rotated, so simply try it out...
		 */
		rotation.setIdentity();
		Angle a = to_norm.getAngle(from_norm);
		rotation.rotate(a, to_connection);

		Angle rot_a_fwd = to_norm.getAngle(rotation * from_norm);

		rotation.setIdentity();
		rotation.rotate(-a, to_connection);

		Angle rot_a_bwd = to_norm.getAngle(rotation * from_norm);

		if(fabs(rot_a_fwd) < fabs(rot_a_bwd)) {
			rotation.setIdentity();
			rotation.rotate(a, to_connection);
		}

		p.setTransformation(trans*rotation);
		from->apply(p);
	}

	Vector3 DNAMutator::getOrthogonalVector(const Vector3& n, const Atom* base, const Atom* at)
	{
		Vector3 dist = at->getPosition() - base->getPosition();
		return dist - n * ((n * dist));
	}

	const Atom* DNAMutator::getSecondNitro(const std::vector<const Atom*>& ring_atoms, const Atom* base)
	{
		for(size_t i = 0; i < ring_atoms.size(); ++i) {
			if((ring_atoms[i] != base) && (ring_atoms[i]->getElement().getSymbol() == "N")) {
				return ring_atoms[i];
			}
		}

		return NULL;
	}

	bool DNAMutator::isPurine(const Atom& baseNitrogen) const
	{
		RingFinder f(5);
		return f(baseNitrogen);
	}

	bool DNAMutator::isPyrimidine(const Atom& baseNitrogen) const
	{
		RingFinder f(6);
		f(baseNitrogen);
		return f.getRingAtoms().size() == 6;
	}

}


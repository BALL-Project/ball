/*
 * File: BALL/STRUCTURE/DNAMutator.C
 * Created: 23.10.2008
 * 
 * Author: Daniel Stoeckel
 */

#include <BALL/COMMON/exception.h>
#include <BALL/STRUCTURE/DNAMutator.h>

#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomHandle.h>      // v2.2 H3c Pattern C: D-H3.8 opt-in
#include <BALL/KERNEL/handleKey.h>       // v2.2 H4 7b.15: makeHandle()
#include <BALL/KERNEL/moleculeStore.h>   // v2.2 H3d.E: Pattern C three-phase rewrite
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/forEach.h>

#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/STRUCTURE/RMSDMinimizer.h>
#include <BALL/STRUCTURE/geometricProperties.h>

#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>

#include <BALL/FORMAT/HINFile.h>

#include <algorithm>

namespace BALL
{
	const char* DNAMutator::bases_[] = {
		"Adenine", "Thymine", "Guanine", "Cytosine", "Uracile"
	};

	const Size DNAMutator::default_num_steps_ = 50;

	DNAMutator::DNAMutator(EnergyMinimizer* mini, ForceField* ff, FragmentDB* frag)
		: Mutator(frag), keep_ff_(true), ff_(ff), minimizer_(mini),
		  num_steps_(default_num_steps_), prop_(Atom::NUMBER_OF_PROPERTIES),
		  first_strand_(0), second_strand_(0), matching_mode_(MATCH_TORSION)
	{
	}

	DNAMutator::~DNAMutator()
	{
		freeFF_();
	}

	void DNAMutator::freeFF_()
	{
		if(!keep_ff_ && ff_) {
			delete ff_;
		}
	}

	void DNAMutator::setup()
	{
		Mutator::setup();

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

	void DNAMutator::setMaxOptimizationSteps(Size steps)
	{
		num_steps_ = steps;
	}

	void DNAMutator::setUsedProperty(Property p)
	{
		prop_ = p;
	}

	void DNAMutator::addMutation(Index i, const String& new_frag_name)
	{
		if(!first_strand_) {
			throw Exception::NotInitialized(__FILE__, __LINE__, "At least one DNA strand needs to be supplied");
		}

		Residue* to_mutate = first_strand_->getResidue(i);

		if(!to_mutate) {
			throw Exception::IndexOverflow(__FILE__, __LINE__, i, first_strand_->countResidues());
		}

		mutations_[to_mutate] = canonizeName_(new_frag_name);
	}

	void DNAMutator::clearMutations()
	{
		mutations_.clear();
	}

	void DNAMutator::setMatchingMode(DNAMutator::MatchingMode mmode)
	{
		matching_mode_ = mmode;
	}

	void DNAMutator::mutate_impl_(MutatorOptions)
	{
		to_optimize_.clear();

		Residue* snd = 0;
		for(MutIterator it = mutations_.begin(); it != mutations_.end(); ++it) {
			mutateSingleBase_(it->first, it->second);
			for(AtomIterator at = it->first->beginAtom(); +at; ++at) { to_optimize_.push_back(makeHandle(*at)); }
			if(second_strand_ && (snd = mapping_.firstToSecond(it->first))) {
				mutateSingleBase_(snd, getComplement_(it->second));
				for(AtomIterator at = snd->beginAtom(); +at; ++at) { to_optimize_.push_back(makeHandle(*at)); }
			}
		}
	}

	void DNAMutator::mutateSingleBase_(Residue* res, const String& basename) const
	{
		// v2.2 H3d.E Pattern C (D-H3c.1-R3): five-phase rewrite per the
		// per-sid validity table in V22-H3d-DESIGN.md R2 (D-H3d.3-R2 /
		// h3d.E). The original v0 code held raw Atom* captures across
		// multiple topology mutations (destroyBond, removeHavingProperty,
		// splice, delete frag, createBond). The rewrite captures
		// StableIds, prevalidates them once, then re-resolves to Atom*
		// at each mutation step. Stale sids skip silently if the resolve
		// returns nullptr -- the structural prevalidation guarantees
		// no skip can happen pre-Phase 2.

		// PHASE 0 (Fragment fetch + sanity checks)
		Fragment* frag = db_->getFragmentCopy(basename);

		if(!frag) {
			throw Exception::InvalidArgument(__FILE__, __LINE__, "Could not find the specified base, please check your FragmentDB.");
		}
		if(!frag->hasProperty(Nucleotide::PROPERTY__NUCLEOTIDE)) {
			throw Exception::InvalidArgument(__FILE__, __LINE__, "The specified base is not a nucleotide.");
		}

		// PHASE 1 (identify): capture sids for all 4 reference atoms +
		// the two connection vectors. NO topology mutation.
		Atom* res_at = markBaseAtoms_(res);
		if(!res_at) {
			throw Exception::InvalidArgument(__FILE__, __LINE__, "Could not select the base. Did you specify a valid nucleotide?");
		}
		Atom* res_connection_at = getConnectionAtom_(res_at);
		if(!res_connection_at) {
			throw Exception::InvalidOption(__FILE__, __LINE__, "Could not find the C1 carbon of the specified residue.");
		}
		Atom* frag_at = markBaseAtoms_(frag);
		if(!frag_at) {
			throw Exception::InvalidArgument(__FILE__, __LINE__, "Could not select the base in the new nucleotide.");
		}
		const Atom* frag_connection_at = getConnectionAtom_(frag_at);
		if(!frag_connection_at) {
			throw Exception::InvalidArgument(__FILE__, __LINE__, "Could not find the C1 carbon of the new base. Check your FragmentDB.");
		}

		// Single-store discipline: res_at + res_connection_at share a
		// store (the input residue's), frag_at + frag_connection_at
		// share a store (the fragment copy's). They may differ; that's
		// OK because the resolve happens against the matching store.
		MoleculeStore* res_store  = res_at->getStore();
		MoleculeStore* frag_store = frag_at->getStore();

		const MoleculeStore::StableId res_at_sid =
			(res_store ? res_store->stable_id(res_at->getStoreIndex()) : 0);
		const MoleculeStore::StableId res_connection_at_sid =
			(res_store ? res_store->stable_id(res_connection_at->getStoreIndex()) : 0);
		const MoleculeStore::StableId frag_at_sid =
			(frag_store ? frag_store->stable_id(frag_at->getStoreIndex()) : 0);
		const MoleculeStore::StableId frag_connection_at_sid =
			(frag_store ? frag_store->stable_id(frag_connection_at->getStoreIndex()) : 0);

		// Connection vectors are CAPTURED here (live positions). No
		// further sid resolution needed for these.
		Vector3 res_connection  = res_connection_at->getPosition() - res_at->getPosition();
		Vector3 frag_connection = frag_connection_at->getPosition() - frag_at->getPosition();

		// PHASE 1.5 (prevalidate): all 4 sids resolve. If a Pass 1.5
		// prevalidation fails the mutation hasn't started; abort cleanly.
		auto resolve_atom = [](MoleculeStore* s, MoleculeStore::StableId sid) -> Atom*
		{
			if (s == nullptr || sid == 0) return nullptr;
			MoleculeStore::Index idx = s->atom_idx_by_stable_id(sid);
			if (idx == MoleculeStore::UNKNOWN_STABLE_ID) return nullptr;
			return s->back_ptr(idx);
		};
		if (resolve_atom(res_store,  res_at_sid)              == nullptr
		 || resolve_atom(res_store,  res_connection_at_sid)   == nullptr
		 || resolve_atom(frag_store, frag_at_sid)             == nullptr
		 || resolve_atom(frag_store, frag_connection_at_sid)  == nullptr)
		{
			delete frag;
			throw Exception::InvalidArgument(__FILE__, __LINE__,
				"DNAMutator: Pass 1.5 prevalidation -- one or more reference atoms unresolvable");
		}

		// PHASE 2 (remove old bonds): destroyBond ×2. After this, all
		// 4 sids are still valid (atoms unbonded but not deleted).
		res_at->destroyBond(*res_connection_at);
		frag_at->destroyBond(*frag_connection_at);

		// PHASE 3 (geometric transforms): operate on captured Vector3
		// connection vectors. No sid resolution needed -- we still hold
		// the cached res_at / frag_at pointers from Phase 1, which are
		// guaranteed valid at this point (no atoms deleted yet).
		// (R2 validity table: res_at_sid and frag_at_sid both still
		// valid through Phase 3.)
		frag->removeNotHavingProperty(prop_);   // removes frag's sugar backbone; frag_at and frag_connection_at REMAIN (both are base atoms or frag_connection_at is on the splice path)

		if(isPurine_(*frag_at) == isPurine_(*res_at))
		{
			rotateSameBases_(frag, res);
		}
		else
		{
			alignBases_(frag, frag_connection, res_connection, frag_at);
			if(matching_mode_ == MINIMUM_ANGLE)
			{
				rotateBasesMinAngle_(frag, res_connection, frag_at, res_at);
			}
			else
			{
				rotateBasesMatchTorsion_(frag, res_connection_at, frag_at, res_at);
			}
		}

		// PHASE 4a (remove input base): res->removeHavingProperty(prop_)
		// removes the prop_-marked base atoms in `res`. res_at_sid now
		// becomes INVALID (base atom deleted). res_connection_at_sid
		// stays valid (C1 not prop_-marked).
		res->removeHavingProperty(prop_);
		res->setName(frag->getName());

		// PHASE 4b (splice): reparent frag's atoms into res. frag_at_sid
		// remains valid -- splice moves the atom but the slot identity
		// is unchanged (the atom's store binding stays put).
		static_cast<Fragment*>(res)->splice(*frag);

		// PHASE 4c (rebond): re-resolve both endpoints. Per R2 validity
		// table both should resolve (res_connection_at_sid was never
		// invalidated; frag_at_sid survived splice). If either fails,
		// we're in a partial-state world the v0 code would have also
		// hit (best-effort no-rollback contract per D-H3c.5-R2).
		Atom* res_conn_now = resolve_atom(res_store, res_connection_at_sid);
		Atom* frag_at_now  = resolve_atom(frag_store, frag_at_sid);
		if (res_conn_now == nullptr || frag_at_now == nullptr)
		{
			// Defense-in-depth: shouldn't happen per the validity table.
			delete frag;
			throw Exception::InvalidArgument(__FILE__, __LINE__,
				"DNAMutator: Phase 4c connection atom no longer resolvable -- partial mutation; tree may be in inconsistent state");
		}
		frag_at_now->createBond(*res_conn_now);

		// PHASE 5 (cleanup): delete frag. frag_connection_at_sid becomes
		// invalid here (frag's atoms freed). All bonds already touch
		// res's now-resident atoms; no surviving sid reference.
		delete frag;
	}

	void DNAMutator::mark_(AtomContainer* atoms) const
	{
		for(AtomIterator it = atoms->beginAtom(); +it; ++it) {
			it->setProperty(prop_);
		}
	}

	void DNAMutator::unmark_(AtomContainer* atoms) const
	{
		for(AtomIterator it = atoms->beginAtom(); +it; ++it) {
			it->clearProperty(prop_);
		}
	}

	bool DNAMutator::optimize()
	{
		if(to_optimize_.empty()) {
			Log.warn() << "No Atoms were specified for optimization!\n";
			return true;
		}

		Atom* front_atom = to_optimize_.front().getAtom();
		Molecule* mol = front_atom ? front_atom->getMolecule() : 0;
		if(!mol) {
			throw Exception::InvalidArgument(__FILE__, __LINE__, "The atom is not contained in a molecule. This is "
			                                                     "probably a bug in the Mutator.");
		}

		System* sys = mol->getSystem();
		if(!sys) {
			Log.error() << "In order to use a Force Field the molecule must be contained in a system. No optimization"
			               " has been performed\n";
			return false;
		}

		ff_->setup(*sys);
		minimizer_->setup(*ff_);

		Log.info() << "Setup of minimizer completed\n";

		//Select all atoms in to_optimize_
		std::for_each(to_optimize_.begin(), to_optimize_.end(), [](const AtomHandle& h) { if (Atom* a = h.getAtom()) a->select(); });

		if(!minimizer_->isValid()) {
			return false;
		}

		if(!minimizer_->minimize(num_steps_)) {
			Log.warn() << "Optimization did not converge. Try a larger number of steps\n";
		}

		//Deselect all atoms in to_optimize_
		std::for_each(to_optimize_.begin(), to_optimize_.end(), [](const AtomHandle& h) { if (Atom* a = h.getAtom()) a->deselect(); });

		return true;
	}

	void DNAMutator::setStrands(Chain* s1, Chain* s2)
	{
		first_strand_ = s1;
		second_strand_ = s2;

		if(first_strand_ != 0 && second_strand_ != 0) {
			mapping_ = NucleotideMapping::assignNaively(*first_strand_, *second_strand_);
		}
	}

	void DNAMutator::setFirstStrand(Chain* s1)
	{
		setStrands(s1, second_strand_);
	}

	void DNAMutator::setSecondStrand(Chain* s2)
	{
		setStrands(first_strand_, s2);
	}

	void DNAMutator::setNucleotideMapping(const NucleotideMapping& bij)
	{
		mapping_ = bij;
		first_strand_ = mapping_.getFirstStrand();
		second_strand_ = mapping_.getSecondStrand();
	}

	Atom* DNAMutator::getAttachmentAtom(AtomContainer* res) const
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

	Atom* DNAMutator::markBaseAtoms_(AtomContainer* res) const
	{
		unmark_(res);
		Atom* n = getAttachmentAtom(res);

		if(!n) {
			throw Exception::InvalidArgument(__FILE__, __LINE__, "Invalid residue specified");
		}

		n->setProperty(prop_);

		/*
		 * The sugar backbone should not contain a nitrogen. So lets simply
		 * mark a nitrogen != n and do a BFS to mark the remaining base atoms
		 */
		std::deque<AtomHandle> queue;
		for(AtomIterator it = res->beginAtom(); +it; ++it) {
			if((it->getElement().getSymbol() == "N") && (&*it != n)) {
				queue.push_back(makeHandle(*it));
				break;
			}
		}

		if(queue.empty()) {
			return NULL;
		}

		while(!queue.empty()) {
			Atom* current = queue.front().getAtom();
			queue.pop_front();
			if(!current) continue;

			current->setProperty(prop_);

			int num_bonds = current->countBonds();
			for(int i = 0; i < num_bonds; ++i) {
				Atom* partner = current->getPartnerAtom(i);
				if(!partner->hasProperty(prop_)) {
					queue.push_back(makeHandle(*partner));
				}
			}
		}

		return n;
	}

	Vector3 DNAMutator::getNormalVector_(const Atom* at) const
	{
		Vector3 dists[2];

		int i = 0;
		AtomBondConstIterator it = at->beginBond();

		while((i < 2) && (it != at->endBond())) {
			const Atom* partner = it->getBoundAtom(*at);
			if(partner->hasProperty(prop_)) {
				dists[i] = partner->getPosition() - at->getPosition();
				++i;
			}
			++it;
		}

		return (dists[0] % dists[1]).normalize();
	}

	Atom* DNAMutator::getConnectionAtom_(Atom* at) const
	{
		for(AtomBondIterator it = at->beginBond(); +it; ++it) {
			Atom* partner = it->getBoundAtom(*at);
			if(!partner->hasProperty(prop_)) {
				return partner;
			}
		}

		return NULL;
	}

	void DNAMutator::rotateSameBases_(AtomContainer* from, AtomContainer* to) const
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
	 * and not only the marked ones.
	 */
	void applyTrafoToList_(const Matrix4x4& trafo, const std::deque<AtomHandle>& atoms)
	{
		for(std::deque<AtomHandle>::const_iterator it = atoms.begin(); it != atoms.end(); ++it) {
			if(Atom* a = it->getAtom()) a->setPosition(trafo * a->getPosition());
		}
	}

	void DNAMutator::alignBases_(AtomContainer* from, const Vector3& from_connection, const Vector3& to_connection, Atom* from_at) const
	{
		//First we have to align the bases with each other.
		Vector3 rot = from_connection % to_connection;

		Matrix4x4 trans = Matrix4x4::getIdentity();
		trans.setTranslation(-from_at->getPosition());

		Matrix4x4 rotation = Matrix4x4::getIdentity();
		rotation.rotate(to_connection.getAngle(from_connection), rot);

		TransformationProcessor p(rotation*trans);
		from->apply(p);
	}

	const Atom* DNAMutator::getTorsionDefiningAtom_(const Atom* atom) const
	{
		//Use a const cast to avoid reimplementing the exact same code as for the
		//non const version.
		return getTorsionDefiningAtom_(const_cast<Atom*>(atom));
	}

	Atom* DNAMutator::getTorsionDefiningAtom_(Atom* atom) const
	{
		const unsigned int neighbour_count = 3;

		for(AtomBondIterator it = atom->beginBond(); +it; ++it)
		{
			Atom* partner = it->getPartner(*atom);
			unsigned int atom_count = 0;

			for(AtomBondIterator at = partner->beginBond(); +at; ++at)
			{
				if(at->getPartner(*partner)->getElement().getAtomicNumber() != 1)
				{
					++atom_count;
				}
			}
			if(partner->hasProperty(prop_) && (atom_count == neighbour_count))
			{
				return partner;
			}
		}

		return 0;
	}


	void DNAMutator::rotateBasesMatchTorsion_(AtomContainer* from, const Atom* to_connection_at, Atom* from_at, const Atom* to_at) const
	{
		//Move the base to its destination
		Matrix4x4 trans = Matrix4x4::getIdentity();
		trans.translate(to_at->getPosition());

		TransformationProcessor p(trans);
		from->apply(p);

		const Atom* sugar_o_atom = NULL;

		for(AtomBondConstIterator it = to_connection_at->beginBond(); +it; ++it)
		{
			const Atom* partner = it->getPartner(*to_connection_at);
			//If the partner is an oxygen
			if(partner->getElement().getAtomicNumber() == 8)
			{
				sugar_o_atom = partner;
				break;
			}
		}

		if(!sugar_o_atom) {
			throw Exception::InvalidArgument(__FILE__, __LINE__, "Could not find oxygen sugar atom. Bailing out.");
		}

		Atom* from_torsion_defining_atom = getTorsionDefiningAtom_(from_at);
		const Atom* to_torsion_defining_atom = getTorsionDefiningAtom_(to_at);

		Angle torsion = calculateTorsionAngle(*sugar_o_atom, *to_connection_at, *to_at, *to_torsion_defining_atom);
		setTorsionAngle(*sugar_o_atom, *to_connection_at, *from_at, *from_torsion_defining_atom, torsion);
	}

	void DNAMutator::rotateBasesMinAngle_(AtomContainer* from, const Vector3& to_connection, Atom* from_at, const Atom* to_at) const
	{
		/*
		 * Now all that is left to do is to rotate around to_connection
		 * Here the rotation that minimizes the distance between the bases
		 * has to be chosen.
		 */
		Matrix4x4 trans = Matrix4x4::getIdentity();
		Matrix4x4 rotation = Matrix4x4::getIdentity();
		trans.translate(to_at->getPosition());

		const Vector3 from_norm = getNormalVector_(from_at);
		const Vector3 to_norm   = getNormalVector_(to_at);

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

		TransformationProcessor p;
		p.setTransformation(trans*rotation);
		from->apply(p);
	}

	bool DNAMutator::isPurine_(const Atom& baseNitrogen) const
	{
		RingFinder f(5);
		return f(baseNitrogen);
	}

	bool DNAMutator::isPyrimidine_(const Atom& baseNitrogen) const
	{
		RingFinder f(6);
		f(baseNitrogen);
		return f.getRingAtoms().size() == 6;
	}

	String DNAMutator::canonizeName_(const String& frag_name) const
	{
		unsigned int num_bases = 5;
		const String bases[] = {"Adenine", "Thymine", "Guanine", "Cytosine", "Uracil"};

		for(unsigned int i = 0; i < num_bases; ++i)
		{
			if(bases[i].hasPrefix(frag_name))
			{
				return bases[i];
			}
		}

		throw Exception::InvalidArgument(__FILE__, __LINE__, "The passed string does not match any known base. Check for case sensitivity");
	}

	String DNAMutator::getComplement_(const String& s) const
	{
		if(s == "Adenine") {
			return "Thymine";
		}
		if(s == "Thymine") {
			return "Adenine";
		}
		if(s == "Cytosine") {
			return "Guanine";
		}
		if(s == "Guanine") {
			return "Cytosine";
		}
		if(s == "Uracil") {
			return "Adenine";
		}

		throw Exception::InvalidArgument(__FILE__, __LINE__, String("Unknown base: ") + s);
	}

}


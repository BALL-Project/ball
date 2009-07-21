// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ballAndStickModel.C,v 1.23.18.1 2007/03/25 22:02:29 oliver Exp $
//

#include <BALL/VIEW/MODELS/ballAndStickModel.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/disc.h>
#include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>
#include <BALL/VIEW/KERNEL/common.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		AddBallAndStickModel::AddBallAndStickModel()
			: AtomBondModelBaseProcessor(),
				ball_radius_((float)0.4),
				stick_radius_((float)0.2),
				ball_and_stick_(true),
				dashed_bonds_(false)
		{
		}

		AddBallAndStickModel::AddBallAndStickModel(const AddBallAndStickModel &add_ball_and_stick)
			: AtomBondModelBaseProcessor(add_ball_and_stick),
				ball_radius_(add_ball_and_stick.ball_radius_),
				stick_radius_(add_ball_and_stick.stick_radius_),
				ball_and_stick_(add_ball_and_stick.ball_and_stick_),
				dashed_bonds_(add_ball_and_stick.dashed_bonds_)
		{
		}

		AddBallAndStickModel::~AddBallAndStickModel()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.info() << "Destructing object " << this << " of class AddBallAndStickModel" << std::endl;
			#endif 
		}

		void AddBallAndStickModel::clear()
		{
			AtomBondModelBaseProcessor::clear();

			ball_radius_ = (float)0.4;
			stick_radius_ = (float)0.2;
			ball_and_stick_ = true;
			dashed_bonds_ = false;
		}

		void AddBallAndStickModel::set(const AddBallAndStickModel &add_ball_and_stick)
		{
			AtomBondModelBaseProcessor::set(add_ball_and_stick);

			ball_radius_ = add_ball_and_stick.ball_radius_;
			stick_radius_ = add_ball_and_stick.stick_radius_;
			ball_and_stick_ = add_ball_and_stick.ball_and_stick_;
			dashed_bonds_ = add_ball_and_stick.dashed_bonds_;
		}

		const AddBallAndStickModel &AddBallAndStickModel::operator = (const AddBallAndStickModel &processor)
		{
			set(processor);
			return *this;
		}

		void AddBallAndStickModel::setBallRadius(const float radius)
			throw(Exception::OutOfRange)
		{
			// radius can never be lower or equal 0
			if (radius <= (float)0)
			{
				throw Exception::OutOfRange(__FILE__, __LINE__);
			}
			
			ball_radius_ = radius;
		}

		void AddBallAndStickModel::setStickRadius(const float radius)
			throw(Exception::OutOfRange)
		{
			// radius can never be lower or equal 0
			if (radius <= (float)0)
			{
				throw Exception::OutOfRange(__FILE__, __LINE__);
			}

			stick_radius_ = radius;
			special_radius_ = radius / 2.4;
		}

		Processor::Result AddBallAndStickModel::operator() (Composite& composite)
		{
			if (dashed_bonds_)
			{
				AtomBondModelBaseProcessor::operator() (composite);
			}

			Atom *atom = RTTI::castTo<Atom>(composite);

			if (atom == 0) return Processor::CONTINUE;

			Sphere* sphere_ptr = new Sphere;

			if (sphere_ptr == 0) throw Exception::OutOfMemory (__FILE__, __LINE__, sizeof(Sphere));

			sphere_ptr->setComposite(atom);

			if (ball_and_stick_)
			{
				sphere_ptr->setRadius(ball_radius_);
			}
			else
			{
				sphere_ptr->setRadius(stick_radius_);
			}

			sphere_ptr->setPositionAddress(atom->getPosition());
			
			// append sphere in Atom
			geometric_objects_.push_back(sphere_ptr);

			// collect used atoms
			insertAtom_(atom);

			return Processor::CONTINUE;
		}

		void AddBallAndStickModel::dump(std::ostream& s, Size depth) const
		{
			BALL_DUMP_STREAM_PREFIX(s);
			
			BALL_DUMP_DEPTH(s, depth);
			BALL_DUMP_HEADER(s, this, this);

			AtomBondModelBaseProcessor::dump(s, depth + 1);

			BALL_DUMP_DEPTH(s, depth);
			s << "ball radius: " << ball_radius_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "stick radius: " << stick_radius_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "b&s model: " << ball_and_stick_ << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "dashed bonds: " << dashed_bonds_ << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		void AddBallAndStickModel::renderStandardBond_(const Bond& bond)
		{
			// generate two colored tube
			TwoColoredTube *tube = new TwoColoredTube;
			tube->setRadius(stick_radius_);
			tube->setComposite(&bond);
			geometric_objects_.push_back(tube);

			if (!ball_and_stick_)
			{
				tube->setVertex1Address(bond.getFirstAtom()->getPosition());
				tube->setVertex2Address(bond.getSecondAtom()->getPosition());
			}
			else
			{
				const Vector3& v1 = bond.getFirstAtom()->getPosition();
				const Vector3& v2 = bond.getSecondAtom()->getPosition();
				Vector3 dir = v2 - v1;
				float f = dir.getLength();
				if (!Maths::isZero(f)) dir /= f;
 				dir *= (ball_radius_ - 0.4 * stick_radius_);
				tube->setVertex1(v1 + dir);
				tube->setVertex2(v2 - dir);
			}
		}

		void AddBallAndStickModel::renderMultipleBond_(const Bond& bond, Vector3 normal, Vector3 dir)
		{
			TwoColoredTube* tube = new TwoColoredTube;
			tube->setRadius(special_radius_);
			tube->setComposite(&bond);
			geometric_objects_.push_back(tube);
				
			TwoColoredTube* tube2 = new TwoColoredTube(*tube);
			geometric_objects_.push_back(tube2);

			if (bond.getOrder() == Bond::ORDER__DOUBLE)
			{
				normal *= stick_radius_ / (float) 1.5;
				tube->setVertex1(bond.getFirstAtom()->getPosition() - normal);
				tube->setVertex2(bond.getSecondAtom()->getPosition() - normal);

				tube2->setVertex1(bond.getFirstAtom()->getPosition() + normal);
				tube2->setVertex2(bond.getSecondAtom()->getPosition() + normal);
			}
			else
			{
				Vector3 normal2 = dir % normal;
				try
				{
					normal2.normalize();
				}
				catch(...)
				{
					return;
				}

				normal *= stick_radius_ / (float) 1.5;
				normal2 *= stick_radius_ / (float) 1.5;
				
				tube->setVertex1(bond.getFirstAtom()->getPosition() - normal - normal2);
				tube->setVertex2(bond.getSecondAtom()->getPosition() - normal - normal2);
				
				tube2->setVertex1(bond.getFirstAtom()->getPosition() + normal - normal2);
				tube2->setVertex2(bond.getSecondAtom()->getPosition() + normal - normal2);

				TwoColoredTube* tube3 = new TwoColoredTube(*tube);
				tube3->setVertex1(bond.getFirstAtom()->getPosition() + normal2);
				tube3->setVertex2(bond.getSecondAtom()->getPosition() + normal2);
				geometric_objects_.push_back(tube3);
			}
		}

		void AddBallAndStickModel::visualiseBond_(const Bond& bond)
		{
			// no visualisation for hydrogen bonds
			if (bond.getType() == Bond::TYPE__HYDROGEN) return;

			if (!dashed_bonds_ 													||
					bond.getOrder() < Bond::ORDER__DOUBLE 	||
					bond.getOrder() > Bond::ORDER__AROMATIC )
			{
				renderStandardBond_(bond);
				return;
			}

			try
			{
				Vector3 dir = bond.getSecondAtom()->getPosition() - bond.getFirstAtom()->getPosition();
				dir.normalize();
				Vector3 normal = VIEW::getNormal(dir);

				if (bond.getFirstAtom()->countBonds() == 3)
				{
					normal = getSP2Plane_(*bond.getFirstAtom(), bond, dir);
				}
				else if(bond.getSecondAtom()->countBonds() == 3)
				{
					normal = getSP2Plane_(*bond.getSecondAtom(), bond, dir);
				}

				if (bond.getOrder() == Bond::ORDER__AROMATIC)
				{
					renderDashedBond_(bond, normal);
				}
				else
				{
					renderMultipleBond_(bond, normal, dir);
				}
			}
			catch(...)
			{
			}
		}

		Vector3 AddBallAndStickModel::getSP2Plane_(const Atom& atom, const Bond& bond, const Vector3& dir) const
		{
			vector<Vector3> vs;
			AtomBondConstIterator bit = atom.beginBond();
			for (; +bit; ++bit)
			{
				if (&*bit != &bond)
				{
					Vector3 x = bit->getPartner(atom)->getPosition() - atom.getPosition();
					float xl = x.getLength();
					if (!Maths::isZero(xl)) vs.push_back(x / xl);
					else 									 vs.push_back(Vector3(1, 0, 0));
				}
			}

			Vector3 n = vs[0] % vs[1];
			float nl = n.getLength();
			if (!Maths::isZero(nl)) n /= nl;
			else 									  n = Vector3(1,0,0);

			n = n % dir;
			nl = n.getLength();
			if (!Maths::isZero(nl)) n /= nl;
			else 									  n = Vector3(1,0,0);

			return n;
		}


		void AddBallAndStickModel::visualiseRings_()
		{
			for (Position r = 0; r < rings_.size(); r++)
			{
				vector<Atom*>& ring = rings_[r];

				vector<Bond*>& bonds = ring_bonds_[r];
				if (bonds.size() == 0) continue;

				vector<Atom*>::iterator ait = ring.begin();

				Vector3 center;
				for (; ait != ring.end(); ait++)
				{
					center += (**ait).getPosition();
				}
				center /= ring.size();


				vector<Vector3> bond_vectors;
				for (Position p = 0; p < bonds.size(); p++)
				{
					Vector3 v1 = bonds[p]->getFirstAtom()->getPosition() - 
											 bonds[p]->getSecondAtom()->getPosition();
					float f = v1.getLength();
					if (Maths::isZero(f)) bond_vectors.push_back(Vector3(1,0,0));
					else 									bond_vectors.push_back(v1 / f);
				}
				
				bond_vectors.push_back(bond_vectors[0]);

				Vector3 rn1 = bond_vectors[0] % bond_vectors[1];
				float f = rn1.getLength();
				if (Maths::isZero(f)) rn1 = Vector3(1,0,0);
				else 								  rn1 / f;

				Vector3 rn = rn1;

				for (Position p = 2; p < bond_vectors.size(); p++)
				{
					Vector3 rnx = bond_vectors[p - 1] % bond_vectors[p];
					float f = rnx.getLength();
					if (Maths::isZero(f)) rnx = Vector3(1,0,0);
					else 								  rnx / f;

					if (rn1.getSquareDistance(rnx) > rn1.getSquareDistance(-rnx)) rnx *= -1;

					rn += rnx;
				}

				if (!Maths::isZero(rn.getSquareLength())) rn.normalize();

				for (Position b = 0; b < bonds.size(); b++)
				{
					Vector3 n = rn % bond_vectors[b];
					float f = n.getLength();
					if (Maths::isZero(f)) n = rn;
					else 									n /= f;

					Vector3 p = bonds[b]->getFirstAtom()->getPosition() + bond_vectors[b] / 2.0;
					Vector3 pc = p - center;

					if (n.getSquareDistance(pc) < n.getSquareDistance(-pc)) n *= -1;

					if (bonds[b]->getOrder() == Bond::ORDER__AROMATIC)
					{
						renderDashedBond_(*bonds[b], n);
					}
					else
					{
						try
						{
							renderMultipleBond_(*bonds[b], n, bond_vectors[b]);
						}
						catch(...)
						{
						}
					}
				}
			}
		}

		void AddBallAndStickModel::renderDashedBond_(const Bond& bond, Vector3 n)
			throw(Exception::DivisionByZero)
		{
			const Atom& a1 = *bond.getFirstAtom();
			const Atom& a2 = *bond.getSecondAtom();

 			n *= stick_radius_ / (float) 1.5;

			// render one tube with full length			
			TwoColoredTube *tube = new TwoColoredTube;
			tube->setRadius(special_radius_);
			tube->setVertex1(a1.getPosition() - n);
			tube->setVertex2(a2.getPosition() - n);
			tube->setComposite(&bond);
			geometric_objects_.push_back(tube);

			// render dashed tubes
 			Vector3 v = a2.getPosition() + n - (a1.getPosition() + n);
			Vector3 v8 = v / 8.;
			Vector3 last = a1.getPosition() + n + v / (float) 4.5;
			for (Position p = 0; p < 3; p++)
			{
				if (p == 1)
				{
					Vector3 middle((a1.getPosition() - a2.getPosition()) / 2 + a2.getPosition());
					TwoColoredTube *tube = new TwoColoredTube;
					tube->setRadius(special_radius_);
					tube->setComposite(&bond);
					tube->setVertex1(middle - (v8) + n);
					tube->setVertex2(middle + (v8) + n);
					geometric_objects_.push_back(tube);

					Disc* disc = new Disc(Circle3(middle - (v8) + n, -v, stick_radius_ / (float) 2.4));
					disc->setComposite(&a1);
					geometric_objects_.push_back(disc);

					disc = new Disc(Circle3(middle + (v8) + n, v, stick_radius_ / (float) 2.4));
					disc->setComposite(&a2);
					geometric_objects_.push_back(disc);

					last += (v /4);

					continue;
				}

				last += (v /4);
			}
		}

		void AddBallAndStickModel::collectRingBonds_()
		{
			vector<HashSet<Atom*> > ring_atom_set;
			for (Position i = 0; i < rings_.size(); i++)
			{
				ring_atom_set.push_back(HashSet<Atom*>());
				for (Position j = 0; j < rings_[i].size(); j++)
				{
					ring_atom_set[i].insert(rings_[i][j]);
				}
			}

			ring_bonds_.clear();
			ring_bonds_.resize(rings_.size());

			const Atom* second_atom_ptr = 0;
			AtomBondConstIterator bond_it;

			// for all used atoms
			list<const Atom*>::const_iterator atom_it;
			for (atom_it  = getAtomList_().begin();
					 atom_it != getAtomList_().end(); ++atom_it)
			{
				// for all bonds connected from first- to second atom
				BALL_FOREACH_ATOM_BOND(**atom_it, bond_it)
				{
					second_atom_ptr = bond_it->getPartner(**atom_it);
					
					// no visualisation for hydrogen bonds
					if (bond_it->getType() == Bond::TYPE__HYDROGEN) continue;

					// use only atoms with greater handles than first atom
					if (**atom_it > *second_atom_ptr || !getAtomSet_().has(second_atom_ptr))
					{
						continue;
					}

					bool special_ring = false;

					if (dashed_bonds_ &&
							bond_it->getOrder() > Bond::ORDER__SINGLE)
					{
						for (Position p = 0; p < ring_atom_set.size(); p++)
						{
							if (ring_atom_set[p].has((Atom*)*atom_it) &&
									ring_atom_set[p].has((Atom*)second_atom_ptr))
							{
								special_ring = true;
								ring_bonds_[p].push_back((Bond*)&*bond_it);
								break;
							}
						}
					}

					if (!special_ring)
					{
 						visualiseBond_(*bond_it);
					}
				}
			}
		}


		bool AddBallAndStickModel::createGeometricObjects()
		{
			if (!dashed_bonds_) 
			{
				buildBondModels_();
				return true;
			}

			collectRingBonds_();
			visualiseRings_();
			rings_.clear();
			ring_bonds_.clear();
			return true;
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/MODELS/ballAndStickModel.iC>
#		endif

	} // namespace VIEW
} // namespace BALL

// $Id: fresnoRingStacking.h,v 1.1.2.2 2004/06/12 15:15:32 anker Exp $
// Molecular Mechanics: Fresno force field, CH---pi interaction

#ifndef BALL_MOLMEC_FRESNO_RINGSTACKING_H
#define BALL_MOLMEC_FRESNO_RINGSTACKING_H

#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>


using namespace std;
namespace BALL
{

	/** Fresno ringstacking component.
			{\bf Definition:} \URL{BALL/MOLMEC/FRESNO/fresnoRingStacking.h}
	*/

  class FresnoRingStacking
		:	public ForceFieldComponent
	{	
		public:
      
			/// A class for storing aromatic rings and information associated with
			/// them. At the moment we only need the ring centres and the atoms.
			class AromaticRing
			{
				public:

					///
					AromaticRing()
						throw();

					///
					AromaticRing(const AromaticRing& aromatic_ring)
						throw();

					///
					AromaticRing(const std::vector<const Atom*>& atoms)
						throw();

					///
					const Vector3& getCentre() const
						throw();

					///
					const Vector3& getNormalVector() const
						throw();

					///
					void setRing(const std::vector<const Atom*>& atoms)
						throw();

					// accessors for the private data members
					const std::vector<const Atom*>& getRing() const 
						throw();

					///
					void dump(std::ostream& s = std::cout) const
						throw();

				private:

					//_ A hash set containing pointers to all ring atoms:
					std::vector<const Atom*> ring_atoms_;

					//_ The centre of the aromatic ring.
					Vector3 centre_;

					//_ The normal vector of the ring plane.
					Vector3 normal_vector_;

					//_
					void computeCentre_()
						throw();

					//_
					void computeNormalVector_()
						throw();

			};

  
			class CHGroup
			{
				public: 

					///
					CHGroup()
						throw();

					///
					~CHGroup()
						throw();

					///
					CHGroup(const CHGroup& CH_groups)
						throw();

					///
					CHGroup (const Atom* C_atom, const Atom* H_atom)
						throw();

					///
					void setCAtom(const Atom* C_atom)
						throw();

					///
					void setHAtom(const Atom* H_atom)
						throw();

					///
					void setAtoms(const Atom* C_atom, const Atom* H_atom)
						throw();

					///
					const Atom* getHAtom() const 
						throw()
					{ 
						return(H_atom_);
					}

					///
					const Atom* getCAtom() const 
						throw()
					{
						return(C_atom_);
					} 

					void dump(std::ostream s = std::cout) const
						throw();

				private :

					//_
					const Atom* H_atom_;

					//_
					const Atom* C_atom_;

			};


			struct Option
			{

				///
				static const String VERBOSITY;

				///
				static const String CX_DISTANCE_UPPER;

				///
				static const String CHX_ANGLE_LOWER;

				///
				static const String HX_PROJECTED_DISTANCE_LOWER;

				///
				static const String HX_PROJECTED_DISTANCE_UPPER;

			};


			struct Default
			{

				///
				static const Size VERBOSITY;

				///
				static const float CX_DISTANCE_UPPER;

				///
				static const float CHX_ANGLE_LOWER;

				///
				static const float HX_PROJECTED_DISTANCE_LOWER;

				///
				static const float HX_PROJECTED_DISTANCE_UPPER;

			};


			/** @name	Constructors and Destructors	
			 */
			//@{ 

			/**	Default constructor.
			 */
			FresnoRingStacking()
				throw();

			/**	Constructor.
			 */
			FresnoRingStacking(ForceField& force_field)
				throw();

			/**	Copy constructor
			 */
			FresnoRingStacking(const FresnoRingStacking& frs)
				throw();

			/**	Destructor.
			 */
			virtual ~FresnoRingStacking()
				throw();

			//@}
			/**	@name	Assignment
			 */
			//@{

			/** Assignment.
			 */
			const FresnoRingStacking& operator = (const FresnoRingStacking& frs)
				throw();

			/** Clear method.
			 */
			virtual void clear()
				throw();

			//@}
			/**	@name	Predicates.
			 */
			//@{

			bool operator == (const FresnoRingStacking& frs) const
				throw();

			//@}
			/**	@name	Setup Methods	
			 */
			//@{

			/**	Setup method.
			 */
			virtual bool setup()
				throw();

			virtual double updateEnergy()
				throw();

			virtual void dump(std::ostream s = std::cout) const
				throw();

			///
			Options options;

		private:

			//_ A vector containing all possible interactions between aliphatic
			//_ C-H groups in the sugar and aromatic rings in the receptor.
			std::vector< std::pair<const AromaticRing*, const CHGroup*> > possible_interactions_;

			//_ A vector containing all aliphatic C-H groups of the ligand.
			std::vector<CHGroup> all_CH_groups_;

			//_ A vector containing all aromatic rings of the receptor.
			std::vector<AromaticRing> all_aromatic_rings_;

			//_
			float CX_distance_upper_;

			//_
			float CHX_angle_lower_;

			//_
			float HX_projected_distance_lower_;

			//_
			float HX_projected_distance_upper_;

	};


  
} // namespace BALL

#endif // BALL_MOLMEC_FRESNO_RINGSTACKING_H

// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Jan Fuhrmann, Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_COMPONENTS_PLP_H
#define BALL_SCORING_COMPONENTS_PLP_H

#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/MOLMEC/MMFF94/MMFF94Parameters.h>


namespace BALL
{
	/** Piecewise Linear Potential */
	class BALL_EXPORT PLP : public ForceFieldComponent
	{
		public:
			struct NonBondedPairData
			{
				Size type;

				enum Type
				{
					steric = 0,
					hydrogen_bond = 1,
					// an atom pair of which both partners are taken from the same ligand molecule in order to evaluate the ligand nonbonded energy
					conformation = 2
				};

				/*
				NonBondedPairData();

				double eij;
				double rij;
				double rij_7;
				double vdw_energy; // for debugging
				// for electrostatics:
				bool 	 is_1_4;
				double qi;
				double qj;
				double es_energy;  // for debugging
				*/
			};

			/// flag to enable ES
			#define MMFF94_ES_ENABLED "enable ES"

			/// flag to enable VDW
			#define MMFF94_VDW_ENABLED "enable VDW"

			BALL_CREATE(PLP)

			///	Default constructor.
			PLP() throw();

			///	Constructor.
			PLP(ForceField& force_field) throw();

			///	Copy constructor
			PLP(const PLP& MMFF94_non_bonded) throw();

			///	Destructor.
			virtual ~PLP() throw();

			/// Assignment operator
			const PLP& operator = (const PLP& anb) throw();

			/** Set the rotatable bonds. This is necessary in order to be able to compute GHTorsion-Energies.\n
			Note that setting the rotatable bonds must of course be done anew for each new ligand !! */
			void setRotatableBonds(std::vector<Bond*>& bonds);

			/** Returns the Gehlhaar Atom-Type (0, 1, 2 or 3) for the given atom.\n
			0 : steric \n
			1 : donor \n
			2 : acceptor \n
			3 : both \n
			4 : metal */
			static Size getAtomType(const Atom* atom);

			/** Update this component using only the given atom-pairs */
			void update(const std::vector<std::pair<Atom*, Atom*> >& atom_vector);

			/// Clear method
			virtual void clear() throw();

			/// Equality operator
			bool operator == (const PLP& anb) throw();

			/** Setup this component according to the given options. */
			bool setup(Options& options);

			///	Calculates and returns the component's energy.
			virtual double updateEnergy() throw();

			///	Calculates and returns the component's forces.
			virtual void updateForces() throw();

			/**	Update the pair list.
			This method is called by the force field whenever
			\link ForceField::update ForceField::update \endlink  is called. It is used
			to recalculate the nonbonded pair list. */
			virtual void update()
				throw(Exception::TooManyErrors);

			double getVDWEnergy() const;

			double getESEnergy() const;

		protected:
			//_	Value of the electrostatic energy
			double	es_energy_;

			//_	Value of the vdw energy
			double	vdw_energy_;

			static bool isNSp3(const Atom* at);
			static bool isSp3(const Atom* at);

		private:
			struct GhTorsion
			{
				Atom* at1;
				Atom* at2;
				Atom* at3;
				Atom* at4;
				Size type;
			};

			void e1(double& d, double& e, double& delta);
			void e2(double& d, double& e, double& delta);

			std::vector<GhTorsion> ghtorsions_;

			ForceField::PairVector atom_pair_vector_;

			std::vector<NonBondedPairData> non_bonded_data_;

			double cut_off_;
			double vdw_cut_on_, vdw_cut_off_;
			double es_cut_on_, es_cut_off_;
			MMFF94VDWParameters vdw_parameters_;

			// dielectric constant
			double dc_;
			// dielectric model exponent
			double n_;
			bool es_enabled_;
			bool vdw_enabled_;
			bool enable_es_switch_;
			bool enable_vdw_switch_;
			double 	es_d_on2_, es_d_off2_,
			es_d_on_, es_d_off_,
			es_ac_, es_bc_,
			es_cc_, es_dc_, es_denom_, es_con_, es_cover3_, es_dover5_,
			es_eadd_, es_eaddr_, es_const_, es_constr_;

			std::vector<bool> dismiss_vector_;

			std::vector<double> rotgrad_;
	};
} //

#endif // BALL_SCORING_COMPONENTS_PLP_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94StretchBend.h,v 1.1.8.1 2007/03/25 21:25:18 oliver Exp $
//

#ifndef BALL_MOLMEC_MMFF94_MMFF94STRETCHBEND_H
#define BALL_MOLMEC_MMFF94_MMFF94STRETCHBEND_H

#ifndef BALL_MOLMEC_COMMON_FORCEFIELDCOMPONENT_H
#	include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#endif

#ifndef BALL_MOLMEC_MMFF94_MMFF94PARAMETERS_H
#	include <BALL/MOLMEC/MMFF94/MMFF94Parameters.h>
#endif

namespace BALL 
{
	class MMFF94;

	/**	MMFF94 bond stretch component
    	\ingroup  MMFF94
	*/
	class BALL_EXPORT MMFF94StretchBend 
		: public ForceFieldComponent
	{
		public:

		struct BALL_EXPORT StretchBend
		{
			double kba_ijk;
			double kba_kji;
			double energy; // debug
			Index sbtijk; 
			Index stretch_i_j, stretch_j_k;
			Index bend_index;
		};

		/// see MMFFANG.PAR
		struct BALL_EXPORT Bend
		{
			Bend();

			double theta0;
			double delta_theta;
			double theta;
			double ka;
			Atom*	atom1;
			Atom*	atom2;
			Atom*	atom3;
			bool is_linear;
			Position ATIJK;
			double energy;  // debug
			bool emperical; // debug
			TVector3<double> n1, n2;
		};
		
		///
		struct BALL_EXPORT Stretch
		{
			Stretch()
			  : atom1(NULL),
			    atom2(NULL),
			    kb(0.0),
			    r0(0.0),
			    delta_r(0.0),
			    sbmb(false),
			    emperical(false)
			{
			}

			Atom* atom1;
			Atom* atom2;
			double kb;
			double r0;
			double delta_r;
			bool  sbmb; 
			bool  emperical;
			Vector3 n;
		};

		/// flag to enable BENDS
		#define MMFF94_BENDS_ENABLED "enable Bends"
		
		/// flag to enable Stretches
		#define MMFF94_STRETCHES_ENABLED "enable Stretches"

		/// flag to enable StretchBends
		#define MMFF94_STRETCHBENDS_ENABLED "enable StrechBends"

		BALL_CREATE(MMFF94StretchBend)

		/**	Default constructor.
		*/
		MMFF94StretchBend();

		/**	Constructor.
		*/
		MMFF94StretchBend(ForceField& force_field);

		/**	Copy constructor
		*/
		MMFF94StretchBend(const MMFF94StretchBend& to_copy);

		/**	Destructor.
		*/
		virtual ~MMFF94StretchBend();

		/**	Setup method.
		*/
		virtual bool setup()
			throw(Exception::TooManyErrors);

		///	Calculates and returns the component's energy.
		virtual double updateEnergy();

		///
		virtual double updateBendEnergy();
		
		///
		virtual double updateStretchEnergy();

		///
		virtual double updateStretchBendEnergy();

		///
		double getStretchEnergy() const;

		///
		double getBendEnergy() const;

		///
		double getStretchBendEnergy() const;

		/// Calculates and returns the component's forces.
		virtual void updateForces();

		///
		virtual void updateBendForces();

		///
		virtual void updateStretchForces();

		///
		virtual void updateStretchBendForces();

		///
		const vector<Bend>& getBends() const { return bends_;}
		
		///
		const vector<Stretch>& getStretches() const { return stretches_;}

		///
		const vector<StretchBend>& getStretchBends() const { return stretch_bends_;}
		
		///
		Index calculateSBTIJK(Position angle_type, 
													bool bond_type1,
													bool bond_type2);
		
		///
		Position getBendType(const Bond& bond1, const Bond& bond2,
										 		 Atom& atom1, Atom& atom2, Atom& atom3) const;
		
		/// Bend emperical reference angle
		double calculateBendEmpiricalReferenceAngle(Atom& atom1, Atom& atom2, Atom& atom3) const;
		
		/// Bend emperical force constant
		double calculateBendEmpiricalForceConstant(Atom& atom1, Atom& atom2, Atom& atom3, double angle_0) const;

		/// Calculate the radius value per Schomaker-Stevenson Rule
		double calculateStretchR0(const Bond& bond);

		///
		double calculateStretchConstant(const Bond& bond, double r0);

		private:

		inline void AddDV3_(Vector3& f3, const TVector3<double> d3);
		void calculateDeltas_();
		bool setupBends_();
		bool setupStretches_();
		bool setupStretchBends_();
		
		void errorOccured_(const String& string, 
											 const Atom& a1, const Atom& a2, const Atom& a3);

		vector<Bend> bends_;
		vector<Stretch> stretches_;
		vector<StretchBend> stretch_bends_;

		const MMFF94StretchParameters* stretch_parameters_;
		MMFF94BendParameters bend_parameters_;
 		MMFF94StretchBendParameters sb_parameters_;

		static double bend_z_[];
		static double bend_c_[];
		static String bend_elements_[];

		double stretch_energy_;
		double bend_energy_;
		double stretch_bend_energy_;

		MMFF94* mmff94_;
		bool stretch_enabled_, bend_enabled_, stretchbend_enabled_;
	};
} // namespace BALL

#endif // BALL_MOLMEC_MMFF94_MMFF94STRETCHBEND_H

// $Id: fresno.h,v 1.1.2.20 2005/01/30 14:06:40 anker Exp $

#ifndef BALL_MOLMEC_FRESNO_FRESNO_H
#define BALL_MOLMEC_FRESNO_FRESNO_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
#	include <BALL/MOLMEC/COMMON/forceField.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
#endif

namespace BALL 
{
	/** Fresno force field class.
			This force field implements the function developed by Rognan et al.
			(J. Med. Chem. 42:4650-4658, 1999). \\
			{\bf Definition:} \URL{BALL/MOLMEC/FRESNO/fresno.h}
	*/
	class FresnoFF
		: public ForceField
	{

		public:

		/** Define the interface for the base function
		*/
		class BaseFunction
		{

			public:

			///
			BaseFunction()
				throw();

			///
			BaseFunction(float lower, float upper)
				throw();

			///
			virtual void setLower(float lower)
				throw();

			///
			virtual void setUpper(float upper)
				throw();

			///
			virtual float calculate(float x) const
				throw();

			///
			virtual float calculate(float x, float lower, float upper) 
				throw();


			protected:

			//_
			float lower_;

			//_
			float upper_;

		};

		/** Linear base function
		*/
		class BaseFunctionLinear : public BaseFunction
		{

			public:

			///
			BaseFunctionLinear()
				throw();

			///
			BaseFunctionLinear(float lower, float upper)
				throw();

			///
			float calculate(float x) const
				throw();

			///
			float calculate(float x, float lower, float upper)
				throw();

		};

		/** Sigmoidal base function
		*/
		class BaseFunctionSigmoidal: public BaseFunction
		{

			public:

			/// 
			BaseFunctionSigmoidal()
				throw();

			/// 
			BaseFunctionSigmoidal(float lower, float upper)
				throw();

			///
			void setLower(float lower)
				throw();

			///
			void setUpper(float upper)
				throw();

			///
			float calculate(float x) const
				throw();

			///
			float calculate(float x, float lower, float upper)
				throw();


			private:

			//_
			void computeSigmoidParameters_()
				throw();

			//_
			float a_;

			//_
			float b_;

		};

		/**	@name Constant Definitions
		*/
		//@{

		/**	Option names
		*/
		struct Option
		{
			/**	The additive constant (@see Default::CONST)
			*/
			static const char* CONST;

			/**
			*/
			static const char* HB;

			/**
			*/
			static const char* LIPO;

			/**
			*/
			static const char* ROT;

			/**
			*/
			static const char* BP;

			/**
			*/
			static const char* DESOLV;

			/**
			*/
			static const char* METAL;

			/**
			*/
			static const char* NONPOLAR;

			/**
			*/
			static const char* HB_IDEAL_LENGTH;

			/**
			*/
			static const char* HB_IDEAL_ANGLE;

			/**
			*/
			static const char* HB_DIST_LOWER;

			/**
			*/
			static const char* HB_DIST_UPPER;

			/**
			*/
			static const char* HB_ANG_LOWER;

			/**
			*/
			static const char* HB_ANG_UPPER;

			/**
			*/
			static const char* LIPO_R1_OFFSET;

			/**
			*/
			static const char* LIPO_R2_OFFSET;

			/**
			*/
			static const char* BP_R1_OFFSET;

			/**
			*/
			static const char* BP_R2_OFFSET;

			/**
			*/
			static const char* ROT_BIND_OFFSET;

			/**
			*/
			static const char* ROT_GRID_SPACING;

			/**
			*/
			static const char* ROT_ALGORITHM;

			/**
			*/
			static const char* ROT_METHOD;

			/**
			*/
			static const char* METAL_R1;

			/**
			*/
			static const char* METAL_R2;

			/**
			*/
			static const char* DESOLV_METHOD;

			/**
			*/
			static const char* DESOLV_GB;

			/**
			*/
			static const char* DESOLV_AVG;

			/**
			*/
			static const char* DESOLV_FOCUS_GRID_AROUND_LIGAND;

			/**
			*/
			static const char* PROBE_RADIUS;

			/**
			*/
			static const char* SURFACE_TENSION;

			/**
			*/
			static const char* UHLIG_CONSTANT;

			/**
			*/
			static const char* SOLVENT_NUMBER_DENSITY;

			/**
			*/
			static const char* ABSOLUTE_TEMPERATURE;

			/**
			*/
			static const char* NONPOLAR_METHOD;

			/**
			*/
			static const char* VERBOSITY;

			/**
			*/
			static const char* SOLVENT_DESCRIPTOR_FILE;

			/**
			*/
			static const char* LJ_PARAM_FILE;

			/**
			*/
			static const char* ATOM_TYPE_FILE;

			/**
			*/
			static const char* BASE_FUNCTION_TYPE;

			/**
			*/
			static const char* GB_SCALING_FILE;


		};

		/** Default values for FRESNO options.
		*/
		struct Default
		{
			/**	The additive constant (@see Default::CONST)
			*/
			static const float CONST;

			/**
			*/
			static const float HB;

			/**
			*/
			static const float LIPO;

			/**
			*/
			static const float ROT;

			/**
			*/
			static const float BP;

			/**
			*/
			static const float DESOLV;

			/**
			*/
			static const float NONPOLAR;

			/**
			*/
			static const float METAL;

			/**
			*/
			static const float HB_IDEAL_LENGTH;

			/**
			*/
			static const float HB_IDEAL_ANGLE;

			/**
			*/
			static const float HB_DIST_LOWER;

			/**
			*/
			static const float HB_DIST_UPPER;

			/**
			*/
			static const float HB_ANG_LOWER;

			/**
			*/
			static const float HB_ANG_UPPER;

			/**
			*/
			static const float LIPO_R1_OFFSET;

			/**
			*/
			static const float LIPO_R2_OFFSET;

			/**
			*/
			static const float BP_R1_OFFSET;

			/**
			*/
			static const float BP_R2_OFFSET;

			/**
			*/
			static const float ROT_BIND_OFFSET;

			/**
			*/
			static const float ROT_GRID_SPACING;

			/**
			*/
			static const Size ROT_ALGORITHM;

			/**
			*/
			static const Size ROT_METHOD;

			/**
			*/
			static const float METAL_R1;

			/**
			*/
			static const float METAL_R2;

			/**
			*/
			static const Size DESOLV_METHOD;

			/**
			*/
			static const bool DESOLV_GB;

			/**
			*/
			static const Size DESOLV_AVG;

			/**
			*/
			static const bool DESOLV_FOCUS_GRID_AROUND_LIGAND;

			/**
			*/
			static const float PROBE_RADIUS;

			/**
			*/
			static const float SURFACE_TENSION;

			/**
			*/
			static const float UHLIG_CONSTANT;

			/**
			*/
			static const float SOLVENT_NUMBER_DENSITY;

			/**
			*/
			static const float ABSOLUTE_TEMPERATURE;

			/**
			*/
			static const Size NONPOLAR_METHOD;

			/**
			*/
			static const Size VERBOSITY;

			/**
			*/
			static const String SOLVENT_DESCRIPTOR_FILE;

			/**
			*/
			static const String LJ_PARAM_FILE;

			/**
			*/
			static const String ATOM_TYPE_FILE;

			/**
			*/
			static const Size BASE_FUNCTION_TYPE;

			/**
			*/
			static const String GB_SCALING_FILE;

		};


		/**
		*/
		enum BaseFunctionType
		{
			/**
			*/
			BASE_FUNCTION_TYPE__LINEAR,

			/**
			 */
			BASE_FUNCTION_TYPE__SIGMOIDAL
		};


		/**
		*/
		enum TypeAssignment
		{
			/**
			*/
			ASSIGNMENT__ELDRIDGE,

			/**
			 */
			ASSIGNMENT__FRESNO
		};

		/**
		*/
		enum FresnoType
		{
			/** Unknown type.
			*/
			UNKNOWN,

			/** Lipophilic atom.
					Defined as:
					\begin{itemize}
						\item chlorine, bromine and iodine atoms which are not ions
						\item sulphurs which are not acceptor or polar
						\item carbons which are not polar
					\end{itemize}
			*/
			LIPOPHILIC,

			/** H-bond donor atom.
					Defined as:
					\begin{itemize}
						\item nitrogens with hydrogens attached
						\item hydrogens attached to oxygon or nitrogen
					\end{itemize}
			*/
			HBOND_DONOR,

			/** H-bond donor/acceptor atom.
					Defined as:
					\begin{itemize}
						\item oxygens attached to hydrogen atoms
						\item imine nitrogen (i. e. C=NH nitrogen)
					\end{itemize}
			*/
			HBOND_ACCEPTOR_DONOR,

			/** H-bond acceptor atom.
					Defined as:
					\begin{itemize}
						\item oxygenz not attached to hydrogen
						\item nitrogens with no hydrogens attached and one ore two bonds
						\item halogens which are ions
						\item sulphurs wiht only one connection
					\end{itemize}
			*/
			HBOND_ACCEPTOR,

			/** H-bond hydrogen.
			*/
			HBOND_HYDROGEN,

			/** Polar (non H-bonding) atoms.
					Defined as:
					\begin{itemize}
						\item nitrogens with no hydrogens attached and more than two bonds
						\item phosphorus
						\item sulphurs attached to one or more polar atoms (including
						H-bonding atoms and excluding polar carbon atoms or flourine
						atoms)
						\item carbons attached to two or more polar atoms (including
						H-bonding atoms and excluding polar carbon atoms or flourine
						atoms)
						\item carbons in nitriles or carbonyls
						\item nitrogens with no hydrogens and four binds
						\item flourine atoms
					\end{itemize}
			*/
			POLAR,

			/** Metal atoms.
			*/
			METAL
		};

		//@}
		/** @name	Constructors and Destructors
		*/
		//@{

		BALL_CREATE(FresnoFF)

		/**	Default constructor.
		*/
		FresnoFF()
			throw();

		/** Construct a FresnoFF with a system.
		*/
		FresnoFF(System& system)
			throw();

		/** Construct a FresnoFF with a system and pointers to protein and
				ligand
		*/
		FresnoFF(System& system, Molecule* protein, Molecule* ligand)
			throw();

		/** Construct a FresnoFF with a system and options.
		*/
		FresnoFF(System& system, const Options& options)
			throw();

		/** Construct a FresnoFF with a system and options.
		*/
		FresnoFF(System& system, Molecule* protein, Molecule* ligand,
				const Options& options)
			throw();

		/**	Copy constructor
		*/
		FresnoFF(const FresnoFF& ff)
			throw();

		/** Destructor
		*/
		virtual ~FresnoFF()
			throw();

		//@}
		/**	@name Assignment
		*/
		//@{

		/**	Assignment operator.
		*/
		const FresnoFF& operator = (const FresnoFF& ff)
			throw();

		/** Clear method.
		*/
		virtual void clear()
			throw();

		//@}
		/**	@name	Setup Methods
		*/
		//@{

		/**	Force field specific setup
		*/
		virtual bool specificSetup()
			throw();

		//@}
		/**	@name Accessors specific to the FRESNO force field
		*/
		//@{

		/**
		*/
		void setProtein(Molecule* protein)
			throw();

		/**
		*/
		void setLigand(Molecule* ligand)
			throw();

		/**
		*/
		Molecule* getProtein() const
			throw();

		/**
		*/
		Molecule* getLigand() const
			throw();

		/**
		*/
		double getHydrogenBondEnergy() const
			throw();

		/**
		*/
		double getLipophilicEnergy() const
			throw();

		/**
		*/
		double getRotationalEnergy() const
			throw();

		/**
		*/
		double getBuriedPolarEnergy() const
			throw();

		/**
		*/
		double getDesolvationEnergy() const
			throw();

		/**
		*/
		double getNonPolarEnergy() const
			throw();

		/**
		*/
		const HashMap<const Atom*, short>& getFresnoTypes() const
			throw();

		/**
		*/
		String getFresnoTypeString(short type) const
			throw();

		/**
		*/
		String getFresnoTypeString(const Atom* atom) const
			throw();

		/** The base funtion (sigmoidal or linear?)
		*/
		BaseFunction* base_function;


		protected:

		/** @name Protected Attributes
		*/
		//@{

		/** We need a definite assignment discriminating receptor and ligand.
		*/
		Molecule* protein_;
		Molecule* ligand_;

		/** A hash map mapping atom pointers to fresno types.
		*/
		HashMap<const Atom*, short> fresno_types_;

		/**
		*/
		Size verbosity_;

		//@}

		private:

		/*_
		*/
		void registerComponents_()
			throw();

	};

} // namespace BALL

#endif // BALL_MOLMEC_FRESNO_FRESNO_H

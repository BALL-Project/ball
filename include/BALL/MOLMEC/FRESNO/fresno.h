// $Id: fresno.h,v 1.1.2.2 2002/02/14 19:03:29 anker Exp $

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

		/**	@name Constant Definitions
		*/
		//@{

		/**	Option names
		*/
		struct Option
		{
			/**	The parameter file name (@see Default::FILENAME)
			*/
			static const char* FILENAME;

			/**	automatically assign charges to the system (during setup)
			*/
			static const char* ASSIGN_CHARGES;

			/**	automatically assign type names to the system (during setup)
			*/
			static const char* ASSIGN_TYPENAMES;

			/**	automatically assign types to the system's atoms (during setup)
			*/
			static const char* ASSIGN_TYPES;

			/**	during charge assignment, overwrite even non-zero charges
			*/
			static const char* OVERWRITE_CHARGES;

			/**	during charge assignment, overwrite even non-empty type names
			*/
			static const char* OVERWRITE_TYPENAMES;
		};

		/** Default values for FRESNO options.
		*/
		struct Default
		{
			/**	Default filename for the parameter file (@see Option::FILENAME).
			*/
			static const char* FILENAME;

			/**	automatically assign charges to the system (during setup)
			*/
			static const bool ASSIGN_CHARGES;

			/**	automatically assign type names to the system (during setup)
			*/
			static const bool ASSIGN_TYPENAMES;

			/**	automatically assign types to the system's atoms (during setup)
			*/
			static const bool ASSIGN_TYPES;

			/**	during charge assignment, overwrite even non-zero charges
			*/
			static const bool OVERWRITE_CHARGES;

			/**	during charge assignment, overwrite even non-empty type names
			*/
			static const bool OVERWRITE_TYPENAMES;
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

		/** Construct a FresnoFF with a system and options.
		*/
		FresnoFF(System& system, const Options& options)
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
			throw(Exception::FileNotFound);

		//@}
		/**	@name Accessors specific to the FRESNO force field
		*/
		//@{

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
		const HashMap<const Atom*, short>& getFresnoTypes() const
			throw();

		//@}


		protected:

		/** @name Protected Attributes
		*/
		//@{

		/*_ The filename of the parameter file
		*/
		String	filename_;

		/*_ Are all parameters initialized?
		*/
		bool parameters_initialized_;

		/** A hash map mapping atom pointers to fresno types.
		*/
		HashMap<const Atom*, short> fresno_types_;

		//@}

		private:

		void registerComponents_()
			throw();

	};

} // namespace BALL

#endif // BALL_MOLMEC_FRESNO_FRESNO_H

// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Slick-development Team, Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_SCORING_COMPONENTS_FRESNOTYPES_H
#define BALL_SCORING_COMPONENTS_FRESNOTYPES_H

#include <BALL/SCORING/COMMON/scoringComponent.h>
#include <BALL/KERNEL/system.h>


namespace BALL
{
	class BALL_EXPORT FresnoTypes
	{
		public:
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
							\item imine nitrogen (i. e. C = NH nitrogen)
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

			FresnoTypes(AtomContainer* system);
			virtual ~FresnoTypes() {}

			virtual bool setup();

			const HashMap<const Atom*, Size>* getTypeMap() const;

		private:
			AtomContainer* container_;

			//_ The hash map containing the mapping of atom pointers to atom types
			HashMap<const Atom*, Size> type_map_;
	};
}


#endif // BALL_SCORING_COMPONENTS_FRESNOTYPES_H

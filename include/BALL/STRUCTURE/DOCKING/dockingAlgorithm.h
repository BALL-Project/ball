// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:


#ifndef BALL_STRUCTURE_DOCKING_DOCKINGALGORITHM_H
#define BALL_STRUCTURE_DOCKING_DOCKINGALGORITHM_H

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_STRUCTURE_DOCKING_RANKEDCONFORMATIONS_H
# include <BALL/STRUCTURE/DOCKING/rankedConformations.h>
#endif

namespace BALL
{
	/** A class defining the interface for docking algorithms.
		This is a first iteration.
	*/
	class DockingAlgorithm
	{
		public:

			virtual ~DockingAlgorithm() {};

			/** Attributes
			*/
			Options options;


			/** Operations
			*/

			/** Full setup.
			 *  Assigns systems 1 and two and the options for this DockingAlgorithm.
			 */
			virtual void setup(System& system1, System& system2, Options& new_options)
				throw();

			/** Alternative setup.
			 *  Assigns systems 1 and two without changing the options for this DockingAlgorithm.
			 */
			virtual void setup(System& system1, System& system2)
				throw();

			/**
			*/
			virtual void start()
				throw();

			/**
			*/
			virtual void pause()
				throw();

			/**
			*/
			virtual void proceed()
				throw();

			/**
			*/
			virtual void abort()
				throw();

			/**
			*/
			virtual bool hasFinished() const
				throw();

			/**
			*/
			virtual float getProgress() const
				throw();

			/** Return total_conformations putative complexes, ordered
			 *  according to their rank.
			 *  If total_conformations == 0, the option TOP_N is used
			 */
			virtual RankedConformations getRankedConformations(Index total_conformations = 0)
				throw();

		protected:
			System system1_;
			System system2_;
	};

} // namespace BALL

#endif

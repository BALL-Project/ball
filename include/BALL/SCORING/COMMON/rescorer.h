// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $id$

#ifndef BALL_SCORING_COMMON_RESCORER_H
#define BALL_SCORING_COMMON_RESCORER_H

#ifndef BALL_COMMON_H
#       include <BALL/common.h>
#endif

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/SCORING/FUNCTIONS/rescoring.h>
#include <BALL/DOCKING/COMMON/structurePreparer.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/DATATYPE/string.h>

namespace BALL
{

	/** Class that performs rescoring. Taken originally from CADDSuite.
	 */
	class BALL_EXPORT Rescorer
	{

		public:
			// assignment operator
			Rescorer& operator = (const Rescorer& rescorer);

			// runs a rescoring
			static int runRescoring(CommandlineParser& par, bool simple_rescoring, bool train);

		private:

			static void processMultiMoleculeFile(ScoringFunction* scoring_function, StructurePreparer* sp, String par_file, Rescoring* rescoring, bool train, double min_dock_score, String dock_score_label, GenericMolFile* input, GenericMolFile* output, double output_score_threshold, bool ignore_top);

	};

}

#endif // BALL_SCORING_COMMON_RESCORER_H

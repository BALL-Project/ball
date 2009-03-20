// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: fieldLineCreator.h,v 1.1.4.2 2007/04/11 11:55:51 amoll Exp $
//

#ifndef BALL_VIEW_MODELS_FIELDLINE_CREATOR_H
#define BALL_VIEW_MODELS_FIELDLINE_CREATOR_H

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef  BALL_VIEW_KERNEL_REPRESENTATION_H
# include <BALL/VIEW/KERNEL/representation.h>
#endif

namespace BALL
{
	class AtomContainer;

	namespace VIEW
	{

		/** FieldLineCreator class.
				\ingroup  ViewModels
		*/
		class BALL_VIEW_EXPORT FieldLineCreator
		{
			public:

			typedef TRegularData3D<Vector3> VectorGrid;

			BALL_CREATE(FieldLineCreator)

			/** Default Constructor.
			*/
			FieldLineCreator();

			/** Copy constructor.
			*/
			FieldLineCreator(const FieldLineCreator& fc);

			/** Destructor.
			*/
			virtual ~FieldLineCreator();

			Representation* createLines();

			void createFieldLine_(const Vector3& point, Representation& rep);

			inline void calculateLinePoints_(Vector3 point, vector<Vector3>& points, float factor = 1.);

			/** For the Stalling approach: use the potential or the vector field as basis for the placement
			*/
			void setUsePotentialGrid(bool state) { use_potential_grid_ = state;}

			float tolerance_;
			Size max_steps_;
			Size interpolation_steps_;
			Size icosaeder_steps_;
			float atom_distance_;
			Size field_line_errors_;
			bool use_atoms_;
			Size monte_carlo_nr_lines_;
			RegularData3D* potential_grid_;
			VectorGrid*    vector_grid_;
			AtomContainer* 	ac_;
			bool 						use_potential_grid_;
		};

	} // namespace VIEW
} // namspace BALL

#endif // BALL_VIEW_MODELS_FIELDLINE_CREATOR_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glBoundingBoxModel.h,v 1.1.2.1 2003/01/07 13:23:14 anker Exp $
// molview tutorial example
// ------------------------
// create a processor that computes the bounding box of a molecular structure

// BALL includes
#include <BALL/MATHS/box3.h>

// BALLVIEW includes
#include <BALL/VIEW/GUI/PRIMITIV/glsimpleBox.h>
#include <BALL/MOLVIEW/FUNCTOR/baseModel.h>

namespace BALL
{

	namespace MOLVIEW
	{

		class GLBoundingBoxModel
			: public BaseModelProcessor
		{
			public:

				// constructors and destructor
				GLBoundingBoxModel() 
					throw();
				virtual ~GLBoundingBoxModel()
					throw();

				// accessors
				void setColor(const ColorRGBA &color) 
					throw();

				// processor related methods
				virtual bool start()
					throw();
				virtual bool finish()
					throw();
				virtual Processor::Result operator() (Composite& composite)
					throw();

			private:
				ColorRGBA color_;
				bool new_start_;
				Composite* start_composite_;
				Box3 bbox_;
		};
	
	}

}

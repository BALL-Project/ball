// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: representation.h,v 1.1.4.1 2003/02/05 15:31:34 anker Exp $

#ifndef BALL_MOLVIEW_KERNEL_REPRESENTATION_H
#define BALL_MOLVIEW_KERNEL_REPRESENTATION_H

#ifndef BALL_VIEW_GEOMETRIC_OBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_BASEMODEL_H
#	include <BALL/MOLVIEW/FUNCTOR/baseModel.h>
#endif

//using BALL::VIEW::SelectionMessage;

namespace BALL
{
	namespace MOLVIEW
	{
		/** Representation
		*/
		class Representation
		{
			public:

			/**	@name	Constructors and Destuctor
			*/	
			//@{

			/** Default Constructor
			*/
			Representation()
				throw();

			/** Copy constructor
			*/
			Representation(const Representation& rp)
				throw();

			/** Destructor
			*/
			virtual ~Representation()
				throw();

			//@}
			
			void clear()
				throw();

			void setVisible(bool visible = true)
				throw() { visible_ = visible; };

			bool isVisible() 
				throw() { return visible_; };
		
			void setModelProcessor(BaseModelProcessor* processor)
				throw() { model_processor_ = processor; };

			BaseModelProcessor* getBaseModelProcessor()
				throw() { return model_processor_; };

			List<GeometricObject*>& getGeometricObjects()
				throw() { return geometic_objects_;};

			void clearGeometricObjects()
				throw();

			private:

			bool visible_;

			bool enable_dynamic_modell_;

			BaseModelProcessor* model_processor_;

			List<GeometricObject*> geometic_objects_;

			HashSet<Composite*> composites_;

		};

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_KERNEL_REPRESENTATION_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_MODELS_LABELMODEL_H
#define BALL_VIEW_MODELS_LABELMODEL_H

#ifndef BALL_VIEW_MODELS_MODELPROCESSOR_H
# include <BALL/VIEW/MODELS/modelProcessor.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLORRGBA_H
# include <BALL/VIEW/DATATYPE/colorRGBA.h>
#endif

#ifndef BALL_STRUCTURE_GEOMETRICPROPERTIES_H
# include <BALL/STRUCTURE/geometricProperties.h>
#endif


#include <QtGui/QFont>

namespace BALL
{
	namespace VIEW
	{

		/** LabelModel class.
				The LabelModel is a model processor that is responsible
				for creating and updateing Labels.
				It supports special text tags, that allows for automaticaly
				created labels, see the documentation for the Label class.
				@see Label
				\ingroup  ViewModels
		*/
		class BALL_VIEW_EXPORT LabelModel: public ModelProcessor
		{
			public:

			BALL_CREATE(LabelModel)

			///
			enum Mode
			{
				///
				ALL_ITEMS,
				///
				ALL_ATOMS,
				///
				ALL_RESIDUES,
				///
				ONE_LABEL
			};


			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
			*/
			LabelModel();

			/** Copy constructor.
			*/
			LabelModel(const LabelModel& model);

			/** Destructor.
			*/
			virtual ~LabelModel();

			///
			void setFont(const QFont& font) { font_ = font;} 

			///
			const QFont& getFont() const { return font_;}
			
			///
			void setColor(const ColorRGBA& color) { color_ = color;}

			///
			const ColorRGBA& getColor() const { return color_;}

			///
			void setMode(Mode mode) { mode_ = mode;}

			///
			Mode getType() const { return mode_;}

			///
			void setText(const String& text) { text_ = text;}
			
			///
			const String& getText() const { return text_;}


			//@} 
			/**	@name Processor specific methods 
			*/ 
			//@{

			/**	Operator method.
			*/
			virtual Processor::Result operator() (Composite& composite);
						
			///
			virtual bool createGeometricObjects();
			//@}

			protected:

			QFont 										font_;
			ColorRGBA 								color_;
			Mode 											mode_;
			String 										text_;
			Size 											nr_objects_;
			Vector3 									center_;
			GeometricCenterProcessor 	center_processor_;
		};

	} // namespace VIEW
} // namspace BALL

#endif // BALL_VIEW_MODELS_LabelMODEL_H

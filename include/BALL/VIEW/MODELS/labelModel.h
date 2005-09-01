// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: labelModel.h,v 1.1.2.4 2005/09/01 22:17:57 amoll Exp $
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


#include <qfont.h>

namespace BALL
{
	namespace VIEW
	{

		/** LabelModel class.
				The class LabelModel is a model processor that is responsible
				for creating and updateing Labels.
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
			LabelModel()
				throw();

			/** Copy constructor.
			*/
			LabelModel(const LabelModel& model)
				throw();

			/** Destructor.
			*/
			virtual ~LabelModel()
				throw();

			///
			void setFont(const QFont& font) throw() { font_ = font;} 

			///
			const QFont& getFont() const throw() { return font_;}
			
			///
			void setColor(const ColorRGBA& color) throw() { color_ = color;}

			///
			const ColorRGBA& getColor() const throw() { return color_;}

			///
			void setMode(Mode mode) throw() { mode_ = mode;}

			///
			Mode getType() const throw() { return mode_;}

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
			virtual bool createGeometricObjects() 
				throw();
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

// $Id: moleculeObjectProcessor.C,v 1.1 2000/09/23 15:39:14 hekl Exp $

#include <BALL/MOLVIEW/GUI/FUNCTOR/moleculeObjectProcessor.h>

using namespace std;

namespace BALL
{

	namespace MOLVIEW
	{

		MoleculeObjectProcessor::MoleculeObjectProcessor()
			:	ObjectProcessor(),
				fragmentdb(),
				model_connector_()
		{
			color_calculator_ = &element_color_calculator_;
		}

		MoleculeObjectProcessor::MoleculeObjectProcessor
			(const MoleculeObjectProcessor& object_processor, bool deep)
			:	ObjectProcessor(object_processor, deep),
				fragmentdb(),
				model_connector_(object_processor.model_connector_)
		{
			color_calculator_ = &element_color_calculator_;
		}

		MoleculeObjectProcessor::~MoleculeObjectProcessor()
		{
			#ifdef BALL_MOLVIEW_DEBUG
				cout << "Destructing object " << (void *)this 
					<< " of class " << RTTI::getName<MoleculeObjectProcessor>() << endl;
			#endif 

			destroy();
		}

	  void MoleculeObjectProcessor::applyOn(Composite &composite)
    {

// ------------------------------------------------------------------------------
// models -----------------------------------------------------------------------
// ------------------------------------------------------------------------------

			AddGLBallAndStickModel ball_and_stick_model;
			AddGLLineModel line_model;
			AddGLSurfaceModel surface_model;
			AddGLVanDerWaalsModel van_der_waals_model;
			RemoveModel remove_model;
			GeometricObjectSelector selector;			

			ball_and_stick_model.registerModelConnector(model_connector_);
			line_model.registerModelConnector(model_connector_);
			van_der_waals_model.registerModelConnector(model_connector_);
		
// ------------------------------------------------------------------------------
// RTTI -------------------------------------------------------------------------
// ------------------------------------------------------------------------------

			BaseModelProcessor *static_base_model_pointer = NULL;
			BaseModelProcessor *dynamic_base_model_pointer = NULL;

// -----------------------------------------------------------------------------
// selects and setups the static model processor -------------------------------
// -----------------------------------------------------------------------------

			switch (ObjectProcessor::getValue(ADDRESS__STATIC_MODEL))
			{
			  case VALUE__MODEL_LINES:
					static_base_model_pointer = (BaseModelProcessor *)&line_model;
					break;

			  case VALUE__MODEL_STICK:
					{
						AddGLBallAndStickModel *model_pointer = &ball_and_stick_model;
						static_base_model_pointer = (BaseModelProcessor *)model_pointer;
						model_pointer->enableStickModel();
					}
					break;
				
			  case VALUE__MODEL_BALL_AND_STICK:
					{
						AddGLBallAndStickModel *model_pointer = &ball_and_stick_model;
						static_base_model_pointer = (BaseModelProcessor *)model_pointer;
						model_pointer->enableBallAndStickModel();
					}
					break;
				
  			case VALUE__MODEL_SURFACE:
					static_base_model_pointer = (BaseModelProcessor *)&surface_model;
					break;

  			case VALUE__MODEL_VAN_DER_WAALS:
					static_base_model_pointer = (BaseModelProcessor *)&van_der_waals_model;
					break;

  			case VALUE__MODEL_REMOVE:
					static_base_model_pointer = (BaseModelProcessor *)&remove_model;
					break;

  			case VALUE__SELECT:
					selector.useSelectionColor();
					static_base_model_pointer = (BaseModelProcessor *)&selector;
					break;

  			case VALUE__DESELECT:
					selector.useObjectColor();
					static_base_model_pointer = (BaseModelProcessor *)&selector;
					break;

			  default:
					{
						AddGLBallAndStickModel *model_pointer = &ball_and_stick_model;
						static_base_model_pointer = (BaseModelProcessor *)model_pointer;
						model_pointer->enableStickModel();
					}
					break;
			}

			// ------------------------------------------
			// setup drawing precision ------------------
			// ------------------------------------------
			
			switch (ObjectProcessor::getValue(ADDRESS__STATIC_DRAWING_PRECISION))
			{
			  case VALUE__DRAWING_PRECISION_LOW:
					static_base_model_pointer
						->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_LOW);
					break;
					
			  case VALUE__DRAWING_PRECISION_MEDIUM:
					static_base_model_pointer
						->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_MEDIUM);
					break;

			  case VALUE__DRAWING_PRECISION_HIGH:
					static_base_model_pointer
						->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH);
					break;
					
			  case VALUE__DRAWING_PRECISION_ULTRA:
					static_base_model_pointer
						->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_ULTRA);
					break;
					
			  default:
					static_base_model_pointer
						->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH);
					break;
			}
			
			
			// ------------------------------------------
			// setup drawing mode -----------------------
			// ------------------------------------------
			
			switch (ObjectProcessor::getValue(ADDRESS__STATIC_DRAWING_MODE))
			{
			  case VALUE__DRAWING_MODE_DOTS:
					static_base_model_pointer
						->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_MODE_DOTS);
					break;
					
			  case VALUE__DRAWING_MODE_WIREFRAME:
					static_base_model_pointer
						->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME);
					break;
					
			  case VALUE__DRAWING_MODE_SOLID:
					static_base_model_pointer
						->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_MODE_SOLID);
					break;
					
			  default:
					static_base_model_pointer
						->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_MODE_SOLID);
					break;
			}
			
			// switch to dynamic model
			static_base_model_pointer
				->setProperty(VIEW::GeometricObject::PROPERTY__OBJECT_STATIC);
			static_base_model_pointer
				->clearProperty(VIEW::GeometricObject::PROPERTY__OBJECT_DYNAMIC);

// ------------------------------------------------------------------------------
// register the color calculator ------------------------------------------------
// ------------------------------------------------------------------------------
			
			static_base_model_pointer->registerColorCalculator(*color_calculator_);

// ------------------------------------------------------------------------------
// applies the processors to the composite --------------------------------------
// ------------------------------------------------------------------------------

			remove_model.setProperty(VIEW::GeometricObject::PROPERTY__OBJECT_STATIC);

			if (ObjectProcessor::getValue(ADDRESS__STATIC_MODEL) == VALUE__MODEL_REMOVE)
			{
				applyOnComposite_(composite, &remove_model);
			}
			else
			{
				//				if (ObjectProcessor::getValue(ADDRESS__STATIC_MODEL) != VALUE__SELECT
				//						&& ObjectProcessor::getValue(ADDRESS__STATIC_MODEL) != VALUE__DESELECT)
				//				{
				//					applyOnComposite_(composite, &remove_model);
				//				}

				applyOnComposite_(composite, static_base_model_pointer);
			}

			
// -----------------------------------------------------------------------------
// selects and setups the dynamic model processor ------------------------------
// -----------------------------------------------------------------------------

			switch (ObjectProcessor::getValue(ADDRESS__DYNAMIC_MODEL))
			{
			  case VALUE__MODEL_LINES:
					dynamic_base_model_pointer = (BaseModelProcessor *)&line_model;
					break;
					
			  case VALUE__MODEL_STICK:
					{
						AddGLBallAndStickModel *model_pointer = &ball_and_stick_model;
						dynamic_base_model_pointer = (BaseModelProcessor *)model_pointer;
						model_pointer->enableStickModel();
					}
					break;
					
			  case VALUE__MODEL_BALL_AND_STICK:
					{
						AddGLBallAndStickModel *model_pointer = &ball_and_stick_model;
						dynamic_base_model_pointer = (BaseModelProcessor *)model_pointer;
						model_pointer->enableBallAndStickModel();
					}
					break;
					
			  case VALUE__MODEL_SURFACE:
					dynamic_base_model_pointer = (BaseModelProcessor *)&surface_model;
					break;
					
			  case VALUE__MODEL_VAN_DER_WAALS:
					dynamic_base_model_pointer = (BaseModelProcessor *)&van_der_waals_model;
					break;
					
  			case VALUE__MODEL_REMOVE:
					dynamic_base_model_pointer = (BaseModelProcessor *)&remove_model;
					break;

  			case VALUE__SELECT:
					selector.useSelectionColor();
					dynamic_base_model_pointer = (BaseModelProcessor *)&selector;
					break;

  			case VALUE__DESELECT:
					selector.useObjectColor();
					dynamic_base_model_pointer = (BaseModelProcessor *)&selector;
					break;

			  default:
					{
						AddGLLineModel *model_pointer = &line_model;
						dynamic_base_model_pointer = (BaseModelProcessor *)model_pointer;
					}
					break;
			}


			// switch to dynamic model
			dynamic_base_model_pointer
				->setProperty(VIEW::GeometricObject::PROPERTY__OBJECT_DYNAMIC);
			dynamic_base_model_pointer
				->clearProperty(VIEW::GeometricObject::PROPERTY__OBJECT_STATIC);
			
			
			// ------------------------------------------
			// setup drawing precision ------------------
			// ------------------------------------------
			
			switch (ObjectProcessor::getValue(ADDRESS__DYNAMIC_DRAWING_PRECISION))
			{
			  case VALUE__DRAWING_PRECISION_LOW:
					dynamic_base_model_pointer
						->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_LOW);
					break;
					
			  case VALUE__DRAWING_PRECISION_MEDIUM:
					dynamic_base_model_pointer
						->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_MEDIUM);
					break;
					
			  case VALUE__DRAWING_PRECISION_HIGH:
					dynamic_base_model_pointer
						->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH);
					break;
					
			  case VALUE__DRAWING_PRECISION_ULTRA:
					dynamic_base_model_pointer
						->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_ULTRA);
					break;
					
			  default:
					dynamic_base_model_pointer
						->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_PRECISION_HIGH);
					break;
			}
			
			
			// ------------------------------------------
			// setup drawing mode -----------------------
			// ------------------------------------------
			
			switch (ObjectProcessor::getValue(ADDRESS__DYNAMIC_DRAWING_MODE))
			{
			  case VALUE__DRAWING_MODE_DOTS:
					dynamic_base_model_pointer
						->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_MODE_DOTS);
					break;
					
			  case VALUE__DRAWING_MODE_WIREFRAME:
					dynamic_base_model_pointer
						->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_MODE_WIREFRAME);
					break;
					 
			  case VALUE__DRAWING_MODE_SOLID:
					dynamic_base_model_pointer
						->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_MODE_SOLID);
					break;
					
			  default:
					dynamic_base_model_pointer
						->setProperty(VIEW::GeometricObject::PROPERTY__DRAWING_MODE_SOLID);
					break;
			}
			
// ------------------------------------------------------------------------------
// register the color calculator ------------------------------------------------
// ------------------------------------------------------------------------------
			
			dynamic_base_model_pointer->registerColorCalculator(*color_calculator_);

// ------------------------------------------------------------------------------
// applies the processors to the composite --------------------------------------
// ------------------------------------------------------------------------------

			//				object_pointer->apply(*static_base_model_pointer);
			//			object_pointer->apply(*dynamic_base_model_pointer);
			remove_model.setProperty(VIEW::GeometricObject::PROPERTY__OBJECT_DYNAMIC);

			if (ObjectProcessor::getValue(ADDRESS__DYNAMIC_MODEL) == VALUE__MODEL_REMOVE)
			{
				applyOnComposite_(composite, &remove_model);
			}
			else
			{
				//				if (ObjectProcessor::getValue(ADDRESS__DYNAMIC_MODEL) != VALUE__SELECT
				//						&& ObjectProcessor::getValue(ADDRESS__DYNAMIC_MODEL) != VALUE__DESELECT)
				//				{
				//					applyOnComposite_(composite, &remove_model);
				//				}

				applyOnComposite_(composite, dynamic_base_model_pointer);
			}
				 
// ------------------------------------------------------------------------------
// calculate the center of the composite ----------------------------------------
// ------------------------------------------------------------------------------

			calculateCenter(composite);
    }

	  void MoleculeObjectProcessor::calculateCenter(Composite &composite)
    {
			GeometricCenterProcessor center;

			applyOnComposite_(composite, &center);
				
			ObjectProcessor::setViewCenter(center.getCenter());
			ObjectProcessor::setViewDirection(2);

			if (ObjectProcessor::getValue(ADDRESS__CAMERA_DISTANCE) != BALL::VIEW::VALUE__INVALID)
			{
				ObjectProcessor::setViewDistance(ObjectProcessor::getValue(ADDRESS__CAMERA_DISTANCE));
			}
			else
			{
				BoundingBoxProcessor bbox;

				applyOnComposite_(composite, &bbox);

				//				cout << (bbox.getUpper() - bbox.getLower()).getLength() - center.getCenter().z << endl;

				ObjectProcessor::setViewDistance((bbox.getUpper() - bbox.getLower()).getLength() - center.getCenter().z + 3);
			}
		}

	  bool MoleculeObjectProcessor::checkResidue(Composite &composite)
    {
			ResidueChecker residue_checker(fragmentdb);

			if (RTTI::isKindOf<AtomContainer>(composite))
			{
				(RTTI::castTo<AtomContainer>(composite))->apply(residue_checker);

				return residue_checker.getStatus();
			}
			else if (RTTI::isKindOf<System>(composite))
			{
				(RTTI::castTo<System>(composite))->apply(residue_checker);

				return residue_checker.getStatus();				
			} else {
				Log << "ResidueChecker: cannot apply to a " << typeid(composite).name() << " object" << endl;
			}
			
			return false;
    }

		void MoleculeObjectProcessor::setColorCalculator
			(ColorCalculatorValues values, const ColorRGBA& first_color,
			 const ColorRGBA& /* second_color */, const ColorRGBA& /* third_color */)
		{
			switch (values)
			{
   			default:
  			case COLORCALCULATOR_VALUES__CUSTOM:
					color_calculator_ = &custom_color_calculator_;
					color_calculator_->setDefaultColor(first_color);
					break;

  			case COLORCALCULATOR_VALUES__ELEMENT:
					color_calculator_ = &element_color_calculator_;
					break;

  			case COLORCALCULATOR_VALUES__RESIDUE_NAME:
					color_calculator_ = &residue_name_color_calculator_;
					break;

  			case COLORCALCULATOR_VALUES__ATOM_CHARGE:
					color_calculator_ = &atom_charge_color_calculator_;
					break;
      }
		}
		
	  void MoleculeObjectProcessor::setColorCalculator(ColorCalculator& color_calculator)
    {
			color_calculator_ = &color_calculator;
    }

  	void MoleculeObjectProcessor::applyOnComposite_(Composite &composite, UnaryProcessor<Composite> *processor)
    {
			composite.apply(*processor);
		}

  	void MoleculeObjectProcessor::applyOnComposite_(Composite &composite, UnaryProcessor<Atom> *processor)
    {
			composite.apply(*processor);
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/GUI/FUNCTOR/moleculeObjectProcessor.iC>
#		endif

	} // namespace MOLVIEW

} // namespace BALL

// $Id: moleculeObjectProcessor.h,v 1.3 2001/01/07 15:45:31 hekl Exp $

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_MOLECULEOBJECTPROCESSOR_H
#define BALL_MOLVIEW_GUI_FUNCTOR_MOLECULEOBJECTPROCESSOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_MOLECULE_H
#	include <BALL/KERNEL/molecule.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
# include <BALL/KERNEL/protein.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_STRUCTURE_GEOMETRICPROPERTIES_H
# include <BALL/STRUCTURE/geometricProperties.h>
#endif

#ifndef BALL_STRUCTURE_FRAGMENTDB_H
# include <BALL/STRUCTURE/fragmentDB.h>
#endif

#ifndef BALL_STRUCTURE_RESIDUECHECKER_H
# include <BALL/STRUCTURE/residueChecker.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_GEOMETRICOBJECTSELECTOR_H
# include <BALL/MOLVIEW/FUNCTOR/geometricObjectSelector.h>
#endif

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLATOMBONDMODELCONNECTOR_H
# include <BALL/MOLVIEW/GUI/FUNCTOR/glAtomBondModelConnector.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_REMOVEMODEL_H
# include <BALL/MOLVIEW/FUNCTOR/removeModel.h>
#endif

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLBACKBONEMODEL_H
# include <BALL/MOLVIEW/GUI/FUNCTOR/glBackboneModel.h>
#endif

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLBALLANDSTICKMODEL_H
# include <BALL/MOLVIEW/GUI/FUNCTOR/glBallAndStickModel.h>
#endif

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLLINEMODEL_H
# include <BALL/MOLVIEW/GUI/FUNCTOR/glLineModel.h>
#endif

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLSURFACEMODEL_H
# include <BALL/MOLVIEW/GUI/FUNCTOR/glSurfaceModel.h>
#endif

#ifndef BALL_MOLVIEW_GUI_FUNCTOR_GLVANDERWAALSMODEL_H
# include <BALL/MOLVIEW/GUI/FUNCTOR/glVanDerWaalsModel.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_STANDARDCOLORCALCULATOR_H
#	include <BALL/MOLVIEW/FUNCTOR/standardColorCalculator.h>
#endif

#ifndef BALL_VIEW_GUI_FUNCTOR_OBJECTPROCESSOR_H
#	include <BALL/VIEW/GUI/FUNCTOR/objectProcessor.h>
#endif

  
namespace BALL
{

	namespace MOLVIEW
	{

		using VIEW::ObjectProcessor;

		/**
		*/
		class MoleculeObjectProcessor
			: public ObjectProcessor
		{
			public:

			BALL_CREATE_DEEP(MoleculeObjectProcessor)
			/**	@name	Constructors and Destructors
			*/
			//@{

			MoleculeObjectProcessor();

			MoleculeObjectProcessor
				(const MoleculeObjectProcessor& object_processor, bool deep = true);

			virtual ~MoleculeObjectProcessor()
				throw();
			//@}


			/**	@name	Exceptions
			*/
			//@{
			
			class AllocationFailure
				:	public Exception::GeneralException
			{
				public:

			  AllocationFailure(const char* file, int line);
			};

			//@}

			/**	@name Accessors
			*/
			//@{
			virtual void applyOn(Composite &composite);

			virtual void calculateCenter(Composite &composite);

			virtual bool checkResidue(Composite &composite);

			virtual void setColorCalculator(ColorCalculatorValues values,
																			const ColorRGBA &first_color = ColorRGBA(),
																			const ColorRGBA &second_color = ColorRGBA(),
																			const ColorRGBA &third_color = ColorRGBA());

			virtual void setColorCalculator(ColorCalculator& color_calculator);

			//@}

			FragmentDB fragmentdb;

		  private:

			void applyOnComposite_(Composite &composite, UnaryProcessor<Composite> *processor);		
			void applyOnComposite_(Composite &composite, UnaryProcessor<Atom> *processor);		
			ColorCalculator *color_calculator_;

			GLAtomBondModelConnector model_connector_;

			ElementColorCalculator     element_color_calculator_;
			ResidueNameColorCalculator residue_name_color_calculator_;
			AtomChargeColorCalculator  atom_charge_color_calculator_;
			CustomColorCalculator      custom_color_calculator_;
		};

#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/GUI/FUNCTOR/moleculeObjectProcessor.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_GUI_FUNCTOR_MOLECULEOBJECTPROCESSOR_H 

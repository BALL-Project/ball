// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularModel.h,v 1.2 2003/08/26 08:04:35 oliver Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_MOLECULARMODEL_H
#define BALL_MOLVIEW_FUNCTOR_MOLECULARMODEL_H

#ifndef BALL_VIEW_GUI_FUNCTOR_MODELPROCESSOR_H
#	include <BALL/VIEW/GUI/FUNCTOR/modelProcessor.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_BASEMODELCONNECTOR_H
#	include <BALL/MOLVIEW/FUNCTOR/baseModelConnector.h>
#endif

namespace BALL
{
	using VIEW::ModelProcessor;
	using VIEW::GeometricObject;

	namespace MOLVIEW
	{
		/** MolecularModelProcessor class.
				The class MolecularModelProcessor is base class for all model processors. 
				Every model must have a MolecularModelConnector object.
				Some protected helper methods are included, which are used by some derived classes. 
				\ingroup  MolviewFunctorsModels
		*/
		class MolecularModelProcessor
			: public ModelProcessor
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
			*/
			MolecularModelProcessor()
				throw();

			/** Copy constructor.
			*/
			MolecularModelProcessor(const MolecularModelProcessor& base_model_processor)
			  throw();

			//@} 
			/** @name Destructors 
			*/ 
			//@{

			/** Destructor.
			*/
			virtual ~MolecularModelProcessor()
				throw();

			/** Explicit default initialization.
					Calls ModelProcessor::clear.
			*/
			virtual void clear()
				throw();

			//@} 
			/**	@name	Assignment 
			*/ 
			//@{

			/** Assignment.
			*/
			void set(const MolecularModelProcessor& base_model_processor)
				throw();

			/** Assignment operator.
					Calls set().
			*/
			const MolecularModelProcessor& operator = (const MolecularModelProcessor& base_model_processor)
				throw();

			/** Swapping of a MolecularModelProcessor.
			*/
			void swap(MolecularModelProcessor& base_model_processor)
				throw();

			//@} 
			/**	@name	Accessors: inspectors and mutators 
			*/ 
			//@{
			
			/** Change the MolecularModelConnector of this MolecularModelProcessor.
					\param    model_connector the new MolecularModelConnector
			*/
			void registerModelConnector(const BaseModelConnector& model_connector)
				throw();

			/** Set the MolecularModelConnector to the default one.
			*/
			void unregisterModelConnector()
				throw();

			/** Mutable inspection of the MolecularModelConnector.
			*/
			BaseModelConnector* getModelConnector()
				throw();

			//@} 
			/**	@name Processor specific methods 
			*/ 
			//@{
			
			/** Start method.
			*/
			virtual bool start();
			
			//@} 
			/**	@name	debuggers and diagnostics 
			*/ 
			//@{

			/** Internal state and consistency self-validation.
					Calls MolecularModelConnector::isValid.
					\return		bool true if the internal state is correct
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of this instance to 
					the output ostream with a dumping depth.
					\param   s output stream where to output the state 
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			private:

			void dump_(std::ostream& s, Size depth) const;

			/* pointer to a visitor used for connecting the different models */
			BaseModelConnector* model_connector_;
		};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/MOLVIEW/FUNCTOR/molecularModel.iC>
#	endif

	} // namespace MOLVIEW
} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_MOLECULARMODEL_H 

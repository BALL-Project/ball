// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomBondModelConnector.h,v 1.3 2003/08/29 10:36:40 amoll Exp $
//

#ifndef BALL_VIEW_MODELS_ATOMBONDMODELCONNECTOR_H
#define BALL_VIEW_MODELS_ATOMBONDMODELCONNECTOR_H

#ifndef BALL_VIEW_PRIMITIVES_LINE_H
#	include <BALL/VIEW/PRIMITIVES/line.h>
#endif

#ifndef BALL_VIEW_PRIMITIVES_TUBE_H
#	include <BALL/VIEW/PRIMITIVES/tube.h>
#endif

#ifndef BALL_VIEW_MODELS_BASEMODELCONNECTOR_H
#	include <BALL/VIEW/MODELS/baseModelConnector.h>
#endif

#ifndef BALL_VIEW_PRIMITIVES_TWOCOLOREDLINE_H
#	include <BALL/VIEW/PRIMITIVES/twoColoredLine.h>
#endif

#ifndef BALL_VIEW_PRIMITIVES_TWOCOLOREDTUBE_H
#	include <BALL/VIEW/PRIMITIVES/twoColoredTube.h>
#endif

namespace BALL
{
	class Atom;
	class Bond;

	namespace VIEW
	{
		/** AtomBondModelConnector class.
				The class AtomBondModelConnector is derived from BaseModelConnector.
				This class connects different models with each other.
				The connector model is used for the Bond objects between the various models
				of the adjacent Atom objects.
				\ingroup  ViewModels
		*/
		class AtomBondModelConnector
			: public BaseModelConnector
		{
			public:
			
			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
			*/
			AtomBondModelConnector()
				throw();

			/** Copy constructor.
			*/
			AtomBondModelConnector(const AtomBondModelConnector& connector)
				throw();

			/** Destructor.
			*/
			virtual ~AtomBondModelConnector()
				throw();

			//@} 
			/**	@name	Accessors: inspectors and mutators 
			*/ 
			//@{

			/** Visit method.
					If the composite is of kind Bond, the model of the two adjacent Atom
					objects is determined. The following connector model is created for
					the bond. The first two models are the models of the adjacent atoms. The connector
					model for the swapped atom models are the same.
					\param  composite the Composite object (the Bond object) for which a connection of two models 
									should be created
					\exception  OutOfMemory thrown if the memory allocation for creating the connector model failed
			*/
			virtual void visit(Composite& composite);

			///
			void setModelType(Index type)
				throw() { model_type_ = type;}
			

			protected:

			//@} 
			/** @name Protected members 
			*/ 
			//@{

			/** Inspection of the model of an atom.
			*/
			Property getModel_(Atom& atom)
				throw();

			//@}
			
  		private:

			void createLineRepresentation_(Bond& bond)
				throw(Exception::OutOfMemory);

			void createTubeRepresentation_(Bond& bond)
				throw(Exception::OutOfMemory);

			Index model_type_;

		};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_MODELS_ATOMBONDMODELCONNECTOR_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomBondModelConnector.h,v 1.13 2003/08/26 08:04:32 oliver Exp $
//

#ifndef BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELCONNECTOR_H
#define BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELCONNECTOR_H

#ifndef BALL_VIEW_PRIMITIV_LINE_H
#	include <BALL/VIEW/PRIMITIV/line.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_TUBE_H
#	include <BALL/VIEW/PRIMITIV/tube.h>
#endif

#ifndef BALL_MOLVIEW_FUNCTOR_BASEMODELCONNECTOR_H
#	include <BALL/MOLVIEW/FUNCTOR/baseModelConnector.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_TWOCOLOREDLINE_H
#	include <BALL/VIEW/PRIMITIV/twoColoredLine.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_TWOCOLOREDTUBE_H
#	include <BALL/VIEW/PRIMITIV/twoColoredTube.h>
#endif

using BALL::VIEW::Line;
using BALL::VIEW::Tube;
using BALL::VIEW::TwoColoredLine;
using BALL::VIEW::TwoColoredTube;

namespace BALL
{
	class Atom;
	class Bond;

	namespace MOLVIEW
	{
		/** AtomBondModelConnector class.
				The class AtomBondModelConnector is derived from BaseModelConnector.
				This class connects different models with each other.
				The connector model is used for the Bond objects between the various models
				of the adjacent Atom objects.
				\ingroup  MolviewFunctorsModels
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

	} // namespace MOLVIEW
} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELCONNECTOR_H

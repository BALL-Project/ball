// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomBondModelConnector.h,v 1.12 2003/03/26 13:56:46 anhi Exp $

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

#ifndef BALL_MOLVIEW_PRIMITIV_TWOCOLOREDLINE_H
#	include <BALL/MOLVIEW/PRIMITIV/twoColoredLine.h>
#endif

#ifndef BALL_MOLVIEW_PRIMITIV_TWOCOLOREDTUBE_H
#	include <BALL/MOLVIEW/PRIMITIV/twoColoredTube.h>
#endif

using BALL::VIEW::Line;
using BALL::VIEW::Tube;

namespace BALL
{
	class Atom;
	class Bond;

	namespace MOLVIEW
	{
		/** AtomBondModelConnector class.
				The class AtomBondModelConnector is derived from  \link BaseModelConnector BaseModelConnector \endlink .
				This class is a connector class which connects different models with each
				other.
				The following models are connected with the specified models:
				
				  -  {\em Line Model} - {\em Line Model} : connected with {\em Line Model}
				  -  {\em Line Model} - {\em Ball And Stick Model} : connected with {\em Line Model}
				  -  {\em Ball And Stick Model} - {\em Ball And Stick Model} : connected with {\em Ball And Stick Model}
				  -  {\em Line Model} - {\em Van Der Waals Model} : connected with {\em Line Model}
				  -  {\em Ball And Stick Model} - {\em Van der Waals Model} : connected with {\em Ball And Stick Model}
				  -  {\em Van der Waals Model} - {\em Van der Waals Model} : connected with no model
				
				The connector model is used for the  \link Bond Bond \endlink  objects between the various models
				of the adjacent  \link Atom Atom \endlink  objects.
				This class is used by the model processors to determine the connector model
				between different models.  \par
				
    		\ingroup  MolviewFunctorsBase
		*/
		class AtomBondModelConnector
			: public BaseModelConnector
		{
			public:
			
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new atomBondModelConnector.
					Initialize the state of {\em *this} atomBondModelConnector to the
					default state.
					@return      AtomBondModelConnector new constructed atomBondModelConnector
					@see         BaseModelConnector
			*/
			AtomBondModelConnector()
				throw();

			/** Copy constructor.
					Construct new atomBondModelConnector by copying the atomBondModelConnector {\em connector}.
					Initializes the state of {\em this} atomBondModelConnector to the state
					of {\em connector}.
					@param       connector the atomBondModelConnector to be copied
					@param       deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em connector}
					@return      AtomBondModelConnector new constructed atomBondModelConnector copied from {\em connector}
					@see         BaseModelConnector
			*/
			AtomBondModelConnector
				(const AtomBondModelConnector& connector, bool deep = true)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} atomBondModelConnector.
					Calls  \link BaseModelConnector::destroy BaseModelConnector::destroy \endlink .
					@see  BaseModelConnector
			*/
			virtual ~AtomBondModelConnector()
				throw();

			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			/** Visit method.
					If {\em composite} is of kind  \link Bond Bond \endlink  the model of the two adjacent  \link Atom Atom \endlink 
					objects is determined. The following connector model is created for
					the bond. The first two models are the models of the adjacent atoms. The connector
					model for the swapped atom models are the same.

				    -  {\em Line Model} - {\em Line Model} : connected with {\em Line Model}
						-  {\em Line Model} - {\em Ball And Stick Model} : connected with {\em Line Model}
						-  {\em Ball And Stick Model} - {\em Ball And Stick Model} : connected with {\em Ball And Stick Model}
						-  {\em Line Model} - {\em Van Der Waals Model} : connected with {\em Line Model}
						-  {\em Ball And Stick Model} - {\em Van der Waals Model} : connected with {\em Ball And Stick Model}
						-  {\em Van der Waals Model} - {\em Van der Waals Model} : connected with no model
					
					@param  composite the  \link Composite Composite \endlink  object (the  \link Bond Bond \endlink  object) for which a connector model should be created
					@exception  OutOfMemory thrown if the memory allocation for creating the connector model failed
					@see        getModel_
			*/
			virtual void visit(Composite& composite);
			//@}
			

			protected:

			/** @name Protected members
			*/
			//@{
			/** Inspection of the model of an atom.
					Inspection of the model of {\em atom}. This method retrieves the model of the
					graphical representation that is appended to {\em atom}. Returns {\em PROPERTY__UNKNOWN}
					if no model is appended to {\em atom}.
					This method uses the object  \link FindGeometricObjects FindGeometricObjects \endlink  to search for models containing
					the same properties as {\em *this} atomBondModelConnector.
					@return  Property the property (the model) of the given  \link Atom Atom \endlink 
					@see     visit
					@see     BaseModelConnector
					@see     FindGeometricObjects
					@see     Property
					@see     Atom
			*/
			Property getModel_(Atom& atom)
				throw();
			//@}
			
		  protected:

			/** @name Creation methods
					This methods create primitives without OpenGL implementation.
					This methods are overridden by the class  \link GLAtomBondModelConnector GLAtomBondModelConnector \endlink 
					to create primitives with OpenGL implementation to generate the graphical
					representation of the shapes they represent.
					@see  GLAtomBondModelConnector
			*/
			//@{
			/** Create a one colored line.
					Create a  \link Line Line \endlink .
					This method is overridden by the class  \link GLAtomBondModelConnector GLAtomBondModelConnector \endlink  to create
					a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create one colored
					lines for  \link Bond Bond \endlink  objects whose  \link Atom Atom \endlink  objects have the same
					element types and thus the same color.
					@see  Line
					@see  GLAtomBondModelConnector
					@see  Atom
					@see  Bond
			*/
			virtual Line* createLine_();

			/** Create a two colored line.
					Create a  \link TwoColoredLine TwoColoredLine \endlink .
					This method is overridden by the class  \link GLAtomBondModelConnector GLAtomBondModelConnector \endlink  to create
					a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create two colored
					lines for  \link Bond Bond \endlink  objects whose  \link Atom Atom \endlink  objects have different
					element types and thus different colors.
					@see  TwoColoredLine
					@see  GLAtomBondModelConnector
					@see  Atom
					@see  Bond
			*/
			virtual TwoColoredLine* createTwoColoredLine_();

			/** Create a one colored tube.
					Create a  \link Tube Tube \endlink .
					This method is overridden by the class  \link GLAtomBondModelConnector GLAtomBondModelConnector \endlink  to create
					a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create one colored
					tubes for  \link Bond Bond \endlink  objects whose  \link Atom Atom \endlink  objects have the same
					element types and thus the same color.
					@see  Tube
					@see  GLAtomBondModelConnector
					@see  Atom
					@see  Bond
			*/
			virtual Tube* createTube_();

			/** Create a two colored tube.
					Create a  \link TwoColoredTube TwoColoredTube \endlink .
					This method is overridden by the class  \link GLAtomBondModelConnector GLAtomBondModelConnector \endlink  to create
					a primitive with openGL implementation.
					The method  \link operator() operator() \endlink  uses this method to create two colored
					tubes for  \link Bond Bond \endlink  objects whose  \link Atom Atom \endlink  objects have different
					element types and thus different colors.
					@see  TwoColoredTube
					@see  GLAtomBondModelConnector
					@see  Atom
					@see  Bond
			*/
			virtual TwoColoredTube* createTwoColoredTube_();
			//@}

  		private:

			void createLineRepresentation_(Bond& bond)
				throw(Exception::OutOfMemory);

			void createTubeRepresentation_(Bond& bond)
				throw(Exception::OutOfMemory);

		};

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELCONNECTOR_H

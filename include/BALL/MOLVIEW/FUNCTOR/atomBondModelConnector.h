// $Id: atomBondModelConnector.h,v 1.3 2001/05/13 14:55:21 hekl Exp $

#ifndef BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELCONNECTOR_H
#define BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELCONNECTOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_BOND_H
#	include <BALL/KERNEL/bond.h>
#endif

#ifndef BALL_VIEW_DATATYPE_COLOR_H
#	include <BALL/VIEW/DATATYPE/color.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_LINE_H
#	include <BALL/VIEW/PRIMITIV/line.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_TUBE_H
#	include <BALL/VIEW/PRIMITIV/tube.h>
#endif

#ifndef BALL_MOLVIEW_COMMON_COMMON_H
#	include <BALL/MOLVIEW/COMMON/common.h>
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

//using namespace BALL;
using namespace BALL::VIEW;

 
namespace BALL
{

	namespace MOLVIEW
	{
		
		/** AtomBondModelConnector class.
				{\bf Framework:} BALL/MOLVIEW/FUNCTOR\\
				{\bf Definition:} \URL{BALL/MOLVIEW/FUNCTOR/atomBondModelConnector.h}\\ \\
				The class AtomBondModelConnector is derived from \Ref{BaseModelConnector}.
				This class is a connector class which connects different models with each
				other.
				The following models are connected with the specified models:
				\begin{itemize}
				  \item  {\em Line Model} - {\em Line Model} : connected with {\em Line Model}
				  \item  {\em Line Model} - {\em Ball And Stick Model} : connected with {\em Line Model}
				  \item  {\em Ball And Stick Model} - {\em Ball And Stick Model} : connected with {\em Ball And Stick Model}
				  \item  {\em Line Model} - {\em Van Der Waals Model} : connected with {\em Line Model}
				  \item  {\em Ball And Stick Model} - {\em Van der Waals Model} : connected with {\em Ball And Stick Model}
				  \item  {\em Van der Waals Model} - {\em Van der Waals Model} : connected with no model
				\end{itemize}
				The connector model is used for the \Ref{Bond} objects between the various models
				of the adjacent \Ref{Atom} objects.
				This class is used by the model processors to determine the connector model
				between different models.
				@memo    AtomBondModelConnector class (BALL MOLVIEW functor framework)
				@author  $Author: hekl $
				@version $Revision: 1.3 $
				@date    $Date: 2001/05/13 14:55:21 $
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
					@param       deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em connector}
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
					Calls \Ref{BaseModelConnector::destroy}.
					@see  BaseModelConnector
			*/
			virtual ~AtomBondModelConnector()
				throw();

			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{
			/** Visit method.
					If {\em composite} is of kind \Ref{Bond} the model of the two adjacent \Ref{Atom}
					objects is determined. The following connector model is created for
					the bond. The first two models are the models of the adjacent atoms. The connector
					model for the swapped atom models are the same.
					\begin{itemize}
				    \item  {\em Line Model} - {\em Line Model} : connected with {\em Line Model}
						\item  {\em Line Model} - {\em Ball And Stick Model} : connected with {\em Line Model}
						\item  {\em Ball And Stick Model} - {\em Ball And Stick Model} : connected with {\em Ball And Stick Model}
						\item  {\em Line Model} - {\em Van Der Waals Model} : connected with {\em Line Model}
						\item  {\em Ball And Stick Model} - {\em Van der Waals Model} : connected with {\em Ball And Stick Model}
						\item  {\em Van der Waals Model} - {\em Van der Waals Model} : connected with no model
					\end{itemize}
					@param  composite the \Ref{Composite} object (the \Ref{Bond} object) for which a connector model should be created
					@exception  OutOfMemory thrown if the memory allocation for creating the connector model failed
					@see        getModel_
			*/
			virtual void visit(Composite& composite)
				throw(Exception::OutOfMemory);
			//@}
			

			protected:

			/** @name Protected members
			*/
			//@{
			/** Inspection of the model of an atom.
					Inspection of the model of {\em atom}. This method retrieves the model of the
					graphical representation that is appended to {\em atom}. Returns {\em PROPERTY__UNKNOWN}
					if no model is appended to {\em atom}.
					This method uses the object \Ref{FindGeometricObjects} to search for models containing
					the same properties as {\em *this} atomBondModelConnector.
					@return  Property the property (the model) of the given \Ref{Atom}
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
					This methods are overridden by the class \Ref{GLAtomBondModelConnector}
					to create primitives with OpenGL implementation to generate the graphical
					representation of the shapes they represent.
					@see  GLAtomBondModelConnector
			*/
			//@{
			/** Create a one colored line.
					Create a \Ref{Line}.
					This method is overridden by the class \Ref{GLAtomBondModelConnector} to create
					a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create one colored
					lines for \Ref{Bond} objects whose \Ref{Atom} objects have the same
					element types and thus the same color.
					@see  Line
					@see  GLAtomBondModelConnector
					@see  Atom
					@see  Bond
			*/
			virtual Line* createLine_();

			/** Create a two colored line.
					Create a \Ref{TwoColoredLine}.
					This method is overridden by the class \Ref{GLAtomBondModelConnector} to create
					a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create two colored
					lines for \Ref{Bond} objects whose \Ref{Atom} objects have different
					element types and thus different colors.
					@see  TwoColoredLine
					@see  GLAtomBondModelConnector
					@see  Atom
					@see  Bond
			*/
			virtual TwoColoredLine* createTwoColoredLine_();

			/** Create a one colored tube.
					Create a \Ref{Tube}.
					This method is overridden by the class \Ref{GLAtomBondModelConnector} to create
					a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create one colored
					tubes for \Ref{Bond} objects whose \Ref{Atom} objects have the same
					element types and thus the same color.
					@see  Tube
					@see  GLAtomBondModelConnector
					@see  Atom
					@see  Bond
			*/
			virtual Tube* createTube_();

			/** Create a two colored tube.
					Create a \Ref{TwoColoredTube}.
					This method is overridden by the class \Ref{GLAtomBondModelConnector} to create
					a primitive with openGL implementation.
					The method \Ref{operator()} uses this method to create two colored
					tubes for \Ref{Bond} objects whose \Ref{Atom} objects have different
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

			
#			ifndef BALL_NO_INLINE_FUNCTIONS
#				include <BALL/MOLVIEW/FUNCTOR/atomBondModelConnector.iC>
#			endif

	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_FUNCTOR_ATOMBONDMODELCONNECTOR_H

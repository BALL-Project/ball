// $Id: glObjectCollector.h,v 1.5 2001/07/09 18:26:43 sturm Exp $

#ifndef BALL_VIEW_GUI_FUNCTOR_GLOBJECTCOLLECTOR_H
#define BALL_VIEW_GUI_FUNCTOR_GLOBJECTCOLLECTOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glObject.h>
#endif


namespace BALL
{
	
	namespace VIEW
	{

		/** GLObjectCollector class.
				{\bf Framework:} BALL/VIEW/GUI/FUNCTOR\\
				{\bf Defintion:} \URL{BALL/VIEW/GUI/FUNCTOR/glObjectCollector.h}\\
				{\bf Category:} \Ref{GLObject} container\\ \\
				The class GLObjectCollector is responsible for collecting the \Ref{GLObject}
				objects that are found in a \Ref{Composite} tree for the later processing
				in the render engine \Ref{CompositeDescriptor}. The \Ref{GLObject} objects
				are separated into different visualization groups (=lists) that are necessary
				for proper rendering.
				This class will be used internally from the \Ref{CompositeDescriptor} class
				to collect {\em *this} objects.
				@memo    GLObjectCollector class (BALL VIEW gui functor framework)
				@author  $Author: sturm $
				@version $Revision: 1.5 $
				@date    $Date: 2001/07/09 18:26:43 $
		*/
		class GLObjectCollector: public UnaryProcessor<Composite>
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new glObjectCollector.
					The state of {\em *this} glObjectCollector is: all \Ref{GLObject} lists
					are empty and the root composite is set to {\tt 0}.
					@return GLObjectCollector new constructed glObjectCollector
					@see    setRootComposite
					@see    GLObject
			 */
			GLObjectCollector()
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glObjectCollector.
					Calls \Ref{destroy}.
					@see         destroy
			 */
			virtual ~GLObjectCollector()
				throw();

			/** Explicit default initialization.
					Clears all \Ref{GLObject} lists of {\em *this} glObjectCollector.
					@see     GLObject
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Append new glObject to the static list.
					Append new \Ref{GLObject} to the static list of {\em *this} glObjectCollector.
					See \Ref{GeometricObject} for information concerning the different
					properties an object can have.
					@param  GL_object the \Ref{GLObject} to be appended to the static list of {\em *this} glObjectCollector.
					@see    getStaticList
					@see    GLObject
					@see    GeometricObject
			*/
			void appendToStaticList(GLObject* GL_object)
				throw();

			/** Mutable inspection of the static list.
					Access a mutable reference of the static list of {\em *this} glObjectCollector.
					See \Ref{GeometricObject} for information concerning the different
					properties an object can have.
					@return  list<GLObject*>& mutable reference of the static list of {\em *this} glObjectCollector
					@see    appendToStaticList
					@see    GLObject
					@see    GeometricObject
			*/
			list<GLObject *>& getStaticList()
				throw();

			/** Non-mutable inspection of the static list.
					For further information see \Ref{getStaticList}.
			*/
			const list<GLObject *>& getStaticList() const
				throw();

			/** Append new glObject to the static always front list.
					Append new \Ref{GLObject} to the static always front list of {\em *this} 
					glObjectCollector.
					See \Ref{GeometricObject} for information concerning the different
					properties an object can have.
					@param  GL_object the \Ref{GLObject} to be appended to the static always front list of {\em *this} glObjectCollector.
					@see    getStaticAlwaysFrontList
					@see    GLObject
					@see    GeometricObject
			*/
			void appendToStaticAlwaysFrontList(GLObject* GL_object)
				throw();

			/** Mutable inspection of the static always front list.
					Access a mutable reference of the static always front list of {\em *this}
					glObjectCollector.
					See \Ref{GeometricObject} for information concerning the different
					properties an object can have.
					@return  list<GLObject*>& mutable reference of the static always front list of {\em *this} glObjectCollector
					@see    appendToStaticAlwaysFrontList
					@see    GLObject
					@see    GeometricObject
			*/
			list<GLObject *>& getStaticAlwaysFrontList()
				throw();

			/** Non-mutable inspection of the static always front list.
			For further information see \Ref{getStaticAlwaysFrontList}.
			*/
			const list<GLObject *>& getStaticAlwaysFrontList() const
				throw();

			/** Append new glObject to the static wireframe list.
					Append new \Ref{GLObject} to the static wireframe list of {\em *this} 
					glObjectCollector.
					See \Ref{GeometricObject} for information concerning the different
					properties an object can have.
					@param  GL_object the \Ref{GLObject} to be appended to the static wireframe list of {\em *this} glObjectCollector.
					@see    getStaticWireframeList
					@see    GLObject
					@see    GeometricObject
			*/
			void appendToStaticWireframeList(GLObject* GL_object)
				throw();

			/** Mutable inspection of the static wireframe list.
					Access a mutable reference of the static wireframe list of {\em *this}
					glObjectCollector.
					See \Ref{GeometricObject} for information concerning the different
					properties an object can have.
					@return  list<GLObject*>& mutable reference of the static wireframe list of {\em *this} glObjectCollector
					@see    appendToStaticWireframeList
					@see    GLObject
					@see    GeometricObject
			*/
			list<GLObject *>& getStaticWireframeList()
				throw();

			/** Non-mutable inspection of the static wireframe list.
					For further information see \Ref{getStaticWireframeList}.
			*/
			const list<GLObject *>& getStaticWireframeList() const
				throw();

			/** Append new glObject to the static wireframe always front list.
					Append new \Ref{GLObject} to the static wireframe always front list of
					{\em *this} glObjectCollector.
					See \Ref{GeometricObject} for information concerning the different
					properties an object can have.
					@param  GL_object the \Ref{GLObject} to be appended to the static wireframe always front list of {\em *this} glObjectCollector.
					@see    getStaticWireframeAlwaysFrontList
					@see    GLObject
					@see    GeometricObject
			*/
			void appendToStaticWireframeAlwaysFrontList(GLObject* GL_object)
				throw();

			/** Mutable inspection of the static wireframe always front list.
					Access a mutable reference of the static wireframe always front list of
					{\em *this} glObjectCollector.
					See \Ref{GeometricObject} for information concerning the different
					properties an object can have.
					@return  list<GLObject*>& mutable reference of the static wireframe always front list of {\em *this} glObjectCollector
					@see    appendToStaticWireframeAlwaysFrontList
					@see    GLObject
					@see    GeometricObject
			*/
			list<GLObject *>& getStaticWireframeAlwaysFrontList()
				throw();

			/** Non-mutable inspection of the static wireframe always front list.
					For further information see \Ref{getStaticWireframeAlwaysFrontList}.
			*/
			const list<GLObject *>& getStaticWireframeAlwaysFrontList() const
				throw();

			/** Append new glObject to the dynamic list.
					Append new \Ref{GLObject} to the dynamic list of
					{\em *this} glObjectCollector.
					See \Ref{GeometricObject} for information concerning the different
					properties an object can have.
					@param  GL_object the \Ref{GLObject} to be appended to the dynamic list of {\em *this} glObjectCollector.
					@see    getDynamicList
					@see    GLObject
					@see    GeometricObject
			*/
			void appendToDynamicList(GLObject* GL_object)
				throw();

			/** Mutable inspection of the dynamic list.
					Access a mutable reference of the dynamic list of
					{\em *this} glObjectCollector.
					See \Ref{GeometricObject} for information concerning the different
					properties an object can have.
					@return  list<GLObject*>& mutable reference of the dynamic list of {\em *this} glObjectCollector
					@see    appendToDynamicList
					@see    GLObject
					@see    GeometricObject
			*/
			list<GLObject *>& getDynamicList()
				throw();

			/** Non-mutable inspection of the dynamic list.
					For further information see \Ref{getDynamicList}.
			*/
			const list<GLObject *>& getDynamicList() const
				throw();

			/** Append new glObject to the dynamic always front list.
					Append new \Ref{GLObject} to the dynamic always front list of
					{\em *this} glObjectCollector.
					See \Ref{GeometricObject} for information concerning the different
					properties an object can have.
					@param  GL_object the \Ref{GLObject} to be appended to the dynamic always front list of {\em *this} glObjectCollector.
					@see    getDynamicAlwaysFrontList
					@see    GLObject
					@see    GeometricObject
			*/
			void appendToDynamicAlwaysFrontList(GLObject* GL_object)
				throw();

			/** Mutable inspection of the dynamic always front list.
					Access a mutable reference of the dynamic always front list of
					{\em *this} glObjectCollector.
					See \Ref{GeometricObject} for information concerning the different
					properties an object can have.
					@return  list<GLObject*>& mutable reference of the dynamic always front list of {\em *this} glObjectCollector
					@see    appendToDynamicAlwaysFrontList
					@see    GLObject
					@see    GeometricObject
			*/
			list<GLObject *>& getDynamicAlwaysFrontList()
				throw();

			/** Non-mutable inspection of the dynamic always front list.
					For further information see \Ref{getDynamicAlwaysFrontList}.
			*/
			const list<GLObject *>& getDynamicAlwaysFrontList() const
				throw();

			/** Append new glObject to the transparent list.
					Append new \Ref{GLObject} to the transparent list of
					{\em *this} glObjectCollector.
					See \Ref{GeometricObject} for information concerning the different
					properties an object can have.
					@param  GL_object the \Ref{GLObject} to be appended to the transparent list of {\em *this} glObjectCollector.
					@see    getTransparentList
					@see    GLObject
					@see    GeometricObject
			*/
			void appendToTransparentList(GLObject* GL_object)
				throw();

			/** Mutable inspection of the transparent list.
					Access a mutable reference of the transparent list of
					{\em *this} glObjectCollector.
					See \Ref{GeometricObject} for information concerning the different
					properties an object can have.
					@return  list<GLObject*>& mutable reference of the transparent list of {\em *this} glObjectCollector
					@see    appendToTransparentList
					@see    GLObject
					@see    GeometricObject
			*/
			list<GLObject *>& getTransparentList()
				throw();

			/** Non-mutable inspection of the transparent list.
					For further information see \Ref{getTransparentList}.
			*/
			const list<GLObject *>& getTransparentList() const
				throw();

			/** Append new glObject to the transparent always front list.
					Append new \Ref{GLObject} to the transparent always front list of
					{\em *this} glObjectCollector.
					See \Ref{GeometricObject} for information concerning the different
					properties an object can have.
					@param  GL_object the \Ref{GLObject} to be appended to the transparent always front list of {\em *this} glObjectCollector.
					@see    getTransparentAlwaysFrontList
					@see    GLObject
					@see    GeometricObject
			*/
			void appendToTransparentAlwaysFrontList(GLObject* GL_object)
				throw();

			/** Mutable inspection of the transparent always front list.
					Access a mutable reference of the transparent always front list of
					{\em *this} glObjectCollector.
					See \Ref{GeometricObject} for information concerning the different
					properties an object can have.
					@return  list<GLObject*>& mutable reference of the transparent always front list of {\em *this} glObjectCollector
					@see    appendToTransparentAlwaysFrontList
					@see    GLObject
					@see    GeometricObject
			*/
			list<GLObject *>& getTransparentAlwaysFrontList()
				throw();

			/** Non-mutable inspection of the transparent always front list.
					For further information see \Ref{getTransparentAlwaysFrontList}.
			*/
			const list<GLObject *>& getTransparentAlwaysFrontList() const
				throw();

			/** Change the root composite.
					Change the root \Ref{Composite} object of {\em *this} glObjectCollector.
					This method is used internally from the \Ref{CompositeDescriptor} object. In the
					\Ref{CompositeDescriptor} the \Ref{Composite} object that {\em *this}
					glObjectCollector is applied on is stored with this method for later use.
					@param  composite pointer to the new root \Ref{Composite} object of {\em *this} glObjectCollector
					@see    getRootComposite
					@see    Composite
			*/
			void setRootComposite(const Composite* composite)
				throw();

			/** Inspection of the root composite.
					Access the root \Ref{Composite} object of {\em *this} glObjectCollector.
					@return  Composite* the constant pointer to the root \Ref{Composite} object of {\em *this} glObjectCollector
					@see     setRootComposite
					@see     Composite
			*/
			const Composite* getRootComposite() const
				throw();

			//@}

			/**	@name Processor specific methods
			*/
			//@{
			/** Start method.
					Calls \Ref{clear}.
					@return bool {\tt true} if the start of {\em *this} glObjectCollector was successful, {\tt false} otherwise
			*/
			virtual bool start()
				throw();
			
			/** Finish method.
					Empty for further purpose. Override this method for finalizing
					the glObjectCollector.
					@return bool {\tt true} if the finish of {\em *this} glObjectCollector was successful, {\tt false} otherwise
			*/
			virtual bool finish()
				throw();
			
			/**	Operator ().
					Traverse the \Ref{Composite} tree with the start \Ref{Composite} {\em composite}
					and search for \Ref{GeometricObject} objects that are also of kind \Ref{GLObject}.
					According to their property they will be collected into the appropriate lists.
					@param  composite the \Ref{Composite} to be searched for \Ref{GLObject} objects
					@return Processor::Result the result of {\em *this} glObjectCollector
					@see    appendToStaticList
					@see    appendToStaticAlwaysFrontList
					@see    appendToStaticWireframeList
					@see    appendToStaticWireframeAlwaysFrontList
					@see    appendToDynamicList
					@see    appendToDynamicAlwaysFrontList
					@see    appendToTransparentList
					@see    appendToTransparentAlwaysFrontList
					@see    start
					@see    finish
					@see    GeometricObject
					@see    Processor
					@see    Composite
			*/
			virtual Processor::Result operator() (Composite& composite)
				throw();

			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{
			/** Internal value dump.
					Dump the current internal state of {\em *this} glObjectCollector to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the internal state of {\em *this} glObjectCollector
					@param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			//@}

			private:

			Composite* composite_;

			list<GLObject *> static_list_;
			list<GLObject *> static_always_front_list_;
			list<GLObject *> static_wireframe_list_;
			list<GLObject *> static_wireframe_always_front_list_;

			list<GLObject *> dynamic_list_;
			list<GLObject *> dynamic_always_front_list_;

			list<GLObject *> transparent_list_;
			list<GLObject *> transparent_always_front_list_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/FUNCTOR/glObjectCollector.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_FUNCTOR_GLOBJECTCOLLECTOR_H 

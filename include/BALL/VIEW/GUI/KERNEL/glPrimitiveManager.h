// $Id: glPrimitiveManager.h,v 1.5 2001/05/13 13:57:02 hekl Exp $

#ifndef BALL_VIEW_GUI_KERNEL_GLPRIMITIVEMANAGER_H
#define BALL_VIEW_GUI_KERNEL_GLPRIMITIVEMANAGER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLDISPLAYLIST_H
#	include <BALL/VIEW/GUI/KERNEL/glDisplayList.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLQUADRICOBJECT_H
#	include <BALL/VIEW/GUI/KERNEL/glQuadricObject.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#define BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE 12

namespace BALL
{

	namespace VIEW
	{
		/**	GLDisplayListObject_ class.
				{\bf Framework:} BALL/VIEW/GUI/KERNEL\\
				{\bf Definition:}\URL{BALL/VIEW/GUI/KERNEL/glPrimitiveManager.h}\\ \\
				The class GLDisplayListObject_ is a base class for the concrete display list
				classes \Ref{GLSphereDisplayLists_}, \Ref{GLTubeDisplayLists_} and
				\Ref{GLSimpleBoxDisplayLists_}. It defines an interface that every display
				list class must have.
				This is an internally used class and should not be used in any programs.
				@memo    GLDisplayListObject_ class (BALL VIEW gui kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.5 $
				@date    $Date: 2001/05/13 13:57:02 $
		*/
		class GLDisplayListObject_
		{
			/** @name Class friends

					\begin{itemize}
						\item class GeometricObject
					\end{itemize}
			*/
			
			friend class GeometricObject;

			public:

			/**	@name	Exceptions
			*/
			//@{
			
			/** NoGLDisplayListsAvailable Exception class.
					This exeption will be thrown if no memory for \Ref{GLDisplayList} objects
					can be allocated. This exception will be used by derived classes.
					@see         GeneralException			
					@see         GLDisplayList
			*/
			class NoGLDisplayListsAvailable:	public Exception::GeneralException
			{
				public:

				NoGLDisplayListsAvailable(const char* file, int line)
					throw();
			};

			/** WrongModes Exception class.
					This exeption will be thrown if the {\em drawing_precision} or
					{\em drawing_mode} of the method \Ref{GLDisplayListObject_::operator()} are
					not allowed. Each derived class will determine which modes are allowed or not.
					@see         GeneralException			
			*/
			class WrongModes:	public Exception::GeneralException
			{
				public:

				WrongModes(const char* file, int line)
					throw();
			};
			//@}

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new glDisplayListObject_.
					@return      GLDisplayListObject_ new constructed glDisplayListObject_
			*/
			GLDisplayListObject_()
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glDisplayListObject_.
			*/
			virtual ~GLDisplayListObject_()
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Return a glDisplayList (Pure virtual method).
					This method must be overridden from the derived classes. They must implement
					this method. 
					@param   drawing_mode the drawing mode the \Ref{GLDisplayList} should have
					@param   drawing_precision the drawing precision the \Ref{GLDisplayList} should have
					@return  GLDisplayList& a reference to a \Ref{GLDisplayList}
					@see     getDrawingModeAndPrecision
			*/
			virtual GLDisplayList& operator ()
				(unsigned int drawing_mode, unsigned int drawing_precision)
				throw() = 0;
			
			/** Return a glDisplayList.
					Calls \Ref{operator()}
					@param   drawing_mode the drawing mode the \Ref{GLDisplayList} should have
					@param   drawing_precision the drawing precision the \Ref{GLDisplayList} should have
					@return  const GLDisplayList& a constant reference to a \Ref{GLDisplayList}
					@see     getDrawingModeAndPrecision
			*/
			const GLDisplayList& operator ()
				(unsigned int drawing_mode, unsigned int drawing_precision) const
				throw();
			//@}
		};


		/**	GLSphereDisplayLists_ class.
				{\bf Framework:} BALL/VIEW/GUI/KERNEL\\
				{\bf Definition:}\URL{BALL/VIEW/GUI/KERNEL/glPrimitiveManager.h}\\ \\
				The class GLSphereDisplayLists_ is the graphical representation of the
				geometric primitive "sphere".
				It is derived from the class \Ref{GLDisplayListObject_} and creates the
				different graphical visualizations of a sphere.
				This is an internally used class and should not be used in any programs.
				@memo    GLSphereDisplayLists_ class (BALL VIEW gui kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.5 $
				@date    $Date: 2001/05/13 13:57:02 $
		*/
		class GLSphereDisplayLists_: public GLDisplayListObject_
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new glSphereDisplayList_.
					The state of {\em *this} glSphereDisplayList_ is:
					\begin{itemize}
					  \item no display lists allocated
					\end{itemize}
			*/
 			GLSphereDisplayLists_()
				throw();
			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glSphereDisplayList_.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~GLSphereDisplayLists_()
				throw();

			/** Explicit destructor.
					Destroy {\em *this} glSphereDisplayList_.
					Deletes all allocated \Ref{GLDisplayList} objects.
					@see GLDisplayList
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Initialize the sphere display lists.
					This method creates the \Ref{GLDisplayList} objects that contain
					the graphical representation of the geometric primitive "sphere" in various
					resolutions and drawing modes.\\
					{\bf Note:} this method will be called by the class \Ref{GLPrimitiveManager}.
					@exception  NoGLDisplayListsAvailable thrown if the memory allocation for the needed \Ref{GLDisplayList} objects failed
					@see GLDisplayList
			*/
			void init()
				throw(NoGLDisplayListsAvailable);

			/** Return a glDisplayList.
					Return a \Ref{GLDisplayList} containing a sphere with the given graphical 
					properties {\em drawing_mode} and {\em drawing_precision}.
					@param   drawing_mode the drawing mode the \Ref{GLDisplayList} the sphere should have
					@param   drawing_precision the drawing precision the \Ref{GLDisplayList} the sphere should have
					@return  GLDisplayList& a reference to a \Ref{GLDisplayList} containing a sphere
					@exception WrongModes thrown if the {\em drawing_mode} or {\em drawing_precision} are not allowed
					@see     getDrawingModeAndPrecision
			*/
			GLDisplayList& operator () (unsigned int drawing_mode, unsigned int drawing_precision)
				throw(WrongModes);
			//@}

			/**	@name	debuggers and diagnostics
			*/
			//@{
			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} glSphereDisplayLists_.
					If all allocated \Ref{GLDisplayList} objects for {\em *this}
					glSphereDisplayLists_ are valid	{\tt true} is returned, {\tt false} otherwise. 
					Calls {GLDisplayList::isValid}.
					@return			bool {\tt true} if the internal state of {\em *this} glSphereDisplayLists_ is correct (self-validated) and consistent,
					 						{\tt false} otherwise
					@see        GLDisplayList::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of {\em *this} glSphereDisplayLists_ to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Dumps all \Ref{GLDisplayList} objects allocated for {\em *this}
					glSphereDisplayLists_.
					@param   s output stream where to output the state of {\em *this} glSphereDisplayLists_
					@param   depth the dumping depth
					@see     GLDisplayList::dump
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}


			private:

			void create_()
				throw();

			/* builts the sphere dots display lists */
			void buildDottedSphere_(int precision)
				throw();

			/* help functions for sphere dots */
			void drawPoint_(Vector3& v)
				throw();

			/* help functions for sphere dots */
			void subdivideTriangle_
				(Vector3& v1, Vector3& v2, Vector3& v3, int precision)
				throw();

			/* the display lists */
			GLDisplayList* GL_display_list_;

			/* static array of vertices for sphere dots */
			static const float vertices_[12][3];
			static const int indices_[20][3];
		};



		/**	GLTubeDisplayLists_ class.
				{\bf Framework:} BALL/VIEW/GUI/KERNEL\\
				{\bf Definition:}\URL{BALL/VIEW/GUI/KERNEL/glPrimitiveManager.h}\\ \\
				The class GLTubeDisplayLists_ is the graphical representation of the
				geometric primitive "tube".
				It is derived from the class \Ref{GLDisplayListObject_} and creates the
				different graphical visualizations of a tube.
				This is an internally used class and should not be used in any programs.
				@memo    GLTubeDisplayLists_ class (BALL VIEW gui kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.5 $
				@date    $Date: 2001/05/13 13:57:02 $
		*/
		class GLTubeDisplayLists_: public GLDisplayListObject_
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new glTubeDisplayList_.
					The state of {\em *this} glTubeDisplayList_ is:
					\begin{itemize}
					  \item no display lists allocated
					\end{itemize}
			*/
			GLTubeDisplayLists_()
				throw();
			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glTubeDisplayList_.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~GLTubeDisplayLists_()
				throw();

			/** Explicit destructor.
					Destroy {\em *this} glTubeDisplayList_.
					Deletes all allocated \Ref{GLDisplayList} objects.
					@see GLDisplayList
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Initialize the tube display lists.
					This method creates the \Ref{GLDisplayList} objects that contain
					the graphical representation of the geometric primitive "tube" in various
					resolutions and drawing modes.\\
					{\bf Note:} this method will be called by the class \Ref{GLPrimitiveManager}.
					@exception  NoGLDisplayListsAvailable thrown if the memory allocation for the needed \Ref{GLDisplayList} objects failed
					@see GLDisplayList
			*/
			void init()
				throw(NoGLDisplayListsAvailable);

			/** Return a glDisplayList.
					Return a \Ref{GLDisplayList} containing a tube with the given graphical 
					properties {\em drawing_mode} and {\em drawing_precision}.
					@param   drawing_mode the drawing mode the \Ref{GLDisplayList} the tube should have
					@param   drawing_precision the drawing precision the \Ref{GLDisplayList} the tube should have
					@return  GLDisplayList& a reference to a \Ref{GLDisplayList} containing a tube
					@exception WrongModes thrown if the {\em drawing_mode} or {\em drawing_precision} are not allowed
					@see     getDrawingModeAndPrecision
			*/
			GLDisplayList& operator () 
				(unsigned int drawing_mode, unsigned int drawing_precision)
				throw(WrongModes);
			//@}

			/**	@name	debuggers and diagnostics
			*/
			//@{
			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} glTubeDisplayLists_.
					If all allocated \Ref{GLDisplayList} objects for {\em *this}
					glTubeDisplayLists_ are valid	{\tt true} is returned, {\tt false} otherwise. 
					Calls {GLDisplayList::isValid}.
					@return			bool {\tt true} if the internal state of {\em *this} glTubeDisplayLists_ is correct (self-validated) and consistent,
					 						{\tt false} otherwise
					@see        GLDisplayList::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of {\em *this} glTubeDisplayLists_ to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Dumps all \Ref{GLDisplayList} objects allocated for {\em *this}
					glTubeDisplayLists_.
					@param   s output stream where to output the state of {\em *this} glTubeDisplayLists_
					@param   depth the dumping depth
					@see     GLDisplayList::dump
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}


			private:

			void create_()
				throw();

			/* the display lists */
			GLDisplayList* GL_display_list_;
		};


		/**	GLSimpleBoxDisplayLists_ class.
				{\bf Framework:} BALL/VIEW/GUI/KERNEL\\
				{\bf Definition:}\URL{BALL/VIEW/GUI/KERNEL/glPrimitiveManager.h}\\ \\
				The class GLSimpleBoxDisplayLists_ is the graphical representation of the
				geometric primitive "simpleBox".
				It is derived from the class \Ref{GLDisplayListObject_} and creates the
				different graphical visualizations of a simpleBox.
				This is an internally used class and should not be used in any programs.
				@memo    GLSimpleBoxDisplayLists_ class (BALL VIEW gui kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.5 $
				@date    $Date: 2001/05/13 13:57:02 $
		*/
		class GLSimpleBoxDisplayLists_: public GLDisplayListObject_
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new glSimpleBoxDisplayList_.
					The state of {\em *this} glSimpleBoxDisplayList_ is:
					\begin{itemize}
					  \item no display lists allocated
					\end{itemize}
			*/
			GLSimpleBoxDisplayLists_()
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glSimpleBoxDisplayList_.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~GLSimpleBoxDisplayLists_()
				throw();

			/** Explicit destructor.
					Destroy {\em *this} glSimpleBoxDisplayList_.
					Deletes all allocated \Ref{GLDisplayList} objects.
					@see GLDisplayList
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Initialize the simpleBox display lists.
					This method creates the \Ref{GLDisplayList} objects that contain
					the graphical representation of the geometric primitive "simpleBox" in various
					resolutions and drawing modes.\\
					{\bf Note:} this method will be called by the class \Ref{GLPrimitiveManager}.
					@exception  NoGLDisplayListsAvailable thrown if the memory allocation for the needed \Ref{GLDisplayList} objects failed
					@see GLDisplayList
			*/
			void init()
				throw(NoGLDisplayListsAvailable);

			/** Return a glDisplayList.
					Return a \Ref{GLDisplayList} containing a simpleBox with the given graphical 
					properties {\em drawing_mode} and {\em drawing_precision}.
					@param   drawing_mode the drawing mode the \Ref{GLDisplayList} the simpleBox should have
					@param   drawing_precision the drawing precision the \Ref{GLDisplayList} the simpleBox should have
					@return  GLDisplayList& a reference to a \Ref{GLDisplayList} containing a simpleBox
					@exception WrongModes thrown if the {\em drawing_mode} or {\em drawing_precision} are not allowed
					@see     getDrawingModeAndPrecision
			*/
			GLDisplayList& operator ()
				(unsigned int drawing_mode, unsigned int drawing_precision)
				throw(WrongModes);
			//@}

			/**	@name	debuggers and diagnostics
			*/
			//@{
			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} glSimpleBoxDisplayLists_.
					If all allocated \Ref{GLDisplayList} objects for {\em *this}
					glSimpleBoxDisplayLists_ are valid	{\tt true} is returned, {\tt false} otherwise. 
					Calls {GLDisplayList::isValid}.
					@return			bool {\tt true} if the internal state of {\em *this} glSimpleBoxDisplayLists_ is correct (self-validated) and consistent,
					 						{\tt false} otherwise
					@see        GLDisplayList::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of {\em *this} glSimpleBoxDisplayLists_ to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Dumps all \Ref{GLDisplayList} objects allocated for {\em *this}
					glSimpleBoxDisplayLists_.
					@param   s output stream where to output the state of {\em *this} glSimpleBoxDisplayLists_
					@param   depth the dumping depth
					@see     GLDisplayList::dump
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}


			private:

			void create_()
				throw();

			void createDotBox_()
				throw();
			void createLineBox_()
				throw();
			void createSolidBox_()
				throw();

			/* the display lists */
			GLDisplayList* GL_display_list_;
		};



		/**	GLPrimitiveManager class.
				{\bf Framework:} BALL/VIEW/GUI/KERNEL\\
				{\bf Definition:}\URL{BALL/VIEW/GUI/KERNEL/glPrimitiveManager.h}\\ \\
				The class GLPrimitiveManager is container for the predefined primitives
				\Ref{GLSphereDisplayLists_}, \Ref{GLTubeDisplayLists_} and \Ref{GLSimpleBoxDisplayLists_}.
				Further it provides methods for naming and accessing \Ref{GeometricObject} objects.
				This mechanism is used by the class \Ref{Scene} to name all primitives to identify them
				when picked.
				@memo    GLPrimitiveManager class (BALL VIEW gui kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.5 $
				@date    $Date: 2001/05/13 13:57:02 $
		*/
		class GLPrimitiveManager
		{
			public:

			/** @name Type Definitions
			*/
			//@{
			typedef unsigned int Name;
			//@}

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new glPrimitiveManager.
					The state of {\em *this} glPrimitiveManager is:
					\begin{itemize}
					  \item sphere display lists empty
					  \item tube display lists empty
					  \item simpleBox display lists empty
					  \item name management empty
					\end{itemize}
					@see     GLSphereDisplayLists_
					@see     GLTubeDisplayLists_
					@see     GLSimpleBoxDisplayLists_
			*/
			GLPrimitiveManager()
				throw();
			
			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glPrimitiveManager.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~GLPrimitiveManager()
				throw();

			/** Explicit destructor.
					Destroy {\em *this} glPrimitiveManager.
					Calls \Ref{GLSphereDisplayLists_::destroy}
					Calls \Ref{GLTubeDisplayLists_::destroy}
					Calls \Ref{GLSimpleBoxDisplayLists_::destroy}
					@see     GLSphereDisplayLists_::destroy
					@see     GLTubeDisplayLists_::destroy
					@see     GLSimpleBoxDisplayLists_::destroy
			*/
			virtual void destroy()
				throw();
			//@}
		
			/**	@name	Accessors
			*/
			//@{

			/** Initialization.
					Initializes all display list object of {\em *this} glPrimitiveManager.				
					Calls \Ref{GLSphereDisplayLists_::init}
					Calls \Ref{GLTubeDisplayLists_::init}
					Calls \Ref{GLSimpleBoxDisplayLists_::init}
					@see     GLSphereDisplayLists_::init
					@see     GLTubeDisplayLists_::init
					@see     GLSimpleBoxDisplayLists_::init
			*/
			void init()
				throw();

			/** Reset name management.
					Clears all previously assigned names. 
					@see    getObject
					@see    getName
			*/
			void clearNames()
				throw();

			/** Retrieve a name for a geometricObject.
					This method is called internally whenever a \Ref{GeometricObject} needs a
					name that is unique.
					If the \Ref{GeometricObject} {\em object} is first used with this method a new
					unique name will be created. If it was already named that name will be returned.
					A name is a normal integer value that will start at {\tt 0} and will be increased
					for each new \Ref{GeometricObject} that is used with this method.
					@param  object the \Ref{GeometricObject} that should be named.
					@return Name a name for the \Ref{GeometricObject} {\em object}
					@see    getObject
			*/
			Name getName(const GeometricObject& object)
				throw();

			/** Access the geometricObject of a name.
					This method is called internally whenever a \Ref{GeometricObject} is searched by
					the parameter {\em name}.
					If {\em name} has no associated \Ref{GeometricObject} {\tt 0} will be returned.
					@param   Name a name for a \Ref{GeometricObject}
					@return  GeometricObject* the \Ref{GeometricObject} that is found with the parameter {\em name} ({\tt 0} is returned if no such \Ref{GeometricObject} exists)
					@see    getName
			*/
			GeometricObject* getObject(Name name) const
				throw();
			//@}

			/** @name Predefined display lists.
			*/		
			//@{

			/** Sphere display list.
					See \Ref{GLSphereDisplayLists_} for further information about the sphere primitive.
					@see  GLSphereDisplayLists_
			*/
			GLSphereDisplayLists_ Sphere;

			/** Tube display list.
					See \Ref{GLTubeDisplayLists_} for further information about the tube primitive.
					@see  GLTubeDisplayLists_
			*/
			GLTubeDisplayLists_ Tube;

			/** SimpleBox display list.
					See \Ref{GLSimpleBoxDisplayLists_} for further information about the
					simpleBox primitive.
					@see  GLSimpleBoxDisplayLists_
			*/
			GLSimpleBoxDisplayLists_ SimpleBox;
			//@}

			/**	@name	debuggers and diagnostics
			*/
			//@{
			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} glPrimitiveManager.
					Calls \Ref{GLSphereDisplayLists_::isValid}
					Calls \Ref{GLTubeDisplayLists_::isValid}
					Calls \Ref{GLSimpleBoxDisplayLists_::isValid}
					@return			bool {\tt true} if the internal state of {\em *this} glPrimitiveManager is correct (self-validated) and consistent,
					 						{\tt false} otherwise
					@see     GLSphereDisplayLists_::isValid
					@see     GLTubeDisplayLists_::isValid
					@see     GLSimpleBoxDisplayLists_::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of {\em *this} glPrimitiveManager to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls \Ref{GLSphereDisplayLists_::dump}
					Calls \Ref{GLTubeDisplayLists_::dump}
					Calls \Ref{GLSimpleBoxDisplayLists_::dump}
					@param   s output stream where to output the state of {\em *this} glPrimitiveManager
					@param   depth the dumping depth
					@see     GLSphereDisplayLists_::dump
					@see     GLTubeDisplayLists_::dump
					@see     GLSimpleBoxDisplayLists_::dump
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			/**	@name	Storers
			*/
			//@{

			/** Persistent stream output and state restorage.
  			  Read persistent glPrimitiveManager data from the input stream {\em s} and 
				  restore the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s input stream from where to restore the internal state of {\em *this} glPrimitiveManager
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			  Write persistent glPrimitiveManager data to the output stream {\em s} and 
				  store the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s output stream to where to store the internal state of {\em *this} glPrimitiveManager
			*/
			virtual void write(std::ostream& s) const
				throw();
			//@}

			private:

			typedef HashMap<GeometricObject*, Name> NameHashMap;
			typedef HashMap<Name, GeometricObject*> GeometricObjectHashMap;

			GeometricObjectHashMap name_to_object_;
			NameHashMap object_to_name_;

			Name all_names_;
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/glPrimitiveManager.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_KERNEL_GLPRIMITIVEMANAGER_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glPrimitiveManager.h,v 1.15 2003/03/14 11:50:14 sturm Exp $

#ifndef BALL_VIEW_GUI_KERNEL_GLPRIMITIVEMANAGER_H
#define BALL_VIEW_GUI_KERNEL_GLPRIMITIVEMANAGER_H

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLDISPLAYLIST_H
#	include <BALL/VIEW/GUI/KERNEL/glDisplayList.h>
#endif

#define BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE 12

namespace BALL
{
	namespace VIEW
	{
    /**  \addtogroup  ViewGuiKernelOpenGLPredefined
     *  @{
     */
		/**	GLDisplayListObject_ class.
				The class GLDisplayListObject_ is a base class for the concrete display list
				classes  \link GLSphereDisplayLists_ GLSphereDisplayLists_ \endlink ,  \link GLTubeDisplayLists_ GLTubeDisplayLists_ \endlink  and
				 \link GLSimpleBoxDisplayLists_ GLSimpleBoxDisplayLists_ \endlink . It defines an interface that every display
				list class must have.
				This is an internally used class and should not be used in any programs.  \par
				
		*/
		class GLDisplayListObject_
		{
			public:

			/**	@name	Exceptions
			*/
			//@{
			
			/** NoGLDisplayListsAvailable Exception class.
					This exeption will be thrown if no memory for  \link GLDisplayList GLDisplayList \endlink  objects
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
					{\em drawing_mode} of the method  \link GLDisplayListObject_::operator() GLDisplayListObject_::operator() \endlink  are
					not allowed. Each derived class will determine which modes are allowed or not.
					@see         GeneralException			
			*/
			class WrongModes:	public Exception::GeneralException
			{
				public:

				WrongModes(const char* file, int line, int mode, int precision)
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
					This method must be overridden from the derived classes. They must implement this method. 
					@param   drawing_mode the drawing mode the  \link GLDisplayList GLDisplayList \endlink  should have
					@param   drawing_precision the drawing precision the  \link GLDisplayList GLDisplayList \endlink  should have
					@return  GLDisplayList& a reference to a  \link GLDisplayList GLDisplayList \endlink 
					@see     getDrawingModeAndPrecision
			*/
			virtual GLDisplayList& operator () (unsigned int drawing_mode, unsigned int drawing_precision)
				throw(WrongModes) = 0;
			
			/** Return a glDisplayList.
					Calls  \link operator() operator() \endlink 
					@param   drawing_mode the drawing mode the  \link GLDisplayList GLDisplayList \endlink  should have
					@param   drawing_precision the drawing precision the  \link GLDisplayList GLDisplayList \endlink  should have
					@return  const GLDisplayList& a constant reference to a  \link GLDisplayList GLDisplayList \endlink 
					@see     getDrawingModeAndPrecision
			*/
			const GLDisplayList& operator () (unsigned int drawing_mode, unsigned int drawing_precision) const
				throw(WrongModes);
			//@}
		};


		/**	GLSphereDisplayLists_ class.
				The class GLSphereDisplayLists_ is the graphical representation of the
				geometric primitive "sphere".
				It is derived from the class  \link GLDisplayListObject_ GLDisplayListObject_ \endlink  and creates the
				different graphical visualizations of a sphere.
				This is an internally used class and should not be used in any programs.  \par
				
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

					  - no display lists allocated
					
			*/
 			GLSphereDisplayLists_()
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glSphereDisplayList_.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~GLSphereDisplayLists_()
				throw();

			/** Explicit destructor.
					Destroy {\em *this} glSphereDisplayList_.
					Deletes all allocated  \link GLDisplayList GLDisplayList \endlink  objects.
					@see GLDisplayList
			*/
			virtual void destroy()
				throw();

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Initialize the sphere display lists.
					This method creates the  \link GLDisplayList GLDisplayList \endlink  objects that contain
					the graphical representation of the geometric primitive "sphere" in various
					resolutions and drawing modes. \par
					<b>Note:</b> this method will be called by the class  \link GLPrimitiveManager GLPrimitiveManager \endlink .
					@exception  NoGLDisplayListsAvailable thrown if the memory allocation for the needed  \link GLDisplayList GLDisplayList \endlink  
					  					objects failed
					@see GLDisplayList
			*/
			void init()
				throw(NoGLDisplayListsAvailable);

			/** Return a glDisplayList.
					Return a  \link GLDisplayList GLDisplayList \endlink  containing a sphere with the given graphical 
					properties {\em drawing_mode} and {\em drawing_precision}.
					@param   drawing_mode the drawing mode the  \link GLDisplayList GLDisplayList \endlink  the sphere should have
					@param   drawing_precision the drawing precision the  \link GLDisplayList GLDisplayList \endlink  the sphere should have
					@return  GLDisplayList& a reference to a  \link GLDisplayList GLDisplayList \endlink  containing a sphere
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
					If all allocated  \link GLDisplayList GLDisplayList \endlink  objects for {\em *this}
					glSphereDisplayLists_ are valid	<tt>true</tt> is returned, <tt>false</tt> otherwise. 
					Calls {GLDisplayList::isValid}.
					@return			bool <tt>true</tt> if the internal state of {\em *this} glSphereDisplayLists_ is correct 
					@see        GLDisplayList::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of {\em *this} glSphereDisplayLists_ to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Dumps all  \link GLDisplayList GLDisplayList \endlink  objects allocated for {\em *this}
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
			void subdivideTriangle_(Vector3& v1, Vector3& v2, Vector3& v3, int precision)
				throw();

			/* the BALL_VIEW_MAXIMAL_DISPLAY_LIST_OBJECT_SIZE (12) display lists
			 	 (drawing_mode[3] * drawing_precision[4] = 12)
			*/
			GLDisplayList* GL_display_list_;

			/* static array of vertices for sphere dots */
			static const float vertices_[12][3];
			static const int indices_[20][3];
		};


		/**	GLTubeDisplayLists_ class.
				The class GLTubeDisplayLists_ is the graphical representation of the
				geometric primitive "tube".
				It is derived from the class  \link GLDisplayListObject_ GLDisplayListObject_ \endlink  and creates the
				different graphical visualizations of a tube.
				This is an internally used class and should not be used in any programs.  \par
				
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

					  - no display lists allocated
					
			*/
			GLTubeDisplayLists_()
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glTubeDisplayList_.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~GLTubeDisplayLists_()
				throw();

			/** Explicit destructor.
					Destroy {\em *this} glTubeDisplayList_.
					Deletes all allocated  \link GLDisplayList GLDisplayList \endlink  objects.
					@see GLDisplayList
			*/
			virtual void destroy()
				throw();

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Initialize the tube display lists.
					This method creates the  \link GLDisplayList GLDisplayList \endlink  objects that contain
					the graphical representation of the geometric primitive "tube" in various
					resolutions and drawing modes. \par
					<b>Note:</b> this method will be called by the class  \link GLPrimitiveManager GLPrimitiveManager \endlink .
					@exception  NoGLDisplayListsAvailable thrown if the memory allocation for the needed  \link GLDisplayList GLDisplayList \endlink  
					  					objects failed
					@see GLDisplayList
			*/
			void init()
				throw(NoGLDisplayListsAvailable);

			/** Return a glDisplayList.
					Return a  \link GLDisplayList GLDisplayList \endlink  containing a tube with the given graphical 
					properties {\em drawing_mode} and {\em drawing_precision}.
					@param   drawing_mode the drawing mode the  \link GLDisplayList GLDisplayList \endlink  the tube should have
					@param   drawing_precision the drawing precision the  \link GLDisplayList GLDisplayList \endlink  the tube should have
					@return  GLDisplayList& a reference to a  \link GLDisplayList GLDisplayList \endlink  containing a tube
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
					of {\em *this} glTubeDisplayLists_.
					If all allocated  \link GLDisplayList GLDisplayList \endlink  objects for {\em *this}
					glTubeDisplayLists_ are valid	<tt>true</tt> is returned, <tt>false</tt> otherwise. 
					Calls {GLDisplayList::isValid}.
					@return			bool <tt>true</tt> if the internal state of {\em *this} glTubeDisplayLists_ is correct
					@see        GLDisplayList::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of {\em *this} glTubeDisplayLists_ to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Dumps all  \link GLDisplayList GLDisplayList \endlink  objects allocated for {\em *this}
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
				The class GLSimpleBoxDisplayLists_ is the graphical representation of the
				geometric primitive "simpleBox".
				It is derived from the class  \link GLDisplayListObject_ GLDisplayListObject_ \endlink  and creates the
				different graphical visualizations of a simpleBox.
				This is an internally used class and should not be used in any programs.  \par
				
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

					  - no display lists allocated
					
			*/
			GLSimpleBoxDisplayLists_()
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glSimpleBoxDisplayList_.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~GLSimpleBoxDisplayLists_()
				throw();

			/** Explicit destructor.
					Destroy {\em *this} glSimpleBoxDisplayList_.
					Deletes all allocated  \link GLDisplayList GLDisplayList \endlink  objects.
					@see GLDisplayList
			*/
			virtual void destroy()
				throw();

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Initialize the simpleBox display lists.
					This method creates the  \link GLDisplayList GLDisplayList \endlink  objects that contain
					the graphical representation of the geometric primitive "simpleBox" in various
					resolutions and drawing modes. \par
					<b>Note:</b> this method will be called by the class  \link GLPrimitiveManager GLPrimitiveManager \endlink .
					@exception  NoGLDisplayListsAvailable thrown if the memory allocation for the needed  \link GLDisplayList GLDisplayList \endlink  
						  				objects failed
					@see GLDisplayList
			*/
			void init()
				throw(NoGLDisplayListsAvailable);

			/** Return a glDisplayList.
					Return a  \link GLDisplayList GLDisplayList \endlink  containing a simpleBox with the given graphical 
					properties {\em drawing_mode} and {\em drawing_precision}.
					@param   drawing_mode the drawing mode the  \link GLDisplayList GLDisplayList \endlink  the simpleBox should have
					@param   drawing_precision the drawing precision the  \link GLDisplayList GLDisplayList \endlink  the simpleBox should have
					@return  GLDisplayList& a reference to a  \link GLDisplayList GLDisplayList \endlink  containing a simpleBox
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
					If all allocated  \link GLDisplayList GLDisplayList \endlink  objects for {\em *this}
					glSimpleBoxDisplayLists_ are valid	<tt>true</tt> is returned, <tt>false</tt> otherwise. 
					Calls {GLDisplayList::isValid}.
					@return			bool <tt>true</tt> if the internal state of {\em *this} glSimpleBoxDisplayLists_ is correct
					@see        GLDisplayList::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of {\em *this} glSimpleBoxDisplayLists_ to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Dumps all  \link GLDisplayList GLDisplayList \endlink  objects allocated for {\em *this}
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
				The class GLPrimitiveManager is container for the predefined primitives
				 \link GLSphereDisplayLists_ GLSphereDisplayLists_ \endlink ,  \link GLTubeDisplayLists_ GLTubeDisplayLists_ \endlink  and  \link GLSimpleBoxDisplayLists_ GLSimpleBoxDisplayLists_ \endlink .
				Further it provides methods for naming and accessing  \link GeometricObject GeometricObject \endlink  objects.
				This mechanism is used by the class  \link Scene Scene \endlink  to name all primitives to identify them
				when picked.  \par
				
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

					  - sphere display lists empty
					  - tube display lists empty
					  - simpleBox display lists empty
					  - name management empty
					
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
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~GLPrimitiveManager()
				throw();

			/** Explicit destructor.
					Destroy {\em *this} glPrimitiveManager.
					Calls  \link GLSphereDisplayLists_::destroy GLSphereDisplayLists_::destroy \endlink 
					Calls  \link GLTubeDisplayLists_::destroy GLTubeDisplayLists_::destroy \endlink 
					Calls  \link GLSimpleBoxDisplayLists_::destroy GLSimpleBoxDisplayLists_::destroy \endlink 
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
					Calls  \link GLSphereDisplayLists_::init GLSphereDisplayLists_::init \endlink 
					Calls  \link GLTubeDisplayLists_::init GLTubeDisplayLists_::init \endlink 
					Calls  \link GLSimpleBoxDisplayLists_::init GLSimpleBoxDisplayLists_::init \endlink 
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
					This method is called internally whenever a  \link GeometricObject GeometricObject \endlink  needs a
					name that is unique.
					If the  \link GeometricObject GeometricObject \endlink  {\em object} is first used with this method a new
					unique name will be created. If it was already named that name will be returned.
					A name is a normal integer value that will start at <tt>0</tt> and will be increased
					for each new  \link GeometricObject GeometricObject \endlink  that is used with this method.
					@param  object the  \link GeometricObject GeometricObject \endlink  that should be named.
					@return Name a name for the  \link GeometricObject GeometricObject \endlink  {\em object}
					@see    getObject
			*/
			Name getName(const GeometricObject& object)
				throw();

			/** Access the geometricObject of a name.
					This method is called internally whenever a  \link GeometricObject GeometricObject \endlink  is searched by
					the parameter {\em name}.
					If {\em name} has no associated  \link GeometricObject GeometricObject \endlink  <tt>0</tt> will be returned.
					@param   Name a name for a  \link GeometricObject GeometricObject \endlink 
					@return  GeometricObject* the  \link GeometricObject GeometricObject \endlink  that is found with the parameter {\em name} 
										(<tt>0</tt> is returned if no such  \link GeometricObject GeometricObject \endlink  exists)
					@see    getName
			*/
			GeometricObject* getObject(Name name) const
				throw();

			//@}
			/** @name Predefined display lists.
			*/		
			//@{

			/** Sphere display list.
					See  \link GLSphereDisplayLists_ GLSphereDisplayLists_ \endlink  for further information about the sphere primitive.
					@see  GLSphereDisplayLists_
			*/
			GLSphereDisplayLists_ Sphere;

			/** Tube display list.
					See  \link GLTubeDisplayLists_ GLTubeDisplayLists_ \endlink  for further information about the tube primitive.
					@see  GLTubeDisplayLists_
			*/
			GLTubeDisplayLists_ Tube;

			/** SimpleBox display list.
					See  \link GLSimpleBoxDisplayLists_ GLSimpleBoxDisplayLists_ \endlink  for further information about the
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
					Calls  \link GLSphereDisplayLists_::isValid GLSphereDisplayLists_::isValid \endlink 
					Calls  \link GLTubeDisplayLists_::isValid GLTubeDisplayLists_::isValid \endlink 
					Calls  \link GLSimpleBoxDisplayLists_::isValid GLSimpleBoxDisplayLists_::isValid \endlink 
					@return			bool <tt>true</tt> if the internal state of {\em *this} glPrimitiveManager is correct
					@see     GLSphereDisplayLists_::isValid
					@see     GLTubeDisplayLists_::isValid
					@see     GLSimpleBoxDisplayLists_::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of {\em *this} glPrimitiveManager to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Calls  \link GLSphereDisplayLists_::dump GLSphereDisplayLists_::dump \endlink 
					Calls  \link GLTubeDisplayLists_::dump GLTubeDisplayLists_::dump \endlink 
					Calls  \link GLSimpleBoxDisplayLists_::dump GLSimpleBoxDisplayLists_::dump \endlink 
					@param   s output stream where to output the state of {\em *this} glPrimitiveManager
					@param   depth the dumping depth
					@see     GLSphereDisplayLists_::dump
					@see     GLTubeDisplayLists_::dump
					@see     GLSimpleBoxDisplayLists_::dump
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			//@}

			private:

			typedef HashMap<GeometricObject*, Name> NameHashMap;
			typedef HashMap<Name, GeometricObject*> GeometricObjectHashMap;

			GeometricObjectHashMap	name_to_object_;
			NameHashMap							object_to_name_;

			Name all_names_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/glPrimitiveManager.iC>
#		endif 

  /** @} */
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_KERNEL_GLPRIMITIVEMANAGER_H

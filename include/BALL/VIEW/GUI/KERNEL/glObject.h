// $Id: glObject.h,v 1.5 2001/02/11 13:04:38 hekl Exp $

#ifndef BALL_VIEW_GUI_KERNEL_GLOBJECT_H
#define BALL_VIEW_GUI_KERNEL_GLOBJECT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLPRIMITIVEMANAGER_H
#	include <BALL/VIEW/GUI/KERNEL/glPrimitiveManager.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif


namespace BALL
{

	namespace VIEW
	{

		/* forward */
		class CompositeDescriptor;
		
		/** GLObject class.
				
				{\bf Framework:} BALL/VIEW/GUI/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/GUI/KERNEL/glObject.h}
				\\

				The class GLObject is the base class for all geometric primitives that have a 
				graphical representation. GLObject declares an interface for the drawing mechanism
				that is used for creating the visualisation.
				All geometric primitives with graphical representation must be derived from this class.
				The class glObject has access to the class glPrimitiveManager that holds a number
				of primitives in variable resolutions. See \Ref{GLPrimitiveManager} for further
				information concerning available primitives.
				
				@memo    GLObject class (BALL VIEW gui kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.5 $
				@date    $Date: 2001/02/11 13:04:38 $
		*/
		class GLObject
		{
			/** @name Class friends

					\begin{itemize}
						\item class CompositeDescriptor
					\end{itemize}
			*/

			friend class CompositeDescriptor;

			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new glObject.

					@return      GLObject - new constructed glObject
					@see         GLPrimitiveManager::GLPrimitiveManager
			*/
			GLObject();
			
			/** Copy constructor.
					Construct new glObject by copying the glObject {\em GL_object}.
					The glPrimitiveManager of {\em *this} glObject is initialized to the glPrimitiveManager of the glObject {\em GL_object}.\\

					@param       GL_object the glObject to be copied 
					@return      GLObject - new constructed glObject cloned from {\em GL_object}
					@see         GLPrimitiveManager::GLPrimitiveManager
			*/
			GLObject(const GLObject& GL_object);

			//@}

			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} glObject.
					Calls \Ref{GLObject::destroy}.
					@see         GLObject::destroy
			*/
			virtual ~GLObject();

			/** Explicit default initialization.
			*/
			virtual void clear();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy();
			//@}

			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the glObject {\em GL_object} to {\em *this} glObject.
					The glPrimitiveManager of {\em *this} glObject is initialized to the glPrimitiveManager of the glObject {\em GL_object}.\\

					@param       GL_object the glObject to be copied
					@see         GLObject::GLObject
			*/
			void set(const GLObject& GL_object);

			/** Assignment operator.
					Assign the glObject {\em GL_object} to {\em *this} glObject.
					Calls \Ref{GLObject::set}.
					The glPrimitiveManager of {\em *this} glObject is initialized to the glPrimitiveManager of the glObject {\em GL_object}.\\

					@param       GL_object the glObject to be copied
					@return      GLObject& - {\em *this} glObject
					@see         GLObject::set
			*/
			GLObject& operator = (const GLObject& GL_object);

			/** Copying.
					Copy {\em *this} glObject to the glObject {\em GL_object}.
					Calls \Ref{GLObject::set}.
					The glPrimitiveManager of {\em *this} glObject is initialized to the glPrimitiveManager of the glObject {\em GL_object}.\\

					@param       GL_object the glObject to be assigned to
					@see         GLObject::set
			*/
			void get(GLObject& GL_object) const;

			/** Swapping of glObjects.
					Swap the glPrimitiveManagers of {\em *this} glObject with the glObject
					{\em GL_object}.

					@param       GL_object the glObject being swapped with {\em *this} glObject 
					@see         GLObject::GLObject
			*/
			void swap(GLObject& GL_object);
			//@}

			/**	@name	debuggers and diagnostics
			*/
			//@{
			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} glObject.
					If the internal state of {\em *this} glObject is correct (self-validated) and 
					consistent {\tt true} is returned, {\tt false} otherwise. 
					Calls {GLPrimitiveManager::isValid}.

					@return			bool -
											{\tt true} if the internal state of {\em *this} glObject is correct (self-validated) and consistent,
					 						{\tt false} otherwise
					@see        GLPrimitiveManager::isValid
			*/
			virtual bool isValid() const;

			/** Internal value dump.
					Dump the current state of {\em *this} glObject to 
					the output ostream {\em s} with dumping depth {\em depth}.

					@param   s output stream where to output the state of {\em *this} glObject
					@param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			/** Persistent stream output and state restorage.
  			 Read persistent glObject data from the input stream {\em s} and 
				 restore the state of {\em *this}.
				 \\
				 {\bf Note:} Not yet implemented.
		 
				 @param       s input stream from where to restore the internal state of {\em *this} glObject
					@exception   NotImplemented - always
			*/
			virtual void read(std::istream& s);

			/** Persistent stream output and state storage.
  			 Write persistent glObject data to the output stream {\em s} and 
				 store the state of {\em *this}.
				 \\
				 {\bf Note:} Not yet implemented.
		 
				 @param       s output stream to where to store the internal state of {\em *this} glObject
					@exception   NotImplemented - always
			*/
			virtual void write(std::ostream& s) const;
			//@}

			

			protected:

			/** @name Graphical interface methods
			*/
			//@{
			
			/** Creation of the graphical representation.
					Override this method for creating the graphical representation. OpenGL code can be
					used in this function and predefined shapes can be accessed by the 
					getGLPrimitiveManager method.
					The parameter {\em with_names} indicates whether the openGL command {\em glLoadName}
					must be used for naming the graphical object (necessary for picking mode in the
					scene).
					
					@param     with_names flag if the graphical objects must have a name
					@return    bool - {\tt true} if successful,	{\tt false} otherwise
					@see       GLObject::getGLPrimitiveManager
			*/
			virtual bool draw(bool with_names = false);

			/** Export method.
					This method handles the export of {\em *this} glObject into another
					format (eg. POVRAY, VRML)
				  \\
				  {\bf Note:} Not yet implemented.

					@return    bool - {\tt true} if successful,	{\tt false} otherwise
			*/
			virtual bool extract();
			
			/** Experimental method.
					Please avoid using this method.
			*/
			virtual bool drawUserDefined();

			//@}

			/** @name PrimitiveManager methods
			*/
			//@{

			/** Internal primitive manager register method.
					This methods registers a \Ref{GLPrimitiveManager} which contains a number
					of predefined primitives in variable resolutions. This method will
					be called for each \Ref{Scene} by the render engine before calling the method
					draw. 

					@param    GL_primitive_manager the primitive manager for the current scene
					@see      GLObject::draw
					@see      GLPrimitiveManager::GLPrimitiveManager
					@see      Scene::Scene
			*/
			void setGLPrimitiveManager(const GLPrimitiveManager& GL_primitive_manager);
			
			/** Internal primitive manager access method.
					If the primitive uses predefined shapes that are available in \Ref{GLPrimitiveManager}
					then this method should be used in the draw method to access these shapes.
					There is the possibility that before each call to the draw method the primitive
					manager changes. Therefore it is necessary that this method should be used
					when accessing predefined shapes instead of permanent storing a pointer to the
					primitive	manager.

					@return     GLPrimitiveManager* - a pointer to a primitive manager containing predefined shapes
					@see        GLobject::setGLPrimitiveManager
					@see        GLPrimitiveManager::GLPrimitiveManager
			*/
			GLPrimitiveManager* getGLPrimitiveManager() const;

			//@}
				
			private:

			/* pointer to the DisplayListManager */
			GLPrimitiveManager* GL_primitive_manager_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/glObject.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_KERNEL_GLOBJECT_H


// $Id: glEntityDescriptor.h,v 1.5 2001/05/13 13:57:01 hekl Exp $

#ifndef BALL_VIEW_GUI_KERNEL_GLENTITYDESCRIPTOR_H
#define BALL_VIEW_GUI_KERNEL_GLENTITYDESCRIPTOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLDISPLAYLIST_H
#	include <BALL/VIEW/GUI/KERNEL/glDisplayList.h>
#endif


namespace BALL
{
	
	namespace VIEW
	{

		/** GLEntityDescriptor class.
				{\bf Framework:} BALL/VIEW/GUI/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/GUI/KERNEL/glEntityDescriptor.h} \\
				{\bf Category:} \Ref{GLDisplayList} container\\	\\
				The class GLEntityDescriptor is a container for \Ref{GLDisplayList}'s.
				A graphical representation of an object contains different graphical
				visualizations (e.g. a solid part, a transparent part, a wireframe part, etc.).
				If a graphical object should be drawn correctly (with OpenGL) it must be
				divided into these parts. Later if the object is drawn these parts will
				be drawn in a very strict order (e.g. first the solid part and than the
				transparent part). If this order is not maintained the visualization of
				the object will look incorrect.
				The class GLEntityDescriptor handles the necessary different visualizations
				needed by the render engine. It stores different parts of the visualization
				in \Ref{GLDisplayList} objects. The class is used by the 
				\Ref{CompositeDescriptor} class.
				The different visualization parts are:
				\begin{itemize}
				  \item  {\bf static display list} - this \Ref{GLDisplayList} stores the visualization of an object in the non-moveable state
					\item  {\bf static always front display list} - this \Ref{GLDisplayList} stores the visualization of an object in the non-moveable state that should be always in front of other objects
					\item  {\bf static wireframe display list} - this \Ref{GLDisplayList} stores the visualization of an object in the non-moveable state that should be drawn as wireframe
					\item  {\bf static always front wireframe display list} - this \Ref{GLDisplayList} stores the visualization of an object in the non-moveable state that should be drawn as wireframe and should be always in front of other objects.
					\item  {\bf dynamic display list} - this \Ref{GLDisplayList} stores the visualization of an object in the moveable state
					\item  {\bf dynamic always front display list} - this \Ref{GLDisplayList} stores the visualization of an object in the moveable state that should be always in front of other objects
					\item  {\bf transparent display list} - this \Ref{GLDisplayList} stores the visualization of an object in the moveable state that should be drawn transparent
					\item  {\bf transparent always front display list} - this \Ref{GLDisplayList} stores the visualization of an object in the moveable state that should be drawn transparent and should be always in front of other objects.
				\end{itemize}
				This class is used internally by the \Ref{CompositeDescriptor} class.
				@memo    GLEntityDescriptor class (BALL VIEW gui kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.5 $
				@date    $Date: 2001/05/13 13:57:01 $
		*/
		class GLEntityDescriptor
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new glEntityDescriptor.
					There is no copy constructor because \Ref{GLDisplayList} objects
					have no copy constructor.
					The state of {\em *this} glEntityDescriptor is set to:
					\begin{itemize}
					  \item all display lists are empty
						\item update is necessary
					\end{itemize}
					@return      GLEntityDescriptor new constructed glEntityDescriptor
					@see         GLDisplayList
			*/
			GLEntityDescriptor()
				throw();
			
			//@}

			/** @name Destructors
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} glEntityDescriptor.
					Calls \Ref{destroy}
					@see  destroy
			*/
			virtual ~GLEntityDescriptor()
				throw();

			/** Explicit default initialization.
					Destroys all internal \Ref{GLDisplayList} objects.
					That means that the graphical representation of {\em *this}
					glEntityDescriptor is cleared.
					Calls \Ref{GLDisplayList::destroy}
					@see  GLDisplayList::destroy
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
			
			/** Mutable inspection of the static display list.
					Access the mutable pointer of the static display list of {\em *this} 
					glEntityDescriptor. This method will be 
					called internally by \Ref{CompositeDescriptor}.
					@return      GLDisplayList* mutable pointer to the static display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			GLDisplayList* getStaticDisplayList()
				throw();

			/** Non-mutable inspection of the static display list.
					Access the constant pointer of the static display list of {\em *this} 
					glEntityDescriptor. This method will be 
					called internally by \Ref{CompositeDescriptor}.
					@return      const GLDisplayList* constant pointer to the static display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			const GLDisplayList* getStaticDisplayList() const
				throw();

			/** Mutable inspection of the static always front display list.
					Access the mutable pointer of the static always front display list of
					{\em *this} glEntityDescriptor. This method will be 
					called internally by \Ref{CompositeDescriptor}.
					@return      GLDisplayList* mutable pointer to the static always front display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			GLDisplayList* getStaticAlwaysFrontDisplayList()
				throw();

			/** Non-mutable inspection of the static always front display list.
					Access the constant pointer of the static always front display list of
					{\em *this} glEntityDescriptor. This method will be 
					called internally by \Ref{CompositeDescriptor}.
					@return      const GLDisplayList* constant pointer to the static always front display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			const GLDisplayList* getStaticAlwaysFrontDisplayList() const
				throw();

			/** Mutable inspection of the static wireframe display list.
					Access the mutable pointer of the static wireframe display list of
					{\em *this} glEntityDescriptor. This method will be 
					called internally by \Ref{CompositeDescriptor}.
					@return      GLDisplayList* mutable pointer to the static wireframe display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			GLDisplayList* getStaticWireframeDisplayList()
				throw();

			/** Non-mutable inspection of the static wireframe display list.
					Access the constant pointer of the static wireframe display list of
					{\em *this} glEntityDescriptor. This method will be 
					called internally by \Ref{CompositeDescriptor}.
					@return      const GLDisplayList* constant pointer to the static wireframe display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			const GLDisplayList* getStaticWireframeDisplayList() const
				throw();

			/** Mutable inspection of the static wireframe always front display list.
					Access the mutable pointer of the static wireframe always front display
					list of	{\em *this} glEntityDescriptor. This method will be 
					called internally by \Ref{CompositeDescriptor}.
					@return      GLDisplayList* mutable pointer to the static wireframe always front display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			GLDisplayList* getStaticWireframeAlwaysFrontDisplayList()
				throw();

			/** Non-mutable inspection of the static wireframe always front display list.
					Access the constant pointer of the static wireframe always front display
					list of	{\em *this} glEntityDescriptor. This method will be 
					called internally by \Ref{CompositeDescriptor}.
					@return      const GLDisplayList* constant pointer to the static wireframe always front display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			const GLDisplayList* getStaticWireframeAlwaysFrontDisplayList() const
				throw();

			/** Mutable inspection of the dynamic display list.
					Access the mutable pointer of the dynamic display
					list of	{\em *this} glEntityDescriptor. This method will be 
					called internally by \Ref{CompositeDescriptor}.
					@return      GLDisplayList* mutable pointer to the dynamic display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			GLDisplayList* getDynamicDisplayList()
				throw();

			/** Non-mutable inspection of the dynamic display list.
					Access the constant pointer of the dynamic display
					list of	{\em *this} glEntityDescriptor. This method will be 
					called internally by \Ref{CompositeDescriptor}.
					@return      const GLDisplayList* constant pointer to the dynamic display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			const GLDisplayList* getDynamicDisplayList() const
				throw();

			/** Mutable inspection of the dynamic always front display list.
					Access the mutable pointer of the dynamic always front display
					list of	{\em *this} glEntityDescriptor. This method will be 
					called internally by \Ref{CompositeDescriptor}.
					@return      GLDisplayList* mutable pointer to the dynamic always front display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			GLDisplayList* getDynamicAlwaysFrontDisplayList()
				throw();

			/** Non-mutable inspection of the dynamic always front display list.
					Access the constant pointer of the dynamic always front display
					list of	{\em *this} glEntityDescriptor. This method will be 
					called internally by \Ref{CompositeDescriptor}.
					@return      const GLDisplayList* constant pointer to the dynamic always front display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			const GLDisplayList* getDynamicAlwaysFrontDisplayList() const
				throw();

			/** Mutable inspection of the transparent display list.
					Access the mutable pointer of the transparent display
					list of	{\em *this} glEntityDescriptor. This method will be 
					called internally by \Ref{CompositeDescriptor}.
					@return      GLDisplayList* mutable pointer to the transparent display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			GLDisplayList* getTransparentDisplayList()
				throw();

			/** Non-mutable inspection of the transparent display list.
					Access the constant pointer of the transparent display
					list of	{\em *this} glEntityDescriptor. This method will be 
					called internally by \Ref{CompositeDescriptor}.
					@return      const GLDisplayList* constant pointer to the transparent display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			const GLDisplayList* getTransparentDisplayList() const
				throw();

			/** Mutable inspection of the transparent always front display list.
					Access the mutable pointer of the transparent always front display
					list of	{\em *this} glEntityDescriptor. This method will be 
					called internally by \Ref{CompositeDescriptor}.
					@return      GLDisplayList* mutable pointer to the transparent always front display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			GLDisplayList* getTransparentAlwaysFrontDisplayList()
				throw();

			/** Non-mutable inspection of the transparent always front display list.
					Access the constant pointer of the transparent always front display
					list of	{\em *this} glEntityDescriptor. This method will be 
					called internally by \Ref{CompositeDescriptor}.
					@return      const GLDisplayList* constant pointer to the transparent always front display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			const GLDisplayList* getTransparentAlwaysFrontDisplayList() const
				throw();

			/** Mark for update.
					Mark {\em *this} glEntityDescriptor for update.
					If {\em *this} glEntityDescriptor is updated all \Ref{GLDisplayList} objects
					will be cleared (see \Ref{clear}). This method will be 
					called internally by \Ref{CompositeDescriptor}.
					@see   cancelUpdate
					@see   needUpdate
			*/
			void update()
				throw();

			/** Cancel the update.
					Cancel the update of {\em *this} glEntityDescriptor. This method will be 
					called internally by \Ref{CompositeDescriptor}.
					@see   update
					@see   needUpdate
			*/
			void cancelUpdate()
				throw();
 			//@}

			/**	@name	Predicates
			*/
			//@{

			/** Test if update needed.
					Test if {\em *this} glEntityDescriptor needs an update. This method will be 
					called internally by \Ref{CompositeDescriptor}.
					@see   update
					@see   cancelUpdate
			*/
			bool needUpdate() const
				throw();
			//@}

			/**	@name	debuggers and diagnostics
			*/
			//@{
			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure 
					consistencies	of {\em *this} glEntityDescriptor.
					If the internal state of {\em *this} glEntityDescriptor is correct
					(self-validated) and consistent {\tt true} is returned,
					{\tt false} otherwise. 
					{\em *this} glEntityDescriptor is valid if all internal display lists
					are valid.
					Calls \Ref{GLDisplayList::isValid}
					@return			bool {\tt true} if the internal state of {\em *this} glEntityDescriptor is correct (self-validated) and consistent,
					 						{\tt false} otherwise
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of {\em *this} glEntityDescriptor to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Dumps all display lists.
					Calls \Ref{GLDisplayList::dump}
					@param   s output stream where to output the state of {\em *this} glEntityDescriptor
					@param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			/**	@name	Storers
			*/
			//@{

			/** Persistent stream output and state restorage.
  			  Read persistent glEntityDescriptor data from the input stream {\em s} and 
				  restore the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s input stream from where to restore the internal state of {\em *this} glEntityDescriptor
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			  Write persistent glEntityDescriptor data to the output stream {\em s} and 
				  store the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s output stream to where to store the internal state of {\em *this} glEntityDescriptor
			*/
			virtual void write(std::ostream& s) const
				throw();
			//@}
				

			private:
			
			GLDisplayList static_display_list_;
			GLDisplayList static_always_front_display_list_;
			GLDisplayList static_wireframe_display_list_;
			GLDisplayList static_wireframe_always_front_display_list_;
			GLDisplayList dynamic_display_list_;
			GLDisplayList dynamic_always_front_display_list_;
			GLDisplayList transparent_display_list_;
			GLDisplayList transparent_always_front_display_list_;

			bool update_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/glEntityDescriptor.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_KERNEL_GLENTITYDESCRIPTOR_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: glEntityDescriptor.h,v 1.14 2003/02/21 16:07:26 anhi Exp $

#ifndef BALL_VIEW_GUI_KERNEL_GLENTITYDESCRIPTOR_H
#define BALL_VIEW_GUI_KERNEL_GLENTITYDESCRIPTOR_H

#ifndef BALL_VIEW_GUI_KERNEL_GLDISPLAYLIST_H
#	include <BALL/VIEW/GUI/KERNEL/glDisplayList.h>
#endif

namespace BALL
{
	namespace VIEW
	{

		/** GLEntityDescriptor class.
				The class GLEntityDescriptor is a container for  \link GLDisplayList GLDisplayList \endlink 's.
				A graphical representation of an object contains different graphical
				visualizations (e.g. a solid part, a transparent part, a wireframe part, etc.).
				If a graphical object should be drawn correctly (with OpenGL) it must be
				divided into these parts. Later if the object is drawn these parts will
				be drawn in a very strict order (e.g. first the solid part and then the
				transparent part). If this order is not maintained the visualization of
				the object will look incorrect.
				The class GLEntityDescriptor handles the necessary different visualizations
				needed by the render engine. It stores different parts of the visualization
				in  \link GLDisplayList GLDisplayList \endlink  objects. The class is used by the 
				 \link CompositeDescriptor CompositeDescriptor \endlink  class.
				The different visualization parts are:
				\begin{itemize}
				  \item  <b>static display list</b> - this  \link GLDisplayList GLDisplayList \endlink  stores the 
								 visualization of an object in the non-moveable state
					\item  <b>static always front display list</b> - this  \link GLDisplayList GLDisplayList \endlink  stores the 
								 visualization of an object in the non-moveable state that should be always in front of other objects
					\item  <b>static wireframe display list</b> - this  \link GLDisplayList GLDisplayList \endlink  stores the 
								 visualization of an object in the non-moveable state that should be drawn as wireframe
					\item  <b>static always front wireframe display list</b> - this  \link GLDisplayList GLDisplayList \endlink  stores the visualization 
								 of an object in the non-moveable state that should be drawn as wireframe and should be always 
								 in front of other objects.
					\item  <b>dynamic display list</b> - this  \link GLDisplayList GLDisplayList \endlink  stores the visualization of an object in the 
								 moveable state
					\item  <b>dynamic always front display list</b> - this  \link GLDisplayList GLDisplayList \endlink  stores the visualization 
								 of an object in the moveable state that should be always in front of other objects
	  			\item  <b>dynamic wireframe display list</b> - this  \link GLDisplayList GLDisplayList \endlink  stores the 
					 			 visualization of an object in the moveable state that should be drawn as wireframe
	  			\item  <b>dynamic wireframe always front display list</b> - this  \link GLDisplayList GLDisplayList \endlink  stores the 
					 			 visualization of an object in the moveable state that should be drawn as wireframe and should be always
								 in front of other objects
				  \item  <b>transparent display list</b> - this  \link GLDisplayList GLDisplayList \endlink  stores the visualization of 
								 an object in the moveable state that should be drawn transparent
					\item  <b>transparent always front display list</b> - this  \link GLDisplayList GLDisplayList \endlink  stores the visualization
								 of an object in the moveable state that should be drawn transparent and should be always in front 
								 of other objects.
				\end{itemize}
				This class is used internally by the  \link CompositeDescriptor CompositeDescriptor \endlink  class.  \par
				<b>Definition:</b> BALL/VIEW/GUI/KERNEL/glEntityDescriptor.h
		*/
		class GLEntityDescriptor
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new glEntityDescriptor.
					There is no copy constructor because  \link GLDisplayList GLDisplayList \endlink  objects
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
					Calls  \link destroy destroy \endlink 
					@see  destroy
			*/
			virtual ~GLEntityDescriptor()
				throw();

			/** Explicit default initialization.
					Destroys all internal  \link GLDisplayList GLDisplayList \endlink  objects.
					That means that the graphical representation of {\em *this}
					glEntityDescriptor is cleared.
					Calls  \link GLDisplayList::destroy GLDisplayList::destroy \endlink 
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
					called internally by  \link CompositeDescriptor CompositeDescriptor \endlink .
					@return      GLDisplayList* mutable pointer to the static display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			GLDisplayList* getStaticDisplayList()
				throw();

			/** Non-mutable inspection of the static display list.
					For further information see  \link getStaticDisplayList getStaticDisplayList \endlink .
			*/
			const GLDisplayList* getStaticDisplayList() const
				throw();

			/** Mutable inspection of the static always front display list.
					Access the mutable pointer of the static always front display list of
					{\em *this} glEntityDescriptor. This method will be 
					called internally by  \link CompositeDescriptor CompositeDescriptor \endlink .
					@return      GLDisplayList* mutable pointer to the static always front display list 
					@see         GLDisplayList
			*/
			GLDisplayList* getStaticAlwaysFrontDisplayList()
				throw();

			/** Non-mutable inspection of the static always front display list.
					For further information see  \link getStaticAlwaysFrontDisplayList getStaticAlwaysFrontDisplayList \endlink .
			*/
			const GLDisplayList* getStaticAlwaysFrontDisplayList() const
				throw();

			/** Mutable inspection of the static wireframe display list.
					Access the mutable pointer of the static wireframe display list of
					{\em *this} glEntityDescriptor. This method will be 
					called internally by  \link CompositeDescriptor CompositeDescriptor \endlink .
					@return      GLDisplayList* mutable pointer to the static wireframe display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			GLDisplayList* getStaticWireframeDisplayList()
				throw();

			/** Non-mutable inspection of the static wireframe display list.
					For further information see  \link getStaticWireframeDisplayList getStaticWireframeDisplayList \endlink .
			*/
			const GLDisplayList* getStaticWireframeDisplayList() const
				throw();

			/** Mutable inspection of the static wireframe always front display list.
					Access the mutable pointer of the static wireframe always front display
					list of	{\em *this} glEntityDescriptor. This method will be 
					called internally by  \link CompositeDescriptor CompositeDescriptor \endlink .
					@return      GLDisplayList* mutable pointer to the static wireframe always front display list 
					@see         GLDisplayList
			*/
			GLDisplayList* getStaticWireframeAlwaysFrontDisplayList()
				throw();

			/** Non-mutable inspection of the static wireframe always front display list.
					For further information see  \link getStaticWireframeAlwaysFrontDisplayList getStaticWireframeAlwaysFrontDisplayList \endlink .
			*/
			const GLDisplayList* getStaticWireframeAlwaysFrontDisplayList() const
				throw();

			/** Mutable inspection of the dynamic display list.
					Access the mutable pointer of the dynamic display
					list of	{\em *this} glEntityDescriptor. This method will be 
					called internally by  \link CompositeDescriptor CompositeDescriptor \endlink .
					@return      GLDisplayList* mutable pointer to the dynamic display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			GLDisplayList* getDynamicDisplayList()
				throw();

			/** Non-mutable inspection of the dynamic display list.
					For further information see  \link getDynamicDisplayList getDynamicDisplayList \endlink .
			*/
			const GLDisplayList* getDynamicDisplayList() const
				throw();

			/** Mutable inspection of the dynamic always front display list.
					Access the mutable pointer of the dynamic always front display
					list of	{\em *this} glEntityDescriptor. This method will be 
					called internally by  \link CompositeDescriptor CompositeDescriptor \endlink .
					@return      GLDisplayList* mutable pointer to the dynamic always front display list of 
											 {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			GLDisplayList* getDynamicAlwaysFrontDisplayList()
				throw();

			/** Non-mutable inspection of the dynamic always front display list.
					For further information see  \link getDynamicAlwaysFrontDisplayList getDynamicAlwaysFrontDisplayList \endlink .
			*/
			const GLDisplayList* getDynamicAlwaysFrontDisplayList() const
				throw();

			/** Mutable inspection of the dynamic wireframe display list.
					Access the mutable pointer of the dynamic wireframe display
					list of	{\em *this} glEntityDescriptor. This method will be 
					called internally by  \link CompositeDescriptor CompositeDescriptor \endlink .
					@return      GLDisplayList* mutable pointer to the dynamic wireframe display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			GLDisplayList* getDynamicWireframeDisplayList()
				throw();

			/** Non-mutable inspection of the dynamic wireframe display list.
					For further information see  \link getDynamicWireframeDisplayList getDynamicWireframeDisplayList \endlink .
			*/
			const GLDisplayList* getDynamicWireframeDisplayList() const
				throw();
			
			/** Mutable inspection of the dynamic wireframe always front display list.
					Access the mutable pointer of the dynamic wireframe always front display
					list of	{\em *this} glEntityDescriptor. This method will be 
					called internally by  \link CompositeDescriptor CompositeDescriptor \endlink .
					@return      GLDisplayList* mutable pointer to the dynamic wireframe always front display 
												list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			GLDisplayList* getDynamicWireframeAlwaysFrontDisplayList()
				throw();

			/** Non-mutable inspection of the dynamic wireframe always front display list.
					For further information see  \link getDynamicWireframeAlwaysFrontDisplayList getDynamicWireframeAlwaysFrontDisplayList \endlink .
			*/
			const GLDisplayList* getDynamicWireframeAlwaysFrontDisplayList() const
				throw();

			/** Mutable inspection of the transparent display list.
					Access the mutable pointer of the transparent display
					list of	{\em *this} glEntityDescriptor. This method will be 
					called internally by  \link CompositeDescriptor CompositeDescriptor \endlink .
					@return      GLDisplayList* mutable pointer to the transparent display list of {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			GLDisplayList* getTransparentDisplayList()
				throw();

			/** Non-mutable inspection of the transparent display list.
					For further information see  \link getTransparentDisplayList getTransparentDisplayList \endlink . 
			*/
			const GLDisplayList* getTransparentDisplayList() const
				throw();

			/** Mutable inspection of the transparent always front display list.
					Access the mutable pointer of the transparent always front display
					list of	{\em *this} glEntityDescriptor. This method will be 
					called internally by  \link CompositeDescriptor CompositeDescriptor \endlink .
					@return      GLDisplayList* mutable pointer to the transparent always front display list of 
											 {\em *this} glEntityDescriptor
					@see         GLDisplayList
			*/
			GLDisplayList* getTransparentAlwaysFrontDisplayList()
				throw();

			/** Non-mutable inspection of the transparent always front display list.
					For further information see  \link getTransparentAlwaysFrontDisplayList getTransparentAlwaysFrontDisplayList \endlink .
			*/
			const GLDisplayList* getTransparentAlwaysFrontDisplayList() const
				throw();

			/** Mark for update.
					Mark {\em *this} glEntityDescriptor for update.
					If {\em *this} glEntityDescriptor is updated all  \link GLDisplayList GLDisplayList \endlink  objects
					will be cleared (see  \link clear clear \endlink ). This method will be 
					called internally by  \link CompositeDescriptor CompositeDescriptor \endlink .
					@see   cancelUpdate
					@see   needUpdate
			*/
			void update()
				throw();

			/** Cancel the update.
					Cancel the update of {\em *this} glEntityDescriptor. This method will be 
					called internally by  \link CompositeDescriptor CompositeDescriptor \endlink .
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
					called internally by  \link CompositeDescriptor CompositeDescriptor \endlink .
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
					(self-validated) and consistent <tt>true</tt> is returned,
					<tt>false</tt> otherwise. 
					{\em *this} glEntityDescriptor is valid if all internal display lists
					are valid.
					Calls  \link GLDisplayList::isValid GLDisplayList::isValid \endlink 
					@return			bool <tt>true</tt> if the internal state of {\em *this} glEntityDescriptor is correct
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of {\em *this} glEntityDescriptor to 
					the output ostream {\em s} with dumping depth {\em depth}.
					Dumps all display lists.
					Calls  \link GLDisplayList::dump GLDisplayList::dump \endlink 
					@param   s output stream where to output the state of {\em *this} glEntityDescriptor
					@param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			//@}
				
			private:
			
			GLDisplayList static_display_list_;
			GLDisplayList static_always_front_display_list_;
			GLDisplayList static_wireframe_display_list_;
			GLDisplayList static_wireframe_always_front_display_list_;
			GLDisplayList dynamic_display_list_;
			GLDisplayList dynamic_always_front_display_list_;
			GLDisplayList dynamic_wireframe_display_list_;
			GLDisplayList dynamic_wireframe_always_front_display_list_;
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

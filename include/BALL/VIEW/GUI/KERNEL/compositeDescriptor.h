// $Id: compositeDescriptor.h,v 1.8 2001/07/09 18:33:18 sturm Exp $

#ifndef BALL_VIEW_GUI_KERNEL_COMPOSITEDESCRIPTOR_H
#define BALL_VIEW_GUI_KERNEL_COMPOSITEDESCRIPTOR_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_MATHS_ANGLE_H
#	include <BALL/MATHS/angle.h>
#endif

#ifndef BALL_MATHS_QUATERNION_H
#	include <BALL/MATHS/quaternion.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_VIEW_GUI_FUNCTOR_GLOBJECTCOLLECTOR_H
#	include <BALL/VIEW/GUI/FUNCTOR/glObjectCollector.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLENTITYDESCRIPTOR_H
#	include <BALL/VIEW/GUI/KERNEL/glEntityDescriptor.h>
#endif

#ifndef BALL_VIEW_GUI_KERNEL_GLPRIMITIVEMANAGER_H
#	include <BALL/VIEW/GUI/KERNEL/glPrimitiveManager.h>
#endif


namespace BALL
{
	
	namespace VIEW
	{

		/** CompositeDescriptor class.
				{\bf Framework:} BALL/VIEW/GUI/KERNEL\\
				{\bf Defintion:} \URL{BALL/VIEW/GUI/KERNEL/compositeDescriptor.h}\\
				{\bf Category:} \Ref{CompositeDescriptor} container\\
				{\bf Category:} \Ref{GLEntityDescriptor} container\\ \\
				The class CompositeDescriptor is responsible for maintaining
				the graphical representation stored in \Ref{GLEntityDescriptor} objects of 
				a given \Ref{Composite}.
				This class is part of the render engine for creating the graphical representation.
				Certain methods create, compile, update and maintain the visualization.
				This class will be used internally by the \Ref{MainControl} class for
				handling \Ref{Composite} objects.
				All necessary information concerning visualization of a \Ref{Composite} like
				name, center vector and rotation axis are available in this class.
				Further this class creates, updates and maintains \Ref{GLEntityDescriptor}
				objects for every \Ref{Scene}.
				These \Ref{GLEntityDescriptor} contain precompiled display lists of the
				\Ref{Composite} of {\em *this} compositeDescriptor for faster visualization.
				It is possible to create a shallow copy of {\em *this} compositeDescriptor.
				Such a shallow copy has only a link to its parent compositeDescriptor. It shares
				all attributes including the \Ref{Composite} and the graphical representation 
				with its parent.
				If the parent of such a shallow copy will be deleted the shallow copy will be
				deleted as well because all attributs are no longer available. This mechanism
				will work automatically.
				@memo    CompositeDescriptor class (BALL VIEW gui kernel framework)
				@author  $Author: sturm $
				@version $Revision: 1.8 $
				@date    $Date: 2001/07/09 18:33:18 $
		*/
		class CompositeDescriptor
		{
			public:

			/** @name Deep cloning facility.
			*/
			//@{
			BALL_CREATE_DEEP(CompositeDescriptor)
			//@}

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new compositeDescriptor.
					The state of {\em *this} compositeDescriptor is:
					\begin{itemize}
						\item name is set to "unknown"
						\item center vector is set (0.0,0.0,0.0)
						\item quaternion is set to (0.0,0.0,0.0,1.0)
						\item composite pointer is set to 0
						\item entities are cleared
						\item objectCollector pointer is set to 0
						\item primitiveManager pointer is set to 0
						\item {\em *this} is shallow copy
						\item no shallow copies
						\item no parent 
					\end{itemize}
					@return CompositeDescriptor new constructed compositeDescriptor
					@see    Composite
					@see    Quaternion
					@see    GLPrimitiveManager
					@see    GLEntityDescriptor
					@see    GLObjectCollector
			 */
			CompositeDescriptor()
				throw();
			
			/** Copy constructor.
					Construct new compositeDescriptor by copying the compositeDescriptor
					{\em composite_descriptor}.
					The copy is either deep (default) or shallow. In this case deep means
					that the composite of the given compositeDescriptor {\em composite_descriptor}
					is also copied deep. If the flag {\em deep} is set to {\tt false} then
					only a link to the given compositeDescriptor
					{\em composite_descriptor} is stored in this new compositeDescriptor.
					In the case of a shallow copy all the other attributes of {\em *this}
					compositeDescriptor are linked to {\em composite_descriptor} compositeDescriptor.
					If the parent compositeDescriptor of a shallow compositeDescriptor is deleted
					so the shallow compositeDescriptor will also be deleted because all the 
					necessary attributes and the \Ref{Composite} are originally stored in the parent
					compositeDescriptor.
					@param  composite_descriptor the compositeDescriptor to be copied (cloned)
					@param  deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em composite_descriptor}
					@return CompositeDescriptor new constructed compositeDescriptor cloned from {\em composite_descriptor}.
					@see    CompositeDescriptor
			 */
			CompositeDescriptor(const CompositeDescriptor& composite_descriptor, bool deep = true)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} compositeDescriptor.
					Calls \Ref{destroy}.
					@see         destroy
			 */
			virtual ~CompositeDescriptor()
				throw();

			/** Explicit default initialization.
					Clears all \Ref{GLEntityDescriptor} objects of {\em *this} compositeDescriptor 
					and resets the internal linkage.
					Calls \Ref{GLEntityDescriptor::clear}.
					@see     GLEntityDescriptor::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Destroy {\em *this} compositeDescriptor.
					Deletes all graphical representation stored in \Ref{GLEntityDescriptor} objects
					for the \Ref{Composite} of {\em *this} compositeDescriptor. Deletes all shallow
					copies of {\em *this} compositeDescriptor.
					If {\em *this} compositeDescriptor was a shallow copy of another 
					compositeDescriptor it will be removed from it. If {\em *this} compositeDescriptor
					was a deep copy of another compositeDescriptor the \Ref{Composite}
					of {\em *this} compositeDescriptor will be deleted.
					Calls \Ref{clear}
					@see         clear
			*/
			virtual void destroy()
				throw();
			//@}

			/**	@name	Assignment
			*/
			//@{

			/** Assignment with cloning facility.
					Assign the compositeDescriptor {\em composite_descriptor} to {\em *this}
					compositeDescriptor.
					The assignment is either deep (default) or shallow.
					If this method is called {\em *this} compositeDescriptor is internally destroyed.
					Calls \Ref{destroy}
					@param  composite_descriptor the compositeDescriptor to be copied (cloned)
					@param  deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em composite_descriptor}
					@see    CompositeDescriptor
					@see    destroy
			*/
			void set(CompositeDescriptor& composite_descriptor, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the compositeDescriptor {\em composite_descriptor} to {\em *this}
					compositeDescriptor.
					The assignment is deep.
					Calls \Ref{set}.
					@param   composite_descriptor the compositeDescriptor to be copied (cloned)
					@return  CompositeDescriptor& constant reference of {\em *this} compositeDescriptor
					@see     set
			*/
			const CompositeDescriptor& operator = (CompositeDescriptor& composite_descriptor)
				throw();

			/** Copying with cloning facility.
					Copy {\em *this} compositeDescriptor to the compositeDescriptor 
					{\em composite_descriptor}.
					The assignment is either deep (default) or shallow.
					Calls \Ref{set}.
					@param  composite_descriptor the compositeDescriptor to be assigned to
					@see    set
			*/
			void get(CompositeDescriptor& composite_descriptor, bool deep = true)
				throw();
			
			/** Swapping of CompositeDescriptors.
					Swap the states of {\em *this} with {\em composite_descriptor}.\\
 				  {\bf Note:} Not yet implemented.
					@param  composite_descriptor the CompositeDescriptor {\em *this} is being swapped with
			*/
			void swap(CompositeDescriptor& composite_descriptor)
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change of the CompositeDescriptor's name.
					Change the name of {\em *this} compositeDescriptor to the parameter {\em name}.
					Every graphical representation of the \Ref{Composite} of {\em *this}
					compositeDescriptor needs a name to identify later.
					@param  name the new name of {\em *this} compositeDescriptor
					@see    getName
			*/
			void setName(const String& name)
				throw();

			/** Mutable inspection of the name.
					Access a mutable reference of the name of {\em *this} compositeDescriptor.
					@return  String& mutable reference of the name of {\em *this} compositeDescriptor
					@see     setName
			*/
			String& getName()
				throw();

			/** Constant inspection of the name.
					For further information see \Ref{getName}.
			*/
			const String& getName() const
				throw();

			/** Change of the CompositeDescriptor's center vector.
					Change the center vector of {\em *this} compositeDescriptor to \Ref{Vector3}
					{\em center}.
					The center vector is the vector that is the origin of the graphical
					representation of the \Ref{Composite} of {\em *this}	compositeDescriptor.
					@param  center the new center vector of {\em *this} compositeDescriptor
					@see    getCenter
					@see     Vector3
			*/
			void setCenter(const Vector3& center)
				throw();

			/** Mutable inspection of the center vector.
					Access a mutable reference to the center vector of {\em *this} compositeDescriptor.
					@return  Vector3& mutable reference to the center vector of {\em *this} compositeDescriptor
					@see     setCenter
					@see     Vector3
			*/
			Vector3& getCenter()
				throw();

			/** Non-mutable inspection of the center vector.
					For further information see \Ref{getCenter}.
			*/
			const Vector3& getCenter() const
				throw();

			/** Change of the CompositeDescriptor's quaternion.
					Change the \Ref{Quaternion} of {\em *this} compositeDescriptor to the
					\Ref{Quaternion} {\em quaternion}.
					The \Ref{Quaternion} describes the rotation of the graphical representation 
					of the \Ref{Composite} of {\em *this}	compositeDescriptor.
					@param  quaternion the new quaternion
					@see    getQuaternion
					@see    Quaternion
			*/
			void setQuaternion(const Quaternion& quaternion)
				throw();

			/** Change of the CompositeDescriptor's quaternion.
					Change the \Ref{Quaternion} of {\em *this} compositeDescriptor to the
					\Ref{Quaternion} constructed from {\em axis} and {\em angle}.
					The \Ref{Quaternion} describes the rotation of the graphical representation 
					of the \Ref{Composite} of {\em *this}	compositeDescriptor.
					@param  axis the new quaternion axis
					@param  angle the new quaternion angle
					@see    getQuaternion
					@see    Quaternion
					@see    Vector3
			*/
			void setQuaternion(const Vector3& axis, const Real angle)
				throw();

			/** Change of the CompositeDescriptor's quaternion.
					Change the \Ref{Quaternion} of {\em *this} compositeDescriptor to the 
					\Ref{Quaternion} constructed from the parameter {\em axis} and {\em angle}.
					The \Ref{Quaternion} describes the rotation of the graphical representation 
					of the \Ref{Composite} of {\em *this}	compositeDescriptor.
					@param  axis the new \Ref{Quaternion} axis
					@param  angle the new \Ref{Quaternion} angle
					@see    getQuaternion
					@see    Quaternion
					@see    Vector3
			*/
			void setQuaternion(const Vector3& axis, const Angle& angle)
				throw();

			/** Mutable inspection of the quaternion.
					Access a mutable reference of the \Ref{Quaternion} of {\em *this} compositeDescriptor.
					@return  Quaternion& mutable reference of the \Ref{Quaternion} of {\em *this} compositeDescriptor
					@see     setQuaternion
					@see     Quaternion
			*/
			Quaternion& getQuaternion()
				throw();

			/** Constant inspection of the quaternion.
					For further information see \Ref{getQuaternion}.
			*/
			const Quaternion& getQuaternion() const
				throw();

			/** Change of the CompositeDescriptor's composite.
					Change the \Ref{Composite} of {\em *this} compositeDescriptor to {\em composite}.
					@param  composite the new \Ref{Composite} of {\em *this} compositeDescriptor
					@param  allow_deletion states if the given composite may be deleted by destruction of {\em *this} compositeDescriptor
					@see    getComposite
					@see    Composite
			*/
			void setComposite(const Composite* composite, bool allow_deletion = false)
				throw();

			/** Mutable inspection of the composite.
					Access a mutable pointern to the \Ref{Composite} of {\em *this} compositeDescriptor.
					@return  Composite* mutable pointer to the \Ref{Composite} of {\em *this} compositeDescriptor
					@see     setComposite
					@see     Composite
			*/
			Composite* getComposite()
				throw();

			/** Non-mutable inspection of the composite.
					For further information see \Ref{getComposite}.
			*/
			const Composite* getComposite() const
				throw();
				
			/** Mark all graphical representation for update.
					This method marks the \Ref{Composite} of {\em *this} and all shallow copies
					of {\em *this} for update. That means that if the next time the render engine
					wants to draw the graphical representation of {\em *this} compositeDescriptor
					or one of the shallow copies the precompiled lists stored in the current
					\Ref{GLEntityDescriptor} will be rebuild. 
					Call this method if the structure of the \Ref{Composite} or its graphical
					representation has changed.
					@see    compileEntity
					@see    GLEntityDescriptor
			*/
			void update()
				throw();
			//@}

			/** @name Internally used render engine methods.
			*/
			//@{

			/** Registers a \Ref{GLPrimitiveManager}.
					The \Ref{GLPrimitiveManager} stores some optimized primitives
					for faster rendering. The \Ref{GLPrimitiveManager} is used by 
					\Ref{GLEntityDescriptor} for creating the graphical representation. 
					For each \Ref{GLPrimitiveManager} there will be a new \Ref{GLEntityDescriptor}
					created.
					This method will be called internally from the render engine.
					@param manager the new \Ref{GLPrimitiveManager}
					@see   GLPrimitiveManager
					@see   GLEntityDescriptor
			*/
			void registerPrimitiveManager(const GLPrimitiveManager& manager)
				throw();

			/** Registers a \Ref{GLObjectCollector}.
					The \Ref{GLObjectCollector} is used by the \Ref{GLEntityDescriptor}
					for collecting all openGL objects from the \Ref{Composite} of {\em *this}
					compositeDescriptor.
					This method will be called internally from the render engine.
					@param collector the new \Ref{GLObjectCollector}
					@see   GLObjectCollector
					@see   GLEntityDescriptor
			*/
			void registerGLObjectCollector(const GLObjectCollector& collector)
				throw();

			/** Create display lists of the graphical representation.
					If the graphical representation of the \Ref{Composite} of {\em *this}
					compositeDescriptor does not change often it is better to compile this
					graphical representation into a faster processable structure. The
					\Ref{GLEntityDescriptor} {\em entity} contains the new compiled lists
					of the graphical representation of the \Ref{Composite} of {\em *this}
					compositeDescriptor.
					This method will be called internally by the 
					\Ref{drawEntity} method.
					@param entity the new \Ref{GLEntityDescriptor} that will hold the lists of 
					              the graphical representation of the \Ref{Composite} of {\em *this}
												compositeDescriptor
					@see   drawEntity
			*/
			void compileEntity(GLEntityDescriptor* entity)
				throw();

			/** Draws the graphical representation fast. 
					This method uses precompiled lists created with \Ref{compileEntity} for 
					drawing the graphical representation of the \Ref{Composite} of {\em *this}
					compositeDescriptor faster.
					The representation can either be drawn in hires
					quality (default) or in lowres quality used for quicker moving the graphic.
					This method will be called internally from the render engine.
					Calls \Ref{compileEntity}
					@param dynamic the new drawing mode ({\tt true} - lowres drawing mode, 
					               {\tt false} (Default) - hires drawing mode.
												 See \Ref{GeometricObject} for description of the different
												 drawing modes)
					@see   compileEntity
					@see   registerPrimitiveManager
					@see   GeometricObject
			*/
			void drawEntity(bool dynamic = false)
				throw();

			/** Draws the graphical representation directly.
					The representation can either be drawn in hires
					quality (default) or in lowres quality used for quicker displaying.
					This version of the drawEntity method does not use the optimized functions
					given by the \Ref{GLPrimitiveManager} but instead draws the primitives directly.
					Normaly before the drawing there will be the compilation phase that optimizes
					the graphical represenation for faster drawing. This will use a little time but
					the rendering will be faster afterwards. However if the representation changes
					itself often the time used for compilation will slow down the process
					considerably. Another feature of this method is the possibily that each
					graphical object can be named. That is essential for the selection mode.
					This method will be called internally from the render engine.
					@param dynamic the new drawing mode ({\tt true} - lowres drawing mode, 
					               {\tt false} (Default) - hires drawing mode.
												 See \Ref{GeometricObject} for description of the different
												 drawing modes)
					@param with_names the flag if names should be used or not (default)
					@see   registerPrimitiveManager
					@see   GeometricObject
			*/
			void drawDirect(bool dynamic = false, bool with_names = false)
				throw();
			//@}

			/**	@name	Predicates
			*/
			//@{
			
			/** Determines if the compositeDescriptor is a shallow copy.
					@return  bool {\tt true} if {\em *this} compositeDescriptor is a shallow copy, {\tt false} otherwise
					@see     isDeepCopy
			*/
			bool isShallowCopy() const
				throw();

			/** Test if compositeDescriptor is a deep copy.
					@return  bool {\tt true} if {\em *this} compositeDescriptor is a deep copy, {\tt false} otherwise
					@see     isShallowCopy
			*/
			bool isDeepCopy() const
				throw();
			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} compositeDescriptor.
					If the internal state of {\em *this} compositeDescriptor is correct 
					(self-validated) and consistent {\tt true} is returned, {\tt false} otherwise. 
					This method checks whether all shallow copies of {\em *this} 
					compositeDescriptor are connected to {\em *this} compositeDescriptor.
					@return			bool {\tt true} if the internal state of {\em *this} compositeDescriptor is correct (self-validated) and consistent, {\tt false} otherwise
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current internal state of {\em *this} compositeDescriptor to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the internal state of {\em *this} compositeDescriptor
					@param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			/**	@name	Storers
			*/
			//@{

			/** Persistent stream output and state restorage.
  			  Read persistent compositeDescriptor data from the input stream {\em s} and 
				  restore the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s input stream from where to restore the internal state of {\em *this} compositeDescriptor
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			  Write persistent compositeDescriptor data to the output stream {\em s} and 
				  store the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s output stream to where to store the internal state of {\em *this} compositeDescriptor
			*/
			virtual void write(std::ostream& s) const
				throw();
			//@}
		

			private:
			
			void drawDirectStatic_(bool with_names = false);

			void drawDirectDynamic_(bool with_names = false);

			void addShallowCompositeDescriptor_
				(CompositeDescriptor* composite_descriptor);

			void removeShallowCompositeDescriptor_
				(CompositeDescriptor* composite_descriptor);

			void setParentCompositeDescriptor_
				(CompositeDescriptor* composite_descriptor);

			CompositeDescriptor* getParentCompositeDescriptor_();

			String name_;
			String *name_ptr_;

			Vector3 center_;
			Vector3 *center_ptr_;

			Quaternion quaternion_;
			Quaternion* quaternion_ptr_;

			Composite* composite_;

			typedef HashMap<GLPrimitiveManager *, GLEntityDescriptor *>
				EntityHashMap;

			EntityHashMap entities_;
			GLObjectCollector* object_collector_;
			GLPrimitiveManager* primitive_manager_;

			bool shallow_copy_;

			List<CompositeDescriptor*> shallow_copies_;
			CompositeDescriptor* parent_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/KERNEL/compositeDescriptor.iC>
#		endif 

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_KERNEL_COMPOSITEDESCRIPTOR_H

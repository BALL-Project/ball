// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: compositeDescriptor.h,v 1.16 2003/03/14 11:50:13 sturm Exp $

#ifndef BALL_VIEW_GUI_KERNEL_COMPOSITEDESCRIPTOR_H
#define BALL_VIEW_GUI_KERNEL_COMPOSITEDESCRIPTOR_H

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
#	include <BALL/DATATYPE/list.h>
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
    /**  \addtogroup  ViewGuiKernelComposite
     *  @{
     */
		/** CompositeDescriptor class.
				The class CompositeDescriptor is responsible for maintaining
				the graphical representation stored in  \link GLEntityDescriptor GLEntityDescriptor \endlink  objects of 
				a given  \link Composite Composite \endlink .
				This class is part of the render engine for creating the graphical representation.
				Certain methods create, compile, update and maintain the visualization.
				This class will be used internally by the  \link MainControl MainControl \endlink  class for
				handling  \link Composite Composite \endlink  objects.
				All necessary information concerning visualization of a  \link Composite Composite \endlink  like
				name, center vector and rotation axis are available in this class.
				Further this class creates, updates and maintains  \link GLEntityDescriptor GLEntityDescriptor \endlink 
				objects for every  \link Scene Scene \endlink .
				These  \link GLEntityDescriptor GLEntityDescriptor \endlink  contain precompiled display lists of the
				 \link Composite Composite \endlink  of {\em *this} compositeDescriptor for faster visualization.
				It is possible to create a shallow copy of {\em *this} compositeDescriptor.
				Such a shallow copy has only a link to its parent compositeDescriptor. It shares
				all attributes including the  \link Composite Composite \endlink  and the graphical representation 
				with its parent.
				If the parent of such a shallow copy will be deleted the shallow copy will be
				deleted as well because all attributs are no longer available. This mechanism
				will work automatically.  \par
				<b>Defintion:</b> BALL/VIEW/GUI/KERNEL/compositeDescriptor.h
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

						- name is set to "unknown"
						- center vector is set (0.0,0.0,0.0)
						- quaternion is set to (0.0,0.0,0.0,1.0)
						- composite pointer is set to 0
						- entities are cleared
						- objectCollector pointer is set to 0
						- primitiveManager pointer is set to 0
						- {\em *this} is shallow copy
						- no shallow copies
						- no parent 
					
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
					is also copied deep. If the flag {\em deep} is set to <tt>false</tt> then
					only a link to the given compositeDescriptor
					{\em composite_descriptor} is stored in this new compositeDescriptor.
					In the case of a shallow copy all the other attributes of {\em *this}
					compositeDescriptor are linked to {\em composite_descriptor} compositeDescriptor.
					If the parent compositeDescriptor of a shallow compositeDescriptor is deleted
					so the shallow compositeDescriptor will also be deleted because all the 
					necessary attributes and the  \link Composite Composite \endlink  are originally stored in the parent
					compositeDescriptor.
					@param  composite_descriptor the compositeDescriptor to be copied (cloned)
					@param  deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em composite_descriptor}
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
					Calls  \link destroy destroy \endlink .
					@see         destroy
			 */
			virtual ~CompositeDescriptor()
				throw();

			/** Explicit default initialization.
					Clears all  \link GLEntityDescriptor GLEntityDescriptor \endlink  objects of {\em *this} compositeDescriptor 
					and resets the internal linkage.
					Calls  \link GLEntityDescriptor::clear GLEntityDescriptor::clear \endlink .
					@see     GLEntityDescriptor::clear
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Destroy {\em *this} compositeDescriptor.
					Deletes all graphical representation stored in  \link GLEntityDescriptor GLEntityDescriptor \endlink  objects
					for the  \link Composite Composite \endlink  of {\em *this} compositeDescriptor. Deletes all shallow
					copies of {\em *this} compositeDescriptor.
					If {\em *this} compositeDescriptor was a shallow copy of another 
					compositeDescriptor it will be removed from it. If {\em *this} compositeDescriptor
					was a deep copy of another compositeDescriptor the  \link Composite Composite \endlink 
					of {\em *this} compositeDescriptor will be deleted.
					Calls  \link clear clear \endlink 
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
					Calls  \link destroy destroy \endlink 
					@param  composite_descriptor the compositeDescriptor to be copied (cloned)
					@param  deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy of {\em composite_descriptor}
					@see    CompositeDescriptor
					@see    destroy
			*/
			void set(CompositeDescriptor& composite_descriptor, bool deep = true)
				throw();

			/** Assignment operator.
					Assign the compositeDescriptor {\em composite_descriptor} to {\em *this}
					compositeDescriptor.
					The assignment is deep.
					Calls  \link set set \endlink .
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
					Calls  \link set set \endlink .
					@param  composite_descriptor the compositeDescriptor to be assigned to
					@see    set
			*/
			void get(CompositeDescriptor& composite_descriptor, bool deep = true)
				throw();
			
			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change of the CompositeDescriptor's name.
					Change the name of {\em *this} compositeDescriptor to the parameter {\em name}.
					Every graphical representation of the  \link Composite Composite \endlink  of {\em *this}
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
					For further information see  \link getName getName \endlink .
			*/
			const String& getName() const
				throw();

			/** Change of the CompositeDescriptor's center vector.
					Change the center vector of {\em *this} compositeDescriptor to  \link Vector3 Vector3 \endlink 
					{\em center}.
					The center vector is the vector that is the origin of the graphical
					representation of the  \link Composite Composite \endlink  of {\em *this}	compositeDescriptor.
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
					For further information see  \link getCenter getCenter \endlink .
			*/
			const Vector3& getCenter() const
				throw();

			/** Change of the CompositeDescriptor's quaternion.
					Change the  \link Quaternion Quaternion \endlink  of {\em *this} compositeDescriptor to the
					 \link Quaternion Quaternion \endlink  {\em quaternion}.
					The  \link Quaternion Quaternion \endlink  describes the rotation of the graphical representation 
					of the  \link Composite Composite \endlink  of {\em *this}	compositeDescriptor.
					@param  quaternion the new quaternion
					@see    getQuaternion
					@see    Quaternion
			*/
			void setQuaternion(const Quaternion& quaternion)
				throw();

			/** Change of the CompositeDescriptor's quaternion.
					Change the  \link Quaternion Quaternion \endlink  of {\em *this} compositeDescriptor to the
					 \link Quaternion Quaternion \endlink  constructed from {\em axis} and {\em angle}.
					The  \link Quaternion Quaternion \endlink  describes the rotation of the graphical representation 
					of the  \link Composite Composite \endlink  of {\em *this}	compositeDescriptor.
					@param  axis the new quaternion axis
					@param  angle the new quaternion angle
					@see    getQuaternion
					@see    Quaternion
					@see    Vector3
			*/
			void setQuaternion(const Vector3& axis, const Real angle)
				throw();

			/** Change of the CompositeDescriptor's quaternion.
					Change the  \link Quaternion Quaternion \endlink  of {\em *this} compositeDescriptor to the 
					 \link Quaternion Quaternion \endlink  constructed from the parameter {\em axis} and {\em angle}.
					The  \link Quaternion Quaternion \endlink  describes the rotation of the graphical representation 
					of the  \link Composite Composite \endlink  of {\em *this}	compositeDescriptor.
					@param  axis the new  \link Quaternion Quaternion \endlink  axis
					@param  angle the new  \link Quaternion Quaternion \endlink  angle
					@see    getQuaternion
					@see    Quaternion
					@see    Vector3
			*/
			void setQuaternion(const Vector3& axis, const Angle& angle)
				throw();

			/** Mutable inspection of the quaternion.
					Access a mutable reference of the  \link Quaternion Quaternion \endlink  of {\em *this} compositeDescriptor.
					@return  Quaternion& mutable reference of the  \link Quaternion Quaternion \endlink  of {\em *this} compositeDescriptor
					@see     setQuaternion
					@see     Quaternion
			*/
			Quaternion& getQuaternion()
				throw();

			/** Constant inspection of the quaternion.
					For further information see  \link getQuaternion getQuaternion \endlink .
			*/
			const Quaternion& getQuaternion() const
				throw();

			/** Change of the CompositeDescriptor's composite.
					Change the  \link Composite Composite \endlink  of {\em *this} compositeDescriptor to {\em composite}.
					@param  composite the new  \link Composite Composite \endlink  of {\em *this} compositeDescriptor
					@param  allow_deletion states if the given composite may be deleted by destruction of {\em *this} compositeDescriptor
					@see    getComposite
					@see    Composite
			*/
			void setComposite(const Composite* composite, bool allow_deletion = false)
				throw();

			/** Mutable inspection of the composite.
					Access a mutable pointern to the  \link Composite Composite \endlink  of {\em *this} compositeDescriptor.
					@return  Composite* mutable pointer to the  \link Composite Composite \endlink  of {\em *this} compositeDescriptor
					@see     setComposite
					@see     Composite
			*/
			Composite* getComposite()
				throw();

			/** Non-mutable inspection of the composite.
					For further information see  \link getComposite getComposite \endlink .
			*/
			const Composite* getComposite() const
				throw();
				
			/** Mark all graphical representation for update.
					This method marks the  \link Composite Composite \endlink  of {\em *this} and all shallow copies
					of {\em *this} for update. That means that if the next time the render engine
					wants to draw the graphical representation of {\em *this} compositeDescriptor
					or one of the shallow copies the precompiled lists stored in the current
					 \link GLEntityDescriptor GLEntityDescriptor \endlink  will be rebuild. 
					Call this method if the structure of the  \link Composite Composite \endlink  or its graphical
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

			/** Registers a  \link GLPrimitiveManager GLPrimitiveManager \endlink .
					The  \link GLPrimitiveManager GLPrimitiveManager \endlink  stores some optimized primitives
					for faster rendering. The  \link GLPrimitiveManager GLPrimitiveManager \endlink  is used by 
					 \link GLEntityDescriptor GLEntityDescriptor \endlink  for creating the graphical representation. 
					For each  \link GLPrimitiveManager GLPrimitiveManager \endlink  there will be a new  \link GLEntityDescriptor GLEntityDescriptor \endlink 
					created.
					This method will be called internally from the render engine.
					@param manager the new  \link GLPrimitiveManager GLPrimitiveManager \endlink 
					@see   GLPrimitiveManager
					@see   GLEntityDescriptor
			*/
			void registerPrimitiveManager(const GLPrimitiveManager& manager)
				throw();

			/** Registers a  \link GLObjectCollector GLObjectCollector \endlink .
					The  \link GLObjectCollector GLObjectCollector \endlink  is used by the  \link GLEntityDescriptor GLEntityDescriptor \endlink 
					for collecting all openGL objects from the  \link Composite Composite \endlink  of {\em *this}
					compositeDescriptor.
					This method will be called internally from the render engine.
					@param collector the new  \link GLObjectCollector GLObjectCollector \endlink 
					@see   GLObjectCollector
					@see   GLEntityDescriptor
			*/
			void registerGLObjectCollector(const GLObjectCollector& collector)
				throw();

			/** Create display lists of the graphical representation.
					If the graphical representation of the  \link Composite Composite \endlink  of {\em *this}
					compositeDescriptor does not change often it is better to compile this
					graphical representation into a faster processable structure. The
					 \link GLEntityDescriptor GLEntityDescriptor \endlink  {\em entity} contains the new compiled lists
					of the graphical representation of the  \link Composite Composite \endlink  of {\em *this}
					compositeDescriptor.
					This method will be called internally by the 
					 \link drawEntity drawEntity \endlink  method.
					@param entity the new  \link GLEntityDescriptor GLEntityDescriptor \endlink  that will hold the lists of 
					              the graphical representation of the  \link Composite Composite \endlink  of {\em *this}
												compositeDescriptor
					@see   drawEntity
			*/
			void compileEntity(GLEntityDescriptor* entity)
				throw();

			/** Draws the graphical representation fast. 
					This method uses precompiled lists created with  \link compileEntity compileEntity \endlink  for 
					drawing the graphical representation of the  \link Composite Composite \endlink  of {\em *this}
					compositeDescriptor faster.
					The representation can either be drawn in hires
					quality (default) or in lowres quality used for quicker moving the graphic.
					This method will be called internally from the render engine.
					Calls  \link compileEntity compileEntity \endlink 
					@param dynamic the new drawing mode (<tt>true</tt> - lowres drawing mode, 
					               <tt>false</tt> (Default) - hires drawing mode.
												 See  \link GeometricObject GeometricObject \endlink  for description of the different
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
					given by the  \link GLPrimitiveManager GLPrimitiveManager \endlink  but instead draws the primitives directly.
					Normaly before the drawing there will be the compilation phase that optimizes
					the graphical represenation for faster drawing. This will use a little time but
					the rendering will be faster afterwards. However if the representation changes
					itself often the time used for compilation will slow down the process
					considerably. Another feature of this method is the possibily that each
					graphical object can be named. That is essential for the selection mode.
					This method will be called internally from the render engine.
					@param dynamic the new drawing mode (<tt>true</tt> - lowres drawing mode, 
					               <tt>false</tt> (Default) - hires drawing mode.
												 See  \link GeometricObject GeometricObject \endlink  for description of the different
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
					@return  bool <tt>true</tt> if {\em *this} compositeDescriptor is a shallow copy, <tt>false</tt> otherwise
					@see     isDeepCopy
			*/
			bool isShallowCopy() const
				throw();

			/** Test if compositeDescriptor is a deep copy.
					@return  bool <tt>true</tt> if {\em *this} compositeDescriptor is a deep copy, <tt>false</tt> otherwise
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
					(self-validated) and consistent <tt>true</tt> is returned, <tt>false</tt> otherwise. 
					This method checks whether all shallow copies of {\em *this} 
					compositeDescriptor are connected to {\em *this} compositeDescriptor.
					@return			bool <tt>true</tt> if the internal state of {\em *this} compositeDescriptor is correct (self-validated) and consistent, <tt>false</tt> otherwise
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
    /** @} */
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_KERNEL_COMPOSITEDESCRIPTOR_H

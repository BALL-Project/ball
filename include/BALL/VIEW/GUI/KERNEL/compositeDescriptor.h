// $Id: compositeDescriptor.h,v 1.3 2000/09/23 14:15:03 hekl Exp $

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
				{\bf Defintion:} \URL{BALL/VIEW/GUI/KERNEL/compositeDescriptor.h}
				\\
				{\bf Category:} \Ref{GLEntityDescriptor} container\\
				
				The {\tt CompositeDescriptor} class is a container for a given
				composite and its graphical representation, the class 
				\Ref{GLEntityDescriptor}.
				The class \Ref{Scene} makes use of this class for rendering
				the graphical representation.
				The class \Ref{MainControl} is a container for the class
				CompositeDescriptor.
		*/
		class CompositeDescriptor
		{
			public:

 			BALL_CREATE_DEEP(CompositeDescriptor)

			/**	@name	 Constructors and Destructors
			*/
			//@{

			/** Default Constructor
					Constructs a new CompositeDescriptor.

					@return CompositeDescriptor - new constructed compositeDescriptor
			 */
			CompositeDescriptor();
			
			/** Copy constructor
					Construct new compositeDescriptor by copying the compositeDescriptor
					{\em composite_descriptor}.
					The copy is either deep (default) or shallow. In this case deep means
					that the composite of the given compositeDescriptor {\em composite_descriptor}
					is also copied deep. If the flag {\em deep} is set to {\tt false} then
					only a link to the given compositeDescriptor
					{\em composite_descriptor} is stored in this new compositeDescriptor.
					
					@return CompositeDescriptor - new constructed compositeDescriptor cloned from {\em composite_descriptor}.
			 */
			CompositeDescriptor
				(const CompositeDescriptor& composite_descriptor, bool deep = true);

			/** Destructor.
					Default destruction of {\em *this} compositeDescriptor.
					If the compositeDescriptor is a deep copy from another compositeDescriptor
					then the stored composite will be deleted. If on the other case the
					compositeDescriptor is a shallow copy from another compositeDescriptor then
					all links to that other compositeDescriptor will be deleted.
					Calls \Ref{CompositeDescriptor::destroy}.
					@see CompositeDescriptor::destroy					
			 */
			virtual ~CompositeDescriptor();

			/** Clears all graphical representations \Ref{GLEntityDescriptor} created for
					the stored composite (as in \Ref{CompositeDescriptor::destroy}), deletes 
					all links from shallow copies and if {\em *this}
					is a deep copy, the stored composite will be deleted.
			 */
			virtual void clear();

			/** Clears all graphical representations \Ref{GLEntityDescriptor} created for
					the stored composite.
			 */
			virtual void destroy();
			//@}

			/**	@name	Assignment
			*/
			//@{

			/** Assignment with cloning facility.
					Assign {\em composite_descriptor} to {\em *this} instance.
					The assignment is either deep (default) or shallow.
					@param  composite_descriptor the composite_descriptor to be copied (cloned)
					@param  deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em composite_descriptor}
			*/
			void set
				(CompositeDescriptor& composite_descriptor, bool deep = true);

			/** Assignment operator.
					Assign {\em composite_descriptor} to {\em *this} instance.
					The assignment is deep.
					@param   composite_descriptor the CompositeDescriptor to be copied (cloned)
					@return  CompositeDescriptor& - {\em *this} CompositeDescriptor
					@see     CompositeDescriptor::set
			*/
			CompositeDescriptor& operator =
				(CompositeDescriptor& composite_descriptor);

			/** Copying with cloning facility.
					Copy {\em *this} to {\em composite_descriptor}.
					The assignment is either deep (default) or shallow.
					@param  composite_descriptor the CompositeDescriptor to be assigned to
					@see    CompositeDescriptor::set
			*/
			void get
				(CompositeDescriptor& composite_descriptor, bool deep = true);
			
			/** Swapping of CompositeDescriptors.
					Swap the states of {\em *this} with {\em composite_descriptor}.
					@param  composite_descriptor the CompositeDescriptor {\em *this} is being swapped with
			*/
			void swap(CompositeDescriptor& composite_descriptor);
			//@}

			/**	@name	Accessors
			*/
			//@{

			/** Change of the CompositeDescriptor's name.
					Change the name of {\em *this} to {\em name}.
					@param  name the new name
					@see    CompositeDescriptor::getName
			*/
			void setName(const String& name);

			/** Mutable inspection of the name.
					Access a mutable reference to the name of {\em *this}.
					@return  String& - mutable reference to the name
					@see     CompositeDescriptor::setName
			*/
			String& getName();

			/** Constant inspection of the name.
					Access a constant reference to the name of {\em *this}.
					@return  String& - constant reference to the name of {\em *this}
					@see     CompositeDescriptor::setName
			*/
			const String& getName() const;

			/** Change of the CompositeDescriptor's center.
					Change the center of {\em *this} to {\em center}.
					@param  center the new center
					@see    CompositeDescriptor::getCenter
			*/
			void setCenter(const Vector3& center);

			/** Mutable inspection of the center.
					Access a mutable reference to the center of {\em *this}.
					@return  Vector3& - mutable reference to the center
					@see     CompositeDescriptor::setCenter
			*/
			Vector3& getCenter();

			/** Constant inspection of the center.
					Access a constant reference to the center of {\em *this}.
					@return  Vector3& - constant reference to the center of {\em *this}
					@see     CompositeDescriptor::setCenter
			*/
			const Vector3& getCenter() const;

			/** Change of the CompositeDescriptor's quaternion.
					Change the quaternion of {\em *this} to {\em quaternion}.
					@param  quaternion the new quaternion
					@see    CompositeDescriptor::getQuaternion
			*/
			void setQuaternion(const Quaternion& quaternion);

			/** Change of the CompositeDescriptor's quaternion.
					Change the quaternion of {\em *this} to the \Ref{Quaternion} constructed
					from {\em axis} and {\em angle}.
					@param  axis the new quaternion axis
					@param  angle the new quaternion angle
					@see    CompositeDescriptor::getQuaternion
			*/
			void setQuaternion(const Vector3& axis, const Real angle);

			/** Change of the CompositeDescriptor's quaternion.
					Change the quaternion of {\em *this} to the \Ref{Quaternion} constructed
					from {\em axis} and {\em angle}.
					@param  axis the new quaternion axis
					@param  angle the new quaternion angle
					@see    CompositeDescriptor::getQuaternion
			*/
			void setQuaternion(const Vector3& axis, const Angle& angle);

			/** Mutable inspection of the quaternion.
					Access a mutable reference to the quaternion of {\em *this}.
					@return  Quaternion& - mutable reference to the quaternion
					@see     CompositeDescriptor::setQuaternion
			*/
			Quaternion& getQuaternion();

			/** Constant inspection of the quaternion.
					Access a constant reference to the quaternion of {\em *this}.
					@return  Quaternion& - constant reference to the quaternion of {\em *this}
					@see     CompositeDescriptor::setQuaternion
			*/
			const Quaternion& getQuaternion() const;

			/** Change of the CompositeDescriptor's composite.
					Change the composite of {\em *this} to {\em composite}.
					@param  composite the new composite
					@param  allow_deletion states if the given composite may be deleted by destruction of {\em *this}
					@see    CompositeDescriptor::getComposite
			*/
			void setComposite(const Composite* composite, bool allow_deletion = false);

			/** Mutable inspection of the composite.
					Access a mutable reference to the composite of {\em *this}.
					@return  Composite* - mutable reference to the composite
					@see     CompositeDescriptor::setComposite
			*/
			Composite* getComposite();

			/** Constant inspection of the composite.
					Access a constant reference to the composite of {\em *this}.
					@return  Composite* - constant reference to the composite of {\em *this}
					@see     CompositeDescriptor::setComposite
			*/
			const Composite* getComposite() const;
				
			/** Marks all graphical representation of {\em *this} and all shallow copies
					of {\em *this} for update.
			*/
			void update();

			/** Registers a \Ref{GLPrimitiveManager} the stores some optimized primitives
					for faster rendering. The GLPrimitiveManager is used by \Ref{GLEntityDescriptor}
					for creating the graphical representation. 
					For each \Ref{GLPrimitiveManager} there will be a new \Ref{GLEntityDescriptor}
					stored.
					@param manager the new manager
			*/
			void registerPrimitiveManager(const GLPrimitiveManager& manager);

			/** Registers a \Ref{GLObjectCollector} that is used by the \Ref{GLEntityDescriptor}
					for collecting all openGL objects from the stored composite.
					@param collector the new collector
			*/
			void registerGLObjectCollector(const GLObjectCollector& collector);

			/** Create the graphical representation of the stored composite and stores
					it in the given \Ref{GLEntityDescriptor} {\em entity}.
					@param entity the new entity
			*/
			void compileEntity(GLEntityDescriptor* entity);

			/** Draws the graphical representation corresponding to a registered 
					\Ref{GLPrimitiveManager}. The representation can either be drawn in hires
					quality (default) or in lowres quality used for quicker moving the graphic.
					@param dynamic the new mode
					@see   CompositeDescriptor::registerPrimitiveManager
			*/
			void drawEntity(bool dynamic = false);

			/** Draws the graphical representation corresponding to a registered 
					\Ref{GLPrimitiveManager}. The representation can either be drawn in hires
					quality (default) or in lowres quality used for quicker moving the graphic.
					This version of the drawEntity function does not use the optimized functions
					given by the \Ref{GLPrimitiveManager} but instead draws the primitives directly.
					Normaly before the drawing there will be the compilation phase that optimizes
					the graphical represenation for faster drawing. This will use a little time but
					the rendering will be faster afterwards. However if the representation changes
					itself often the time used for compilation will slow down the hole process
					considerably. Another feat of this function is the possibily that each
					graphical object can be named. That is essential for the selection mode.
					@param dynamic the new mode
					@param with_names the flag if names should be used or not (default)
					@see   CompositeDescriptor::registerPrimitiveManager
			*/
			void drawDirect(bool dynamic = false, bool with_names = false);
			//@}

			/**	@name	Predicates
			*/
			//@{
			
			/** Determines if the compositeDescriptor is a shallow copy.
					@return  bool {\tt true} if shallow copy {\tt false} otherwise
					@see     CompositeDescriptor::isDeepCopy
			*/
			bool isShallowCopy() const;

			/** Determines if the compositeDescriptor is a deep copy.
					@return  bool {\tt true} if deep copy {\tt false} otherwise
					@see     CompositeDescriptor::isShallowCopy
			*/
			bool isDeepCopy() const;
			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} compositeDescriptor.
					If the internal state of {\em *this} compositeDescriptor is correct (self-validated) and 
					consistent {\tt true} is returned, {\tt false} otherwise. 
					@return			bool -
											{\tt true} if the internal state of {\em *this} compositeDescriptor is correct (self-validated) and consistent,
					 						{\tt false} otherwise
			*/
			virtual bool isValid() const;

			/** Internal state dump.
					Dump the current internal state of {\em *this} compositeDescriptor to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s - output stream where to output the internal state of {\em *this} compositeDescriptor
					@param   depth - the dumping depth
			*/
			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			/** Persistent stream input and state restorage.
					Not yet implemented
			*/
			virtual void read(std::istream& s);

			/* Persistent stream output and state storage.
 				 Not yet implemented
			*/
			virtual void write(std::ostream& s) const;
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

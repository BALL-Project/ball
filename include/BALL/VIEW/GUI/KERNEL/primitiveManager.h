// $Id: primitiveManager.h,v 1.1.2.1 2002/11/23 17:39:45 amoll Exp $

#ifndef BALL_VIEW_GUI_KERNEL_PRIMITIVEMANAGER_H
#define BALL_VIEW_GUI_KERNEL_PRIMITIVEMANAGER_H

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_CONCEPT_KERNEL_H
# include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
# include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_DATATYPE_HASHSET_H
# include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_DATATYPE_LIST_H
# include <BALL/DATATYPE/list.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/**	PrimitiveManager class.
		 		Every composite stored in a PrimitiveManager has at least one GeometricObject.
				{\bf Definition:}\URL{BALL/VIEW/GUI/KERNEL/primitiveManager.h}
		*/
		class PrimitiveManager
		{
			public:
							
			typedef List<GeometricObject*>::iterator List_it;
			typedef List<GeometricObject*>::const_iterator List_const_it;
			typedef pair<List_it, List_it> It_pair;			
			typedef HashMap<Composite*, It_pair>::Iterator Map_it;			

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
			*/
			PrimitiveManager()
				throw();
			
			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~PrimitiveManager()
				throw();

			/** Explicit destructor.
			*/
			virtual void destroy()
				throw();

			//@}
			/**	@name	Accessors
			*/
			//@{

			bool hasComposite(const Composite* composite) const
				throw();
							
			bool hasGeometricObject(const GeometricObject* object) const
				throw();
						
			bool insertGeometricObject(GeometricObject* object, Composite* composite)
				throw();
								
			bool removeGeometricObject(GeometricObject* object)
				throw();
							
			/** Apply a processor to all geometric objects of a composite
			 */
			bool applyToGeometricObjects(UnaryProcessor<GeometricObject>& processor, Composite* composite = 0)
				throw();
						
			bool applyToAllGeometricObjects(UnaryProcessor<GeometricObject>& processor)
				throw();
								
			/** Apply a processor to all composites 
			 */
			bool applyToAllComposites(UnaryProcessor<Composite>& processor)
				throw();

			Size countGeometricObjects(Composite* composite = 0) const
				throw();
								
			Size getNumberOfGeometricObjects() const
				throw();

						
			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{
			/** Internal state and consistency self-validation.
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of {\em *this} PrimitiveManager to 
					the output ostream {\em s} with dumping depth {\em depth}.
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			//@}
			/**	@name	Storers
			*/
			//@{

			/** Persistent stream output and state restorage.
  			  Read persistent PrimitiveManager data from the input stream {\em s} and 
				  restore the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s input stream from where to restore the internal state of {\em *this} PrimitiveManager
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			  Write persistent PrimitiveManager data to the output stream {\em s} and 
				  store the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s output stream to where to store the internal state of {\em *this} PrimitiveManager
			*/
			virtual void write(std::ostream& s) const
				throw();
			//@}

			private:

			List_it getFirstIterator(Composite* composite)
				throw();
							
			List_it getSecondIterator(Composite* composite)
				throw();

			List_const_it getFirstIterator(Composite* composite) const
				throw();
							
			List_const_it getSecondIterator(Composite* composite) const
				throw();



						
			List<GeometricObject*> 				objects_list_;		
						
			// used to prevent multiple insertion of same GeometricObject			
			HashSet<GeometricObject*>			object_set_;						
						
			HashMap<Composite*, It_pair> 	composite_to_objects_;			
		};

//#		ifndef BALL_NO_INLINE_FUNCTIONS
//#			include <BALL/VIEW/GUI/KERNEL/PrimitiveManager.iC>
//#		endif 


	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_GUI_KERNEL_PrIMITIVEMANAGER_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: tube.h,v 1.3 2003/08/29 10:53:56 amoll Exp $
//

#ifndef BALL_VIEW_PRIMITIV_TUBE_H
#define BALL_VIEW_PRIMITIV_TUBE_H

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_DATATYPE_VERTEX2_H
#	include <BALL/VIEW/DATATYPE/vertex2.h>
#endif

namespace BALL
{
	namespace VIEW
	{
         
		/** Tube class.				
				An instance of Tube represents an instance of the geometric representation "tube".
				A tube has the following properties. 
				  - color   - the color of the tube
					- radius  - the radius of the tube
					- vertex1 - the first vertex of the tube
					- vertex2 - the second vertex of the tube
				\par
				The class Tube is derived from the classes GeometricObject
				Radius and Vertex2. 
				See these classes for further information concerning
				interface and additional methods. \par
		*/
		class Tube
			: public GeometricObject,
   			public Vertex2
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new tube.
					The properties of this tube are set to:
  				  - color   - to the color black
						- radius  - set to 1
		  			- vertex1 - to the vector (0,0,0)
		  			- vertex2 - to the vector (0,0,0)
					\par
					\return      Tube new constructed tube
					\see         GeometricObject
					\see         Radius
					\see         Vertex2
			*/
			Tube()
				throw();

			/** Copy constructor with cloning facility.
					\see         GeometricObject
					\see         Radius
					\see         Vertex2
			*/
			Tube(const Tube& tube)
				throw();

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of this tube.
			*/
			virtual ~Tube()
				throw();

			/** Explicit default initialization.
					Calls GeometricObject::clear
					Calls Radius::clear
					Calls Vertex2::clear
					\see  GeometricObject::clear
					\see  Radius::clear
					\see  Vertex2::clear
			*/
			virtual void clear()
				throw();

			//@}

			/**	@name	Assignment methods */
			//@{

			/** Assignment.
					\param       tube the tube to be copied
					\see         Tube
			*/
			void set(const Tube& tube)
				throw();

			/** Assignment operator.
					Assign the tube <b> tube</b> to this tube.
					Calls set.
					\param       tube the tube to be copied
					\return      Tube& this tube
					\see         set
			*/
			const Tube& operator = (const Tube& tube)
				throw();

			/** Copying with cloning facility.
					Copy this tube to the tube <b> tube</b>.
					Calls set.
					\param       tube the tube to be assigned to
					\see         set
			*/
			void get(Tube& tube) const
				throw();

			/** Swapping of tube's.
					Swap the value of this tube with the tube <b> tube</b>.
					\param       tube the tube being swapped with this tube 
			*/
			void swap(Tube& tube)
				throw();

			//@}
			/**	@name	Accessors: inspectors and mutators */
			//@{

			/** Inspection of the length of the tube.
					Access the length of this tube.
					\return  Real the length of this tube
			*/
			Real getLength() const
				throw() {return (getVertex1() - getVertex2()).getLength();}

			/** Inspection of the square length of the tube.
					Access the square length of this tube.
					\return  Real the square length of this tube
			*/
			Real getSquareLength() const
				throw() {return (getVertex1() - getVertex2()).getSquareLength();}

			//@}
			/**	@name	debuggers and diagnostics */
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of this tube.
					If the internal state of this tube is correct (self-validated) and 
					consistent <tt> true</tt> is returned, <tt> false</tt> otherwise. 
					Calls GeometricObject::isValid.
					Calls Vertex2::isValid.
					\return			bool <tt> true</tt> if the internal state of this tube is correct 
											(self-validated) and consistent, <tt> false</tt> otherwise
					\see        GeometricObject::isValid
					\see        Vertex2::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current value of this tube to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					Calls GeometricObject::dump.
					Calls Radius::dump.
					Calls Vertex2::dump.
					\param   s output stream where to output the value of this tube
					\param   depth the dumping depth
					\see        GeometricObject::dump
					\see        Radius::dump
					\see        Vertex2::dump
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			//@}
			/**	@name	Storers */
			//@{

			/**	Get the radius.
			 */
			Real getRadius() const
				throw() { return radius_;}

			/** Set the radius.
			 */
			void setRadius(Real radius)
				throw() { radius_ = radius;}

			//@}
				
			private:

			Real radius_;
		};

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_PRIMITIV_TUBE_H

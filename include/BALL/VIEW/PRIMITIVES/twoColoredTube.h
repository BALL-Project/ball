// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: twoColoredTube.h,v 1.11.16.1 2007-03-25 21:26:12 oliver Exp $
//

#ifndef BALL_VIEW_PRIMITIVES_TWOCOLOREDTUBE_H
#define BALL_VIEW_PRIMITIVES_TWOCOLOREDTUBE_H

#ifndef BALL_VIEW_DATATYPE_COLOREXTENSIONS_H
#	include <BALL/VIEW/DATATYPE/colorExtensions.h>
#endif

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
		/** TwoColoredTube class.
				An instance of TwoColoredTube represents an instance of the geometric
				representation of a two colored "tube".
				A twoColoredTube has the following properties. 
				  - color1 - the color for the first vertex of the tube
				  - color2 - the color for the second vertex of the tube
					- vertex1 - the first vertex of the tube
					- vertex2 - the second vertex of the tube
				\par
				The class TwoColoredTube is a tube that is half colored in the color 1 and
				in color 2. The tube is exactly divided in the middle. One half (starting
				from vertex1 til middle vertex) is colored by the color1 and the other half 
				(starting from middle vertex to vertex2) is colored by color2.
				The class TwoColoredTube is derived from the classes GeometricObject,
				ColorExtension2, Radius	and Vertex2.
				See these classes for further information concerning
				interface and additional methods. \par
				\ingroup ViewPrimitives
		*/
		class BALL_VIEW_EXPORT TwoColoredTube
			: public GeometricObject,
			  public ColorExtension2,
   			public Vertex2
		{
			public:

			BALL_CREATE(TwoColoredTube)

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new twoColoredTube.
					The properties of this twoColoredTube are set to:
  				  - color1 - set to the color black
						- color2 - set to the color black
						- radius - set to 1
		  			- vertex1 - set to the vector (0,0,0)
		  			- vertex2 - set to the vector (0,0,0)
					\par
					\return      TwoColoredTube new constructed twoColoredTube
					\see         GeometricObject
					\see         ColorExtension2
					\see         Vertex2
			*/
			TwoColoredTube();

			/** Copy constructor with cloning facility.
					\see         GeometricObject
					\see         ColorExtension2
					\see         Vertex2
			*/
			TwoColoredTube(const TwoColoredTube& two_colored_tube);

			//@}
			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of this twoColoredTube.
			*/
			virtual ~TwoColoredTube();

			/** Explicit default initialization.
					Calls GeometricObject::clear.
					Calls ColorExtension2::clear.
					Calls Vertex2::clear.
					\see  GeometricObject::clear
					\see  ColorExtension2::clear
					\see  Radius::clear
					\see  Vertex2::clear
			*/
			virtual void clear();
			
			//@}
			/**	@name	Assignment methods */
			//@{

			/** Assignment.
					Calls GeometricObject::set.
					Calls ColorExtension2::set.
					Calls Vertex2::set.
			*/
			void set(const TwoColoredTube& two_colored_tube);

			/** Assignment operator.
					\see         set
			*/
			const TwoColoredTube& operator = (const TwoColoredTube& two_colored_tube);

			/** Swapping of two_colored_tube's.
			*/
			void swap(TwoColoredTube& two_colored_tube);

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Inspection of the length of the tube.
					Access the length of this tube.
					\return  float the length of this tube
			*/
			float getLength() const;

			/** Inspection of the middle vector.
					Access the geometric middle vertex between vertex1 and vertex2 of
					this twoColoredTube.
					\return. Vector3 the middle vertex between vertex1 and vertex 2.
					\see     Vector3
			*/
			Vector3 getMiddleVertex() const;

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure
					consistencies	of this twoColoredTube.
					If the internal state of this twoColoredTube is correct 
					(self-validated) and 
					consistent <tt> true</tt> is returned, <tt> false</tt> otherwise. 
					Calls {GeometricObject::isValid}.
					\return			bool <tt> true</tt> if the internal state of this twoColoredTube 
											is correct (self-validated) and consistent, <tt> false</tt> otherwise
					\see        GeometricObject::isValid
			*/
			virtual bool isValid() const;

			/** Internal value dump.
					Dump the current value of this twoColoredTube to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					Calls GeometricObject::dump.
					Calls ColorExtension2::dump.
					Calls Radius::dump.
					Calls Vertex2::dump.
					\param   s output stream where to output the value of this twoColoredTube
					\param   depth the dumping depth
					\see        GeometricObject::dump
					\see        ColorExtension2::dump
					\see        Radius::dump
					\see        Vertex2::dump
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

			//@}
			/**	@name	Storers */
			//@{

			///	Get the radius.
			float getRadius() const
				{ return radius_;}

			/// Set the radius.
			void setRadius(float radius)
				{ radius_ = radius;}
			
			// Method to get all vertices from a geometric object
			virtual void getVertices(vector<Vector3>& vertices) const;

			//@}
			
			private:

			float radius_;
		};

	#ifndef BALL_NO_INLINE_FUNCTIONS
	# include <BALL/VIEW/PRIMITIVES/twoColoredTube.iC>
	#endif

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_PRIMITIVES_TWOCOLOREDTUBE_H

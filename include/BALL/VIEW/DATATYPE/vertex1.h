// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vertex1.h,v 1.10.20.1 2007/03/25 21:25:42 oliver Exp $
//

#ifndef BALL_VIEW_DATATYPE_VERTEX1_H
#define BALL_VIEW_DATATYPE_VERTEX1_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL
{
	namespace VIEW
	{
		/** Vertex is a base class for all GeometricObject 's that
				have a single vertex. It provides the derived class with methods for accessing
				that vertex. Further there is the possibility to give an address to a 
				vector Vector3 as vertex. So if the value of that given vector
				changes the value of this vertex changes as well. To avoid 
				segmentation faults this vector address must be valid as long as 
				this vertex exists. \par
				\ingroup ViewDatatypeGeometricObject
		*/
		class BALL_VIEW_EXPORT Vertex
		{
			public:

			BALL_CREATE(Vertex)

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					The vector of this vertex is set to (0.0, 0.0, 0.0). The vertex
					address is set to the address of the own vector of this vertex.
			*/
			Vertex();

			/** Copy constructor.
					The vector of <b> vertex</b> is copied to the vector of this vertex.
					The vertex address of this vertex is set to the value of 
					the vertex address of <b> vertex</b> vertex.
			*/
			Vertex(const Vertex& vertex);

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			*/
			virtual ~Vertex();

			/** Explicit default initialization.
					Set the vector of this vertex to the vector (0.0, 0.0, 0.0).
					The vertex address of this vertex is set to the address of 
					the own vector of this vertex.
			*/
			virtual void clear();

			//@}
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					The vector and the vertex address of this vertex is initialized 
					to the vector and vertex address of the vertex <b> v</b>.\par
			*/
			void set(const Vertex& v);

			/** Assignment operator.
					Calls set.
					The vector and the vertex address of this vertex is initialized
					to the vector and the vertex address of the vertex <b> v</b>.\par
			*/
			const Vertex& operator = (const Vertex& v);

			/** Swapping of vertices.
					Swap the vector and vertex address of this vertex with 
					the vector and vertex address of the vertex	<b> v</b>.
			*/
			void swap(Vertex& v);

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the vector of this vertex.
					\see         getVertex
			*/
			void setVertex(const Vector3& v);

			/** Change the vector of this vertex.
					\see         getVertex
			*/
			void setVertex(const float x, const float y, const float z);

			/** Mutable inspection of the vector of this vertex.
					\see         setVertex
			*/
			Vector3& getVertex();

			/** Non-mutable inspection of the vector of this vertex.
					For further information see getVertex.
			*/
			const Vector3& getVertex() const;

			/** Inspection of the vector of this vertex.
					\see         setVertex
			*/
			void getVertex(Vector3& v) const;

			/** Access the components of the vector of this vertex by using float.
					\see         setVertex
			*/
			void getVertex(float& x, float& y, float& z) const;

			/** Change the vector address of this vertex to the vector address
					represented by the parameter <b> v</b>.
					If a vector address is given the value of the vector of this vertex
					is ignored. Instead if the method getVertex (or any other access method)
					is called the value of the vector given by the vertex address is returned.
					The vector to which the vertex address points must exist and be valid as
					long as this vertex exists. An object that uses this method can
					hook itself onto another object (speaking in terms of position). If the
					object changes its position so the object derived from this vertex
					changes its position.
					\param       v the new vector address of this vertex
					\see         getVertexAddress
			*/
			void setVertexAddress(const Vector3& v);
		
			/** Change the vector address of this vertex to the default address.
					This method resets the vertex address to the vector address of
					this vertex. So the value of the vector of this vertex 
					will no longer be ignored and all access methods will return it again.
					This method unhooks the object from any other objects prio connected
					with setVertexAddress.
					\see         setVertexAddress
			*/
			void setDefaultVertexAddress();

			/** Access the pointer of the vector that contains the value of
					this	vertex.
					\see         setVertexAddress
			*/
			Vector3* getVertexAddress() const;

			//@}
			/**	@name	Predicates
			*/
			//@{

			/** Vertex address test.
					Tests if the vertex address of this vertex points to the vector
					of this vertex.
					\see        setVertexAddress					
					\see        getVertexAddress					
					\see        setDefaultVertexAddress					
			*/
			bool isDefaultVertexAddress() const;

			//@}
			/**	@name	debuggers and diagnostics
			*/	
			//@{

			/** Internal state and consistency self-validation.
					Calls Vector3::isValid
			*/
			virtual bool isValid() const;

			/** Internal value dump.
					Dump the current state of this vertex to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the state of this vertex
					\param   depth the dumping depth
					\see     Vector3::operator <<
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

			//@}

			private:
		
			/* vertex */
			Vector3 vertex1_;

			/* vertex pointer */
			Vector3* vertex1_ptr_;
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/DATATYPE/vertex1.iC>
#		endif
  
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_DATATYPE_VERTEX1_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vertex1.h,v 1.3 2003/08/29 15:36:45 amoll Exp $
//

#ifndef BALL_VIEW_KERNEL_VERTEX1_H
#define BALL_VIEW_KERNEL_VERTEX1_H

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
		/** Vertex class.
				The class Vertex is used as a base class for all geometric objects that
				have a single vertex. It provides the derived class with methods for accessing
				that vertex. Further there is the possibility to give an address to a 
				vector Vector3 as vertex. So if the value of that given vector
				changes the value of this vertex changes as well. To avoid 
				segmentation faults this vector address must be valid as long as 
				this vertex exists. \par
		*/
		class Vertex
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new vertex.
					The vector of this vertex is set to (0.0, 0.0, 0.0). The vertex
					address is set to the address of the own vector of this vertex.
					\return      Vertex new constructed vertex
					\see         Vector3
			*/
			Vertex()
				throw();

			/** Copy constructor.
					Construct new vertex by copying the vertex <b> vertex</b>. The vector of
					<b> vertex</b> is copied to the vector of this vertex.
					The vertex address of this vertex is set to the value of 
					the vertex address of <b> vertex</b> vertex.
					\param       vertex the vertex to be copied
					\return      Vertex new constructed vertex copied from <b> vertex</b>
			*/
			Vertex(const Vertex& vertex)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of this vertex.
			*/
			virtual ~Vertex()
				throw();

			/** Explicit default initialization.
					Set the vector of this vertex to the vector (0.0, 0.0, 0.0).
					The vertex address of this vertex is set to the address of 
					the own vector of this vertex.
			*/
			virtual void clear()
				throw();

			//@}
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the vertex <b> v</b> to this vertex.
					The vector and the vertex address of this vertex is initialized 
					to the vector and vertex address of the vertex <b> v</b>.\par
					\param       v the vertex to be copied
					\see         setVertex
					\see         setVertexAddress
					\see         Vector3
			*/
			void set(const Vertex& v)
				throw();

			/** Assignment operator.
					Calls set.
					The vector and the vertex address of this vertex is initialized
					to the vector and the vertex address of the vertex <b> v</b>.\par
					\param       v the vertex to be copied
					\return      Vertex& constant reference of this vertex
					\see         set
			*/
			const Vertex& operator = (const Vertex& v)
				throw();

			/** Swapping of vertices.
					Swap the vector and vertex address of this vertex with 
					the vector and vertex address of the vertex	<b> v</b>.
					\param       v the vertex being swapped with this vertex 
					\see         Vertex
			*/
			void swap(Vertex& v)
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the vector of this vertex.
					Change the vector of this vertex to the vector
					represented by the parameter <b> v</b>.
					\param       v the new vector of this vertex
					\see         getVertex
					\see         Vector3
			*/
			void setVertex(const Vector3& v)
				throw();

			/** Change the vector of this vertex.
					Change the vector of this vertex to the vector represented by the
					parameters <b> x</b>, <b> y</b> and <b> z</b>.
					\param       x the x component of the new vector of this vertex
					\param       y the y component of the new vector of this vertex
					\param       z the z component of the new vector of this vertex
					\see         getVertex
					\see         Vector3
					\see         Real
			*/
			void setVertex(const Real x, const Real y, const Real z)
				throw();

			/** Mutable inspection of the vector of this vertex.
					Access the mutual reference of the vector of this vertex.
					\return      Vector3& mutable reference to the vector of this vertex
					\see         setVertex
					\see         Vector3
			*/
			Vector3& getVertex()
				throw();

			/** Non-mutable inspection of the vector of this vertex.
					For further information see getVertex.
			*/
			const Vector3& getVertex() const
				throw();

			/** Inspection of the vector of this vertex.
					Access the vector of this vertex by using Vector3.
					\param       v the vector receiving the vector of this vertex
					\see         setVertex
					\see         Vector3
			*/
			void getVertex(Vector3& v) const
				throw();

			/** Inspection of the components of the vector of this vertex.
					Access the components of the vector of this vertex by using Real.
					\param       x the x component of the vector of this vertex
					\param       y the y component of the vector of this vertex
					\param       z the z component of the vector of this vertex
					\see         setVertex
					\see         Real
			*/
			void getVertex(Real& x, Real& y, Real& z) const
				throw();

			/** Change the vector address of this vertex.
					Change the vector address of this vertex to the vector address
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
					\see         Vector3
			*/
			void setVertexAddress(const Vector3& v)
				throw();
		
			/** Change the vector address of this vertex to the default address.
					This method resets the vertex address to the vector address of
					this vertex. So the value of the vector of this vertex 
					will no longer be ignored and all access methods will return it again.
					This method unhooks the object from any other objects prio connected
					with setVertexAddress.
					\see         setVertexAddress
			*/
			void setDefaultVertexAddress()
				throw();

			/** Mutable inspection of the vertex address of this vertex.
					Access the pointer of the vector that contains the value of
					this	vertex.
					\return      Vector3* pointer to the vector that contains the value of this vertex
					\see         setVertexAddress
					\see         Vector3
			*/
			Vector3* getVertexAddress() const
				throw();

			//@}
			
			/**	@name	Predicates
			*/
			//@{

			/** Vertex address test.
					Tests if the vertex address of this vertex points to the vector
					of this vertex.
					\return			bool <tt> true</tt> if the vertex address of this vertex points to the vector of this vertex.
					 						<tt> false</tt> otherwise
					\see        setVertexAddress					
					\see        getVertexAddress					
					\see        setDefaultVertexAddress					
			*/
			bool isDefaultVertexAddress() const
				throw();
			//@}

			/**	@name	debuggers and diagnostics
			*/	
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of this vertex.
					If the internal state of this vertex is correct (self-validated) and 
					consistent <tt> true</tt> is returned, <tt> false</tt> otherwise. 
					Calls {Vector3::isValid}.
					\return			bool -
											<tt> true</tt> if the internal state of this vertex is correct (self-validated) and consistent,
					 						<tt> false</tt> otherwise
					\see        Vector3::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of this vertex to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the state of this vertex
					\param   depth the dumping depth
					\see     Vector3::operator <<
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
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

#endif // BALL_VIEW_KERNEL_VERTEX1_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vertex2.h,v 1.1 2003/08/26 12:11:01 oliver Exp $
//

#ifndef BALL_VIEW_KERNEL_VERTEX2_H
#define BALL_VIEW_KERNEL_VERTEX2_H

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
		/** Vertex2 class.
				The class Vertex2 is used as a base class for all geometric objects that
				have two vertices. It provides the derived class with methods for accessing
				that vertices. Further there is the possibility to give an address to a 
				vector Vector3 as vertices. So if the values of these given vectors
				changes the values of the vectors of this vertex2 changes 
				as well. To avoid segmentation faults these vector addresses must be 
				valid as long as this vertex2 exists. \par
		*/
		class Vertex2
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new vertex2.
					The vectors of this vertex2 are set to (0.0, 0.0, 0.0). The vertex
					addresses are set to the addresses of the own vectors of this 
					vertex2.
					\return      Vertex2 new constructed vertex2
					\see         Vector3
			*/
			Vertex2()
				throw();

			/** Copy constructor.
					Construct new vertex2 by copying the vertex2 <b> vertex</b>. The vectors of
					<b> vertex</b> are copied to the vectors of this vertex2.
					The vertex addresses of this vertex are set to the values of 
					the vertex addresses of <b> vertex</b> vertex2.
					\param       vertex the vertex2 to be copied
					\return      Vertex2 new constructed vertex2 copied from <b> vertex</b>
			*/
			Vertex2(const Vertex2& vertex)
				throw();

			//@}

			/** @name Destructors 
		  */
			//@{

			/** Destructor.
					Default destruction of this vertex2.
			*/
			virtual ~Vertex2()
				throw();

			/** Explicit default initialization.
					Set the vectors of this vertex2 to the vector (0.0, 0.0, 0.0).
					The vertex addresses of this vertex2 are set to the addresses of 
					the own vectors of this vertex2.
			*/
			virtual void clear()
				throw();

			//@}
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the vertex2 <b> vertex</b> to this vertex2.
					The vectors and the vertex addresses of this vertex2 are initialized 
					to the vectors and vertex addresses of the vertex2 <b> vertex</b>.\par
					\param       vertex the vertex2 to be copied
					\see         setVertex1
					\see         setVertex1Address
					\see         setVertex2
					\see         setVertex2Address
					\see         Vector3
			*/
			void set(const Vertex2& vertex)
				throw();

			/** Assignment operator.
					Assign the vertex2 <b> vertex</b> to this vertex2.
					Calls set.
					The vectors and the vertex addresses of this vertex2 are initialized
					to the vectors and the vertex addresses of the vertex2 <b> vertex</b>.\par
					\param       vertex the vertex2 to be copied
					\return      Vertex2& constant reference of this vertex2
					\see         set
			*/
			const Vertex2& operator = (const Vertex2& vertex)
				throw();

			/** Copying.
					Copy this vertex2 to the vertex2 <b> vertex</b>.
					Calls set.
					The vectors and the vertex addresses of this vertex2 are initialized
					to the vectors and the vertex addresses of the vertex <b> vertex</b>.\par
					\param       vertex the vertex2 to be assigned to
					\see         set
			*/
			void get(Vertex2& vertex) const
				throw();

			/** Swapping of vertices.
					Swap the vectors of this vertex2 with the vertex2 <b> vertex</b>
					and swaps the vertex addresses of this vertex2 with the vertex2
					<b> vertex</b>.
					\param       vertex the vertex2 being swapped with this vertex2 
					\see         Vertex2
			*/
			void swap(Vertex2& vertex)
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the first vector of this vertex2.
					Change the first vector of this vertex2 to the vector
					represented by the parameter <b> v</b>.
					\param       v the new first vector of this vertex2
					\see         getVertex1
					\see         Vector3
			*/
			void setVertex1(const Vector3& v)
				throw();

			/** Change the first vector of this vertex2.
					Change the first vector of this vertex2 to the vector 
					represented by the parameters <b> x</b>, <b> y</b> and <b> z</b>.
					\param       x the x component of the new first vector of this vertex2
					\param       y the y component of the new first vector of this vertex2
					\param       z the z component of the new first vector of this vertex2
					\see         getVertex1
					\see         Vector3
					\see         Real
			*/
			void setVertex1(const Real x, const Real y, const Real z)
				throw();

			/** Mutable inspection of the first vector of this vertex2.
					Access the mutual reference of the first vector of this vertex2.
					\return      Vector3& mutable reference to the first vector of this vertex2
					\see         setVertex1
					\see         Vector3
			*/
			Vector3& getVertex1()
				throw();

			/** Non-mutable inspection of the first vector of this vertex2.
					For further information see getVertex1.
			*/
			const Vector3& getVertex1() const
				throw();

			/** Inspection of the first vector of this vertex2.
					Access the first vector of this vertex by using Vector3.
					\param       v the vector receiving the first vector of this vertex2
					\see         setVertex1
					\see         Vector3
			*/
			void getVertex1(Vector3& v) const
				throw();

			/** Inspection of the components of the first vector of this vertex2.
					Access the components of the first vector of this vertex2 by using Real.
					\param       x the x component of the first vector of this vertex2
					\param       y the y component of the first vector of this vertex2
					\param       z the z component of the first vector of this vertex2
					\see         setVertex1
					\see         Real
			*/
			void getVertex1(Real& x, Real& y, Real& z) const
				throw();

			/** Change the vector address of the first vector of this vertex2.
					Change the vector address of the first vector of this vertex2
					to the vector address	represented by the parameter <b> v</b>.
					If a vector address is given the value of the first vector of this
					vertex2	is ignored. Instead if the method getVertex1 (or any other access
					method concerning the first vector) are called the value of the vector
					given by the vertex address is returned.
					The vector to which the first vertex address points must exist and be
					valid as long as this vertex2 exists.
					An object that uses this method can hook itself onto another object
					(speaking in terms of position). If the	object changes its first
					position so the object derived from this vertex2	changes
					its first position.
					\param       v the new first vector address of this vertex2
					\see         getVertex1Address
					\see         Vector3
			*/
			void setVertex1Address(const Vector3& v)
				throw();

			/** Change the first vector address of this vertex2 to the default address.
					This method resets the first vertex address to the first vector of
					this vertex2. So the value of the first vector of this
					vertex2 will no longer be ignored and all access methods will return 
					it again.
					This method unhooks the object from any other objects prio connected
					with setVertex1Address.
					\see         Vertex::setVertex1Address
			*/
			void setDefaultVertex1Address()
				throw();

			/** Mutable inspection of the first vertex address of this vertex2.
					Access the pointer of the first vector that contains the value of
					this	vertex2.
					\return      Vector3* pointer to the first vector that contains the value of this vertex2
					\see         setVertex1Address
					\see         Vector3
			*/
			Vector3* getVertex1Address() const
				throw();

			/** Change the second vector of this vertex2.
					Change the second vector of this vertex2 to the vector
					represented by the parameter <b> v</b>.
					\param       v the new second vector of this vertex2
					\see         getVertex2
					\see         Vector3
			*/
			void setVertex2(const Vector3& v)
				throw();

			/** Change the second vector of this vertex2.
					Change the second vector of this vertex2 to the vector 
					represented by the parameters <b> x</b>, <b> y</b> and <b> z</b>.
					\param       x the x component of the new second vector of this vertex2
					\param       y the y component of the new second vector of this vertex2
					\param       z the z component of the new second vector of this vertex2
					\see         getVertex2
					\see         Vector3
					\see         Real
			*/
			void setVertex2(const Real x, const Real y, const Real z)
				throw();

			/** Mutable inspection of the second vector of this vertex2.
					Access the mutual reference of the second vector of this vertex2.
					\return      Vector3& mutable reference to the second vector of this vertex2
					\see         setVertex2
					\see         Vector3
			*/
			Vector3& getVertex2()
				throw();

			/** Non-mutable inspection of the second vector of this vertex2.
					For further information see getVertex2.
			*/
			const Vector3& getVertex2() const
				throw();

			/** Inspection of the second vector of this vertex2.
					Access the second vector of this vertex by using Vector3.
					\param       v the vector receiving the second vector of this vertex2
					\see         setVertex2
					\see         Vector3
			*/
			void getVertex2(Vector3& v) const
				throw();

			/** Inspection of the components of the second vector of this vertex2.
					Access the components of the second vector of this vertex2 
					by using Real.
					\param       x the x component of the second vector of this vertex2
					\param       y the y component of the second vector of this vertex2
					\param       z the z component of the second vector of this vertex2
					\see         setVertex2
					\see         Real
			/** Inspection of the first and second vector of this vertex2.
					Access the first and second vector of this vertex 
					by using Vector3.
					\param       vertex1 the vector receiving the first vector of this vertex2
					\param       vertex2 the vector receiving the second vector of this vertex2
					\see         setVertices
					\see         Vector3
			*/
			void getVertices(Vector3& vertex1, Vector3& vertex2)
				throw();

			/** Inspection of the components of the first and second vector of 
					this vertex2.
					Access the components of the first and second vector of this
					vertex2 by using Real.
					\param       vertex1_x the x component of the first vector of this vertex2
					\param       vertex1_y the y component of the first vector of this vertex2
					\param       vertex1_z the z component of the first vector of this vertex2
					\param       vertex2_x the x component of the second vector of this vertex2
					\param       vertex2_y the y component of the second vector of this vertex2
					\param       vertex2_z the z component of the second vector of this vertex2
					\see         setVertices
					\see         Real
			*/
			void getVertices
				(Real& vertex1_x, Real& vertex1_y, Real& vertex1_z,
				 Real& vertex2_x, Real& vertex2_y, Real& vertex2_z)
				throw();

			/** Change the first and second vector address of this vertex2
					to the default addresses.
					This method resets the first and second vertex address to the first 
					and second vector of this vertex2. So the value of the first 
					and second vector of this vertex2 will no longer be ignored and
					all access methods will return them again.
					This method unhooks the object from any other objects prio connected
					with setVertexAddresses.
					\see         Vertex::setVertex2Addresses
			*/
			void setDefaultVertexAddresses()
				throw();
			//@}

			/**	@name	Predicates
			*/
			//@{

			/** first vertex address test.
					Tests if the first vertex address of this vertex2 points to the 
					first vector of this vertex2.
					\return			bool <tt> true</tt> if the first vertex address of this vertex2 points to the first vector of this vertex2.
					 						<tt> false</tt> otherwise
					\see        setVertex1Address					
					\see        getVertex1Address					
					\see        setDefaultVertex1Address					
			*/
			bool isDefaultVertex1Address() const
				throw();

			/** second vertex address test.
					Test if the second vertex address of this vertex2 points to the 
					second vector of this vertex2.
					\return			bool <tt> true</tt> if the second vertex address of this vertex2 points to the second vector of this vertex2.
					 						<tt> false</tt> otherwise
					\see        setVertex2Address					
					\see        getVertex2Address					
					\see        setDefaultVertex2Address					
			*/
			bool isDefaultVertex2Address() const
				throw();

			/** both vertex addresses test.
					Test if both vertex addresses of this vertex2 each points to its
					own vector of this vertex2.
					\return			bool <tt> true</tt> if both vertex addresses of this vertex2 each points to its own vector of this vertex2.
					 						<tt> false</tt> otherwise
					\see        setVertex1Address					
					\see        getVertex1Address					
					\see        setVertex2Address					
					\see        getVertex2Address					
					\see        setDefaultVertex1Address					
					\see        setDefaultVertex2Address					
		  */
			bool isDefaultVertexAddresses() const
				throw();
			//@}

			/**	@name	debuggers and diagnostics
			*/	
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure 
					consistencies	of this vertex2.
					If the internal state of this vertex2 is correct 
					(self-validated) and consistent <tt> true</tt> is returned,
					<tt> false</tt> otherwise. 
					Calls {Vector3::isValid}.
					\return			bool <tt> true</tt> if the internal state of this vertex is correct (self-validated) and consistent,
					 						<tt> false</tt> otherwise
					\see        Vector3::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of this vertex2 to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the state of this vertex2
					\param   depth the dumping depth
					\see     Vector3::operator <<
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}


			private:
		
			/* vertex 1 */
			Vector3 vertex1_;

			/* vertex 2 */
			Vector3 vertex2_;

			/* vertex 1 pointer */
			Vector3* vertex1_ptr_;

			/* vertex 2 pointer */
			Vector3* vertex2_ptr_;
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/vertex2.iC>
#		endif
  
	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_KERNEL_VERTEX2_H

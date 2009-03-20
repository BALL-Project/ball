// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vertex2.h,v 1.12 2005/12/23 17:02:08 amoll Exp $
//

#ifndef BALL_VIEW_DATATYPE_VERTEX2_H
#define BALL_VIEW_DATATYPE_VERTEX2_H

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

		/** Vertex2 is a base class for all GeometricObject 's that
				have two vertices. It provides the derived class with methods for accessing
				that vertices. Further there is the possibility to give an address to a 
				Vector3 as vertice. So if the values of these given vectors
				changes, the values of the vectors  changes 
				as well. To avoid segmentation faults these vector addresses must be 
				valid as long as the Vertex2 exists. \par
				\ingroup ViewDatatypeGeometricObject
		*/
		class BALL_VIEW_EXPORT Vertex2
		{
			public:

			BALL_CREATE(Vertex2)

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					The vectors  are set to (0.0, 0.0, 0.0). The vertex
					addresses are set to the addresses of the own vectors.
			*/
			Vertex2();

			/** Copy constructor.
			*/
			Vertex2(const Vertex2& vertex);

			//@}
			/** @name Destructors 
		  */
			//@{

			/** Destructor.
			*/
			virtual ~Vertex2();

			/** Explicit default initialization.
					Set the vectors  to the vector (0.0, 0.0, 0.0).
					The vertex addresses  are set to the addresses of 
					the own vectors .
			*/
			virtual void clear();

			//@}
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					The vectors and the vertex addresses  are initialized 
					to the vectors and vertex addresses of the vertex2 <b> vertex</b>.\par
			*/
			void set(const Vertex2& vertex);

			/** Assignment operator.
					Calls set.
					The vectors and the vertex addresses  are initialized
					to the vectors and the vertex addresses of the vertex2 <b> vertex</b>.\par
			*/
			const Vertex2& operator = (const Vertex2& vertex);

			/** Swapping of vertices.
			*/
			void swap(Vertex2& vertex);

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the first vector .
					\param       v the new first vector 
					\see         getVertex1
			*/
			void setVertex1(const Vector3& v);

			/** Change the first vector .
					\see         getVertex1
			*/
			void setVertex1(const float x, const float y, const float z);

			/** Mutable inspection of the first vector
					\return      Vector3& mutable reference to the first vector
					\see         setVertex1
			*/
			Vector3& getVertex1();

			/** Non-mutable inspection of the first vector 
			*/
			const Vector3& getVertex1() const;

			/** Inspection of the first vector .
					Access the first vector of this vertex by using Vector3.
					\param       v the vector receiving the first vector
					\see         setVertex1
			*/
			void getVertex1(Vector3& v) const;

			/** Access the components of the first vector by using float.
					\see         setVertex1
			*/
			void getVertex1(float& x, float& y, float& z) const;

			/** Change the vector address of the first vector.
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
					\param       v the new first vector address
					\see         getVertex1Address
			*/
			void setVertex1Address(const Vector3& v);

			/** Change the first vector address to the default address.
					So the value of the first vector will no longer be ignored 
					and all access methods will return it again.
					This method unhooks the object from any other objects prio connected
					with setVertex1Address.
					\see         Vertex::setVertex1Address
			*/
			void setDefaultVertex1Address();

			/** Mutable inspection of the first vertex address.
					\return      Vector3* pointer to the first vector that contains the value
					\see         setVertex1Address
			*/
			Vector3* getVertex1Address() const;

			/** Change the second vector .
					\param       v the new second vector 
					\see         getVertex2
			*/
			void setVertex2(const Vector3& v);

			/** Change the second vector .
					\see         getVertex2
			*/
			void setVertex2(const float x, const float y, const float z);

			/** Mutable inspection of the second vector.
					\return      Vector3& mutable reference to the second vector
					\see         setVertex2
			*/
			Vector3& getVertex2();

			/** Non-mutable inspection of the second vector .
					For further information see getVertex2.
			*/
			const Vector3& getVertex2() const;

			/** Inspection of the second vector .
					Access the second vector of this vertex by using Vector3.
					\param       v the vector receiving the second vector 
					\see         setVertex2
					\see         Vector3
			*/
			void getVertex2(Vector3& v) const;

			/** Inspection of the components of the second vector .
					\see         setVertex2
			*/
			void getVertex2(float& x, float& y, float& z) const;

			/** Change the vector address of the second vector .
					If a vector address is given the value of the first vector of this
					vertex2	is ignored. Instead if the method getVertex2 (or any other access
					method concerning the second vector) are called the value of the vector
					given by the vertex address is returned.
					The vector to which the second vertex address points must exist and be
					valid as long as this vertex2 exists.
					An object that uses this method can hook itself onto another object
					(speaking in terms of position). If the	object changes its second
					position so the object derived from this vertex2	changes
					its second position.
					\param       v the new second vector address 
					\see         getVertex2Address
			*/
			void setVertex2Address(const Vector3& v);

			/** Change the second vector address  to the default address.
					This method resets the second vertex address to the second vector of
					this vertex2. So the value of the second vector of this
					vertex2 will no longer be ignored and all access methods will return 
					it again.
					This method unhooks the object from any other objects prio connected
					with setVertex2Address.
					\see         Vertex::setVertex2Address
			*/
			void setDefaultVertex2Address();

			/** Mutable inspection of the second vertex address .
					\return      Vector3* pointer to the second vector that contains the value 
					\see         setVertex2Address
			*/
			Vector3* getVertex2Address() const;

			/** Change the first and second vector .
					\param       vertex1 the new first vector 
					\param       vertex2 the new second vector 
					\see         getVertices
			*/
			void setVertices(const Vector3& vertex1, const Vector3& vertex2);

			/** Change the first and second vector .
					\see         getVertices
			*/
			void setVertices
				(const float vertex1_x,
				 const float vertex1_y,
				 const float vertex1_z,
				 const float vertex2_x,
				 const float vertex2_y,
				 const float vertex2_z);

			/** Change the vector addresses of the first and second vector.
					See setVertex1Address or setVertex2Address for further information
					concerning vector addresses.
					\param       vertex1 the new first vector address 
					\param       vertex2 the new second vector address 
					\see         setVertex1Address
					\see         setVertex2Address
			*/
			void setVertexAddresses(const Vector3& vertex1, const Vector3& vertex2);

			/** Inspection of the first and second vector .
					\param       vertex1 the vector receiving the first vector 
					\param       vertex2 the vector receiving the second vector 
					\see         setVertices
			*/
			void getVertices(Vector3& vertex1, Vector3& vertex2);

			/** Access the components of the first and second vector of this
					vertex2 by using float.
					\see         setVertices
			*/
			void getVertices
				(float& vertex1_x, float& vertex1_y, float& vertex1_z,
				 float& vertex2_x, float& vertex2_y, float& vertex2_z);

			/** Change the first and second vector address 
					to the default addresses.
					This method resets the first and second vertex address to the first 
					and second vector . So the value of the first 
					and second vector  will no longer be ignored and
					all access methods will return them again.
					This method unhooks the object from any other objects prio connected
					with setVertexAddresses.
					\see         Vertex::setVertex2Addresses
			*/
			void setDefaultVertexAddresses();

			//@}
			/**	@name	Predicates
			*/
			//@{

			/** first vertex address test.
					Tests if the first vertex address  points to the 
					first vector .
					\return			bool <tt> true</tt> if the first vertex address  points to the first vector .
					 						<tt> false</tt> otherwise
					\see        setVertex1Address					
					\see        getVertex1Address					
					\see        setDefaultVertex1Address					
			*/
			bool isDefaultVertex1Address() const;

			/** second vertex address test.
					Test if the second vertex address  points to the 
					second vector .
					\return			bool <tt> true</tt> if the second vertex address  points to the second vector .
					 						<tt> false</tt> otherwise
					\see        setVertex2Address					
					\see        getVertex2Address					
					\see        setDefaultVertex2Address					
			*/
			bool isDefaultVertex2Address() const;

			/** both vertex addresses test.
					\return			bool <tt> true</tt> if both vertex addresses  each points to its own vector .
					 						<tt> false</tt> otherwise
					\see        setVertex1Address					
					\see        getVertex1Address					
					\see        setVertex2Address					
					\see        getVertex2Address					
					\see        setDefaultVertex1Address					
					\see        setDefaultVertex2Address					
		  */
			bool isDefaultVertexAddresses() const;

			//@}
			/**	@name	debuggers and diagnostics
			*/	
			//@{

			/** Internal state and consistency self-validation.
					Calls Vector3::isValid.
			*/
			virtual bool isValid() const;

			/** Internal value dump.
					Dump the current state  to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the state 
					\param   depth the dumping depth
					\see     Vector3::operator <<
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

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
#			include <BALL/VIEW/DATATYPE/vertex2.iC>
#		endif
	
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_DATATYPE_VERTEX2_H

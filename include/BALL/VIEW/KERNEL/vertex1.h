// $Id: vertex1.h,v 1.5 2001/05/13 13:47:49 hekl Exp $

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
				{\bf Framework:} BALL/VIEW/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/vertex1.h}\\ \\
				The class Vertex is used as a base class for all geometric objects that
				have a single vertex. It provides the derived class with methods for accessing
				that vertex. Further there is the possibility to give an address to a 
				vector \Ref{Vector3} as vertex. So if the value of that given vector
				changes the value of {\em *this} vertex changes as well. To avoid 
				segmentation faults this vector address must be valid as long as 
				{\em *this} vertex exists.
				@memo    Vertex class (BALL VIEW kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.5 $
				@date    $Date: 2001/05/13 13:47:49 $
		*/
		class Vertex
		{
			public:

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new vertex.
					The vector of {\em *this} vertex is set to (0.0, 0.0, 0.0). The vertex
					address is set to the address of the own vector of {\em *this} vertex.
					@return      Vertex new constructed vertex
					@see         Vector3
			*/
			Vertex()
				throw();

			/** Copy constructor.
					Construct new vertex by copying the vertex {\em vertex}. The vector of
					{\em vertex} is copied to the vector of {\em *this} vertex.
					The vertex address of {\em *this} vertex is set to the value of 
					the vertex address of {\em vertex} vertex.
					@param       vertex the vertex to be copied
					@return      Vertex new constructed vertex copied from {\em vertex}
			*/
			Vertex(const Vertex& vertex)
				throw();

			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} vertex.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~Vertex()
				throw();

			/** Explicit default initialization.
					Set the vector of {\em *this} vertex to the vector (0.0, 0.0, 0.0).
					The vertex address of {\em *this} vertex is set to the address of 
					the own vector of {\em *this} vertex.
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();
			//@}
			
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment.
					Assign the vertex {\em v} to {\em *this} vertex.
					The vector and the vertex address of {\em *this} vertex is initialized 
					to the vector and vertex address of the vertex {\em v}.\\
					@param       v the vertex to be copied
					@see         setVertex
					@see         setVertexAddress
					@see         Vector3
			*/
			void set(const Vertex& v)
				throw();

			/** Assignment operator.
					Assign the vertex {\em v} to {\em *this} vertex.
					Calls \Ref{set}.
					The vector and the vertex address of {\em *this} vertex is initialized
					to the vector and the vertex address of the vertex {\em v}.\\
					@param       v the vertex to be copied
					@return      Vertex& constant reference of {\em *this} vertex
					@see         set
			*/
			const Vertex& operator = (const Vertex& v)
				throw();

			/** Copying.
					Copy {\em *this} vertex to the vertex {\em v}.
					Calls \Ref{set}.
					The vector and the vertex address of {\em *this} vertex is initialized
					to the vector and the vertex address of the vertex {\em v}.\\
					@param       v the vertex to be assigned to
					@see         set
			*/
			void get(Vertex& v) const
				throw();

			/** Swapping of vertices.
					Swap the vector and vertex address of {\em *this} vertex with 
					the vector and vertex address of the vertex	{\em v}.
					@param       v the vertex being swapped with {\em *this} vertex 
					@see         Vertex
			*/
			void swap(Vertex& v)
				throw();
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the vector of {\em *this} vertex.
					Change the vector of {\em *this} vertex to the vector
					represented by the parameter {\em v}.
					@param       v the new vector of {\em *this} vertex
					@see         getVertex
					@see         Vector3
			*/
			void setVertex(const Vector3& v)
				throw();

			/** Change the vector of {\em *this} vertex.
					Change the vector of {\em *this} vertex to the vector represented by the
					parameters {\em x}, {\em y} and {\em z}.
					@param       x the x component of the new vector of {\em *this} vertex
					@param       y the y component of the new vector of {\em *this} vertex
					@param       z the z component of the new vector of {\em *this} vertex
					@see         getVertex
					@see         Vector3
					@see         Real
			*/
			void setVertex(const Real x, const Real y, const Real z)
				throw();

			/** Mutable inspection of the vector of {\em *this} vertex.
					Access the mutual reference of the vector of {\em *this} vertex.
					@return      Vector3& mutable reference to the vector of {\em *this} vertex
					@see         setVertex
					@see         Vector3
			*/
			Vector3& getVertex()
				throw();

			/** Non-mutable inspection of the vector of {\em *this} vertex.
					Access the constant reference of the vector of {\em *this} vertex.
					@return      Vector3& constant reference to the vector of {\em *this} vertex
					@see         setVertex
					@see         Vector3
			*/
			const Vector3& getVertex() const
				throw();

			/** Inspection of the vector of {\em *this} vertex.
					Access the vector of {\em *this} vertex by using \Ref{Vector3}.
					@param       v the vector receiving the vector of {\em *this} vertex
					@see         setVertex
					@see         Vector3
			*/
			void getVertex(Vector3& v) const
				throw();

			/** Inspection of the components of the vector of {\em *this} vertex.
					Access the components of the vector of {\em *this} vertex by using \Ref{Real}.
					@param       x the x component of the vector of {\em *this} vertex
					@param       y the y component of the vector of {\em *this} vertex
					@param       z the z component of the vector of {\em *this} vertex
					@see         setVertex
					@see         Real
			*/
			void getVertex(Real& x, Real& y, Real& z) const
				throw();

			/** Change the vector address of {\em *this} vertex.
					Change the vector address of {\em *this} vertex to the vector address
					represented by the parameter {\em v}.
					If a vector address is given the value of the vector of {\em *this} vertex
					is ignored. Instead if the method getVertex (or any other access method)
					is called the value of the vector given by the vertex address is returned.
					The vector to which the vertex address points must exist and be valid as
					long as {\em *this} vertex exists. An object that uses this method can
					hook itself onto another object (speaking in terms of position). If the
					object changes its position so the object derived from {\em *this} vertex
					changes its position.
					@param       v the new vector address of {\em *this} vertex
					@see         getVertexAddress
					@see         Vector3
			*/
			void setVertexAddress(const Vector3& v)
				throw();
		
			/** Change the vector address of {\em *this} vertex to the default address.
					This method resets the vertex address to the vector address of
					{\em *this} vertex. So the value of the vector of {\em *this} vertex 
					will no longer be ignored and all access methods will return it again.
					This method unhooks the object from any other objects prio connected
					with setVertexAddress.
					@see         setVertexAddress
			*/
			void setDefaultVertexAddress()
				throw();

			/** Mutable inspection of the vertex address of {\em *this} vertex.
					Access the pointer of the vector that contains the value of
					{\em *this}	vertex.
					@return      Vector3* pointer to the vector that contains the value of {\em *this} vertex
					@see         setVertexAddress
					@see         Vector3
			*/
			Vector3* getVertexAddress() const
				throw();

			//@}
			
			/**	@name	Predicates
			*/
			//@{

			/** Vertex address test.
					Tests if the vertex address of {\em *this} vertex points to the vector
					of {\em *this} vertex.
					@return			bool {\tt true} if the vertex address of {\em *this} vertex points to the vector of {\em *this} vertex.
					 						{\tt false} otherwise
					@see        setVertexAddress					
					@see        getVertexAddress					
					@see        setDefaultVertexAddress					
			*/
			bool isDefaultVertexAddress() const
				throw();
			//@}

			/**	@name	debuggers and diagnostics
			*/	
			//@{

			/** Internal state and consistency self-validation.
					Initiate self-validation of the internal state and data structure consistencies
					of {\em *this} vertex.
					If the internal state of {\em *this} vertex is correct (self-validated) and 
					consistent {\tt true} is returned, {\tt false} otherwise. 
					Calls {Vector3::isValid}.
					@return			bool -
											{\tt true} if the internal state of {\em *this} vertex is correct (self-validated) and consistent,
					 						{\tt false} otherwise
					@see        Vector3::isValid
			*/
			virtual bool isValid() const
				throw();

			/** Internal value dump.
					Dump the current state of {\em *this} vertex to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the state of {\em *this} vertex
					@param   depth the dumping depth
					@see     Vector3::operator <<
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();
			//@}

			/**	@name	Storers
			*/
			//@{

			/** Persistent stream output and state restorage.
  			  Read persistent vertex data from the input stream {\em s} and 
				  restore the state of {\em *this}.\\
				  {\bf Note:} Not yet implemented.
				  @param       s input stream from where to restore the internal state of {\em *this} vertex
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			  Write persistent vertex data to the output stream {\em s} and 
				  store the state of {\em *this}. \\
				  {\bf Note:} Not yet implemented.
				  @param       s output stream to where to store the internal state of {\em *this} vertex
			*/
			virtual void write(std::ostream& s) const
				throw();
			//@}

			private:
		
			/* vertex */
			Vector3 vertex1_;

			/* vertex pointer */
			Vector3* vertex1_ptr_;
		};


#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/vertex1.iC>
#		endif
		
	} // namespace VIEW
	
} // namespace BALL

#endif // BALL_VIEW_KERNEL_VERTEX1_H

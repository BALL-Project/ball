// $Id: vertex1.h,v 1.3 1999/12/28 18:37:47 oliver Exp $

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

		/**	
		*/
		class Vertex
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			Vertex();

			Vertex(const Vertex& vertex, bool deep = true);

			virtual ~Vertex();

			virtual void clear();

			virtual void destroy();
			//@}
			
			/**	@name	 Assignment
			*/
			//@{
			void set
				(const Vertex& v, bool deep = true);

			Vertex& operator = (const Vertex& v);

			void get(Vertex& v, bool deep = true) const;

			void swap(Vertex& v);
			//@}

			/**	@name	Accessors
			*/
			//@{

			void setVertex(const Vector3& v);

			void setVertex(const Real x, const Real y, const Real z);

			Vector3& getVertex();

			const Vector3& getVertex() const;

			void getVertex(Vector3& v) const;

			void getVertex(Real& x, Real& y, Real& z) const;

			void setVertexAddress(const Vector3& v);
		
			void setDefaultVertexAddress();

			Vector3* getVertexAddress() const;

			void getVertexAddress(Vector3*& vector_ptr) const;

			//@}
			
			/**	@name	Predicates
			*/
			//@{

			bool isDefaultVertexAddress() const;
			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;
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

// $Id: vertex2.h,v 1.2 1999/12/19 17:19:30 oliver Exp $

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

		/**
		*/
		class Vertex2
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{
			Vertex2();

			Vertex2(const Vertex2& vertex, bool deep = true);

			virtual ~Vertex2();

			virtual void clear();

			virtual void destroy();
			//@}
		
			/**	@name	Assignment
			*/
			//@{

			void set(const Vertex2& vertex, bool deep = true);

			Vertex2& operator = (const Vertex2& vertex);

			void get(Vertex2& vertex, bool deep = true) const;

			void swap(Vertex2& vertex);
			//@}

			/**	@name	Accessors
			*/
			//@{

			void setVertex1(const Vector3& v);

			void setVertex1(const Real x, const Real y, const Real z);

			Vector3& getVertex1();

			const Vector3& getVertex1() const;

			void getVertex1(Vector3& v) const;

			void getVertex1(Real& x, Real& y, Real& z) const;

			void setVertex1Address(const Vector3& v);

			void setDefaultVertex1Address();

			Vector3* getVertex1Address() const;

			void getVertex1Address(Vector3*& v);

			void setVertex2(const Vector3& v);

			void setVertex2(const Real x, const Real y, const Real z);

			Vector3& getVertex2();

			const Vector3& getVertex2() const;

			void getVertex2(Vector3& v) const;

			void getVertex2(Real& x, Real& y, Real& z) const;

			void setVertex2Address(const Vector3& v);

			void setDefaultVertex2Address();

			Vector3* getVertex2Address() const;

			void getVertex2Address(Vector3*& v);


			void setVertices(const Vector3& vertex1, const Vector3& vertex2);

			void setVertices
				(const Real vertex1_x,
				 const Real vertex1_y,
				 const Real vertex1_z,
				 const Real vertex2_x,
				 const Real vertex2_y,
				 const Real vertex2_z);

			void setVertexAddresses
				(const Vector3& vertex1,
				 const Vector3& vertex2);

			void getVertices(Vector3& vertex1, Vector3& vertex2);

			void getVertices
				(Real& vertex1_x, Real& vertex1_y, Real& vertex1_z,
				 Real& vertex2_x, Real& vertex2_y, Real& vertex2_z);

			void getVertexAddresses(Vector3*& vertex1, Vector3*& vertex2);

			void setDefaultVertexAddresses();
			//@}

			/**	@name	Predicates
			*/
			//@{

			bool isDefaultVertex1Address() const;

			bool isDefaultVertex2Address() const;

			bool isDefaultVertexAddresses() const;
			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{

			virtual bool isValid() const;

			virtual void dump
				(ostream& s = cout, unsigned long depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{

			virtual void read(istream& s);

			virtual void write(ostream& s) const;
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

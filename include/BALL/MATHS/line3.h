// $Id: line3.h,v 1.7 2000/02/16 17:04:57 oliver Exp $

#ifndef BALL_MATHS_LINE3_H
#define BALL_MATHS_LINE3_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifdef BALL_INCLUDE_IEEEFP
#	include <ieeefp.h>
#endif

#include <math.h>
#include <iostream>

#ifndef BALL_MATHS_POINT3_H
#	include <BALL/MATHS/point3.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL 
{

	/**	Generic Line in Three-Dimensional Space.
      {\bf Definition:} \URL{BALL/MATHS/.h}
      \\
	*/
	template <class T>
	class TLine3
	{
		public:

		BALL_CREATE(TLine3<T>)

		/**	@name	Enums
		*/
		//@{

		///
		enum Form
		{
			FORM__PARAMETER  = 0,
			FORM__TWO_POINTS = 1
		};
		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		///
		TLine3()
			:	p(),
				d()
		{
		}

		///
		TLine3(const TLine3& line,
					 bool /* deep */ = true)
			:	p(line.p),
				d(line.d)
		{
		}

		// form: PARAMETER (default) or TWO_POINTS

		/**
		*/
		TLine3(const TVector3<T>& point, const TVector3<T>& vector, 
					 Form form = FORM__PARAMETER)
			:	p(point),
				d((form == FORM__PARAMETER) 
					? vector 
					: vector - point)
		{
		}

		///
		virtual ~TLine3()
		{
		}
		//@}

		/**	@name	Assignment	
		*/
		//@{

		///
		void swap(TLine3& line)
		{
			TVector3<T> temp_point(p);
			p = line.p;
			line.p = temp_point;

			TVector3<T> temp_vector(d);
			d = line.d;
			line.d = temp_vector;
		}

		///
		void set(const TLine3& line, bool /* deep */ = true)
		{
			p = line.p;
			d = line.d;
		}

		///
		void set(const TVector3<T>& point, const TVector3<T>& vector, Form form = FORM__PARAMETER)
		{
			p = point;
			if (form == FORM__PARAMETER) 
			{
				d = vector;
			} else {
				d = vector - point;
			}
		}

		///
		TLine3& operator = (const TLine3& line)
		{
			p = line.p;
			d = line.d;

			return *this;
		}

		///
		void get(TLine3& line, bool /* deep */ = true) const
		{
			line.p = p;
			line.d = d;
		}

		///
		void get(TVector3<T>& point,TVector3<T>& vector,
						 Form form = FORM__PARAMETER) const
		{
			point = p;
			if (form == FORM__PARAMETER) 
			{
				d = vector;
			} else {
				d = vector - point;
			}
		}

		//@}

		/**	@name	Accessors
		*/
		//@{

		///
		void normalize()
		{
			d.normalize();
		}
		//@}

		/**	@name	Predicates
		*/
		//@{

		///
		bool operator ==(const TLine3& line) const
		{
			return (bool)(p == line.p && d == line.d);
		}

		///
		bool operator != (const TLine3& line) const
		{
			return (bool)(p != line.p || d != line.d);
		}

		///
		bool has(const TVector3<T>& point) const
		{
			if (Maths::isNotZero(d.x))
			{
				T c = (point.x - p.x) / d.x;

				return (bool)(Maths::isEqual(p.y + c * d.y, point.y)
											&& Maths::isEqual(p.z + c * d.z, point.z));
			} else if (Maths::isNotZero(d.y))
			{
				T c = (point.y - p.y) / d.y;

				return (bool)(Maths::isEqual(p.x, point.x)   // invariant: d.x == 0
											&& Maths::isEqual(p.z + c * d.z, point.z));
			}
			else if (Maths::isNotZero(d.z))
			{
				return (bool)(Maths::isEqual(p.x, point.x)   // invariant: d.x == 0
											&& Maths::isEqual(p.y, point.y)); // invariant: d.y == 0
			} else {
				return false;
			}
		}
		//@}

		/**	@name	Debugging and Diagnostics
		*/
		//@{

		///
		bool isValid() const
		{
			return true;
		}

		///
		void dump(std::ostream& s = std::cout, Size depth = 0) const
		{
			BALL_DUMP_STREAM_PREFIX(s);

			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "  position: " << p << endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "  direction: " << d << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}
		//@}


		/**	@name	Storers
		*/
		//@{

		///
		friend std::istream& operator >> (std::istream &s, TLine3& line)
		{
			return (s >> line.p >> line.d);
		}

		///
		friend std::ostream& operator << (std::ostream& s, const TLine3& line)
		{
			return (s << line.p << line.d);
		}
		//@}


		/**	@name	Attributes
		*/
		//@{

		///
		TVector3<T> p;

		///
		TVector3<T> d;
		//@}
	};

	///
	typedef TLine3<float> Line3;

} // namespace BALL

#endif // BALL_MATHS_LINE3_H

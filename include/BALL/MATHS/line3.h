// $Id: line3.h,v 1.14 2000/03/03 02:20:36 amoll Exp $

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

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL 
{

	template <typename T>
	class TLine3;
	
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TLine3<T>& line);

	template <typename T>
	std::istream& operator >> (std::istream& s, TLine3<T>& line);
	


	/**	Generic Line in Three-Dimensional Space.
      {\bf Definition:} \URL{BALL/MATHS/line3.h}
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

		/** form of parameter to describe the line:
				{\tt 0} one Point and one Vector
				{\tt 1} two Points
		*/
		enum Form
		{
			FORM__PARAMETER  = 0,
			FORM__TWO_POINTS = 1
		};
		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TLine3 object..
		*/
		TLine3()
			:	p(),
				d()
		{
		}

		/**	Copy constructor.
				Create a new TVector4 object from another.
				@param TLine3 the TLine3 object to be copied
				@param bool ignored - just for interface consistency
		*/	
		TLine3(const TLine3& line,
					 bool /* deep */ = true)
			:	p(line.p),
				d(line.d)
		{
		}

		// form: PARAMETER (default) or TWO_POINTS

		/**	Detailled constructor.
				Create a new TLine3 object from a point and a vector
				of from two points, dependent from Form.
				@param	point assigned to {\tt p}
				@param	vector assigned to {\tt d}
				@param	form assigns form of parameter
		*/
		TLine3(const TVector3<T>& point, const TVector3<T>& vector, 
					 Form form = FORM__PARAMETER)
			:	p(point),
				d((form == FORM__PARAMETER) 
					? vector 
					: vector - point)
		{
		}

		/**	Destructor.	
				Destructs the TLine3 object. As there are no dynamic
				data structures, nothing happens.
		*/
		virtual ~TLine3()
		{
		}
		//@}

		/**	@name	Assignment	
		*/
		//@{

		/**	Swap the contents of two lines.
				@param	line the TLine3 to swap contents with
		*/
		void swap(TLine3& line)
		{
			TVector3<T> temp_point(p);
			p = line.p;
			line.p = temp_point;

			TVector3<T> temp_vector(d);
			d = line.d;
			line.d = temp_vector;
		}

		/**	Assign from another TLine3.
				@param line	the TLine3 object to assign from
				@param deep ignored
		*/
		void set(const TLine3& line, bool /* deep */ = true)
		{
			p = line.p;
			d = line.d;
		}

		/**	Assign from one point and a vector
				of from two points, dependent from form.
				@param	point assigned to {\tt p}
				@param	vector assigned to {\tt d}
				@param	form assigns form of parameter
		*/
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

		/**	Assignment operator.
				Assign the components from another line.
				@param line the vector to assign from
		**/
		TLine3& operator = (const TLine3& line)
		{
			p = line.p;
			d = line.d;

			return *this;
		}

		/**	Assign to another TLine3.
				Assigns the vector components to another vector.
				@param line	the line to be asigned to
				@param deep ignored
		*/
		void get(TLine3& line, bool /* deep */ = true) const
		{
			line.p = p;
			line.d = d;
		}

		/**	Assign to two {\tt TVector3}.
				art of componenents depends of form:
				{\tt 0} two points
				{\tt 1} one point and the vector component
				@param	point the first point
				@param	vector the second point or the vector component
				@param	rh the h component
		*/
		void get(TVector3<T>& point,TVector3<T>& vector,
						 Form form = FORM__PARAMETER) const
		{
			point = p;
			if (form == FORM__PARAMETER) 
			{
				vector = d;
			} else {
				vector - point = d;
			}
		}

		//@}

		/**	@name	Accessors
		*/
		//@{

		/**	Normalize the vector- component.
				The vector is scaled with its length:
				$\{x|y|z|} *= \sqrt{x^2 + y^2 + z^2}$.
				@exception DivisionByZero if the length of the vector is 0
		*/
		void normalize()
		{
			d.normalize();
		}
		//@}

		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return bool, {\bf true} if both components are equal, {\bf false} otherwise
		*/
		bool operator ==(const TLine3& line) const
		{
			return (bool)(p == line.p && d == line.d);
		}

		/**	Inequality operator.
				@return bool, {\bf true} if the two lines differ in at least on component, {\bf false} otherwise
		*/
		bool operator != (const TLine3& line) const
		{
			return (bool)(p != line.p || d != line.d);
		}

		/**	Test if a given point is a member of the line.
				@return bool, {\bf true} or {\bf false}
		*/
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

		/**	Test if instance is valid.
				always returns true
				@return bool {\bf true}
		*/
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
			s << "  position: " << p << std::endl;

			BALL_DUMP_DEPTH(s, depth);
			s << "  direction: " << d << std::endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}
		//@}


		/**	@name	Attributes
		*/
		//@{

		/**	Point- Component
		*/
		TVector3<T> p;

		/**	Vector- Component
		*/
		TVector3<T> d;
		//@}
	};

	/**	Default line of type {\bf float}
	*/
	typedef TLine3<float> Line3;

	/**	Input- Operator
			reads two {\bf TVector3} obejcts from an {\tt istream} and
			assigns them to {\tt d} and {\tt p}
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TLine3<T>& line)
	{
		char c;
		for (int i=0; i<5; i++)
		{
			s >> c;
		}
		s >> line.p >> line.d >> c;
		return s;
	}

	/**	Output-Operator
			writes the two public attrobutes {\tt d} and {\tt p} to an {\tt ostream}
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TLine3<T>& line)
	{
		return (s << "LINE(" << line.p << ' ' << line.d << ')');
	}
	//@}


} // namespace BALL

#endif // BALL_MATHS_LINE3_H

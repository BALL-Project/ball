// $Id: line3.h,v 1.28 2000/12/19 00:42:24 amoll Exp $

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
	std::ostream& operator << (std::ostream& s, const TLine3<T>& line)
		throw();

	template <typename T>
	std::istream& operator >> (std::istream& s, TLine3<T>& line)
		throw();
	


	/**	Generic Line in Three-Dimensional Space.
      {\bf Definition:} \URL{BALL/MATHS/line3.h}
      \\
	*/
	template <typename T>
	class TLine3
	{
		public:

		BALL_CREATE(TLine3<T>)

		/**	@name	Enums
		*/
		//@{

		/** form of parameter to describe the line:
				{\tt 0} one Point and one Vector.
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
				This method creates a new TLine3 object.
		*/
		TLine3()
			throw()
			:	p(),
				d()
		{
		}

		/**	Copy constructor.
				Create a new TVector4 object from another.
				@param TLine3 the TLine3 object to be copied
				@param bool ignored - just for interface consistency
		*/	
		TLine3(const TLine3& line)
			throw()
			:	p(line.p),
				d(line.d)
		{
		}

		// form: PARAMETER (default) or TWO_POINTS

		/**	Detailed constructor.
				Depending on form, create a new TLine3 object from a point and a vector
				or from two points.
				{\tt 0} one Point and one Vector\\
				{\tt 1} two Points
				@param	point assigned to {\tt p}
				@param	vector assigned to {\tt d}
				@param	form assigns form of parameter
		*/
		TLine3(const TVector3<T>& point, const TVector3<T>& vector, Form form = FORM__PARAMETER)
			throw()
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
			throw()
		{
		}
		//@}

		/**	@name	Assignment	
		*/
		//@{

		/**	Swap the contents of two instances of lines.
				@param	line the TLine3 to swap contents with
		*/
		void swap(TLine3& line)
			throw()
		{
			TVector3<T> temp_point(p);
			p = line.p;
			line.p = temp_point;

			TVector3<T> temp_vector(d);
			d = line.d;
			line.d = temp_vector;
		}

		/**	Assign from another instance of TLine3.
				@param line	the TLine3 object to assign from
		*/
		void set(const TLine3& line)
			throw()
		{
			p = line.p;
			d = line.d;
		}

		/**	Assign from one point and a vector
				or from two points, depending on form.
				@param	point assigned to {\tt p}
				@param	vector assigned to {\tt d}
				@param	form assigns form of parameter
		*/
		void set(const TVector3<T>& point, const TVector3<T>& vector, Form form = FORM__PARAMETER)
			throw()
		{
			p = point;
			if (form == FORM__PARAMETER) 
			{
				d = vector;
			}
			else 
			{
				d = vector - point;
			}
		}

		/**	Assignment operator.
				Assign the components from another instance of line.
				@param line the vector to assign from
		**/
		const TLine3& operator = (const TLine3& line)
			throw()
		{
			p = line.p;
			d = line.d;

			return *this;
		}

		/**	Assign to another instance of TLine3.
				Assigns the vector components to another vector.
				@param line	the line to be assigned to
		*/
		void get(TLine3& line)
			throw()
		{
			line.p = p;
			line.d = d;
		}

		/**	Assign to two instances of {\tt TVector3}.
				Type of components depends on form.
				If form is {\tt 0} two points, else 
				{\tt 1} one point and the vector component.
				@param	point the first point
				@param	vector the second point or the vector component
				@param	rh the h component
		*/
		void get(TVector3<T>& point,TVector3<T>& vector, Form form = FORM__PARAMETER) const
			throw()
		{
			point = p;
			if (form == FORM__PARAMETER) 
			{
				vector = d;
			}
			else 
			{
				vector - point = d;
			}
		}

		//@}

		/**	@name	Accessors
		*/
		//@{

		/**	Normalize the vector component.
				The vector is scaled with its length:
				$\{x|y|z|\} *= \sqrt{x^2 + y^2 + z^2}$.
				@exception DivisionByZero if the length of the vector is 0
		*/
		void normalize()
			throw()
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
			throw()
		{
			return (p == line.p && d == line.d);
		}

		/**	Inequality operator.
				@return bool, {\bf true} if the two lines differ in at least on component, {\bf false} otherwise
		*/
		bool operator != (const TLine3& line) const
			throw()
		{
			return (p != line.p || d != line.d);
		}

		/**	Test whether a given point is a member of the line.
				@return bool, {\bf true} or {\bf false}
		*/
		bool has(const TVector3<T>& point) const
			throw()
		{
			if (Maths::isNotZero(d.x))
			{
				T c = (point.x - p.x) / d.x;

				return (Maths::isEqual(p.y + c * d.y, point.y) && Maths::isEqual(p.z + c * d.z, point.z));
			}
			else 
			{
				if (Maths::isNotZero(d.y))
				{
					T c = (point.y - p.y) / d.y;

					return (Maths::isEqual(p.x, point.x)   // invariant: d.x == 0
												&& Maths::isEqual(p.z + c * d.z, point.z));
				}
				else 
				{
					if (Maths::isNotZero(d.z))
					{
						return (Maths::isEqual(p.x, point.x)   // invariant: d.x == 0
													&& Maths::isEqual(p.y, point.y)); // invariant: d.y == 0
					}
					else 
					{
						return false;
					}
				}
			}
		}
		//@}

		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/**	Test whether instance is valid.
				Always returns true.
				@return bool {\bf true}
		*/
		bool isValid() const
			throw()
		{
			return true;
		}

		/** Internal state dump.
				Dump the current internal state of {\em *this} to 
				the output ostream {\em s} with dumping depth {\em depth}.
				@param   s - output stream where to output the internal state of {\em *this}
				@param   depth - the dumping depth
		*/
		void dump(std::ostream& s = std::cout, Size depth = 0) const
			throw()
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

		/**	Point Component.
		*/
		TVector3<T> p;

		/**	Vector Component.
		*/
		TVector3<T> d;
		//@}
	};

	/**	Default line of type {\bf float}.
	*/
	typedef TLine3<float> Line3;

	/**	Stream operators of Line3
	*/
	//@{
	/**	Input- Operator.
			reads two objcts of type {\bf TVector3} from an {\tt istream} and
			assigns them to {\tt d} and {\tt p}.
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TLine3<T>& line)
		throw()
	{
		char c;
		s >> c >> line.p >> line.d >> c;
		return s;
	}

	/**	Output-Operator.
			Writes the two public attributes {\tt d} and {\tt p} to an output stream.
			The values of the two vectors are enclosed in brackets.\\
			{\bf Example:}\\
			{\tt ((0 0 0) (1 2 1))}
			@see TVector3::operator<<
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TLine3<T>& line)
		throw()
	{
		s << '(' << line.p << ' ' << line.d << ')';
		return s;
	}
	//@}


} // namespace BALL

#endif // BALL_MATHS_LINE3_H

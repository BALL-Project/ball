// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: line3.h,v 1.48 2004/07/05 20:57:28 oliver Exp $
//

#ifndef BALL_MATHS_LINE3_H
#define BALL_MATHS_LINE3_H

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL 
{
	/** \defgroup Line Generic Line in Three-Dimensional Space.

		\ingroup GeometricObjects
	*/
	
	//@{

	template <typename T>
	class TLine3;
	
	/** @name Storers
		Stream operators of Line3
	*/
	//@{
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TLine3<T>& line)
		;

	template <typename T>
	std::istream& operator >> (std::istream& s, TLine3<T>& line)
		;
	//@}
	
	/**	Generic Line in Three-Dimensional Space.
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
				<tt>0</tt> one Point and one Vector.
				<tt>1</tt> two Points
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

		/**	Default constructor
		*/
		TLine3()
			
			:	p(),
				d()
		{
		}

		/**	Copy constructor.
				@param TLine3 the TLine3 object to be copied
				@param bool ignored - just for interface consistency
		*/	
		TLine3(const TLine3& line)
			
			:	p(line.p),
				d(line.d)
		{
		}

		// form: PARAMETER (default) or TWO_POINTS

		/**	Detailed constructor.
				Depending on form, create a new TLine3 object from a point and a vector
				or from two points.
				@param	point assigned to <tt>p</tt>
				@param	vector assigned to <tt>d</tt>
				@param	form assigns form of parameter
								<tt>0</tt> one Point and one Vector \par
								<tt>1</tt> two Points
		*/
		TLine3(const TVector3<T>& point, const TVector3<T>& vector, Form form = FORM__PARAMETER)
			
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

		/**	Clear method.
				The values are set to 0.
		*/
		virtual void clear() 
			
		{
			p.clear();
			d.clear();
		}

		//@}

		/**	@name	Assignment	
		*/
		//@{

		/**	Swap the contents of two instances of lines.
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

		/**	Assign from another instance of TLine3.
				@param line	the TLine3 object to assign from
		*/
		void set(const TLine3& line)
			
		{
			p = line.p;
			d = line.d;
		}

		/**	Assign from one point and a vector
				or from two points, depending on form.
				@param	point assigned to <tt>p</tt>
				@param	vector assigned to <tt>d</tt>
				@param	form assigns form of parameter
		*/
		void set(const TVector3<T>& point, const TVector3<T>& vector, Form form = FORM__PARAMETER)
			
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
		TLine3& operator = (const TLine3& line)
			
		{
			p = line.p;
			d = line.d;

			return *this;
		}

		/**	Assign to another instance of TLine3.
				Assigns the vector components to another vector.
				@param line	the line to be assigned to
		*/
		void get(TLine3& line) const 
		{
			line.p = p;
			line.d = d;
		}

		/**	Assign to two instances of <tt>TVector3</tt>.
				Type of components depends on form.
				@param	point the first point
				@param	vector the second point or the vector component
				@param	form assigns form of parameter  \par
								<tt>0</tt> one Point and one Vector  \par
								<tt>1</tt> two Points
		*/
		void get(TVector3<T>& point,TVector3<T>& vector, Form form = FORM__PARAMETER) const
			
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
				\f$\{x|y|z|\} *= \sqrt{x^2 + y^2 + z^2}\f$.
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
				@return bool, <b>true</b> if both components are equal, <b>false</b> otherwise
		*/
		bool operator == (const TLine3& line) const
			
		{
			return (p == line.p && d == line.d);
		}

		/**	Inequality operator.
				@return bool, <b>true</b> if the two lines differ in at least one component, <b>false</b> otherwise
		*/
		bool operator != (const TLine3& line) const
			
		{
			return (p != line.p || d != line.d);
		}

		/**	Test whether a given point is a member of the line.
				@return bool, <b>true</b> or <b>false</b>
		*/
		bool has(const TVector3<T>& point) const
			
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
				@return bool <b>true</b>
		*/
		bool isValid() const
			
		{
			return true;
		}

		/** Internal state dump.
				Dump the current internal state of {\em *this} to 
				the output ostream <b>  s </b> with dumping depth <b>  depth </b>.
				@param   s - output stream where to output the internal state of {\em *this}
				@param   depth - the dumping depth
		*/
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

		/**	Point Component.
		*/
		TVector3<T> p;

		/**	Vector Component.
		*/
		TVector3<T> d;
		//@}
	};
	//@}

	/**	Default line of type <b>float</b>.
	 		\ingroup Line
	*/
	typedef TLine3<float> Line3;

	/**	Input operator.
			Reads two objcts of type <b>TVector3</b> from an <tt>istream</tt> and
			assigns them to <tt>d</tt> and <tt>p</tt>.
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TLine3<T>& line)
		
	{
		char c;
		s >> c >> line.p >> line.d >> c;
		return s;
	}

	/**	Output operator.
			Writes the two public attributes <tt>d</tt> and <tt>p</tt> to an output stream.
			The values of the two vectors are enclosed in brackets. \par
			<b>Example:</b> \par
			<tt>((0 0 0) (1 2 1))</tt>
			@see TVector3::operator<<
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TLine3<T>& line)
		
	{
		s << '(' << line.p << ' ' << line.d << ')';
		return s;
	}
} // namespace BALL

#endif // BALL_MATHS_LINE3_H

// $Id: angle.h,v 1.3 1999/10/30 12:53:26 oliver Exp $

#ifndef BALL_MATHS_ANGLE_H
#define BALL_MATHS_ANGLE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MATHS_COMMON_H
#	include <BALL/MATHS/common.h>
#endif

namespace BALL {

	/**	Generic Angle Class.
			Use this class to describe angles. The TAngle class permits the conversion
			from degree to radians and is a return type for all functions used to calculate
			Angles.\\
			{\bf Definition:} \URL{BALL/MATHS/angle.h}
			\\
	*/
	template <class T>
	class TAngle
	{
		public:

		BALL_CREATE(TAngle<T>)

		/**	@name	Enums
		*/
		//@{
		///
		enum Range
		{
			RANGE__UNLIMITED = 0, // no limitations
			RANGE__UNSIGNED  = 1, // 0° <= angle <= 360°, 0 <= angle <= (Constants::PI * 2)
			RANGE__SIGNED    = 2  // -180° <= angle <= 180°, -Constants::PI <= angle <= Constants::PI
		};
		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				Creates a new angle object. Its value is set to 0.
		*/
		TAngle()
			: value(0)
		{
		}

		/**	Copy constructor.
				Create a copy of an TAngle object. Copies are always
				shallow.
				@param	angle the object to be copied
				@param	deep ignored
		*/
		TAngle(const TAngle& angle, bool /* deep */ = true)
			:	value(angle.value)
		{
		}

		/**	Detailled constructor.
				Create a new angle object and set its value to 
				{\tt new\_value}. {\tt radian} determines whether {\tt new\_value}
				is in radians or in degrees.
				@param	new_value the value of the angle object
				@param	radian {\bf true} if {\tt new\_value} is in radians, {\tt false} otherwise 
		*/
		TAngle(const T& new_value, bool radian = true)
			:	value((radian == true)
				 ? (T)new_value 
				 : (T)BALL_ANGLE_DEGREE_TO_RADIAN((double)new_value))
		{
		}

		/**	Destructor.
		*/
		virtual ~TAngle()
		{
		}
		//@}


		/**	@name	Assignment
		*/
		//@{

		/**	Swap the contents of two angles.
		*/
		void swap(TAngle& angle)
		{
			T temp = value;
			value = angle.value;
			angle.value = temp;
		}

		/**	Assign an angle form another.
				@param	angle	the angle to assign from
				@param	deep ignored
		*/
		void set(const TAngle& angle, bool /* deep */ = true)
		{
			value = angle.value;
		}

		/**	Assign a new value to the angle.
				{\tt radian} determines whether {\tt new\_value}
				is in radians or in degrees.
				@param	new_value the value of the angle object
				@param	radian {\bf true} if {\tt new\_value} is in radians, {\tt false} otherwise 
		*/
		void set(const T& new_value, bool radian = true)
		{
			value = (radian == true)
				 ? new_value 
				 : BALL_ANGLE_DEGREE_TO_RADIAN(new_value);
		}

		/**	Assignment operator
		*/
		const TAngle& operator = (const TAngle& angle)
		{
			value = angle.value;

			return *this;
		}

		/**	Assignment operator for floats.
				Assign a float value to the angle.
				the assigned value has to be in radians!
				@param	new_value the new value
		*/
		const TAngle& operator = (const T& new_value)
		{
			value = new_value;

			return *this;
		}

		/**	
		*/
		void get(TAngle& angle, bool /* deep */ = true) const
		{
			angle.value = value;
		}

		/**	
		*/
		void get(T& val, bool radian = true) const
		{
			val = (radian == true)
						 ? value 
						 : BALL_ANGLE_RADIAN_TO_DEGREE(value);
		}

		/**	
		*/
		void setNull()
		{
			value = 0;
		}

		//@}

		/**	@name	Accessors
		*/
		//@{

		/**
		*/
		static TAngle getNull()
		{
			return TAngle(0);
		}

		/**
		*/
		operator T () const
		{
			return value;
		}

		/**
		*/
		T toRadian() const
		{
			return value;
		}

		/**
		*/
		static T toRadian(const T& degree)
		{
			return BALL_ANGLE_DEGREE_TO_RADIAN(degree);
		}

		/**
		*/
		T toDegree() const
		{
			return BALL_ANGLE_RADIAN_TO_DEGREE(value);
		}

		/**
		*/
		static T toDegree(const T& radian)
		{
			return BALL_ANGLE_RADIAN_TO_DEGREE(radian);
		}

		///
		static TAngle getTorsionTAngle
			(const T &ax, const T &ay, const T &az,
			 const T &bx, const T &by, const T &bz,
			 const T &cx, const T &cy, const T &cz, 
			 const T &dx, const T &dy, const T &dz)
		{
			T abx = ax - bx;
			T aby = ay - by;
			T abz = az - bz;

			T cbx = cx - bx;
			T cby = cy - by;
			T cbz = cz - bz;

			T cdx = cx - dx;
			T cdy = cy - dy;
			T cdz = cz - dz;

			// Calculate the normals to the two planes n1 and n2
			// this is given as the cross products:
			//		 AB x BC
			//		--------- = n1
			//		|AB x BC|
			//
			//		 BC x CD
			// 	  --------- = n2
			// 	  |BC x CD|

			// Normal to plane 1 
			T ndax = aby * cbz - abz * cby; 
			T nday = abz * cbx - abx * cbz;
			T ndaz = abx * cby - aby * cbx;

			// Normal to plane 2 
			T neax = cbz * cdy - cby * cdz; 
			T neay = cbx * cdz - cbz * cdx;
			T neaz = cby * cdx - cbx * cdy;

			// Calculate the length of the two normals 
			T bl = sqrt((double)ndax * ndax + nday * nday + ndaz * ndaz);
			T el = sqrt((double)neax * neax + neay * neay + neaz * neaz);
			T bel = ndax * neax + nday * neay + ndaz * neaz;
			
			bel /= (bl * el);
			if (Maths::isGreater(bel, 1)) {
				bel = 1;
			} else if (Maths::isLess(bel, -1)) {
				bel = -1;
			}

			T acosbel = acos(bel);

			if (Maths::isLess(cbx * (ndaz * neay - nday * neaz) 
								 + cby * (ndax * neaz - ndaz * neax) 
								 + cbz * (nday * neax - ndax * neay), 
								 0))
			{
				acosbel = -acosbel;
			}
			
			acosbel = (Maths::isGreater(acosbel, 0)) ? Constants::PI - acosbel : -(Constants::PI + acosbel);
			
			return TAngle(acosbel);
		}

		///
		void normalize(Range range)
		{
			if (range == RANGE__UNLIMITED)
			{
				return;
			}

			long mod_factor = (long)(value / (2 * Constants::PI));
			value -= mod_factor * (Constants::PI * 2);
	
			if (range == RANGE__SIGNED) { 
				// invariant: -180 to 180:

				if (Maths::isGreater(value, Constants::PI)) {
					value -= (Constants::PI * 2);
				}
			} else { // invariant: 0 to 360:
				if (Maths::isLess(value, 0)) {
					value += (Constants::PI * 2);
				}
			}
		}

		///
		void negate() 
		{
			value = -value;
		}

		///
		TAngle operator + () const 
		{
			return *this;
		}

		///
		TAngle operator - () const 
		{
			return TAngle(-value);
		}

		///
		TAngle& operator += (const TAngle& angle) 
		{
			value += angle.value;

			return *this;
		}

		///
		TAngle& operator += (const T& value) 
		{
			value += value;

			return *this;
		}

		///
		TAngle operator +	(const TAngle& angle) 
		{
			return TAngle(value + angle.value);
		}

		///
		TAngle operator + (const T& val) 
		{
			return TAngle(value + val);
		}

		///
		friend TAngle operator + (const T& val, const TAngle& angle) 
		{
			return TAngle(val + angle.value);
		}

		///
		TAngle& operator -= (const TAngle& angle) 
		{
			value -= angle.value;

			return *this;
		}

		///
		TAngle &operator -= (const T& val) 
		{
			value -= val;

			return *this;
		}

		///
		TAngle operator - (const TAngle& angle) 
		{
			return TAngle(value - angle.value);
		}

		///
		TAngle operator - (const T& val) 
		{
			return TAngle(value - val);
		}

		///
		friend TAngle operator - (const T &val, const TAngle& angle) 
		{
			return TAngle(val - angle.value);
		}

		///
		TAngle& operator *= (const TAngle& angle) 
		{
			value *= angle.value;

			return *this;
		}

		///
		TAngle& operator *=	(const T& val) 
		{
			value *= val;

			return *this;
		}

		///
		TAngle operator * (const T& value)
		{
			return TAngle(value * value);
		}

		///
		friend TAngle operator * (const T& val, const TAngle& angle) 
		{
			return TAngle(val * angle.value);
		}

		///
		TAngle& operator /= (const TAngle& angle)
		{
			value /= angle.value;

			return *this;
		}

		///
		TAngle& operator /=	(const T& val) 
		{
			value /= val;

			return *this;
		}

		///
		TAngle operator /	(const T& val) 
		{
			return TAngle(value / val);
		}
		//@}

		/**	@name	Predicates
		*/
		//@{

		///
		bool operator == (const TAngle& angle) const 
		{
			return Maths::isEqual(value, angle.value);
		}

		///
		bool operator == (const T& val) const
		{
			return Maths::isEqual(value, val);
		}

		///
		bool operator != (const TAngle& angle) const 
		{
			return Maths::isNotEqual(value, angle.value);
		}

		///
		bool operator !=	(const T& val) const 
		{
			return Maths::isNotEqual(value, val);
		}

		///
		bool operator <	(const TAngle& angle) const	
		{
			return Maths::isLess(value, angle.value);
		}

		///
		bool operator <	(const T& val) const 
		{
			return Maths::isLess(value, val);
		}

		///
		bool operator <= (const TAngle& angle) const
		{
			return Maths::isLessOrEqual(value, angle.value);
		}

		///
		bool operator >=	(const TAngle& angle) const 
		{
			return Maths::isGreaterOrEqual(value, angle.value);
		}

		///
		bool operator > (const TAngle& angle) const 
		{
			return Maths::isGreater(value, angle.value);
		}

		///
		bool isEquivalent(TAngle angle) const
		{
			TAngle this_angle(*this);

			this_angle.normalize(RANGE__UNSIGNED);
			angle.normalize(RANGE__UNSIGNED);

			return (bool)(this_angle == angle);
		}

		///
		bool isValid () const 
		{
			return true;
		}
		//@}


		/**	@name	Debugging and Diagnostics
		*/
		//@{

		///
		void dump(std::ostream& s = std::cout, unsigned long depth = 0) const 
		{
			BALL_DUMP_STREAM_PREFIX(s);

			BALL_DUMP_HEADER(s, this, this);

			BALL_DUMP_DEPTH(s, depth);
			s << "  value: " << value << endl;

			BALL_DUMP_STREAM_SUFFIX(s);
		}

		T value;
	};
	//@}

	/**	The Default Angle Type.
			If double precision is not needed, {\tt TAngle<Real>} should
			be used. It is predefined as {\tt Angle} for convenience.
	*/
	typedef TAngle<Real> Angle;

} // namespace BALL

#endif // BALL_MATHS_ANGLE_H

// $Id: box3.h,v 1.25 2000/09/27 08:00:26 oliver Exp $

#ifndef BALL_MATHS_BOX3_H
#define BALL_MATHS_BOX3_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL 
{

	/**	Generic three-dimensional Box.
			This class describes a three-dimensional box with edges parallel
			to the coordinate system. Hence, a box is given by the coordinates of the lower
			left front corner and ther upper right back corner.\\
			{\bf Definition:} \URL{BALL/MATHS/box3.h}
			\\
	*/
	template <typename T>
	class TBox3
	{
		public:

		BALL_CREATE(TBox3<T>)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TBox3 object. The three components
				of the two TVector3 are initialized to {\tt (T)0}.
		*/
		TBox3();

		/**	Copy constructor.
				Create a new TBox3 object from another.
				@param vector the TBox3 object to be copied
		*/	
		TBox3(const TBox3& box);

		/**	Detailed constructor.
				Create a new TBox3 object from two instances of TVector3.
				@param	a assigned to {\tt a}
				@param	b assigned to {\tt b}
		*/
		TBox3(const TVector3<T>& a, const TVector3<T>& b);

		/**	Detailed constructor.
				Create a new TBox3 object from six variables of type {\tt T}.
				@param	ax assigned to {\tt a.x}
				@param	ay assigned to {\tt a.y}
				@param	az assigned to {\tt a.z}
				@param	bx assigned to {\tt b.x}
  			@param	by assigned to {\tt b.y}
				@param	bz assigned to {\tt b.z}
		*/
		TBox3(const T& ax, const T& ay, const T& az,
					const T& bx, const T& by, const T& bz);

		/**	Destructor.	
				Destructs the TBox3 object. As there are no dynamic
				data structures, nothing happens.
		*/	
		virtual ~TBox3()
		{
		}
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another instance of TBox3.
				@param vector	the TBox3 object to assign from
		*/
		void set(const TBox3& box);

		/**	Assign from two points.
				@param lower the lower corner of the box
				@param upper the upper corner of the box
		*/
		void set(const TVector3<T>& lower, const TVector3<T>& upper);

		/**	Assign from six values of type {\tt T}.
				@param	ax assigned to {\tt a.x}
				@param	ay assigned to {\tt a.y}
				@param	az assigned to {\tt a.z}
				@param	bx assigned to {\tt b.x}
  			@param	by assigned to {\tt b.y}
				@param	bz assigned to {\tt b.z}
		*/
		void set(const T& ax, const T& ay, const T& az,
						 const T& bx, const T& by, const T& bz);

		/**	Assignment operator.
				Assign the box components from another instance of TBox3.
				@param box the TBox3 to assign from
		**/
		TBox3& operator = (const TBox3& box);

		/**	Assign to another instance of TBox3.
				Assigns the box components to another box.
				@param box	the box to be assigned to
		*/
		void get(TBox3& box) const;

		/**	Assign to two variables of type {\tt TVector3}.
				@param	a the first vector
				@param	b the second vector
		*/
		void get(TVector3<T>& lower, TVector3<T>& upper) const;

		/**	Assign to six variables of type {\tt T}.
				@param	ax is assigned {\tt a.x}
				@param	ay is assigned {\tt a.y}
				@param	az is assigned {\tt a.z}
				@param	bx is assigned {\tt b.x}
				@param	by is assigned {\tt b.y}
				@param	bz is assigned {\tt b.z}
		*/
		void get(T& ax, T& ay, T& az, T& bx, T& by, T& bz) const;

		/**	Swap the contents of two instances.
				@param	box the box to swap contents with
		*/
		void swap(TBox3& box);
		//@}


		/**	@name	Accessors
		*/
		//@{
		/**	Calculate the surface area.
				@return T the surface
		*/
		T getSurface() const;

		/**	Calculate the volume.
				@return T the volume
		*/
		T getVolume() const;

		/**	Return the width of the box.
				@return T the width
		*/
		T getWidth() const;
	
		/**	Return the height of the box.
				@return T the height
		*/
		T getHeight() const;
	
		/**	Return the depth of the box.
				@return T the depth
		*/
		T getDepth() const;
	
		/**	Join the box with an other.
				Instance is set to the box containing
				both, the box and a given box.
				@param	box the box containing both boxes
		*/
		void join(const TBox3& box);
		//@}

		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return bool, {\bf true} if all box components are equal, {\bf false} otherwise
		*/
		bool operator == (const TBox3& box) const;

		/**	Inequality operator.
				@return bool, {\bf true} if the two boxes differ in at least one component, {\bf false} otherwise
		*/
		bool operator != (const TBox3& box) const;

		/**	Test if a given point is a member of the box.
				Optional it can be testet, if it is a member of the surface.
				@param point the point to be tested
				@param on_surface true to test the surface (default = false)
				@return bool, {\bf true} or {\bf false}
		*/
		bool has(const TVector3<T>& point, bool on_surface = false) const;

		/**	Test if two boxes intersect.
				@param box the box to be tested
				@return bool, {\bf true} if the two boxes are intersecting, {\bf false} otherwise
		*/
		bool isIntersecting(const TBox3& box) const;
		//@}

		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/**	Test whether instance is valid.
				Always returns true.
				@return bool {\bf true}
		*/
		bool isValid() const;

		/** Internal state dump.
				Dump the current internal state of {\em *this} to 
				the output ostream {\em s} with dumping depth {\em depth}.
				@param   s - output stream where to output the internal state of {\em *this}
				@param   depth - the dumping depth
		*/
		void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}


		/**	@name	Public members
		*/
		//@{

		/**	First point of the box.
		*/
		TVector3<T> a;

		/**	Second point of the box.
		*/
		TVector3<T> b;
		//@}
	};

	template <typename T>
	TBox3<T>::TBox3()
		:	a(),
			b()
	{
	}

	template <typename T>
	TBox3<T>::TBox3(const TBox3<T>& box)
		:	a(box.a),
			b(box.b)
	{
	}

	template <typename T>
	TBox3<T>::TBox3(const TVector3<T>& lower, const TVector3<T>& upper)
		:	a(lower),
			b(upper)
	{
	}

	template <typename T>
	TBox3<T>::TBox3(const T& ax, const T& ay, const T& az,
						 		  const T& bx, const T& by, const T& bz)
		:	a(ax, ay, az),
			b(bx, by, bz)
	{
	}

	template <typename T>
	BALL_INLINE 
	void TBox3<T>::set(const TBox3<T>& box)
	{
		a.set(box.a);
		b.set(box.b);
	}

	template <typename T>
	BALL_INLINE 
	void TBox3<T>::set(const TVector3<T>& lower, const TVector3<T>& upper)
	{
		a.set(lower);
		b.set(upper);
	}

	template <typename T>
	BALL_INLINE 
	void TBox3<T>::set(const T& ax, const T& ay, const T& az,
										 const T& bx, const T& by, const T& bz)
	{
		a.set(ax, ay, az);
		b.set(bx, by, bz);
	}

	template <typename T>
	BALL_INLINE
	TBox3<T>& TBox3<T>::operator = (const TBox3<T> &box)
	{
		set(box);
		return *this;
	}

	template <typename T>
	BALL_INLINE 
	void TBox3<T>::get(TBox3<T>& box) const
	{
		box.set(*this);
	}

	template <typename T>
	BALL_INLINE 
	void TBox3<T>::get(TVector3<T>& lower, TVector3<T>& upper) const
	{
		lower.set(a);
		upper.set(b);
	}

	template <typename T>
	BALL_INLINE 
	void TBox3<T>::get(T& ax, T& ay, T& az, T& bx, T& by, T& bz) const
	{
		a.get(ax, ay, az);
		b.get(bx, by, bz);
	}

	template <typename T>
	BALL_INLINE 
	void TBox3<T>::swap(TBox3<T>& box)
	{
		a.swap(box.a);
		b.swap(box.b);
	}

	template <typename T>
	BALL_INLINE 
	T TBox3<T>::getSurface() const
	{
		T width = Maths::abs(b.x - a.x);
		T height = Maths::abs(b.y - a.y);
		T depth = Maths::abs(b.z - a.z);
		
		return ((width  * height + width  * depth  + height * depth) * 2);
	}

	template <typename T>
	BALL_INLINE 
	T TBox3<T>::getVolume() const
	{
		T width = Maths::abs(b.x - a.x);
		T height = Maths::abs(b.y - a.y);
		T depth = Maths::abs(b.z - a.z);
		
		return (width * height * depth);
	}

	template <typename T>
	BALL_INLINE 
	T TBox3<T>::getWidth() const
	{
		return Maths::abs(b.x - a.x);
	}

	template <typename T>
	BALL_INLINE
	T TBox3<T>::getHeight() const
	{
		return Maths::abs(b.y - a.y);
	}

	template <typename T>
	BALL_INLINE
	T TBox3<T>::getDepth() const
	{
		return Maths::abs(b.z - a.z);
	}

	template <typename T>
	void TBox3<T>::join(const TBox3<T>& box)
	{
		T minimum = a.x;
		minimum = Maths::min(minimum, b.x);
		minimum = Maths::min(minimum, box.a.x);
		minimum = Maths::min(minimum, box.b.x);
		
		T maximum = a.x;
		maximum = Maths::max(maximum, b.x);
		maximum = Maths::max(maximum, box.a.x);
		maximum = Maths::max(maximum, box.b.x);
		
		a.x = minimum;
		b.x = maximum;
		
		minimum = a.y;
		minimum = Maths::min(minimum, b.y);
		minimum = Maths::min(minimum, box.a.y);
		minimum = Maths::min(minimum, box.b.y);

		maximum = a.y;
		maximum = Maths::max(maximum, b.y);
		maximum = Maths::max(maximum, box.a.y);
		maximum = Maths::max(maximum, box.b.y);
		
		a.y = minimum;
		b.y = maximum;
		
		minimum = a.z;
		minimum = Maths::min(minimum, b.z);
		minimum = Maths::min(minimum, box.a.z);
		minimum = Maths::min(minimum, box.b.z);

		maximum = a.z;
		maximum = Maths::max(maximum, b.z);
		maximum = Maths::max(maximum, box.a.z);
		maximum = Maths::max(maximum, box.b.z);

		a.z = minimum;
		b.z = maximum;
	}

	template <typename T>
	BALL_INLINE 
	bool TBox3<T>::operator == (const TBox3<T>& box) const
	{
		return (a == box.a && b == box.b);
	}

	template <typename T>
	BALL_INLINE 
	bool TBox3<T>::operator != (const TBox3<T> &box) const
	{
		return !(*this == box);
	}

	template <typename T>
	BALL_INLINE 
	bool TBox3<T>::isValid() const
	{
		return (a.isValid() && b.isValid());
	}

	template <typename T>
	bool TBox3<T>::has(const TVector3<T>& point, bool on_surface) const
	{
		if (!on_surface)
		{
			if (Maths::isLess(b[0],a[0]) || Maths::isLess(b[1],a[1]) || Maths::isLess(b[2],a[2]))
			{
				for (int i = 0; i < 3; i++)
				{
					if (Maths::isLess(point[i],b[i]) || Maths::isLess(a[i],point[i]))
					{
						return false;
					}						
				}
			}
			else
			{
				for (int i = 0; i < 3; i++)
				{
					if (Maths::isLess(point[i],a[i]) || Maths::isLess(b[i],point[i]))
					{
						return false;
					}						
				}
			}
			return true;
		}

		bool temp = false;
    for (int i = 0; i < 3; i++)
		{
			if (Maths::isEqual(point[i],a[i]) || Maths::isEqual(point[i],b[i]))
			{
				temp = true;
				break;
			}
		}
		return (temp && has(point, false));
	}

	template <typename T>
	bool TBox3<T>::isIntersecting(const TBox3& box) const
	{
		const TVector3<T>* lower;
		const TVector3<T>* higher;
		const TVector3<T>* box_lower;
		const TVector3<T>* box_higher;

		if (Maths::isLess(b[0],a[0]) || 
				Maths::isLess(b[1],a[1]) || 
				Maths::isLess(b[2],a[2]))
		{
			lower  = &b;
			higher = &a;
		}
		else
		{
			lower  = &a; 
			higher = &b;
		}

		if (Maths::isLess(box.b[0],box.a[0]) || 
				Maths::isLess(box.b[1],box.a[1]) ||
        Maths::isLess(box.b[2],box.a[2]))
		{
			box_lower  = &box.b;
			box_higher = &box.a;
		}
		else
		{
			box_lower  = &box.a;
			box_higher = &box.b;;
		}

    for (int i = 0; i < 3; i++)
    {
      if (Maths::isLess((*box_higher)[i],(*lower)[i]) || Maths::isLess((*higher)[i],(*box_lower)[i]))
      {
        return false;
      }
    }
		
		return true;
	}

	template <typename T>
	void TBox3<T>::dump(std::ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		BALL_DUMP_HEADER(s, this, this);
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  a: " << a << std::endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  b: " << b << std::endl;
		
		BALL_DUMP_STREAM_SUFFIX(s);
	}

	/**	@name	Storers
	*/
	//@{

	/**	Input- Operator.
			Reads in two objects of type TVector3 a and b 
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TBox3<T>& box)
	{
		char c;
		s >> c >> box.a >> box.b >> c;
		return s;
	}

	/**	Output Operator.
			Writes the two coordinates of the box to an output stream.
			The vectors are enclosed by brackets.\\
			{\bf Example:}\\
			{\tt ((0 1 2) (7.5 3.4 10))}\\
			@see TVector3::operator<<
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TBox3<T>& box)
	{
		return s << "(" << box.a << ' ' << box.b << ')';
	}
	//@}

	/**	Default three-dimensional box class of type {\bf float}
	*/
	typedef TBox3<float> Box3;

} // namespace BALL

#endif // BALL_MATHS_BOX3_H

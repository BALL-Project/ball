// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: simpleBox3.h,v 1.9 2004/05/27 19:49:42 oliver Exp $
//

#ifndef BALL_MATHS_SIMPLEBOX3_H
#define BALL_MATHS_SIMPLEBOX3_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL 
{
	/** \defgroup SimpleBox Box
	 		\ingroup GeometricObjects
	 */
	//@{

	/**	Generic three-dimensional Box.
			This class describes a three-dimensional box with edges parallel to
			the coordinate system. Hence, a box is given by the coordinates of
			the lower left front corner and ther upper right back corner.\\
	*/
	template <typename T>
	class TSimpleBox3
	{
		public:

		BALL_CREATE(TSimpleBox3<T>)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
				This method creates a new TSimpleBox3 object. The three components
				of the two TVector3 are initialized to {\tt (T)0}.
		*/
		TSimpleBox3()
			;

		/**	Copy constructor.
				Create a new TSimpleBox3 object from another.
				@param box the TSimpleBox3 object to be copied
		*/	
		TSimpleBox3(const TSimpleBox3& box);

		/**	Detailed constructor.
				Create a new TSimpleBox3 object from two instances of TVector3.
				@param	a assigned to <TT>  a </TT>
				@param	b assigned to <TT>  b </TT>
		*/
		TSimpleBox3(const TVector3<T>& a, const TVector3<T>& b);

		/**	Detailed constructor.
				Create a new TSimpleBox3 object from six variables of type <TT>  T </TT>.
				@param	ax assigned to <TT>  a.x </TT>
				@param	ay assigned to <TT>  a.y </TT>
				@param	az assigned to <TT>  a.z </TT>
				@param	bx assigned to <TT>  b.x </TT>
  			@param	by assigned to <TT>  b.y </TT>
				@param	bz assigned to <TT>  b.z </TT>
		*/
		TSimpleBox3(const T& ax, const T& ay, const T& az, const T& bx, const T& by, const T& bz);

		/**	Destructor.	
				Destructs the TSimpleBox3 object. As there are no dynamic
				data structures, nothing happens.
		*/	
		virtual ~TSimpleBox3()
			
		{
		}

		/** Clear method.
				The values are set to 0.
		*/
		virtual void clear();

		//@}
		/**	@name	Assignment
		*/
		//@{

		/**	Assign from another instance of TSimpleBox3.
				@param box	the TSimpleBox3 object to assign from
		*/
		void set(const TSimpleBox3& box);

		/**	Assign from two points.
				@param lower the lower corner of the box
				@param upper the upper corner of the box
		*/
		void set(const TVector3<T>& lower, const TVector3<T>& upper);

		/**	Assign from six values of type <TT>  T </TT>.
				@param	ax assigned to <TT>  a.x </TT>
				@param	ay assigned to <TT>  a.y </TT>
				@param	az assigned to <TT>  a.z </TT>
				@param	bx assigned to <TT>  b.x </TT>
  			@param	by assigned to <TT>  b.y </TT>
				@param	bz assigned to <TT>  b.z </TT>
		*/
		void set(const T& ax, const T& ay, const T& az, const T& bx, const T& by, const T& bz);

		/**	Assignment operator.
				Assign the box components from another instance of TSimpleBox3.
				@param box the TSimpleBox3 to assign from
		**/
		const TSimpleBox3& operator = (const TSimpleBox3& box);

		/**	Assign to another instance of TSimpleBox3.
				Assigns the box components to another box.
				@param box	the box to be assigned to
		*/
		void get(TSimpleBox3& box) const;

		/**	Assign to two variables of type <TT>  TVector3 </TT>.
				@param lower the lower corner of the box
				@param upper the upper corner of the box
		*/
		void get(TVector3<T>& lower, TVector3<T>& upper) const;

		/**	Assign to six variables of type <TT>  T </TT>.
				@param	ax is assigned <TT>  a.x </TT>
				@param	ay is assigned <TT>  a.y </TT>
				@param	az is assigned <TT>  a.z </TT>
				@param	bx is assigned <TT>  b.x </TT>
				@param	by is assigned <TT>  b.y </TT>
				@param	bz is assigned <TT>  b.z </TT>
		*/
		void get(T& ax, T& ay, T& az, T& bx, T& by, T& bz) const;

		/**	Swap the contents of two instances.
				@param	box the box to swap contents with
		*/
		void swap(TSimpleBox3& box);

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
				both, this box and a given box.
				@param	box the box to join with
		*/
		void join(const TSimpleBox3& box);

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**	Equality operator.
				@return bool, <b>  true </b> if all box components are equal, <b>  false </b> otherwise
		*/
		bool operator == (const TSimpleBox3& box) const;

		/**	Inequality operator.
				@return bool, <b>  true </b> if the two boxes differ in at least one component, <b>  false </b> otherwise
		*/
		bool operator != (const TSimpleBox3& box) const;

		/**	Test if a given point is a member of the box.
				Optional it can be testet, if the point lies on the surface.
				@param point the point to be tested
				@param on_surface true to test the surface (default = false)
				@return bool, <b>  true </b> or <b>  false </b>
		*/
		bool has(const TVector3<T>& point, bool on_surface = false) const;

		/**	Test if two boxes intersect.
				@param box the box to be tested
				@return bool, <b>  true </b> if the two boxes are intersecting, <b>  false </b> otherwise
		*/
		bool isIntersecting(const TSimpleBox3& box) const;
		//@}

		/**	@name	Debugging and Diagnostics
		*/
		//@{

		/**	Test whether instance is valid.
				Always returns true.
				@return bool <b>  true </b>
		*/
		bool isValid() const;

		/** Internal state dump.
				Dump the current internal state of {\em *this} to 
				the output ostream <b>  s </b> with dumping depth <b>  depth </b>.
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
	//@}

	template <typename T>
	TSimpleBox3<T>::TSimpleBox3()
		:	a(),
			b()
	{
	}

	template <typename T>
	TSimpleBox3<T>::TSimpleBox3(const TSimpleBox3<T>& box)
		:	a(box.a),
			b(box.b)
	{
	}

	template <typename T>
	TSimpleBox3<T>::TSimpleBox3(const TVector3<T>& lower, const TVector3<T>& upper)
		
		:	a(lower),
			b(upper)
	{
	}

	template <typename T>
	TSimpleBox3<T>::TSimpleBox3(const T& ax, const T& ay, const T& az,
						 		  const T& bx, const T& by, const T& bz)
		
		:	a(ax, ay, az),
			b(bx, by, bz)
	{
	}

	template <typename T>
	BALL_INLINE 
	void TSimpleBox3<T>::set(const TSimpleBox3<T>& box)
		
	{
		a.set(box.a);
		b.set(box.b);
	}

	template <typename T>
	BALL_INLINE 
	void TSimpleBox3<T>::set(const TVector3<T>& lower, const TVector3<T>& upper)
		
	{
		a.set(lower);
		b.set(upper);
	}

	template <typename T>
	BALL_INLINE 
	void TSimpleBox3<T>::set(const T& ax, const T& ay, const T& az,
										 const T& bx, const T& by, const T& bz)
		
	{
		a.set(ax, ay, az);
		b.set(bx, by, bz);
	}

	template <typename T>
	BALL_INLINE
	const TSimpleBox3<T>& TSimpleBox3<T>::operator = (const TSimpleBox3<T> &box)
		
	{
		set(box);
		return *this;
	}

	template <typename T>
	BALL_INLINE 
	void TSimpleBox3<T>::get(TSimpleBox3<T>& box) const
		
	{
		box.set(*this);
	}

	template <typename T>
	BALL_INLINE 
	void TSimpleBox3<T>::get(TVector3<T>& lower, TVector3<T>& upper) const
		
	{
		lower.set(a);
		upper.set(b);
	}

	template <typename T>
	BALL_INLINE 
	void TSimpleBox3<T>::get(T& ax, T& ay, T& az, T& bx, T& by, T& bz) const
		
	{
		a.get(ax, ay, az);
		b.get(bx, by, bz);
	}

	template <typename T>
	BALL_INLINE 
	void TSimpleBox3<T>::swap(TSimpleBox3<T>& box)
		
	{
		a.swap(box.a);
		b.swap(box.b);
	}

	template <typename T>
	BALL_INLINE
	void TSimpleBox3<T>::clear()
		
	{
		a.set((T)0, (T)0, (T)0);
		b.set((T)0, (T)0, (T)0);
	}

	template <typename T>
	BALL_INLINE 
	T TSimpleBox3<T>::getSurface() const
		
	{
		T width  = Maths::abs(b.x - a.x);
		T height = Maths::abs(b.y - a.y);
		T depth  = Maths::abs(b.z - a.z);
		
		return ((width  * height + width  * depth  + height * depth) * 2);
	}

	template <typename T>
	BALL_INLINE 
	T TSimpleBox3<T>::getVolume() const
		
	{
		T width  = Maths::abs(b.x - a.x);
		T height = Maths::abs(b.y - a.y);
		T depth  = Maths::abs(b.z - a.z);
		
		return (width * height * depth);
	}

	template <typename T>
	BALL_INLINE 
	T TSimpleBox3<T>::getWidth() const
		
	{
		return Maths::abs(b.x - a.x);
	}

	template <typename T>
	BALL_INLINE
	T TSimpleBox3<T>::getHeight() const
		
	{
		return Maths::abs(b.y - a.y);
	}

	template <typename T>
	BALL_INLINE
	T TSimpleBox3<T>::getDepth() const
		
	{
		return Maths::abs(b.z - a.z);
	}

	template <typename T>
	void TSimpleBox3<T>::join(const TSimpleBox3<T>& box)
		
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
	bool TSimpleBox3<T>::operator == (const TSimpleBox3<T>& box) const
		
	{
		return (a == box.a && b == box.b);
	}

	template <typename T>
	BALL_INLINE 
	bool TSimpleBox3<T>::operator != (const TSimpleBox3<T> &box) const
		
	{
		return !(*this == box);
	}

	template <typename T>
	BALL_INLINE 
	bool TSimpleBox3<T>::isValid() const
		
	{
		return (a.isValid() && b.isValid());
	}

	template <typename T>
	bool TSimpleBox3<T>::has(const TVector3<T>& point, bool on_surface) const
		
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
	bool TSimpleBox3<T>::isIntersecting(const TSimpleBox3& box) const
		
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
	void TSimpleBox3<T>::dump(std::ostream& s, Size depth) const
		
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

	/**	Input operator.
			Reads in two objects of type TVector3 a and b 
	 		\ingroup SimpleBox
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TSimpleBox3<T>& box)
		
	{
		char c;
		s >> c >> box.a >> box.b >> c;
		return s;
	}

	/**	Output Operator.
			Writes the two coordinates of the box to an output stream.
			The vectors are enclosed by brackets.\par
			<b> Example</b> \par
			<tt> ((0 1 2) (7.5 3.4 10)) </tt>\par
			@see TVector3::operator<<
	 		\ingroup SimpleBox
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TSimpleBox3<T>& box)
		
	{
		return s << "(" << box.a << ' ' << box.b << ')';
	}

	/**	Default three-dimensional box class of type <b> float</b>
	 		\ingroup SimpleBox
	*/
	typedef TSimpleBox3<float> SimpleBox3;

} // namespace BALL

#endif // BALL_MATHS_SimpleBox3_H

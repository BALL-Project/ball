// $Id: box3.h,v 1.1 1999/08/26 07:53:18 oliver Exp $

#ifndef BALL_MATHS_BOX3_H
#define BALL_MATHS_BOX3_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL {

	/**	Generic Three-dimensional Box.
			This class describes a three-dimensional box with edges parallel
			to the coordinate system. Hence, a box is given by the coordinates of the lower
			left front corner and ther upper right back corner.\\
			{\bf Definition:} \URL{BALL/MATHS/box3.h}
			\\
	*/
	template <class T>
	class TBox3
	{
		public:

		BALL_CREATE(TBox3<T>)

		/**	@name	Constructors and Destructors
		*/
		//@{

		///
		TBox3(void);

		///
		TBox3(const TBox3& box, bool deep = true);

		///
		TBox3(const TVector3<T>& a, const TVector3<T>& b);

		///
		TBox3
			(const T& ax, const T& ay, const T& az,
			 const T& bx, const T& by, const T& bz);

		///
		virtual ~TBox3(void);
		//@}

		/**	@name	Assignment
		*/
		//@{

		///
		void set(const TBox3& box, bool deep = true);

		///
		void set(const TVector3<T>& a, const TVector3<T>& b);

		///
		void set
			(const T& ax, const T& ay, const T& az,
			 const T& bx, const T& by, const T& bz);

		///
		TBox3& operator = (const TBox3& box);

		///
		void get(TBox3& box, bool deep = true) const;

		///
		void get(TVector3<T>& a, TVector3<T>& b) const;

		///
		void get(T& ax, T& ay, T& az, T& bx, T& by, T& bz) const;

		///
		void swap(TBox3& box);
		//@}


		/**	@name	Accessors
		*/
		//@{
		///
		T getSurface(void) const;

		///
		T getVolume(void) const;

		///
		T getWidth(void) const;
	
		///
		T getHeight(void) const;
	
		///
		T getDepth(void) const;
	
		///
		void join(const TBox3& box);
		//@}

		/**	@name	Predicates
		*/
		//@{

		///
		bool operator == (const TBox3& box) const;

		///
		bool operator != (const TBox3& box) const;
		//@}

		/**	@name	Debugging and Diagnostics
		*/
		//@{
		///
		bool isValid(void) const;

		///
		void dump(ostream& s = cout, unsigned long depth = 0) const;
		//@}


		/**	@name	Public members
		*/
		//@{
		///
		TVector3<T> a;

		///
		TVector3<T> b;
		//@}
	};

	template <class T>
	TBox3<T>::TBox3(void)
		:	a(0, 0, 0),
			b(0, 0, 0)
	{
	}

	template <class T>
	TBox3<T>::TBox3(const TBox3<T>& box, bool /* deep */)
		:	a(box.a),
			b(box.b)
	{
	}

	template <class T>
	TBox3<T>::TBox3(const TVector3<T>& a, const TVector3<T>& b)
		:	a(a),
			b(b)
	{
	}

	template <class T>
	TBox3<T>::TBox3
		(const T& ax, const T& ay, const T& az,
		 const T& bx, const T& by, const T& bz)
		:	a(ax, ay, az),
			b(bx, by, bz)
	{
	}

	template <class T>
	TBox3<T>::~TBox3(void)
	{
	}

	template <class T>
	BALL_INLINE 
	void TBox3<T>::set(const TBox3<T>& box, bool /* deep */)
	{
		a.set(box.a);
		b.set(box.b);
	}

	template <class T>
	BALL_INLINE 
	void TBox3<T>::set(const TVector3<T>& new_a, const TVector3<T>& new_b)
	{
		a.set(new_a);
		b.set(new_b);
	}

	template <class T>
	BALL_INLINE 
	void TBox3<T>::set
		(const T& ax, const T& ay, const T& az,
		 const T& bx, const T& by, const T& bz)
	{
		a.set(ax, ay, az);
		b.set(bx, by, bz);
	}

	template <class T>
	BALL_INLINE
	TBox3<T>& TBox3<T>::operator = (const TBox3<T> &box)
	{
		set(box);
		
		return *this;
	}

	template <class T>
	BALL_INLINE 
	void TBox3<T>::get(TBox3<T>& box, bool deep) const
	{
		box.set(*this, deep);
	}

	template <class T>
	BALL_INLINE 
	void TBox3<T>::get(TVector3<T>& a, TVector3<T>& b) const
	{
		a.set(a);
		b.set(b);
	}

	template <class T>
	BALL_INLINE 
	void TBox3<T>::get(T& ax, T& ay, T& az, T& bx, T& by, T& bz) const
	{
		a.get(ax, ay, az);
		b.get(bx, by, bz);
	}

	template <class T>
	BALL_INLINE 
	void TBox3<T>::swap(TBox3<T>& box)
	{
		a.swap(box.a);
		b.swap(box.b);
	}

	template <class T>
	BALL_INLINE 
	T TBox3<T>::getSurface(void) const
	{
		T width = Maths::abs(b.x - a.x);
		T height = Maths::abs(b.y - a.y);
		T depth = Maths::abs(b.z - a.z);
		
		return ((width  * height + width  * depth  + height * depth) * 2);
	}

	template <class T>
	BALL_INLINE 
	T TBox3<T>::getVolume(void) const
	{
		T width = Maths::abs(b.x - a.x);
		T height = Maths::abs(b.y - a.y);
		T depth = Maths::abs(b.z - a.z);
		
		return (width * height * depth);
	}

	template <class T>
	BALL_INLINE 
	T TBox3<T>::getWidth(void) const
	{
		return Maths::abs(b.x - a.x);
	}

	template <class T>
	BALL_INLINE T 
	TBox3<T>::getHeight(void) const
	{
		return Maths::abs(b.y - a.y);
	}

	template <class T>
	BALL_INLINE T TBox3<T>::getDepth(void) const
	{
		return Maths::abs(b.z - a.z);
	}

	template <class T>
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

	template <class T>
	BALL_INLINE 
	bool TBox3<T>::operator == (const TBox3<T>& box) const
	{
		return (bool)(a == box.a && b == box.b);
	}

	template <class T>
	BALL_INLINE 
	bool TBox3<T>::operator != (const TBox3<T> &box) const
	{
		return (bool)!(*this == box);
	}

	template <class T>
	BALL_INLINE 
	bool TBox3<T>::isValid(void) const
	{
		return (bool)(a.isValid() && b.isValid());
	}

	template <class T>
	void TBox3<T>::dump(ostream& s, unsigned long depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		BALL_DUMP_HEADER(s, this, this);
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  a: " << a << endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  b: " << b << endl;
		
		BALL_DUMP_STREAM_SUFFIX(s);
	}


	///
	typedef TBox3<Real> Box3;

} // namespace BALL

#endif // BALL_MATHS_BOX3_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: box3.h,v 1.39 2003/08/19 16:43:09 amoll Exp $

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
    /**  \addtogroup  GeometricObjects
     *  @{
     */
	/** @name Box
	 */
	//@{

	/**	Generic three-dimensional Box.
			This class describes a three-dimensional box.
			A box is given by the coordinates of the lower left front corner,
			width, height, depth and the right vector.\\
			{\bf Definition:} \URL{BALL/MATHS/box3.h}\\
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
		*/
		TBox3()
			throw();

		/**	Copy constructor.
				@param box the TBox3 object to be copied
		*/	
		TBox3(const TBox3& box)
			throw();

		/**	Detailed constructor.
		*/
		TBox3(const TVector3<T>& point, 
					const TVector3<T>& right_vector  = TVector3<T>((T) 0, (T)1, (T)0),
					const TVector3<T>& height_vector = TVector3<T>((T)-1, (T)0, (T)0),
					const T& depth = 1)
			throw();

		/**	Destructor.	
		*/	
		virtual ~TBox3()
			throw()
		{
		}

		/** Clear method.
				The values are set to 0.
		*/
		virtual void clear()
			throw();

		//@}
		/**	@name	Assignment */
		//@{

		/**	Assign from another instance of TBox3.
				@param box	the TBox3 object to assign from
		*/
		void set(const TBox3& box)
			throw();

		/**	Assign from two points.
				@param lower the lower corner of the box
				@param upper the upper corner of the box
		*/
		void set(const TVector3<T>& lower, const TVector3<T>& upper)
			throw();

		/**	Assignment operator.
				Assign the box components from another instance of TBox3.
				@param box the TBox3 to assign from
		**/
		const TBox3& operator = (const TBox3& box)
			throw();

		/**	Assign to another instance of TBox3.
				Assigns the box components to another box.
				@param box	the box to be assigned to
		*/
		void get(TBox3& box) const
			throw();

		/**	Swap the contents of two instances.
				@param	box the box to swap contents with
		*/
		void swap(TBox3& box)
			throw();

		//@}
		/**	@name	Accessors */
		//@{
		
		/// Set width
		void setWidth(T width)
			throw();

		/// Get width
		T getWidth() const
			throw() { return width_;}

		/// Set height
		void setHeight(T height) 
			throw();
		
		/// Get height
		T getHeight() const
			throw() { return height_;}

		/// Set depth
		void setDepth(T depth) 
			throw() { depth_ = depth;}

		/// Get depth
		T getDepth() const
			throw() { return depth_;}

		/// Get the point
		const TVector3<T>& getPoint() const
			throw() { return point_;}

		/// Set the point
		void setPoint(const TVector3<T>& point) 
			throw() { point_ = point;}

		/// Get the right vector
		const TVector3<T>& getRightVector() const
			throw() { return right_vector_;}

		/// Set the right vector
		void setRightVector(const TVector3<T>& v)
			throw();

		/// Get the height vector
		const TVector3<T>& getHeightVector() const
			throw() { return height_vector_;}
		
		/// Set the height vector
		void setHeightVector(const TVector3<T>& v)
			throw();
		
		/**	Calculate the surface area.
				@return T the surface
		*/
		T getSurface() const
			throw();

		/**	Calculate the volume.
				@return T the volume
		*/
		T getVolume() const
			throw();

		///
		TVector3<T> getDiagonalVector() const
			throw();

		//@}
		/**	@name	Predicates */
		//@{

		/**	Equality operator.
				@return bool, <b>true</b> if all box components are equal, <b>false</b> otherwise
		*/
		bool operator == (const TBox3& box) const
			throw();

		/**	Inequality operator.
				@return bool, <b>true</b> if the two boxes differ in at least one component, <b>false</b> otherwise
		*/
		bool operator != (const TBox3& box) const
			throw();

		/*	Test if a given point is a member of the box.
				Optional it can be testet, if the point lies on the surface.
				@param point the point to be tested
				@param on_surface true to test the surface (default = false)
				@return bool, <b>true</b> or <b>false</b>
		*/
		//bool has(const TVector3<T>& point, bool on_surface = false) const
		//	throw();

		/*	Test if two boxes intersect.
				@param box the box to be tested
				@return bool, <b>true</b> if the two boxes are intersecting, <b>false</b> otherwise
		*/
		//bool isIntersecting(const TBox3& box) const
		//	throw();
	
		//@}
		/**	@name	Debugging and Diagnostics */
		//@{

		/**	Test whether instance is valid.
				Always returns true.
				@return bool <b>true</b>
		*/
		bool isValid() const
			throw();

		/** Internal state dump.
				Dump the current internal state of {\em *this} to 
				the output ostream {\em s} with dumping depth {\em depth}.
				@param   s - output stream where to output the internal state of {\em *this}
				@param   depth - the dumping depth
		*/
		void dump(std::ostream& s = std::cout, Size depth = 0) const
			throw();
		//@}

		protected:

		//_	lower left point of the box.
		TVector3<T> point_;
		TVector3<T> right_vector_;
		TVector3<T> height_vector_;

		T width_;
		T height_;
		T depth_;
	};


	template <typename T>
	TBox3<T>::TBox3(const TVector3<T>& point, 
									const TVector3<T>& right_vector,
									const TVector3<T>& height_vector,
									const T& depth)
		throw()
		: point_(point),
			right_vector_(right_vector),
			height_vector_(height_vector),
			width_(right_vector.getLength()),
			height_(height_vector.getLength()),
			depth_(depth)
	{}

	template <typename T>
	TBox3<T>::TBox3()
		throw()
		:	point_((T)0, (T)0, (T)0),
			right_vector_((T)0, (T)1, (T)0),
			height_vector_((T)-1, (T)0, (T)0),
			width_((T)1),
			height_((T)1),
			depth_((T)1)
	{
	}

	template <typename T>
	TBox3<T>::TBox3(const TBox3<T>& box)
		throw()
		: point_(box.point_),
			right_vector_(box.right_vector_),
			height_vector_(box.height_vector_),
			width_(box.width_),
			height_(box.height_),
			depth_(box.depth_)
	{
	}

	template <typename T>
	void TBox3<T>::set(const TBox3<T>& box)
		throw()
	{
		point_ = box.point_;
		right_vector_ = box.right_vector_;
		height_vector_ = box.height_vector_;
		width_ = box.width_;
		height_ = box.height_;
		depth_ = box.depth_;
	}

	template <typename T>
	BALL_INLINE
	const TBox3<T>& TBox3<T>::operator = (const TBox3<T> &box)
		throw()
	{
		set(box);
		return *this;
	}

	template <typename T>
	void TBox3<T>::swap(TBox3<T>& box)
		throw()
	{
		point_.swap(box.swap);
		right_vector_.swap(box.right_vector_);
		height_vector_.swap(box.height_vector_);
		T tmp;

		tmp = width_;
		width_ = box.width_;
		box.width_ = tmp;

		tmp = height_;
		height_ = box.height_;
		box.height_ = tmp.height_;

		tmp = depth_;
		depth_ = box.depth_;
		box.depth_ = tmp;
	}

	template <typename T>
	void TBox3<T>::clear()
		throw()
	{
		point_.clear();
		right_vector_ = TVector3<T>((T)0, (T)1, (T)0);
		height_vector_ = TVector3<T>((T)-1, (T)0, (T)0);
		width_ = (T) 1;
		height_ = (T) 1;
		depth_  = (T) 1;
	}
	

	template <typename T>
	BALL_INLINE
	void TBox3<T>::setWidth(T width) 
		throw()
	{
		right_vector_.normalize();
		right_vector_ *= width;
		width_ = width;
	}

	template <typename T>
	BALL_INLINE
	void TBox3<T>::setHeight(T height) 
		throw()
	{
		height_vector_.normalize();
		height_vector_ *= height;
		height_ = height;
	}

	template <typename T>
	BALL_INLINE
	void TBox3<T>::setRightVector(const TVector3<T>& v)
		throw()
	{
		right_vector_ = v;
		width_ = right_vector_.getLength();
	}

	template <typename T>
	BALL_INLINE
	void TBox3<T>::setHeightVector(const TVector3<T>& v)
		throw()
	{
		height_vector_ = v;
		height_ = height_vector_.getLength();
	}

	template <typename T>
	BALL_INLINE 
	T TBox3<T>::getSurface() const
		throw()
	{
		return ((width_  * height_ + width_  * depth_  + height_ * depth_) * 2);
	}

	template <typename T>
	TVector3<T> TBox3<T>::getDiagonalVector() const
		throw()
	{
		TVector3<T> v = right_vector_ % height_vector_;
		v.normalize();
		v *= depth_;
		return (v + right_vector_ + height_vector_);
	}

	template <typename T>
	BALL_INLINE 
	T TBox3<T>::getVolume() const
		throw()
	{
		return (width_ * height_ * depth_);
	}

	template <typename T>
	bool TBox3<T>::operator == (const TBox3<T>& box) const
		throw()
	{
		return (point_ 					== box.point_ 					&&
						right_vector_ 	== box.right_vector_ 		&&
						height_vector_ 	== box.height_vector_ 	&&
						width_ 					== box.width_ 					&&
						height_ 				== box.height_ 					&&
						depth_ 					== box.depth_);
	}

	template <typename T>
	BALL_INLINE 
	bool TBox3<T>::operator != (const TBox3<T> &box) const
		throw()
	{
		return !(*this == box);
	}

	template <typename T>
	BALL_INLINE 
	bool TBox3<T>::isValid() const
		throw()
	{
		return (point_.isValid() && 
						right_vector_.isValid() &&
						height_vector_.isValid() &&
						!right_vector_.isZero() &&
						!height_vector_.isZero() && 
						right_vector_.isOrthogonalTo(height_vector_));
						
	}

	template <typename T>
	void TBox3<T>::dump(std::ostream& s, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		BALL_DUMP_HEADER(s, this, this);
		
		BALL_DUMP_DEPTH(s, depth);
		s << "point: " << point_ << std::endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "right_vector: " << right_vector_ << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "height_vector: " << height_vector_ << std::endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "width: " << width_ << std::endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "height: " << height_ << std::endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "depth: " << depth_ << std::endl;
		
		BALL_DUMP_STREAM_SUFFIX(s);
	}

	/**	@name	Storers
	*/
	//@{

	/**	Input operator.
			Reads in two objects of type TVector3 a and b 
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TBox3<T>& box)
		throw()
	{
		TVector3<T> point, right, height;
		T depth;
		s >> point >> right >> height >> depth;
		box.setPoint(point);
		box.setRightVector(right);
		box.setHeightVector(height);
		box.setDepth(depth);
		return s;
	}

	/**	Output Operator.
			Writes the two coordinates of the box to an output stream.
			The vectors are enclosed by brackets. \par
			<b>Example:</b> \par
			<tt>((0 1 2) (7.5 3.4 10))</tt> \par
			@see TVector3::operator<<
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TBox3<T>& box)
		throw()
	{
		return s << box.getPoint() << " " 
						 << box.getRightVector() << " " 
						 << box.getHeightVector() << " " 
						 << box.getDepth();
	}

	//@}

	/**	Default three-dimensional box class of type <b>float</b>
	*/
	typedef TBox3<float> Box3;

} // namespace BALL

#endif // BALL_MATHS_BOX3_H

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: box3.h,v 1.46 2005/12/23 17:01:48 amoll Exp $
//

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
	/** \defgroup Box Generic three-dimensional box.

      \ingroup  GeometricObjects
	 */
	//@{

	/**	Generic three-dimensional Box.
			This class describes a three-dimensional box.
			A box is given by the coordinates of the lower left front corner,
			a right vector, a height vector and a depth vector.\\
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
		TBox3();

		/**	Copy constructor.
				@param box the TBox3 object to be copied
		*/	
		TBox3(const TBox3& box);

		/**	Detailed constructor.
				@param point the lower left corner of the box
				@param right_vector the right vector of the box
				@param heigth_vector the height vector of the box
				@param depth the depth of the box
		*/
		TBox3(const TVector3<T>& point, 
					const TVector3<T>& right_vector,
					const TVector3<T>& height_vector,
					const T& depth = 1);
		
		/**	Detailed constructor.
				@param point the lower left corner of the box
				@param right_vector the right vector of the box
				@param heigth_vector the height vector of the box
				@param depth_vector the depth vector of the box
		*/
		TBox3(const TVector3<T>& point, 
					const TVector3<T>& right_vector  = TVector3<T>((T) 0, (T)1, (T)0),
					const TVector3<T>& height_vector = TVector3<T>((T) 1, (T)0, (T)0),
					const TVector3<T>& depth_vector  = TVector3<T>((T) 0, (T)0, (T)1));
		
		/**	Destructor.	
		*/	
		virtual ~TBox3()
		{
		}

		/** Clear method.
				The values are set to 0.
		*/
		virtual void clear();

		//@}
		/**	@name	Assignment */
		//@{

		/**	Assign from another instance of TBox3.
				@param box	the TBox3 object to assign from
		*/
		void set(const TBox3& box);

		/**	Assignment operator.
				Assign the box components from another instance of TBox3.
				@param box the TBox3 to assign from
		**/
		const TBox3& operator = (const TBox3& box);

		/**	Swap the contents of two instances.
				@param	box the box to swap contents with
		*/
		void swap(TBox3& box);

		//@}
		/**	@name	Accessors */
		//@{
		
		/// Set width
		void setWidth(T width);

		/// Get width
		T getWidth() const { return width_;}

		/// Set height
		void setHeight(T height);
		
		/// Get height
		T getHeight() const { return height_;}

		/// Set depth
		void setDepth(T depth);

		/// Get depth
		T getDepth() const { return depth_;}

		/// Get the point
		const TVector3<T>& getPoint() const { return point_;}

		/// Set the point
		void setPoint(const TVector3<T>& point)  { point_ = point;}

		/// Get the right vector
		const TVector3<T>& getRightVector() const { return right_vector_;}

		/// Set the right vector
		void setRightVector(const TVector3<T>& v);

		/// Get the height vector
		const TVector3<T>& getHeightVector() const { return height_vector_;}
		
		/// Set the height vector
		void setHeightVector(const TVector3<T>& v);
		
		/// Get the depth vector
		const TVector3<T>& getDepthVector() const
			{ return depth_vector_;}
		
		/// Set the depth vector
		void setDepthVector(const TVector3<T>& v);
		
		/**	Calculate the surface area.
				@return T the surface
		*/
		T getSurface() const;

		/**	Calculate the volume.
				@return T the volume
		*/
		T getVolume() const;

		///
		TVector3<T> getDiagonalVector() const;

		//@}
		/**	@name	Predicates */
		//@{

		/**	Equality operator.
				@return bool, <b>true</b> if all box components are equal, <b>false</b> otherwise
		*/
		bool operator == (const TBox3& box) const;

		/**	Inequality operator.
				@return bool, <b>true</b> if the two boxes differ in at least one component, <b>false</b> otherwise
		*/
		bool operator != (const TBox3& box) const;

		/*	Test if a given point is a member of the box.
				Optional it can be testet, if the point lies on the surface.
				@param point the point to be tested
				@param on_surface true to test the surface (default = false)
				@return bool, <b>true</b> or <b>false</b>
		*/
		//bool has(const TVector3<T>& point, bool on_surface = false) const;

		/*	Test if two boxes intersect.
				@param box the box to be tested
				@return bool, <b>true</b> if the two boxes are intersecting, <b>false</b> otherwise
		*/
		//bool isIntersecting(const TBox3& box) const;
	
		//@}
		/**	@name	Debugging and Diagnostics */
		//@{

		/**	Test whether instance is valid.
				Always returns true.
				@return bool <b>true</b>
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

		protected:

		//_	lower left point of the box.
		TVector3<T> point_;
		TVector3<T> right_vector_;
		TVector3<T> height_vector_;
		TVector3<T> depth_vector_;

		T width_;
		T height_;
		T depth_;
	};
	//@}


	template <typename T>
	TBox3<T>::TBox3(const TVector3<T>& point, const TVector3<T>& right_vector,const TVector3<T>& height_vector,const T& depth)
		: point_(point),
			right_vector_(right_vector),
			height_vector_(height_vector),
			depth_vector_((right_vector % height_vector).normalize() * depth),
			width_(right_vector.getLength()),
			height_(height_vector.getLength()),
			depth_(depth)
	{}
	
	template <typename T>
	TBox3<T>::TBox3(const TVector3<T>& point, 
									const TVector3<T>& right_vector,
									const TVector3<T>& height_vector,
									const TVector3<T>& depth_vector)
		: point_(point),
			right_vector_(right_vector),
			height_vector_(height_vector),
			depth_vector_(depth_vector),
			width_(right_vector.getLength()),
			height_(height_vector.getLength()),
			depth_(depth_vector.getLength())
	{}
	
	template <typename T>
	TBox3<T>::TBox3()
		:	point_((T)0, (T)0, (T)0),
			right_vector_((T)0, (T)1, (T)0),
			height_vector_((T)1, (T)0, (T)0),
			depth_vector_((T)0, (T)0, (T)1),
			width_((T)1),
			height_((T)1),
			depth_((T)1)
	{
	}

	template <typename T>
	TBox3<T>::TBox3(const TBox3<T>& box)
		: point_(box.point_),
			right_vector_(box.right_vector_),
			height_vector_(box.height_vector_),
			depth_vector_(box.depth_vector_),
			width_(box.width_),
			height_(box.height_),
			depth_(box.depth_)
	{
	}

	template <typename T>
	void TBox3<T>::set(const TBox3<T>& box)
	{
		point_ = box.point_;
		right_vector_ = box.right_vector_;
		height_vector_ = box.height_vector_;
		depth_vector_ = box.depth_vector_;
		width_ = box.width_;
		height_ = box.height_;
		depth_ = box.depth_;
	}

	template <typename T>
	BALL_INLINE
	const TBox3<T>& TBox3<T>::operator = (const TBox3<T> &box)
	{
		set(box);
		return *this;
	}

	template <typename T>
	void TBox3<T>::swap(TBox3<T>& box)
	{
		point_.swap(box.point_);
		right_vector_.swap(box.right_vector_);
		height_vector_.swap(box.height_vector_);
		depth_vector_.swap(box.depth_vector_);

		std::swap(width_, box.width_);
		std::swap(height_, box.height_);
		std::swap(depth_, box.depth_);
	}

	template <typename T>
	void TBox3<T>::clear()
	{
		point_.clear();
		right_vector_ = TVector3<T>((T)0, (T)1, (T)0);
		height_vector_ = TVector3<T>((T)-1, (T)0, (T)0);
		depth_vector_ = TVector3<T>((T)0, (T)0, (T)1);
		width_ = (T) 1;
		height_ = (T) 1;
		depth_  = (T) 1;
	}
	

	template <typename T>
	BALL_INLINE
	void TBox3<T>::setWidth(T width) 
	{
		right_vector_.normalize();
		right_vector_ *= width;
		width_ = width;
	}

	template <typename T>
	BALL_INLINE
	void TBox3<T>::setHeight(T height)
	{
		height_vector_.normalize();
		height_vector_ *= height;
		height_ = height;
	}

	template <typename T>
	BALL_INLINE
	void TBox3<T>::setDepth(T depth) 
	{
		depth_vector_.normalize();
		depth_vector_ *= depth;
		depth_ = depth;
	}
	
	template <typename T>
	BALL_INLINE
	void TBox3<T>::setRightVector(const TVector3<T>& v)
	{
		right_vector_ = v;
		width_ = right_vector_.getLength();
	}

	template <typename T>
	BALL_INLINE
	void TBox3<T>::setHeightVector(const TVector3<T>& v)
	{
		height_vector_ = v;
		height_ = height_vector_.getLength();
	}
	
	template <typename T>
	BALL_INLINE
	void TBox3<T>::setDepthVector(const TVector3<T>& v)
	{
		depth_vector_ = v;
		depth_ = depth_vector_.getLength();
	}
	
	template <typename T>
	BALL_INLINE 
	T TBox3<T>::getSurface() const
	{
		return ((width_  * height_ + width_  * depth_  + height_ * depth_) * 2);
	}
	
	template <typename T>
	TVector3<T> TBox3<T>::getDiagonalVector() const
	{
		return (right_vector_ + height_vector_ + depth_vector_);
	}

	template <typename T>
	BALL_INLINE 
	T TBox3<T>::getVolume() const
	{
		return (width_ * height_ * depth_);
	}

	template <typename T>
	bool TBox3<T>::operator == (const TBox3<T>& box) const
	{
		return (point_ 					== box.point_ 					&&
						right_vector_ 	== box.right_vector_ 		&&
						height_vector_ 	== box.height_vector_ 	&&
						depth_vector_ 	== box.depth_vector_ 		&&
						width_ 					== box.width_ 					&&
						height_ 				== box.height_ 					&&
						depth_ 					== box.depth_);
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
		return (point_.isValid() && 
						right_vector_.isValid() &&
						height_vector_.isValid() &&
						depth_vector_.isValid() &&
						!right_vector_.isZero() &&
						!height_vector_.isZero() && 
						!depth_vector_.isZero());
						
	}

	template <typename T>
	void TBox3<T>::dump(std::ostream& s, Size depth) const
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
		s << "depth_vector: " << depth_vector_ << std::endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "width: " << width_ << std::endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "height: " << height_ << std::endl;
		
		BALL_DUMP_DEPTH(s, depth);
		s << "depth: " << depth_ << std::endl;
		
		BALL_DUMP_STREAM_SUFFIX(s);
	}

	/** @name Storers
	 		\ingroup Box
	*/
	//@{
	/**	Input operator.
			Reads in four objects of type TVector3 point, right, height, depth 
	*/
	template <typename T>
	std::istream& operator >> (std::istream& s, TBox3<T>& box)
	{
		TVector3<T> point, right, height, depth;
		s >> point >> right >> height >> depth;
		box.setPoint(point);
		box.setRightVector(right);
		box.setHeightVector(height);
		box.setDepthVector(depth);
		return s;
	}

	/**	Output Operator.
			Writes the coordinates of the lower left corner of the box,
			and the three spanning vectors to an output stream.
			The vectors are enclosed by brackets. \par
			<b>Example:</b> \par
			<tt>((0 0 0) (0 1 0) (1 0 0) (0 0 1)</tt> \par
			@see TVector3::operator<<
	*/
	template <typename T>
	std::ostream& operator << (std::ostream& s, const TBox3<T>& box)
	{
		return s << box.getPoint() << " " 
						 << box.getRightVector() << " " 
						 << box.getHeightVector() << " " 
						 << box.getDepthVector();
	}
	//@}

	/**	Default three-dimensional box class of type <b>float</b>
      \ingroup Box 
	*/
	typedef TBox3<float> Box3;

} // namespace BALL

#endif // BALL_MATHS_BOX3_H

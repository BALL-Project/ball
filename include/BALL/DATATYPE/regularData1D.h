// $Id: regularData1D.h,v 1.5 2000/11/02 18:23:14 oliver Exp $

#ifndef BALL_DATATYPE_REGULARDATA1D_H
#define BALL_DATATYPE_REGULARDATA1D_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <vector>

namespace BALL
{

	/**	A class to store regularly spaced data.
			This class can is intended to hold regularly spaced, one-dimensional data sets.
			It might be useful to hold data sets like spectra, or precomputed funciton values.
			\\
			{\bf Definition:} \URL{BALL/DATATYPE/regularData1D.h}
	*/
	template <typename T>
	class TRegularData1D
	{
		public:
			
		BALL_CREATE(TRegularData1D<T>)

		/**	@name Type definitions
		*/
		//@{
		/**	The vector type.
				This type is used to store the data.
		*/
		typedef std::vector<T>	VectorType;
		//@}
	
		/** @name Constructors and Destructors.
		*/
		//@{
			
		/**	Default constructor
		*/
		TRegularData1D();

		/**	Copy constructor
		*/
		TRegularData1D(const TRegularData1D& data);

		/**	Destructor
		*/
		virtual ~TRegularData1D();

		/**	Clear the contents
		*/
		virtual void clear();

		/**	Clear the contents and reset the object dimensions
		*/
		virtual void destroy();

		//@}

		/**	@name Assignment
		*/
		//@{

		/**	Assignment operator.
				Copy the data and the boundaries.
		*/
		const TRegularData1D& operator = (const TRegularData1D& data);

		/**	Assignment from a {\tt vector} of {\tt T}.
				Copy the contents of the data without changing the boundaries.
		*/
		const TRegularData1D& operator = (const VectorType& data);
		//@}
		
		/**	@name Predicates
		*/
		//@{
		/**	Equality operator
		*/
		bool operator == (const TRegularData1D& data) const;
		//@}
	
		/**	@name	Accessors
		*/
		//@{	
			
		/**	Constant random access operator.
				@exception IndexOverflow if {\tt index} is out of range
		*/	
		const T& operator [] (Position index) const;
			
		/**	Mutable random access operator.
				@exception IndexOverflow if {\tt index} is out of range
		*/	
		T& operator [] (Position index);
			
		/**	Return the number of items 
		*/
		Size getSize() const;

		/**	Return the lower bound
		*/
		double getLower() const;
		
		/**	Return the upper bound
		*/
		double getUpper() const;

		/**	Set the upper bound
		*/
		void setUpper(double upper);

		/**	Set the lower bound
		*/
		void setLower(double lower);

		/**	Resize the data.
				If {\tt new_size} is larger than the current size, the data {\tt vector}
				is extended to the new size and filled with default constructed items of type
				{\tt T}. Resizing to a value lesser than the current size truncates the vector.
				\\
				The boundaries are adapted keep the positions of the retained items fixed, i.e.
				the upper bound is increased or decreased proportionally while the lower bound
				remains unchanged.
				
				@param new_size the new size
		*/
		void resize(Size new_size);
		//@}
	
		protected:
		/**	The lower bound
		*/
		double			lower_;

		/**	The upper bound
		*/
		double			upper_;

		/**	The data
		*/
		VectorType	data_;
	};

	/**	Default type
	*/
	typedef TRegularData1D<float> RegularData1D;
	
	template <typename T>
	TRegularData1D<T>::TRegularData1D()
	{
	}

	template <typename T>
	TRegularData1D<T>::~TRegularData1D()
	{
	}

	template <typename T>
	TRegularData1D<T>::TRegularData1D(const TRegularData1D<T>& data)
		:	lower_(data.lower_),
			upper_(data.upper_),
			data_(data.data_)
	{
	}

	template <typename T>
	void TRegularData1D<T>::clear()
	{
		// iterate over the data and reset all values to their default
		// boundaries and vector size remain unchanged
		T default_value;
		VectorType::iterator it = data_.begin();
		for (; it != data_.end(); ++it)
		{
			*it = default_value;
		}
	}

	template <typename T>
	void TRegularData1D<T>::destroy()
	{
		// clear the vector and the boundaries
		data_.clear();
		lower_ = 0.0;
		upper_ = 0.0;
	}

	template <typename T>
	const TRegularData1D<T>& TRegularData1D<T>::operator = (const TRegularData1D<T>& data)
	{
		// copy all members...
		data_ = data.data_;
		lower_ = data.lower_;
		upper_ = data.upper_;
	}

	template <typename T>
	const TRegularData1D<T>& TRegularData1D<T>::operator = (const TRegularData1D<T>::VectorType& data)
	{
		// Copy the data. The boundaries remain unchanged.
		data_ = data;
	}

	template <typename T>
	bool TRegularData1D<T>::operator == (const TRegularData1D<T>& data) const
	{
		return ((lower_ == data.lower_) 
						&& (upper_ == data.upper_)
						&& (data_ == data.data_));
	}
	
	template <typename T>
	BALL_INLINE
	const T& TRegularData1D<T>::operator [] (Position index) const
	{
		if (index >= data_.size)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, index);
		}
		
		return data[index];
	}	
	
	template <typename T>
	BALL_INLINE
	T& TRegularData1D<T>::operator [] (Position index)
	{
		if (index >= data_.size)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, index);
		}
		
		return data[index];
	}	

	template <typename T>
	BALL_INLINE
	Size TRegularData1D<T>::getSize() const
	{
		return data_.size();
	}
	
	template <typename T>
	BALL_INLINE
	double TRegularData1D<T>::getLower() const
	{
		return lower_;
	}
	
	template <typename T>
	BALL_INLINE
	double TRegularData1D<T>::getUpper() const
	{
		return upper_;
	}

	template <typename T>
	BALL_INLINE
	void TRegularData1D<T>::setLower(double lower)
	{
		lower_ = lower;
	}
	
	template <typename T>
	BALL_INLINE
	void TRegularData1D<T>::setUpper(double lower)
	{
		upper_ = upper;
	}
	
	template <typename T>
	void resize(Size new_size)
	{
		if (data_.size() > 0)
		{
			upper_ *= (double)new_size / (double)data_.size();
		}
		data_.resize(new_size);
	}
		
} // namespace BALL

#endif // BALL_DATATYPE_REGULARDATA1D_H

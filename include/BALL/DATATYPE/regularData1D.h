// $Id: regularData1D.h,v 1.11 2001/03/08 11:26:44 amoll Exp $

#ifndef BALL_DATATYPE_REGULARDATA1D_H
#define BALL_DATATYPE_REGULARDATA1D_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <vector>

namespace BALL
{

	/**	A class to store regularaly spaced data.
			This class can is intended to hold regularly spaced, one-dimensional data sets.
			It might be useful to hold data sets like spectra, or precomputed function values.\\
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
		TRegularData1D()
			throw();

		/**	Copy constructor
		*/
		TRegularData1D(const TRegularData1D& data)
			throw();

		/**	Destructor
		*/
		virtual ~TRegularData1D()
			throw();

		/**	Clear the contents
		*/
		virtual void clear()
			throw();

		/**	Clear the contents and reset the object dimensions
		*/
		virtual void destroy()
			throw();

		//@}
		/**	@name Assignment
		*/
		//@{

		/**	Assignment operator.
				Copy the data and the boundaries.
		*/
		const TRegularData1D& operator = (const TRegularData1D& data)
			throw();

		/**	Assignment from a {\tt vector} of {\tt T}.
				Copy the contents of the data without changing the boundaries.
		*/
		const TRegularData1D& operator = (const VectorType& data)
			throw();

		//@}
		/**	@name Predicates
		*/
		//@{

		/**	Equality operator
		*/
		bool operator == (const TRegularData1D& data) const
			throw();

		//@}
		/**	@name	Accessors
		*/
		//@{	
			
		/**	Constant random access operator.
				@exception IndexOverflow if {\tt index} is out of range
		*/	
		const T& operator [] (Position index) const
			throw(Exception::IndexOverflow);
			
		/**	Mutable random access operator.
				@exception IndexOverflow if {\tt index} is out of range
		*/	
		T& operator [] (Position index)
			throw(Exception::IndexOverflow);
			
		/**	Return the number of items 
		*/
		Size getSize() const
			throw();

		/**	Return the lower bound
		*/
		double getLowerBound() const
			throw();
		
		/**	Return the upper bound
		*/
		double getUpperBound() const
			throw();

		/**	Set the upper bound
		*/
		void setUpperBound(double upper)
			throw();

		/**	Set the lower bound
		*/
		void setLowerBound(double lower)
			throw();

		/**	Resize the data.
				If {\tt new_size} is larger than the current size, the data 
				{\tt vector} is extended to the new size and filled with default
				constructed items of type {\tt T}. Resizing to a value lesser than
				the current size truncates the vector. \\
				The boundaries are adapted and the positions of the retained items
				fixed, i.e.  the upper bound is increased or decreased
				proportionally while the lower bound remains unchanged.
				@param new_size the new size
		*/
		void resize(Size new_size)
			throw();

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
		throw()
	{
	}

	template <typename T>
	TRegularData1D<T>::~TRegularData1D()
		throw()
	{
	}

	template <typename T>
	TRegularData1D<T>::TRegularData1D(const TRegularData1D<T>& data)
		throw()
		:	lower_(data.lower_),
			upper_(data.upper_),
			data_(data.data_)
	{
	}

	template <typename T>
	void TRegularData1D<T>::clear()
		throw()
	{
		// iterate over the data and reset all values to their default
		// boundaries and vector size remain unchanged
		T default_value = T();
		VectorType::iterator it = data_.begin();
		for (; it != data_.end(); ++it)
		{
			*it = default_value;
		}
	}

	template <typename T>
	void TRegularData1D<T>::destroy()
		throw()
	{
		// clear the vector and the boundaries
		data_.clear();
		lower_ = 0.0;
		upper_ = 0.0;
	}

	template <typename T>
	const TRegularData1D<T>& TRegularData1D<T>::operator = (const TRegularData1D<T>& data)
		throw()
	{
		// copy all members...
		data_ = data.data_;
		lower_ = data.lower_;
		upper_ = data.upper_;
	}

	template <typename T>
	const TRegularData1D<T>& TRegularData1D<T>::operator = (const TRegularData1D<T>::VectorType& data)
		throw()
	{
		// Copy the data. The boundaries remain unchanged.
		data_ = data;
	}

	template <typename T>
	bool TRegularData1D<T>::operator == (const TRegularData1D<T>& data) const
		throw()
	{
		return (lower_ == data.lower_ &&
						upper_ == data.upper_ &&
						 data_ == data.data_    );
	}
	
	template <typename T>
	BALL_INLINE
	const T& TRegularData1D<T>::operator [] (Position index) const
		throw(Exception::IndexOverflow)
	{
		if (index >= data_.size())
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, index);
		}
		
		return data_[index];
	}	
	
	template <typename T>
	BALL_INLINE
	T& TRegularData1D<T>::operator [] (Position index)
		throw(Exception::IndexOverflow)
	{
		if (index >= data_.size())
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, index);
		}
		
		return data_[index];
	}	

	template <typename T>
	BALL_INLINE
	Size TRegularData1D<T>::getSize() const
		throw()
	{
		return data_.size();
	}
	
	template <typename T>
	BALL_INLINE
	double TRegularData1D<T>::getLowerBound() const
		throw()
	{
		return lower_;
	}
	
	template <typename T>
	BALL_INLINE
	double TRegularData1D<T>::getUpperBound() const
		throw()
	{
		return upper_;
	}

	template <typename T>
	BALL_INLINE
	void TRegularData1D<T>::setLowerBound(double lower)
		throw()
	{
		lower_ = lower;
	}
	
	template <typename T>
	BALL_INLINE
	void TRegularData1D<T>::setUpperBound(double upper)
		throw()
	{
		upper_ = upper;
	}
	
	template <typename T>
	void TRegularData1D<T>::resize(Size new_size)
		throw()
	{
		if (data_.size() > 0)
		{
			upper_ *= (double)new_size / (double)data_.size();
		}
		data_.resize(new_size);
	}
		
} // namespace BALL

#endif // BALL_DATATYPE_REGULARDATA1D_H

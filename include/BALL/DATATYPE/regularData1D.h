// $Id: regularData1D.h,v 1.15 2001/07/09 22:06:28 amoll Exp $

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

		/**	Set the boundaries.
				If the values for upper and lower boundary dont match, the
				values are swapped.
		*/
		void setBoundaries(double lower, double upper)	
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

		/**	Rescale the data.
				Keep the current boundaries of the data and reinterpolate
				the data to reflect the new size. To create a data set of {\tt new_size}
				data points, the data is interpolated linearly at the new data points.
				
				@param new_size the new data set size
		*/
		void rescale(Size new_size)
			throw();

		/**	Rescale and change the boundaries.
				{\tt lower} and {\tt upper} define the new boundaries, while {\tt new_size}
				gives the number of new data points. The existing data is cut/extended to the
				new boundaries and the resulting new data points ({\tt new_size} data
				points in total) are obtained by linear interpolation of the current data.
				if the boundaries are extended, the resulting excess data points are initialized
				with zero.

				@param new_size the new size of the data set
				@param lower the lower boundary
				@param upper the upper boundary
		*/
		void rescale(double lower, double upper, Size new_size)
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
		: lower_(T()),
		  upper_(T()),
			data_(VectorType())
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
		typename VectorType::iterator it = data_.begin();
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

		return *this;
	}

	template <typename T>
	const TRegularData1D<T>& TRegularData1D<T>::operator = (const TRegularData1D<T>::VectorType& data)
		throw()
	{
		// Copy the data. The boundaries remain unchanged.
		data_ = data;
		
		return *this;
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
	void TRegularData1D<T>::setBoundaries(double lower, double upper)
		throw()
	{
		if (lower > upper)
		{
			double temp(lower);
			lower = upper;
			upper = temp;
		}
		
		lower_ = lower;
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
		

	template <typename T>
	void TRegularData1D<T>::rescale(Size new_size)
		throw()
	{
		// if the new and the old size coincide: done.
		if (new_size == data_.size())
		{
			return;
		}

		// if the data set is empty...
		if (data_.size() == 0)
		{
			// ...there's nothing to do: a resize was requested
			data_.resize(new_size);
			return;
		}
		
		// if the data set contains only a single value,
		// we fill everything with this value
		if ((data_.size() == 1) && (new_size > 1))
		{
			T old_value = data_[0];
			data_.resize(new_size);
			for (Position i = 1; i < new_size; i++)
			{
				data_[i] = old_value;
			}

			return;
		}

		// that's the default case: use linear interpolation
		// to determine the values at the new positions
		VectorType new_data(new_size);
		double factor1 = (double)data_.size() / (double)new_size;
		double factor2 = (double)(data_.size() - 1) / (new_size - 1);
		for (Size i = 0; i < new_size; i++)
		{
			// determine the interval of the old data set we are currently in
			// ([old_idx, old_idx + 1])
			Position old_idx = (Position)(i * factor1);
			double factor3 = (double)i * factor2 - (double)old_idx;
			new_data[i] = data_[old_idx] * (1 - factor3) + factor3 * data_[old_idx + 1];
		}

		// assign the new data
		data_ = new_data;
	}

	template <typename T>
	void TRegularData1D<T>::rescale(double lower, double upper, Size new_size)
		throw()
	{
		// if the data set is empty...
		if (data_.size() == 0)
		{
			// ...there's nothing to do: a resize was requested
			data_.resize(new_size);
			return;
		}
		
		if (lower > upper)
		{
			swap(lower, upper);
		}
		
		// if the new boundaries and the old boundaries do not overlap,
		// we create a new (empty) data set
		if ((lower_ > upper) || (upper_ < lower))
		{
			data_.resize(new_size);
			static T default_value =(T)0;
			for (Position i = 0; i < new_size; i++)
			{
				data_[i] = default_value;
			}

			return;
		}

		// that's the default case: use linear interpolation
		// to determine the values at the new positions

		// first, we create a new vector of appropriate size
		VectorType new_data(new_size);
		double s = (upper_ - lower_) / ((double)data_.size() - 1);
		for (Size i = 0; i < new_size; i++)
		{
			double new_x = lower + (double)i / (new_size - 1) * (upper - lower);
			if ((new_x >= lower_) && (new_x <= upper_))
			{
				Position old_idx = (Position)((new_x - lower_) / s);
				double factor3 = ((new_x - lower_) -  (double)old_idx * s) / s;
				new_data[i] = data_[old_idx] * (1 - factor3) + factor3 * data_[old_idx + 1];
			}
		}

		// assign the new data and the boundaries
		data_ = new_data;
		lower_ = lower;
		upper_ = upper;
	}


} // namespace BALL

#endif // BALL_DATATYPE_REGULARDATA1D_H

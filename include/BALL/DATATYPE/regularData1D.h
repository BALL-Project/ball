// $Id: regularData1D.h,v 1.1 2000/09/12 21:22:10 oliver Exp $

#ifndef BALL_DATATYPE_REGULARDATA1D_H
#define BALL_DATATYPE_REGULARDATA1D_H

namespace BALL
{

	/**	A class to store regularaly spaced data.
			This class can is intended to hold regularly spaced, one-dimensional data sets.
			It might be useful to hold data sets like spectra, or precomputed funciton values.
			\\
			{\bf Definition:} \URL{BALL/DATATYPE/regularData1D.h}
	*/
	template <typename T>
	class TRegularData1D
	{
		public:
			
		BALL_CREATE(RegularData1D<T>)

		/**	@name Type definitions
		*/
		//@{
		/**	The vector type.
				This type is used to store the data.
		*/
		typedef vector<T>	VectorType;

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
		const TRegularData1D& operator = (const DataType& data);
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
			
		/**
				@exception IndexOverflow if {\tt index} is out of range
		*/	
		const T& operator [] (Position index) const;
			
		/**
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
		double		lower_;
		double		upper_;
		DataType	data_;
	};

	/**	Default type
	*/
	typedef TRegularData1D<float> RegularData1D;

		
	template <typename T>
	TRegularData1D::TRegularData1D()
	{
	}

	template <typename T>
	TRegularData1D::~TRegularData1D()
	{
	}

	template <typename T>
	TRegularData1D::TRegularData1D(const TRegularData1D<T>& data)
		:	lower_(data.lower_),
			upper_(data.upper_),
			data_(data.data_)
	{
	}

	template <typename T>
	void TRegularData1D::clear()
	{
		// iterate over the data and reset all values to their default
		// boundaries and vector size remain unchanged
		T default;
		DataType::iterator it = data_.begin();
		for (; it != data_.end(); ++it)
		{
			*it = default;
		}
	}

	template <typename T>
	void TRegularData1D::destroy()
	{
		// clear the vector and the boundaries
		data_.clear();
		lower_ = 0.0;
		upper_ = 0.0;
	}

	template <typename T>
	const TRegularData1D<T>& TRegularData1D::operator = (const TRegularData1D<T>& const data)
	{
		// copy all members...
		data_ = data.data_;
		lower_ = data.lower_;
		upper_ = data.upper_;
	}

	template <typename T>
	const TRegularData1D<T>& TRegularData1D::operator = (const TRegularData1D<T>::DataType& const data)
	{
		// Copy the data. The boundaries remain unchanged.
		data_ = data;
	}

	template <typename T>
	bool TRegularData1D::operator == (const TRegularData1D<T>& data)
	{
		return ((lower_ == data.lower_) 
						&& (upper_ == data.upper_)
						&& (data_ == data.data_));
	}
	
	BALL_INLINE
	template <typename T>
	const T& TRegularData1D::operator [] (Position index) const
	{
		if (index >= data_.size)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, index);
		}
		
		return data[index];
	}	
	
	BALL_INLINE
	template <typename T>
	T& TRegularData1D::operator [] (Position index)
	{
		if (index >= data_.size)
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, index);
		}
		
		return data[index];
	}	

	BALL_INLINE
	template <typename T>
	double TRegularData1D::getSize() const
	{
		return data_.size();
	}
	
	BALL_INLINE
	template <typename T>
	double TRegularData1D::getLower() const
	{
		return lower_;
	}
	
	BALL_INLINE
	template <typename T>
	double TRegularData1D::getUpper() const
	{
		return upper_;
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
		
}

#endif // BALL_DATATYPE_REGULARDATA1D_H

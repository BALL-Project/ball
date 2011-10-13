#ifndef BALL_SYSTEM_BINARYFILEADAPTOR_H
#define BALL_SYSTEM_BINARYFILEADAPTOR_H

#include <iostream>
#include <algorithm>

#ifndef BALL_COMMON_LOGSTREAM_H
# include <BALL/COMMON/logStream.h>
#endif

namespace BALL
{
	/** Coping with endianness. This function swaps the bytes of a variable
			of type T if this type is of size 2n.
	*/
	template <typename T>
	void swapBytes(T& t)
	{
		if (sizeof(T) % 2 != 0)
		{
			Log.error() << "Cannot swap types of uneven size." << std::endl;
			return;
		}

		char* tmp = reinterpret_cast<char*>(&t);
		std::reverse(tmp, tmp + sizeof(T));
	}

	//In the following some specialisations of swapBytes are provided for efficiency reasons
	//These should also cover BALL types like Size, Position and Index
	template<> BALL_EXPORT void swapBytes(unsigned short&);
	template<> BALL_EXPORT void swapBytes(short&);
	template<> BALL_EXPORT void swapBytes(unsigned int&);
	template<> BALL_EXPORT void swapBytes(int&);
	template<> BALL_EXPORT void swapBytes(unsigned long&);
	template<> BALL_EXPORT void swapBytes(long&);
	template<> BALL_EXPORT void swapBytes(float&);
	template<> BALL_EXPORT void swapBytes(double&);

	/**
	 * Helper class for data conversion.
	 * BinaryFileAdaptors are used to read and write binary data from and to
	 * streams. This is done by reading the member <tt>data</tt> as a byte stream
	 * through an explicit cast and utilizing the stream read() and write()
	 * functions. \par
	 * <b>Caveat:</b> This concept relies on the C++ memory layout and thus
	 * is highly non-portable!
	 * \par
	 * The flag swap_endian can be used to swap between big- and little-endian.
	 * \ingroup System
	*/
	template <typename T>
	class BinaryFileAdaptor
	{

		public:

		/// @name Constructors and destructor
		//@{

		/// Default constructor
		BinaryFileAdaptor();

		/// Detailed constructor
		BinaryFileAdaptor(const T& data, bool swap_endian = false);

		//@}
		///@name Accessors
		//@{

		/// Set the swap_endian flag
		void setSwapEndian(bool swap_endian);

		/// return the swap_endian flag
		bool getSwapEndian() const;

		/** Sets the member <tt>data</tt> to the desired value.
				@param data data of type T
		*/
		void setData(const T& data);

		/** Returns a const reference to the data stored in the adaptor
		*/
		const T& getData() const;

		/** Returns a mutable reference to the data stored in the adaptor
				*/
		T& getData();

		//@}

		protected:

		//_ The member data.
		T data_;

		//_ A flag indicating whether we should swap all reads and writes
		bool swap_endian_;
	};

	template <typename T>
	BinaryFileAdaptor<T>::BinaryFileAdaptor()
		: data_(),
		  swap_endian_(false)
	{
	}

	template <typename T>
	BinaryFileAdaptor<T>::BinaryFileAdaptor(const T& data, bool swap_endian)
		: data_(data),
		  swap_endian_(swap_endian)
	{
	}

	template <typename T>
	void BinaryFileAdaptor<T>::setSwapEndian(bool swap_endian)
	{
		swap_endian_ = swap_endian;
	}

	template <typename T>
	bool BinaryFileAdaptor<T>::getSwapEndian() const
	{
		return swap_endian_;
	}

	template <typename T>
	void BinaryFileAdaptor<T>::setData(const T& data)
	{
		data_ = data;
	}

	template <typename T>
	const T& BinaryFileAdaptor<T>::getData() const
	{
		return data_;
	}

	template <typename T>
	T& BinaryFileAdaptor<T>::getData()
	{
		return data_;
	}

	/// Output stream for BinaryFileAdaptors
	template <typename T>
	std::ostream& operator << (std::ostream& os, const BinaryFileAdaptor<T>& data)
	{
		// do we need to swap endianness?
		if (!data.getSwapEndian())
		{
			os.write(reinterpret_cast<const char*>(&data.getData()), sizeof(T));
		}
		else
		{
			T swapped_data = data.getData();
			swapBytes(swapped_data);
			os.write(reinterpret_cast<const char*>(&swapped_data), sizeof(T));
		}
		return os;
	}

	/// Input stream for BinaryFileAdaptors
	template <typename T>
	std::istream& operator >> (std::istream& is, BinaryFileAdaptor<T>& data)
	{
		// do we need to swap endianness?
		if (!data.getSwapEndian())
		{
			is.read(reinterpret_cast<char*>(&data.getData()), sizeof(T));
		}
		else
		{
			T swapped_data;
			is.read(reinterpret_cast<char*>(&swapped_data), sizeof(T));
			swapBytes(swapped_data);
			data.setData(swapped_data);
		}
		return is;
	}

} //namespace BALL

#ifndef BALL_NO_INLINE_FUNCTIONS
	#include <BALL/SYSTEM/binaryFileAdaptor.iC>
#endif

#endif //BALL_SYSTEM_BINARYFILEADAPTOR_H

#ifndef BALL_SYSTEM_BINARYFILEADAPTOR_H
#define BALL_SYSTEM_BINARYFILEADAPTOR_H

#include <iostream>

namespace BALL
{
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
	BALL_INLINE BinaryFileAdaptor<T>::BinaryFileAdaptor()
		: data_(),
		  swap_endian_(false)
	{
	}

	template <typename T>
	BALL_INLINE BinaryFileAdaptor<T>::BinaryFileAdaptor(const T& data, bool swap_endian)
		: data_(data),
		  swap_endian_(swap_endian)
	{
	}

	template <typename T>
	BALL_INLINE void BinaryFileAdaptor<T>::setSwapEndian(bool swap_endian)
	{
		swap_endian_ = swap_endian;
	}

	template <typename T>
	BALL_INLINE bool BinaryFileAdaptor<T>::getSwapEndian() const
	{
		return swap_endian_;
	}

	template <typename T>
	BALL_INLINE void BinaryFileAdaptor<T>::setData(const T& data)
	{
		data_ = data;
	}

	template <typename T>
	BALL_INLINE const T& BinaryFileAdaptor<T>::getData() const
	{
		return data_;
	}

	template <typename T>
	BALL_INLINE T& BinaryFileAdaptor<T>::getData()
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

	/** Coping with endianness. This function swaps the bytes of a variable
			of type T if this type is of size 2n.
	*/
	template <typename T>
	BALL_INLINE void swapBytes(T& t)
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
#define BALL_SWAP_BYTES_16(x)\
	x = ((x >> 8)  & 0x00FF) | ((x << 8)  & 0xFF00)

#define BALL_SWAP_BYTES_32(x)\
	x = ((x >> 8)  & 0x00FF00FF) | ((x << 8)  & 0xFF00FF00);\
	x = ((x >> 16) & 0x0000FFFF) | ((x << 16) & 0xFFFF0000)

#define BALL_SWAP_BYTES_64(x)\
	x = ((x >> 8)  & 0x00FF00FF00FF00FF) | ((x << 8)  & 0xFF00FF00FF00FF00);\
	x = ((x >> 16) & 0x0000FFFF0000FFFF) | ((x << 16) & 0xFFFF0000FFFF0000);\
	x = ((x >> 32) & 0x00000000FFFFFFFF) | ((x << 32) & 0xFFFFFFFF00000000)

	template <>
	BALL_INLINE void swapBytes(unsigned short& t)
	{
		BALL_SWAP_BYTES_16(t);
	}

	template <>
	BALL_INLINE void swapBytes(short& t)
	{
		BALL_SWAP_BYTES_16(t);
	}

	template <>
	BALL_INLINE void swapBytes(uint32_t& t)
	{
		BALL_SWAP_BYTES_32(t);
	}

	template <>
	BALL_INLINE void swapBytes(int32_t& t)
	{
		BALL_SWAP_BYTES_32(t);
	}

	template <>
	BALL_INLINE void swapBytes(uint64_t& t)
	{
		BALL_SWAP_BYTES_64(t);
	}

	namespace __private
	{
		//We need these unions to provide a safe cast (without violating strict aliasing)
		//from float/double to a bitfield
		union U32
		{
			U32(float f_) : f(f_) { }

			uint32_t u;
			float f;
		};

		union U64
		{
			U64(double f_) : f(f_) { }

			uint64_t u;
			double f;
		};
	}

	template <>
	BALL_INLINE void swapBytes(float& f)
	{
		__private::U32 u(f);
		BALL_SWAP_BYTES_32(u.u);
		f = u.f;
	}

	template <>
	BALL_INLINE void swapBytes(double& f)
	{
		__private::U64 u(f);
		BALL_SWAP_BYTES_64(u.u);
		f = u.f;
	}

} //namespace BALL

#endif //BALL_SYSTEM_BINARYFILEADAPTOR_H

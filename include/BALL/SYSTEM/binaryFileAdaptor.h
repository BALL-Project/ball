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
#if   BALL_USHORT_SIZE == 2
			BALL_SWAP_BYTES_16(t);
#elif BALL_USHORT_SIZE == 4
			BALL_SWAP_BYTES_32(t);
#elif BALL_USHORT_SIZE == 8
			BALL_SWAP_BYTES_64(t);
#else
	#error "Unknown unsigned short size, refusing to compile."
#endif
	}

	template <>
	BALL_INLINE void swapBytes(short& t)
	{
#if   BALL_SHORT_SIZE == 2
			BALL_SWAP_BYTES_16(t);
#elif BALL_SHORT_SIZE == 4
			BALL_SWAP_BYTES_32(t);
#elif BALL_SHORT_SIZE == 8
			BALL_SWAP_BYTES_64(t);
#else
	#error "Unknown short size, refusing to compile."
#endif
	}

	template <>
	BALL_INLINE void swapBytes(unsigned int& t)
	{
#if   BALL_UINT_SIZE == 2
			BALL_SWAP_BYTES_16(t);
#elif BALL_UINT_SIZE == 4
			BALL_SWAP_BYTES_32(t);
#elif BALL_UINT_SIZE == 8
			BALL_SWAP_BYTES_64(t);
#else
	#error "Unknown unsigned int size, refusing to compile."
#endif
	}

	template <>
	BALL_INLINE void swapBytes(int& t)
	{
#if   BALL_INT_SIZE == 2
			BALL_SWAP_BYTES_16(t);
#elif BALL_INT_SIZE == 4
			BALL_SWAP_BYTES_32(t);
#elif BALL_INT_SIZE == 8
			BALL_SWAP_BYTES_64(t);
#else
	#error "Unknown int size, refusing to compile."
#endif
	}

	template <>
	BALL_INLINE void swapBytes(long& t)
	{
#if   BALL_LONG_SIZE == 2
			BALL_SWAP_BYTES_16(t);
#elif BALL_LONG_SIZE == 4
			BALL_SWAP_BYTES_32(t);
#elif BALL_LONG_SIZE == 8
			BALL_SWAP_BYTES_64(t);
#else
	#error "Unknown long size, refusing to compile."
#endif
	}

	template <>
	BALL_INLINE void swapBytes(unsigned long& t)
	{
#if   BALL_ULONG_SIZE == 2
			BALL_SWAP_BYTES_16(t);
#elif BALL_ULONG_SIZE == 4
			BALL_SWAP_BYTES_32(t);
#elif BALL_ULONG_SIZE == 8
			BALL_SWAP_BYTES_64(t);
#else
	#error "Unknown unsigned long size, refusing to compile."
#endif
	}

	namespace __private
	{
		//We need these unions to provide a safe cast (without violating strict aliasing)
		//from float/double to a bitfield
		union UFloat
		{
			UFloat(float f_) : f(f_) { }

#if   BALL_FLOAT_SIZE == 2
			uint16_t u;
#elif BALL_FLOAT_SIZE == 4
			uint32_t u;
#elif BALL_FLOAT_SIZE == 8
			uint64_t u;
#else
	#error "Unknown double size, refusing to compile."
#endif
			float f;
		};

		union UDouble
		{
			UDouble(double f_) : f(f_) { }

#if   BALL_DOUBLE_SIZE == 2
			uint16_t u;
#elif BALL_DOUBLE_SIZE == 4
			uint32_t u;
#elif BALL_DOUBLE_SIZE == 8
			uint64_t u;
#else
	#error "Unknown double size, refusing to compile."
#endif
			double f;
		};
	}

	template <>
	BALL_INLINE void swapBytes(float& f)
	{
		__private::UFloat u(f);
#if   BALL_FLOAT_SIZE == 2
		BALL_SWAP_BYTES_16(u.u);
#elif BALL_FLOAT_SIZE == 4
		BALL_SWAP_BYTES_32(u.u);
#elif BALL_FLOAT_SIZE == 8
		BALL_SWAP_BYTES_64(u.u);
#else
	#error "Unknown float size, refusing to compile."
#endif
		f = u.f;
	}

	template <>
	BALL_INLINE void swapBytes(double& f)
	{
		__private::UDouble u(f);
#if   BALL_DOUBLE_SIZE == 2
		BALL_SWAP_BYTES_16(u.u);
#elif BALL_DOUBLE_SIZE == 4
		BALL_SWAP_BYTES_32(u.u);
#elif BALL_DOUBLE_SIZE == 8
		BALL_SWAP_BYTES_64(u.u);
#else
	#error "Unknown double size, refusing to compile."
#endif
		f = u.f;
	}

} //namespace BALL

#endif //BALL_SYSTEM_BINARYFILEADAPTOR_H

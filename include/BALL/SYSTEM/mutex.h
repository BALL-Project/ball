// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_SYSTEM_MUTEX_H
#define BALL_SYSTEM_MUTEX_H

#include <QtCore/qmutex.h>

// TEST
#define BALL_HAS_BOOST_THREAD

#ifdef BALL_HAS_BOOST_THREAD
#	include <boost/thread/mutex.hpp>
#endif

namespace BALL
{
	/** This class provides a very thin wrapper around different mutex types.
	 *  As default, BALL supports QMutex, but if it has been configured with boost::thread
	 *  support enabled, boost::mutex can also be used. Similarly, we can simply provide
	 *  instantiations using, e.g, Intel thread building blocks.
	 *
	 * 	A BALL-mutex needs to provide the functions void lock(), void unlock(), and bool tryLock().
	 */
	template <class MutexType>
	class TMutex
		: public MutexType
	{
		public:
			TMutex()
				: MutexType()
			{}
	};

#ifdef BALL_HAS_BOOST_THREAD

	// Boost-based mutexes only require a mapping of tryLock to try_lock.
	template <>
	class TMutex<boost::mutex>
		: public boost::mutex
	{
		public:
			TMutex()
				: boost::mutex()
			{}

			bool tryLock()
			{
				return try_lock();
			}
	};

// required for visual studio
#ifdef BALL_COMPILER_MSVC
	template class BALL_EXPORT TMutex<boost::mutex>;
#endif

#endif

// required for visual studio
#ifdef BALL_COMPILER_MSVC
	template class BALL_EXPORT TMutex<QMutex>;
#endif

	// TEST!
	// the standard mutex to use
//	typedef TMutex<QMutex> Mutex;
	typedef TMutex<boost::mutex> Mutex;
}

#endif // BALL_SYSTEM_MUTEX_H
// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_SYSTEM_MUTEX_H
#define BALL_SYSTEM_MUTEX_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QtCore/qmutex.h>

#ifdef BALL_HAS_BOOST_THREAD
#	include <boost/thread/mutex.hpp>
#endif

#undef BALL_USE_THREAD_CHECKER_API
#ifdef BALL_USE_THREAD_CHECKER_API
#	include <libittnotify.h>
#endif

#define BALL_DEFAULT_MUTEX_TYPE QMutex

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

			void lock()
			{
				return boost::mutex::lock();
			}

			bool tryLock()
			{
				return try_lock();
			}
	};

#endif


#ifdef BALL_USE_THREAD_CHECKER_API
	template <>
	class TMutex<QMutex>
		: public QMutex
	{
		public:
			TMutex()
				: QMutex()
			{
			}

			void lock()
			{
				__itt_notify_sync_prepare((void *)this);
				QMutex::lock();
				__itt_notify_sync_acquired((void*)this);
			}

			void unlock()
			{
				__itt_notify_sync_releasing((void *)this);
				QMutex::unlock();
			}

			bool tryLock()
			{
				__itt_notify_sync_prepare((void*)this);
				bool result = QMutex::tryLock();

				if (result)
					__itt_notify_sync_acquired((void*)this);
				else
					__itt_notify_sync_cancel((void*)this);

				return result;
			}
	};
#else
// required for visual studio
#ifdef BALL_COMPILER_MSVC
	template class BALL_EXPORT TMutex<BALL_DEFAULT_MUTEX_TYPE>;
#endif
#endif

	// the standard mutex to use
	typedef TMutex<BALL_DEFAULT_MUTEX_TYPE> Mutex;
}

#endif // BALL_SYSTEM_MUTEX_H
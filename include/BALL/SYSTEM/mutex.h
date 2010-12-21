// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_SYSTEM_MUTEX_H
#define BALL_SYSTEM_MUTEX_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#include <QtCore/QMutex>
#include <QtCore/QReadWriteLock>
#include <QtCore/QMutexLocker>
#include <QtCore/QReadLocker>
#include <QtCore/QWriteLocker>

#ifdef BALL_HAS_BOOST_THREAD
# include <boost/version.hpp>
# if BOOST_VERSION >= 103500
#	include <boost/thread/mutex.hpp>
# define BALL_HAS_BOOST_MUTEX
# endif
#endif

#undef BALL_USE_THREAD_CHECKER_API
#ifdef BALL_USE_THREAD_CHECKER_API
#	include <libittnotify.h>
#endif

#define BALL_DEFAULT_MUTEX_TYPE QMutex
#define BALL_DEFAULT_MUTEXLOCKER_TYPE QMutexLocker
#define BALL_DEFAULT_READWRITELOCK_TYPE QReadWriteLock
#define BALL_DEFAULT_READLOCKER_TYPE QReadLocker
#define BALL_DEFAULT_WRITELOCKER_TYPE QWriteLocker

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
			TMutex(bool is_recursive = false)
				: MutexType()
			{}
	};

	template <>
	class TMutex<QMutex>
		: public QMutex
	{
		public:
			TMutex(bool is_recursive = false)
				: QMutex( is_recursive ? Recursive : NonRecursive )
			{
			}
	};

	/** This class provides a very thin wrapper around different mutex locker types.
	 *  As default, BALL supports QMutexLocker, but it can be easily
	 *  extended to support the same functionality with classes from boost or Intel
	 *  Thread Building Blocks. 
	 *
	 * 	A BALL-mutex locker needs to provide a constructor which is called with
	 * 	a pointer to a corresponding TMutex. Upon construction, the mutex is
	 * 	automatically locked, and is automatically unlocked when the mutex runs
	 * 	out of scope.
	 */
	template <class MutexLockerType, class MutexType>
	class TMutexLocker
		: public MutexLockerType
	{
		public:
			TMutexLocker(MutexType* mutex)
				: MutexLockerType(mutex)
			{}
	};

	/** This class provides a very thin wrapper around different read-write mutex types.
	 *  As default, BALL supports QReadWriteLock, but if it has been configured with boost::thread
	 *  support enabled, boost::mutex can also be used. Similarly, we can simply provide
	 *  instantiations using, e.g, Intel thread building blocks.
	 *
	 * 	A BALL-mutex needs to provide the functions void lockForRead(), void lockForWrite(), 
	 * 	void unlock(), bool tryLockForRead(), and bool tryLockForWrite().
	 */
	template <class ReadWriteLockType>
	class TReadWriteLock
		: public ReadWriteLockType
	{
		public:
			TReadWriteLock(bool is_recursive = false)
				: ReadWriteLockType()
			{}
	};

	template <>
	class TReadWriteLock<QReadWriteLock>
		: public QReadWriteLock
	{
		public:
			TReadWriteLock(bool is_recursive = false)
// NOTE: Qt only supports non-recursive read write locks since version 4.4. Before that, they were always recursive.
#if (QT_VERSION >= QT_VERSION_CHECK(4, 4, 0))
				: QReadWriteLock( is_recursive ? Recursive : NonRecursive )
#else
				: QReadWriteLock()
#endif
			{
			}
	};

	/** This class provides a very thin wrapper around different mutex read-locker types.
	 *  As default, BALL supports QReadLocker, but it can be easily
	 *  extended to support the same functionality with classes from boost or Intel
	 *  Thread Building Blocks. 
	 *
	 * 	A BALL-mutex read-locker needs to provide a constructor which is called with
	 * 	a pointer to a corresponding TReadWriteLock. Upon construction, the mutex is
	 * 	automatically locked for read, and is automatically unlocked when the mutex runs
	 * 	out of scope.
	 */
	template <class ReadLockerType, class ReadWriteLockType>
	class TReadLocker
		: public ReadLockerType
	{
		public:
			TReadLocker(ReadWriteLockType* lock)
				: ReadLockerType(lock)
			{}
	};

	/** This class provides a very thin wrapper around different mutex write-locker types.
	 *  As default, BALL supports QWriteLocker, but it can be easily
	 *  extended to support the same functionality with classes from boost or Intel
	 *  Thread Building Blocks. 
	 *
	 * 	A BALL-mutex write-locker needs to provide a constructor which is called with
	 * 	a pointer to a corresponding TReadWriteLock. Upon construction, the mutex is
	 * 	automatically locked for write, and is automatically unlocked when the mutex runs
	 * 	out of scope.
	 */
	template <class WriteLockerType, class ReadWriteLockType>
	class TWriteLocker
		: public WriteLockerType
	{
		public:
			TWriteLocker(ReadWriteLockType* lock)
				: WriteLockerType(lock)
			{}
	};



#ifdef BALL_HAS_BOOST_MUTEX
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

	// TODO: similar instantiation for TReadWriteLock

#else
// required for visual studio
#ifdef BALL_COMPILER_MSVC
	//template class BALL_EXPORT TMutex<BALL_DEFAULT_MUTEX_TYPE>;
	//template class BALL_EXPORT TReadWriteLock<BALL_DEFAULT_READWRITELOCK_TYPE>;
	template class BALL_EXPORT TMutexLocker<BALL_DEFAULT_MUTEXLOCKER_TYPE, BALL_DEFAULT_MUTEX_TYPE>;
	template class BALL_EXPORT TReadLocker<BALL_DEFAULT_READLOCKER_TYPE, BALL_DEFAULT_READWRITELOCK_TYPE>;
	template class BALL_EXPORT TWriteLocker<BALL_DEFAULT_WRITELOCKER_TYPE, BALL_DEFAULT_READWRITELOCK_TYPE>;
#endif
#endif

	// the standard mutex to use
	typedef TMutex<BALL_DEFAULT_MUTEX_TYPE> Mutex;
	typedef TReadWriteLock<BALL_DEFAULT_READWRITELOCK_TYPE> ReadWriteLock;
	typedef TMutexLocker<BALL_DEFAULT_MUTEXLOCKER_TYPE, BALL_DEFAULT_MUTEX_TYPE> MutexLocker;
	typedef TReadLocker<BALL_DEFAULT_READLOCKER_TYPE, BALL_DEFAULT_READWRITELOCK_TYPE> ReadLocker;
	typedef TWriteLocker<BALL_DEFAULT_WRITELOCKER_TYPE, BALL_DEFAULT_READWRITELOCK_TYPE> WriteLocker;
}

#endif // BALL_SYSTEM_MUTEX_H


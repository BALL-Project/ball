// $Id: timeStamp.h,v 1.9 2000/12/09 21:28:00 amoll Exp $

#ifndef BALL_CONCEPT_TIMESTAMP_H
#define BALL_CONCEPT_TIMESTAMP_H

#ifndef BALL_CONFIG_CONFIG_H
#	include <BALL/CONFIG/config.h>
#endif

#ifndef BALL_COMMON_CREATE_H
#	include <BALL/COMMON/create.h>
#endif

#ifndef BALL_COMMON_DEBUG_H
#	include <BALL/COMMON/debug.h>
#endif

#include <iostream>

namespace BALL 
{

	class PersistenceManager;

	/**	Time class.
			Used to store a point of time.
			This class  provides a higher precision than \Ref{Time}
			(below seconds).
			\\
			{\bf Interface:} {\tt Storable}
			\\
			{\bf Definition:} \URL{BALL/CONCEPT/timeStamp.h}
			\\
	*/
	class PreciseTime
	{
		
		public:

		BALL_CREATE(PreciseTime)

		/**	@name Constructors and Destructors.
		*/
		//@{
		/**	Default constructor.
				Initialize with zero.
		*/
		PreciseTime()
			throw();
			
		/**	Copy constructor
		*/
		PreciseTime(const PreciseTime& time)
			throw();

		/**	Detailed constructor
		*/
		PreciseTime(long secs, long usecs)
			throw();
			
		/**	Destructor
		*/
		virtual ~PreciseTime()
			throw();
		//@}

		/**	@name Constants.
		*/
		//@{
		/**	Zero object.
		*/
		static const PreciseTime ZERO;
		//@}

		/**	@name Assignment
		*/
		//@{
		/** Assignment method
		*/
		void set(long secs, long usecs) 
			throw();

		/** Assignment method
		*/
		void set(const PreciseTime& time) 
			throw();

		/**	Assignment operator
		*/
		const PreciseTime& operator = (const PreciseTime& time) 
			throw();

		/**	Clear method
		*/
		virtual void clear() 
			throw();
		//@}

		/**	@name Predicates
		*/
		//@{
		/**	Greater than operator.
		*/
		bool operator < (const PreciseTime& time) const 
			throw();

		/**	Lesser than operator.
		*/
		bool operator > (const PreciseTime& time) const 
			throw();

		/**	Equality operator.
		*/
		bool operator == (const PreciseTime& time) const 
			throw();
		//@}
		
		/**	@name Accessors 
		*/
		//@{

		/**	Return the seconds since Jan. 1, 1970.
		*/
		long getSeconds() const 
			throw();

		/**	Return the microseconds.
		*/
		long getMicroSeconds() const 
			throw();

		/**	Return the current time.
				@return PreciseTime the current time in seconds since Jan. 1, 1970
		*/
		static PreciseTime now() 
			throw();
		//@}
    /** @name Storable interface.
    */
    //@{

    /** Persistent stream writing.
        This method writes the contents of the \Ref{PreciseTime} objects to the
        persistent stream using the {\tt writePrimitive} method
        of the PersistenceManager.
        @param pm the persistence manager
    */
    void write(PersistenceManager& pm) const
			throw();

    /** Persistent stream reading.
        This method reads the contents of a \Ref{PreciseTime} object from the
        persistent stream using the {\tt readPrimitive} method
        of the PersistenceManager.
        @param pm the persistence manager
    */
    bool read(PersistenceManager& pm)
			throw();
		//@}
 
		protected:

		long secs_;
		long usecs_;
	};

	/**	Time stamp concept.
			
			This class implements a so-caleld time stamp. It is used to 
			store modification or creation times of objects.
	
			{\bf Interface:} {\tt Storable}
			\\
			{\bf Definition:} \URL{BALL/CONCEPT/timeStamp.h}
			\\
	*/
	class TimeStamp
	{
		public:

		BALL_CREATE(TimeStamp)
		/**	@name	Constructors and Destructors
		*/
		//@{

		/** Default constructor
		*/
		TimeStamp()
			throw();

		/** Destructor
		*/
		virtual ~TimeStamp()
			throw();
		//@}

		/**	@name	Predicates
		*/
		//@{

		/**	Check the time stamp.
		*/
		bool isNewerThan(const PreciseTime& time) const 
			throw();
		
		/**	Check the time stamp.
		*/
		bool isOlderThan(const PreciseTime& time) const 
			throw();

		/**	Check the time stamp.
		*/
		bool isNewerThan(const TimeStamp& stamp) const 
			throw();
		
		/**	Check the time stamp.
		*/
		bool isOlderThan(const TimeStamp& stamp) const 
			throw();

		/** Equality operator
		*/
		bool operator == (const TimeStamp& stamp) const 
			throw();
		
		//@}

		/**	@name Accessors
		*/	
		//@{
		/**	Update the time stamp.
				Store the value of {\tt time} in the internal time stamp.
				If {\tt time} is 0, use the current time (as given by
				\Ref{PreciseTime::now}).
				@param time the new time stamp (default = \Ref{PreciseTime::now})														
		*/
		virtual void stamp(const PreciseTime& time = PreciseTime::ZERO) 
			throw();

		/**	Return the time of last modification
				@return the time stamp
		*/
		const PreciseTime& getTime() const 
			throw();
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assignment operator
		*/
		const PreciseTime& operator = (const PreciseTime& time) 
			throw();

		/**	Clear method
		*/
		virtual void clear() 
			throw();

		//@}

    /** @name Storable interface.
    */
    //@{

    /** Persistent stream writing.
        This method writes the contents of the \Ref{TimeStamp} objects to the
        persistent stream using the {\tt writePrimitive} method
        of the PersistenceManager.
        @param pm the persistence manager
    */
    void write(PersistenceManager& pm) const
			throw();

    /** Persistent stream reading.
        This method reads the contents of a \Ref{TimeStamp} object from the
        persistent stream using the {\tt readPrimitive} method
        of the PersistenceManager.
        @param pm the persistence manager
    */
    bool read(PersistenceManager& pm)
			throw();
		//@}

		protected:

		/**	The time stamp.
		*/
		PreciseTime time_;
	};

	/**	Global stream operators for PreciseTime and TimeStamp
	*/
	//@{
	/**	Print the contents of a PreciseTime object to a stream.
	*/
	std::ostream& operator << (std::ostream& os, const PreciseTime& time)
		throw();

	/**	Print the contents of a TimeStamp object to a stream.
	*/
	std::ostream& operator << (std::ostream& os, const TimeStamp& stamp)
		throw();
	//@}


#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/timeStamp.iC>
#	endif

} // namespace BALL

#endif // BALL_CONCEPT_TIMESTAMP_H

// $Id: timeStamp.h,v 1.5 2000/10/05 22:27:45 oliver Exp $

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
			\\
			{\bf Interface:} {\tt Storable}
			\\
			{\bf Definition:} \URL{BALL/CONCEPT/timeStamp.h}
			\\
	*/
	class Time
	{
		
		public:
		BALL_CREATE(Time)

		/**	@name Constructors and Destructors.
		*/
		//@{
		/**	Default constructor.
				Initialize with zero.
		*/
		Time();
			
		/**	Copy constructor
		*/
		Time(const Time& time);
			
		/**	Destructor
		*/
		virtual ~Time();
		//@}

		/**	@name Constants.
		*/
		//@{
		/**	Zero object.
		*/
		static const Time ZERO;
		//@}

		/**	@name Assignment
		*/
		//@{
		/** 
		*/
		void set(long secs, long usecs) throw();

		/**
		*/
		void set(const Time& time) throw();

		/**	Assignment operator
		*/
		const Time& operator = (const Time& time) throw();
		//@}

		/**	@name Predicates
		*/
		//@{
		/**	Greater than operator.
		*/
		bool operator < (const Time& time) const throw();

		/**	Lesser than operator.
		*/
		bool operator > (const Time& time) const throw();

		/**	Equality operator.
		*/
		bool operator == (const Time& time) const throw();
		//@}
		
		/**	@name Accessors 
		*/
		//@{

		/**	Return the seconds since Jan. 1, 1970.
		*/
		long getSeconds() const throw();

		/**	Return the microseconds.
		*/
		long getMicroSeconds() const throw();

		/**	Return the current time.
				@return Time the current time in seconds since Jan. 1, 1970
		*/
		static const Time& now() throw();
		//@}
    /** @name Storable interface.
    */
    //@{

    /** Persistent stream writing.
        This method writes the contents of the \Ref{Time} objects to the
        persistent stream using the {\tt writePrimitive} method
        of the PersistenceManager.
        @param pm the persistence manager
    */
    void write(PersistenceManager& pm) const;

    /** Persistent stream reading.
        This method reads the contents of a \Ref{Time} object from the
        persistent stream using the {\tt readPrimitive} method
        of the PersistenceManager.
        @param pm the persistence manager
    */
    bool read(PersistenceManager& pm);
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
		TimeStamp();

		/** Destructor
		*/
		virtual ~TimeStamp();
		//@}

		/**	@name	Predicates
		*/
		//@{

		/**	Check the time stamp.
		*/
		bool isNewerThan(const Time& time) const throw ();
		
		/**	Check the time stamp.
		*/
		bool isOlderThan(const Time& time) const throw ();

		/**	Check the time stamp.
		*/
		bool isNewerThan(const TimeStamp& stamp) const throw ();
		
		/**	Check the time stamp.
		*/
		bool isOlderThan(const TimeStamp& stamp) const throw ();
		//@}

		/**	@name Accessors
		*/	
		//@{
		/**	Update the time stamp.
				Store the value of {\tt time} in the internal time stamp.
				If {\tt time} is 0, use the current time (as given by
				\Ref{Time::now}).
				@param time the new time stamp (default = \Ref{Time::now})														
		*/
		virtual void stamp(const Time& time = Time::ZERO) throw ();

		/**	Return the time of last modification
				@return Time the time stamp
		*/
		const Time& getTime() const throw ();
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
    void write(PersistenceManager& pm) const;

    /** Persistent stream reading.
        This method reads the contents of a \Ref{TimeStamp} object from the
        persistent stream using the {\tt readPrimitive} method
        of the PersistenceManager.
        @param pm the persistence manager
    */
    bool read(PersistenceManager& pm);
		//@}

		protected:

		/**	The time stamp.
		*/
		Time time_;
	};

	/**	Global stream operators for Time and TimeStamp
	*/
	//@{
	/**	Print the contents of a Time object to a stream.
	*/
	std::ostream& operator << (std::ostream& os, const Time& time);

	/**	Print the contents of a TimeStamp object to a stream.
	*/
	std::ostream& operator << (std::ostream& os, const TimeStamp& stamp);
	//@}


#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/timeStamp.iC>
#	endif

} // namespace BALL

#endif // BALL_CONCEPT_TIMESTAMP_H

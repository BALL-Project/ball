// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

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

#ifndef BALL_CONCEPT_PERSISTENCEMANAGER_H
# include <BALL/CONCEPT/persistenceManager.h>
#endif

#include <iostream>

namespace BALL 
{

	/**	Time class.
			Used to store a point of time.
			This class  provides a higher precision than  \link Time Time \endlink 
			(below seconds).  \par
			<b>Interface:</b> <tt>Storable</tt>	 \par
			
			\ingroup ConceptsMiscellaneous
	*/
	class BALL_EXPORT PreciseTime
	{

		public:

		BALL_CREATE(PreciseTime)

		/**	@name Constructors and Destructors.
		*/
		//@{

		/**	Default constructor.
				Initialize with zero.
		*/
		PreciseTime();
			
		/**	Copy constructor
		*/
		PreciseTime(const PreciseTime& time);

		/**	Detailed constructor
		*/
		PreciseTime(long secs, long usecs);
			
		/**	Destructor
		*/
		virtual ~PreciseTime();

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
		void set(long secs, long usecs);

		/** Assignment method
		*/
		void set(const PreciseTime& time);

		/**	Assignment operator
		*/
		const PreciseTime& operator = (const PreciseTime& time);

		/**	Clear method
		*/
		virtual void clear() 
			;

		//@}
		/**	@name Predicates
		*/
		//@{

		/**	Greater than operator.
		*/
		bool operator < (const PreciseTime& time) const;

		/**	Lesser than operator.
		*/
		bool operator > (const PreciseTime& time) const;

		/**	Equality operator.
		*/
		bool operator == (const PreciseTime& time) const;

		//@}
		/**	@name Accessors 
		*/
		//@{

		/**	Return the seconds since Jan. 1, 1970.
		*/
		long getSeconds() const;

		/**	Return the microseconds.
		*/
		long getMicroSeconds() const;

		/**	Return the current time.
				@return PreciseTime the current time in seconds since Jan. 1, 1970
		*/
		static PreciseTime now();

		//@}

    /** @name Storable interface.
    */
    //@{

    /** Persistent stream writing.
        This method writes the contents of the  \link PreciseTime PreciseTime \endlink  objects to the
        persistent stream using the <tt>writePrimitive</tt> method
        of the PersistenceManager.
        @param pm the persistence manager
    */
    void write(PersistenceManager& pm) const;

    /** Persistent stream reading.
        This method reads the contents of a  \link PreciseTime PreciseTime \endlink  object from the
        persistent stream using the <tt>readPrimitive</tt> method
        of the PersistenceManager.
        @param pm the persistence manager
    */
    bool read(PersistenceManager& pm);

		//@}
 
		protected:

		long secs_;
		long usecs_;

		#ifdef BALL_HAS_WINDOWS_PERFORMANCE_COUNTER
			static long ticks_;
		#endif
	};

	/**	Time stamp concept.
			This class implements a so-called time stamp. It is used to 
			store modification or creation times of objects.
			<b>Interface:</b> <tt>Storable</tt>	 \par
			
	*/
	class BALL_EXPORT TimeStamp
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
		bool isNewerThan(const PreciseTime& time) const;
		
		/**	Check the time stamp.
		*/
		bool isOlderThan(const PreciseTime& time) const;

		/**	Check the time stamp.
		*/
		bool isNewerThan(const TimeStamp& stamp) const;
		
		/**	Check the time stamp.
		*/
		bool isOlderThan(const TimeStamp& stamp) const;

		/** Equality operator
		*/
		bool operator == (const TimeStamp& stamp) const;
		
		//@}
		/**	@name Accessors
		*/	
		//@{

		/**	Update the time stamp.
				Store the value of <tt>time</tt> in the internal time stamp.
				If <tt>time</tt> is 0, use the current time (as given by
				 \link PreciseTime::now PreciseTime::now \endlink ).
				@param time the new time stamp (default =  \link PreciseTime::now PreciseTime::now \endlink )														
		*/
		virtual void stamp(const PreciseTime& time = PreciseTime::ZERO);

		/**	Return the time of last modification
				@return the time stamp
		*/
		const PreciseTime& getTime() const;

		//@}
		/**	@name	Assignment
		*/
		//@{

		/**	Assignment operator
		*/
		const PreciseTime& operator = (const PreciseTime& time);

		/**	Clear method
		*/
		virtual void clear();

		//@}
    /** @name Storable interface.
    */
    //@{

    /** Persistent stream writing.
        This method writes the contents of the  \link TimeStamp TimeStamp \endlink  objects to the
        persistent stream using the <tt>writePrimitive</tt> method
        of the PersistenceManager.
        @param pm the persistence manager
    */
    void write(PersistenceManager& pm) const;

    /** Persistent stream reading.
        This method reads the contents of a  \link TimeStamp TimeStamp \endlink  object from the
        persistent stream using the <tt>readPrimitive</tt> method
        of the PersistenceManager.
        @param pm the persistence manager
    */
    bool read(PersistenceManager& pm);

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
	BALL_EXPORT
	std::ostream& operator << (std::ostream& os, const PreciseTime& time);

	/**	Print the contents of a TimeStamp object to a stream.
	*/
	BALL_EXPORT
	std::ostream& operator << (std::ostream& os, const TimeStamp& stamp);

	//@}


#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/CONCEPT/timeStamp.iC>
#	endif

} // namespace BALL

#endif // BALL_CONCEPT_TIMESTAMP_H

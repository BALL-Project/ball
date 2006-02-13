// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MPISupport.h,v 1.1.2.1 2006/02/13 20:38:59 anhi Exp $
//

#ifndef BALL_SYSTEM_MPISUPPORT_H
#define BALL_SYSTEM_MPISUPPORT_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
# include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
# include <BALL/COMMON/exception.h>
#endif

#include <iostream>

// Workaround!
#include <mpi.h>
#define BALL_HAS_MPI2_SUPPORT true

namespace BALL
{
	class System;
	class Options;

	/** This class provides some of the most important MPI functions
	 *  to BALL classes.
	 */
	class BALL_EXPORT MPISupport
	{
		public:

			/** TAGS used for send and receive operations
			 */
			enum TAGS
			{
				TAG_SYSTEM,
				TAG_OPTIONS
			};

			/** Constructors and Destructors
			*/
			//@{

			/** Default constructor.
			 *  Does _not_ call MPI_Init.
			 *  default_communicator can be used to change the broadcasting
			 *  behaviour of the MPI calls. The default is MPI_COMM_WORLD.
			 */
			MPISupport(MPI_Comm default_communicator = MPI_COMM_WORLD)
				throw();

			/** Detailed constructor.
			 *  If MPI has not been initialized yet, argc and argv are
			 *  passed to MPI_Init. If MPI _has_ already been initialized,
			 *  we can't re-initialize and throw a Precondition Exception.
			 *  default_communicator can be used to change the broadcasting
			 *  behaviour of the MPI calls. The default is MPI_COMM_WORLD.
			 *
			 *  If we find a parent of this process, we have been spawned
			 *  somehow. In this case, we will by default use a communicator
			 *  containing the old communicator of our parent and all spawned
			 *  processes and none of the spawned processes will
			 *  become a new master process. This behaviour can be avoided
			 *  by setting accept_communicator to false
			 */
			MPISupport(int argc, char** argv, 
								 MPI_Comm default_communicator = MPI_COMM_WORLD, 
								 bool accept_communicator = true);
			
			/** Destructor.
			 *  By default, we call MPI_Finalize here. This can be avoided
			 *  by setting the finalize_on_destruct_ flag of the class to false
			 */
			~MPISupport();
			//@}

			/** Accessors.
			 */
			//@{
			
			/// Return the rank of this process.
			Index getRank()
				throw();

			/// Return the number of processes in MPI_COMM_WORLD
			Index getSize()
				throw();

			/// Return the default communicator used for MPI calls.
			MPI_Comm getDefaultCommunicator()
				throw();

			/// Set the default communicator used for MPI calls.
			void setDefaultCommunicator(MPI_Comm default_communicator)
				throw();

			/** Returns true if MPI_Finalize will be called in the destructor,
			 *  false otherwise.
			 */
			bool getFinalizeOnDestruct()
				throw();

			/// Decides whether MPI_Finalize will be called in the destructor
			void setFinalizeOnDestruct(bool new_value)
				throw();

			/** Set the internal MPI_Info object. This is currently only a workaround,
			 *  and the memory handling (MPI_Info_free) has to be performed by the
			 *  calling process.
			 */
			void setMpiInfo(const MPI_Info &mpi_info)
				throw();

			/// Returns true if this process is the master, false otherwise.
			bool isMaster()
				throw();

			/** Initialize MPI using argc and argv. If MPI_Init has already
				* been called, a BALL_PRECONDITION_EXCEPTION is thrown.
				* If we find a parent of this process, we have been spawned
				* somehow. In this case, we will by default use a communicator
				* containing the old communicator of our parent and all spawned
				* processes and none of the spawned processes will
				* become a new master process. This behaviour can be avoided
				* by setting accept_communicator to false
				*/
			void init(int argc, char** argv, bool accept_communicator = true);

			/** Set an MPI_Barrier, i.e., this function will only return after
			 *  all processes in the default_communicator_ have called this function
			 * 	(or have directly called MPI_Barrier). This can be used to syncronize
			 * 	the workflow.
			 */
			void setBarrier()
				throw();

			/** Send a system across the communicator.
			 *  This function relies on BALL's XDRPersistenceManagement.
			 *  If broadcast is true, all processes in default_communicator_ will receive the
			 *  system. Note that all of these processes _must_ call receiveSystem!
			 *  If broadcast is set to false, the message is sent to receiver only.
			 */
			void sendSystem(const System& system, bool broadcast = true, int receiver = 0)
				throw();

			/** Receive a system from the communicator.
			 *  If broadcast is true, we expect a broadcast, otherwise a send.
			 *  In case of directed send, the source can be given as well.
			 *  Note that we will have to ensure that the system is deleted ourselves.
			 */
			System* receiveSystem(bool broadcast = true, int source = MPI_ANY_SOURCE)
				throw();

			/** Send a BALL option class across the communicator.
			 *  This function relies on BALL's XDRPersistenceManagement.
			 *  If broadcast is true, all processes in default_communicator_ will receive the
			 *  options. Note that all of these processes _must_ call receiveOptions!
			 *  If broadcast is set to false, the message is sent to receiver only.
			 */
			void sendOptions(const Options& options, bool broadcast = true, int receiver = 0)
				throw();

			/** Receive a BALL option class from the communicator.
			 *  If broadcast is true, we expect a broadcast, otherwise a send.
			 *  In case of directed send, the source can be given as well.
			 *  Note that we will have to ensure that the Options are deleted ourselves.
			 */
			Options* receiveOptions(bool broadcast = true, int source = MPI_ANY_SOURCE)
				throw();

			/** Distribute input vector as evenly as possible over all
			 *  processes in the communicator. Stores the part of the datapoints
			 *  the sender itself has to process in our_share. 
			 *  Note that _all_ processes in the communicator have to call acceptDatapoints.
			 */
			template <typename valuetype>
			void distributeDatapoints(const std::vector<valuetype>& input, std::vector<valuetype>& our_share)
				throw(Exception::OutOfMemory);

			/** Accept datapoints that are distributed by some source in the
			 *  communicator. Stores the result in our_share.
			 */
			template <typename valuetype>
			void acceptDatapoints(std::vector<valuetype>& our_share)
				throw(Exception::OutOfMemory);

			/** Distribute input of type datatype as evenly as possible over the
			 *  processes in the communicator. Returns the number of datapoints
			 *  the sender itself has to process. The points themselves are stored
			 *  in our_data. Note that _all_ processes in the
			 *  communicator have to call acceptDatapoints.
			 *  The caller has to ensure that the returned array is free()'d.
			 *  If memory allocation fails, 0 is returned.
			 */
			Size distributeDatapoints(const void* input, int size, void* our_data, MPI_Datatype datatype)
				throw();

			/** Accept datapoints that are distributed by some source in the
			 *  communicator. The caller has to ensure that the array is free()'d.
			 *  If memory allocation fails, 0 is returned, otherwise the number of
			 *  points we need to process.
			 */
			Size acceptDatapoints(void* our_data, MPI_Datatype datatype)
				throw();
			
			/** Spawn new processes.
			 *  This function allows the current instance to spawn new processes
			 *  that can communicate via MPI. The spawned processes are assigned
			 *  a new MPI_COMM_WORLD communicator by MPI, excluding /this/ process.
			 *  We thus overwrite their default_communicator_, /and/ the default_communicator_
			 *  of /this/ process, with a communicator connecting all spawned processes
			 *  with all processes in the default_communicator_ of this one. 
			 *  More complex communicator handling (like keeping the existing 
			 *  default_communicator_ of /this/ process) is currently not supported
			 *  and can only be achieved by directly calling the corresponding MPI
			 *  routines.
			 *
			 *  Note:
			 *  	- this function relies on the MPI-2 standard. Thus, if only
			 *  	  MPI-1 is supported, this function will _not be compiled_
			 *  	- the executables that will be spawned do _not_ need
			 *  		to be identical with the executable of the spawning process
			 *  	- if the number of processes to spawn is set to zero, we will
			 *  	  try to estimate the maximum sensible number using MPI_UNIVERSE
			 *  	- currently, the use of MPI_Info objects to transmit information
			 *  	  like search paths or hostnames to the spawned processes is not
			 *  	  really supported. We merely pass an internal MPI_Info object
			 *  	  that is set to MPI_INFO_NULL by default to MPI_Spawn. This
			 *  	  object can be accessed from the outside using setMpiInfo(), but
			 *  	  the external caller has to take care of the memory handling for
			 *  	  this object
			 *
			 *  \param command The path to the executable to spawn.
			 *  \param argv The command line arguments for the executable
			 *  \param wanted_number_of_processes The maximum number of processes to spawn
			 *  \return The number of processes that were succesfully spawned 
		 	 */
#ifdef BALL_HAS_MPI2_SUPPORT
			Size spawn(const String& command, char *argv[], Size wanted_number_of_processes = 0)
				throw();
#endif
			
			/** Sum up the local_value of all processes. If this process is the
			 *  master, this function will return the result. For all non-master 
			 *  processes, the result is undefined.
			 *	This is implemented as a template function to encapsulate the
			 *  MPI_Datatype - handling.
		 	 */
			template <typename valuetype>
			valuetype getSum(valuetype& local_value)
				throw();
			
			/** Multiply the local_value of all processes. If this process is the
			 *  master, this function will return the result. For all non-master 
			 *  processes, the result is undefined.
			 *	This is implemented as a template function to encapsulate the
			 *  MPI_Datatype - handling.
		 	 */
			template <typename valuetype>
			valuetype getProduct(valuetype& local_value)
				throw();
			
			/** Determine the maximum of the local_values of all processes. 
			 *  If this process is the
			 *  master, this function will return the result. For all non-master 
			 *  processes, the result is undefined.
			 *	This is implemented as a template function to encapsulate the
			 *  MPI_Datatype - handling.
		 	 */
			template <typename valuetype>
			valuetype getMaximum(valuetype& local_value)
				throw();
			
			/** Determine the minimum of the local_values of all processes. 
			 *  If this process is the
			 *  master, this function will return the result. For all non-master 
			 *  processes, the result is undefined.
			 *	This is implemented as a template function to encapsulate the
			 *  MPI_Datatype - handling.
		 	 */
			template <typename valuetype>
			valuetype getMinimum(valuetype& local_value)
				throw();
			//@}
			
		protected:
			/** Register MPI_Datatypes used for sending BALL objects around.
			 */
			void registerTypes_()
				throw();

			/** Helper function for sending BALL - objects: sends a string
			 *  containing a persistence stream over the communicator.
			 */
			void sendPersistenceStream_(const std::ostringstream& stream, 
																	TAGS tag = MPI_ANY_TAG, bool broadcast = true, int receiver = 0)
				throw();
			
			/** Helper function for receiving BALL - objects: receives a string
			 *  containing a persistence stream from the communicator and stores
			 *  it in the istream
			 */
			void receivePersistenceStream_(std::istringstream& in, TAGS tag = MPI_ANY_TAG, 
										 							   bool broadcast = true, int source = 0)
				throw();	

			Index 		rank_;
			Index 		comm_size_;
			bool  		finalize_on_destruct_;
			MPI_Comm  default_communicator_;
			MPI_Info  mpi_info_object_;
			MPI_Datatype  mpi_Vector3_float_type_;
			MPI_Datatype  mpi_Vector3_double_type_;
	};

	template <>
	signed char MPISupport::getSum(signed char& local_value)
		throw()
	{
		signed char result;
		MPI_Reduce(&local_value, &result, 1, MPI_CHAR, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed short int MPISupport::getSum(signed short int& local_value)
		throw()
	{
		signed short int result;
		MPI_Reduce(&local_value, &result, 1, MPI_SHORT, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed int MPISupport::getSum(signed int& local_value)
		throw()
	{
		signed int result;
		MPI_Reduce(&local_value, &result, 1, MPI_INT, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed long int MPISupport::getSum(signed long int& local_value)
		throw()
	{
		signed long int result;
		MPI_Reduce(&local_value, &result, 1, MPI_LONG, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned char MPISupport::getSum(unsigned char& local_value)
		throw()
	{
		unsigned char result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_CHAR, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned short MPISupport::getSum(unsigned short& local_value)
		throw()
	{
		unsigned short result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_SHORT, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned int MPISupport::getSum(unsigned int& local_value)
		throw()
	{
		unsigned int result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned long int MPISupport::getSum(unsigned long int& local_value)
		throw()
	{
		unsigned long int result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	float MPISupport::getSum(float& local_value)
		throw()
	{
		float result;
		MPI_Reduce(&local_value, &result, 1, MPI_FLOAT, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	double MPISupport::getSum(double& local_value)
		throw()
	{
		double result;
		MPI_Reduce(&local_value, &result, 1, MPI_DOUBLE, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	long double MPISupport::getSum(long double& local_value)
		throw()
	{
		long double result;
		MPI_Reduce(&local_value, &result, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed char MPISupport::getProduct(signed char& local_value)
		throw()
	{
		signed char result;
		MPI_Reduce(&local_value, &result, 1, MPI_CHAR, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed short int MPISupport::getProduct(signed short int& local_value)
		throw()
	{
		signed short int result;
		MPI_Reduce(&local_value, &result, 1, MPI_SHORT, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed int MPISupport::getProduct(signed int& local_value)
		throw()
	{
		signed int result;
		MPI_Reduce(&local_value, &result, 1, MPI_INT, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed long int MPISupport::getProduct(signed long int& local_value)
		throw()
	{
		signed long int result;
		MPI_Reduce(&local_value, &result, 1, MPI_LONG, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned char MPISupport::getProduct(unsigned char& local_value)
		throw()
	{
		unsigned char result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_CHAR, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned short MPISupport::getProduct(unsigned short& local_value)
		throw()
	{
		unsigned short result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_SHORT, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned int MPISupport::getProduct(unsigned int& local_value)
		throw()
	{
		unsigned int result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned long int MPISupport::getProduct(unsigned long int& local_value)
		throw()
	{
		unsigned long int result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_LONG, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	float MPISupport::getProduct(float& local_value)
		throw()
	{
		float result;
		MPI_Reduce(&local_value, &result, 1, MPI_FLOAT, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	double MPISupport::getProduct(double& local_value)
		throw()
	{
		double result;
		MPI_Reduce(&local_value, &result, 1, MPI_DOUBLE, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	long double MPISupport::getProduct(long double& local_value)
		throw()
	{
		long double result;
		MPI_Reduce(&local_value, &result, 1, MPI_LONG_DOUBLE, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed char MPISupport::getMaximum(signed char& local_value)
		throw()
	{
		signed char result;
		MPI_Reduce(&local_value, &result, 1, MPI_CHAR, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed short int MPISupport::getMaximum(signed short int& local_value)
		throw()
	{
		signed short int result;
		MPI_Reduce(&local_value, &result, 1, MPI_SHORT, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed int MPISupport::getMaximum(signed int& local_value)
		throw()
	{
		signed int result;
		MPI_Reduce(&local_value, &result, 1, MPI_INT, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed long int MPISupport::getMaximum(signed long int& local_value)
		throw()
	{
		signed long int result;
		MPI_Reduce(&local_value, &result, 1, MPI_LONG, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned char MPISupport::getMaximum(unsigned char& local_value)
		throw()
	{
		unsigned char result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_CHAR, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned short MPISupport::getMaximum(unsigned short& local_value)
		throw()
	{
		unsigned short result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_SHORT, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned int MPISupport::getMaximum(unsigned int& local_value)
		throw()
	{
		unsigned int result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned long int MPISupport::getMaximum(unsigned long int& local_value)
		throw()
	{
		unsigned long int result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_LONG, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	float MPISupport::getMaximum(float& local_value)
		throw()
	{
		float result;
		MPI_Reduce(&local_value, &result, 1, MPI_FLOAT, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	double MPISupport::getMaximum(double& local_value)
		throw()
	{
		double result;
		MPI_Reduce(&local_value, &result, 1, MPI_DOUBLE, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	long double MPISupport::getMaximum(long double& local_value)
		throw()
	{
		long double result;
		MPI_Reduce(&local_value, &result, 1, MPI_LONG_DOUBLE, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed char MPISupport::getMinimum(signed char& local_value)
		throw()
	{
		signed char result;
		MPI_Reduce(&local_value, &result, 1, MPI_CHAR, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed short int MPISupport::getMinimum(signed short int& local_value)
		throw()
	{
		signed short int result;
		MPI_Reduce(&local_value, &result, 1, MPI_SHORT, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed int MPISupport::getMinimum(signed int& local_value)
		throw()
	{
		signed int result;
		MPI_Reduce(&local_value, &result, 1, MPI_INT, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed long int MPISupport::getMinimum(signed long int& local_value)
		throw()
	{
		signed long int result;
		MPI_Reduce(&local_value, &result, 1, MPI_LONG, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned char MPISupport::getMinimum(unsigned char& local_value)
		throw()
	{
		unsigned char result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_CHAR, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned short MPISupport::getMinimum(unsigned short& local_value)
		throw()
	{
		unsigned short result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_SHORT, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned int MPISupport::getMinimum(unsigned int& local_value)
		throw()
	{
		unsigned int result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned long int MPISupport::getMinimum(unsigned long int& local_value)
		throw()
	{
		unsigned long int result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_LONG, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	float MPISupport::getMinimum(float& local_value)
		throw()
	{
		float result;
		MPI_Reduce(&local_value, &result, 1, MPI_FLOAT, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	double MPISupport::getMinimum(double& local_value)
		throw()
	{
		double result;
		MPI_Reduce(&local_value, &result, 1, MPI_DOUBLE, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	long double MPISupport::getMinimum(long double& local_value)
		throw()
	{
		long double result;
		MPI_Reduce(&local_value, &result, 1, MPI_LONG_DOUBLE, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	void MPISupport::distributeDatapoints(const std::vector<TVector3<float> >& input,
																					    std::vector<TVector3<float> >& our_share)
		throw(Exception::OutOfMemory)
	{
		// We'll need to take care of the pointers passed around...
		float* our_data = 0;
		Size num_points = distributeDatapoints(&input[0], input.size(), our_data, mpi_Vector3_float_type_);

		if (our_data == 0)
			throw(Exception::OutOfMemory(__FILE__, __LINE__, num_points*3*sizeof(float)));

		// create the output vector
		our_share.resize(num_points);

		for (Size i=0; i<num_points; i++)
		{
			our_share[i].set(our_data[3*i], our_data[3*i+1], our_data[3*i+2]);
		}

		free(our_data);
	}

	template <>
	void MPISupport::acceptDatapoints(std::vector<TVector3<float> >& our_share)
		throw(Exception::OutOfMemory)
	{
		// We'll need to take care of the pointers passed around...
		float* our_data = 0;
		Size num_points = acceptDatapoints(our_data, mpi_Vector3_float_type_);

		if (our_data == 0)
			throw(Exception::OutOfMemory(__FILE__, __LINE__, num_points*3*sizeof(float)));

		// create the output vector
		our_share.resize(num_points);

		for (Size i=0; i<num_points; i++)
		{
			our_share[i].set(our_data[3*i], our_data[3*i+1], our_data[3*i+2]);
		}

		free(our_data);
	}
}
#endif

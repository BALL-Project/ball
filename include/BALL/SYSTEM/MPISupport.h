// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MPISupport.h,v 1.1.2.4 2006/03/02 18:52:57 anhi Exp $
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

#include <mpi.h>

#if MPI_VERSION >= 2
# define BALL_HAS_MPI2_SUPPORT TRUE
#else
# undef BALL_HAS_MPI2_SUPPORT
#endif

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
			MPISupport(MPI_Comm default_communicator = MPI_COMM_WORLD);

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
			Index getRank();

			/// Return the number of processes in MPI_COMM_WORLD
			Index getSize();

			/// Return the default communicator used for MPI calls.
			MPI_Comm getDefaultCommunicator();

			/// Set the default communicator used for MPI calls.
			void setDefaultCommunicator(MPI_Comm default_communicator);

			/** Returns true if MPI_Finalize will be called in the destructor,
			 *  false otherwise.
			 */
			bool getFinalizeOnDestruct();

			/// Decides whether MPI_Finalize will be called in the destructor
			void setFinalizeOnDestruct(bool new_value);

			/** Set the internal MPI_Info object. This is currently only a workaround,
			 *  and the memory handling (MPI_Info_free) has to be performed by the
			 *  calling process.
			 */
			void setMpiInfo(const MPI_Info &mpi_info);

			/// Returns true if this process is the master, false otherwise.
			bool isMaster();

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
			void setBarrier();

			/** Send a system across the communicator.
			 *  This function relies on BALL's XDRPersistenceManagement.
			 *  If broadcast is true, all processes in default_communicator_ will receive the
			 *  system. Note that all of these processes _must_ call receiveSystem!
			 *  If broadcast is set to false, the message is sent to receiver only.
			 */
			void sendSystem(const System& system, bool broadcast = true, int receiver = 0);

			/** Receive a system from the communicator.
			 *  If broadcast is true, we expect a broadcast, otherwise a send.
			 *  In case of directed send, the source can be given as well.
			 *  Note that we will have to ensure that the system is deleted ourselves.
			 */
			System* receiveSystem(bool broadcast = true, int source = MPI_ANY_SOURCE);

			/** Send a BALL option class across the communicator.
			 *  This function relies on BALL's XDRPersistenceManagement.
			 *  If broadcast is true, all processes in default_communicator_ will receive the
			 *  options. Note that all of these processes _must_ call receiveOptions!
			 *  If broadcast is set to false, the message is sent to receiver only.
			 */
			void sendOptions(const Options& options, bool broadcast = true, int receiver = 0);

			/** Receive a BALL option class from the communicator.
			 *  If broadcast is true, we expect a broadcast, otherwise a send.
			 *  In case of directed send, the source can be given as well.
			 *  Note that we will have to ensure that the Options are deleted ourselves.
			 */
			Options* receiveOptions(bool broadcast = true, int source = MPI_ANY_SOURCE);

			/** Distribute input vector as evenly as possible over all
			 *  processes in the communicator. Stores the part of the datapoints
			 *  the sender itself has to process in our_share. 
			 *  Note that _all_ processes in the communicator have to call acceptDatapoints.
			 */
			template <typename valuetype>
			void distributeDatapoints(const std::vector<valuetype>& input, std::vector<valuetype>& our_share);

			/** Accept datapoints that are distributed by some source in the
			 *  communicator. Stores the result in our_share.
			 */
			template <typename valuetype>
			void acceptDatapoints(std::vector<valuetype>& our_share);

			/** Combine datapoints from all processes of the communicator.
			 *  Exactly one process has to accept the data by calling 
			 *  acceptCombinedDatapoints instead of combineDatapoints.
			 *  If the required memory cannot be allocated, an Exception::OutOfMemory is thrown.
			 **/
			template <typename valuetype>
			void combineDatapoints(const std::vector<valuetype>& our_share);

			/** Accept datapoints that are combined from all processes of the
			 *  communicator. 
			 *  If the required memory cannot be allocated, an Exception::OutOfMemory is thrown.
			 */
			template <typename valuetype>
			void acceptCombinedDatapoints(std::vector<valuetype>& combined_set, std::vector<valuetype>& our_share);

			/** Distribute input of type datatype as evenly as possible over the
			 *  processes in the communicator. Returns the datapoints
			 *  the sender itself has to process. The number of points to process
			 *  is stored in numpoints. Note that _all_ processes in the
			 *  communicator have to call acceptDatapoints.
			 *  The caller has to ensure that the returned array is free()'d.
			 *  If memory allocation fails, 0 is returned.
			 */
			void* distributeDatapoints(const void* input, int size, Size& numpoints, MPI_Datatype datatype);

			/** Accept datapoints that are distributed by some source in the
			 *  communicator. The caller has to ensure that the array is free()'d.
			 *  If memory allocation fails, 0 is returned. The number of
			 *  points we need to process is stored in numpoints.
			 */
			void* acceptDatapoints(Size& numpoints, MPI_Datatype datatype);

			/** Combine distributed data from all processes in the communicator. 
			 *  Note that _all but one_ of the processes in the
			 *  communicator have to call combineDatapoints, while exactly one has
			 *  to call acceptCombinedDatapoints.
			 */
			void combineDatapoints(const void* input, int size, MPI_Datatype datatype);

			/** Combine distributed data from all processes in the communicator 
			 *  and return the result. The array input contains our own share
			 *  of the data we need to combine. The number of points gathered 
			 *  is stored in numpoints. Note that _all other_ processes in the
			 *  communicator have to call combineDatapoints. 
			 *  The caller has to ensure that the returned array is free()'d.
			 *  If memory allocation fails, 0 is returned.
			 */
			void* acceptCombinedDatapoints(const void* input, int size, Size& numpoints, MPI_Datatype datatype);

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
			Size spawn(const String& command, char *argv[], Size wanted_number_of_processes = 0, bool merge_communicator = true);
#endif
			
			/** Sum up the local_value of all processes. If this process is the
			 *  master, this function will return the result. For all non-master 
			 *  processes, the result is undefined.
			 *	This is implemented as a template function to encapsulate the
			 *  MPI_Datatype - handling.
		 	 */
			template <typename valuetype>
			valuetype getSum(valuetype& local_value);
			
			/** Multiply the local_value of all processes. If this process is the
			 *  master, this function will return the result. For all non-master 
			 *  processes, the result is undefined.
			 *	This is implemented as a template function to encapsulate the
			 *  MPI_Datatype - handling.
		 	 */
			template <typename valuetype>
			valuetype getProduct(valuetype& local_value);
			
			/** Determine the maximum of the local_values of all processes. 
			 *  If this process is the
			 *  master, this function will return the result. For all non-master 
			 *  processes, the result is undefined.
			 *	This is implemented as a template function to encapsulate the
			 *  MPI_Datatype - handling.
		 	 */
			template <typename valuetype>
			valuetype getMaximum(valuetype& local_value);
			
			/** Determine the minimum of the local_values of all processes. 
			 *  If this process is the
			 *  master, this function will return the result. For all non-master 
			 *  processes, the result is undefined.
			 *	This is implemented as a template function to encapsulate the
			 *  MPI_Datatype - handling.
		 	 */
			template <typename valuetype>
			valuetype getMinimum(valuetype& local_value);
			//@}
			
		protected:
			/** Register MPI_Datatypes used for sending BALL objects around.
			 */
			void registerTypes_();

			/** Helper function for sending BALL - objects: sends a string
			 *  containing a persistence stream over the communicator.
			 */
			void sendPersistenceStream_(const std::ostringstream& stream, 
																	int tag = MPI_ANY_TAG, bool broadcast = true, int receiver = 0);
			
			/** Helper function for receiving BALL - objects: receives a string
			 *  containing a persistence stream from the communicator and stores
			 *  it in the istream
			 */
			void receivePersistenceStream_(std::istringstream& in, int tag = MPI_ANY_TAG, 
										 							   bool broadcast = true, int source = 0);	

			Index 		rank_;
			Index 		comm_size_;
			bool  		finalize_on_destruct_;
			MPI_Comm  default_communicator_;
			MPI_Info  mpi_info_object_;
			MPI_Datatype  mpi_Vector3_float_type_;
			MPI_Datatype  mpi_Vector3_double_type_;
	};
}
#endif



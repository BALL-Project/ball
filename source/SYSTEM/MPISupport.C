// TODO: Why does XDRManager fail for Options???
#include <BALL/SYSTEM/MPISupport.h>

#include <BALL/KERNEL/system.h>
#include <BALL/DATATYPE/options.h>

#include <BALL/CONCEPT/XDRPersistenceManager.h>
#include <BALL/CONCEPT/textPersistenceManager.h>

#include <vector>
#include <iostream>
#include <iterator>

namespace BALL
{
	MPISupport::MPISupport(MPI_Comm default_communicator)
		throw()
		: finalize_on_destruct_(true),
			default_communicator_(default_communicator),
			mpi_info_object_(MPI_INFO_NULL)
	{
	}

	MPISupport::MPISupport(int argc, char** argv, MPI_Comm default_communicator, bool accept_communicator)
		: finalize_on_destruct_(true),
			default_communicator_(default_communicator),
			mpi_info_object_(MPI_INFO_NULL)
	{
		int is_initialized;
		MPI_Initialized(&is_initialized);
		BALL_PRECONDITION_EXCEPTION(!is_initialized, 
				"MPI has already been initialized! Ignoring set argc and argv!");

		MPI_Init(&argc, &argv);

		registerTypes_();

		// should we try to get a default_communicator_ from a potential parent?
		if (accept_communicator)
		{
			// do we _have_ a parent?
			MPI_Comm parent;
			MPI_Comm_get_parent(&parent); 
			if (parent != MPI_COMM_NULL)
			{
				default_communicator_ = parent;
				
				// now change to the new communicator
				MPI_Intercomm_merge(parent, true, &default_communicator_);
			}
		}
		
		// Initialize rank and size
		MPI_Comm_rank(default_communicator_, &rank_);
		MPI_Comm_size(default_communicator_, &comm_size_);
	}

	MPISupport::~MPISupport()
	{
		if (finalize_on_destruct_)
			MPI_Finalize();
	}

	Index MPISupport::getRank()
		throw()
	{
		return rank_;
	}

	Index MPISupport::getSize()
		throw()
	{
		return comm_size_;
	}

	MPI_Comm MPISupport::getDefaultCommunicator()
		throw()
	{
		return default_communicator_;
	}
	
	void MPISupport::setDefaultCommunicator(MPI_Comm default_communicator)
		throw()
	{
		default_communicator_ = default_communicator;
	}

	bool MPISupport::getFinalizeOnDestruct()
		throw()
	{
		return finalize_on_destruct_;
	}

	void MPISupport::setFinalizeOnDestruct(bool new_value)
		throw()
	{
		finalize_on_destruct_ = new_value;
	}

	void MPISupport::setMpiInfo(const MPI_Info& mpi_info)
		throw()
	{
		mpi_info_object_ = mpi_info;
	}

	bool MPISupport::isMaster()
		throw()
	{
		return (rank_ == 0);
	}

	void MPISupport::init(int argc, char** argv, bool accept_communicator)
	{
		int is_initialized;
		MPI_Initialized(&is_initialized);
		BALL_PRECONDITION_EXCEPTION(!is_initialized, 
				"MPI has already been initialized! Ignoring set argc and argv!");

		MPI_Init(&argc, &argv);
		
		registerTypes_();

		// should we try to get a default_communicator_ from a potential parent?
		if (accept_communicator)
		{
			// do we _have_ a parent?
			MPI_Comm parent;
			MPI_Comm_get_parent(&parent); 
			if (parent != MPI_COMM_NULL)
			{
				default_communicator_ = parent;
				
				// now change to the new communicator
				MPI_Intercomm_merge(parent, true, &default_communicator_);
			}
		}
		
		// Initialize rank and size
		MPI_Comm_rank(default_communicator_, &rank_);
		MPI_Comm_size(default_communicator_, &comm_size_);
	}

	void MPISupport::setBarrier()
		throw()
	{
		MPI_Barrier(default_communicator_);
	}

	void MPISupport::sendSystem(const System& system, bool broadcast, int receiver)
		throw()
	{
		// First, convert the System into a string using the XDRManager
		ostringstream persistent_system;
		XDRPersistenceManager pm(persistent_system);
		pm << system;

		// And send it across the communicator
		sendPersistenceStream_(persistent_system, TAG_SYSTEM, broadcast, receiver);
	}

	System* MPISupport::receiveSystem(bool broadcast, int source)
		throw()
	{
		istringstream is;
		receivePersistenceStream_(is, TAG_SYSTEM, broadcast, source);
		XDRPersistenceManager pm(is);

		System *S;
		S = (System*) pm.readObject();

		// and return
		return S;
	}

	void MPISupport::sendOptions(const Options& options, bool broadcast, int receiver)
		throw()
	{
		// First, convert the Options into a string using the XDRManager
		ostringstream persistent_options;
		//XDRPersistenceManager pm(persistent_options);
		TextPersistenceManager pm(persistent_options);
		options.write(pm);

		// And send it across the communicator
		sendPersistenceStream_(persistent_options, TAG_OPTIONS, broadcast, receiver);
	}

	Options* MPISupport::receiveOptions(bool broadcast, int source)
		throw()
	{
		istringstream is;
		receivePersistenceStream_(is, TAG_OPTIONS, broadcast, source);
		//XDRPersistenceManager pm(is);
		TextPersistenceManager pm(is);

		Options *O = new Options;
		O->read(pm);

		// and return
		return O;
	}

	Size MPISupport::distributeDatapoints(const void* input, int size, void* our_data, MPI_Datatype datatype)
		throw()
	{
		/** Distribute our rank to all processes **/
		for (int i=0; i<comm_size_; i++)
		{
			if (i != rank_)
				MPI_Send(&rank_, 1, MPI_INT, i, 0, default_communicator_);
		}

		/** Distribute the number of points to all processes **/
		MPI_Bcast(&size, 1, MPI_INT, rank_, default_communicator_);	
		
		/** Compute the number of data points to send and receive **/
		std::vector<int> datapoints_per_process(comm_size_);
		std::vector<int> stride(comm_size_);

		int base_size = (int)floor(size / comm_size_);
		int remainder = size % comm_size_;
		int current_stride = 0;

		for (int i=0; i<comm_size_; i++)
		{
			datapoints_per_process[i] = base_size;
			stride[i]									= current_stride;

			if (i < remainder)
				datapoints_per_process[i]++;

			current_stride += datapoints_per_process[i];
		}

		/** Distribute the data **/
		int data_size;
		MPI_Type_size(datatype, &data_size);
printf("Master: %d\n", datapoints_per_process[rank_]*data_size);
		our_data = (void*) malloc(datapoints_per_process[rank_]*data_size);
		if (our_data == 0)
			return datapoints_per_process[rank_];

		void* inputp = const_cast<void*>(input);
		MPI_Scatterv(inputp, 		&datapoints_per_process[0],     &stride[0], datatype,
							   our_data,   datapoints_per_process[rank_],             datatype,
								 rank_,      default_communicator_);

		/** And return our own share of it... **/
		return datapoints_per_process[rank_];
	}

	Size MPISupport::acceptDatapoints(void* our_data, MPI_Datatype datatype)
		throw()
	{
		/** Wait for the source rank... **/
		MPI_Status status;

		int source;
		MPI_Recv(&source, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, default_communicator_, &status);

		/** Accept the number of points in total **/
		int size;
		MPI_Bcast(&size, 1, MPI_INT, source, default_communicator_);	
		
		/** Compute the number of data points to send and receive **/
		std::vector<int> datapoints_per_process(comm_size_);
		std::vector<int> stride(comm_size_);

		int base_size = (int)floor(size / comm_size_);
		int remainder = size % comm_size_;
		int current_stride = 0;

		for (int i=0; i<comm_size_; i++)
		{
			datapoints_per_process[i] = base_size;
			stride[i]									= current_stride;

			if (i < remainder)
				datapoints_per_process[i]++;

			current_stride += datapoints_per_process[i];
		}

		/** Allocate enough data **/
		int data_size;
		MPI_Type_size(datatype, &data_size);

		our_data = (void*) malloc(datapoints_per_process[rank_]*data_size);
		if (our_data == 0)
			return datapoints_per_process[rank_];

		/** Accept our part of the datapoints **/
		MPI_Scatterv(0, 		    &datapoints_per_process[0],     &stride[0], datatype,
							   our_data,   datapoints_per_process[rank_],  						datatype,
								 source,     default_communicator_);

		/** And return it... **/
		return datapoints_per_process[rank_];
	}
	
	void MPISupport::sendPersistenceStream_(const ostringstream& stream, TAGS tag, bool broadcast, int receiver)
		throw()
	{
		// should we broadcast or send directed?
		if (broadcast)
		{
			/** This is a bit tedious: we need to broadcast our rank manually to the receiving processes
			 */
			for (int i=0; i<comm_size_; i++)
			{
				if (i != rank_)
					MPI_Send(&rank_, 1, MPI_INT, i, tag, default_communicator_);
			}

			// send the length of the data
			int length = (int)stream.str().size();
			MPI_Bcast(&length, 1, MPI_INT, rank_, default_communicator_);

			// and now the data itself
			MPI_Bcast(const_cast<char*>(stream.str().c_str()), length+1, MPI_CHAR, rank_, default_communicator_);
		}
		else
		{
			// send the length of the data
			int length = (int)stream.str().size();
			MPI_Send(&length, 1, MPI_INT, receiver, tag, default_communicator_);

			// and now the data itself
			MPI_Send(const_cast<char*>(stream.str().c_str()), length+1, MPI_CHAR, receiver, tag, default_communicator_);
		}
	}

	void MPISupport::receivePersistenceStream_(std::istringstream& in, TAGS tag, bool broadcast, int source)
		throw()
	{
		std::vector<char> streambuf;

		// is this a broadcast?
		if (broadcast)
		{
			MPI_Status status;

			// first receive the rank of the root
			int root;
			MPI_Recv(&root, 1, MPI_INT, MPI_ANY_SOURCE, tag, default_communicator_, &status);

			// then the length of the stream
			int length;
			MPI_Bcast(&length, 1, MPI_INT, root, default_communicator_);

			// allocate the memory
			streambuf.resize(length+1);

			// and receive the string
			MPI_Bcast(&(streambuf[0]), length+1, MPI_CHAR, root, default_communicator_);

			streambuf[length] = 0;
		}
		else
		{
			MPI_Status status;

			// receive the length of the data
			int length;
			MPI_Recv(&length, 1, MPI_INT, source, tag, default_communicator_, &status);

			// allocate the memory
			streambuf.resize(length+1);

			// and receive the stream
			MPI_Recv(&(streambuf[0]), length+1, MPI_CHAR, source, tag, default_communicator_, &status);

			streambuf[length] = 0;
		}

		// Prepare the Text - Stuff...
		// TODO: This should work _much_ more efficiently somehow...
		ostringstream received_stream;
		std::copy(streambuf.begin(), streambuf.end(), ostream_iterator<char>(received_stream));
		in.str(received_stream.str());
	}

#ifdef BALL_HAS_MPI2_SUPPORT
	Size MPISupport::spawn(const String& command, char *argv[], Size wanted_number_of_processes)
		throw()
	{
		/** Should we use MPI_UNIVERSE to find a sensible number of processes? **/
		if (wanted_number_of_processes == 0)
		{
			int *universe_size;
			int has_universe;
			MPI_Attr_get(default_communicator_, MPI_UNIVERSE_SIZE, &universe_size, &has_universe);  

			if (!has_universe) // this MPI does not support MPI_UNIVERSE... we can't spawn here
				return 0;

			wanted_number_of_processes = *universe_size - 1;
		}

		// now try the spawning
		MPI_Comm new_communicator_;
		std::vector<int> errcodes(wanted_number_of_processes);

		MPI_Comm_spawn(const_cast<char*>(command.c_str()), argv, wanted_number_of_processes, mpi_info_object_, 
									 rank_, default_communicator_, &new_communicator_, &(errcodes[0]));

		// and determine the number of successes
		Size number_spawned = 0;
		for (Size i=0; i<wanted_number_of_processes; i++)
		{
			if (errcodes[i] == MPI_SUCCESS) 
				number_spawned++;
		}

		// now change to the new communicator
		MPI_Intercomm_merge(new_communicator_, true, &default_communicator_);

		// Update size and rank
		MPI_Comm_size(default_communicator_, &comm_size_);
		MPI_Comm_rank(default_communicator_, &rank_);

		// and return
		return number_spawned;
	}
#endif

	void MPISupport::registerTypes_()
		throw()
	{
		MPI_Type_contiguous(3, MPI_FLOAT,  &mpi_Vector3_float_type_);
		MPI_Type_contiguous(3, MPI_DOUBLE, &mpi_Vector3_double_type_);

		MPI_Type_commit(&mpi_Vector3_float_type_);
		MPI_Type_commit(&mpi_Vector3_double_type_);
	}

}

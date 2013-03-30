// TODO: - Why does XDRManager fail for Options???
// 			 - The distribute / combine / accept methods for TVector currently just cast
// 			   TVector3<float> -> float[3]. This is _not_ portable...
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
				if (default_communicator_ != MPI_COMM_WORLD)
					MPI_Comm_free(&default_communicator_);
				// now change to the new communicator
				MPI_Intercomm_merge(parent, true, &default_communicator_);
				MPI_Comm_free(&parent);
			}
		}
		
		// Initialize rank and size
		MPI_Comm_rank(default_communicator_, &rank_);
		MPI_Comm_size(default_communicator_, &comm_size_);
	}

	MPISupport::~MPISupport()
	{
		if (finalize_on_destruct_)
		{
			int is_initialized;
			MPI_Initialized(&is_initialized);
			if (default_communicator_ != MPI_COMM_WORLD)
				MPI_Comm_free(&default_communicator_);

			if (is_initialized)
				MPI_Finalize();
		}
	}

	Index MPISupport::getRank()
	{
		return rank_;
	}

	Index MPISupport::getSize()
	{
		return comm_size_;
	}

	MPI_Comm MPISupport::getDefaultCommunicator()
	{
		return default_communicator_;
	}
	
	void MPISupport::setDefaultCommunicator(MPI_Comm default_communicator)
	{
		default_communicator_ = default_communicator;
	}

	bool MPISupport::getFinalizeOnDestruct()
	{
		return finalize_on_destruct_;
	}

	void MPISupport::setFinalizeOnDestruct(bool new_value)
	{
		finalize_on_destruct_ = new_value;
	}

	void MPISupport::setMpiInfo(const MPI_Info& mpi_info)
	{
		mpi_info_object_ = mpi_info;
	}

	bool MPISupport::isMaster()
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
				if (default_communicator_ != MPI_COMM_WORLD)
					MPI_Comm_free(&default_communicator_);
				
				// now change to the new communicator
				MPI_Intercomm_merge(parent, true, &default_communicator_);

			  MPI_Comm_free(&parent);
			}
		}
		
		// Initialize rank and size
		MPI_Comm_rank(default_communicator_, &rank_);
		MPI_Comm_size(default_communicator_, &comm_size_);
	}

	void MPISupport::setBarrier()
	{
		MPI_Barrier(default_communicator_);
	}

	void MPISupport::sendSystem(const System& system, bool broadcast, int receiver)
	{
		// First, convert the System into a string using the XDRManager
		std::ostringstream persistent_system;
		XDRPersistenceManager pm(persistent_system);
		pm << system;

		// And send it across the communicator
		sendPersistenceStream_(persistent_system, TAG_SYSTEM, broadcast, receiver);
	}

	System* MPISupport::receiveSystem(bool broadcast, int source)
	{
		std::istringstream is;
		receivePersistenceStream_(is, TAG_SYSTEM, broadcast, source);
		XDRPersistenceManager pm(is);

		System *S;
		S = (System*) pm.readObject();

		// and return
		return S;
	}

	void MPISupport::sendOptions(const Options& options, bool broadcast, int receiver)
	{
		// First, convert the Options into a string using the XDRManager
		std::ostringstream persistent_options;
		//XDRPersistenceManager pm(persistent_options);
		TextPersistenceManager pm(persistent_options);
		options.write(pm);

		// And send it across the communicator
		sendPersistenceStream_(persistent_options, TAG_OPTIONS, broadcast, receiver);
	}

	Options* MPISupport::receiveOptions(bool broadcast, int source)
	{
		std::istringstream is;
		receivePersistenceStream_(is, TAG_OPTIONS, broadcast, source);
		//XDRPersistenceManager pm(is);
		TextPersistenceManager pm(is);

		Options *O = new Options;
		O->read(pm);

		// and return
		return O;
	}

	void* MPISupport::distributeDatapoints(const void* input, int size, Size &numpoints, MPI_Datatype datatype)
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

		int base_size = (int)floor(size / (float)comm_size_);
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

		numpoints = datapoints_per_process[rank_];

		/** Distribute the data **/
		int data_size;
		MPI_Type_size(datatype, &data_size);

		void* our_data = (void*) malloc(datapoints_per_process[rank_]*data_size);
		if (our_data == 0)
			return 0;

		void* inputp = const_cast<void*>(input);
		MPI_Scatterv(inputp, 		&datapoints_per_process[0],     &stride[0], datatype,
							   our_data,   datapoints_per_process[rank_],             datatype,
								 rank_,      default_communicator_);

		/** And return our own share of it... **/
		return our_data;
	}

	void* MPISupport::acceptDatapoints(Size& numpoints, MPI_Datatype datatype)
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

		int base_size = (int)floor(size / (float)comm_size_);
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
		numpoints = datapoints_per_process[rank_];

		/** Allocate enough data **/
		int data_size;
		MPI_Type_size(datatype, &data_size);

		void* our_data = (void*) malloc(datapoints_per_process[rank_]*data_size);
		if (our_data == 0)
			return 0;

		/** Accept our part of the datapoints **/
		MPI_Scatterv(0, 		    &datapoints_per_process[0],     &stride[0], datatype,
							   our_data,   datapoints_per_process[rank_],  						datatype,
								 source,     default_communicator_);

		/** And return it... **/
		return our_data;
	}

	
	void MPISupport::sendPersistenceStream_(const std::ostringstream& stream, int tag, bool broadcast, int receiver)
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

	void MPISupport::receivePersistenceStream_(std::istringstream& in, int tag, bool broadcast, int source)
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
		std::ostringstream received_stream;
		std::copy(streambuf.begin(), streambuf.end(), std::ostream_iterator<char>(received_stream));
		in.str(received_stream.str());
	}

#ifdef BALL_HAS_MPI2_SUPPORT
	Size MPISupport::spawn(const String& command, char *argv[], Size wanted_number_of_processes, bool merge_communicator)
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

		if (merge_communicator)
		{
			// now change to the new communicator
			if (default_communicator_ != MPI_COMM_WORLD)
				MPI_Comm_free(&default_communicator_);

			MPI_Intercomm_merge(new_communicator_, true, &default_communicator_);
			MPI_Comm_free(&new_communicator_);

			// Update size and rank
			MPI_Comm_size(default_communicator_, &comm_size_);
			MPI_Comm_rank(default_communicator_, &rank_);
		}

		// and return
		return number_spawned;
	}
#endif

	void MPISupport::registerTypes_()
	{
		MPI_Type_contiguous(3, MPI_FLOAT,  &mpi_Vector3_float_type_);
		MPI_Type_contiguous(3, MPI_DOUBLE, &mpi_Vector3_double_type_);

		MPI_Type_commit(&mpi_Vector3_float_type_);
		MPI_Type_commit(&mpi_Vector3_double_type_);
	}

	template <>
	signed char MPISupport::getSum(signed char& local_value)
	{
		signed char result;
		MPI_Reduce(&local_value, &result, 1, MPI_CHAR, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed short int MPISupport::getSum(signed short int& local_value)
	{
		signed short int result;
		MPI_Reduce(&local_value, &result, 1, MPI_SHORT, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed int MPISupport::getSum(signed int& local_value)
	{
		signed int result;
		MPI_Reduce(&local_value, &result, 1, MPI_INT, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed long int MPISupport::getSum(signed long int& local_value)
	{
		signed long int result;
		MPI_Reduce(&local_value, &result, 1, MPI_LONG, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned char MPISupport::getSum(unsigned char& local_value)
	{
		unsigned char result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_CHAR, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned short MPISupport::getSum(unsigned short& local_value)
	{
		unsigned short result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_SHORT, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned int MPISupport::getSum(unsigned int& local_value)
	{
		unsigned int result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned long int MPISupport::getSum(unsigned long int& local_value)
	{
		unsigned long int result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_LONG, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	float MPISupport::getSum(float& local_value)
	{
		float result;
		MPI_Reduce(&local_value, &result, 1, MPI_FLOAT, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	double MPISupport::getSum(double& local_value)
	{
		double result;
		MPI_Reduce(&local_value, &result, 1, MPI_DOUBLE, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	long double MPISupport::getSum(long double& local_value)
	{
		long double result;
		MPI_Reduce(&local_value, &result, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed char MPISupport::getProduct(signed char& local_value)
	{
		signed char result;
		MPI_Reduce(&local_value, &result, 1, MPI_CHAR, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed short int MPISupport::getProduct(signed short int& local_value)
	{
		signed short int result;
		MPI_Reduce(&local_value, &result, 1, MPI_SHORT, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed int MPISupport::getProduct(signed int& local_value)
	{
		signed int result;
		MPI_Reduce(&local_value, &result, 1, MPI_INT, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed long int MPISupport::getProduct(signed long int& local_value)
	{
		signed long int result;
		MPI_Reduce(&local_value, &result, 1, MPI_LONG, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned char MPISupport::getProduct(unsigned char& local_value)
	{
		unsigned char result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_CHAR, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned short MPISupport::getProduct(unsigned short& local_value)
	{
		unsigned short result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_SHORT, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned int MPISupport::getProduct(unsigned int& local_value)
	{
		unsigned int result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned long int MPISupport::getProduct(unsigned long int& local_value)
	{
		unsigned long int result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_LONG, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	float MPISupport::getProduct(float& local_value)
	{
		float result;
		MPI_Reduce(&local_value, &result, 1, MPI_FLOAT, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	double MPISupport::getProduct(double& local_value)
	{
		double result;
		MPI_Reduce(&local_value, &result, 1, MPI_DOUBLE, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	long double MPISupport::getProduct(long double& local_value)
	{
		long double result;
		MPI_Reduce(&local_value, &result, 1, MPI_LONG_DOUBLE, MPI_PROD, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed char MPISupport::getMaximum(signed char& local_value)
	{
		signed char result;
		MPI_Reduce(&local_value, &result, 1, MPI_CHAR, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed short int MPISupport::getMaximum(signed short int& local_value)
	{
		signed short int result;
		MPI_Reduce(&local_value, &result, 1, MPI_SHORT, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed int MPISupport::getMaximum(signed int& local_value)
	{
		signed int result;
		MPI_Reduce(&local_value, &result, 1, MPI_INT, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed long int MPISupport::getMaximum(signed long int& local_value)
	{
		signed long int result;
		MPI_Reduce(&local_value, &result, 1, MPI_LONG, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned char MPISupport::getMaximum(unsigned char& local_value)
	{
		unsigned char result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_CHAR, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned short MPISupport::getMaximum(unsigned short& local_value)
	{
		unsigned short result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_SHORT, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned int MPISupport::getMaximum(unsigned int& local_value)
	{
		unsigned int result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned long int MPISupport::getMaximum(unsigned long int& local_value)
	{
		unsigned long int result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_LONG, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	float MPISupport::getMaximum(float& local_value)
	{
		float result;
		MPI_Reduce(&local_value, &result, 1, MPI_FLOAT, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	double MPISupport::getMaximum(double& local_value)
	{
		double result;
		MPI_Reduce(&local_value, &result, 1, MPI_DOUBLE, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	long double MPISupport::getMaximum(long double& local_value)
	{
		long double result;
		MPI_Reduce(&local_value, &result, 1, MPI_LONG_DOUBLE, MPI_MAX, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed char MPISupport::getMinimum(signed char& local_value)
	{
		signed char result;
		MPI_Reduce(&local_value, &result, 1, MPI_CHAR, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed short int MPISupport::getMinimum(signed short int& local_value)
	{
		signed short int result;
		MPI_Reduce(&local_value, &result, 1, MPI_SHORT, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed int MPISupport::getMinimum(signed int& local_value)
	{
		signed int result;
		MPI_Reduce(&local_value, &result, 1, MPI_INT, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	signed long int MPISupport::getMinimum(signed long int& local_value)
	{
		signed long int result;
		MPI_Reduce(&local_value, &result, 1, MPI_LONG, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned char MPISupport::getMinimum(unsigned char& local_value)
	{
		unsigned char result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_CHAR, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned short MPISupport::getMinimum(unsigned short& local_value)
	{
		unsigned short result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_SHORT, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned int MPISupport::getMinimum(unsigned int& local_value)
	{
		unsigned int result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	unsigned long int MPISupport::getMinimum(unsigned long int& local_value)
	{
		unsigned long int result;
		MPI_Reduce(&local_value, &result, 1, MPI_UNSIGNED_LONG, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	float MPISupport::getMinimum(float& local_value)
	{
		float result;
		MPI_Reduce(&local_value, &result, 1, MPI_FLOAT, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	double MPISupport::getMinimum(double& local_value)
	{
		double result;
		MPI_Reduce(&local_value, &result, 1, MPI_DOUBLE, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	long double MPISupport::getMinimum(long double& local_value)
	{
		long double result;
		MPI_Reduce(&local_value, &result, 1, MPI_LONG_DOUBLE, MPI_MIN, 0, default_communicator_);

		/** Note: the value of result is undefined if rank_ != 0 **/
		return result;
	}

	template <>
	void MPISupport::distributeDatapoints(const std::vector<TVector3<float> >& input,
																					    std::vector<TVector3<float> >& our_share)
	{
		// We'll need to take care of the pointers passed around...
		float* our_data = 0;
		Size   num_points;
		
		// Vector3 does _not_ guarantee a sensible memory alignment. So let's test this first...
		if (sizeof(TVector3<float>) == 3*sizeof(float))
			our_data = (float*) distributeDatapoints(&input[0], input.size(), num_points, mpi_Vector3_float_type_);
		else
		{
			// we need to provide a buffer and copy the input
			float* input_buffer = new float[3*input.size()];
			for (Size i=0; i<input.size(); i++)
			{
				input_buffer[3*i]   = input[i].x;
				input_buffer[3*i+1] = input[i].y;
				input_buffer[3*i+2] = input[i].z;
			}
			our_data = (float*) distributeDatapoints(input_buffer, input.size(), num_points, mpi_Vector3_float_type_);

			// and free the buffer
			delete[](input_buffer);
		}

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
	{
		// We'll need to take care of the pointers passed around...
		Size num_points;
		float* our_data = (float*) acceptDatapoints(num_points, mpi_Vector3_float_type_);

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
	void MPISupport::distributeDatapoints(const std::vector<TVector3<double> >& input,
																					    std::vector<TVector3<double> >& our_share)
	{
		// We'll need to take care of the pointers passed around...
		double* our_data = 0;
		Size 		num_points;

		// Vector3 does _not_ guarantee a sensible memory alignment. So let's test this first...
		if (sizeof(TVector3<double>) == 3*sizeof(double))
			our_data = (double*) distributeDatapoints(&input[0], input.size(), num_points, mpi_Vector3_double_type_);
		else
		{
			// we need to provide a buffer and copy the input
			double* input_buffer = new double[3*input.size()];
			for (Size i=0; i<input.size(); i++)
			{
				input_buffer[3*i]   = input[i].x;
				input_buffer[3*i+1] = input[i].y;
				input_buffer[3*i+2] = input[i].z;
			}
			our_data = (double*) distributeDatapoints(input_buffer, input.size(), num_points, mpi_Vector3_double_type_);

			// and free the buffer
			delete[](input_buffer);
		}

		if (our_data == 0)
			throw(Exception::OutOfMemory(__FILE__, __LINE__, num_points*3*sizeof(double)));

		// create the output vector
		our_share.resize(num_points);

		for (Size i=0; i<num_points; i++)
		{
			our_share[i].set(our_data[3*i], our_data[3*i+1], our_data[3*i+2]);
		}

		free(our_data);
	}

	template <>
	void MPISupport::acceptDatapoints(std::vector<TVector3<double> >& our_share)
	{
		// We'll need to take care of the pointers passed around...
		Size num_points;
		double* our_data = (double*) acceptDatapoints(num_points, mpi_Vector3_double_type_);

		if (our_data == 0)
			throw(Exception::OutOfMemory(__FILE__, __LINE__, num_points*3*sizeof(double)));

		// create the output vector
		our_share.resize(num_points);

		for (Size i=0; i<num_points; i++)
		{
			our_share[i].set(our_data[3*i], our_data[3*i+1], our_data[3*i+2]);
		}

		free(our_data);
	}

	template <>
	void MPISupport::distributeDatapoints(const std::vector<float>& input,
																					    std::vector<float>& our_share)
	{
		// We'll need to take care of the pointers passed around...
		Size num_points;
		float* our_data = (float*) distributeDatapoints(&input[0], input.size(), num_points, MPI_FLOAT);

		if (our_data == 0)
			throw(Exception::OutOfMemory(__FILE__, __LINE__, num_points*sizeof(float)));

		// create the output vector
		// TODO: is this copying necessary?
		our_share.resize(num_points);

		for (Size i=0; i<num_points; i++)
		{
			our_share[i] = our_data[i];
		}

		free(our_data);
	}

	template <>
	void MPISupport::acceptDatapoints(std::vector<float>& our_share)
	{
		// We'll need to take care of the pointers passed around...
		Size num_points;
		float* our_data = (float*) acceptDatapoints(num_points, MPI_FLOAT);

		if (our_data == 0)
			throw(Exception::OutOfMemory(__FILE__, __LINE__, num_points*sizeof(float)));

		// create the output vector
		// TODO: is this copying necessary?
		our_share.resize(num_points);

		for (Size i=0; i<num_points; i++)
		{
			our_share[i] = our_data[i];
		}

		free(our_data);
	}

	template <>
	void MPISupport::distributeDatapoints(const std::vector<double>& input,
																					    std::vector<double>& our_share)
	{
		// We'll need to take care of the pointers passed around...
		Size num_points;
		double* our_data = (double*) distributeDatapoints(&input[0], input.size(), num_points, MPI_DOUBLE);

		if (our_data == 0)
			throw(Exception::OutOfMemory(__FILE__, __LINE__, num_points*sizeof(double)));

		// create the output vector
		// TODO: is this copying necessary?
		our_share.resize(num_points);

		for (Size i=0; i<num_points; i++)
		{
			our_share[i] = our_data[i];
		}

		free(our_data);
	}

	template <>
	void MPISupport::acceptDatapoints(std::vector<double>& our_share)
	{
		// We'll need to take care of the pointers passed around...
		Size num_points;
		double* our_data = (double*) acceptDatapoints(num_points, MPI_DOUBLE);

		if (our_data == 0)
			throw(Exception::OutOfMemory(__FILE__, __LINE__, num_points*sizeof(double)));

		// create the output vector
		// TODO: is this copying necessary?
		our_share.resize(num_points);

		for (Size i=0; i<num_points; i++)
		{
			our_share[i] = our_data[i];
		}

		free(our_data);
	}

	template <>
	void MPISupport::distributeDatapoints(const std::vector<int>& input,
																					    std::vector<int>& our_share)
	{
		// We'll need to take care of the pointers passed around...
		Size num_points;
		int* our_data = (int*) distributeDatapoints(&input[0], input.size(), num_points, MPI_INT);

		if (our_data == 0)
			throw(Exception::OutOfMemory(__FILE__, __LINE__, num_points*sizeof(int)));

		// create the output vector
		// TODO: is this copying necessary?
		our_share.resize(num_points);

		for (Size i=0; i<num_points; i++)
		{
			our_share[i] = our_data[i];
		}

		free(our_data);
	}

	template <>
	void MPISupport::acceptDatapoints(std::vector<int>& our_share)
	{
		// We'll need to take care of the pointers passed around...
		Size num_points;
		int* our_data = (int*) acceptDatapoints(num_points, MPI_INT);

		if (our_data == 0)
			throw(Exception::OutOfMemory(__FILE__, __LINE__, num_points*sizeof(int)));

		// create the output vector
		// TODO: is this copying necessary?
		our_share.resize(num_points);

		for (Size i=0; i<num_points; i++)
		{
			our_share[i] = our_data[i];
		}

		free(our_data);
	}

	template <>
	void MPISupport::combineDatapoints(const std::vector<TVector3<float> >& our_share)
	{
		// distribute the data
		combineDatapoints(&our_share[0], our_share.size(), mpi_Vector3_float_type_);
	}

	template <>
	void MPISupport::acceptCombinedDatapoints(std::vector<TVector3<float> >& combined_set, 
																						std::vector<TVector3<float> >& our_share)
	{
		// distribute and accept 
		Size numpoints;
		float *result = (float*) acceptCombinedDatapoints(&our_share[0], our_share.size(), numpoints, mpi_Vector3_float_type_);

		if (result == 0)
			throw(Exception::OutOfMemory(__FILE__, __LINE__, numpoints*3*sizeof(float)));

		// and create the output vector
		combined_set.resize(numpoints);

		for (Size i=0; i<numpoints; i++)
			combined_set[i].set(result[3*i], result[3*i+1], result[3*i+2]);

		// free the result buffer
		free(result);
	}

	template <>
	void MPISupport::combineDatapoints(const std::vector<TVector3<double> >& our_share)
	{
		// distribute the data
		combineDatapoints(&our_share[0], our_share.size(), mpi_Vector3_double_type_);
	}

	template <>
	void MPISupport::acceptCombinedDatapoints(std::vector<TVector3<double> >& combined_set, 
																						std::vector<TVector3<double> >& our_share)
	{
		// distribute the data
		Size numpoints;
		double *result = (double*) acceptCombinedDatapoints(&our_share[0], our_share.size(), numpoints, mpi_Vector3_double_type_);

		if (result == 0)
			throw(Exception::OutOfMemory(__FILE__, __LINE__, numpoints*3*sizeof(double)));

		// and create the output vector
		combined_set.resize(numpoints);

		for (Size i=0; i<numpoints; i++)
			combined_set[i].set(result[3*i], result[3*i+1], result[3*i+2]);

		// free the result buffer
		free(result);
	}

	template <>
	void MPISupport::combineDatapoints(const std::vector<float>& our_share)
	{
		// distribute the data 
		combineDatapoints(&our_share[0], our_share.size(), MPI_FLOAT);
	}

	template <>
	void MPISupport::acceptCombinedDatapoints(std::vector<float>& combined_set, 
																						std::vector<float>& our_share)
	{
		// distribute the data
		Size numpoints;
		float *result = (float*) acceptCombinedDatapoints(&our_share[0], our_share.size(), numpoints, MPI_FLOAT);

		if (result == 0)
			throw(Exception::OutOfMemory(__FILE__, __LINE__, numpoints*sizeof(float)));

		// and create the output vector
		combined_set.resize(numpoints);

		for (Size i=0; i<numpoints; i++)
			combined_set[i] = result[i];

		// free the result buffer
		free(result);
	}

	template <>
	void MPISupport::combineDatapoints(const std::vector<double>& our_share)
	{
		// distribute the data
		combineDatapoints(&our_share[0], our_share.size(), MPI_DOUBLE);
	}

	template <>
	void MPISupport::acceptCombinedDatapoints(std::vector<double>& combined_set, 
																						std::vector<double>& our_share)
	{
		// distribute the data
		Size numpoints;
		double *result = (double*) acceptCombinedDatapoints(&our_share[0], our_share.size(), numpoints, MPI_DOUBLE);

		if (result == 0)
			throw(Exception::OutOfMemory(__FILE__, __LINE__, numpoints*sizeof(double)));

		// and create the output vector
		combined_set.resize(numpoints);

		for (Size i=0; i<numpoints; i++)
			combined_set[i] = result[i];

		// free the result buffer
		free(result);
	}

	template <>
	void MPISupport::combineDatapoints(const std::vector<int>& our_share)
	{
		// distribute the data
		combineDatapoints(&our_share[0], our_share.size(), MPI_INT);
	}

	template <>
	void MPISupport::acceptCombinedDatapoints(std::vector<int>& combined_set, 
																						std::vector<int>& our_share)
	{
		// distribute the data
		Size numpoints;
		int *result = (int*) acceptCombinedDatapoints(&our_share[0], our_share.size(), numpoints, MPI_INT);

		if (result == 0)
			throw(Exception::OutOfMemory(__FILE__, __LINE__, numpoints*sizeof(int)));
		
		// and create the output vector
		combined_set.resize(numpoints);

		for (Size i=0; i<numpoints; i++)
			combined_set[i] = result[i];

		// free the result buffer
		free(result);
	}

	void MPISupport::combineDatapoints(const void* input, int size, MPI_Datatype datatype)
	{
		/** Wait for the source rank... **/
		MPI_Status status;

		int source;
		MPI_Recv(&source, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, default_communicator_, &status);

		// This might not always be the most efficient idea, but we will just gather all sizes
		// of all subarrays so gathering is easy for the master
		MPI_Gather(&size, 1, MPI_INT, 0, 0, MPI_INT, source, default_communicator_);
		
		// Gathering is simple for us: we just need to send our own data...
		MPI_Gatherv(const_cast<void*>(input), size, datatype, 0, 0, 0, datatype, source, default_communicator_);
	}	

	void* MPISupport::acceptCombinedDatapoints(const void* input, int size, Size& numpoints, MPI_Datatype datatype)
	{
		/** Distribute our rank to all processes **/
		for (int i=0; i<comm_size_; i++)
		{
			if (i != rank_)
				MPI_Send(&rank_, 1, MPI_INT, i, 0, default_communicator_);
		}

		/** Instead of computing the strides and sizes ourselves, we gather
		 *  them from the slaves.
		 */
		std::vector<int> datapoints_per_process(comm_size_);
		std::vector<int> stride(comm_size_);

		MPI_Gather(&size, 1, MPI_INT, &datapoints_per_process[0], 1, MPI_INT, rank_, default_communicator_);

		// Compute the strides
		int current_stride = 0;
		for (int i=0; i<comm_size_; i++)
		{
			stride[i] = current_stride;
			current_stride += datapoints_per_process[i];
		}

		// current_stride now holds the number of datapoints
		numpoints = current_stride;

		/** Create the array to store the data **/
		int data_size;
		MPI_Type_size(datatype, &data_size);

		void* gathered_data = (void*) malloc(current_stride*data_size);
		if (gathered_data == 0)
			return 0;

		/** And finally, we can gather everything together... **/
		MPI_Gatherv(const_cast<void*>(input), size, datatype,
							  gathered_data, &datapoints_per_process[0], &stride[0],
						 		datatype, rank_, default_communicator_);

		/** And return it... **/
		return gathered_data;
	}

}

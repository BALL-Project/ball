// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//
	class TRRFile
	  : TrajectoryFile
	{
%TypeHeaderCode
	#include <BALL/FORMAT/TRRFile.h>
  #include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
		public:
		class TRRHeader
		{
			public: 
			Size        MAGIC;
			Size        VERSION;
			Size        title_string_length;
			String      title_string;
			Size        ir_size;
			Size        e_size;
			Size        bounding_box_data_size;
			Size        vir_size;
			Size        pres_size;
			Size        top_size;
			Size        sym_size;
			Size        position_data_size;
			Size        velocity_data_size;
			Size        force_data_size;
			Size        number_of_atoms;
			Size        timestep_index;
			Size        nre;
			float       timestep_time;
			float       lambda;

			TRRHeader()	throw();
		};

		TRRFile()	throw();
		TRRFile(const String& /name/, OpenMode /open_mode/ = std::ios::in) throw();
		virtual ~TRRFile() throw();

		// const TRRFile& operator = (const TRRFile& /file/)	throw();
		virtual void clear() throw();

		bool operator == (const TRRFile& /file/) const throw();
		virtual bool init() throw();
		virtual bool writeNextHeader(const TRRHeader& /header/) throw();
		virtual bool readNextHeader(TRRHeader& /header/) throw();
		virtual bool append(const SnapShot& /snapshot/) throw();
		virtual bool read(SnapShot& /snapshot/) throw();
		virtual TRRFile& operator >> (SnapShotManager& /ssm/)	throw();
		
		// virtual bool flushToDisk(const std::vector<SnapShot>& buffer) throw(File::CannotWrite);
		bool hasVelocities() const throw();
		void setVelocityStorage(bool /storage/) throw();
		bool hasForces() const throw();
		void setForceStorage(bool /storage/) throw();
		Size getPrecision() const throw();
		bool setPrecision(const Size /precision/) throw();
		float getTimestep() const throw();
		void setTimestep(const float /timestep/) throw();
		Vector3 getBoundingBoxX() const	throw();
		Vector3 getBoundingBoxY() const	throw();
		Vector3 getBoundingBoxZ() const	throw();
		void setBoundingBox(const Vector3& /x/, const Vector3& /y/, const Vector3& /z/) throw();
		private:
		TRRFile(const TRRFile& /file/);
	};

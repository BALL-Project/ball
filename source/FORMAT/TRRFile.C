// $Id: TRRFile.C,v 1.1 2001/09/13 11:45:10 anhi Exp $

#include <BALL/FORMAT/TRRFile.h>
#include <BALL/MOLMEC/COMMON/snapShot.h>
#include <BALL/MOLMEC/COMMON/snapShotManager.h>

using namespace std;

namespace BALL
{
  TRRFile::TRRFile()
		throw()
		:TrajectoryFile(),
		 header_(),
		 precision_(4),
		 has_velocities_(false),
		 has_forces_(false),
		 timestep_index_(0),
		 timestep_(0.002),
		 box1_(),
		 box2_(),
		 box3_()
	{
		init();
	}

	TRRFile::TRRFile(const TRRFile& file)
		throw(Exception::FileNotFound)
		: TrajectoryFile(file),
			header_(file.header_),
			precision_(file.precision_),
			has_velocities_(file.has_velocities_),
			has_forces_(file.has_forces_),
			timestep_index_(file.timestep_index_),
			timestep_(file.timestep_),
			box1_(file.box1_),
			box2_(file.box2_),
			box3_(file.box3_)
	{
		init();
	}

	TRRFile::TRRFile(const String& name, File::OpenMode open_mode)
		throw()
		: TrajectoryFile(name, open_mode),
			header_(),
			precision_(4),
			has_velocities_(false),
			has_forces_(false),
			timestep_index_(0),
			timestep_(0.002),
			box1_(),
			box2_(),
			box3_()
	{
		init();
	}

	TRRFile::~TRRFile()
		throw()
	{
		close();
		clear();
	}

	const TRRFile& TRRFile::operator = (const TRRFile& file)
		throw()
	{
		TrajectoryFile::operator = (file);
		header_ = file.header_;
		precision_ = file.precision_;
		has_velocities_ = file.has_velocities_;
		has_forces_ = file.has_forces_;
		timestep_index_ = file.timestep_index_;
		timestep_ = file.timestep_;
		box1_ = file.box1_;
		box2_ = file.box2_;
		box3_ = file.box3_;

		return *this;
	}

	void TRRFile::clear()
		throw()
	{
		header_ = TRRHeader();
		precision_ = 4;
		has_velocities_ = false;
		has_forces_ = false;
		timestep_index_ = 0;
		timestep_ = 0.002;
		box1_ = Vector3();
		box2_ = Vector3();
		box3_ = Vector3();
		TrajectoryFile::clear();
	}

	bool TRRFile::operator == (const TRRFile& file) const
		throw()
	{
		return ((TrajectoryFile::operator == (file)) && (timestep_index_ == file.timestep_index_)
						&& (timestep_ == file.timestep_) && (precision_ == file.precision_)
						&& (box1_ == file.box1_) && (box2_ == file.box2_) && (box3_ == file.box3_)); 
	}

	bool TRRFile::hasVelocities() const
		throw()
	{
		return has_velocities_;
	}

	void TRRFile::setVelocityStorage(const bool storage)
		throw()
	{
		has_velocities_ = storage;
	}

	bool TRRFile::hasForces() const
		throw()
	{
		return has_forces_;
	}

	void TRRFile::setForceStorage(const bool storage)
		throw()
	{
		has_forces_ = storage;
	}

	Size TRRFile::getPrecision() const
		throw()
	{
		return precision_;
	}

	bool TRRFile::setPrecision(const Size newprecision)
		throw()
	{
		if ((newprecision == 4) || (newprecision == 8))
		{
			precision_ = newprecision;

			return true;
		}
		else
			return false;
	}

	float TRRFile::getTimestep() const
		throw()
	{
		return timestep_;
	}

	void TRRFile::setTimestep(const float timestep)
		throw()
	{
		timestep_ = timestep;
	}

	Vector3 TRRFile::getBoundingBoxX() const
		throw()
	{
		return box1_;
	}

	Vector3 TRRFile::getBoundingBoxY() const
		throw()
	{
		return box2_;
	}

	Vector3 TRRFile::getBoundingBoxZ() const
		throw()
	{
		return box3_;
	}

	void TRRFile::setBoundingBox(const Vector3 box[3])
		throw()
	{
		box1_ = box[0];
		box2_ = box[1];
		box3_ = box[2];
	}

	bool TRRFile::writeNextHeader(const TRRHeader header)
		throw()
	{
		Size i;

		*this << header.MAGIC;
		*this << header.VERSION;
		*this << header.title_string_length;

		for (i=0; i<header.title_string_length; i++)
		{
			*this << header.title_string[i];
		}

		*this << header.ir_size;
		*this << header.e_size;
		*this << header.bounding_box_data_size;
		*this << header.vir_size;
		*this << header.pres_size;
		*this << header.top_size;
		*this << header.sym_size;
		*this << header.position_data_size;
		*this << header.velocity_data_size;
		*this << header.force_data_size;
		*this << header.number_of_atoms;
		*this << header.timestep_index;
		*this << header.nre;
		*this << header.timestep_time;
		*this << header.lambda;
		
		return true;
	}

	bool TRRFile::readNextHeader(TRRHeader &header)
		throw()
	{
		BinaryFileAdaptor<char>   adapt_char;
		BinaryFileAdaptor<Size>   adapt_size;
		BinaryFileAdaptor<float>  adapt_float;

		Size i;

		*this >> adapt_size;
		header.MAGIC = adapt_size.getData();

		if (header.MAGIC != 1993)
		{
			Log.error() << "TRRFile::readNextHeader(): "
									<< "the magic number of header # "
									<< timestep_index_
									<< " is incorrect; expected \"1999\", got "
				          << header.MAGIC << endl;

			return false;
		}

		*this >> adapt_size;
		header.VERSION = adapt_size.getData();

		*this >> adapt_size;
		header.title_string_length = adapt_size.getData();

		// read the title string one by one character (because of the
		// lacking NULL - termination
		for (i=0; i<header.title_string_length; i++)
		{
			*this >> adapt_char;
			header.title_string[i] = adapt_char.getData();
		}

		*this >> adapt_size;
		header.ir_size = adapt_size.getData();
		*this >> adapt_size;
		header.e_size = adapt_size.getData();
		*this >> adapt_size;
		header.bounding_box_data_size = adapt_size.getData();
		*this >> adapt_size;
		header.vir_size = adapt_size.getData();
		*this >> adapt_size;
		header.pres_size = adapt_size.getData();
		*this >> adapt_size;
		header.top_size = adapt_size.getData();
		*this >> adapt_size;
		header.sym_size = adapt_size.getData();
		*this >> adapt_size;
		header.position_data_size = adapt_size.getData();
		*this >> adapt_size;
		header.velocity_data_size = adapt_size.getData();
		*this >> adapt_size;
		header.force_data_size = adapt_size.getData();
		*this >> adapt_size;
		header.number_of_atoms = adapt_size.getData();
		*this >> adapt_size;
		header.timestep_index = adapt_size.getData();
		*this >> adapt_size;
		header.nre = adapt_size.getData();
		*this >> adapt_float;
		header.timestep_time = adapt_float.getData();
		*this >> adapt_float;
		header.lambda = adapt_float.getData();

		if (header.velocity_data_size != 0)
		{
			setVelocityStorage(true);
		}
		if (header.force_data_size != 0)
		{
			setForceStorage(true);
		}

		return true;
	}


	bool TRRFile::append(const SnapShot& snapshot)
		throw()
	{
		BinaryFileAdaptor<float>  adapt_float;
		BinaryFileAdaptor<double> adapt_double;

		Size noa = snapshot.getNumberOfAtoms();
		header_.number_of_atoms = noa;

		header_.bounding_box_data_size = 
			                     (box1_.y == box1_.z == 
				                    box2_.x == box2_.z ==
				                    box3_.x == box3_.y == 0) ? 3*precision_ : 9*precision_;

		header_.position_data_size = precision_ * noa;
		header_.velocity_data_size = (has_velocities_) ? precision_ * noa : 0;
		header_.force_data_size    = (has_forces_    ) ? precision_ * noa : 0;

		header_.timestep_index = timestep_index_;

		// !!! I am not sure if the record timestep_time contains
		// the length of each timestep or the elapsed time. I will
		// assume the first for now!!!
		header_.timestep_time = timestep_;

		if (!writeNextHeader(header_))
			return false;

		// now follows the bounding box information
		if (header_.bounding_box_data_size == 3 * precision_)
		{
			// We have to assume a rectangular system. The bounding box
			// size in this case is stored in the form x1 y2 z3
			if (precision_ == 4)
			{
				adapt_float.setData( (float) box1_.x); *this << adapt_float.getData();
				adapt_float.setData( (float) box2_.y); *this << adapt_float.getData();
				adapt_float.setData( (float) box3_.z); *this << adapt_float.getData();
			}
			else
			{
				adapt_double.setData( (double) box1_.x); *this << adapt_double.getData();
				adapt_double.setData( (double) box2_.y); *this << adapt_double.getData();
				adapt_double.setData( (double) box3_.z); *this << adapt_double.getData();
			}
		}
		else
		{
			if (precision_ == 4)
			{
				adapt_float.setData( (float) box1_.x); *this << adapt_float.getData();
				adapt_float.setData( (float) box1_.y); *this << adapt_float.getData();
				adapt_float.setData( (float) box1_.z); *this << adapt_float.getData();
				adapt_float.setData( (float) box2_.x); *this << adapt_float.getData();
				adapt_float.setData( (float) box2_.y); *this << adapt_float.getData();
				adapt_float.setData( (float) box2_.z); *this << adapt_float.getData();
				adapt_float.setData( (float) box3_.x); *this << adapt_float.getData();
				adapt_float.setData( (float) box3_.y); *this << adapt_float.getData();
				adapt_float.setData( (float) box3_.z); *this << adapt_float.getData();
			}
			else
			{
				adapt_double.setData( (double) box1_.x); *this << adapt_double.getData();
				adapt_double.setData( (double) box1_.y); *this << adapt_double.getData();
				adapt_double.setData( (double) box1_.z); *this << adapt_double.getData();
				adapt_double.setData( (double) box2_.x); *this << adapt_double.getData();
				adapt_double.setData( (double) box2_.y); *this << adapt_double.getData();
				adapt_double.setData( (double) box2_.z); *this << adapt_double.getData();
				adapt_double.setData( (double) box3_.x); *this << adapt_double.getData();
				adapt_double.setData( (double) box3_.y); *this << adapt_double.getData();
				adapt_double.setData( (double) box3_.z); *this << adapt_double.getData();
			}
		}

		// Now could follow the vir_size and pres_size blocks, but we will
		// ignore them. Therefore we can jump to the position data.
		// Each atom is represented by three sequential 4 or 8-byte values.
		Size actAtom;
		
		if (precision_ == 4)
		{
			const vector< TVector3<float> >& positions = snapshot.getAtomPositions();

			for (actAtom=0; actAtom<noa; actAtom++)
			{
				adapt_float.setData( (float) positions[actAtom].x); *this << adapt_float.getData();
				adapt_float.setData( (float) positions[actAtom].y); *this << adapt_float.getData();
				adapt_float.setData( (float) positions[actAtom].z); *this << adapt_float.getData();
			}
		}
		else
		{
			// This is what we really should use:
			// const vector< TVector3<double> >& positions = snapshot.getAtomPositions();
			// but unfortunately, system only allows float - vectors, and cast for vectors
			// is not yet implemented...
			const vector< TVector3<float> >& positions = snapshot.getAtomPositions();

			for (actAtom=0; actAtom<noa; actAtom++)
			{
				adapt_double.setData( (float) positions[actAtom].x); *this << adapt_double.getData();
				adapt_double.setData( (float) positions[actAtom].y); *this << adapt_double.getData();
				adapt_double.setData( (float) positions[actAtom].z); *this << adapt_double.getData();
			}
		}


		// Now it's time to find out if we need to store the velocities
		if (has_velocities_)
		{
			if (precision_ ==4)
			{
				const vector< TVector3<float> >& velocities = snapshot.getAtomVelocities();

				for (actAtom=0; actAtom<noa; actAtom++)
			  {
					adapt_float.setData( (float) velocities[actAtom].x); *this << adapt_float.getData();
					adapt_float.setData( (float) velocities[actAtom].y); *this << adapt_float.getData();
					adapt_float.setData( (float) velocities[actAtom].z); *this << adapt_float.getData();
				}
			}
			else
			{
				// same problem as above
				// const vector< TVector3<double> >& velocities = snapshot.getAtomVelocities();
				const vector< TVector3<float> >& velocities = snapshot.getAtomVelocities();

				for (actAtom=0; actAtom<noa; actAtom++)
				{	 
					adapt_double.setData( (float) velocities[actAtom].x); *this << adapt_double.getData();
					adapt_double.setData( (float) velocities[actAtom].y); *this << adapt_double.getData();
					adapt_double.setData( (float) velocities[actAtom].z); *this << adapt_double.getData();
				}
			}
		}

		// And the same for the forces.
		if (has_forces_)
		{
			if (precision_ == 4)
			{
				const vector< TVector3<float> >& forces = snapshot.getAtomForces();

				for (actAtom=0; actAtom<noa; actAtom++)
			  {
					adapt_float.setData( (float) forces[actAtom].x); *this << adapt_float.getData();
					adapt_float.setData( (float) forces[actAtom].y); *this << adapt_float.getData();
					adapt_float.setData( (float) forces[actAtom].z); *this << adapt_float.getData();
				}
			}
			else
			{
				// same problem as above
				// const vector< TVector3<double> >& forces = snapshot.getAtomForces();
				const vector< TVector3<float> >& forces = snapshot.getAtomForces();

				for (actAtom=0; actAtom<noa; actAtom++)
				{
					adapt_double.setData( (float) forces[actAtom].x); *this << adapt_double.getData();
					adapt_double.setData( (float) forces[actAtom].y); *this << adapt_double.getData();
					adapt_double.setData( (float) forces[actAtom].z); *this << adapt_double.getData();
				}
			}
		}

		// That's it. We're done. We just have to increase the index of the actual
		// snapshot.
		timestep_index_++;
		
		return true;
	}

	bool TRRFile::read(SnapShot& snapshot)
		throw()
	{
		BinaryFileAdaptor<float>  adapt_float;
		BinaryFileAdaptor<double> adapt_double;

 		if (!readNextHeader(header_))
			return false;

		Size noa = header_.number_of_atoms;

		// Try to figure out the precision.
		precision_ = header_.position_data_size / (3*noa);

		if (!( (precision_ == 4) || (precision_ == 8) ))
		{
			Log.error() << "TRRFile::read(): "
									<< "the TRR file header number "
									<< header_.timestep_index
									<< " is corrupt!"
				          << endl;

			return false;
		}

		// Now find out if the bounding box information is stored in
		// short or long format.
		Size bounding_box_block_length = header_.bounding_box_data_size / precision_;

		if (!(   (bounding_box_block_length == 3) 
				  || (bounding_box_block_length == 9)))
		{
			Log.error() << "TRRFile::read(): "
									<< "the TRR file header number "
									<< header_.timestep_index
									<< " is corrupt!"
									<< endl;

			return false;
		}

		// Read the bounding box information.
		if (bounding_box_block_length == 3)
		{
			// The information is stored in the short format.
			if (precision_ == 4)
			{
				*this >> adapt_float; box1_.x = adapt_float.getData();
				*this >> adapt_float; box2_.y = adapt_float.getData();
				*this >> adapt_float; box3_.z = adapt_float.getData();
			}
			else
			{
				*this >> adapt_double; box1_.x = adapt_double.getData();
				*this >> adapt_double; box2_.y = adapt_double.getData();
				*this >> adapt_double; box3_.z = adapt_double.getData();
			}
		}
		else
		{
			if (precision_ == 4)
			{
				*this >> adapt_float; box1_.x = adapt_float.getData();
				*this >> adapt_float; box1_.y = adapt_float.getData();
				*this >> adapt_float; box1_.z = adapt_float.getData();
				*this >> adapt_float; box2_.x = adapt_float.getData();
				*this >> adapt_float; box2_.y = adapt_float.getData();
				*this >> adapt_float; box2_.z = adapt_float.getData();
				*this >> adapt_float; box3_.x = adapt_float.getData();
				*this >> adapt_float; box3_.y = adapt_float.getData();
				*this >> adapt_float; box3_.z = adapt_float.getData();
			}
			else
			{
				*this >> adapt_double; box1_.x = adapt_double.getData();
				*this >> adapt_double; box1_.y = adapt_double.getData();
				*this >> adapt_double; box1_.z = adapt_double.getData();
				*this >> adapt_double; box2_.x = adapt_double.getData();
				*this >> adapt_double; box2_.y = adapt_double.getData();
				*this >> adapt_double; box2_.z = adapt_double.getData();
				*this >> adapt_double; box3_.x = adapt_double.getData();
				*this >> adapt_double; box3_.y = adapt_double.getData();
				*this >> adapt_double; box3_.z = adapt_double.getData();
			}
		}

		// Now it is possible that there are vir_size and pres_size blocks.
		// If so, we'll just discard the data there.
		Size actAtom;

		if ((header_.vir_size + header_.pres_size) != 0)
		{
			for (actAtom=0; actAtom<(header_.vir_size + header_.pres_size); actAtom++)
			{
				if (precision_ == 4)
				{
					*this >> adapt_double;
				}
				else
				{
					*this >> adapt_float;
				}
			}
		}

		// With this obstacle out of the way, we can now concentrate on the positions
		// of the atoms in the system.
		if (precision_ == 4)
		{
			vector< TVector3<float> > positions(noa);

			for (actAtom=0; actAtom<noa; actAtom++)
			{
				*this >> adapt_float; positions[actAtom].x = adapt_float.getData();
				*this >> adapt_float; positions[actAtom].y = adapt_float.getData();
				*this >> adapt_float; positions[actAtom].z = adapt_float.getData();
			}
			snapshot.setAtomPositions(positions);
		}
		else
		{
			// see append() for description of problem
			// vector< TVector3<double> > positions(noa);
			vector< TVector3<float> > positions(noa);


			for (actAtom=0; actAtom<noa; actAtom++)
			{
				*this >> adapt_double; positions[actAtom].x = adapt_double.getData();
				*this >> adapt_double; positions[actAtom].y = adapt_double.getData();
				*this >> adapt_double; positions[actAtom].z = adapt_double.getData();
			}
			snapshot.setAtomPositions(positions);
		}
	
		if (has_velocities_)
		{
			if (precision_ == 4)
			{
				vector< TVector3<float> > velocities(noa);

				for (actAtom=0; actAtom<noa; actAtom++)
			  {
					*this >> adapt_float; velocities[actAtom].x = adapt_float.getData();
					*this >> adapt_float; velocities[actAtom].y = adapt_float.getData();
					*this >> adapt_float; velocities[actAtom].z = adapt_float.getData();

				}
					snapshot.setAtomVelocities(velocities);
			}
			else
		  {
				// same problem as above
				// vector< TVector3<double> > velocities(noa);
				vector< TVector3<float> > velocities(noa);

				for (actAtom=0; actAtom<noa; actAtom++)
				{
					*this >> adapt_double; velocities[actAtom].x = adapt_double.getData();
					*this >> adapt_double; velocities[actAtom].y = adapt_double.getData();
					*this >> adapt_double; velocities[actAtom].z = adapt_double.getData();
				}
				snapshot.setAtomVelocities(velocities);
			}
		}

		if (has_forces_)
		{
			if (precision_ == 4)
			{
				vector< TVector3<float> > forces(noa);

				for (actAtom=0; actAtom<noa; actAtom++)
			  {
					*this >> adapt_float; forces[actAtom].x = adapt_float.getData();
					*this >> adapt_float; forces[actAtom].y = adapt_float.getData();
					*this >> adapt_float; forces[actAtom].z = adapt_float.getData();
				}
				snapshot.setAtomForces(forces);
			}
			else
		  {
				// same problem as above
				// vector< TVector3<double> > forces(noa);
				vector< TVector3<float> > forces(noa);

				for (actAtom=0; actAtom<noa; actAtom++)
				{
					*this >> adapt_double; forces[actAtom].x = adapt_double.getData();
					*this >> adapt_double; forces[actAtom].y = adapt_double.getData();
					*this >> adapt_double; forces[actAtom].z = adapt_double.getData();
				}
				snapshot.setAtomForces(forces);
			}
		}

		timestep_index_++;

		return true;
	}

	TRRFile& TRRFile::operator >> (SnapShotManager& ssm)
		throw()
	{
			System S = *(ssm.getSystem());
			SnapShot sn;

			sn.takeSnapShot(S);
			
			while ((this->good() && read(sn)))
			{
				sn.applySnapShot(S);
				ssm.setSystem(&S);
				ssm.takeSnapShot();
			}
		  
			return *this;
	}
	

	bool TRRFile::flushToDisk(const ::std::vector<SnapShot> buffer)
		throw()
	{
		::std::vector<SnapShot>::const_iterator it = buffer.begin();

		reopen(File::APP | File::BINARY);
		for(; it != buffer.end(); ++it)
		{
			append(*it);
		}

		return true;
	}

	bool TRRFile::init()
		throw()
	{
		if (sizeof(Size) != 4)
		{
			Log.error() << "TRRFile::TRRFile(): "
									<< "Size of int is not equal to 4 on this machine." << endl;

			return false;
		}

		if (sizeof(float) != 4)
		{
			Log.error() << "TRRFile::TRRFile(): "
									<< "Size of float is not equal to 4 on this machine." << endl;

			return false;
		}

		if (sizeof(double) != 8)
		{
			Log.error() << "TRRFile::TRRFile(): "
									<< "Size of double is not equal to 8 on this machine." << endl;

			return false;
		}

		return true;
	}
}

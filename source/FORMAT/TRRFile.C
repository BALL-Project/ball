// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/TRRFile.h>
#include <BALL/MOLMEC/COMMON/snapShot.h>
#include <BALL/MOLMEC/COMMON/snapShotManager.h>
#include <BALL/KERNEL/system.h>

using namespace std;

namespace BALL
{
	const double TRRFile::to_angstrom  = 10.;
	const double TRRFile::to_nanometer = 10.;

  TRRFile::TRRFile()
	 : TrajectoryFile(),
		 header_(),
		 precision_(4),
		 has_velocities_(false),
		 has_forces_(false),
		 timestep_index_(0),
		 timestep_(0.002),
		 box1_(),
		 box2_(),
		 box3_(),
		 old_file_size_(0)
	{
		init();
	}

	TRRFile::TRRFile(const String& name, File::OpenMode open_mode)
		: TrajectoryFile(name, open_mode),
			header_(),
			precision_(4),
			has_velocities_(false),
			has_forces_(false),
			timestep_index_(0),
			timestep_(0.002),
			box1_(),
			box2_(),
			box3_(),
			old_file_size_(0)
	{
		if (!(open_mode & std::ios::binary))
		{
			reopen(open_mode | std::ios::binary);
		}
		init();
	}

	TRRFile::~TRRFile()
	{
		close();
		clear();
	}

	void TRRFile::clear()
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
		old_file_size_ = 0;
		TrajectoryFile::clear();
	}

	bool TRRFile::operator == (const TRRFile& file) const
	{
		return ((TrajectoryFile::operator == (file)) && (timestep_index_ == file.timestep_index_)
						&& (timestep_ == file.timestep_) && (precision_ == file.precision_)
						&& (box1_ == file.box1_) && (box2_ == file.box2_) && (box3_ == file.box3_)
						&& (old_file_size_ == file.old_file_size_)); 
	}

	bool TRRFile::hasVelocities() const
	{
		return has_velocities_;
	}

	void TRRFile::setVelocityStorage(bool storage)
	{
		has_velocities_ = storage;
	}

	bool TRRFile::hasForces() const
	{
		return has_forces_;
	}

	void TRRFile::setForceStorage(bool storage)
	{
		has_forces_ = storage;
	}

	Size TRRFile::getPrecision() const
	{
		return precision_;
	}

	bool TRRFile::setPrecision(const Size newprecision)
	{
		if ((newprecision == 4) || (newprecision == 8))
		{
			precision_ = newprecision;
			return true;
		}

		return false;
	}

	float TRRFile::getTimestep() const
	{
		return timestep_;
	}

	void TRRFile::setTimestep(float timestep)
	{
		timestep_ = timestep;
	}

	Vector3 TRRFile::getBoundingBoxX() const
	{
		return box1_;
	}

	Vector3 TRRFile::getBoundingBoxY() const
	{
		return box2_;
	}

	Vector3 TRRFile::getBoundingBoxZ() const
	{
		return box3_;
	}

	void TRRFile::setBoundingBox(const Vector3& x, const Vector3& y, const Vector3& z)
	{
		box1_ = x;
		box2_ = y;
		box3_ = z;
	}

	bool TRRFile::writeNextHeader(const TRRHeader& header)
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
	{
		Size i;

		*this >> adapt_size_;
		header.MAGIC = adapt_size_.getData();

		if (header.MAGIC != 1993)
		{
			Log.error() << "TRRFile::readNextHeader(): "
									<< "the magic number of header # "
									<< timestep_index_
									<< " is incorrect; expected \"1993\", got "
				          << header.MAGIC << endl;

			return false;
		}

		*this >> adapt_size_;
		header.VERSION = adapt_size_.getData();

		*this >> adapt_size_;
		header.title_string_length = adapt_size_.getData();

		// read the title string one by one character (because of the
		// lacking NULL - termination
		for (i=0; i<header.title_string_length; i++)
		{
			*this >> adapt_char_;
			header.title_string[i] = adapt_char_.getData();
		}

		*this >> adapt_size_;
		header.ir_size = adapt_size_.getData();
		*this >> adapt_size_;
		header.e_size = adapt_size_.getData();
		*this >> adapt_size_;
		header.bounding_box_data_size = adapt_size_.getData();
		*this >> adapt_size_;
		header.vir_size = adapt_size_.getData();
		*this >> adapt_size_;
		header.pres_size = adapt_size_.getData();
		*this >> adapt_size_;
		header.top_size = adapt_size_.getData();
		*this >> adapt_size_;
		header.sym_size = adapt_size_.getData();
		*this >> adapt_size_;
		header.position_data_size = adapt_size_.getData();
		*this >> adapt_size_;
		header.velocity_data_size = adapt_size_.getData();
		*this >> adapt_size_;
		header.force_data_size = adapt_size_.getData();
		*this >> adapt_size_;
		header.number_of_atoms = adapt_size_.getData();
		*this >> adapt_size_;
		header.timestep_index = adapt_size_.getData();
		*this >> adapt_size_;
		header.nre = adapt_size_.getData();
		*this >> adapt_float_;
		header.timestep_time = adapt_float_.getData();
		*this >> adapt_float_;
		header.lambda = adapt_float_.getData();

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
	{
		Size noa = snapshot.getNumberOfAtoms();
		header_.number_of_atoms = noa;

		header_.bounding_box_data_size = 
			                     (((box1_.y == 0.0) && (box1_.z == 0.0)  
				                    && (box2_.x == 0.0) && (box2_.z == 0.0)
				                    && (box3_.x == 0.0) && (box3_.y == 0.0)) 
															? (3 * precision_) : (9 * precision_));

		header_.position_data_size = precision_ * noa;
		header_.velocity_data_size = (has_velocities_) ? precision_ * noa : 0;
		header_.force_data_size    = (has_forces_    ) ? precision_ * noa : 0;

		header_.timestep_index = timestep_index_;

		// !!! I am not sure if the record timestep_time contains
		// the length of each timestep or the elapsed time. I will
		// assume the first for now!!!
		header_.timestep_time = timestep_;

		if (!writeNextHeader(header_))
		{
			return false;
		}

		// now follows the bounding box information
		if (header_.bounding_box_data_size == 3 * precision_)
		{
			// We have to assume a rectangular system. The bounding box
			// size in this case is stored in the form x1 y2 z3
			if (precision_ == 4)
			{
				adapt_float_.setData( (float) to_nanometer * box1_.x); *this << adapt_float_.getData();
				adapt_float_.setData( (float) to_nanometer * box2_.y); *this << adapt_float_.getData();
				adapt_float_.setData( (float) to_nanometer * box3_.z); *this << adapt_float_.getData();
			}
			else
			{
				adapt_double_.setData( (double) to_nanometer * box1_.x); *this << adapt_double_.getData();
				adapt_double_.setData( (double) to_nanometer * box2_.y); *this << adapt_double_.getData();
				adapt_double_.setData( (double) to_nanometer * box3_.z); *this << adapt_double_.getData();
			}
		}
		else
		{
			if (precision_ == 4)
			{
				adapt_float_.setData( (float) to_nanometer * box1_.x); *this << adapt_float_.getData();
				adapt_float_.setData( (float) to_nanometer * box1_.y); *this << adapt_float_.getData();
				adapt_float_.setData( (float) to_nanometer * box1_.z); *this << adapt_float_.getData();
				adapt_float_.setData( (float) to_nanometer * box2_.x); *this << adapt_float_.getData();
				adapt_float_.setData( (float) to_nanometer * box2_.y); *this << adapt_float_.getData();
				adapt_float_.setData( (float) to_nanometer * box2_.z); *this << adapt_float_.getData();
				adapt_float_.setData( (float) to_nanometer * box3_.x); *this << adapt_float_.getData();
				adapt_float_.setData( (float) to_nanometer * box3_.y); *this << adapt_float_.getData();
				adapt_float_.setData( (float) to_nanometer * box3_.z); *this << adapt_float_.getData();
			}
			else
			{
				adapt_double_.setData( (double) to_nanometer * box1_.x); *this << adapt_double_.getData();
				adapt_double_.setData( (double) to_nanometer * box1_.y); *this << adapt_double_.getData();
				adapt_double_.setData( (double) to_nanometer * box1_.z); *this << adapt_double_.getData();
				adapt_double_.setData( (double) to_nanometer * box2_.x); *this << adapt_double_.getData();
				adapt_double_.setData( (double) to_nanometer * box2_.y); *this << adapt_double_.getData();
				adapt_double_.setData( (double) to_nanometer * box2_.z); *this << adapt_double_.getData();
				adapt_double_.setData( (double) to_nanometer * box3_.x); *this << adapt_double_.getData();
				adapt_double_.setData( (double) to_nanometer * box3_.y); *this << adapt_double_.getData();
				adapt_double_.setData( (double) to_nanometer * box3_.z); *this << adapt_double_.getData();
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
				adapt_float_.setData( (float) to_nanometer * positions[actAtom].x); *this << adapt_float_.getData();
				adapt_float_.setData( (float) to_nanometer * positions[actAtom].y); *this << adapt_float_.getData();
				adapt_float_.setData( (float) to_nanometer * positions[actAtom].z); *this << adapt_float_.getData();
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
				adapt_double_.setData( (float) to_nanometer * positions[actAtom].x); *this << adapt_double_.getData();
				adapt_double_.setData( (float) to_nanometer * positions[actAtom].y); *this << adapt_double_.getData();
				adapt_double_.setData( (float) to_nanometer * positions[actAtom].z); *this << adapt_double_.getData();
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
					adapt_float_.setData( (float) to_nanometer * velocities[actAtom].x); *this << adapt_float_.getData();
					adapt_float_.setData( (float) to_nanometer * velocities[actAtom].y); *this << adapt_float_.getData();
					adapt_float_.setData( (float) to_nanometer * velocities[actAtom].z); *this << adapt_float_.getData();
				}
			}
			else
			{
				// same problem as above
				// const vector< TVector3<double> >& velocities = snapshot.getAtomVelocities();
				const vector< TVector3<float> >& velocities = snapshot.getAtomVelocities();

				for (actAtom=0; actAtom<noa; actAtom++)
				{	 
					adapt_double_.setData( (float) to_nanometer * velocities[actAtom].x); *this << adapt_double_.getData();
					adapt_double_.setData( (float) to_nanometer * velocities[actAtom].y); *this << adapt_double_.getData();
					adapt_double_.setData( (float) to_nanometer * velocities[actAtom].z); *this << adapt_double_.getData();
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
					adapt_float_.setData( (float) to_nanometer * forces[actAtom].x); *this << adapt_float_.getData();
					adapt_float_.setData( (float) to_nanometer * forces[actAtom].y); *this << adapt_float_.getData();
					adapt_float_.setData( (float) to_nanometer * forces[actAtom].z); *this << adapt_float_.getData();
				}
			}
			else
			{
				// same problem as above
				// const vector< TVector3<double> >& forces = snapshot.getAtomForces();
				const vector< TVector3<float> >& forces = snapshot.getAtomForces();

				for (actAtom=0; actAtom<noa; actAtom++)
				{
					adapt_double_.setData( (float) to_nanometer * forces[actAtom].x); *this << adapt_double_.getData();
					adapt_double_.setData( (float) to_nanometer * forces[actAtom].y); *this << adapt_double_.getData();
					adapt_double_.setData( (float) to_nanometer * forces[actAtom].z); *this << adapt_double_.getData();
				}
			}
		}

		// That's it. We're done. We just have to increase the index of the actual
		// snapshot.
		timestep_index_++;
		
		return true;
	}

	bool TRRFile::read(SnapShot& snapshot)
	{
 		if (!readNextHeader(header_)) return false;

		Size noa = header_.number_of_atoms;
		snapshot.setNumberOfAtoms(noa);

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
				*this >> adapt_float_; box1_.x = adapt_float_.getData() * to_angstrom;
				*this >> adapt_float_; box2_.y = adapt_float_.getData() * to_angstrom;
				*this >> adapt_float_; box3_.z = adapt_float_.getData() * to_angstrom;
			}
			else
			{
				*this >> adapt_double_; box1_.x = adapt_double_.getData() * to_angstrom;
				*this >> adapt_double_; box2_.y = adapt_double_.getData() * to_angstrom;
				*this >> adapt_double_; box3_.z = adapt_double_.getData() * to_angstrom;
			}
		}
		else
		{
			if (precision_ == 4)
			{
				*this >> adapt_float_; box1_.x = adapt_float_.getData() * to_angstrom;
				*this >> adapt_float_; box1_.y = adapt_float_.getData() * to_angstrom;
				*this >> adapt_float_; box1_.z = adapt_float_.getData() * to_angstrom;
				*this >> adapt_float_; box2_.x = adapt_float_.getData() * to_angstrom;
				*this >> adapt_float_; box2_.y = adapt_float_.getData() * to_angstrom;
				*this >> adapt_float_; box2_.z = adapt_float_.getData() * to_angstrom;
				*this >> adapt_float_; box3_.x = adapt_float_.getData() * to_angstrom;
				*this >> adapt_float_; box3_.y = adapt_float_.getData() * to_angstrom;
				*this >> adapt_float_; box3_.z = adapt_float_.getData() * to_angstrom;
			}
			else
			{
				*this >> adapt_double_; box1_.x = adapt_double_.getData() * to_angstrom;
				*this >> adapt_double_; box1_.y = adapt_double_.getData() * to_angstrom;
				*this >> adapt_double_; box1_.z = adapt_double_.getData() * to_angstrom;
				*this >> adapt_double_; box2_.x = adapt_double_.getData() * to_angstrom;
				*this >> adapt_double_; box2_.y = adapt_double_.getData() * to_angstrom;
				*this >> adapt_double_; box2_.z = adapt_double_.getData() * to_angstrom;
				*this >> adapt_double_; box3_.x = adapt_double_.getData() * to_angstrom;
				*this >> adapt_double_; box3_.y = adapt_double_.getData() * to_angstrom;
				*this >> adapt_double_; box3_.z = adapt_double_.getData() * to_angstrom;
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
					*this >> adapt_double_;
				}
				else
				{
					*this >> adapt_float_;
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
				*this >> adapt_float_; positions[actAtom].x = adapt_float_.getData() * to_angstrom;
				*this >> adapt_float_; positions[actAtom].y = adapt_float_.getData() * to_angstrom;
				*this >> adapt_float_; positions[actAtom].z = adapt_float_.getData() * to_angstrom;
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
				*this >> adapt_double_; positions[actAtom].x = adapt_double_.getData() * to_angstrom;
				*this >> adapt_double_; positions[actAtom].y = adapt_double_.getData() * to_angstrom;
				*this >> adapt_double_; positions[actAtom].z = adapt_double_.getData() * to_angstrom;
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
					*this >> adapt_float_; velocities[actAtom].x = adapt_float_.getData() * to_angstrom;
					*this >> adapt_float_; velocities[actAtom].y = adapt_float_.getData() * to_angstrom;
					*this >> adapt_float_; velocities[actAtom].z = adapt_float_.getData() * to_angstrom;

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
					*this >> adapt_double_; velocities[actAtom].x = adapt_double_.getData() * to_angstrom;
					*this >> adapt_double_; velocities[actAtom].y = adapt_double_.getData() * to_angstrom;
					*this >> adapt_double_; velocities[actAtom].z = adapt_double_.getData() * to_angstrom;
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
					*this >> adapt_float_; forces[actAtom].x = adapt_float_.getData() * to_angstrom;
					*this >> adapt_float_; forces[actAtom].y = adapt_float_.getData() * to_angstrom;
					*this >> adapt_float_; forces[actAtom].z = adapt_float_.getData() * to_angstrom;
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
					*this >> adapt_double_; forces[actAtom].x = adapt_double_.getData() * to_angstrom;
					*this >> adapt_double_; forces[actAtom].y = adapt_double_.getData() * to_angstrom;
					*this >> adapt_double_; forces[actAtom].z = adapt_double_.getData() * to_angstrom;
				}
				snapshot.setAtomForces(forces);
			}
		}

		timestep_index_++;

		return true;
	}

	bool TRRFile::skipFrame()
	{
 		if (!readNextHeader(header_)) return false;

		Size noa = header_.number_of_atoms;

		// Try to figure out the precision.
		precision_ = header_.position_data_size / (3*noa);

		if (!( (precision_ == 4) || (precision_ == 8) ))
		{
			Log.error() << "TRRFile::skipFrame(): "
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
				*this >> adapt_float_;
				*this >> adapt_float_;
				*this >> adapt_float_;
			}
			else
			{
				*this >> adapt_double_;
				*this >> adapt_double_;
				*this >> adapt_double_;
			}
		}
		else
		{
			if (precision_ == 4)
			{
				*this >> adapt_float_;
				*this >> adapt_float_;
				*this >> adapt_float_;
				*this >> adapt_float_;
				*this >> adapt_float_;
				*this >> adapt_float_;
				*this >> adapt_float_;
				*this >> adapt_float_;
				*this >> adapt_float_;
			}
			else
			{
				*this >> adapt_double_;
				*this >> adapt_double_;
				*this >> adapt_double_;
				*this >> adapt_double_;
				*this >> adapt_double_;
				*this >> adapt_double_;
				*this >> adapt_double_;
				*this >> adapt_double_;
				*this >> adapt_double_;
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
					*this >> adapt_double_;
				}
				else
				{
					*this >> adapt_float_;
				}
			}
		}

		// With this obstacle out of the way, we can now concentrate on the positions
		// of the atoms in the system.
		if (precision_ == 4)
		{
			for (actAtom=0; actAtom<noa; actAtom++)
			{
				*this >> adapt_float_;
				*this >> adapt_float_;
				*this >> adapt_float_;
			}
		}
		else
		{
			for (actAtom=0; actAtom<noa; actAtom++)
			{
				*this >> adapt_double_;
				*this >> adapt_double_;
				*this >> adapt_double_;
			}
		}
	
		if (has_velocities_)
		{
			if (precision_ == 4)
			{
				for (actAtom=0; actAtom<noa; actAtom++)
			  {
					*this >> adapt_float_;
					*this >> adapt_float_;
					*this >> adapt_float_;

				}
			}
			else
		  {
				for (actAtom=0; actAtom<noa; actAtom++)
				{
					*this >> adapt_double_;
					*this >> adapt_double_;
					*this >> adapt_double_;
				}
			}
		}

		if (has_forces_)
		{
			if (precision_ == 4)
			{
				for (actAtom=0; actAtom<noa; actAtom++)
			  {
					*this >> adapt_float_;
					*this >> adapt_float_;
					*this >> adapt_float_;
				}
			}
			else
		  {
				for (actAtom=0; actAtom<noa; actAtom++)
				{
					*this >> adapt_double_;
					*this >> adapt_double_;
					*this >> adapt_double_;
				}
			}
		}

		return true;
	}

	Size TRRFile::getNumberOfSnapShots()
	{
		// do we have current information?
		Size current_file_size = getSize();

		if (current_file_size == old_file_size_)
		{
			return number_of_snapshots_;
		}

		// save position
		Position old_ts_pos = tellg();

		// rewind
		seekg(0);
		number_of_snapshots_ = 0;

		while (skipFrame())
			number_of_snapshots_++;

		// and go back
		seekg(old_ts_pos);

		old_file_size_ = current_file_size;

		return number_of_snapshots_;
	}

	TRRFile& TRRFile::operator >> (SnapShotManager& ssm)
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

	bool TRRFile::flushToDisk(const std::vector<SnapShot>& buffer)
	{
		if (!reopen(File::MODE_APP | File::MODE_BINARY) && good())
		{
			throw (File::CannotWrite(__FILE__, __LINE__, name_));
		}

		std::vector<SnapShot>::const_iterator it = buffer.begin();

		for(; it != buffer.end(); ++it)
		{
			append(*it);
		}

		return true;
	}

	bool TRRFile::readHeader()
	{
		// TRRFiles don't have a common file header...
		return true;
	}

	bool TRRFile::init()
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

		// try to determine the endianness of the file

		*this >> adapt_size_;

		this->reopen();
		if (adapt_size_.getData() != 1993)
		{
			adapt_size_.setSwapEndian(true);
			*this >> adapt_size_;
		 
			if (adapt_size_.getData() != 1993)
			{
				Log.error() << "TRRFile::init(): "
										<< "first header could not be identified. "
										<< "This does not seem to be a correct TRR file. Aborting!" << endl;

				return false;
			}
			else
			{
				Log.info() << "TRRFile::init(): "
									 << "TRRFile seems to use non-standard endianness. Will try auto-byte-swapping." << endl;
			// chars can not be swapped!
			//	adapt_char_.setSwapEndian(true);
				adapt_float_.setSwapEndian(true);
				adapt_double_.setSwapEndian(true);
			}
		}

		// read a first header to extract the number of atoms
		reopen();
		readNextHeader(header_);
		number_of_atoms_ = header_.number_of_atoms;
		reopen();

		return true;
	}
}

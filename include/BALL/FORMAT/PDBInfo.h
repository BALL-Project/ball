// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_PDBINFO_H
#define BALL_FORMAT_PDBINFO_H

#include <vector>
#include <stdexcept>

#ifndef BALL_FORMAT_PDBRECORDS_H
#	include <BALL/FORMAT/PDBRecords.h>
#endif

#ifndef BALL_FORMAT_PDBDEFS_H
#	include <BALL/FORMAT/PDBdefs.h>
#endif

#ifndef BALL_CONCEPT_PERSISTENCEMANAGER_H
#	include <BALL/CONCEPT/persistenceManager.h>
#endif

namespace BALL 
{
	
	/**	PDB information class.
			This class contains	holds summary information
			on a PDB file read by \link GenericPDBFile \endlink or
			\link PDBFile \endlink as well as unparse records.
	*/
	class BALL_EXPORT PDBInfo
			  : public PersistentObject
	{
		public:
			
		BALL_CREATE(PDBInfo)
		
		/**	@name Constructors and Destructor */
		//@{
		///
		PDBInfo() ;
		///
		PDBInfo(const PDBInfo& pdbi) ;
		///
		virtual ~PDBInfo() ;
		/// Reset all attributes to their initial state
		virtual void clear();
		//@}

		/**	Assignment
		*/
		//@{
		///
		PDBInfo& operator = (const PDBInfo& rhs);
		//@}

		/**	Predicates
		*/
		//@{
		/// Equality operator
		bool operator == (const PDBInfo& rhs) const;
		/// Inequality operator
		bool operator != (const PDBInfo& rhs) const;
		//@}

		/**	Accessors
		*/
		//@{
		/// Return the unparsed records
		PDBRecords& getSkippedRecords() { return skipped_records_; }
		/// Return the unparsed records
		const PDBRecords& getSkippedRecords() const { return skipped_records_; }
		/// Return the unparsed records
		PDBRecords& getInvalidRecords() { return invalid_records_; }
		/// Return the unparsed records
		const PDBRecords& getInvalidRecords() const { return invalid_records_; }
		/// Return the filename
		const String& getFilename() const { return filename_; }
		/// Set the filename
		void setFilename(const String& filename) { filename_ = filename; }
		/// Return the PDB ID 
		const String& getID() const { return id_; }
		/// Set the PDB ID 
		void setID(const String& id) { id_ = id; }
		/// Return the total number of records read
		Size getNumberOfRecords() const { return number_of_records_; }
		/// Set the number of records
		void setNumberOfRecords(Size n) { number_of_records_ = n; }
		/// Return the number of models
		Size getNumberOfModels() const { return number_of_models_; }
		/// Return the number of models
		Size& getNumberOfModels() { return number_of_models_; }
		/// Set the number of models
		void setNumberOfModels(Size n) { number_of_models_ = n; }
		/// Return the number of the last model read
		Position getCurrentModel() const { return model_read_; }
		/// Return the number of the last model read
		Position& getCurentModel() { return model_read_; }
		/// Set the number of the model read
		void setCurrentModel(Position n) { model_read_ = n; }
		/// Return the number of atom records read
		Size getNumberOfATOMRecords() const { return number_of_atom_records_; }
		/// Return the number of atom records read
		Size& getNumberOfATOMRecords() { return number_of_atom_records_; }
		/// Set the number of atom records read
		void setNumberOfATOMRecords(Size n) { number_of_atom_records_ = n; }
		/// Return the number of hetatm records read
		Size getNumberOfHETATMRecords() const { return number_of_hetatm_records_; }
		/// Return the number of hetatm records read
		Size& getNumberOfHETATMRecords() { return number_of_hetatm_records_; }
		/// Set the number of hetatm records read
		void setNumberOfHETATMRecords(Size n) { number_of_hetatm_records_ = n; }
		/// Return the name of the system as contained in the HEADER record
		const String& getName() const { return name_; }
 		/// Set the name of the system. This is written to the HEADER record (truncated!).
		void setName(const String& name) { name_ = name; }
		/**	Return a list of indices matchin a specific record type.
				This list is constructed by a linear search over all records currently
				stored in the skipped indices.
		*/
		std::list<Position> getRecordIndices(PDB::RecordType record) const;
		//@}

		/** @name Storable Interface
		*/
		//@{

		/** Persistent stream writing.
		*/
		//void write(PersistenceManager& pm) const;
		void persistentWrite(PersistenceManager& pm, const char* name) const;

		/** Persistent stream reading.
		*/
		//bool read(PersistenceManager& pm);
		void persistentRead(PersistenceManager& pm);

		//@}


		protected:
		/// Skipped PDB records (not parsed)
		PDBRecords skipped_records_;
		/// Invalid PDB records (tried to parse, but failed)
		PDBRecords invalid_records_;
		/// Filename
		String filename_;
		/// PDB ID
		String id_;
		/// Name 
		String name_;
		/// Total file size in bytes
		Size number_of_records_;
		/// Total number of models
		Size number_of_models_;
		/// Model last read
		Position model_read_;
		/// Number of ATOM records
		Size number_of_atom_records_;
		/// Number of HETATM records
		Size number_of_hetatm_records_;
	};
	
} // namespace BALL

#endif // BALL_FORMAT_PDBINFO_H

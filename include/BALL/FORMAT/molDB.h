// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann, Marc Röttig $
// ----------------------------------------------------

#ifndef MOLDB_H
#define MOLDB_H

#include <QtSql/QtSql>
#include <BALL/COMMON/exception.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/DATATYPE/string.h>

#include <BALL/KERNEL/forEach.h>
#include <BALL/DOCKING/COMMON/result.h>
#include <set>
#include <BALL/STRUCTURE/molecularSimilarity.h>

#if defined BALL_HAS_OPENBABEL || defined BALL_HAS_OPENEYE
	#include <BALL/STRUCTURE/logP.h>
	#ifdef BALL_HAS_OPENEYE
		#include <oechem/graphmol.h>
	#endif
#endif


namespace BALL
{
		/**     Molecule database class. \n
				This class is intended to store small molecules, information about them (fingerprint, smiles, etc.) and results of docking or (re-)scoring.\n
				Topologies and conformations are stored separately in order to prevent waste of space. IDs will be automatically assigned to new topologies and conformations.\n \n

				When a given molecule is to be stored in the database (by use of MolDB::storeMolecule()), it is automatically detected (by use of hash-keys for topologies and conformations) whether its topology and conformation need to be written to the database or whether they already exists in the database. This way, waste of disk space is automatically prevented.\n
				Furthermore, all Properties of a given molecule will also be saved and will be restored when retrieving the molecule later.\n \n

				Additionally, the following things that can be used for searching molecules later will be automatically calculated and stored in the database:
				-- molecular weight
				-- canonical smile (if OpenEye or OpenBabel is available)
				-- logP (if OpenEye or OpenBabel is available; needed for SMARTS matching)
				-- binary pathway-based fingerprint
				-- functional group counts (if OpenEye or OpenBabel is available; needed for SMARTS matching). \n \n

				Information about docking or (re-)scoring can be stored by use MolDB::storeResult() and retrieved later by MolDB::fetchResults(). \n
		 */
		class BALL_EXPORT MolDB
		{
			public:

				struct VendorInfo
				{
					String database_compound_ID;
					String vendor_name;
					String vendors_compound_ID;
					double molecular_weight;
				};

				// static function for creating a new database
				static void createNewDatabase(String& host, String& user, String& password, String& db_name, int port=0);

				/** Get version number of MolDB.
				*/
				int getVersion();

				/** Create MolDB instance with database connection parameters.
				@param hostname
				@param username
				@param password
				@param dbname
				*/
				MolDB(String hostname,String user, String password, String db, int port=3306);

				virtual	~MolDB();

				/** Get a summary of all datasets currently stored in the data base.
				@param dataset_names the name of each dataset will be stored here
				@param no_isomers the number of distinct isomers of each of the datasets
				@param no_conformations the number of distinct conformations of each of the datasets */
				void getDatasetSummary(vector<String>& dataset_names, vector<Size>& no_isomers, vector<Size>& no_conformations);

				void searchMoleculesByUCK(String uck, list<LongSize>& conformation_IDs);

				Molecule* fetchMoleculeById(LongSize conformation_id);

				vector<Molecule*> fetchMoleculeById(vector<LongSize>& conformation_ids);

				LongSize storeMolecule(Molecule *mol, String* conformation_hash);

				LongSize storeMolecule(Molecule *mol)
				{
					return storeMolecule(mol,0);
				};

				void deleteIsomer(LongSize isomer_id);

				void deleteConformation(LongSize conformation_id);

				void storeResult(Result* result, String& target_name);

				vector<Result*> fetchResults(list<String>& conformation_hashs, String target_name="");

				/** Filter database according to specified logP thresholds. */
				void searchMoleculesByLogP(const double& min_logP, const double& max_logP, list<LongSize>& conformation_IDs, const list<LongSize>* conformation_IDs_to_check, bool input_isomer_IDs=0, bool output_isomer_IDs=0);

				/** Filter database according to specified molecular weight thresholds. */
				void searchMoleculesByMW(const double& min_MW, const double& max_MW, list<LongSize>& conformation_IDs, const list<LongSize>* conformation_IDs_to_check, bool input_isomer_IDs, bool output_isomer_IDs);

				/** Fetch all conformation-IDs for a given target. */
				void fetchDatasetConformationIDs(String& target_name, list<LongSize>& conformation_IDs, const list<LongSize>* conformation_IDs_to_check=0, bool input_isomer_IDs=0, bool output_isomer_IDs=0);

				/** Fetch all conformation-IDs for those molecules that have a tanimoto coefficient larger or equal than the given cutoff to at least one of the query fingerprints. */
				void searchSimilarMoleculesByFingerprint(list<vector<bool> >& query_fingerprints, const float& minimal_similarity, const float& maximal_similarity, list<LongSize>& conformation_IDs, const list<LongSize>* conformation_IDs_to_check=0, bool input_isomer_IDs=0, bool output_isomer_IDs=0);

				#if defined BALL_HAS_OPENBABEL || defined BALL_HAS_OPENEYE
				/** Searches molecules by use of the specified SMARTS patterns. */
				void searchMoleculesBySmarts(const list<String>& smarts, list<LongSize>& conformation_IDs, const list<LongSize>* conformation_IDs_to_check=0, const list<Size>* no_of_matches=0, bool intersection=1, bool input_isomer_IDs=0, bool output_isomer_IDs=0);
				#endif

				/** Search for vendors for the given compound.
				@param mol input compound
				@param vendor_info vendor-information found for this compound will be stored here. */
				void fetchVendorInformation(Molecule* mol, list<VendorInfo>& vendor_info);

				/** Find out whether this database has a 'results' table or not.
				If it has such a table, the results of Docking/Scoring have been saved here. */
				bool hasResults();

				/** Disable creation/filling of table 'functional_groups' during molecule import. */
				void disableFunctionalGroupCounting();

				void setVendorPropertyNames(String vendor_name, String vendors_compound_id_tag, bool fetch_vendorname_from_prop, String vendor_version, String url);


			private:
				MolDB(const MolDB& /*tc*/){};

				static const Size VERSION;
				QSqlDatabase database_;
				String dbtype_;
				String hostname_;
				String user_;
				String password_;
				String db_name_;
				bool fg_counting_disabled_;

				bool save_vendor_information_;
				bool fetch_vendorname_from_prop_;
				String property_vendorname_;
				String vendorname_;
				String vendorversion_;
				String vendorurl_;
				String property_vendorID_;

				/** Filter the database using columns in the database table 'isomer'.\n
				@param prop_name the name of the column to be used */
				void searchMoleculesByIsomerColumn_(const String prop_name, const double& min_value, const double& max_value, list<LongSize>& conformation_IDs, const list<LongSize>* conformation_IDs_to_check, bool input_isomer_IDs, bool output_isomer_IDs);

				/** Reconstructs properties to the given Molecule from the database */
				void buildProperties(Molecule* mol, LongSize ID, list<String>& property_columns);

				MolecularSimilarity* molecular_similarity_;

				#if defined BALL_HAS_OPENBABEL || defined BALL_HAS_OPENEYE
					LogP logP_;
				#endif

				void bitvectorToBytearray_(const vector<bool>& bitvector, QByteArray& bytearray);

				void bytearrayToBitvector_(const QByteArray& bytearray, vector<bool>& bitvector);

			public:
				/** Reconstruct molecule topology from binary data.
							@param  target molecule for building topology
							@param binary data fetched from database
					*/
				static void buildTopology(Molecule& mol, QByteArray &data);

				/** Encode the molecule topology into binary data.
							@param  source molecule for extracting topology
							@param  binary data stream to output data
					*/
				static void getTopology(Molecule& mol, QDataStream &ds);

				/** Reconstruct molecule conformation from binary data.
							@param  target molecule for building conformation
							@param binary data fetched from database
					*/
				static void buildCoordinates(Molecule& mol, QByteArray &data);

				/** Encode the molecule conformation into binary data.
							@param  source molecule for extracting conformation
							@param  binary data stream to output data
					*/
				static void getCoordinates(Molecule& mol, QDataStream &ds);

				/** Reconstruct molecule charges from binary data.
							@param  target molecule for building charges
							@param binary data fetched from database
					*/
				static void buildCharges(Molecule& mol, QByteArray &data);

				/** Encode the molecule charge data into binary data.
							@param  source molecule for extracting charges
							@param  binary data stream to output data
					*/
				static void getCharges(Molecule& mol, QDataStream &ds);
		};
}

#endif /* MOLDB_H */

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_DBINTERFACE_H
#define BALL_FORMAT_DBINTERFACE_H

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/FORMAT/MOLFile.h>
#include <BALL/COMMON/exception.h>

#include <QSqlRecord>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtCore/QVariant>

namespace BALL
{

	/** A simple interface to a small molecule structure database.
			The database allows the storage of molecular structures in an SQL database.
			The structure includes the independent storage of the molecule's topology (i.e.
      atoms and bonds) and a number of geometries (conformations, i.e. atom coordinates).
			\par
			The interface has currently been tested with MySQL 4, however it should be easy to
			port it to other databases. In order to create a database, please have a look at
			source/APPLICATIONS/DB. There are SQL scripts provided to create an empty database
			as well as code examples importing structures into the database and extracting structures
			from there again.
			\par 
			The database interface also requires an activated database interface in QT.
			If you did not install the QT database drivers, please recompile QT with the appropriate
			options (see QT documentation for details). Using the database support without the
			enabling the visualization component of BALL is not possible at this time.
	*/
	class BALL_EXPORT DBInterface
	{
		public:
		
		class InvalidQuery
			:	public Exception::GeneralException
		{
			public:
			InvalidQuery(const char* file, int line, const String& s = "<unknown query>") ;
			virtual ~InvalidQuery() throw() {}

			protected:
			std::string query_;
		};

		class NotConnected
			:	public Exception::GeneralException
		{
			public:
			NotConnected(const char* file, int line, const String& s = "<not connected>") ;
			virtual ~NotConnected() throw() {}

			protected:
			std::string query_;
		};

		/// Exception thrown to indicate that the given topology and conformation are not consistent
		class InconsistentTopology
			:	public Exception::GeneralException
		{
			public:
			InconsistentTopology(const char* file, int line, const String& s = "<inconsistent topology>") ;
			virtual ~InconsistentTopology() throw() {}

			protected:
			std::string query_;
		};

		/**	@name Type definitions */
		//@{
			
		/// A database ID
		typedef qlonglong ID;

		/// A vector of database IDs
		typedef std::vector<ID> IDVector;

		/** Description of a method for conformation generation.
				First: name of the method used, second: the paramters used.
		*/
		typedef std::pair<String, String> ConformationMethod;

		/** Description of a method for charge generation.
				First: name of the method used, second: the paramters used.
		*/
		typedef std::pair<String, String> ChargeMethod;
		//@}

		/**	@name Constants */
		//@{

		/// Database status codes
		enum ErrorCodes
		{
			NO_ERROR,
			NO_CONNECTION // the database was not connected/initialized (good() = false)
		};

		/// The file containing the login credentials used by connect()
		static const String BALL_DEFAULT_DBRCFILE;
		/// The default database host
		static const String BALL_DEFAULT_DATABASE_HOST;
		/// The default database port
		static const Size   BALL_DEFAULT_DATABASE_PORT;
		/// The default database driver
		static const String BALL_DEFAULT_DATABASE_DRIVER;
		/// The default database name
		static const String BALL_DEFAULT_DATABASE_NAME;
		/// The section name for the INI file containing the login credentials
		static const String BALL_DEFAULT_DATABASE_SECTIONNAME;
		//@}
		

		/**	@name Constructors and destructors */
		//@{
		///
		DBInterface();
		///
		virtual ~DBInterface() {}
		//@}

		/**	@name Create and retrieve topologies and conformations */
		//@{
		/// Retrieve a molecular topology (no coordinates)
		void getTopology(ID topology, System& system);
		/** Create a new topology. If no ID is specified, it will create a new ID.	
				Otherwise, old data could be overwritten. In any case all associated 
				conformations are deleted as well for consistency reasons.
		*/
		ID newTopology(const System& system, const String& name, const String& source_id, ID id = 0);
		/// Replace an existing topology. This will remove all associated conformatins as well.
		void setTopology(ID topology, const System& system);

		/// Return IDs of all conformations for a given topology
		IDVector getConformationList(ID topology_od);
		/// Return IDs of all conformations for a given topology created with a specific method
		IDVector getConformationList(ID topology_id, ID method_id);
		/// Assign a specific conformation to an existing topology
		void loadConformation(const ID conformation, System& system);

		/// Store the current conformation with energy
		ID storeConformation(ID topology_id, ID method_id, const System& system,double energy);
		/// Store the current conformation 
		ID storeConformation(ID topology, ID method_ID, const System& system);

		/// Return IDs for all current methods for conformation generation
		IDVector getConformationMethods();
		/// Return name and parameters of a conformation generation method
		ConformationMethod getConformationMethod(ID method_id);
		/// Return name and parameters of a conformation generation method
		ID getConformationMethod(const String& method, const String& parameters);
		/// Create a new conformation generation method and return its database ID
		ID newConformationMethod(const String& method, const String& parameters);
				
		/// Store a set of assigned atom charges
		ID storeCharges(ID topology_id, ID method_id, const System& system);
		/// Assign a specific set of atom charges
		void loadCharges(const ID charge_id, System& system);
		/// 
		IDVector getChargeMethods();
		///
		ChargeMethod getChargeMethod(DBInterface::ID method_id);
		///
		ID getChargeMethod(const String& method, const String& parameters);
		///
		ID newChargeMethod(const String& method, const String& parameters);

		///
		ErrorCode getError() const { return error_; }
		///
		void setError(ErrorCode error) { error_ = error; }
		///
		bool good() const { return error_ == 0; }
		//@}

		/**	@name	Simplified database interface */
		//@{
		/** Connect to the database.
		Remember: it is not safe to store clear text passwords in your source code!
		Please use \link connect() connect() \endlink whenever possible.
		\p
		The method returns false, if the database connection could not be established.
		Inspecting query().lastError().text() will help you to identify potential problems.
		*/
		bool connect
			(const String& user, const String& password, 
			 const String& database_name = "structures", const String& host = "diclofenac.informatik.uni-tuebingen.de",
			 Size port = 3306, const String& driver = "QMYSQL3");

		/** Connect to the database using the default login settings.
				The default settings for a user are stored in $HOME/.ballrc.
				Make sure this file is readable for *YOU ONLY* (e.g. by "chmod 600 ~/.ballrc").
				The file adheres to the format definition of an \link INIFile INIFile \endlink.
				The keywords recognized can be seen from the example below:
				\verbatim
					[StructureDatabase]
					user=oliver
					password=mypassword
					database=STRUCTURES
					host=diclofenac.informatik.uni-tuebingen.de
					port=3306
					driver=QMYSQL3
				\verbatim
				Except for user and passwort the values shown will be used as default values,
				if the corresponding key is not given.
				The method returns false, if the database connection could not be established.
				Inspecting query().lastError().text() will help you to identify potential problems.
		*/
		bool connect();
		
		/** Execute a query
		 *  @ŧhrow InvalidQuery if the query was invalid
		 *  @throw NotConnected if the database connection is down
		 */
		QSqlQuery& executeQuery(const String& query_string);
		
		/** Execute a prepared query
		 *  @ŧhrow InvalidQuery if the query was invalid
		 *  @throw NotConnected if the database connection is down
		 */
		QSqlQuery& executeQuery();

		/// Return the internal query.
		QSqlQuery& query() { return *query_; }

		/// Return the (expanded) last query
		String executedQuery() { return query_->executedQuery().toStdString();}

		/// Skip to the first result of the last query.
		bool first() { return query_->first(); }

		/// Skip to the last result of the last query.
		bool last() { return query_->last(); }

		/// Skip to the next result of the last query (if it exists)
		bool next() { return query_->next(); }

		/** Return a vector of IDs, if the last query produced just IDs.
				This will call next() until all records have been extracted.
		*/
		IDVector extractIDs();

		/// Skip to the previous result of the last query (if it exists)
		bool prev() { return query_->previous(); }

		/// Return the last database ID created by an insert statement
		ID lastInsertedID();
		
		/// Return the number of rows returned by the last query
		Size size() { return (Size)query_->size(); }

		/// Return the k-th column value of the current result row
		QVariant value(Position k) { return query_->value(k); }

		/// Prepare a query
		void prepare(const String& s) { query_->prepare(s.c_str()); }

		/// Add bound parameters to the query (replaces ? in a prepared query)
		void addBindValue(const QVariant& v) { query_->addBindValue(v); }
		void addBindValue(const QVariant& v1, const QVariant& v2) { addBindValue(v1); addBindValue(v2);}
		void addBindValue(const QVariant& v1, const QVariant& v2, const QVariant& v3) { addBindValue(v1); addBindValue(v2); addBindValue(v3);}
		void addBindValue(const QVariant& v1, const QVariant& v2, const QVariant& v3, const QVariant& v4) { addBindValue(v1); addBindValue(v2); addBindValue(v3); addBindValue(v4);}
		//@}

		/**	@name Obsolete stuff -- to be removed asap */
		//@{
		/// Retrieve the structure as a single MOLFile (if stored) from the old fingerprints table
		bool getMOLFile(ID id, String& file_text, String& name, String& source_id);
		/// Add the structure from a MOLFile contained in a string to a System
		void addMOLFileToSystem(const String& molfile, System& system);
		//@}

		protected:	

		/// Assign the coordinates from the XDR-encoded bytearray to the system
		static void assignCoordinates_(System& system, const QByteArray& data);
		/// Extract the coordinates from the system and XDR-encode them as a byte array.
		static void extractCoordinates_(const System& system, QByteArray& data);
		/// Assign the charges from the XDR-encoded bytearray to the system
		static void assignCharges_(System& system, const QByteArray& data);
		/// Extract the coordinates from the system and XDR-encode them as a byte array.
		static void extractCharges_(const System& system, QByteArray& data);
		///
		static void encodeArray_(const std::vector<float>& v, QByteArray& a);
		///
		static void decodeArray_(const QByteArray& a, std::vector<float>& v);

		// Database conection
		ErrorCode				error_;
		///
		QSqlDatabase		db_;
		///
		QSqlQuery*			query_;
		
		// Connection details
		String					database_name_;
		Size						port_;
		String					host_;
		String					username_;
		String					password_;

		private:
		// No copy construction allowed. Don't know what QSqlDatabase would
		// do in that case. It can't be copied anyhow...
		DBInterface(const DBInterface& /* interface */) {}
	};

} // namespace BALL
#endif // BALL_FORMAT_DBINTERFACE_H

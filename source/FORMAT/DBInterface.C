// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/DBInterface.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/FORMAT/MOLFile.h>
#include <BALL/FORMAT/INIFile.h>

// Required for XDR encoding of types and charges
#include <rpc/types.h>
#include <rpc/xdr.h>

#include <sstream>
#include <iostream>

#include <QSqlError>

// #define BALL_DEBUG_DBINTERFACE 1

using namespace std;

namespace BALL 
{

	const String DBInterface::BALL_DEFAULT_DBRCFILE = String("~") + FileSystem::PATH_SEPARATOR + ".ballrc";
	const String DBInterface::BALL_DEFAULT_DATABASE_HOST = "diclofenac.informatik.uni-tuebingen.de";
	const Size   DBInterface::BALL_DEFAULT_DATABASE_PORT = 3306;
	const String DBInterface::BALL_DEFAULT_DATABASE_DRIVER = "QMYSQL3";
	const String DBInterface::BALL_DEFAULT_DATABASE_NAME = "structures";
	const String DBInterface::BALL_DEFAULT_DATABASE_SECTIONNAME = "StructureDatabase";

	DBInterface::InvalidQuery::InvalidQuery(const char* file, int line, const String& s) 
		:	Exception::GeneralException(file, line, "DBInterface::InvalidQuery", ""),
			query_(s)										
	{
		message_ = "DBInterface: invalid query '";
		message_.append(s + "'.");

		Exception::globalHandler.setMessage(message_);
	}

	DBInterface::NotConnected::NotConnected(const char* file, int line, const String& s) 
		:	Exception::GeneralException(file, line, "DBInterface::NotConnected", ""),
			query_(s)										
	{
		message_ = "DBInterface: not connected to a database '";
		message_.append(s + "'.");

		Exception::globalHandler.setMessage(message_);
	}

	DBInterface::InconsistentTopology::InconsistentTopology(const char* file, int line, const String& s) 
		:	Exception::GeneralException(file, line, "DBInterface::InconsistentTopology", ""),
			query_(s)										
	{
		message_ = "DBInterface: topology and geometry are not consistent with each other '";
		message_.append(s + "'.");

		Exception::globalHandler.setMessage(message_);
	}

	DBInterface::DBInterface()
		:	error_(DBInterface::NO_ERROR),	
			db_()
	{
	}


	DBInterface::IDVector DBInterface::extractIDs()
	{
		// Walk over all results and return a vector of IDs.
		IDVector ids;
		while (next())
		{
			ids.push_back(value(0).toInt());
		}
		return ids;
	}

	bool DBInterface::getMOLFile(DBInterface::ID id, String& text, String& name, String& source_id)
	{
		if (!good())
		{
			return false;
		}
		executeQuery("SELECT struc, ID, name FROM fingerprints WHERE structure_id = " + String(long(id)));
		if (size() < 1)
		{
			throw DBInterface::InvalidQuery(__FILE__, __LINE__, "unknown structure id " + String((long)id));
		}
		first();
		text = value(0).toString().toStdString();

		source_id = (value(1).isNull() ? String("") : String(value(1).toString().toStdString()));
		name = (value(2).isNull() ? String("") : String(value(2).toString().toStdString()));
		
		return true;
	}

	void DBInterface::addMOLFileToSystem(const String& molfile, System& system)
	{
		// Write the string to a temporary file.
		String file;
		File::createTemporaryFilename(file);
		ofstream os(file.c_str(), std::ios::out);
		os << molfile;
		os.close();

		// Read this file and create a molecule.
		Molecule* m = 0;
		try
		{
			MOLFile mf(file);
			m = mf.read();
			mf.close();
		}
		catch (...) { m = 0; }

		// If we were able to read a molecule, insert it into the system;
		if (m != 0)
		{
			system.insert(*m);
		}
		File::remove(file);
	}

	QSqlQuery& DBInterface::executeQuery(const String& query_string) 
	{
		prepare(query_string);
		return executeQuery();
	}



	QSqlQuery& DBInterface::executeQuery() 
	{
		//check if there is a connection active
		if (!db_.isOpen())
		{
			throw NotConnected(__FILE__,__LINE__);
		}
		#ifdef BALL_DEBUG_DBINTERFACE
			std::cout << "executing query " << query_->lastQuery() << std::endl;
		#endif
		if (!query_->exec())
		{
			#ifdef BALL_DEBUG_DBINTERFACE
				std::cout << "query failed: " << executedQuery() << std::endl;
				std::cout << "  error text: " << query_->lastError().text().ascii() << std::endl;
			#endif
			throw InvalidQuery(__FILE__, __LINE__, String(query_->lastError().text().toStdString()) + " for query " + executedQuery());
		}
		#ifdef BALL_DEBUG_DBINTERFACE
			std::cout << "query was executed: " << executedQuery() << std::endl;
			std::cout << "  result size: " << size() << std::endl;
		#endif
		return *query_;
	}

	DBInterface::ID DBInterface::lastInsertedID()
	{
		executeQuery("SELECT LAST_INSERT_ID()");
		first();
		#ifdef BALL_DEBUG_DBINTERFACE
			std::cout << "lastInsertedID() = " << ID(value(0).toInt()) << std::endl;
		#endif
		return ID(value(0).toInt());
	}

	bool DBInterface::connect
		(const String& user, const String& password, const String& db, 
		 const String& host, Size port, const String& driver) 
	{
		database_name_ = db;

		#ifdef BALL_DEBUG_DBINTERFACE
			Log.info() << "Connecting to " << db << ":" << user << "@" << host << ":" << port << " through " << driver << std::endl;
		#endif

		db_ = QSqlDatabase::addDatabase(QString(driver.c_str()));
		db_.setHostName(host.c_str());
		db_.setUserName(user.c_str());
		db_.setDatabaseName(db.c_str());
		db_.setPassword(password.c_str());
		db_.setPort(port);

		if (!db_.open())
		{
			setError(NO_CONNECTION);
			return false;
		}
		else
		{
			setError(NO_ERROR);
			query_ = new QSqlQuery();
		}
		return true;
	}

	bool DBInterface::connect()
	{
		String filename = BALL_DEFAULT_DBRCFILE;
		FileSystem::canonizePath(filename);
		if (File::isReadable(filename))
		{
			INIFile rc(filename);
			rc.read();
			String user = "";
			String password = "";
			String host = BALL_DEFAULT_DATABASE_HOST;
			String database =  BALL_DEFAULT_DATABASE_NAME;
			Size port = BALL_DEFAULT_DATABASE_PORT;
			String driver = BALL_DEFAULT_DATABASE_DRIVER;
			if (rc.hasEntry(BALL_DEFAULT_DATABASE_SECTIONNAME, "user")
					&& rc.hasEntry(BALL_DEFAULT_DATABASE_SECTIONNAME, "password"))
			{
				user = rc.getValue(BALL_DEFAULT_DATABASE_SECTIONNAME, "user");
				password = rc.getValue(BALL_DEFAULT_DATABASE_SECTIONNAME, "password");
			}
			else	
			{
				// user/password entries are mandatory -- exit if they are not given
				return false;
			}
			if (rc.hasEntry(BALL_DEFAULT_DATABASE_SECTIONNAME, "host"))
			{
				host = rc.getValue(BALL_DEFAULT_DATABASE_SECTIONNAME, "host");
			}
			if (rc.hasEntry(BALL_DEFAULT_DATABASE_SECTIONNAME, "database"))
			{
				database = rc.getValue(BALL_DEFAULT_DATABASE_SECTIONNAME, "database");
			}
			if (rc.hasEntry(BALL_DEFAULT_DATABASE_SECTIONNAME, "driver"))
			{
				driver = rc.getValue(BALL_DEFAULT_DATABASE_SECTIONNAME, "driver");
			}
			if (rc.hasEntry(BALL_DEFAULT_DATABASE_SECTIONNAME, "port"))
			{
				port = (Size)rc.getValue(BALL_DEFAULT_DATABASE_SECTIONNAME, "port").toInt();
			}
			return connect(user, password, database, host, port, driver);
		}
		Log.error() << "Could not open database setup file at " << filename << std::endl;
		return false;
	}

	void DBInterface::getTopology(DBInterface::ID topology_id, System& system)
	{
		// We need to be connected to a database.
		if (!good())
		{
			throw DBInterface::NotConnected(__FILE__, __LINE__);
		}
		
		// Retrieve the name and source_id of the structure
		prepare("SELECT name, source_id FROM topology WHERE topology_id = ?");
		addBindValue(topology_id);
		executeQuery();
		
		// If we could not retrieve even a single result row, the ID was incorrect.
		if (size() == 0)
		{
			throw InvalidQuery(__FILE__, __LINE__, String("no such topology: #") + String((long)topology_id));
		}

		// Otherwise, remember name and source ID of the structure.
		first();
		String source_id = value(1).toString().toStdString();
		String name = value(0).toString().toStdString();

		// Retrieve all atoms belonging to this structure
		executeQuery("SELECT atom_number, element, formal_charge, name "
									"  FROM connection_table_atoms"
									"  WHERE topology_id = " + String(long(topology_id)));

		if (size() < 1)
		{
			throw DBInterface::InvalidQuery(__FILE__, __LINE__, "unknown topology id " + String((long)topology_id));
		}

		// Reserve an empty vector for the atoms,
		// then iterate over all row and create the atoms.
		std::vector<Atom*> atoms(size());
		Index i = 0;
		while (next()) 
		{
			atoms[i] = new Atom;
			atoms[i]->setElement(PTE[value(1).toString().toStdString()]);
			atoms[i]->setFormalCharge(value(2).toInt());
			atoms[i]->setName(value(3).toString().toStdString());
			i++;
		}
		
		// Retrieve all bonds belonging to this topology
		executeQuery("SELECT atom1, atom2, bond_order, stereo "
									"  FROM connection_table_bonds"
									"  WHERE topology_id = " + String(long(topology_id)));
		
		// Iterate over all records and create the bonds
		while (next())
		{
			Position atom1 = value(0).toInt();
			Position atom2 = value(1).toInt();
			Position order = value(2).toInt();
			Index stereo = value(3).toInt();
			if (atom1 < atoms.size() && atom2 < atoms.size())
			{
				Bond* bond = atoms[atom1]->createBond(*atoms[atom2]);
				bond->setOrder(order);
				if (stereo != 0)
				{
					bond->setProperty(MOLFile::Property::BOND_STEREO, stereo);
				}
			}
		}

		// Create a molecule and insert the atoms.
		Molecule* m = new Molecule;
		for (Position i = 0; i < atoms.size(); m->insert(*atoms[i++])) {};
		atoms.clear();

		// Remove old crap from the system and insert the new structure instead.
		system.clear();
		system.insert(*m);
		m->setName(name);
		m->setProperty("SOURCE_ID", source_id);
	}

	DBInterface::ID DBInterface::newTopology
		(const System& system, const String& name, const String& source_id, DBInterface::ID id)
	{
		ID topo_id = id;
		if (id == 0)
		{
			// Create a new ID for this topology.
			prepare("INSERT INTO topology (name, source_id) VALUES(?, ?)");
			addBindValue(name.c_str(), source_id.c_str());
			executeQuery();
			topo_id = lastInsertedID();
		}
		else
		{
			// Insert it with a given ID in the table and overwrite existing content.
			prepare("REPLACE INTO topology (name, source_id, topology_id) VALUES(?, ?, ?)");
			addBindValue(name.c_str(), source_id.c_str(), id);
			executeQuery();
		}
		setTopology(id, system);
		
		return topo_id;
	}

	void DBInterface::setTopology(DBInterface::ID id, const System& system)
	{
		// delete atom_coord first -----
		prepare("SELECT id FROM conformations WHERE topology_id = ?");
		addBindValue(id);
		executeQuery();

		// Remove all older conformations for consistency (the number/order of atoms might have changed!)
		prepare("DELETE FROM conformations WHERE topology_id = ?");
		addBindValue(id);
		executeQuery();

  
/*
		// same for all charges -------
		prepare("SELECT id FROM molecule_charge WHERE topology_id = ?");
		addBindValue(id);
		executeQuery();
  
		ids = extractIDs();

		for (Position i = 0; i < ids.size(); ++i)
    {
			prepare(string("DELETE FROM atom_charge WHERE molecule_charge_id = ?") );
			addBindValue(ids[i]);
      executeQuery();
    }
  

		prepare("DELETE FROM molecule_charge WHERE topology_id = ?");
		addBindValue(id);
		executeQuery();
*/

		//-----------------------------
		prepare("DELETE FROM connection_table_atoms WHERE topology_id = ?");
		addBindValue(id);
		executeQuery();

		prepare("DELETE FROM connection_table_bonds WHERE topology_id = ?");
		addBindValue(id);
		executeQuery();

		// Add new entries for the connection table.
		Size idx = 0;
		HashMap<const Atom*, Position> atom_to_idx;
		for (AtomConstIterator ai = system.beginAtom(); +ai; ++ai, ++idx)
		{
			// Remember the number of the atom
			atom_to_idx[&*ai] = idx;
			prepare("INSERT INTO connection_table_atoms "
											"  (topology_id, atom_number, element, formal_charge, name)"
											"  VALUES(?, ?, ?, ?, ?)");
			addBindValue(id);
			addBindValue(idx);
			addBindValue(ai->getElement().getSymbol().c_str());
			addBindValue(ai->getFormalCharge());
			addBindValue(ai->getName().c_str());
			executeQuery();
		}
		AtomConstIterator ai;
		Atom::BondConstIterator bi;
		Size bond_idx = 0;
		BALL_FOREACH_BOND(system, ai, bi)
		{
			// Look up the indices of the atoms involved
			if (atom_to_idx.has(bi->getFirstAtom()) && atom_to_idx.has(bi->getSecondAtom()))
			{
				prepare("INSERT INTO connection_table_bonds "
												"  (topology_id, bond_number, atom1, atom2, bond_order, stereo)"
												"  VALUES(?, ?, ?, ?, ?, ?)");
				addBindValue(id);
				addBindValue(bond_idx++);
				addBindValue(atom_to_idx[bi->getFirstAtom()]);
				addBindValue(atom_to_idx[bi->getSecondAtom()]);
				addBindValue(bi->getOrder());
				addBindValue(bi->hasProperty(MOLFile::Property::BOND_STEREO) 
														 ? bi->getProperty(MOLFile::Property::BOND_STEREO).getInt() : 0);
				executeQuery();
			}
		}
	}

	/// Return IDs of all conformations for a given topology
	DBInterface::IDVector DBInterface::getConformationList(DBInterface::ID topology_id)
	{
		prepare("SELECT id FROM conformations WHERE topology_id = ?");
		addBindValue(topology_id);
		executeQuery();
		return extractIDs();
	}

	/// Return IDs of all conformations created with a given method
	DBInterface::IDVector DBInterface::getConformationList(DBInterface::ID topology_id, DBInterface::ID method_id)
	{
		prepare("SELECT id FROM conformations WHERE topology_id = ? AND method_id = ?");
		addBindValue(topology_id, method_id);
		executeQuery();
		return extractIDs();
	}

	/// Assign a specific conformation to an existing topology
	void DBInterface::loadConformation(DBInterface::ID conformation_id, System& system)
	{
		executeQuery("SELECT coordinates FROM conformations WHERE id = " + String((long)conformation_id));
		if (size() != 1)
		{
			throw InvalidQuery(__FILE__, __LINE__, String("conformation does not exist: #") + String((long)conformation_id));
		}

		// Extract the coordinates from the XDR-formatted BLOB and assign them to
		// the atoms. If the number of coordinates stored in the BLOB deviates
		// from the number of atoms in the system, throw InconsistentTopology
		first();
		assignCoordinates_(system, value(0).toByteArray());
	}

	void DBInterface::assignCoordinates_(System& system, const QByteArray& data)	
	{
		// Decode the XDR data obtained from the ByteArray
		static std::vector<float> v;	
		decodeArray_(data, v);

		// Compare the number of floats to the number of coordinates required.
		if (v.size() != (3 * system.countAtoms()))
		{
			throw InconsistentTopology(__FILE__, __LINE__, 
						String("conformation and topology are not consistent for conformation. ")
						+ String("Number of atoms in conformation/system: ")
						+ String(v.size() / 3) + "/" + String(system.countAtoms()));
		}
		
		// Walk over the atoms of the system and assign the coordinates from the 
		// coordinate vector.
		AtomIterator ai(system.beginAtom());
		for (Position i = 0; i < (v.size() / 3); ++i)
		{
			ai->setPosition(Vector3(v[i], v[i + 1], v[i + 2]));
		}
	}

	void DBInterface::assignCharges_(System& system, const QByteArray& data)	
	{
		// Decode the XDR data obtained from the ByteArray
		static std::vector<float> v;	
		decodeArray_(data, v);

		// Compare the number of floats to the number of coordinates required.
		if (v.size() != system.countAtoms())
		{
			throw InconsistentTopology(__FILE__, __LINE__, 
						String("conformation and topology are not consistent for charge set. ")
						+ String("Number of atoms in charge set/system: ")
						+ String(v.size()) + "/" + String(system.countAtoms()));
		}
		
		// Walk over the atoms of the system and assign the coordinates from the 
		// coordinate vector.
		AtomIterator ai(system.beginAtom());
		for (Position i = 0; i < v.size(); ++i)
		{
			ai->setCharge(v[i]);
		}
	}

	void DBInterface::extractCharges_(const System& system, QByteArray& data)	
	{
		// Copy all charges into a vector.
		std::vector<float> v;
		for (AtomConstIterator ai(system.beginAtom()); +ai; ++ai)
		{
			v.push_back(ai->getCharge());
		}

		// Encode these floats into an XDR blob in data.
		encodeArray_(v, data);
	}

	void DBInterface::decodeArray_(const QByteArray& a, std::vector<float>& v)
	{
		// Create an XDR object ready to *decode* from a memory block (data).
		XDR xdr;
		xdrmem_create(&xdr, const_cast<char*>(a.data()), a.size(), XDR_DECODE);

		// Determine the number of atoms in the system and make sure the
		// number of coordinates stored in the XDR blob is the same.
		Size number_of_entries = 0;
		if (xdr_u_int(&xdr, &number_of_entries) != 1)
		{
			xdr_destroy(&xdr);
			throw InconsistentTopology(__FILE__, __LINE__, 
						String("could not decode number of array entries."));
																
		}

		// Extract the floats from the ByteArray
		v.resize(number_of_entries);
		for (Position i = 0; i < number_of_entries; ++i)
		{
			xdr_float(&xdr, &(v[i]));
		}

		// Free the private datastructures of the XDR struct.
		xdr_destroy(&xdr);
	}

	void DBInterface::extractCoordinates_(const System& system, QByteArray& data)	
	{
		static std::vector<float> v;
		v.resize(system.countAtoms() * 3);
		Size number_of_atoms(system.countAtoms());

		// For each atom, store the x, y, and z coordinates.
		AtomConstIterator ai(system.beginAtom());
		for (Position i = 0; i < 3 * number_of_atoms; i += 3, ++ai)
		{
			const Vector3& r(ai->getPosition());
			v[i] = r.x;
			v[i + 1] = r.y;
			v[i + 2] = r.z;
		}
		
		// Encode the coordinate data as XDR.
		encodeArray_(v, data);
	}

	void DBInterface::encodeArray_(const std::vector<float>& v, QByteArray& a)
	{
		// Determine the amount of space we need and resize the byte array accordingly.
		// Global layout: int (number of entries), number_of_entries * float (data)
		a.resize(4 + 4 * v.size());
	
		// Create an XDR struct ready for writing into the byte array.
		XDR xdr;
		xdrmem_create(&xdr, a.data(), a.size(), XDR_ENCODE);
		
		// Store the number of entries in the XDR blob.
		Size s = v.size();
		xdr_u_int(&xdr, &s);

		for (Position i = 0; i < v.size(); ++i)
		{
			xdr_float(&xdr, const_cast<float*>(&v[i]));
		}

		// Free the internal XDR data structures.
		xdr_destroy(&xdr);
	}

	/// Store the current conformation 
	DBInterface::ID DBInterface::storeConformation
		(DBInterface::ID topology_id, DBInterface::ID method_id, const System& system)
	{
		// Sanity check: make sure the number of atoms/bonds is consistent with the topology.
		Index atom_count = system.countAtoms();
		executeQuery("SELECT count(*) FROM connection_table_atoms WHERE topology_id = " + String((long)topology_id));
		first();
		if (atom_count != value(0).toInt())
		{
			throw InconsistentTopology(__FILE__, __LINE__, "found " + String(value(0).toInt())
																 + " atoms instead of " + String(atom_count) + " in system.");
		}

		// Extract the coordinate from the system and encode them into an XDR-formatted BLOB.
		QByteArray buffer;
		extractCoordinates_(system, buffer);

		// Create a new conformation for the given topology/method.
		prepare("INSERT INTO conformations (topology_id, method_id, coordinates) VALUES (?, ?, ?)");
		addBindValue(topology_id);
		addBindValue(method_id);
		addBindValue(QVariant(buffer));
		executeQuery();

		return lastInsertedID();
	}


	/// Store a charge set
	DBInterface::ID DBInterface::storeCharges
		(DBInterface::ID topology_id, DBInterface::ID method_id, const System& system)
	{
		// Sanity check: make sure the number of atoms/bonds is consistent with the topology.
		Index atom_count = system.countAtoms();
		executeQuery("SELECT count(*) FROM connection_table_atoms WHERE topology_id = " + String((long)topology_id));
		first();
		if (atom_count != value(0).toInt())
		{
			throw InconsistentTopology(__FILE__, __LINE__, "found " + String(value(0).toInt())
																 + " atoms instead of " + String(atom_count) + " in system.");
		}

		// Extract the coordinate from the system and encode them into an XDR-formatted BLOB.
		QByteArray buffer;
		extractCharges_(system, buffer);

		// Create a new conformation for the given topology/method.
		prepare("INSERT INTO charges (topology_id, method_id, charges) VALUES (?, ?, ?)");
		addBindValue(topology_id);
		addBindValue(method_id);
		addBindValue(QVariant(buffer));
		executeQuery();

		return lastInsertedID();
	}

	void DBInterface::loadCharges(DBInterface::ID charge_id, System& system)
	{
		executeQuery("SELECT coordinates FROM conformations WHERE id = " + String((long)charge_id));
		if (size() != 1)
		{
			throw InvalidQuery(__FILE__, __LINE__, String("charge set does not exist: #") + String((long)charge_id));
		}

		// Extract the charges from the XDR-formatted BLOB and assign them to
		// the atoms. If the number of charges stored in the BLOB deviates
		// from the number of atoms in the system, throw InconsistentTopology
		first();
		assignCharges_(system, value(0).toByteArray());
	}


	/// Return IDs for all current methods for conformation generation
	DBInterface::IDVector DBInterface::getConformationMethods()
	{
		// Extract all method IDs from the corresponding table...
		executeQuery("SELECT method_id FROM conformation_generation");
		return extractIDs();
	}

	/// Return IDs for all current methods for charge generation
	DBInterface::IDVector DBInterface::getChargeMethods()
	{
		// Extract all method IDs from the corresponding table...
		executeQuery("SELECT method_id FROM charge_generation");
		return extractIDs();
	}



	/// Return name and parameters of a conformation generation method
	DBInterface::ConformationMethod DBInterface::getConformationMethod(DBInterface::ID method_id)
	{
		executeQuery("SELECT method, parameters FROM conformation_generation WHERE method_id = " + String((long)method_id));
		if (size() > 0)
		{
			first();
			return ConformationMethod(value(0).toString().toStdString(), value(1).toString().toStdString());
		}
		else
		{
			return ConformationMethod("", "");
		}
	}

	/// Return name and parameters of a charge generation method
	DBInterface::ChargeMethod DBInterface::getChargeMethod(DBInterface::ID method_id)
	{
		executeQuery("SELECT method, parameters FROM charge_generation WHERE method_id = " + String((long)method_id));
		if (size() > 0)
		{
			first();
			return ChargeMethod(value(0).toString().toStdString(), value(1).toString().toStdString());
		}
		else 
		{
			return ChargeMethod("", "");
		}
	}


	/// Return name and parameters of a conformation generation method
	DBInterface::ID DBInterface::getConformationMethod(const String& method, const String& parameters)
	{
		prepare("SELECT method_id FROM conformation_generation WHERE method = ? AND parameters = ?");
		addBindValue(method.c_str());
		addBindValue(parameters.c_str());
		executeQuery();
		if (size() != 1)
		{
			std::cout << "result size: " << size() << std::endl;
			std::cout << "query:       " << executedQuery() << std::endl;
			throw InvalidQuery(__FILE__, __LINE__, "no such method: " + method + "/" + parameters);
		}
		first();
		return (ID)(value(0).toInt());
	}

	/// Return name and parameters of a charge generation method
	DBInterface::ID DBInterface::getChargeMethod(const String& method, const String& parameters)
	{
		prepare("SELECT method_id FROM charge_generation WHERE method = ? AND parameters = ?");
		addBindValue(method.c_str());
		addBindValue(parameters.c_str());
		executeQuery();
		if (size() != 1)
    {
      std::cout << "result size: " << size() << std::endl;
      std::cout << "query:       " << executedQuery() << std::endl;
      throw InvalidQuery(__FILE__, __LINE__, "no such method: " + method + "/" + parameters);
    }
		first();
		return (ID)(value(0).toInt());
	}

	/// Create a new conformation generation method and return its database ID
	DBInterface::ID DBInterface::newConformationMethod(const String& method, const String& parameters)
	{
		ID id = 0;
		try
		{
			// Check whether we have an ID for this method already
			id = getConformationMethod(method, parameters);
		}
		catch (InvalidQuery&)
		{
			// Otherwise, insert a new method into the method generation table.
			prepare("INSERT INTO conformation_generation (method, parameters) VALUES (?, ?)");
			addBindValue(method.c_str());
			addBindValue(parameters.c_str());
			executeQuery();
			id = lastInsertedID();
		}
		
		// Return either the old or the new ID.
		return id;
	}
	

	/// Create a new charge generation method and return its database ID
	DBInterface::ID DBInterface::newChargeMethod(const String& method, const String& parameters)
	{
		ID id = 0;
		try
    {
      // Check whether we have an ID for this method already
      id = getChargeMethod(method, parameters);
    }
		catch (InvalidQuery&)
    {
      // Otherwise, insert a new method into the method generation table.
      prepare("INSERT INTO charge_generation (method, parameters) VALUES (?, ?)");
      addBindValue(method.c_str());
      addBindValue(parameters.c_str());
      executeQuery();
      id = lastInsertedID();
    }
  
		// Return either the old or the new ID.
		return id;
	}

} // namespace BALL

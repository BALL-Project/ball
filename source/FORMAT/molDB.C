// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann, Marc Röttig $
// ----------------------------------------------------

#include <BALL/FORMAT/molDB.h>
#include <BALL/DOCKING/COMMON/conformation.h>
#include <BALL/STRUCTURE/UCK.h>

using namespace std;

namespace BALL
{
		// adjust this value if the format of MolDB (layout of mysql-tables, etc.) is changed
		const Size MolDB::VERSION = 1;

		MolDB::MolDB(String hostname,String user, String password, String db, int port)
		{

			molecular_similarity_ = new MolecularSimilarity("fragments/functionalGroups.smarts");

			dbtype_ = "QMYSQL";
			hostname_ = hostname;
			user_ = user;
			password_ = password;
			db_name_ = db;
			save_vendor_information_ = 0;
			vendorname_ = "";
			vendorversion_ = "";
			vendorurl_ = "";
			property_vendorname_ = "";
			property_vendorID_ = "";
			fg_counting_disabled_ = 0;

			database_ = QSqlDatabase::addDatabase(dbtype_.c_str());
			database_.setHostName(hostname_.c_str());
			database_.setDatabaseName(db_name_.c_str());
			database_.setUserName(user_.c_str());
			database_.setPassword(password_.c_str());
			database_.setPort(port);
			if(!database_.open())
			{
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDBConnector connect-error","could not connect to database "+hostname_+"/"+db_name_));
			}
		}

		MolDB::~MolDB()
		{
			delete molecular_similarity_;
		}


		void MolDB::getDatasetSummary(vector<String>& dataset_names, vector<Size>& no_isomers, vector<Size>& no_conformations)
		{
			dataset_names.clear();
			no_isomers.clear();
			no_conformations.clear();

			QSqlQuery query(database_);
			String q = "select distinct target_name from methods";
			bool status = query.exec(q.c_str());
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::getDatasetSummary","query for fetching dataset names failed due to : "+error));
			}

			Size no_datasets = query.size();
			dataset_names.reserve(no_datasets+1);
			no_isomers.reserve(no_datasets+1);
			no_conformations.reserve(no_datasets+1);

			QSqlQuery query2(database_);
			while(query.next())
			{
				String name = query.value(0).toString().toStdString();
				q = "select count(distinct c.isomer_id), count(distinct c.conformation_id) from conformation as c join results as r on r.output_conf_id=c.conformation_id join methods as m on m.method_id=r.method_id where m.target_name=\""+name+"\"";
				status = query2.exec(q.c_str());
				if(!status || !query2.next())
				{
					String error(query2.lastError().databaseText().toAscii().constData());
					throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::getDatasetSummary","query for fetching isomer/conformation counts failed due to : "+error));
				}
				Size isomers = query2.value(0).toInt();
				Size conf = query2.value(1).toInt();

				dataset_names.push_back(name);
				no_isomers.push_back(isomers);
				no_conformations.push_back(conf);
			}

			q = "select count(distinct isomer_id) from isomer";
			status = (query.exec(q.c_str()) && query.next());
			dataset_names.push_back("TOTAL");
			no_isomers.push_back(query.value(0).toInt());
			q = "select count(distinct conformation_id) from conformation";
			status = (query.exec(q.c_str()) && query.next());
			no_conformations.push_back(query.value(0).toInt());
		}


		void MolDB::searchMoleculesByLogP(const double& min_logP, const double& max_logP, list<LongSize>& conformation_IDs, const list<LongSize>* conformation_IDs_to_check, bool input_isomer_IDs, bool output_isomer_IDs)
		{
			searchMoleculesByIsomerColumn_("logP",min_logP,max_logP, conformation_IDs, conformation_IDs_to_check, input_isomer_IDs, output_isomer_IDs);
		}

		void MolDB::searchMoleculesByMW(const double& min_MW, const double& max_MW, list<LongSize>& conformation_IDs, const list<LongSize>* conformation_IDs_to_check, bool input_isomer_IDs, bool output_isomer_IDs)
		{
			searchMoleculesByIsomerColumn_("MW",min_MW,max_MW, conformation_IDs, conformation_IDs_to_check, input_isomer_IDs, output_isomer_IDs);
		}


		void MolDB::searchMoleculesByIsomerColumn_(const String prop_name, const double& min_value, const double& max_value, list<LongSize>& conformation_IDs, const list<LongSize>* conformation_IDs_to_check, bool input_isomer_IDs, bool output_isomer_IDs)
		{
			conformation_IDs.clear();

			// prevent MySql max_allowed_packet error
			if(conformation_IDs_to_check && conformation_IDs_to_check->size()>50000)
			{
				Size i=1;
				list<LongSize> batch_conformation_IDs_to_check;
				for(list<LongSize>::const_iterator it=conformation_IDs_to_check->begin(); it!=conformation_IDs_to_check->end(); i++)
				{
					batch_conformation_IDs_to_check.push_back(*it);
					it++;
					if(i%50000==0 || it==conformation_IDs_to_check->end())
					{
						list<LongSize> batch_found_IDs;

						searchMoleculesByLogP(min_value,max_value,batch_found_IDs, &batch_conformation_IDs_to_check,input_isomer_IDs,output_isomer_IDs);
						conformation_IDs.splice(conformation_IDs.end(),batch_found_IDs);
						batch_conformation_IDs_to_check.clear();
					}
				}
				return;
			}

			QSqlQuery query(database_);
			String q;
			if(output_isomer_IDs && (!conformation_IDs_to_check || input_isomer_IDs))
			{
				q = "select isomer_id from isomer where "+prop_name+">="+String(min_value)+" and "+prop_name+"<="+String(max_value);
				if(conformation_IDs_to_check)
				{
					q+=" and isomer_id IN (";
					for(list<LongSize>::const_iterator it=conformation_IDs_to_check->begin(); it!=conformation_IDs_to_check->end(); )
					{
						q+=String(*it);
						it++;
						if(it!=conformation_IDs_to_check->end()) q+=",";
					}
					q+=")";
				}
			}
			else
			{
				String s1 = "distinct i.isomer_id";
				String s2 = "c.conformation_id";
				if(!output_isomer_IDs) s1="c.conformation_id";
				if(input_isomer_IDs) s2="i.isomer_id";

				q = "select "+s1+" from isomer as i join conformation as c on i.isomer_id=c.isomer_id where "+prop_name+">="+String(min_value)+" and "+prop_name+"<="+String(max_value);
				if(conformation_IDs_to_check)
				{
					q+=" and "+s2+" IN(";
					for(list<LongSize>::const_iterator it=conformation_IDs_to_check->begin(); it!=conformation_IDs_to_check->end(); )
					{
						q+=String(*it);
						it++;
						if(it!=conformation_IDs_to_check->end()) q+=",";
					}
					q+=")";
				}
			}

			bool status = query.exec(q.c_str());
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::searchMoleculesByIsomerColumn_","query for searching molecules failed due to : "+error));
			}

			while(query.next())
			{
				conformation_IDs.push_back(query.value(0).toULongLong());
			}
		}


		void MolDB::fetchDatasetConformationIDs(String& target_name, list<LongSize>& conformation_IDs, const list<LongSize>* conformation_IDs_to_check, bool input_isomer_IDs, bool output_isomer_IDs)
		{
			conformation_IDs.clear();

			// prevent MySql max_allowed_packet error
			if(conformation_IDs_to_check && conformation_IDs_to_check->size()>50000)
			{
				Size i=1;
				list<LongSize> batch_conformation_IDs_to_check;
				for(list<LongSize>::const_iterator it=conformation_IDs_to_check->begin(); it!=conformation_IDs_to_check->end(); i++)
				{
					batch_conformation_IDs_to_check.push_back(*it);
					it++;
					if(i%50000==0 || it==conformation_IDs_to_check->end())
					{
						list<LongSize> batch_found_IDs;

						fetchDatasetConformationIDs(target_name,batch_found_IDs, &batch_conformation_IDs_to_check,output_isomer_IDs);
						conformation_IDs.splice(conformation_IDs.end(),batch_found_IDs);
						batch_conformation_IDs_to_check.clear();
					}
				}
				return;
			}

			QSqlQuery query(database_);
			String q;

			if(!output_isomer_IDs)
			{
				q = "select distinct c.conformation_id ";
			}
			else
			{
				q = "select distinct c.isomer_id ";
			}
			q+=" from conformation as c join results as r on r.output_conf_id=c.conformation_id join methods as m on m.method_id=r.method_id where m.target_name=\""+target_name+"\"";
			if(conformation_IDs_to_check)
			{
				String s = "c.conformation_id";
				if(input_isomer_IDs) s="c.isomer_id";
				q+=" and "+s+" IN (";
				for(list<LongSize>::const_iterator it=conformation_IDs_to_check->begin(); it!=conformation_IDs_to_check->end(); )
				{
					q+=String(*it);
					it++;
					if(it!=conformation_IDs_to_check->end()) q+=",";
				}
				q+=")";
			}

			q+=" order by c.isomer_id,m.method_id";
			bool status = query.exec(q.c_str());
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::fetchDatasetConformationIDs","query for fetching conformation IDs failed due to : "+error));
			}
			while(query.next())
			{
				conformation_IDs.push_back(query.value(0).toULongLong());
			}
		}


	#if defined BALL_HAS_OPENBABEL || defined BALL_HAS_OPENEYE
		void MolDB::searchMoleculesBySmarts(const list<String>& smarts, list<LongSize>& conformation_IDs, const list<LongSize>* conformation_IDs_to_check, const list<Size>* no_of_matches, bool intersection, bool input_isomer_IDs, bool output_isomer_IDs)
		{
			if(no_of_matches && no_of_matches->size()!=smarts.size())
			{
				throw Exception::GeneralException(__FILE__, __LINE__,"MolDB::searchMoleculesBySmarts","Lists containing SMARTS expressions and number of desired matches must have equal sizes!");
			}

			conformation_IDs.clear();
			QSqlQuery query(database_);

			// Do not extract millions of SMARTS at once, which might generate a memory overflow. Instead, make sure that this function is called recursively ...
			list<LongSize> tmp_conformation_IDs_to_check;
			if(!conformation_IDs_to_check)
			{
				String q ="select max(isomer_id)-min(isomer_id)+1 from isomer";
				query.exec(q.c_str());
				query.next();
				LongSize size = query.value(0).toULongLong();
				if(size>50000)
				{
					String q ="select isomer_id from isomer";
					query.exec(q.c_str());
					while(query.next())
					{
						tmp_conformation_IDs_to_check.push_back(query.value(0).toULongLong());
					}
					conformation_IDs_to_check = &tmp_conformation_IDs_to_check;
				}
			}

			// prevent MySql max_allowed_packet error
			if(conformation_IDs_to_check && conformation_IDs_to_check->size()>50000)
			{
				Size i=1;
				list<LongSize> batch_conformation_IDs_to_check;
				for(list<LongSize>::const_iterator it=conformation_IDs_to_check->begin(); it!=conformation_IDs_to_check->end(); i++)
				{
					batch_conformation_IDs_to_check.push_back(*it);
					it++;
					if(i%50000==0 || it==conformation_IDs_to_check->end())
					{
						list<LongSize> batch_found_IDs;

						searchMoleculesBySmarts(smarts,batch_found_IDs, &batch_conformation_IDs_to_check,no_of_matches,intersection,input_isomer_IDs,output_isomer_IDs);
						conformation_IDs.splice(conformation_IDs.end(),batch_found_IDs);
						batch_conformation_IDs_to_check.clear();
					}
				}
				return;
			}

			bool status;
			list<LongSize> isomer_IDs;

			// find isomer_ids to be checked
			if(!conformation_IDs_to_check)
			{
				status = query.exec("select isomer_id,usmile from isomer");
			}
			else
			{
				String q;
				if(!input_isomer_IDs) // conformation IDs are specified
				{
					q = "select distinct(i.isomer_id),usmile from isomer as i join conformation as c on i.isomer_id=c.isomer_id where c.conformation_id IN (";
				}
				else
				{
					q = "select isomer_id,usmile from isomer where isomer_id IN (";
				}
				for(list<LongSize>::const_iterator it=conformation_IDs_to_check->begin(); it!=conformation_IDs_to_check->end(); )
				{
					q+=String(*it);
					it++;
					if(it!=conformation_IDs_to_check->end()) q+=",";
				}
				q+=")";
				status = query.exec(q.c_str());
			}
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::searchMoleculesBySmarts","query for fetching isomer IDs failed due to : "+error));
			}

			Size no_hits=0;

			// now match SMARTS to all those molecules ...
			while(query.next())
			{
				QVariant qvar = query.value(1);
				String usmile = ((QString*)&qvar)->toStdString();

				bool hit=0;
				list<Size>::const_iterator it2;
				if(no_of_matches) it2=no_of_matches->begin();
				for(list<String>::const_iterator it=smarts.begin(); it!=smarts.end(); it++)
				{
					bool ok = 0;
					Size matches;
					Size req_matches = 1;
					if(no_of_matches) req_matches = *it2;

					molecular_similarity_->matchSmarts(usmile,*it,matches,req_matches);
					if(matches >= req_matches) ok=1;

					if(ok)
					{
						hit=1;
						if(!intersection) break; // found one matching SMARTS
					}
					else if(intersection)
					{
						hit=0;
						break; // non-matching SMARTS
					}
				}

				if(hit)
				{
					isomer_IDs.push_back(query.value(0).toULongLong());
					no_hits++;
				}

				if(no_of_matches) it2++;
			}

			if(isomer_IDs.empty()) return;

			if(output_isomer_IDs)
			{
				conformation_IDs = isomer_IDs;
			}
			else
			{
				// Add IDs for all conformations whose isomer is similar to a query to list 'conformation_IDs'
				String q = "select conformation_id from conformation where isomer_id IN (";
				for(list<LongSize>::iterator it=isomer_IDs.begin(); it!=isomer_IDs.end(); )
				{
					q+=String(*it);
					it++;
					if(it!=isomer_IDs.end()) q+=",";
				}
				q+=") order by conformation_id";
				status = query.exec(q.c_str());
				if(!status)
				{
					String error(query.lastError().databaseText().toAscii().constData());
					throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::searchMoleculesBySmarts","query for fetching conformation IDs failed due to : "+error));
				}
				while(query.next())
				{
					conformation_IDs.push_back(query.value(0).toULongLong());
				}
			}
		}
	#endif


		void MolDB::searchSimilarMoleculesByFingerprint(list<vector<bool> >& query_fingerprints, const float& minimal_similarity, const float& maximal_similarity, list<LongSize>& conformation_IDs, const list<LongSize>* conformation_IDs_to_check, bool input_isomer_IDs, bool output_isomer_IDs)
		{
			conformation_IDs.clear();
			QSqlQuery query(database_);

			// Do not extract millions of fingerprints at once, which might generate a memory overflow. Instead, make sure that this function is called recursively ...
			list<LongSize> tmp_conformation_IDs_to_check;
			if(!conformation_IDs_to_check)
			{
				String q ="select max(isomer_id)-min(isomer_id)+1 from isomer";
				query.exec(q.c_str());
				query.next();
				LongSize size = query.value(0).toULongLong();
				if(size>50000)
				{
					String q ="select isomer_id from isomer";
					query.exec(q.c_str());
					while(query.next())
					{
						tmp_conformation_IDs_to_check.push_back(query.value(0).toULongLong());
					}
					conformation_IDs_to_check = &tmp_conformation_IDs_to_check;
				}
			}

			// prevent MySql max_allowed_packet error
			if(conformation_IDs_to_check && conformation_IDs_to_check->size()>50000)
			{
				Size i=1;
				list<LongSize> batch_conformation_IDs_to_check;
				for(list<LongSize>::const_iterator it=conformation_IDs_to_check->begin(); it!=conformation_IDs_to_check->end(); i++)
				{
					batch_conformation_IDs_to_check.push_back(*it);
					it++;
					if(i%50000==0 || it==conformation_IDs_to_check->end())
					{
						list<LongSize> batch_found_IDs;

						searchSimilarMoleculesByFingerprint(query_fingerprints,minimal_similarity,maximal_similarity, batch_found_IDs, &batch_conformation_IDs_to_check,input_isomer_IDs,output_isomer_IDs);
						conformation_IDs.splice(conformation_IDs.end(),batch_found_IDs);
						batch_conformation_IDs_to_check.clear();
					}
				}
				return;
			}

			// Find isomer-IDs to be used ...
			String q ="select isomer_id,fingerprint from isomer";
			if(conformation_IDs_to_check)
			{
				if(!input_isomer_IDs) // conformation IDs are specified
				{
					q="select i.isomer_id,i.fingerprint from isomer as i join conformation as c on c.isomer_id=i.isomer_id where c.conformation_id IN (";
				}
				else
				{
					q ="select isomer_id,fingerprint from isomer where isomer_id IN (";
				}
				for(list<LongSize>::const_iterator it=conformation_IDs_to_check->begin(); it!=conformation_IDs_to_check->end(); )
				{
					q+=String(*it);
					it++;
					if(it!=conformation_IDs_to_check->end()) q+=",";
				}
				q+=")";
			}
			bool status = query.exec(q.c_str());
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::searchSimilarMoleculesByFingerprint","query for fetching isomer IDs failed due to : "+error));
			}

			list<LongSize> isomer_IDs;
			bool found_isomers = 0;

			// Now compute tanimoto coefficients ...
			while(query.next())
			{
				QVariant var = query.value(1);
				QByteArray* ba = (QByteArray*)(&var);
				vector<bool> fp;
				bytearrayToBitvector_(*ba,fp);
				bool found_sim_mol = 0;

				for(list<vector<bool> >::iterator it=query_fingerprints.begin(); it!=query_fingerprints.end(); it++)
				{
					float sim = molecular_similarity_->calculateSimilarity(*it,fp);

					if(sim>=minimal_similarity && sim<=maximal_similarity)
					{
						found_sim_mol = 1;

						// if maximal_similarity==1 we do not need to check if the current hit is too similar to some other query molecules
						if(maximal_similarity>0.99999)
						{
							break;
						}
					}
					else if(sim>maximal_similarity)
					{
						found_sim_mol = 0;
						break;
					}
				}

				if(found_sim_mol)
				{
					isomer_IDs.push_back(query.value(0).toULongLong());
					found_isomers = 1;
				}
			}

			if(!found_isomers) return;

			if(output_isomer_IDs)
			{
				conformation_IDs = isomer_IDs;
			}
			else
			{
				// Add IDs for all conformations whose isomer is similar to a query to list 'conformation_IDs'
				q = "select conformation_id from conformation where isomer_id IN (";
				for(list<LongSize>::iterator it=isomer_IDs.begin(); it!=isomer_IDs.end(); )
				{
					q+=String(*it);
					it++;
					if(it!=isomer_IDs.end()) q+=",";
				}
				q+=") order by conformation_id";
				status = query.exec(q.c_str());
				if(!status)
				{
					String error(query.lastError().databaseText().toAscii().constData());
					throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::searchSimilarMoleculesByFingerprint","query for fetching conformation IDs failed due to : "+error));
				}
				while(query.next())
				{
					conformation_IDs.push_back(query.value(0).toULongLong());
				}
			}
		}



		void MolDB::searchMoleculesByUCK(String uck, list<LongSize>& conformation_IDs)
		{
			QSqlQuery query(database_);
			QString q = "SELECT conformation.conformation_id \
					FROM isomer \
					INNER JOIN conformation ON (isomer.isomer_id=conformation.isomer_id) \
					WHERE isomer.uck = :uck";
			query.prepare(q);
			QVariant qvar(uck.c_str());
			query.bindValue(":uck",qvar);
			bool status = query.exec();
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::searchMoleculesByUCK","query for fetching molecule failed due to : "+error));
			}
			while(query.next())
			{
				conformation_IDs.push_back( query.value(0).toULongLong() );
			}
		}

		Molecule* MolDB::fetchMoleculeById(LongSize conformation_id)
		{
			// fetch current column-names of table 'properties'
			QSqlQuery query(database_);
			bool db_has_properties = query.exec("describe properties");
			list<String> property_columns;
			if(db_has_properties)
			{
				while(query.next())
				{
					property_columns.push_back(query.value(0).toString().toStdString());
				}
			}


			QString q = "SELECT conformation.conformation_id,uncompress(data),uncompress(coordinates),uncompress(charges) \
												 FROM topology \
												 INNER JOIN conformation ON (topology.isomer_id=conformation.isomer_id) \
										     INNER JOIN charges ON (topology.isomer_id=charges.isomer_id) \
					               WHERE conformation.conformation_id = :id";
			query.prepare(q);
			QVariant qvar((int)conformation_id); /// Qt does not seem to accept long!
			query.bindValue(":id",qvar);
			bool status = query.exec();
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::fetchMoleculeById","query for fetching molecule failed due to : "+error));
			}

			Molecule* mol = 0;
			if(query.next())
			{
				mol = new Molecule;
				QByteArray topology = query.value(1).toByteArray();
				QByteArray coords   = query.value(2).toByteArray();
				QByteArray charges  = query.value(3).toByteArray();
				buildTopology(*mol,topology);
				buildCoordinates(*mol,coords);
				buildCharges(*mol,charges);
				LongSize conf_id = query.value(0).toULongLong();
				mol->setName(db_name_+"_"+String(conf_id));

				if(db_has_properties)
				{
					buildProperties(mol,conformation_id,property_columns);
				}

				QSqlQuery query2(database_);
				query2.exec(("select hash from conformation where conformation_id="+String(conformation_id)).c_str());
				if(query2.next())
				{
					mol->setProperty("Conformation_input_UID",query2.value(0).toString().toStdString());
				}

			}

			return mol;
		}


		vector<Molecule*> MolDB::fetchMoleculeById(vector<LongSize>& conformation_ids)
		{
			// fetch current column-names of table 'properties'
			QSqlQuery query(database_);
			bool db_has_properties = query.exec("describe properties");
			list<String> property_columns;
			if(db_has_properties)
			{
				while(query.next())
				{
					property_columns.push_back(query.value(0).toString().toStdString());
				}
			}

			vector<Molecule*> ret;
			QString q = "SELECT conformation.conformation_id,uncompress(data),uncompress(coordinates),uncompress(charges) \
									 FROM topology \
									 INNER JOIN conformation ON (topology.isomer_id=conformation.isomer_id) \
							     INNER JOIN charges ON (topology.isomer_id=charges.isomer_id) \
							     WHERE conformation.conformation_id IN (";
			QString q2 = " order by field(conformation.conformation_id,";
			for(Position i=0;i<conformation_ids.size()-1;i++)
			{
				q+=String(conformation_ids[i]).c_str();
				q2+=String(conformation_ids[i]).c_str();
				q+=",";
				q2+=",";
			}
			q+=String(conformation_ids[conformation_ids.size()-1]).c_str();
			q2+=String(conformation_ids[conformation_ids.size()-1]).c_str();
			q+=")";
			q2+=")";
			q+=q2;

			bool status = query.exec(q);
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::fetchMoleculeById","query for fetching molecule failed due to : "+error));
			}

			while(query.next())
			{
				Molecule* mol = new Molecule();
				String conformation_id = query.value(0).toString().toStdString();
				QVariant topology_var = query.value(1);
				QVariant coords_var = query.value(2);
				QVariant charges_var = query.value(3);
				QByteArray* topology = (QByteArray*)&topology_var;
				QByteArray* coords   = (QByteArray*)&coords_var;
				QByteArray* charges  = (QByteArray*)&charges_var;
				buildTopology(*mol,*topology);
				buildCoordinates(*mol,*coords);
				buildCharges(*mol,*charges);
				LongSize conf_id = query.value(0).toULongLong();
				mol->setName(db_name_+"_"+String(conf_id));

				if(db_has_properties)
				{
					buildProperties(mol,query.value(0).toUInt(),property_columns);
				}

				QSqlQuery query2(database_);
				query2.exec(("select hash from conformation where conformation_id="+conformation_id).c_str());
				if(query2.next())
				{
					mol->setProperty("Conformation_input_UID",query2.value(0).toString().toStdString());
				}

				ret.push_back(mol);
			}
			return ret;
		}


		void MolDB::bitvectorToBytearray_(const vector<bool>& bitvector, QByteArray& bytearray)
		{
			QBuffer buffer(&bytearray);
			buffer.open(QIODevice::WriteOnly);
			QDataStream datastream(&buffer);
			datastream.setByteOrder(QDataStream::BigEndian);
			#if QT_VERSION>=0x040600
				datastream.setFloatingPointPrecision(QDataStream::SinglePrecision);
			#endif

			quint16 size = (quint16) bitvector.size();
			datastream << size;
			for(Size i=0; i<bitvector.size(); i++)
			{
				datastream << bitvector[i];
			}
			buffer.close();
		}


		void MolDB::bytearrayToBitvector_(const QByteArray& bytearray, vector<bool>& bitvector)
		{
			QDataStream datastream(bytearray);
			datastream.setByteOrder(QDataStream::BigEndian);
			#if QT_VERSION>=0x040600
				datastream.setFloatingPointPrecision(QDataStream::SinglePrecision);
			#endif

			// fetch and set length
			quint16 size;
			datastream>>size;
			bitvector.resize(size);

			for(Size i=0; i<size; i++)
			{
				bool b;
				datastream >> b;
				bitvector[i] = b;
			}
		}


		LongSize MolDB::storeMolecule(Molecule* mol, String* conformation_hash)
		{
			bool status = true;

			String isomer_hash;
			FlexibleMolecule::generateTopologyHash(mol,isomer_hash);

			QSqlQuery query(database_);
			LongSize isomer_id;

			/// check whether this isomer is already in database
			String s="select isomer_id from isomer where hash=\""+isomer_hash+"\"";
			query.exec(s.c_str());
			bool db_has_isomer = query.next();
			if(db_has_isomer)
			{
				isomer_id = query.value(0).toULongLong();
			}
			else
			{
				QByteArray topology;
				QBuffer topobuffer(&topology);
				topobuffer.open(QIODevice::WriteOnly);
				QDataStream topods(&topobuffer);
				getTopology(*mol,topods);
				topobuffer.close();

				QByteArray charges;
				QBuffer chargebuffer(&charges);
				chargebuffer.open(QIODevice::WriteOnly);
				QDataStream chargeds(&chargebuffer);
				getCharges(*mol,chargeds);
				chargebuffer.close();

				// create canonical smile
				String cansmile="";
			#ifdef BALL_HAS_OPENEYE
				OEChem::OEMol* omol;
				molecular_similarity_->generateCanSmile(*mol,cansmile,&omol);
			#elif defined BALL_HAS_OPENBABEL
				OpenBabel::OBMol* omol;
				molecular_similarity_->generateCanSmile(*mol,cansmile,&omol);
			#endif

				//create UCK key
				UCK uck_gen(*mol,true);
				String uck = uck_gen.getUCK().trim();

				// calculate molecular weight
				double MW=0;
				for(AtomConstIterator it=mol->beginAtom(); +it; it++)
				{
					MW += it->getElement().getAtomicWeight();
				}

				// calculate binary fingerprint
				vector<bool> fp;
				molecular_similarity_->generatePathFingerprint(*mol,fp);
				QByteArray binary_fingerprint;
				bitvectorToBytearray_(fp,binary_fingerprint);

				// calculate logP
				double logP = 0;
			#if defined BALL_HAS_OPENEYE || defined BALL_HAS_OPENBABEL
				logP = logP_.calculate(cansmile);
			#endif

				query.prepare("INSERT INTO isomer (usmile,uck,hash,MW,fingerprint,logP) VALUES (?,?,?,?,?,?)");
				query.addBindValue(cansmile.c_str());
				query.addBindValue(uck.c_str());
				query.addBindValue(isomer_hash.c_str());
				query.addBindValue(String(MW).c_str());
				query.addBindValue(binary_fingerprint);
				query.addBindValue(String(logP).c_str());
				status = query.exec();
				if(!status)
				{
					String error(query.lastError().databaseText().toAscii().constData());
					throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::storeMolecule","query for inserting molecule failed due to : "+error));
				}

				status = query.exec(("select isomer_id from isomer where hash=\""+isomer_hash+"\"").c_str());
				if(!status || !query.next())
				{
					String error(query.lastError().databaseText().toAscii().constData());
					throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::storeMolecule","fetching ID of inserted isomer failed due to : "+error));
				}
				isomer_id = query.value(0).toULongLong();

				query.prepare("INSERT INTO topology (isomer_id, data) VALUES (?,COMPRESS(?))");
				QVariant qvar((int)isomer_id); // Qt does not seem to accept long!
				query.addBindValue(qvar);
				query.addBindValue(topology);
				status = query.exec();
				if(!status)
				{
					String error(query.lastError().databaseText().toAscii().constData());
					throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::storeMolecule","query for inserting molecule failed due to : "+error));
				}

				query.prepare("INSERT INTO charges (isomer_id, method_id, charges) VALUES (?, ?, COMPRESS(?))");
				query.addBindValue(qvar);
				query.addBindValue(1);
				query.addBindValue(charges);
				status = query.exec();
				if(!status)
				{
					String error(query.lastError().databaseText().toAscii().constData());
					throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::storeMolecule","query for inserting molecule failed due to : "+error));
				}

			#if defined BALL_HAS_OPENBABEL || defined BALL_HAS_OPENEYE
				if(!fg_counting_disabled_)
				{
					/// create functional-group counts and enter this information into db
					vector<Size> fingerprint;
					const vector<String>& group_names = molecular_similarity_->getFunctionalGroupNames();
					molecular_similarity_->generateFingerprint(*omol,fingerprint);
					delete omol;
					bool db_has_funcGroups = query.exec("describe functional_groups");
					if(!db_has_funcGroups)
					{
						String q = "create table functional_groups(isomer_id int(10) unique key,";
						Size no=group_names.size();
						for(Size i=0; i<no;i++)
						{
							q+="`"+group_names[i]+"` tinyint(3)";
							if(i<no-1) q+=",";
						}
						q+=");";
						bool ok = query.exec(q.c_str());
						if(!ok)
						{
							cout<<q<<endl;
							String error(query.lastError().databaseText().toAscii().constData());
							throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::storeMolecule","Could not create table 'functional_groups' due to : "+error));
						}
					}

					String q = "insert into functional_groups VALUES("+String(isomer_id)+",";
					Size no=fingerprint.size();
					for(Size i=0; i<no;i++)
					{
						q+=String(fingerprint[i]);
						if(i<no-1) q+=",";
					}
					q+=");";

					bool ok = query.exec(q.c_str());
					if(!ok)
					{
						String error(query.lastError().databaseText().toAscii().constData());
						throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::storeMolecule","Could not store functional group counts due to : "+error));
					}
				}
			#endif
			} // end of isomer storage

			/// Save vendor information
			if(save_vendor_information_)
			{
				if(mol->hasProperty(property_vendorID_))
				{
					String vendors_compound_id = mol->getProperty(property_vendorID_).toString();
					String vendor_name = "";
					if(fetch_vendorname_from_prop_)
					{
						if(mol->hasProperty(property_vendorname_))
						{
							vendor_name = mol->getProperty(property_vendorname_).toString();
						}
					}
					else
					{
						vendor_name = vendorname_;
					}

					// search vendor-id in table 'vendors'
					s = "select vendor_id from vendors where name=\""+vendor_name+"\" and version=\""+vendorversion_+"\"";
					status = query.exec(s.c_str());
					if(!status)
					{
						String error(query.lastError().databaseText().toAscii().constData());
						throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::storeMolecule","query for searching existing vendor information failed due to : "+error));
					}
					LongSize vendor_id=0;
					if(query.next()) // entry in table 'vendors' found
					{
						vendor_id = query.value(0).toULongLong();
					}
					else // no entry in table 'vendors' found
					{
						s = "insert into vendors (name,version,url) VALUES (\""+vendorname_+"\",\""+vendorversion_+"\",\""+vendorurl_+"\")";
						status = query.exec(s.c_str());
						if(!status)
						{
							String error(query.lastError().databaseText().toAscii().constData());
							throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::storeMolecule","query for inserting new entry into table 'vendors' failed due to : "+error));
						}
						s = "select vendor_id from vendors where name=\""+vendor_name+"\" and version=\""+vendorversion_+"\"";
						status = query.exec(s.c_str());
						if(!status)
						{
							String error(query.lastError().databaseText().toAscii().constData());
							throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::storeMolecule","query for searching existing vendor information failed due to : "+error));
						}
						vendor_id=0;
						if(query.next())
						{
							vendor_id = query.value(0).toULongLong();
						}
						else // still no entry in table 'vendors' found; should never happen since we try to insert this information above ...
						{
							throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::storeMolecule","could not insert new vendor"));
						}
					}

					// now insert isomer_id--vendor_id relations into table 'vendor_info'
					s = "insert into vendor_info values("+String(isomer_id)+",\""+String(vendor_id)+"\",\""+vendors_compound_id+"\")";
					status = query.exec(s.c_str());
					if(!status)
					{
						cout<<s<<endl<<endl;
						String error(query.lastError().databaseText().toAscii().constData());
						throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::storeMolecule","query for inserting vendor information failed due to : "+error));
					}
				}
			}

			/// insert conformation, if it does not already exist in database
			LongSize conformation_id;
			String conf_hash;
			if(mol->hasProperty("Conformation_input_UID"))
			{
				conf_hash = mol->getProperty("Conformation_input_UID").toString();
			}
			else
			{
				Conformation::generateHash(mol,conf_hash);
			}

			if(conformation_hash) // if desired, save to output variable
			{
				*conformation_hash = conf_hash;
			}

			s="select conformation_id from conformation where hash=\""+conf_hash+"\"";
			query.exec(s.c_str());
			bool db_has_conformation = query.next();
			if(db_has_conformation)
			{
				conformation_id = query.value(0).toULongLong();
			}
			else
			{
				QByteArray coords;
				QBuffer coordsbuffer(&coords);
				coordsbuffer.open(QIODevice::WriteOnly);
				QDataStream coordsds(&coordsbuffer);
				getCoordinates(*mol,coordsds);
				coordsbuffer.close();

				query.prepare("INSERT INTO conformation (isomer_id, method_id, energy, coordinates,hash) VALUES (?, ?, ?, COMPRESS(?),?)");

				query.addBindValue(String(isomer_id).c_str());
				query.addBindValue(1);
				query.addBindValue(0.0);
				query.addBindValue(coords);
				query.addBindValue(conf_hash.c_str());
				status = query.exec();
				if(!status)
				{
					String error(query.lastError().databaseText().toAscii().constData());
					throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::storeMolecule","query for inserting molecule failed"));
				}

				status = query.exec(("select conformation_id from conformation where hash=\""+conf_hash+"\"").c_str());
				if(!status || !query.next())
				{
					String error(query.lastError().databaseText().toAscii().constData());
					throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::storeMolecule","fetching ID of inserted conformation failed due to : "+error));
				}
				conformation_id = query.value(0).toULongLong();

				/// insert comformation-properties into table 'properties'
				status = query.exec("describe properties");
				HashSet<String> property_columns;
				if(!status)
				{
					String error(query.lastError().databaseText().toAscii().constData());
					throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::storeMolecule()","table 'properties' does not seem to exist : "+error));
				}
				while(query.next())
				{
					property_columns.insert(query.value(0).toString().toStdString());
				}

				Size no_properties = mol->countProperties();
				Size no_used_properties = 0;
				bool first_prop=1;
				if(no_properties>0)
				{
					String q ="INSERT INTO properties (conformation_id, ";
					String v="("+String(conformation_id)+",";
					for(Size i=0; i<no_properties; i++)
					{
						NamedProperty& np=mol->getNamedProperty(i);
						String key=np.getName();

						// Information about vendors is saved to table 'vendors', so do not also save it as property.
						if(save_vendor_information_ && ((fetch_vendorname_from_prop_&&key==property_vendorname_) || key==property_vendorID_) )
						{
							continue;
						}

						if(!first_prop)
						{
							q+=",";
							v+=",";
						}
						first_prop=0;

						key.substituteAll("-","_"); // '-' is not allowed in mysql column-name
						key.substituteAll(" ","_");  // space is not allowed in mysql column-name

						if(property_columns.find(key)==property_columns.end())
						{
							status = query.exec(String("alter table properties add column "+key+" varchar(250)").c_str());
							if(!status)
							{
								String error(query.lastError().databaseText().toAscii().constData());
								throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::storeMolecule()","adding a column to table 'properties' failed : "+error));
							}
							property_columns.insert(key);
						}

						String value=np.toString();
						q+=key;
						v+="\""+value+"\"";
						no_used_properties++;
					}
					q+=")";
					v+=")";

					status = query.exec(String(q+" VALUES "+v).c_str());
					if(!status && no_used_properties>0)
					{
						String error(query.lastError().databaseText().toAscii().constData());
						throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::storeMolecule()","inserting properties failed : "+error));
					}
				}
			}

			return conformation_id;
		}


		void MolDB::storeResult(Result* result, String& target_name)
		{
			String type = result->getMethodString();
			const String& description = result->getDescription();

			LongSize method_id;
			String s="select method_id from methods where type=\""+type+"\" and description=\""+description+"\" and target_name=\""+target_name+"\"";
			QSqlQuery query(database_);
			query.exec(s.c_str());
			bool db_has_method = query.next();
			if(db_has_method)
			{
				method_id = query.value(0).toULongLong();
			}
			else
			{
				s="insert into methods (type,description,target_name) VALUES (\""+type+"\",\""+description+"\",\""+target_name+"\")";
				query.exec(s.c_str());

				s = "select method_id from methods where type=\""+type+"\" and description=\""+description+"\" and target_name=\""+target_name+"\"";
				bool status = query.exec(s.c_str());
				if(!status || !query.next())
				{
					String error(query.lastError().databaseText().toAscii().constData());
					throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::storeResult","fetching ID of inserted result-item failed due to : "+error));
				}
				method_id = query.value(0).toULongLong();
			}

			const HashMap<String, vector<Result::ResultData> >* data = result->getData();
			for(HashMap<String, vector<Result::ResultData> >::const_iterator it=data->begin(); it!=data->end(); it++)
			{
				// fetch database ID for input conformation, identified by hash-string
				const String& input_conf_hash = it->first;
				String input_conf_id ="";
				query.exec(("select conformation_id from conformation where hash=\""+input_conf_hash+"\"").c_str());
				if(query.next())
				{
					input_conf_id = query.value(0).toString().toStdString();
				}
				else
				{
					throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::storeResult()","no molecule with hash '"+input_conf_hash+"' in database!"));
				}

				const vector<Result::ResultData>& res = it->second;

				for(Size i=0; i<res.size(); i++)
				{
					// fetch database ID for output conformation, identified by hash-string
					String output_conf_hash = res[i].getLigandConformationId();
					String output_conf_id ="";
					query.exec(("select conformation_id from conformation where hash=\""+output_conf_hash+"\"").c_str());
					if(query.next())
					{
						output_conf_id = query.value(0).toString().toStdString();
					}
					else
					{
						throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::storeResult()","no molecule with hash '"+input_conf_hash+"' in database!"));
					}

					double score = res[i].getEnergy();

					s="select input_conf_id from results where input_conf_id=\""+input_conf_id+"\" and output_conf_id=\""+output_conf_id+"\" and method_id="+String(method_id)+"";
					query.exec(s.c_str());
					bool db_has_resultentry = query.next();
					if(!db_has_resultentry)
					{
						s="insert into results (input_conf_id,output_conf_id,method_id,score) VALUES (\""+input_conf_id+"\",\""+output_conf_id+"\","+String(method_id)+","+String(score)+")";
						query.exec(s.c_str());
					}
				}
			}
		}


		vector<Result*> MolDB::fetchResults(list<String>& conformation_hashs, String target_name)
		{
			QSqlQuery query(database_);
			map<LongSize,Result*> methodID_to_result;

			for(list<String>::iterator hash_it=conformation_hashs.begin(); hash_it!=conformation_hashs.end(); hash_it++)
			{
				String q = "select m.method_id,m.type,m.description, r.input_conf_id,r.output_conf_id,r.score from results as r join conformation as c join methods as m on r.output_conf_id=c.conformation_id and m.method_id=r.method_id where ";
				if(target_name!="") q+="m.target_name=\""+target_name+"\" and ";
				q+="c.hash=\""+String(*hash_it)+"\"";

				// Result-items must be sorted accoring to their type, otherwise writing them to a DockResultFile will not work correctly.
				q+=" order by m.method_id,isomer_id";

				bool status = query.exec(q.c_str());
				if(!status)
				{
					String error(query.lastError().databaseText().toAscii().constData());
					throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::fetchResult","query for fetching result-entry failed due to : "+error));
				}

				while(query.next())
				{
					LongSize method_id = query.value(0).toULongLong();
					String method_type = query.value(1).toString().toStdString();
					String description = query.value(2).toString().toStdString();
					String input_conf_id = query.value(3).toString().toStdString();
					String output_conf_id = query.value(4).toString().toStdString();
					double score = query.value(5).toDouble();

					Result* res;
					map<LongSize,Result*>::iterator it=methodID_to_result.find(method_id);
					if(it==methodID_to_result.end())
					{
						res = new Result;
						res->setDescription(description);
						if(method_type=="RECEPTORIMPORT") res->setMethod(Result::RECEPTORIMPORT);
						if(method_type=="LIGANDIMPORT") res->setMethod(Result::LIGANDIMPORT);
						else if(method_type=="DOCKING") res->setMethod(Result::DOCKING);
						else if(method_type=="RESCORING") res->setMethod(Result::RESCORING);
						else if(method_type=="MDRESCORING") res->setMethod(Result::MDRESCORING);
						else if(method_type=="CONSENSUSRESCORING") res->setMethod(Result::CONSENSUSRESCORING);
						else if(method_type=="MOLECULE_CHECK") res->setMethod(Result::MOLECULE_CHECK);
						else
						{
							throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::fetchResult","Unknown method-type encountered: "+method_type));
						}

						methodID_to_result.insert(make_pair(method_id,res));
					}
					else
					{
						res = it->second;
					}

					// fetch hash-string for input-conformation using the input-conformation's database ID
					QSqlQuery query2(database_);
					query2.exec(("select hash from conformation where conformation_id="+input_conf_id).c_str());
					if(!query2.next())
					{
						throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::fetchResult","hash-string for conformation "+input_conf_id+" could not be found in database"));
					}
					String input_conf_hash = query2.value(0).toString().toStdString();

					// fetch hash-string for output-conformation using the output-conformation's database ID
					query2.exec(("select hash from conformation where conformation_id="+output_conf_id).c_str());
					if(!query2.next())
					{
						throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::fetchResult","hash-string for conformation "+output_conf_id+" could not be found in database"));
					}
					String output_conf_hash = query2.value(0).toString().toStdString();

					res->add(input_conf_hash,output_conf_hash,res->getMethod(),score,"0"); /// NO receptor-id storing at the moment!!
				}
			}

			// Return Result-objects sorted by their ID
			vector<Result*> results;
			for(map<LongSize,Result*>::iterator it=methodID_to_result.begin(); it!=methodID_to_result.end(); it++)
			{
				results.push_back(it->second);
			}

			return results;
		}


		void MolDB::deleteIsomer(LongSize isomer_id)
		{
			QSqlQuery query;
			query.prepare("DELETE FROM conformation WHERE isomer_id = ?");
			QVariant qvar((int)isomer_id); /// Qt does not seem to accept long!
			query.addBindValue(qvar);
			bool status = query.exec();
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::deleteIsomer","query for deleting molecule failed due to : "+error));
			}

			query.prepare("DELETE FROM charges WHERE isomer_id = ?");
			query.addBindValue(qvar);
			query.exec();
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::deleteIsomer","query for deleting molecule failed due to : "+error));
			}

			query.prepare("DELETE FROM topology WHERE isomer_id = ?");
			query.addBindValue(qvar);
			query.exec();
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::deleteIsomer","query for deleting molecule failed due to : "+error));
			}

			query.prepare("DELETE FROM isomer WHERE isomer_id = ?");
			query.addBindValue(qvar);
			query.exec();
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::deleteIsomer","query for deleting molecule failed due to : "+error));
			}

		}


		void MolDB::deleteConformation(LongSize conformation_id)
		{
			// Fetch isomer ID
			String s = "select isomer_id from conformation where conformation_id="+String(conformation_id);
			QSqlQuery query(database_);
			bool status = (query.exec(s.c_str()) && query.next());
			LongSize isomer_id = query.value(0).toULongLong();

			// delete conformation
			s = "DELETE FROM conformation WHERE conformation_id =";
			s+=String(conformation_id);
			status = query.exec(s.c_str());
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::deleteConformation","query for deleting molecule failed due to : "+error));
			}

			// Check if the conformation's isomer has any other conformations. If this is not the case, the isomer will also be deleted.
			s = "select conformation_id from conformation where isomer_id="+String(isomer_id);
			status = (query.exec(s.c_str()));
			if(query.size()==0)
			{
				deleteIsomer(isomer_id);
			}
		}


		void MolDB::buildProperties(Molecule* mol, LongSize ID, list<String>& property_columns)
		{
			QSqlQuery query(database_);
			bool status = query.exec(String("select * from properties where conformation_id="+String(ID)).c_str());
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::buildProperties","could not fetch properties from table : "+error));
			}

			if(query.next()) // if properties for current molecule exist in data base
			{
				list<String>::iterator it=property_columns.begin();
				for(Size i=0; it!=property_columns.end(); it++, i++)
				{
					if(i==0) continue; // 'conformation_id' field
					String value=query.value(i).toString().toStdString();
					if(value!="") mol->setProperty(*it,value);
				}
			}
		}


		void MolDB::getCoordinates(Molecule& mol, QDataStream &ds)
		{
			#if QT_VERSION>=0x040600
				ds.setFloatingPointPrecision(QDataStream::SinglePrecision);
			#endif
			ds.setByteOrder(QDataStream::BigEndian);

			quint8 size = mol.countAtoms();
			ds << size;

			for(AtomConstIterator it=mol.beginAtom(); +it; it++)
			{
				const TVector3<float>& pos = it->getPosition();
				ds <<pos.x<<pos.y<<pos.z;
			}
		}

		void MolDB::buildCoordinates(Molecule& mol, QByteArray &data)
		{
			QDataStream ds(data);
			#if QT_VERSION>=0x040600
				ds.setFloatingPointPrecision(QDataStream::SinglePrecision);
			#endif
			ds.setByteOrder(QDataStream::BigEndian);

			quint8 size;
			ds >> size;

			for(AtomIterator it=mol.beginAtom(); +it && !ds.atEnd(); it++)
			{
				float x,y,z;
				ds >> x; ds >> y; ds >> z;
				it->setPosition(Vector3(x,y,z));
			}
		}

		void MolDB::buildCharges(Molecule& mol, QByteArray &data)
		{
			QDataStream ds(data);
			#if QT_VERSION>=0x040600
				ds.setFloatingPointPrecision(QDataStream::SinglePrecision);
			#endif
			ds.setByteOrder(QDataStream::BigEndian);

			quint8 size;
			ds >> size;

			for(AtomIterator it=mol.beginAtom(); +it && !ds.atEnd(); it++)
			{
				float c;
				ds >> c;
				it->setCharge(c);
			}
		}

		void MolDB::getCharges(Molecule& mol, QDataStream &ds)
		{
			#if QT_VERSION>=0x040600
				ds.setFloatingPointPrecision(QDataStream::SinglePrecision);
			#endif
			ds.setByteOrder(QDataStream::BigEndian);

			quint8 size = mol.countAtoms();
			ds << size;

			for(AtomConstIterator it=mol.beginAtom(); +it; it++)
			{
				float q = it->getCharge();
				ds << q;
			}
		}

		void MolDB::getTopology(Molecule& mol, QDataStream &ds)
		{
			quint8 b;
			// ToDo: switch to BigEndian here in next version of TueScreen
			// we are currently bound to LittleEndian, because the current
			// topologies are stored in LittleEndian
			#if QT_VERSION>=0x040600
				ds.setFloatingPointPrecision(QDataStream::SinglePrecision);
			#endif
			ds.setByteOrder(QDataStream::LittleEndian);
			bool huge = false;
			Size nAtoms = mol.countAtoms();
			if(nAtoms>255)
				huge = true;
			quint16 s;
			s = (quint16) nAtoms;
			ds << s;
			HashMap<const Atom*,Position> atom_idx;

			Size i=0;
			for(AtomConstIterator it=mol.beginAtom(); +it; it++, i++)
			{
				const Atom* at = &*it;
				atom_idx[at]=i;
				b = (quint8) at->getElement().getAtomicNumber();
				ds << b;
				b = (quint8) (at->getFormalCharge()+10);
				ds << b;
				b = (quint8) 0;
				ds << b;
			}
			AtomIterator ai;
			AtomBondIterator bi;
			BALL_FOREACH_BOND(mol,ai,bi)
			{
				Bond* b = &(*bi);
				Atom* at1 = (Atom*) b->getFirstAtom();
				Atom* at2 = (Atom*) b->getSecondAtom();
				if(huge)
				{
					quint16 idx1  = (quint16) atom_idx[at1]+1;
					quint16 idx2  = (quint16) atom_idx[at2]+1;
					ds << idx1;
					ds << idx2;
				}
				else
				{
					quint8 idx1  = (quint8) atom_idx[at1]+1;
					quint8 idx2  = (quint8) atom_idx[at2]+1;
					ds << idx1;
					ds << idx2;
				}

				quint8 order = (quint8) b->getOrder();
				ds << order;
			}
		}

		void MolDB::buildTopology(Molecule& mol, QByteArray &data)
		{
			QDataStream ds(data);
			#if QT_VERSION>=0x040600
				ds.setFloatingPointPrecision(QDataStream::SinglePrecision);
			#endif
			ds.setByteOrder(QDataStream::LittleEndian);

			bool huge = false;
			quint16 s;
			quint8 b;
			ds >> s;
			Size nAtoms = (Size) s;
			if(nAtoms>255)
				huge = true;

			vector<Atom*> atoms;

			for(Position i=0;i<nAtoms;i++)
			{
				int elem, fcharge;
				ds >> b;
				elem    = (int) b;
				ds >> b;
				fcharge = (int) b;
				fcharge-=10;
				ds >> b;
				Atom* at = new Atom();
				at->setElement(PTE[elem]);
				at->setFormalCharge(fcharge);
				at->setName(at->getElement().getSymbol());
				mol.append(*at);
				atoms.push_back(at);
			}
			while(!ds.atEnd())
			{
				int idx1, idx2, order;
				if(huge)
				{
					ds >> s;
					idx1  = (int) s;
					ds >> s;
					idx2  = (int) s;
				}
				else
				{
					ds >> b;
					idx1 = (int) b;
					ds >> b;
					idx2 = (int) b;
				}

				ds >> b;
				order = (int) b;
				Atom* at1 = atoms[idx1-1];
				Atom* at2 = atoms[idx2-1];
				Bond* bnd = at1->createBond(*at2);
				bnd->setOrder(order);
			}
		}


		void MolDB::fetchVendorInformation(Molecule* mol, list<VendorInfo>& vendor_info)
		{
			UCK uck_gen(*mol,true);
			String uck = uck_gen.getUCK().trim();

			QSqlQuery query(database_);
			QString q = "SELECT vi.isomer_id,v.name,vi.vendors_compound_id,i.MW \
			FROM vendor_info as vi JOIN isomer as i JOIN vendors as v\
			ON (vi.isomer_id=i.isomer_id and vi.vendor_id=v.vendor_id) \
			WHERE i.uck = \"";
			q+=uck.c_str();
			q+="\"";

			bool status = query.exec(q);
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::fetchVendorInformation","query for fetching vendor-info failed due to : "+error));
			}
			while(query.next())
			{
				VendorInfo vi;
				vi.database_compound_ID = query.value(0).toString().toStdString();
				vi.vendor_name = query.value(1).toString().toStdString();
				vi.vendors_compound_ID = query.value(2).toString().toStdString();
				vi.molecular_weight = query.value(3).toDouble();
				vendor_info.push_back(vi);
			}
		}


		// static function for creating a new database
		void MolDB::createNewDatabase(String& host, String& user, String& password, String& db_name, int port)
		{
			QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
			db.setHostName(host.c_str());
			db.setUserName(user.c_str());
			db.setPassword(password.c_str());
			if (port != 0) db.setPort(port);
			if(!db.open())
			{
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDBConnector","could not connect to database-server "+host));
			}

			QString q = "CREATE DATABASE if not exists "+QString(db_name.c_str());
			QSqlQuery query(db);
			query.prepare(q);
			bool status = query.exec();
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::createNewDatabase","could not create new database: "+error));
			}

			status = query.exec("USE "+QString(db_name.c_str()));

			status = query.exec("CREATE TABLE `charges` (`charge_id` int(10) unsigned NOT NULL auto_increment,`isomer_id` int(10) unsigned default NULL,`method_id` tinyint(4) default NULL,`charges` blob,PRIMARY KEY  (`charge_id`),KEY `isomer_id` (`isomer_id`)) ENGINE=MyISAM DEFAULT CHARSET=latin1");
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::createNewDatabase","could not create table 'charge' : "+error));
			}

			status = query.exec("CREATE TABLE `conformation` (`conformation_id` int(10) unsigned NOT NULL auto_increment,`isomer_id` int(10) unsigned NOT NULL, `method_id` tinyint(4) default NULL,`energy` float default NULL,`coordinates` blob, `hash` varchar(40) unique, PRIMARY KEY  (`conformation_id`),KEY `isomer_id` (`isomer_id`),UNIQUE KEY `hash_idx`(`hash`)) ENGINE=MyISAM DEFAULT CHARSET=latin1");
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::createNewDatabase","could not create table 'conformation' : "+error));
			}

			status = query.exec("CREATE TABLE `isomer`(`isomer_id` int(10) unsigned NOT NULL auto_increment,`usmile` text,`uck` varchar(40), `hash` varchar(40) unique, `MW` float, `fingerprint` blob, `logP` float, PRIMARY KEY (`isomer_id`),KEY `isomer_usmile_idx` (`usmile`(64)), UNIQUE KEY `hash_idx`(`hash`),KEY `uck_idx`(`uck`) ) ENGINE=MyISAM DEFAULT CHARSET=latin1");
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::createNewDatabase","could not create table 'isomer' : "+error));
			}

			status = query.exec("CREATE TABLE `topology`(`isomer_id` int(10) unsigned NOT NULL default '0',`data` blob, PRIMARY KEY  (`isomer_id`)) ENGINE=MyISAM DEFAULT CHARSET=latin1");
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::createNewDatabase","could not create table 'topology' : "+error));
			}

			status = query.exec("create table properties(conformation_id int, score double, PRIMARY KEY(`conformation_id`))");
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::createNewDatabase","could not create table 'properties' : "+error));
			}

			status = query.exec("create table results(input_conf_id int, output_conf_id int, method_id int, score double, key(input_conf_id), key(output_conf_id), key(method_id), constraint unique key(input_conf_id,output_conf_id,method_id))");
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::createNewDatabase","could not create table 'results' : "+error));
			}

			status = query.exec("create table methods(method_id int(10) unsigned NOT NULL auto_increment, type varchar(30), description text, target_name text, PRIMARY KEY (`method_id`))");
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::createNewDatabase","could not create table 'methods' : "+error));
			}

			status = query.exec("create table vendor_info(isomer_id int(10) unsigned NOT NULL, vendor_id int(10), vendors_compound_id varchar(30), KEY (`isomer_id`),constraint unique key(isomer_id,vendor_id,vendors_compound_id))");
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::createNewDatabase","could not create table 'vendor_info' : "+error));
			}

			status = query.exec("create table vendors(vendor_id int(10) unsigned NOT NULL auto_increment, name varchar(30), version varchar(30), url varchar(100), KEY (`vendor_id`), key(name), key(version), constraint unique key(name,version))");
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::createNewDatabase","could not create table 'vendors' : "+error));
			}

			status = query.exec("create table db_info(moldb_version int(10) unsigned NOT NULL, creation_date DATE)");
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::createNewDatabase","could not create table 'db_info' : "+error));
			}


			query.prepare("insert into db_info(moldb_version, creation_date) values (:version,CURDATE())");
			query.bindValue(":version", VERSION);
			status = query.exec();
			if(!status)
			{
				String error(query.lastError().databaseText().toAscii().constData());
				throw(Exception::GeneralException(__FILE__, __LINE__,"MolDB::createNewDatabase","could not initialize 'db_info' table : "+error));
			}
		}

		int MolDB::getVersion()
		{
			QSqlQuery query(database_);
			query.exec("select moldb_version from db_info");
			query.next();
			Size version = query.value(0).toULongLong();
			return version;
		}

		bool MolDB::hasResults()
		{
			QSqlQuery query(database_);
			bool db_hash_results = query.exec("describe results");
			return db_hash_results;
		}

		void MolDB::disableFunctionalGroupCounting()
		{
			fg_counting_disabled_ = true;
		}

		void MolDB::setVendorPropertyNames(String vendor_name, String vendors_compound_id_tag, bool fetch_vendorname_from_prop, String vendor_version, String url)
		{
			save_vendor_information_ = true;
			fetch_vendorname_from_prop_ = fetch_vendorname_from_prop;
			property_vendorID_ = vendors_compound_id_tag;
			vendorversion_ = vendor_version;
			vendorurl_ = url;
			if(fetch_vendorname_from_prop_)
			{
				property_vendorname_ = vendor_name;
			}
			else
			{
				vendorname_ = vendor_name;
			}
		}
}

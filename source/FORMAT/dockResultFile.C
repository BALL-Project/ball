// ----------------------------------------------------
// $Maintainer: Marc Röttig $
// $Authors: Marc Röttig, Marcel Schumann $
// ----------------------------------------------------

#include <BALL/FORMAT/dockResultFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/KERNEL/PTE.h>

using namespace std;

namespace BALL
{

		// adjust this value if the format of DockResultFile is changed
		const String DockResultFile::VERSION = "1.0";

		bool DockResultFile::SHOW_IDENTICAL_LIGANDS_WARNING = true;
		Ligand* DockResultFile::gmf_current_ligand_ = 0;
		Ligand* DockResultFile::gmf_last_ligand_ = 0;
		bool DockResultFile::gmf_new_ligand_read_ = 0;


		DockResultFile::DockResultFile()
		{
		}

		DockResultFile::DockResultFile(const String& name, File::OpenMode open_mode)
			:
			GenericMolFile(),
			receptor_result_(0),
			filename_(name),
			timestamp_(""),
			toolinfo_(""),
			gmf_property_name_("score"),
			gmf_result_(0),
			gmf_import_(0),
			gmf_result_method_(Result::DOCKING),
			gmf_receptor_conf_UID_(0),
			gmf_result_creation_disabled_(0),
			file_(0),
			receptor_(0)
		{
			name_ = name;
			xmlIn_ = 0; xmlOutLigand_ = 0; xmlOutResult_ = 0;xmlOutReceptor_ = 0;

			// If file exists already and output-mode is specified, then use append-mode instead in order to make sure that no result-data will be lost.
			if(open_mode==File::MODE_OUT && ifstream(name.c_str()))
			{
				File file(name);
				if (file.getSize() > 0)
				{
					Log.warn() << "Warning: DockResultFile '"<<name<<"' already exists, it is thus opened in append mode!"<<endl;
					open_mode=std::ios::app;
				}
			}

			open_mode_ = open_mode;

			// For append mode, first read result-section from xml-file. Then close input-stream and open an output-stream to the same file.
			if(open_mode==File::MODE_IN || open_mode==std::ios::app)
			{
				mode_read_ = true;

				file_  = new QFile(filename_.c_str());
				if(!file_->open(QIODevice::ReadOnly | QIODevice::Text))
				{
					throw(Exception::GeneralException(__FILE__, __LINE__,"DockResultFile::DockResultFile","cannot open file for input"));
				}
				xmlIn_ = new QXmlStreamReader(file_);

				readResults_();
				readReceptor_();


				if(open_mode==File::MODE_IN)
				{
					if(results_.size()==0)
					{
						throw(Exception::GeneralException(__FILE__, __LINE__,"DockResultFile::DockResultFile","Cannot read file because it contains no result-section!"));
					}
					// read the IDs of the molecules to be returned by DockResultFile::read().
					// By default this will be the output molecules of the last result section of the xml-file.
					gmf_input_conformation_IDs_ = results_.back()->getOutputIds();
				}
			}
			// read old data

			//
			if(open_mode==File::MODE_OUT || open_mode==File::MODE_APP)
			{
				mode_read_ = false;
				resultfileTmp_  = new QTemporaryFile();
				if(!resultfileTmp_->open())
				{
					throw(Exception::GeneralException(__FILE__, __LINE__,"DockResultFile::DockResultFile","cannot create temporary file for output"));
				}
				resultfile_ = new QFile(resultfileTmp_->fileName());
				if (!resultfile_->open(QIODevice::WriteOnly| QIODevice::Text))
				{
					throw(Exception::GeneralException(__FILE__, __LINE__,"DockResultFile::DockResultFile","cannot open temporary file for output"));
				}
				xmlOutResult_ = new QXmlStreamWriter(resultfile_);
				xmlOutResult_->setAutoFormatting(true);
				xmlOutResult_->writeStartElement(toQString(RESULTSTAG));

				ligandfileTmp_  = new QTemporaryFile();
				if(!ligandfileTmp_->open())
				{
					throw(Exception::GeneralException(__FILE__, __LINE__,"DockResultFile::DockResultFile","cannot create temporary file for output"));
				}
				ligandfile_     = new QFile(ligandfileTmp_->fileName());
				if (!ligandfile_->open(QIODevice::WriteOnly| QIODevice::Text))
				{
					throw(Exception::GeneralException(__FILE__, __LINE__,"DockResultFile::DockResultFile","cannot open temporary file for output"));
				}
				xmlOutLigand_ = new QXmlStreamWriter(ligandfile_);
				xmlOutLigand_->setAutoFormatting(true);
				xmlOutLigand_->writeStartElement(toQString(LIGANDSTAG));

				receptorfileTmp_ = new QTemporaryFile();
				if(!receptorfileTmp_->open())
				{
					throw(Exception::GeneralException(__FILE__, __LINE__,"DockResultFile::DockResultFile","cannot create temporary file for output"));
				}
				receptorfile_    = new QFile(receptorfileTmp_->fileName());
				if (!receptorfile_->open(QIODevice::WriteOnly| QIODevice::Text))
				{
					throw(Exception::GeneralException(__FILE__, __LINE__,"DockResultFile::DockResultFile","cannot open temporary file for output"));
				}
				xmlOutReceptor_ = new QXmlStreamWriter(receptorfile_);
				xmlOutReceptor_->setAutoFormatting(true);
				xmlOutReceptor_->writeStartElement(toQString(RECEPTORSTAG));
				//
				//writeReceptors(*xmlOutReceptor_);
			}
			if(open_mode==std::ios::app)
			{
				// read receptors from file and directly write them to output-stream

				/// TODO: support for more than 1 receptor!!
				//Receptor* rec;
				//while((rec=readReceptor()))
				/*	{
					mode_read_ = true;
					readReceptors_();
					mode_read_ = false;
					if(receptor_) writeReceptor(receptor_);
					//delete rec;
				} */

				/*
				file_->close();
				delete file_;
				delete xmlIn_;
				*/
			}
			closed_ = false;
		}

		DockResultFile::DockResultFile(const DockResultFile &df)
		{
		}

		DockResultFile::~DockResultFile()
				throw()
		{
			if(!closed_)
				close();
			if(!mode_read_)
			{
				delete xmlOutReceptor_;
				delete xmlOutLigand_;
				delete xmlOutResult_;
				delete receptorfile_;
				delete ligandfile_;
				delete resultfile_;
				delete receptorfileTmp_;
				delete ligandfileTmp_;
				delete resultfileTmp_;
			}
			else
			{
				delete xmlIn_;
			}
// 			for(Size i=0; i<results_.size(); i++)
// 			{
// 				delete results_[i];
// 			}
// 			for(Size i=0; i<ligands_.size(); i++)
// 			{
// 				delete ligands_[i];
// 			}
			delete file_;
		}

		void DockResultFile::setOutputParameters(Result::Method method, String property_name, String& receptor_conf_UID, String method_description)
		{
			gmf_result_method_ = method;
			gmf_property_name_ = property_name;
			gmf_receptor_conf_UID_ = receptor_conf_UID;
			gmf_result_description_ = method_description;
		}

		void DockResultFile::setToolInfo(const String& toolinfo, const String& timestamp)
		{
			toolinfo_ = toolinfo;
			timestamp_ = timestamp;
		}

		void DockResultFile::setInputParameters(list<Size>& selected_results)
		{
			gmf_input_conformation_IDs_.clear();

			// read the IDs of the molecules to be returned by DockResultFile::read().
			for(list<Size>::iterator it=selected_results.begin();
				it!=selected_results.end(); it++)
			{
				if(*it>=results_.size())
				{
					throw BALL::Exception::GeneralException(__FILE__,__LINE__,"DockResultFile::setInputParameters()","Result ID "+String(*it)+" is out of range!");
				}
				gmf_input_conformation_IDs_ += results_[*it]->getOutputIds();
			}
		}

		void DockResultFile::selectAllResultsForInput()
		{
			gmf_input_conformation_IDs_.clear();
			for(Size i=0; i<results_.size(); i++)
			{
				gmf_input_conformation_IDs_ += results_[i]->getOutputIds();
			}
		}

		void DockResultFile::disableAutomaticResultCreation()
		{
			gmf_result_creation_disabled_ = true;
		}

		const FlexibleMolecule* DockResultFile::getCurrentLigand()
		{
			return gmf_current_ligand_;
		}


		Molecule* DockResultFile::read()
			throw (Exception::ParseError)
		{
			if(!mode_read_)
			{
				throw BALL::Exception::ParseError(__FILE__,__LINE__,filename_,"Not in read mode!");
			}
			if(results_.size()==0)
			{
				throw BALL::Exception::ParseError(__FILE__,__LINE__,filename_,"No results in file!");
				return 0;
			}

			bool found_conf=0;
			while(!found_conf)
			{
				if(gmf_input_conformations_.size()==0)
				{
					// delete only if this has not been done by write() yet
					if(gmf_new_ligand_read_) delete gmf_last_ligand_;
					gmf_last_ligand_ = gmf_current_ligand_;

					if(gmf_current_ligand_) gmf_new_ligand_read_ = 1;
					gmf_current_ligand_ = readLigand();
					if(!gmf_current_ligand_) return 0;
					gmf_input_conformations_ = gmf_current_ligand_->getConformations();
				}

				while(gmf_input_conformations_.size()>0 && !found_conf)
				{
					if(gmf_input_conformation_IDs_.find(gmf_input_conformations_.front()->getId())!=gmf_input_conformation_IDs_.end())
					{
						found_conf=true;
						break;
					}
					else
					{
						gmf_input_conformations_.erase(gmf_input_conformations_.begin());
					}
				}
			}

			if(found_conf)
			{
				const Molecule* base_conformation_mol = gmf_current_ligand_->getParent();

				Molecule* mol = new Molecule(*base_conformation_mol);

				gmf_input_conformations_.front()->applyConformation(*mol);
				String lig_id(gmf_current_ligand_->getId());
				String lig_conf_id(gmf_input_conformations_.front()->getId());
				gmf_input_conformations_.erase(gmf_input_conformations_.begin());
			//	String rec_conf_id = input_conformations_.front()->getId();
				mol->setProperty("Ligand_UID",lig_id);
				mol->setProperty("Conformation_input_UID",lig_conf_id);

				for(Size i=0; i<results_.size(); i++)
				{
					String method_name = results_[i]->getMethodString();
					if(method_name=="LIGANDIMPORT") continue;
					if(results_[i]->hasOutputData(lig_conf_id))
					{
						double score = results_[i]->getOutputData(lig_conf_id).getEnergy();
						mol->setProperty("score_"+method_name,score);
						if(i==results_.size()-1) mol->setProperty("score",score);
					}
				}

				return mol;
			}
			else return NULL;
		}

		bool DockResultFile::write(const Molecule& mol)
			throw (BALL::File::CannotWrite)
		{
			if(mode_read_)
			{
				throw BALL::File::CannotWrite(__FILE__,__LINE__,filename_);
				return 0;
			}

			Protein* protein = dynamic_cast<Protein*>(const_cast<Molecule*>(&mol));
			if(protein)
			{
				if (!receptor_)
				{
					// Make a copy of the (parent-) protein, because the user might delete the
					// original before we write all protein conformations to file. We also cannot
					// write the receptor to file _now_ because additional conformations might be added later.
					Protein* protein_copy = new Protein(*protein);
					receptor_ = new Receptor(protein_copy);
				}
				else
				{
					Conformation* conf = new Conformation(receptor_->getParent(), protein);
					receptor_->addConformation(conf);
				}

				if(!receptor_result_)
				{
					receptor_result_ = Result::makeReceptorImportResult();
					receptor_result_->setToolInfo(toolinfo_);
					receptor_result_->setTimestamp(timestamp_);
				}

				writeResult(receptor_result_);
				return true;
			}

			bool status = 1;
			bool mol_has_lig_uid  = mol.hasProperty("Ligand_UID");
			bool mol_has_conf_uid = mol.hasProperty("Conformation_input_UID");
			bool lig_id_match=0;
			String lig_id;
			String conf_id;
			if(mol_has_lig_uid)
			{
				stringstream stream;
				stream.str(mol.getProperty("Ligand_UID").toString());
				stream >> lig_id;
				if(gmf_current_ligand_ && lig_id==gmf_current_ligand_->getId())
				{
					lig_id_match=true;
				}
			}
			if(mol_has_conf_uid)
			{
				stringstream stream;
				stream.str(mol.getProperty("Conformation_input_UID").toString());
				stream >> conf_id;
			}

			// If a new Ligand-section was opened by the last call of read(), write the previous Ligand to output-file before doing anything else
			if( gmf_last_ligand_ && (gmf_new_ligand_read_ || !mol_has_lig_uid || !lig_id_match) )
			{
				status = writeLigand(gmf_last_ligand_);
				if(!gmf_new_ligand_read_) delete gmf_last_ligand_; // delete only if this has not been done by read()
				gmf_last_ligand_ = gmf_current_ligand_;
				gmf_new_ligand_read_ = false;
			}

			// If no Ligand has been read yet (because read() has not been used yet), then create a new FlexibleMolecule object for the given Molecule.
			// Assign the correct ID to this new FlexibleMolecule in order to make sure that Molecules written by the next calls of this function can be appended to it (if they are conformation of the same structure).
			if( !gmf_current_ligand_ || !mol_has_lig_uid || !lig_id_match)
			{
				Molecule* mol_copy = new Molecule(mol);
				gmf_current_ligand_ = new FlexibleMolecule(mol_copy);
				gmf_last_ligand_ = gmf_current_ligand_;
				gmf_current_ligand_->clearConformations();

				if(mol_has_lig_uid)
				{
					gmf_current_ligand_->setId(lig_id);
				}
			}

			Molecule* nc_mol = const_cast<Molecule*>(&mol);
			Conformation* conformation = new Conformation(gmf_current_ligand_->getParent(),nc_mol);

			// Assign input conformation-ID only if current Ligand does not yet have a Conformation with this ID. Else the current Molecule contains a modified structure (e.g. docking output), so that the newly generated ID will be retained.
			bool lig_has_conf_uid = gmf_current_ligand_->hasConformation(conf_id);
// 			if(mol_has_conf_uid && !lig_has_conf_uid)
// 			{
// 				conformation->setId(conf_id);
// 			}

			// If the conformation is identical to the conformation that is stored in the current ligand with the same ID, then do not insert the new conformation again.
			if( !mol_has_conf_uid || !lig_has_conf_uid || (lig_has_conf_uid &&
				*gmf_current_ligand_->getConformationById(conf_id)!=*conformation) )
			{
				gmf_current_ligand_->addConformation(conformation);
			}
			else
			{
				delete conformation;
				conformation = gmf_current_ligand_->getConformationById(conf_id);
			}

			if(gmf_result_creation_disabled_)
			{
				return true;
			}

			// If result-section has already been read, skip adding information about ligand-import here.
			bool create_ligimports = false;
			if(open_mode_!=std::ios::app) create_ligimports = true;
			else
			{
				if(results_.size()==0 || (results_.back()==gmf_import_))
				{
					create_ligimports = true;
				}
				else
				{
					// If there is only a ReceptorImport-section yet, but no LigandImport-section, then we need to create LigandImport-Section and LigandImport-entries as well.
					bool found_importsection=false;
					for(Size i=0; i<results_.size(); i++)
					{
						if(results_[i]->getMethod()==Result::LIGANDIMPORT)
						{
							found_importsection=true;
							break;
						}
					}
					if(!found_importsection) create_ligimports = true;
				}
			}

			if(create_ligimports)
			{
				if(!gmf_import_)
				{
					gmf_import_ = Result::makeLigandImportResult();
					results_.push_back(gmf_import_);
					gmf_import_->setTimestamp(timestamp_);
					gmf_import_->setToolInfo(toolinfo_);
				}

				Conformation* input_conformation = gmf_current_ligand_->getConformationById(conf_id);
				if(!input_conformation) input_conformation = conformation;
				Result::ResultData rd(Result::LIGANDIMPORT);
				rd.setLigandConformation(input_conformation);
				gmf_import_->add(input_conformation,rd);
			}

			if(mol_has_conf_uid)  // Add Docking/Rescoring result
			{
				if(mol.hasProperty(gmf_property_name_))
				{
					// create a new result-section for all molecules to be written by this function
					if(!gmf_result_)
					{
						gmf_result_ = new Result(gmf_result_method_);
						gmf_result_->setDescription(gmf_result_description_);
						gmf_result_->setTimestamp(timestamp_);
						gmf_result_->setToolInfo(toolinfo_);
						results_.push_back(gmf_result_);
					}

					// If any receptor-conformations are part of this DockResultFile, use the ID of the last inserted conformation. However, if the user has manuall set an ID, then use this one.
					String rec_id = gmf_receptor_conf_UID_;
					if (receptor_ && rec_id == "0")
					{
						rec_id = receptor_->getConformations().back()->getId();
					}

					double score = String(mol.getProperty(gmf_property_name_).toString()).toFloat();
					gmf_result_->add(conf_id,conformation->getId(),gmf_result_method_,score, rec_id);
				}
			}
			return status;
		}

		const vector<Result*>* DockResultFile::getResults()
		{
			return &results_;
		}


		void DockResultFile::writeResult(Result* res)
		{
			if(!mode_read_)
			{
				writeResult(res,*xmlOutResult_);
			}
		}

		void DockResultFile::writeResults(vector<Result*>& res)
		{
			if(!mode_read_)
			{
				for(Position i=0;i<res.size();i++)
				{
					writeResult(res[i],*xmlOutResult_);
					delete res[i];
				}
				results_.clear();
			}
		}

		void DockResultFile::addResult(Result* res)
		{
			results_.push_back(res);
		}


		Size DockResultFile::countConformations()
		{
			HashSet<String> IDs;
			for(Size i=0; i<results_.size(); i++)
			{
				HashSet<String> res_ids = results_[i]->getOutputIds();
				IDs += res_ids;
			}
			return IDs.size();
		}


		Ligand* DockResultFile::readLigand()
					throw(Exception::ParseError)
		{
			current_ligand_ = 0;
			Ligand* ret = 0;
			//bool ligandRead = false;
			while(!xmlIn_->atEnd() && !xmlIn_->hasError())
			{
				QXmlStreamReader::TokenType token = xmlIn_->readNext();
				String tagname = fromQString( xmlIn_->name().toString() );
				tagname.toLower();

				if(token == QXmlStreamReader::StartElement)
				{
					QXmlStreamAttributes attrs = xmlIn_->attributes();
					if(tagname==LIGANDTAG)
					{
						readLigand_();
						//ligandRead = true;
						ret = current_ligand_;
						break;
					}

				}
				if(token == QXmlStreamReader::EndElement)
				{
					if(tagname==LIGANDSTAG)
					{
						break;
					}
				}
			}
			if(xmlIn_->hasError())
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readLigand",String(xmlIn_->errorString().toLatin1().data())));
			}
			return ret;
		}

		void DockResultFile::concatenate()
		{
			int buffersize = 20000;

			file_  = new QFile(filename_.c_str());
			if(!file_->open(QIODevice::WriteOnly| QIODevice::Text))
			{
				throw(Exception::GeneralException(__FILE__, __LINE__,"DockResultFile::DockResultFile","cannot open file for output"));
			}
			String s = "<?xml version=\""+VERSION+"\" encoding=\"UTF-8\"?>\n";
			file_->write(s.c_str());
			file_->write("<dockingfile>");

			resultfile_->open(QIODevice::ReadOnly | QIODevice::Text);
			while(!resultfile_->atEnd())
			{
				QByteArray data = resultfile_->read(buffersize);
				file_->write(data);
			}
			resultfile_->close();

			receptorfile_->open(QIODevice::ReadOnly | QIODevice::Text);
			while(!receptorfile_->atEnd())
			{
				QByteArray data = receptorfile_->read(buffersize);
				file_->write(data);
			}
			receptorfile_->close();

			ligandfile_->open(QIODevice::ReadOnly | QIODevice::Text);
			while(!ligandfile_->atEnd())
			{
				QByteArray data = ligandfile_->read(buffersize);
				file_->write(data);
			}
			ligandfile_->close();

			file_->write("\n</dockingfile>");

		}

		void DockResultFile::close()
		{
			if(!mode_read_)
			{
				if(gmf_last_ligand_)
				{
					writeLigand(gmf_last_ligand_);
					writeResults(results_);
				}
				/*
				else
				{
					writeLigands(*xmlOutLigand_);
					writeResults(results_);
				}
				*/

				if(receptor_!=0)
				{
					writeReceptor(receptor_);
				}

				xmlOutResult_->writeEndElement();
				resultfile_->close();

				xmlOutLigand_->writeEndElement();
				ligandfile_->close();

				xmlOutReceptor_->writeEndElement();
				receptorfile_->close();

				concatenate();
			}

			if(mode_read_ && gmf_new_ligand_read_)
			{
				if(gmf_current_ligand_!=gmf_last_ligand_)
				{
					delete gmf_current_ligand_;
				}
				gmf_current_ligand_ = 0;
			}
			else if(!mode_read_ && gmf_new_ligand_read_)
			{
				delete gmf_last_ligand_;
				gmf_last_ligand_ = 0;
			}

			written_ligand_IDs_.clear();
			file_->close();
			
			// Necessary to perform output compression if enabled
			GenericMolFile::close();
			
			closed_ = true;
		}

		void DockResultFile::addReceptor(Receptor* r)
		{
			receptor_ = r;
		}

		void DockResultFile::writeReceptor(Receptor* receptor)
		{
			if(!mode_read_)
			{
				writeReceptor(receptor,*xmlOutReceptor_);
			}
		}


		bool DockResultFile::writeLigand(Ligand* lig)
		{
			if(!mode_read_)
			{
				return writeLigand(lig,*xmlOutLigand_);
			}
			return false;
		}

		void DockResultFile::addLigand(Ligand* lig)
		{
			ligands_.push_back(lig);
		}


		QString DockResultFile::toQString(const String &s)
		{
			return QString(s.c_str());
		}

		void DockResultFile::writeLigands(QXmlStreamWriter &out)
		{
			vector<Ligand*>::iterator iter = ligands_.begin();
			out.writeStartElement(toQString(LIGANDSTAG));
			for(;iter!=ligands_.end();iter++)
			{
				writeLigand(*iter,out);
			}
			out.writeEndElement();
		}

		void DockResultFile::writeResults(QXmlStreamWriter &out)
		{
			vector<Result*>::iterator iter = results_.begin();
			out.writeStartElement(toQString(RESULTSTAG));
			for(;iter!=results_.end();iter++)
			{
				writeResult(*iter,out);
			}
			out.writeEndElement();
		}

		//
		void DockResultFile::writeReceptors(QXmlStreamWriter &out)
		{
			//vector<Receptor*>::iterator iter = receptors_.begin();
			out.writeStartElement(toQString(RECEPTORSTAG));
			//for(;iter!=receptors_.end();iter++)
			//{
			writeReceptor(receptor_,out);
			//}
			out.writeEndElement();
		}

		void DockResultFile::writeReceptor(Receptor* recep, QXmlStreamWriter &out)
		{
			out.writeStartElement(toQString(RECEPTORTAG));
			// write String as attribute
			String id = recep->getId();
			out.writeAttribute(toQString(RECEPTOR_A_ID),toQString(id));
			// write wrapped protein
			writeProtein((Protein*) recep->getParent(),out);

			// write all conformations
			vector<Conformation*> confs = recep->getConformations();
			vector<Conformation*>::iterator iter = confs.begin();
			out.writeStartElement(toQString(CONFORMATIONSTAG));
			for(;iter!=confs.end();iter++)
			{
				writeConformation(*iter,out);
			}
			out.writeEndElement();

			// write flexibility data
			vector<FlexDefinition> flexis = recep->getFlexDefinitions();
			vector<FlexDefinition>::iterator flexiter = flexis.begin();
			out.writeStartElement(toQString(FLEXIBILITIESTAG));
			for(;flexiter!=flexis.end();flexiter++)
			{
				writeFlexibility(*flexiter,out);
			}
			out.writeEndElement();

			out.writeEndElement();
		}

		//
		void DockResultFile::writeConformation(Conformation* conf,  QXmlStreamWriter &out)
		{
			const HashMap<Size,Vector3>* coords = conf->getAtomPositions();
			String id = conf->getId();
			out.writeStartElement(toQString(CONFORMATIONTAG));
			out.writeAttribute("id",toQString(id));
			HashMap<Size,Vector3>::const_iterator it = coords->begin();
			for(;it!=coords->end();it++)
			{
				out.writeStartElement("coord");
				out.writeAttribute("i",toQString(String(it->first)));
				out.writeAttribute("x",toQString(String(it->second.x)));
				out.writeAttribute("y",toQString(String(it->second.y)));
				out.writeAttribute("z",toQString(String(it->second.z)));
				out.writeEndElement();
			}

			const vector<pair<String,String> >* properties = conf->getProperties();
			for(Size i=0; i<properties->size(); i++)
			{
				out.writeStartElement("property");
				out.writeAttribute(toQString((*properties)[i].first),toQString((*properties)[i].second));
				out.writeEndElement();
			}

			out.writeEndElement();
		}


		void DockResultFile::writeProtein(Protein* prot, QXmlStreamWriter &out)
		{
			ResidueIterator ri;
			out.writeStartElement(toQString(PROTEINTAG));
			out.writeAttribute(toQString(PROTEIN_A_NAME),toQString(prot->getName()));
			BALL_FOREACH_RESIDUE(*prot,ri)
			{
				Residue* res = &(*ri);
				writeResidue(res,out);
			}
			out.writeEndElement();
		}

		void DockResultFile::writeResidue(Residue* res, QXmlStreamWriter &out)
		{
			out.writeStartElement(toQString(RESIDUETAG));
			out.writeAttribute(toQString(RESIDUE_A_NAME),toQString(res->getName()));
			out.writeAttribute(toQString(RESIDUE_A_IDX),toQString(res->getID()));
			AtomIterator pai;
			BALL_FOREACH_ATOM(*res, pai)
			{
				writePDBAtom(&(*pai),out);
			}
			out.writeEndElement();
		}

		void DockResultFile::writePDBAtom(Atom* at, QXmlStreamWriter &out)
		{
			Vector3 coords = at->getPosition();
			out.writeStartElement(toQString(PDBATOMTAG));
			out.writeAttribute(toQString(PDBATOM_A_NAME),toQString(at->getName()));
			out.writeAttribute("x",toQString(String(coords.x)));
			out.writeAttribute("y",toQString(String(coords.y)));
			out.writeAttribute("z",toQString(String(coords.z)));
			out.writeEndElement();
		}

		void DockResultFile::writePDBAtom(PDBAtom* at, QXmlStreamWriter &out)
		{
			writePDBAtom((Atom*) at, out);
		}


		void DockResultFile::writeFlexibility(const FlexDefinition &fd, QXmlStreamWriter &out)
		{
			out.writeStartElement(toQString(FLEXIBILITYTAG));
			vector<Position> fullyflexible = fd.getFullyFlexibleResidues();
			vector<Position>::iterator iter = fullyflexible.begin();
			for(;iter!=fullyflexible.end();iter++)
			{
				writeFullyFlexibleResidue(*iter,out);
			}
			vector<Position> rotamericflexible = fd.getRotamerFlexibleResidues();
			iter = rotamericflexible.begin();
			for(;iter!=rotamericflexible.end();iter++)
			{
				writeRotamericFlexibleResidue(*iter,out);
			}
			out.writeEndElement();
		}

		void DockResultFile::writeFullyFlexibleResidue(Position idx, QXmlStreamWriter &out)
		{
			out.writeStartElement(toQString("fullflexresidue"));
			out.writeAttribute("idx",toQString(String(idx)));
			out.writeEndElement();
		}

		void DockResultFile::writeRotamericFlexibleResidue(Position idx, QXmlStreamWriter &out)
		{
			out.writeStartElement(toQString("rotamericresidue"));
			out.writeAttribute("idx",toQString(String(idx)));
			out.writeEndElement();
		}

		void DockResultFile::writeResult(Result* result, QXmlStreamWriter &out)
		{
			out.writeStartElement(toQString(RESULTTAG));
			out.writeAttribute(toQString(RESULT_A_METHOD),toQString(result->getMethod()));
			out.writeAttribute(toQString(RESULT_A_METHODSTRING),toQString(result->getMethodString()));
			const String& description = result->getDescription();
			if(description!="")
			{
				out.writeAttribute("description",description.c_str());
			}

			out.writeAttribute("toolinfo",result->getToolInfo().c_str());

			out.writeAttribute("time",result->getTimestamp().c_str());

			const vector<String>* inpose_ids = result->getInputConformations();
			vector<String>::const_iterator poseiter = inpose_ids->begin();
			for(;poseiter!=inpose_ids->end();poseiter++)
			{
				out.writeStartElement(toQString(SUBRESULTTAG));
				String inpose_id = *poseiter;
				out.writeAttribute(toQString(SUBRESULT_A_LIGCONFID),toQString(inpose_id));
				const vector<Result::ResultData>* rds = result->get(inpose_id);
				vector<Result::ResultData>::const_iterator dataiter = rds->begin();
				for(;dataiter!=rds->end();dataiter++)
				{
					Result::ResultData rd = *dataiter;
					writeResultData(rd,out);
				}
				out.writeEndElement();
			}
			out.writeEndElement();
		}

		void DockResultFile::writeResultData(const Result::ResultData& rd, QXmlStreamWriter &out)
		{
			out.writeStartElement(toQString(RESULTDATATAG));

			if(rd.hasLigandConformationId())
			{
				String lig_id      = rd.getLigandConformationId();
				out.writeAttribute(toQString(RESULTDATA_A_LIGID),toQString(lig_id));
			}
			if(rd.hasReceptorConformationId())
			{
				String receptor_id = rd.getReceptorConformationId();
				out.writeAttribute(toQString(RESULTDATA_A_CONFID),toQString(receptor_id));
			}
			if(rd.hasScoringMethod())
			{
				String method      = String(rd.getScoringMethod());
				out.writeAttribute(toQString(RESULTDATA_A_METHOD),toQString(method));
			}
			if(rd.hasEnergy())
			{
				String energy      = String(rd.getEnergy());
				out.writeAttribute(toQString(RESULTDATA_A_ENERGY),toQString(energy));
			}

			out.writeEndElement();
		}

		bool DockResultFile::writeLigand(Ligand* lig, QXmlStreamWriter &out)
		{
			String id = lig->getId();

			if(written_ligand_IDs_.has(id))
			{
				if(SHOW_IDENTICAL_LIGANDS_WARNING)
				{
					String name = lig->getParent()->getName();
					if(name=="") name = lig->getId();
					Log.warn() << "[Warning:] Ligand "<<name<<","<<id<<" has already been written to this file, it is thus ignored now."<<endl;
				}
				return false;
			}
			else
			{
				written_ligand_IDs_.insert(id);
			}

			out.writeStartElement(toQString(LIGANDTAG));
			out.writeAttribute(toQString(LIGAND_A_ID),toQString(id));
			writeMolecule(lig->getParent(),out);

			// write all conformations
			vector<Conformation*> confs = lig->getConformations();
			vector<Conformation*>::iterator iter = confs.begin();
			out.writeStartElement(toQString(CONFORMATIONSTAG));
			for(;iter!=confs.end();iter++)
			{
				writeConformation(*iter, out);
			}
			out.writeEndElement();
			out.writeEndElement();
			return true;
		}

		void DockResultFile::writeMolecule(Molecule* molecule, QXmlStreamWriter &out)
		{
			out.writeStartElement(toQString(MOLECULETAG));
			out.writeAttribute(toQString(MOLECULE_A_NAME),toQString(molecule->getName()));

			// preprocess atoms
			int idx = 1;
			AtomIterator a_it = molecule->beginAtom();
			BALL_FOREACH_ATOM(*molecule, a_it)
			{
				Atom* at = &(*a_it);
				at->setProperty("atomidx",idx);
				idx++;
			}
			// write atoms
			out.writeStartElement(toQString(ATOMSTAG));
			a_it = molecule->beginAtom();
			BALL_FOREACH_ATOM(*molecule, a_it)
			{
				Atom* at = &(*a_it);
				writeAtom(at,out);
			}
			out.writeEndElement();

			// write bonds
			out.writeStartElement(toQString(BONDSTAG));
			Atom::BondIterator b_it;
			BALL_FOREACH_BOND(*molecule, a_it, b_it)
			{
				Bond* b = &(*b_it);
				writeBond(b,out);
			}
			// clean-up
			a_it = molecule->beginAtom();
			BALL_FOREACH_ATOM(*molecule, a_it)
			{
				Atom* at = &(*a_it);
				at->clearProperty("atomidx");
			}
			out.writeEndElement();
			out.writeEndElement();
		}

		void DockResultFile::writeAtom(Atom* at, QXmlStreamWriter &out)
		{
			Vector3 coords = at->getPosition();
			String element = at->getElement().getSymbol();
			out.writeStartElement(toQString(ATOMTAG));
			out.writeAttribute(toQString(ATOM_A_ELEMENT),toQString(element));
			out.writeAttribute("x",toQString(String(coords.x)));
			out.writeAttribute("y",toQString(String(coords.y)));
			out.writeAttribute("z",toQString(String(coords.z)));
			float charge = at->getCharge();

			if(fabs(charge)>1e-03)
			{
				out.writeAttribute("chrg",String(charge).c_str());
			}
			out.writeEndElement();
		}

		void DockResultFile::writeBond(Bond* b, QXmlStreamWriter &out)
		{
			const Atom* at1  = b->getFirstAtom();
			const Atom* at2  = b->getSecondAtom();

			// each atom of a molecule has a index
			// which is set in writeMolecule
			String from(at1->getProperty("atomidx").getInt());
			String to(at2->getProperty("atomidx").getInt());

			String order(b->getOrder());
			out.writeStartElement(toQString(BONDTAG));
			out.writeAttribute(toQString(BOND_A_FROM),toQString(from));
			out.writeAttribute(toQString(BOND_A_TO),toQString(to));
			out.writeAttribute(toQString(BOND_A_ORDER),toQString(order));
			out.writeEndElement();
		}


		bool DockResultFile::readResults_()
					throw(Exception::ParseError)
		{
			while(!xmlIn_->atEnd() && !xmlIn_->hasError())
			{
				QXmlStreamReader::TokenType token = xmlIn_->readNext();
				String tagname = fromQString( xmlIn_->name().toString() );
				tagname.toLower();
				if(token == QXmlStreamReader::StartElement)
				{
					if(tagname==RESULTTAG)
					{
						readResult();
						this->addResult(current_result_);
					}
				}
				if(token == QXmlStreamReader::EndElement)
				{
					if(tagname==RESULTSTAG)
					{
						break;
					}
				}
			}
			if(xmlIn_->hasError())
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readResults_",String(xmlIn_->errorString().toLatin1().data())));
			}
			return true;
		}

		bool DockResultFile::readResult()
					throw(Exception::ParseError)
		{
			HashMap<String,String> attrs;
			attributesToHashMap(xmlIn_->attributes(),attrs);

			if(!attrs.has(RESULT_A_METHOD))
				 throw Exception::ParseError(__FILE__, __LINE__, "DockResultFile::readResult", "result method attribute is missing");
			String resulttype = attrs[RESULT_A_METHOD];
			current_result_ = new Result(Result::getMethod(resulttype.toInt()));

			String description = "";
			HashMap<String,String>::iterator it=attrs.find("description");
			if(it!=attrs.end())
			{
				description = it->second;
				current_result_->setDescription(description);
			}

			it=attrs.find("time");
			if(it!=attrs.end())
			{
				String time = it->second;
				current_result_->setTimestamp(time);
			}

			it=attrs.find("toolinfo");
			if(it!=attrs.end())
			{
				String toolinfo = it->second;
				current_result_->setToolInfo(toolinfo);
			}

			while(!xmlIn_->atEnd() && !xmlIn_->hasError())
			{
				QXmlStreamReader::TokenType token = xmlIn_->readNext();
				String tagname = fromQString( xmlIn_->name().toString() );
				tagname.toLower();
				// read all subresults ...
				if(token == QXmlStreamReader::StartElement)
				{
					QXmlStreamAttributes attrs = xmlIn_->attributes();
					if(tagname==SUBRESULTTAG)
					{
						readSubResult();
					}
				}
				// until end of section
				if(token == QXmlStreamReader::EndElement)
				{
					if(tagname==RESULTTAG)
					{
						break;
					}
				}
			}
			if(xmlIn_->hasError())
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readResult",String(xmlIn_->errorString().toLatin1().data())));
			}
			return true;
		}

		bool DockResultFile::readSubResult()
					throw(Exception::ParseError)
		{
			HashMap<String,String> attrs;
			attributesToHashMap(xmlIn_->attributes(),attrs);
			if(!attrs.has(SUBRESULT_A_LIGCONFID))
				throw Exception::ParseError(__FILE__, __LINE__, "DockResultFile::readSubResult", "sub result id attribute is missing" );
			String inputpose = attrs[SUBRESULT_A_LIGCONFID];
			current_Result_InputPoseId_ = inputpose;

			while(!xmlIn_->atEnd() && !xmlIn_->hasError())
			{
				QXmlStreamReader::TokenType token = xmlIn_->readNext();
				String tagname = fromQString( xmlIn_->name().toString() );
				tagname.toLower();
				if(token == QXmlStreamReader::StartElement)
				{
					QXmlStreamAttributes attrs = xmlIn_->attributes();
					if(tagname==RESULTDATATAG)
					{
						readEntry();
					}
				}
				if(token == QXmlStreamReader::EndElement)
				{
				  if(tagname==SUBRESULTTAG)
					{
						break;
					}
				}
			}
			if(xmlIn_->hasError())
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readSubResult",String(xmlIn_->errorString().toLatin1().data())));
			}
			return true;
		}

		bool DockResultFile::readEntry()
					throw(Exception::ParseError)
		{
			Result::ResultData rd;
			HashMap<String,String> attrs;
			attributesToHashMap(xmlIn_->attributes(),attrs);
			HashMap<String,String>::Iterator iter = attrs.begin();
			for(;iter!=attrs.end();iter++)
			{
				const String& key   = iter->first;
				//key.toLower();
				const String& value = iter->second;

				if(key==RESULTDATA_A_ENERGY)
				{
					float energy;
					bool ok = retrieveFloat(value,energy);
					if(!ok)
						throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readEntry","found invalid energy (non-numeric)"));
					rd.setEnergy(energy);
				}
				if(key==RESULTDATA_A_METHOD)
				{
					int method;
					bool ok = retrieveInt(value,method);
					if(!ok)
						throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readEntry","found invalid method id (non-numeric)"));
					rd.setScoringMethod(method);
				}
				if(key==RESULTDATA_A_CONFID)
				{
					rd.setReceptorConformationId(value);
				}
				if(key==RESULTDATA_A_LIGID)
				{
					rd.setLigandConformationId(value);
				}
			}
			current_result_->add(current_Result_InputPoseId_,rd);
			return true;
		}

		bool DockResultFile::readLigands()
					throw(Exception::ParseError)
		{
			while(!xmlIn_->atEnd() && !xmlIn_->hasError())
			{
				QXmlStreamReader::TokenType token = xmlIn_->readNext();
				String tagname = fromQString( xmlIn_->name().toString() );
				tagname.toLower();
				if(token == QXmlStreamReader::StartElement)
				{
					QXmlStreamAttributes attrs = xmlIn_->attributes();
					if(tagname==LIGANDTAG)
					{
						readLigand();
					}
				}
				if(token == QXmlStreamReader::EndElement)
				{
					if(tagname==LIGANDTAG)
					{
						break;
					}
				}
			}
			if(xmlIn_->hasError())
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readLigands",String(xmlIn_->errorString().toLatin1().data())));
			}
			return true;
		}

		void DockResultFile::buildLigand()
				throw(Exception::ParseError)
		{
			current_ligand_ = new Ligand(current_molecule_);
			current_ligand_->setId(current_ligand_id_);
			//addLigand(currentLigand);
		}

		void DockResultFile::buildReceptor()
				throw(Exception::ParseError)
		{
			current_receptor_ = new Receptor(current_protein_);
			current_receptor_->setId(current_receptor_id_);
			//addReceptor(currentReceptor);
		}

		bool DockResultFile::readLigand_()
					throw(Exception::ParseError)
		{
			HashMap<String,String> attrs;
			attributesToHashMap(xmlIn_->attributes(),attrs);
			if(!attrs.has(LIGAND_A_ID))
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readLigand_","found no id for ligand"));
			String ligand_id = attrs[LIGAND_A_ID];
			current_ligand_id_ = ligand_id;
			while(!xmlIn_->atEnd() && !xmlIn_->hasError())
			{
				QXmlStreamReader::TokenType token = xmlIn_->readNext();
				String tagname = fromQString( xmlIn_->name().toString() );
				tagname.toLower();
				if(token == QXmlStreamReader::StartElement)
				{
					if(tagname==MOLECULETAG)
					{
						readMolecule();
						buildLigand();
					}
					if(tagname==CONFORMATIONSTAG)
					{
						readConformations(current_ligand_);
					}
				}
				if(token == QXmlStreamReader::EndElement)
				{
					if(tagname==LIGANDTAG)
					{
						addLigand(current_ligand_);
						break;
					}
				}
			}
			if(xmlIn_->hasError())
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readLigand_",String(xmlIn_->errorString().toLatin1().data())));
			}

			return true;
		}


		bool DockResultFile::readFlexibilities()
					throw(Exception::ParseError)
		{
			flexdef_idx = 0;
			while(!xmlIn_->atEnd() && !xmlIn_->hasError())
			{
				QXmlStreamReader::TokenType token = xmlIn_->readNext();
				String tagname = fromQString( xmlIn_->name().toString() );
				tagname.toLower();

				if(token == QXmlStreamReader::StartElement)
				{
					if(tagname==FLEXIBILITYTAG)
					{
						readFlexibility();
						current_receptor_->setFlexDefinition(flexdef_idx,current_flexdef_);
						flexdef_idx++;
					}
				}
				if(token == QXmlStreamReader::EndElement)
				{
					if(tagname==FLEXIBILITIESTAG)
					{
						break;
					}
				}
			}
			if(xmlIn_->hasError())
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readFlexibilities",String(xmlIn_->errorString().toLatin1().data())));
			}
			return true;
		}

		bool DockResultFile::readConformations(FlexibleMolecule* target)
					throw(Exception::ParseError)
		{
			target->clearConformations();

			while(!xmlIn_->atEnd() && !xmlIn_->hasError())
			{
				QXmlStreamReader::TokenType token = xmlIn_->readNext();
				String tagname = fromQString( xmlIn_->name().toString() );
				tagname.toLower();
				if(token == QXmlStreamReader::StartElement)
				{
					if(tagname==CONFORMATIONTAG)
					{
						Conformation* conformation = new Conformation;
						readConformation(conformation);
						conformation->setAtomPositions(target->getParent(),current_conformation_data_);
						conformation->setID(current_conformation_id_);
// 						if(!conformation->checkID(current_conformation_id_))
// 						{
// 							throw(Exception::ParseError(__FILE__, __LINE__,"\rFile corruption in input file '"+filename_+"' detected!! Conformations stored in this file have been accidently modified!",""));
// 						}
						current_conformation_data_.clear();

						target->addConformation(conformation);
					}
				}
				if(token == QXmlStreamReader::EndElement)
				{
					if(tagname==CONFORMATIONSTAG)
					{
						//buildLigand();
						break;
					}
				}
			}
			if(xmlIn_->hasError())
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readConformations",String(xmlIn_->errorString().toLatin1().data())));
			}
			return true;
		}

		bool DockResultFile::readConformation(Conformation* conformation)
					throw(Exception::ParseError)
		{
			HashMap<String,String> attrs;
			attributesToHashMap(xmlIn_->attributes(),attrs);
			if(!attrs.has(CONFORMATION_A_ID))
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readConformation","found no id for conformation"));
			current_conformation_id_ = attrs[CONFORMATION_A_ID];

			while(!xmlIn_->atEnd() && !xmlIn_->hasError())
			{
				QXmlStreamReader::TokenType token = xmlIn_->readNext();
				String tagname = fromQString( xmlIn_->name().toString() );
				tagname.toLower();
				if(token == QXmlStreamReader::StartElement)
				{
					if(tagname=="coord")
					{
						readCoordinates();
					}
					if(tagname=="property")
					{
						HashMap<String,String> attrs;
						attributesToHashMap(xmlIn_->attributes(),attrs);
						HashMap<String,String>::Iterator it = attrs.begin();
						for(;it!=attrs.end();it++)
						{
							conformation->addProperty(it->first,it->second);
						}
					}
				}
				if(token == QXmlStreamReader::EndElement)
				{
					if(tagname==CONFORMATIONTAG)
					{
						break;
					}
				}
			}
			if(xmlIn_->hasError())
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readConformation",String(xmlIn_->errorString().toLatin1().data())));
			}
			return true;
		}

		bool DockResultFile::readFlexibility()
					throw(Exception::ParseError)
		{
			current_flexdef_ = FlexDefinition();
			while(!xmlIn_->atEnd() && !xmlIn_->hasError())
			{
				QXmlStreamReader::TokenType token = xmlIn_->readNext();
				String tagname = fromQString( xmlIn_->name().toString() );
				tagname.toLower();
				if(token == QXmlStreamReader::StartElement)
				{
					if(tagname=="fullflexresidue")
					{
						readFullFlexResidue();
					}
					if(tagname=="rotamericresidue")
					{
						readRotamericResidue();
					}
				}
				if(token == QXmlStreamReader::EndElement)
				{
					if(tagname==FLEXIBILITYTAG)
					{
						break;
					}
				}
			}
			if(xmlIn_->hasError())
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readFlexibility",String(xmlIn_->errorString().toLatin1().data())));
			}
			return true;
		}

		bool DockResultFile::readFullFlexResidue()
					throw(Exception::ParseError)
		{
			HashMap<String,String> attrs;
			attributesToHashMap(xmlIn_->attributes(),attrs);
			HashMap<String,String>::Iterator iter = attrs.begin();
			bool hasIdx = false;
			for(;iter!=attrs.end();iter++)
			{
				const String& key   = iter->first;
				//key.toLower();
				const String& value = iter->second;
				if(key=="idx")
				{
					int idx;
					bool ok = retrieveInt(value,idx);
					if(!ok)
						throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readFullFlexResidue","found invalid index (non-numeric)"));
					hasIdx = true;
					current_flexdef_.addFullyFlexibleResidue(idx);
				}
			}
			if(!hasIdx)
					throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readFullFlexResidue","found no index"));
			return true;
		}

		bool DockResultFile::readRotamericResidue()
					throw(Exception::ParseError)
		{
			HashMap<String,String> attrs;
			attributesToHashMap(xmlIn_->attributes(),attrs);
			HashMap<String,String>::Iterator iter = attrs.begin();
			bool hasIdx = false;
			for(;iter!=attrs.end();iter++)
			{
				const String& key   = iter->first;
				//key.toLower();
				const String& value = iter->second;
				if(key=="idx")
				{
					int idx;
					bool ok = retrieveInt(value,idx);
					if(!ok)
						throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readRotamericResidue","found invalid index (non-numeric)"));
					hasIdx = true;
					current_flexdef_.addRotamerFlexibleResidue(idx);
				}
			}
			if(!hasIdx)
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readRotamericResidue","found no index"));

			return true;
		}



		bool DockResultFile::readCoordinates()
					throw(Exception::ParseError)
		{
			HashMap<String,String> attrs;
			attributesToHashMap(xmlIn_->attributes(),attrs);
			HashMap<String,String>::Iterator iter = attrs.begin();
			bool hasX = false; bool hasY = false; bool hasZ = false;
			bool hasI = false;
			Size index = 0;
			for(;iter!=attrs.end();iter++)
			{
				const String& key   = iter->first;
				//key.toLower();
				const String& value = iter->second;
				if(key=="i")
				{
					hasI = true;
					index = value.toUnsignedInt();
				}
				if(key=="x")
				{
					bool is_ok = setCoordinate(0, value);
					if(!is_ok)
						throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readCoordinates","found invalid x-coordinate (non-numeric)"));
					hasX = true;
				}
				if(key=="y")
				{
					bool is_ok = setCoordinate(1, value);
					if(!is_ok)
						throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readCoordinates","found invalid y-coordinate (non-numeric)"));
					hasY = true;
				}
				if(key=="z")
				{
					bool is_ok = setCoordinate(2, value);
					if(!is_ok)
						throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readCoordinates","found invalid z-coordinate (non-numeric)"));
					hasZ = true;
				}
			}
			if(!hasX)
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readCoordinates","no x-coordinate found"));
			}
			if(!hasY)
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readCoordinates","no y-coordinate found"));
			}
			if(!hasZ)
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readCoordinates","no z-coordinate found"));
			}
			if(!hasI)
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readCoordinates","no atom-index found"));
			}

			current_conformation_data_.insert(make_pair(index,current_coords_));
			return true;
		}


		void DockResultFile::buildMolecule()
			throw(Exception::ParseError)
		{
			current_molecule_ = new Molecule();
			current_molecule_->setName(current_molecule_name_);
			//connect atoms
			int nAtoms = current_atoms_.size();
			int nBonds = bonds_from_.size();

			for(int i=0;i<nBonds;i++)
			{
				if(bonds_from_[i]>=0 && bonds_from_[i]<nAtoms)
				{
					if(bonds_to_[i]>=0 && bonds_to_[i]<nAtoms)
					{
						Atom* at1 = current_atoms_[bonds_from_[i]];
						Atom* at2 = current_atoms_[bonds_to_[i]];
						Bond* b = at1->createBond(*at2);
						b->setOrder(bonds_order_[i]);
					}
					else
						throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::buildMolecule","2nd atom of bond #"+String(i)+" does not exist"));
				}
				else
					throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::buildMolecule","1st atom of bond #"+String(i)+" does not exist"));
			}
			// add atoms to molecule
			for(int i=0;i<nAtoms;i++)
			{
				current_molecule_->append(*current_atoms_[i]);
			}
			// clear cache
			bonds_order_.clear();
			bonds_from_.clear();
			bonds_to_.clear();
			current_atoms_.clear();
		}

		bool DockResultFile::readMolecule()
					throw(Exception::ParseError)
		{
			HashMap<String,String> attrs;
			attributesToHashMap(xmlIn_->attributes(),attrs);
			if(attrs.has("name"))
				current_molecule_name_ = attrs["name"];
			else
				current_molecule_name_ = "";
			while(!xmlIn_->atEnd() && !xmlIn_->hasError())
			{
				QXmlStreamReader::TokenType token = xmlIn_->readNext();
				String tagname = fromQString( xmlIn_->name().toString() );
				tagname.toLower();
				if(token == QXmlStreamReader::StartElement)
				{
					if(tagname==ATOMSTAG)
					{
						readAtoms();
					}
					if(tagname==BONDSTAG)
					{
						readBonds();
					}
				}
				if(token == QXmlStreamReader::EndElement)
				{
					if(tagname==MOLECULETAG)
					{
						buildMolecule();
						break;
					}
				}
			}
			if(xmlIn_->hasError())
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readMolecule",String(xmlIn_->errorString().toLatin1().data())));
			}
			return true;
		}

		bool DockResultFile::readAtoms()
					throw(Exception::ParseError)
		{
			while(!xmlIn_->atEnd() && !xmlIn_->hasError())
			{
				QXmlStreamReader::TokenType token = xmlIn_->readNext();
				String tagname = fromQString( xmlIn_->name().toString() );
				tagname.toLower();
				if(token == QXmlStreamReader::StartElement)
				{
					QXmlStreamAttributes attrs = xmlIn_->attributes();
					if(tagname==ATOMTAG)
					{
						readAtom();
					}
				}
				if(token == QXmlStreamReader::EndElement)
				{
					if(tagname==ATOMSTAG)
							break;
				}
			}
			if(xmlIn_->hasError())
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readAtoms",String(xmlIn_->errorString().toLatin1().data())));
			}
			return true;
		}

		bool DockResultFile::readAtom()
					throw(Exception::ParseError)
		{
			current_atom_ = new Atom();
			HashMap<String,String> attrs;
			attributesToHashMap(xmlIn_->attributes(),attrs);
			HashMap<String,String>::Iterator iter = attrs.begin();
			bool hasElement = false; bool hasX = false; bool hasY = false; bool hasZ = false;
			for(;iter!=attrs.end();iter++)
			{
				const String& key   = iter->first;
				//key.toLower();
				const String& value = iter->second;

				if(key==ATOM_A_ELEMENT)
				{
					current_atom_->setElement(PTE[value]);
					if(current_atom_->getElement()== Element::UNKNOWN)
					{
						throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readAtom","found invalid element"));
					}
					hasElement = true;
				}
				if(key=="x")
				{
					bool is_ok = setCoordinate(0, value);
					if(!is_ok)
						throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readAtom","found invalid x-coordinate (non-numeric)"));
					hasX = true;
				}
				if(key=="y")
				{
					bool is_ok = setCoordinate(1, value);
					if(!is_ok)
						throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readAtom","found invalid y-coordinate (non-numeric)"));
					hasY = true;
				}
				if(key=="z")
				{
					bool is_ok = setCoordinate(2, value);
					if(!is_ok)
						throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readAtom","found invalid z-coordinate (non-numeric)"));
					hasZ = true;
				}
				if(key=="chrg")
				{
					current_atom_->setCharge(value.toDouble());
				}
			}
			if(!hasElement)
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readAtom","no element for atom found"));
			if(!hasX)
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readAtom","no x-coordinate for atom found"));
			if(!hasY)
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readAtom","no y-coordinate for atom found"));
			if(!hasZ)
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readAtom","no z-coordinate for atom found"));

			current_atom_->setPosition(current_coords_);
			current_atoms_.push_back(current_atom_);
			return true;
		}

		bool DockResultFile::readBonds()
					throw(Exception::ParseError)
		{
			while(!xmlIn_->atEnd() && !xmlIn_->hasError())
			{
				QXmlStreamReader::TokenType token = xmlIn_->readNext();
				String tagname = fromQString( xmlIn_->name().toString() );
				tagname.toLower();
				if(token == QXmlStreamReader::StartElement)
				{
					QXmlStreamAttributes attrs = xmlIn_->attributes();
					if(tagname==BONDTAG)
					{
						readBond();
					}
				}
				if(token == QXmlStreamReader::EndElement)
				{
					if(tagname==BONDSTAG)
						break;
				}
			}
			if(xmlIn_->hasError())
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readBonds",String(xmlIn_->errorString().toLatin1().data())));
			}
			return true;
		}

		bool DockResultFile::readBond()
					throw(Exception::ParseError)
		{
			HashMap<String,String> attrs;
			attributesToHashMap(xmlIn_->attributes(),attrs);
			HashMap<String,String>::ConstIterator iter = attrs.begin();
			for(;iter!=attrs.end();iter++)
			{
				const String& key   = iter->first;
				//key.toLower();
				const String& value = iter->second;
				int idx=-1;
				bool ret = retrieveInt(value,idx);
				if(!ret||idx<0)
					throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readBond","found invalid atom index"));
				if(key==BOND_A_ORDER)
				{
					if(idx>0&&idx<6)
						bonds_order_.push_back(idx);
					else
						throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readBond","found invalid bond order"));
				}
				if(key==BOND_A_FROM)
				{
					bonds_from_.push_back( idx-1 ) ;
				}
				if(key==BOND_A_TO)
				{
					bonds_to_.push_back( idx-1 ) ;
				}
			}
			return true;
		}


		bool DockResultFile::readReceptors_()
					throw(Exception::ParseError)
		{
			while(!xmlIn_->atEnd() && !xmlIn_->hasError())
			{
				QXmlStreamReader::TokenType token = xmlIn_->readNext();
				String tagname = fromQString( xmlIn_->name().toString() );
				tagname.toLower();
				if(token == QXmlStreamReader::StartElement)
				{
					QXmlStreamAttributes attrs = xmlIn_->attributes();
					if(tagname==RECEPTORTAG)
					{
						readReceptor();
					}
				}
				if(token == QXmlStreamReader::EndElement)
				{
					if(tagname==RECEPTORSTAG)
					{
						break;
					}
				}
			}
			if(xmlIn_->hasError())
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readReceptors_",String(xmlIn_->errorString().toLatin1().data())));
			}
			return true;
		}

		Receptor* DockResultFile::readReceptor()
				throw(Exception::ParseError)
		{
			current_receptor_ = 0;
			readReceptor_();
			return receptor_;
		}

		bool DockResultFile::readReceptor_()
					throw(Exception::ParseError)
		{
			HashMap<String,String> attrs;
			attributesToHashMap(xmlIn_->attributes(),attrs);
			String receptorid = attrs["id"];
			current_receptor_id_ = receptorid;
			while(!xmlIn_->atEnd() && !xmlIn_->hasError())
			{

				QXmlStreamReader::TokenType token = xmlIn_->readNext();
				String tagname = fromQString( xmlIn_->name().toString() );
				tagname.toLower();
				if(token == QXmlStreamReader::StartElement)
				{
					if(tagname==PROTEINTAG)
					{
						readProtein();
						buildReceptor();
					}
					if(tagname==CONFORMATIONSTAG)
					{
						readConformations(current_receptor_);
					}
					if(tagname==FLEXIBILITIESTAG)
					{
						readFlexibilities();
					}
				}
				if(token == QXmlStreamReader::EndElement)
				{
					if(tagname==RECEPTORSTAG)
					{
						// we currently have only one receptor
						// make receptor_ a vector to store more
						// receptors
						receptor_ = current_receptor_;
						break;
					}
				}
			}
			if(xmlIn_->hasError())
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readReceptor_",String(xmlIn_->errorString().toLatin1().data())));
			}
			return receptor_;
		}

		bool DockResultFile::readProtein()
					throw(Exception::ParseError)
		{
			current_chain_ = new Chain;
			HashMap<String,String> attrs;
			attributesToHashMap(xmlIn_->attributes(),attrs);
			current_protein_name_ = attrs["name"];
			while(!xmlIn_->atEnd() && !xmlIn_->hasError())
			{
				QXmlStreamReader::TokenType token = xmlIn_->readNext();
				String tagname = fromQString( xmlIn_->name().toString() );
				tagname.toLower();
				if(token == QXmlStreamReader::StartElement)
				{
					if(tagname==RESIDUETAG)
					{
						readResidue();
					}
				}
				if(token == QXmlStreamReader::EndElement)
				{
					if(tagname==PROTEINTAG)
					{
						buildProtein();
						break;
					}
				}
			}
			if(xmlIn_->hasError())
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readProtein",String(xmlIn_->errorString().toLatin1().data())));
			}
			return true;
		}

		void DockResultFile::buildProtein()
				throw(Exception::ParseError)
		{
			current_protein_ = new Protein();
			current_protein_->insert(*current_chain_);
			current_protein_->setName(current_protein_name_);
		}

		void DockResultFile::buildResidue()
				throw(Exception::ParseError)
		{
			for(unsigned int i=0;i<current_PDB_atoms_.size();i++)
			{
				PDBAtom* at = current_PDB_atoms_[i];
				current_residue_->insert(*at);
			}
			if(isAminoAcid(current_residue_->getName()))
				current_residue_->setProperty(Residue::PROPERTY__AMINO_ACID);
			current_chain_->insert(*current_residue_);
			current_PDB_atoms_.clear();
		}

		bool DockResultFile::readResidue()
					throw(Exception::ParseError)
		{
			HashMap<String,String> attrs;
			attributesToHashMap(xmlIn_->attributes(),attrs);
			current_residue_ = new Residue;
			if(!attrs.has("name"))
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readResidue","no name for residue found"));
			current_residue_->setName(attrs["name"]);
			if(!attrs.has("id"))
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readResidue","no id for residue found"));
			current_residue_->setID(attrs["id"]);
			while(!xmlIn_->atEnd() && !xmlIn_->hasError())
			{
				QXmlStreamReader::TokenType token = xmlIn_->readNext();
				String tagname = fromQString( xmlIn_->name().toString() );
				tagname.toLower();
				if(token == QXmlStreamReader::StartElement)
				{
					if(tagname==PDBATOMTAG)
					{
						readPDBAtom();
					}
				}
				if(token == QXmlStreamReader::EndElement)
				{
					if(tagname==RESIDUETAG)
					{
						buildResidue();
						break;
					}
				}
			}
			if(xmlIn_->hasError())
			{
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readResidue",String(xmlIn_->errorString().toLatin1().data())));
			}
			return true;
		}

		bool DockResultFile::readPDBAtom()
					throw(Exception::ParseError)
		{
			PDBAtom* atm = new PDBAtom();
			HashMap<String,String> attrs;
			attributesToHashMap(xmlIn_->attributes(),attrs);
			HashMap<String,String>::Iterator iter = attrs.begin();
			bool hasName = false;
			bool hasX    = false;
			bool hasY    = false;
			bool hasZ    = false;
			for(;iter!=attrs.end();iter++)
			{
				const String& key   = iter->first;
				//key.toLower();
				const String& value = iter->second;

				if(key==PDBATOM_A_NAME)
				{
					atm->setName(value);
					// unsafe ?!
					if(value.substr(0,1)=="1"||value.substr(0,1)=="2"||value.substr(0,1)=="3")
						atm->setElement(PTE["H"]);
					else
						atm->setElement(PTE[value.substr(0,1)]);
					if(atm->getElement()== Element::UNKNOWN)
					{
						throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readPDBAtom","found invalid element"));
					}
					hasName = true;
				}
				if(key=="x")
				{
					bool is_ok = setCoordinate(0, value);
					if(!is_ok)
						throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readPDBAtom","found invalid x-coordinate"));
					hasX = true;
				}
				if(key=="y")
				{
					bool is_ok = setCoordinate(1, value);
					if(!is_ok)
						throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readPDBAtom","found invalid y-coordinate"));
					hasY = true;
				}
				if(key=="z")
				{
					bool is_ok = setCoordinate(2, value);
					if(!is_ok)
						throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readPDBAtom","found invalid z-coordinate"));
					hasZ = true;
				}
			}
			if(!hasName)
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readPDBAtom","no name for atom found"));
			if(!hasX)
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readPDBAtom","no x-coordinate for atom found"));
			if(!hasY)
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readPDBAtom","no y-coordinate for atom found"));
			if(!hasZ)
				throw(Exception::ParseError(__FILE__, __LINE__,"DockResultFile::readPDBAtom","no z-coordinate for atom found"));

			atm->setPosition(current_coords_);
			current_PDB_atoms_.push_back(atm);
			return true;
		}

		String DockResultFile::fromQString(const QString &s)
		{
			return String(s.toLatin1().data());
		}

		void DockResultFile::attributesToHashMap(const QXmlAttributes& attributes, HashMap<String,String>& map)
		{
			map.clear();
			for(int i=0;i<attributes.length();i++)
			{
				map[attributes.qName(i).toStdString()] = attributes.value(i).toStdString();
			}
		}

		void DockResultFile::attributesToHashMap(const QXmlStreamAttributes &attributes, HashMap<String,String>& map)
		{
			map.clear();
			for(int i=0;i<attributes.size();i++)
			{
				map[attributes.at(i).name().toString().toStdString()] = attributes.at(i).value().toString().toStdString();
			}
		}

		bool DockResultFile::retrieveInt(const String& s, int &out)
		{
			int d = 0;
			try
			{
				d = String(s).toInt();
			}
			catch(Exception::InvalidFormat& e)
			{
				return false;
			}
			out = d;
			return true;
		}

		bool DockResultFile::retrieveFloat(const String& s, float &out)
		{
			float d = 0;
			try
			{
				d = String(s).toFloat();
			}
			catch(Exception::InvalidFormat& e)
			{
				return false;
			}
			out = d;
			return true;
		}

		bool DockResultFile::isAminoAcid(String code)
		{
			static FragmentDB db("fragments/Fragments.db");
			code.toUpper();
			// this makes HOH also an PDB residue (ok?)
			return db.getResidue(code)!=0;
		}

		bool DockResultFile::setCoordinate(const int& idx , const String& coord)
		{
			double d = 0.0;
			try
			{
				d = coord.toDouble();
			}
			catch(Exception::InvalidFormat& e)
			{
				return false;
			}

			switch(idx)
			{
				case 0:
					current_coords_.x = d;
					break;
				case 1:
					current_coords_.y = d;
					break;
				case 2:
					current_coords_.z = d;
					break;
			}
			return true;
		}

		String DockResultFile::ROOTTAG          = "dockingfile";
		String DockResultFile::RECEPTORSTAG     = "receptors";
		String DockResultFile::RECEPTORTAG      = "receptor";
		String DockResultFile::RECEPTOR_A_ID    = "id";
		String DockResultFile::PROTEINTAG       = "protein";
		String DockResultFile::PROTEIN_A_NAME   = "name";
		String DockResultFile::RESIDUETAG       = "residue";
		String DockResultFile::RESIDUE_A_NAME   = "name";
		String DockResultFile::RESIDUE_A_IDX    = "id";
		String DockResultFile::PDBATOMTAG       = "pdbatom";
		String DockResultFile::PDBATOM_A_NAME   = "name";

		String DockResultFile::CONFORMATIONSTAG = "conformations";
		String DockResultFile::CONFORMATIONTAG  = "conformation";
		String DockResultFile::CONFORMATION_A_ID  = "id";
		String DockResultFile::FLEXIBILITIESTAG = "flexibilities";
		String DockResultFile::FLEXIBILITYTAG   = "flexibility";

		String DockResultFile::LIGANDSTAG       = "ligands";
		String DockResultFile::LIGANDTAG        = "ligand";
		String DockResultFile::LIGAND_A_ID      = "id";
		String DockResultFile::MOLECULETAG      = "molecule";
		String DockResultFile::MOLECULE_A_NAME  = "name";
		String DockResultFile::ATOMSTAG         = "atoms";
		String DockResultFile::ATOMTAG          = "atom";
		String DockResultFile::ATOM_A_ELEMENT   = "element";
		String DockResultFile::BONDSTAG         = "bonds";
		String DockResultFile::BONDTAG          = "bond";
		String DockResultFile::BOND_A_FROM      = "from";
		String DockResultFile::BOND_A_TO        = "to";
		String DockResultFile::BOND_A_ORDER     = "order";

		String DockResultFile::RESULTSTAG       = "results";
		String DockResultFile::RESULTTAG        = "result";
		String DockResultFile::RESULT_A_METHOD  = "method";
		String DockResultFile::RESULT_A_METHODSTRING  = "methodstring";
		String DockResultFile::SUBRESULTTAG     = "subresult";
		String DockResultFile::SUBRESULT_A_LIGCONFID = "lconf";
		String DockResultFile::RESULTDATATAG       = "entry";
		String DockResultFile::RESULTDATA_A_LIGID  = "lconf";
		String DockResultFile::RESULTDATA_A_CONFID = "rconf";
		String DockResultFile::RESULTDATA_A_ENERGY = "energy";
		String DockResultFile::RESULTDATA_A_METHOD = "method";

}

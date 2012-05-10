// ----------------------------------------------------
// $Maintainer: Marc Röttig $
// $Authors: Marc Röttig, Marcel Schumann $
// ----------------------------------------------------

#ifndef BALL_FORMAT_DOCKRESULTFILE_H
#define BALL_FORMAT_DOCKRESULTFILE_H

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/DATATYPE/hashMap.h>
#include <BALL/DOCKING/COMMON/result.h>
#include <BALL/DOCKING/COMMON/receptor.h>

#include <QtXml/QXmlDefaultHandler>
#include <QtXml/QXmlStreamReader>
#include <QtXml/QXmlStreamWriter>
#include <QtCore/QFile>
#include <QtCore/QTemporaryFile>

#include <vector>

namespace BALL
{
		/**     DockResultFile class.
		        Used to store docking results.
		 */
		class BALL_EXPORT DockResultFile : public GenericMolFile
		{
			public:

				static bool SHOW_IDENTICAL_LIGANDS_WARNING;

				BALL_CREATE(DockResultFile)

				DockResultFile();
				DockResultFile(const String& name, File::OpenMode open_mode = std::ios::in);

				virtual ~DockResultFile() throw();

				/** Read receptor object.
							@return Receptor* pointer to the read Receptor object
				 */
				Receptor* readReceptor() throw(Exception::ParseError);

				/** Write receptor object (directly).
							@param pointer to Receptor object
				 */
				void writeReceptor(Receptor* s);

				/** Read receptor object.
							@return Ligand* pointer to the read Ligand object
				 */
				Ligand* readLigand() throw(Exception::ParseError);

				/** Write Ligand object (directly).
							@param pointer to Ligand object
				 */
				bool writeLigand(Ligand* lig);

				/** Write Result object (directly).
							@param pointer to Result object
				 */
				void writeResult(Result* res);

				/** Write vector of Result objects (directly).
							@param vector of pointers to Result objects
				 */
				void writeResults(vector<Result*>& res);

				/** Read result objects.
							@return vector of Result pointers
				 */
				vector<Result*> readResults() throw(Exception::ParseError);

				/**	Close file.  */
				void close();

				const vector<Result*>* getResults();
				void addResult(Result* res);

				/** Return the number of unique conformations currently stored in the result-sections of this DockResultFile. */
				Size countConformations();


				/**	@name GenericMolecule interface functions */
				//@{

				Molecule* read() throw (Exception::ParseError);
				bool write(const Molecule& mol) throw (File::CannotWrite);

				void setOutputParameters(Result::Method, String property_name, String& receptor_conf_UID, String method_description="");

				void setToolInfo(const String& toolinfo, const String& timestamp);

				void setInputParameters(list<Size>& selected_results);

				void selectAllResultsForInput();

				/** Disables the automatic creation of Results-items when using write(const Molecule& mol). However, this is only necessary when merging/sorting DockingFiles when Results are set manually. */
				void disableAutomaticResultCreation();

				/** Return the current Ligand object that was created by the last call of read() or write(const Molecule& mol). */
				const FlexibleMolecule* getCurrentLigand();

				//@}



			private:

				static const String VERSION;

				Result* receptor_result_;
				bool closed_;
				bool mode_read_;
				String filename_;

				String timestamp_;
				String toolinfo_;

				// --- for use of GenericMolFile interface functions ---
				vector<Conformation*> gmf_input_conformations_;
				static FlexibleMolecule* gmf_current_ligand_;
				static FlexibleMolecule* gmf_last_ligand_;
				static bool gmf_new_ligand_read_;
				HashSet<String> gmf_input_conformation_IDs_;
				String gmf_property_name_;
				Result* gmf_result_;
				Result* gmf_import_;
				Result::Method gmf_result_method_;
				String gmf_result_description_;
				String gmf_receptor_conf_UID_;
				bool gmf_result_creation_disabled_;
				// ---------------------------------

				/* Stores the IDs of the Ligands that have already been written to file in order to make sure that identical molecules are not written multiple times. */
				HashSet<String> written_ligand_IDs_;

				QXmlStreamReader* xmlIn_;
				QFile* file_;

				// output file handles
				QTemporaryFile* resultfileTmp_;
				QTemporaryFile* ligandfileTmp_;
				QTemporaryFile* receptorfileTmp_;
				QFile* resultfile_;
				QFile* ligandfile_;
				QFile* receptorfile_;
				QXmlStreamWriter* xmlOutReceptor_;
				QXmlStreamWriter* xmlOutLigand_;
				QXmlStreamWriter* xmlOutResult_;

				vector<Result*> results_;
				Receptor* receptor_;
				vector<Ligand*>  ligands_;

				// receptors
				Protein*  current_protein_;
				Chain*    current_chain_;
				Residue*  current_residue_;

				vector<PDBAtom*>  current_PDB_atoms_;
				HashMap<String,Vector3> pdb_atom_coords_;

				Position  flexdef_idx;
				FlexDefinition current_flexdef_;
				Receptor* current_receptor_;
				Ligand*   current_ligand_;
				String       current_ligand_id_;
				String       current_receptor_id_;
				Molecule* current_molecule_;
				String       current_molecule_id_;
				String       current_conformation_id_;
				String    current_molecule_name_;
				String    current_protein_name_;
				Atom*     current_atom_;
				Vector3   current_coords_;
				vector<Atom*> current_atoms_;
				vector<int> bonds_from_;
				vector<int> bonds_to_;
				vector<int> bonds_order_;
				HashMap<Size,Vector3> current_conformation_data_;

				// results
				Result* current_result_;
				String current_Result_InputPoseId_;

				void addReceptor(Receptor* s);
				void addLigand(Ligand* lig);
				void concatenate();
				// receptor write
				void writeReceptors(QXmlStreamWriter &out);
				void writeReceptor(Receptor* recep, QXmlStreamWriter &out);
				void writeProtein(Protein* prot, QXmlStreamWriter &out);
				void writeResidue(Residue* res, QXmlStreamWriter &out);
				void writePDBAtom(PDBAtom* at, QXmlStreamWriter &out);
				void writePDBAtom(Atom* at, QXmlStreamWriter &out);
				void writeConformation(Conformation* conf, QXmlStreamWriter &out);
				void writeFlexibility(const FlexDefinition &fd, QXmlStreamWriter &out);
				void writeRotamericFlexibleResidue(Position idx, QXmlStreamWriter &out);
				void writeFullyFlexibleResidue(Position idx, QXmlStreamWriter &out);
				// receptor read
				bool readReceptors_() throw(Exception::ParseError);
				bool readReceptor_() throw(Exception::ParseError);
				bool readProtein() throw(Exception::ParseError);
				bool readResidue() throw(Exception::ParseError);
				bool readPDBAtom() throw(Exception::ParseError);
				// result read
				bool readResults_() throw(Exception::ParseError);
				bool readResult() throw(Exception::ParseError);
				bool readSubResult() throw(Exception::ParseError);
				bool readEntry() throw(Exception::ParseError);
				// result write
				void writeResults(QXmlStreamWriter &out);
				void writeResult(Result* result, QXmlStreamWriter &out);
				void writeResultData(const Result::ResultData &rd, QXmlStreamWriter &out);
				// ligand write
				void writeLigands(QXmlStreamWriter &out);
				bool writeLigand(Ligand* lig, QXmlStreamWriter &out);
				void writeMolecule(Molecule* mol, QXmlStreamWriter &out);
				void writeAtom(Atom* at, QXmlStreamWriter &out);
				void writeBond(Bond* b, QXmlStreamWriter &out);
				// ligand read
				bool readLigands() throw(Exception::ParseError);
				bool readLigand_() throw(Exception::ParseError);
				bool readMolecule() throw(Exception::ParseError);
				bool readConformations(FlexibleMolecule* target) throw(Exception::ParseError);
				bool readConformation(Conformation* conformation) throw(Exception::ParseError);
				bool readCoordinates() throw(Exception::ParseError);
				bool readFlexibility() throw(Exception::ParseError);
				bool readFlexibilities() throw(Exception::ParseError);
				bool readFullFlexResidue() throw(Exception::ParseError);
				bool readRotamericResidue() throw(Exception::ParseError);
				bool readAtoms() throw(Exception::ParseError);
				bool readAtom() throw(Exception::ParseError);
				bool readBonds() throw(Exception::ParseError);
				bool readBond() throw(Exception::ParseError);

				// building
				void buildLigand() throw(Exception::ParseError);
				void buildMolecule() throw(Exception::ParseError);
				void buildReceptor() throw(Exception::ParseError);
				void buildProtein() throw(Exception::ParseError);
				void buildResidue() throw(Exception::ParseError);

				// helper methods
				bool retrieveInt(const String& s, int &out);
				bool retrieveFloat(const String& s, float &out);
				void setElement(Atom* at, String symbol);
				bool setCoordinate(const int& idx , const String& coord);
				static String fromQString(const QString &s);
				static QString toQString(const String &s);
				static void attributesToHashMap(const QXmlAttributes& attributes, HashMap<String,String>& map);
				static void attributesToHashMap(const QXmlStreamAttributes& attributes, HashMap<String,String>& map);
				static bool isAminoAcid(String s);

				// we do not allow copy construction ..
				DockResultFile(const DockResultFile &df);
				// .. and assignment
				DockResultFile& operator=(const DockResultFile &df);

				// XML tags
				static String ROOTTAG;
				static String RECEPTORSTAG;
				static String RECEPTORTAG;
				static String RECEPTOR_A_ID;
				static String PROTEINTAG;
				static String PROTEIN_A_NAME;
				static String CONFORMATIONSTAG;
				static String CONFORMATIONTAG;
				static String CONFORMATION_A_ID;
				static String FLEXIBILITIESTAG;
				static String FLEXIBILITYTAG;
				static String RESIDUETAG;
				static String RESIDUE_A_NAME;
				static String RESIDUE_A_IDX;
				static String PDBATOMTAG;
				static String PDBATOM_A_NAME;

				static String RESULTSTAG;
				static String RESULTTAG;
				static String RESULT_A_METHOD;
				static String RESULT_A_METHODSTRING;
				static String SUBRESULTTAG;
				static String SUBRESULT_A_LIGCONFID;
				static String RESULTDATATAG;
				static String POSEDATATAG;
				static String RESULTDATA_A_ENERGY;
				static String RESULTDATA_A_CONFID;
				static String RESULTDATA_A_LIGID;
				static String RESULTDATA_A_METHOD;

				static String LIGANDSTAG;
				static String LIGANDTAG;
				static String LIGAND_A_ID;
				static String MOLECULETAG;
				static String MOLECULE_A_NAME;
				static String ATOMTAG;
				static String ATOM_A_ELEMENT;
				static String ATOMSTAG;
				static String BONDSTAG;
				static String BONDTAG;
				static String BOND_A_FROM;
				static String BOND_A_TO;
				static String BOND_A_ORDER;
		};
}

#endif

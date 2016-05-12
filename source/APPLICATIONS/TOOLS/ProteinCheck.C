// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/DOCKING/COMMON/structurePreparer.h>
#include <BALL/STRUCTURE/secondaryStructureProcessor.h>
#include <BALL/STRUCTURE/peptides.h>
#include <set>
#include "version.h"

using namespace BALL;
using namespace std;

String broken_chains_info = "";
String bond_error_info = "";
String aa_charge_info = "";
String naa_charge_info = "";
String temp_factor_info = "";
String clash_info = "";
String missing_element_info = "";


bool clashCheck(const AtomContainer& container)
{
	map<int,map<int,map<int,list<const Atom*> > > > hashgrid;
	set<String> detected_clashes;
	double resolution = 2.0;
	for(AtomConstIterator a_it=container.beginAtom(); +a_it; a_it++)
	{
		Vector3 pos = a_it->getPosition();
		int x = (floor)(pos.x/resolution);
		int y = (floor)(pos.y/resolution);
		int z = (floor)(pos.z/resolution);
		hashgrid[x][y][z].push_back(&*a_it);
	}

	bool clashes_found = 0;
	for(AtomConstIterator a_it=container.beginAtom(); +a_it; a_it++)
	{
		Vector3 pos = a_it->getPosition();
		int x = (floor)(pos.x/resolution);
		int y = (floor)(pos.y/resolution);
		int z = (floor)(pos.z/resolution);
		int x0 = max(0,x-1);
		int y0 = max(0,y-1);
		int z0 = max(0,z-1);
		for(int i=x0; i<=x+1; i++)
		{
			for(int j=y0; j<=y+1; j++)
			{
				for(int k=z0; k<=z+1; k++)
				{
					const list<const Atom*>& atom_list = hashgrid[i][j][k];
					for(list<const Atom*>::const_iterator it = atom_list.begin();
						it != atom_list.end(); it++)
					{
						//cout << a_it->getFullName() << "  " << (*it)->getFullName() << a_it->getDistance(**it)<<endl;
						if (&*a_it != *it)
						{
							double dist = a_it->getDistance(**it);
							if ( dist < (a_it->getElement().getVanDerWaalsRadius() + (*it)->getElement().getVanDerWaalsRadius() - 1.0)
								&& !a_it->isBoundTo(**it) && !a_it->isGeminal(**it) )
							{
								const Residue* r1 = a_it->getResidue();
								const Residue* r2 = (*it)->getResidue();

								String n1 = a_it->getName();
								String n2 = (*it)->getName();

								// Skip check if both atoms are backbone atoms!
								// Otherwise we will get too many false positives if bonds could not be recreated (due to non-standard atom names)
								if ( r1 != r2
									&& a_it->getElement().getSymbol()!="H"
									&& (*it)->getElement().getSymbol()!="H" &&
									( (!r1->isAminoAcid() || (n1!="C" && n1!="O" && n1!="CA" && n1!="N" && a_it->getFullName()!="PRO:CD")) ||
									 (!r2->isAminoAcid() || (n2!="C" && n2!="O" && n2!="CA" && n2!="N" && (*it)->getFullName()!="PRO:CD")) ) )
								{
									String s1 = r1->getName() + r1->getID() + ":" + a_it->getName();
									String s2 = r2->getName() + r2->getID() + ":" + (*it)->getName();
									String pair1 = s1 + s2;
									String pair2 = s2 + s1;

									if (detected_clashes.find(pair1) == detected_clashes.end()
										&& detected_clashes.find(pair2) == detected_clashes.end())
									{
										if (clash_info != "") clash_info += "\n";
										clash_info += "\t" + s1 + " -- " + s2 + " : " + String(dist) + "A";

										detected_clashes.insert(pair1);
										detected_clashes.insert(pair2);
										clashes_found = true;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return !clashes_found;
}


/* check whether the given Molecule really contains only one molecule */
void checkBonds(const Atom* atom, set<const Atom*>& visited_atoms)
{
	if (visited_atoms.find(atom) != visited_atoms.end())
	{
		return;
	}

	visited_atoms.insert(atom);
	for (Atom::BondConstIterator b_it = atom->beginBond(); +b_it; b_it++)
	{
		checkBonds(b_it->getPartner(*atom), visited_atoms);
	}
}


// Return true if molecule passed all tests, else return false.
bool checkProtein(Protein* protein, bool allow_broken_chains)
{
	bool bond_length_ok = 1;
	bool elements_ok = 1;
	bool one_molecule = 1;
	bool has_atoms = 1;
	bool has_3D_coordinates = 1;
	bool has_hydrogens = 0;

	Size no_atoms = 0;
	bool all_x_zero = 1; // are all x-coordinates in the file set to zero?
	bool all_y_zero = 1;
	bool all_z_zero = 1;
	bool broken_chains = 0;
	//bool aa_partial_charges_ok = 1;
	//bool ligand_partial_charges_ok = 1;
	//bool has_non_aa = 0;
	bool temp_factor_ok = 1;
	set<String> detected_bond_errors;
	bool missing_residues_found = 0;

	for (AtomConstIterator it = protein->beginAtom(); +it; it++, no_atoms++)
	{
		String res_id = it->getResidue()->getName()+it->getResidue()->getID();
		for (Atom::BondConstIterator b_it = it->beginBond(); +b_it; b_it++)
		{
			double length = b_it->getLength();
			if (length < 0.7 || length > 2.5)
			{
				bond_length_ok = false;

				String pair = res_id+b_it->getFirstAtom()->getName()+res_id+b_it->getSecondAtom()->getName();
				if (detected_bond_errors.find(pair) == detected_bond_errors.end())
				{
					if (bond_error_info != "") bond_error_info += "\n";
					bond_error_info += "\t"+res_id+":"+b_it->getFirstAtom()->getName()+"--"+res_id+":"+b_it->getSecondAtom()->getName()+" : "+String(length)+"A";
					detected_bond_errors.insert(pair);
				}
			}
		}

		if (elements_ok)
		{
			if (it->getElement().getName() == BALL_ELEMENT_NAME_DEFAULT)
			{
				elements_ok = false;
				const Residue* res = it->getResidue();
				if (missing_element_info != "") bond_error_info += "\n";
				missing_element_info += "\t"+res->getName()+res->getID()+":"+it->getName();
			}
		}
		if (!has_hydrogens)
		{
			if (it->getElement().getSymbol() == "H")
			{
				has_hydrogens = 1;
			}
		}

		const TVector3<float>& pos = it->getPosition();
		if (all_x_zero)
		{
			if (fabs(pos[0]) > 0.001) all_x_zero = 0;
		}
		if (all_y_zero)
		{
			if (fabs(pos[1]) > 0.001) all_y_zero = 0;
		}
		if (all_z_zero)
		{
			if (fabs(pos[2]) > 0.001) all_z_zero = 0;
		}
	}

	if (no_atoms == 0)
	{
		has_atoms = false;
		Log.error()<<"   Error :"<<" protein has no atoms!"<<endl;
	}
	else
	{
		int chain_no = 1;
		for (ChainConstIterator ch_it = protein->beginChain(); +ch_it; ch_it++, chain_no++)
		{
			list<int> missing_residues;
			bool only_aa = true;
			try
			{
				int last_residue_id = 0;
				for (ResidueConstIterator r_it = ch_it->beginResidue(); +r_it; r_it++)
				{
					// If the current chains contains non-amino-acids (e.g. ligands)
					bool is_aa = true;
					if (!r_it->isAminoAcid())
					{
						is_aa = false;
						only_aa = false;
						//has_non_aa = true;
					}
					for (AtomConstIterator it = r_it->beginAtom(); +it; it++)
					{
						if (is_aa)
						{
							if (abs(it->getFormalCharge()) > 2.001)
							{
								//aa_partial_charges_ok = 0;
								if (aa_charge_info != "") aa_charge_info += "\n";
								aa_charge_info += "\t" +r_it->getName()+r_it->getID()+":"+it->getName()+" : "+String(it->getCharge());
							}
							const PDBAtom* pdbatom = dynamic_cast<const PDBAtom*>(&*it);
							if (pdbatom && pdbatom->getElement().getSymbol()!="H")
							{
								double tf = pdbatom->getTemperatureFactor();
								if (tf < 1 || tf > 100)
								{
									temp_factor_ok = false;
									if (temp_factor_info != "") temp_factor_info += "\n";
									temp_factor_info += "\t" +r_it->getName()+r_it->getID()+":"+it->getName()+" : "+String(tf);
								}
							}
						}
						else
						{
							if (fabs(it->getCharge()) > 4.001)
							{
								//ligand_partial_charges_ok = 0;
								if (naa_charge_info != "") naa_charge_info += "\n";
								naa_charge_info += "\t" +r_it->getName()+r_it->getID()+":"+it->getName()+" : "+String(it->getCharge());
							}
						}
					}

					if (r_it == ch_it->beginResidue())
					{
						last_residue_id = r_it->getID().toInt();
					}
					else
					{
						int id = r_it->getID().toInt();
						for (int i = last_residue_id+1; i < id; i++)
						{
							missing_residues.push_back(i);
							missing_residues_found = true;
						}
						last_residue_id = id;
					}
				}
			}   // abort this test if IDs contain non-numeric labels
			catch(BALL::Exception::GeneralException e) {}

			// No sense in trying to enforce having only one molecule in ligand- or water-chains
			if (!only_aa)
			{
				continue;
			}

			String name = ch_it->getName().trim();
			if (name == "") name = String(chain_no);
			Size no_chain_atoms = ch_it->countAtoms();
			set<const Atom*> visited_atoms;
			checkBonds(&*ch_it->beginAtom(), visited_atoms);
			if (visited_atoms.size() != no_chain_atoms)
			{
				broken_chains = true;
				if (broken_chains_info != "") broken_chains_info += "\n";
				broken_chains_info += "\tchain "+name;

				if (!allow_broken_chains)
				{
					one_molecule = false;
				}

				if (!missing_residues.empty())
				{
					broken_chains_info += ", missing residues: ";
				}
				for (list < int > ::iterator it = missing_residues.begin(); it != missing_residues.end(); it++)
				{
					broken_chains_info += String(*it) + " ";
				}
			}
		}
	}

	bool clashcheck_ok = clashCheck(*protein);

	Log.level(20) << "Check for 3D coordinates ... ";
	if (all_x_zero || all_y_zero || all_z_zero)
	{
		has_3D_coordinates = false;
		Log.level(20)<<"FAILED"<<endl;
	}
	else
	{
		Log.level(20)<<"passed"<<endl;
	}
	Log.level(20) << "Check that all atoms have a valid element ... ";
	if (!elements_ok)
	{
		Log.level(20)<<"FAILED for the following atoms: "<<endl;
		Log.level(20) << missing_element_info << endl;
	}
	else
	{
		Log.level(20) << "passed"<<endl;
	}
	Log.level(20)<<"Check for senseful bond-length (0.7-2.5 A) ... ";
	if (!bond_length_ok)
	{
		Log.level(20)<<"FAILED, "<<endl<<bond_error_info<<endl;
	}
	else
	{
		Log.level(20)<<"passed"<<endl;
	}
	Log.level(20)<<"Check for non-backbone heavy-atom clashes ... ";
	if (!clashcheck_ok)
	{
		Log.level(20)<<"FAILED, "<<endl<<clash_info<<endl;
	}
	else
	{
		Log.level(20)<<"passed"<<endl;
	}

	// Partial charge check makes no real sense if we only use pdb as input.
// 	cout<<"Check for senseful amino acid partial charges (-2 < charge < 2) ... ";
// 	if (aa_partial_charges_ok)
// 	{
// 		cout<<"passed"<<endl;
// 	}
// 	else
// 	{
// 		cout<<"FAILED, "<<endl<<aa_charge_info<<endl;
// 	}
// 	if (has_non_aa)
// 	{
// 		cout<<"Check for senseful non-amino-acid partial charges (-4 < charge < 4) ... ";
// 		if (ligand_partial_charges_ok)
// 		{
// 			cout<<"passed"<<endl;
// 		}
// 		else
// 		{
// 			cout<<"FAILED, "<<endl<<naa_charge_info<<endl;
// 		}
// 	}

	Log.level(20)<<"Check that protein is protonated ... ";
	if (!has_hydrogens)
	{
		Log.level(20)<<"FAILED"<<endl;
	}
	else
	{
		Log.level(20)<<"passed"<<endl;
	}
	Log.level(20)<<"Check for senseful temperature factors (1 < TF < 100) ... ";
	if (temp_factor_ok)
	{
		Log.level(20)<<"passed"<<endl;
	}
	else
	{
		Log.level(20)<<"FAILED, "<<endl<<temp_factor_info<<endl;
	}
	Log.level(20)<<"Check for each protein chain containing only one molecule ... ";
	if (broken_chains)
	{
		Log.level(20)<<"FAILED for the following chains:"<<endl<<broken_chains_info<<endl;
		if (!missing_residues_found)
		{
			Log.level(20)<<"\tPlease note that one reason for this might be amino acids that have non-standard names, so that their bonds could not be reconstructed if no information about them is stored in the input file."<<endl;
		}
	}
	else
	{
		Log.level(20)<<"passed"<<endl;
	}

	return has_atoms && has_hydrogens && has_3D_coordinates && one_molecule && bond_length_ok && elements_ok && temp_factor_ok;
}


void generateReport(Protein& protein, String inputfilename, String outputfile)
{
	SecondaryStructureProcessor processor;
	protein.apply(processor);

	String file1;
	File::createTemporaryFilename(file1,".txt");
	ofstream out1(file1.c_str(),ios::out); // sheet
	String file2;
	File::createTemporaryFilename(file2,".txt");
	ofstream out2(file2.c_str(),ios::out);
	String file3;
	File::createTemporaryFilename(file3,".txt");
	ofstream out3(file3.c_str(),ios::out); // helix
	String file4;
	File::createTemporaryFilename(file4,".txt");
	ofstream out4(file4.c_str(),ios::out); // other
	for (ResidueConstIterator it = protein.beginResidue(); it != protein.endResidue(); it++)
	{
		if (it->isAminoAcid() && !it->isTerminal())
		{
			String sec_struct_type = " ";
			const SecondaryStructure* sec_struct = it->getSecondaryStructure();
			if (sec_struct)
			{
				if (sec_struct->getType() == SecondaryStructure::HELIX)
				{
					sec_struct_type = "H";
				}
				if (sec_struct->getType() == SecondaryStructure::STRAND)
				{
					sec_struct_type = "S";
				}
			}

			double phi = it->getTorsionPhi().toDegree();
			if (phi < -180)
			{
				phi = 360 + phi;
			}
			else if (phi > 180)
			{
				phi = 360 - phi;
			}
			double psi = it->getTorsionPsi().toDegree();
			if (psi < -180)
			{
				psi = 360 + psi;
			}
			else if (phi > 180)
			{
				psi = 360 - psi;
			}

			if (sec_struct_type == "S")
			{
				out1 << phi << "\t" << psi <<endl;
			}
			else if (sec_struct_type == "H")
			{
				out3 << phi << "\t" << psi <<endl;
			}
			else
			{
				out4 << phi << "\t" << psi <<endl;
			}
		}

		// calculate average temperature factor for current residue
		double average_tf = 0;
		Size no_atoms = 0;
		for (AtomConstIterator a_it = it->beginAtom(); +a_it; a_it++)
		{
			const PDBAtom* pdbatom = dynamic_cast<const PDBAtom*>(&*a_it);
			if (pdbatom && pdbatom->getElement().getSymbol()!="H")
			{
				average_tf += pdbatom->getTemperatureFactor();
				no_atoms++;
			}
		}
		average_tf /= no_atoms;
		out2 << average_tf << endl;
	}

	String header = "set terminal postscript eps enhanced color\n";
	header += "unset key\n";
	header += "set style line 1 lt 1 lw 1 pt 1 linecolor rgb \"#132061\"\n";

	String file5;
	File::createTemporaryFilename(file5,".txt");
	String file6;
	File::createTemporaryFilename(file6,".eps");
	ofstream p(file5.c_str(),ios::out);
	p << header << "set output \"" << file6 << "\"" <<endl;
	p << "set xlabel \"phi\"; set ylabel \"psi\"" << endl;
	p << "set key" << endl;
	p << "plot \"" << file1 << "\" ls 1 ti \"sheet\", \"" << file3 << "\" ls 4 ti \"helix\", \"" << file4 << "\" ls 8 ti \"other\"" << endl;
	p.close();
	String command = "gnuplot ";
	command += String(" < ") + file5;
	system(command.c_str());
	File::remove(file1);
	File::remove(file3);
	File::remove(file4);
	File::remove(file5);

	String file7;
	File::createTemporaryFilename(file7,".txt");
	String file8;
	File::createTemporaryFilename(file8,".eps");
	p.open(file7.c_str(),ios::out);
	p << header << "set output \"" << file8 << "\"" <<endl;
	p << "set size ratio 0.5" << endl;
	p << "set xlabel \"Residue\"; set ylabel \"heavy-atom temperature factor\"" << endl;
	p << "plot \"" << file2 << "\"  w lines ls 1" << endl;
	p.close();
	command = "gnuplot ";
	command += String(" < ") + file7;
	system(command.c_str());
	File::remove(file2);
	File::remove(file7);

	// Add excape-characters to symbols that latex would otherwise not like.
	String s = "";
	int index = inputfilename.find_last_of("/");
	if (index > 0)
	{
		inputfilename = inputfilename.substr(index+1);
	}
	Size size = inputfilename.size();
	for(Size i=0; i<size; i++)
	{
		if (inputfilename[i] == '[')
		{
			s += "\\[";
		}
		else if (inputfilename[i] == ']')
		{
			s += "\\]";
		}
		else if (inputfilename[i] == '_')
		{
			s += "\\_";
		}
		else if (inputfilename[i] == '-')
		{
			s += "\\-";
		}
		else if (inputfilename[i] == '+')
		{
			s += "\\+";
		}
		else
		{
			s += inputfilename[i];
		}
	}
	inputfilename = s;

	String file9;
	File::createTemporaryFilename(file9,".tex");
	p.open(file9.c_str(),ios::out);
	p << "\\documentclass{article}\n\
	\\usepackage{geometry}\n\
	\\geometry{a4paper,left=20mm,right=20mm, top=24mm, bottom=22mm}\n\
	\\usepackage[dvips]{color}\n\
	\\usepackage[dvips]{graphicx}\n\
	\\title{ProteinCheck on input-file " << inputfilename << " }\n\
	\\author{ProteinCheck, version " << VERSION << ", build "<< String(__DATE__) << "} \n\
	\\begin{document}\n\
	\\maketitle\n\
	\
	\\section{Broken chains check}" << endl;
	if (broken_chains_info == "")
	{
		p << "No broken chains detected." << endl;
	}
	else
	{
		broken_chains_info.substituteAll("\n","\\\\ --");
		p << "The following protein chains seem to be broken:  \\\\" << endl;
		p << " -- " << broken_chains_info << "\\\\ " << endl;
		p << "If no missing residues were specified for a broken chain, please note that non-standard atom-/residue-names might have been the cause for the chain-break detection due to bonds that could not be recreated." << endl;
	}

	p << "\\section{Missing element check}" << endl;
	if (missing_element_info == "")
	{
		p << "All atoms have a valid assigned element." << endl;
	}
	else
	{
		p << "The following atom do not have a valid assigned element: \\\\" << endl;
		missing_element_info.substituteAll("\n","\\\\ \\hspace*{1cm}");
		p << "\\hspace*{1cm}" << missing_element_info << endl;
	}

	p << "\\section{Bond length check}" << endl;
	if (bond_error_info == "")
	{
		p << "No senseless bond lengths ($>$2.5 or $<$0.7 \\AA) detected." << endl;
	}
	else
	{
		p << "The following senseless bond lengths ($>$2.5 or $<$0.7 \\AA) have been detected: \\\\" << endl;
		bond_error_info.substituteAll("\n","\\\\ \\hspace*{1cm}");
		p << "\\hspace*{1cm}" << bond_error_info << endl;
	}

	// Partial charge check makes no real sense if we only use pdb as input.
	/*
	p << "\\section{Partial charge check}" << endl;
	if (aa_charge_info == "")
	{
		p << "No senseless partial charges ($<$-2 or $>$2) have been found for amino acids. \\\\" << endl;
	}
	else
	{
		p << "The following senseless partial charges ($<$-2 or $>$2) have been found for amino acids: \\\\" << endl;
		aa_charge_info.substituteAll("\n","\\\\ ");
		p << aa_charge_info << endl;
	}
	if (naa_charge_info == "")
	{
		p << "No senseless partial charges ($<$-4 or $>$4) have been found for non-amino-acid residues. \\\\" << endl;
	}
	else
	{
		p << "The following senseless partial charges ($<$-4 or $>$4) have been found for non-amino-acids residues: \\\\" << endl;
		naa_charge_info.substituteAll("\n","\\\\ ");
		p << naa_charge_info << endl;
	}*/

	p << "\\section{Atom clashes}" << endl;
	if (clash_info == "")
	{
		p << "No clashes between atom non-backbone heavy atoms found. \\\\" << endl;
		p << "Considered as sterical clashes were all pair-wise contacts with a distance smaller than the sum of the atoms' van-der-Waals radii minus 1.0\\AA." << endl;
	}
	else
	{
		p << "The following clashes between atom non-backbone heavy atoms were found:\\\\" << endl;
		clash_info.substituteAll("\n","\\\\ \\hspace*{1cm}");
		p << "\\hspace*{1cm}" << clash_info << "\\\\" << endl;
		p << "Considered as sterical clashes were all pair-wise contacts with a distance smaller than the sum of the atoms' van-der-Waals radii minus 1.0\\AA." << endl;
	}

	p << "\\section{Temperature factor check}" << endl;
	if (temp_factor_info == "")
	{
		p << "No dubious temperature factors ($<$1 or $>$100) have been found. " << endl;
	}
	else
	{
		p << "The following dubious temperature factors ($<$1 or $>$100) have been found: \\\\" << endl;
		temp_factor_info.substituteAll("\n","\\\\ \\hspace*{1cm}");
		p << "\\hspace*{1cm}" << temp_factor_info << endl;
	}

	p << "\\section{Secondary structure}  \n ";
	p << "In the secondary structure shown below, amino acid are encoded by their standard one-letter code while water molecules are indicated by '-' and other non-amino-acid residues (e.g. ligands) by '+'. The latter two are shown here only to help interpretation of the temperature factor plot shown later and to give you an overview of the content of the entire input file. \\vspace*{0.5cm} \\\\" << endl;
	String line1 = "";
	String line2 = "";
	String line3 = "";
	Size id=1;
	for (ResidueConstIterator it = protein.beginResidue(); it != protein.endResidue(); it++, id++)
	{
		String sec_struct_type = " ";
		const SecondaryStructure* sec_struct = it->getSecondaryStructure();
		if (sec_struct)
		{
			if (sec_struct->getType() == SecondaryStructure::HELIX)
			{
				sec_struct_type = "H";
			}
			if (sec_struct->getType() == SecondaryStructure::STRAND)
			{
				sec_struct_type = "S";
			}
		}

		if (id < 98)
		{
			if ((id+1)%10 == 0)
			{
				line1 += String(id+1);
			}
			else if (id%10 != 0)
			{
				line1 += " ";
			}
		}
		else if (id>=98 && id<=100)
		{
			if(id==98) line1 += String(100)+" ";
		}
		else if (id < 997)
		{
			if ((id+2)%10 == 0)
			{
				line1 += String(id+2);
			}
			else if (id%10 != 0 && (id-1)%10 != 0)
			{
				line1 += " ";
			}
		}
		else if (id>=997 && id<=1000)
		{
			if(id==997) line1 += String(1000)+" ";
		}
		else
		{
			if ((id+3) % 10 == 0)
			{
				line1 += String(id+3);
			}
			else if (id%10 != 0 && (id-1)%10 != 0 && (id-2)%10 != 0)
			{
				line1 += " ";
			}
		}

		if (!it->isAminoAcid())
		{
			if (it->getName()=="HOH" || it->getName()=="WAT")
			{
				line2 += "-";
			}
			else
			{
				line2 += "+";
			}
		}
		else
		{
			String s = Peptides::OneLetterCode(it->getName());
			if (s != "")
			{
				line2 += s;
			}
			else
			{
				line2 += "?";
			}
		}
		line3 += sec_struct_type;

		ResidueConstIterator tmp = it;
		tmp++;
		if ((id > 1 && id%90==0) || tmp==protein.endResidue())
		{
			p << "\\begin{minipage}[H!]{1\\textwidth}" << endl;
			p << "\\verb|" << line1 << "| \\\\ " << endl;
			p << "\\verb|" << line2 << "| \\\\ " << endl;
			p << "\\verb|" << line3 << "| \\\\ " << endl;
			p << "\\end{minipage}" << endl;
			p << "\\vspace*{0.2cm} \\\\" << endl;
			line1 = "";
			if(id>100) line1 = " ";
			line2 = "";
			line3 = "";
		}
	}

	p << "\\vspace*{0.5cm} \\\\ " << endl;
	p << "\\begin{minipage}[h]{1\\textwidth}\n\
	\\section{Ramachandran plot}\n\
		\\centering\n\
		\\includegraphics[width=0.7\\textwidth]{" << file6 << "}\n\
	\\end{minipage}  \\\\ \
	\\vspace*{1cm}  \n\
	\
	\\begin{minipage}[h]{1\\textwidth}\n\
	\\section{Temperature factor plot}\n\
		\\centering\n\
		\\includegraphics[width=0.8\\textwidth]{" << file8 << "}\n\
	\\end{minipage}\n\
	\\end{document}" << endl;
	p.close();
	command = "latex ";
	command += file9 +" > /dev/null";
	system(command.c_str());

	command = "dvipdf -sPAPERSIZE=a4 ";
	command += (String)(file9.before(".tex")) + ".dvi ";
	system(command.c_str());

	// remove temporary files
	File::remove(file6);
	File::remove(file8);
	File::remove(file9);
	s = (String)(file9.before(".tex")) + ".dvi";
	File::remove(s);
	s = (String)(file9.before(".tex")) + ".log";
	File::remove(s);
	s = (String)(file9.before(".tex")) + ".aux";
	File::remove(s);

	File::move((String)(file9.before(".tex")) + ".pdf", outputfile);
}


int main(int argc, char* argv[])
{
	CommandlineParser parpars("ProteinCheck", "quality check for proteins structures", VERSION, String(__DATE__), "Checks and evaluations");
	parpars.registerMandatoryInputFile("i", "input pdb-file");
	parpars.registerMandatoryOutputFile("o", "pdf-file for quality report");
	parpars.registerFlag("bc", "ignore broken chains");
	String man = "Check a given protein structure for the following errors:\n\
    * bond-lengths may not be completely senseless (i.e. <0.7 or >2.5 Angstroem)\n\
    * each chain may only contain one actual molecule, i.e. there may be no unconnected atoms or fragments. This test is skipped if the above box is checked.\n\
    * each atom must have a valid assigned element\n\
    * the protein must be protonated (since this is necessary for docking/(re-)scoring).\n\
    * 3D coordinates must be present (instead of 2D coordinates; also necessary for docking/(re-)scoring)\n\
    * there may be no senseless temperature factors (<1 or >100)\n\
    * there may be no sterical clashes between atoms\n\nA protein structure quality report, containing the results of those tests and a secondary structure prediction, a Ramachandran plot and a temperature factor plot will be generated and saved as a pdf-file.";
   // * atoms may not have completely senseless partial charges (>2 or <-2 within amino acids; else >4 or <-4).";
	parpars.setSupportedFormats("i","pdb");
	parpars.setSupportedFormats("o","pdf");
	parpars.setToolManual(man);
	parpars.parse(argc, argv);

	PDBFile input(parpars.get("i"));
	Protein protein;
	input >> protein;
	System system;
	system.insert(protein);

	FragmentDB fragdb;
	fragdb.setFilename("fragments/Fragments.db");
	fragdb.init();
	protein.apply(fragdb.build_bonds);

// 	StructurePreparer sp;
// 	sp.assignCharges(&protein, 2);
// 	cout<<endl;

	bool allow_broken_chains = 0;
	if (parpars.get("bc") != CommandlineParser::NOT_FOUND)
	{
		allow_broken_chains = 1;
	}
	bool ok = checkProtein(&protein, allow_broken_chains);

	if(parpars.get("o") != CommandlineParser::NOT_FOUND)
	{
		generateReport(protein, parpars.get("i"), parpars.get("o"));
	}

	if (!ok)
	{
		cerr<<endl<<"protein contained errors!"<<endl;
		return 1;
	}
	else
	{
		Log.level(20)<<endl<<"No errors detected."<<endl;
		return 0;
	}
}

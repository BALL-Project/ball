// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/DOCKING/COMMON/dockingAlgorithm.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/dockResultFile.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/DOCKING/COMMON/structurePreparer.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/STRUCTURE/structureMapper.h>

#include <stdlib.h>
#include <iostream>

using namespace std;

namespace BALL
{
	const String DockingAlgorithm::OPTION_FILE_PARAMETER_NAME = "option";

	DockingAlgorithm::DockingAlgorithm()
	{
		display_mode_ = NO_DISPLAY;
		name_ = "";
		finished_ = 0;
		abort_ = 0;

		new_pose_to_be_visualized = 0;
		min_sec_between_visualizations_ = 1./20; // default: 50ms = 20fps
	}


	DockingAlgorithm::DockingAlgorithm(System& receptor, System& ligand, Options& new_options)
	{
		setup(receptor, ligand, new_options);
		display_mode_ = NO_DISPLAY;
		name_ = "";
		finished_ = 0;
		abort_ = 0;

		new_pose_to_be_visualized = 0;
		min_sec_between_visualizations_ = 1./20; // default: 50ms = 20fps
	}

	DockingAlgorithm::DockingAlgorithm(System& receptor, System& ligand)
	{
		setup(receptor, ligand);
		display_mode_ = NO_DISPLAY;
		name_ = "";
		finished_ = 0;
		abort_ = 0;

		new_pose_to_be_visualized = 0;
		min_sec_between_visualizations_ = 1./20; // default: 50ms = 20fps
	}

	DockingAlgorithm::~DockingAlgorithm()
	{

	}

	void DockingAlgorithm::setup(System& receptor, System& ligand, Options& new_options)
	{
		system1_ = &receptor;
		system2_ = &ligand;
		options  = new_options;
		reference_ligand_ = &ligand;
	}

	void DockingAlgorithm::setup(System& receptor, System& ligand)
	{
		system1_ = &receptor;
		system2_ = &ligand;
		reference_ligand_ = &ligand;
	}


	void DockingAlgorithm::readOptionFile(String filename, Options& output_options, list<Constraint*>& output_constraints, const AtomContainer* ref_ligand)
	{
		INIFile ini(filename);
		ini.read();
		Size num_sections = ini.getNumberOfSections();
		for(Size i = 0; i < num_sections; i++)
		{
			String name = ini.getSection(i)->getName();
			if (name.hasPrefix("ReferenceArea") || name.hasPrefix("PharmacophoreConstraint"))
			{
				continue;
			}

			Options* options_category = &output_options;
			if (name != "Docking-Settings")
			{
				options_category = output_options.createSubcategory(name);
			}

			Log.level(10)<<endl<<"--- Reading parameter-section '" << name << "' from file "<<"'"<<filename<<"' :  -----"<<endl;
			INIFile::LineIterator it = ini.getSectionFirstLine(name);
			INIFile::LineIterator it_end = ini.getSectionLastLine(name).getSectionNextLine();
			it.getSectionNextLine();
			for (; it != it_end; it.getSectionNextLine())
			{
				String line = *it;
				if (line == "") continue;
				String key = line.before("="); key.trim();
				String value = line.after("="); value.trim();
				if (key == "" || value == "") continue;
				options_category->set(key, value);

				if (name == ScoringFunction::SUBCATEGORY_NAME || name == "IMGDock")
				{
					Log.level(10)<<key<<" : "<<value<<endl;
				}
			}
		}

		for (Size i = 0; i < 100; i++)
		{
			string sec_name = "ReferenceArea"+String(i);

			if (!ini.hasSection(sec_name)) break;

			Log.level(10)<<endl<<"--- Reading "<<sec_name<<" from file "<<"'"<<filename<<"' :  -----"<<endl;

			String name = ini.getValue(sec_name, "name");
			bool is_fraction = ini.getValue(sec_name, "is_fraction").toBool();
			double penalty = ini.getValue(sec_name, "penalty").toDouble();
			double atoms = ini.getValue(sec_name, "atoms").toDouble();
			vector<Vector3> v(4);

			Log.level(10)<<"name = "<<name<<endl;
			Log.level(10)<<"is_fraction = "<<is_fraction<<endl;
			Log.level(10)<<"atoms = "<<atoms<<endl;
			Log.level(10)<<"penalty = "<<penalty<<endl;
			ReferenceArea* rf;

			String use_ref = ini.getValue(sec_name, "use_ref_ligand");
			if (use_ref != INIFile::UNDEFINED && use_ref.toBool())
			{
				Log.level(10)<<"use_ref_ligand = true"<<endl;
				if (!ref_ligand)
				{
					throw BALL::Exception::GeneralException(__FILE__, __LINE__, "DockingAlgorithm::readOptionFile()", "Reference-ligand required but not specified!");
				}
				rf = new ReferenceArea(ref_ligand, is_fraction, atoms, penalty);
				v = rf->input_points_;
			}
			else
			{
				for (Size i = 0; i <= 3; i++)
				{
					String pn = "p"+String(i);
					String s = ini.getValue(sec_name, pn);
					if (s == INIFile::UNDEFINED)
					{
						Log.error()<<"[Error:] 4 points must be defined for each ReferenceArea!"<<endl;
						return;
					}
					s.trim();
					double d0 = s.getField(0, ", ").toDouble();
					double d1 = s.getField(1, ", ").toDouble();
					double d2 = s.getField(2, ", ").toDouble();
					v[i] = Vector3(d0, d1, d2);
				}
				rf = new ReferenceArea(v[0], v[1], v[2], v[3], is_fraction, atoms, penalty);
			}

			// Increase size of box (e.g. bounding box around ref-ligand), if desired by the user.
			String inc = ini.getValue(sec_name, "box_size_increase");
			if (inc != INIFile::UNDEFINED)
			{
				rf->enlarge(inc.toDouble());
			}

			Log.level(10)<<"p0 = "<<v[0]<<endl;
			Log.level(10)<<"p1 = "<<v[1]<<endl;
			Log.level(10)<<"p2 = "<<v[2]<<endl;
			Log.level(10)<<"p3 = "<<v[3]<<endl;

			if (name != INIFile::UNDEFINED) rf->setName(name);
			output_constraints.push_back(rf);
		}

		for (Size i = 0; i < 100; i++)
		{
			string sec_name = "PharmacophoreConstraint"+String(i);

			if (!ini.hasSection(sec_name)) break;

			Log.level(10)<<endl<<"--- Reading "<<sec_name<<" from file "<<"'"<<filename<<"' :  -----"<<endl;

			String name = ini.getValue(sec_name, "name");
			double penalty = ini.getValue(sec_name, "penalty").toDouble();
			double desired_energy = ini.getValue(sec_name, "desired interaction energy").toDouble();
			String residues = ini.getValue(sec_name, "residue-IDs");
			vector<String> residue_vector;
			residues.split(residue_vector, ", ");
			String types = ini.getValue(sec_name, "interaction types");
			vector<String> types_vector;
			types.split(types_vector, ", ");
			list<String> types_list;
			for (Size i = 0; i < types_vector.size(); i++)
			{
				types_list.push_back(types_vector[i]);
			}

			Log.level(10)<<"name = "<<name<<endl;
			Log.level(10)<<"residue-IDs = "<<residues<<endl;
			Log.level(10)<<"interaction types = "<<types<<endl;
			Log.level(10)<<"desired interaction energy = "<<desired_energy<<endl;
			Log.level(10)<<"penalty = "<<penalty<<endl;

			PharmacophoreConstraint* phc = new PharmacophoreConstraint(residue_vector, types_list, desired_energy, penalty);
			if (name != INIFile::UNDEFINED) phc->setName(name);
			output_constraints.push_back(phc);
		}
		Log.level(10)<<endl<<"--- finished reading config-file."<<endl<<endl<<endl;
	}


	void DockingAlgorithm::writeSubcategories_(Options& category, ostream& out)
	{
		if (category.begin() != category.end())
		{
			out << "[" << category.getName() << "]" << endl;
		}
		for (Options::ConstIterator it = category.begin(); it != category.end(); it++)
		{
			out << it->first << " = " << it->second << endl;
		}
		out << endl;

		for(StringHashMap<Options*>::Iterator it = category.beginSubcategories(); it != category.endSubcategories(); it++)
		{
			writeSubcategories_(*it->second, out);
		}
	}


	void DockingAlgorithm::writeOptionFile(String filename, Options& input_options, list<Constraint*>& input_constraints)
	{
		ofstream out(filename.c_str());
		input_options.setName("Docking-Settings");
		writeSubcategories_(input_options, out);

		out<<endl<<endl;
		Size ref_count = 0;
		Size pharm_count = 0;
		for (list<Constraint*> ::iterator it = input_constraints.begin(); it != input_constraints.end(); it++)
		{
			ReferenceArea* ref_area = dynamic_cast<ReferenceArea*>(*it);
			PharmacophoreConstraint* phc = dynamic_cast<PharmacophoreConstraint*>(*it);
			if (ref_area)
			{
				out<<"[ReferenceArea"<<ref_count<<"]"<<endl;
				out<<"name = "<<ref_area->name_<<endl;
				Vector3 p = ref_area->input_points_[0];
				out<<"p0 = "<<p[0]<<", "<<p[1]<<", "<<p[2]<<endl;
				p = ref_area->input_points_[1];
				out<<"p1 = "<<p[0]<<", "<<p[1]<<", "<<p[2]<<endl;
				p = ref_area->input_points_[2];
				out<<"p2 = "<<p[0]<<", "<<p[1]<<", "<<p[2]<<endl;
				p = ref_area->input_points_[3];
				out<<"p3 = "<<p[0]<<", "<<p[1]<<", "<<p[2]<<endl;
				out<<"is_fraction = "<<ref_area->atom_fraction_<<endl;
				out<<"atoms = "<<ref_area->ref_atoms_<<endl;
				out<<"penalty = "<<ref_area->penalty_<<endl;
				out<<endl;
				ref_count++;
			}
			else if (phc)
			{
				out<<"[PharmacophoreConstraint"<<pharm_count<<"]"<<endl;
				out<<"name = "<<phc->name_<<endl;
				out<<"residue-IDs = ";
				const vector<String>& residue_IDs = phc->residue_IDs_;
				const vector<String>& chain_IDs = phc->chain_IDs_;
				for (Size i = 0; i < residue_IDs.size(); i++)
				{
					if (i > 0)  out<<", ";
					if (chain_IDs[i] != "") out << chain_IDs[i] << ":";
					out << residue_IDs[i];
				}
				out<<endl;
				out<<"interaction types = ";
				for (list < String > ::iterator c_it = phc->interaction_types_.begin();
				c_it!=phc->interaction_types_.end(); c_it++)
				{
					if (c_it != phc->interaction_types_.begin()) out<<", ";
					out<<*c_it;
				}
				out<<endl;
				out<<"desired interaction energy = "<<phc->desired_interaction_score_<<endl;
				out<<"penalty = "<<phc->penalty_<<endl;
				pharm_count++;
			}
		}
	}

	double DockingAlgorithm::dockLigand(AtomContainer& ligand, bool verbose)
	{
		return 0.0;
	}

	void DockingAlgorithm::setLigand(AtomContainer* ligand)
	{
		ligand_ = ligand;
	}

	void DockingAlgorithm::setDisplayMode(DISPLAYMODE display_mode)
	{
		display_mode_ = display_mode;
	}

	DockingAlgorithm::DISPLAYMODE DockingAlgorithm::getDisplayMode()
	{
		return display_mode_;
	}

	const AtomContainer* DockingAlgorithm::getVisualizationPose()
	{
		return &visualization_pose_;
	}

	void DockingAlgorithm::setVisualizationPose(const double& score)
	{
		// copy only if DockingUpdateThread has already displayed the last pose,
		// since else problems could occur if both threads simultaneously change/access visualization_pose_
		if (!new_pose_to_be_visualized)
		{
			if (visualization_timer_.getClockTime() >= min_sec_between_visualizations_)
			{
				visualization_pose_ = *ligand_; // make a copy of the given pose
				new_pose_to_be_visualized = true;
				visualization_pose_score_ = score;

				visualization_timer_.stop();
				visualization_timer_.reset();
				visualization_timer_.start();
			}
		}
	}

	double DockingAlgorithm::getVisualizationPoseScore()
	{
		return visualization_pose_score_;
	}

	void DockingAlgorithm::setMaxFps(int no)
	{
		min_sec_between_visualizations_ = 1./no;
	}

	void DockingAlgorithm::start()
	{
		pause_ = false;
		abort_ = false;
		finished_ = false;

		visualization_timer_.stop();
		visualization_timer_.reset();
		visualization_timer_.start();
	}

	void DockingAlgorithm::pause()
	{
		pause_ = true;
	}

	void DockingAlgorithm::proceed()
	{
		pause_ = false;
		abort_ = false;
	}

	void DockingAlgorithm::abort()
	{
		abort_ = true;
	}

	void DockingAlgorithm::reset()
	{
		pause_ = false;
		abort_ = false;
		finished_ = false;

		new_pose_to_be_visualized = 0;
	}

	void DockingAlgorithm::finish()
	{
		finished_ = true;
		visualization_timer_.stop();
		visualization_timer_.reset();
	}

	bool DockingAlgorithm::hasFinished() const
	{
		return finished_;
	}

	float DockingAlgorithm::getProgress() const
	{
		Log.error() << "This is just an interface definition. Check your dynamic binding" << endl;
		return 0;
	}

	ConformationSet DockingAlgorithm::getConformationSet(Index /*total_conformations*/)
	{
		ConformationSet dummy;
		return dummy;
	}

	ScoringFunction* DockingAlgorithm::getScoringFunction()
	{
		return scoring_function_;
	}

	void DockingAlgorithm::processMultiMoleculeFile(string input_filename, string output_filename, double score_cutoff, vector<double>* min_atoms_in_ref_areas, String toolinfo, String timestamp)
	{
		GenericMolFile* input = MolFileFactory::open(input_filename);
		if (!input)
		{
			String m = "Format of input file '"+input_filename+"' is not supported!";
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "DockingAlgorithm::processMuliMoleculeFile() error", m);
		}

		GenericMolFile* output = MolFileFactory::open(output_filename, ios::out, input);
		if (!output)
		{
			String m = "Format of output file '"+output_filename+"' is not supported!";
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "DockingAlgorithm::processMuliMoleculeFile() error", m);
		}
		
		DockResultFile* drf_output = dynamic_cast<DockResultFile*>(output);
		if (drf_output)
		{
			String dummy = "0";
			drf_output->setOutputParameters(Result::DOCKING, "score", dummy, name_+"+"+scoring_function_->getName());
			drf_output->setToolInfo(toolinfo, timestamp);
		}
		

		bool output_failed_dockings = (options.setDefaultBool("output_failed_dockings", false) && score_cutoff>=1e10);

		StructurePreparer sp;
		if (scoring_type_.hasSubstring("PLP"))
		{
			sp.setScoringType("PLP");
		}

		Timer timer;
		timer.start();
		int no_dockings = 0;

		ScoringFunction* scoring = getScoringFunction();

		bool b = 1;
		for (int mol_no = 1; b; mol_no++)
		{
			double score = 1e100;

			//AtomContainer ligand_i;
			Log.level(20)<<"====== ligand candidate "<<mol_no<<flush;
			Molecule* ligand_i = NULL;
			try
			{
				ligand_i = input->read();
				if (ligand_i == NULL) break;

				String name = ligand_i->getName();
				if (name != "") Log.level(20)<<", "<<name<<" ";
				Log.level(20)<<"============"<<endl<<flush;

				if (ligand_i->hasProperty("score_ligcheck"))
				{
					double score_ligcheck = ((String)ligand_i->getProperty("score_ligcheck").toString()).toDouble();
					if (score_ligcheck < 0.95) // 0 = error, 1 = check passed
					{
						Log.level(20)<<"Skipping compound because it has been marked as containing errors by LigCheck."<<endl;

						if (output_failed_dockings)
						{
							ligand_i->setProperty("score", 1e12);
							ligand_i->setProperty("docking-error", "molecule ignored because it did not pass LigCheck test");
							*output << *ligand_i;
							output->flush();
						}
						delete ligand_i;
						continue;
					}
				}

				sp.prepare(ligand_i, parameter_filename_);
				score = dockLigand(*ligand_i);
				Log.level(20)<<endl;
				scoring->printResult();

				if (score < score_cutoff)
				{
					list<Constraint*>& refs = scoring->constraints;
					vector<double> atoms_in_ref_areas(refs.size(), 0);

					int i = 0;
					for (list < Constraint* > ::iterator it = refs.begin(); it != refs.end(); it++, i++)
					{
						ReferenceArea* ref = dynamic_cast<ReferenceArea*>(*it);
						if (!ref) continue;
						atoms_in_ref_areas[i] = ref->getContainedAtoms();
						String name = "atoms in ";
						String n = ref->getName();
						if (name != "")
						{
							name += n;
						}
						else
						{
							name += "ReferenceArea "+String(i);
						}

						ligand_i->setProperty(name, atoms_in_ref_areas[i]);
					}

					ligand_i->setProperty("score", score);

					bool ok = 1;
					if (min_atoms_in_ref_areas != NULL)
					{
						for (Size i = 0; i < min_atoms_in_ref_areas->size() && i < atoms_in_ref_areas.size(); i++)
						{
							if ((*min_atoms_in_ref_areas)[i] > atoms_in_ref_areas[i])
							{
								ok = 0;
								break;
							}
						}
					}
					if (ok)
					{
						*output << *ligand_i;
						output->flush();
					}
				}
			}
			catch(BALL::Exception::GeneralException e)
			{
				Log.level(20)<<"Error for compound "<<mol_no<<" ! Skipping this compound."<<endl;

				if (output_failed_dockings)
				{
					ligand_i->setProperty("score", 1e12);
					ligand_i->setProperty("docking-error", e.getMessage());
					*output << *ligand_i;
					output->flush();
				}

				delete ligand_i;
				continue;
			}
			no_dockings++;
			Log.level(20)<<endl;
			delete ligand_i;
		}
		timer.stop();
		Log.level(20)<<"\nDocking "<<no_dockings<<" compounds: "<<scoring->convertTime(timer.getClockTime())<<endl;

		delete input;
		delete output;
	}



	void DockingAlgorithm::mapLigandOntoReferenceLigand()
	{
		if (!scoring_function_)
		{
			Log.error()<<"Error DockingAlgorithm::mapLigandOntoReferenceLigand() : ScoringFunction not set!"<<endl;
			return;
		}

		AtomContainer* ligand = scoring_function_->getLigand();

		if (!ligand)
		{
			Log.error()<<"Error DockingAlgorithm::mapLigandOntoReferenceLigand() : Ligand not set!"<<endl;
			return;
		}
		if (!reference_ligand_)
		{
			Log.error()<<"Error DockingAlgorithm::mapLigandOntoReferenceLigand() : Reference ligand not set!"<<endl;
			return;
		}

		Timer timer;
		timer.start();

		double lower_bound = 2;
		double upper_bound = 5;
		double tolerance = 1;
		Size no_matched_atoms = 0;
		double rmsd = 0;

		Matrix4x4 T = mapCompounds(*ligand, *reference_ligand_, no_matched_atoms, rmsd, upper_bound, lower_bound, tolerance);

		for (AtomIterator it = ligand->beginAtom(); +it; it++)
		{
			it->setPosition(T*it->getPosition());
		}

		timer.stop();
		Log.level(10)<<"superposing ligand: "<<timer.getClockTime()<<" seconds"<<endl;
	}


	double DockingAlgorithm::calculateRMSD(const AtomContainer& S1, const AtomContainer& S2, int* no_matched_atoms)
	{
		double lower_bound = 2;
		double upper_bound = 5;
		double tolerance = 1;
		Size matched_atoms = 0;
		double rmsd = 0;

		mapCompounds(S1, S2, matched_atoms, rmsd, upper_bound, lower_bound, tolerance);

		if (no_matched_atoms != NULL) *no_matched_atoms = matched_atoms;

		return rmsd;
	}


	Matrix4x4 DockingAlgorithm::mapCompounds(const AtomContainer& S1, const AtomContainer& S2, Size& no_matched_heavy_atoms, double& rmsd, double upper_bound, double lower_bound, double tolerance)
	{
	// calculate bounding box of protein S1
		BoundingBoxProcessor box_processor;

		// BoundingBoxProcessor can unfortunately not operate on const AtomContainer, althought it does NOT change the system
		AtomContainer* S1nc = const_cast<AtomContainer*>(&S1);
		S1nc->apply(box_processor);

	/// insert positions of the heavy atoms of S1 into a three-dimensional hashgrid and in the array heavy_atoms

		Vector3 upper_bound_vector(upper_bound, upper_bound, upper_bound);

		HashGrid3 < Position > grid_S1(box_processor.getLower() - upper_bound_vector, box_processor.getUpper() - box_processor.getLower() + (float) 2.0 * upper_bound_vector, upper_bound);

		AtomConstIterator atom_it;
		vector<Vector3> heavy_atoms_S1;
//	vector<Position> index_heavy_S1;
		Position no_heavy_S1 = 0;

		for (atom_it = S1.beginAtom(); +atom_it; ++atom_it)
		{
			if (((*atom_it).getElement() != PTE[Element::H]))
			{
				grid_S1.insert((*atom_it).getPosition(), no_heavy_S1);
				no_heavy_S1++;
				heavy_atoms_S1.push_back((*atom_it).getPosition());
	// index_heavy_S1.push_back(type_map[(*atom_it).getFragment()->getName()]);
			}
		}
	// calculate bounding box of protein S2

		// BoundingBoxProcessor can unfortunately not operate on const AtomContainer, althought it does NOT change the system
		AtomContainer* S2nc = const_cast<AtomContainer*>(&S2);
		S2nc->apply(box_processor);

	/// insert positions of the heavy atoms of S2 into the hashgrid grid_S2
		HashGrid3<Position> grid_S2(box_processor.getLower() - upper_bound_vector, box_processor.getUpper() - box_processor.getLower() +(float) 2.0 * upper_bound_vector, upper_bound);
		Vector3 tolerance_vector(2 * tolerance, 2 * tolerance, 2 * tolerance);
		HashGrid3<Position> fine_grid_S2(box_processor.getLower() - tolerance_vector, box_processor.getUpper() - box_processor.getLower() + tolerance_vector, 2 * tolerance);

		vector<Vector3> heavy_atoms_S2;
//	vector<Position> index_heavy_S2;
		Size no_heavy_S2 = 0;

		for (atom_it = S2.beginAtom(); +atom_it; ++atom_it)
		{
			if (((*atom_it).getElement() != PTE[Element::H]))
			{
				grid_S2.insert((*atom_it).getPosition(), no_heavy_S2);
				fine_grid_S2.insert((*atom_it).getPosition(), no_heavy_S2);

				no_heavy_S2++;
				heavy_atoms_S2.push_back((*atom_it).getPosition());
	// index_heavy_S2.push_back(type_map[(*atom_it).getFragment()->getName()]);
			}
		}

	/// calculate triangles between heavy atoms of S2 whose edge length are larger than lower_bound
	/// and smaller than upperbound and store them in a hashgrid with respect to their edge length
		Vector3 upper(upper_bound + 1, upper_bound + 1, upper_bound + 1);
		Vector3 lower(lower_bound - 1, lower_bound - 1, lower_bound - 1);

		HashGrid3<TVector3<Position> > triangles_S2(lower, upper - lower, tolerance); // tolerance == spacing of hashgrid

		HashGrid3 < Position >::BoxIterator b_it1;
		HashGridBox3 < Position >::BoxIterator b_it2, b_it3;
		HashGridBox3 < Position >::DataIterator d_it1, d_it2, d_it3;
		TVector3 < Position > index_vector;
		Vector3 distance_vector;
		float square_upper = upper_bound * upper_bound;
		float square_lower = lower_bound * lower_bound;
		float distance1, distance2, distance3;

		for (b_it1 = grid_S2.beginBox(); +b_it1; ++b_it1)
		{
			for (d_it1 = (*b_it1).beginData(); +d_it1; ++d_it1)
			{
				for (b_it2 = (*b_it1).beginBox(); +b_it2; ++b_it2)
				{
					for (d_it2 = (*b_it2).beginData(); +d_it2; ++d_it2)
					{
						if ((*d_it2) != (*d_it1))
						{
							distance1 = heavy_atoms_S2[(*d_it1)].getSquareDistance(heavy_atoms_S2[(*d_it2)]);
							if (distance1 < square_upper && distance1 > square_lower)
							{
								for (b_it3 = (*b_it1).beginBox(); +b_it3; ++b_it3)
								{
									for (d_it3 = (*b_it3).beginData(); +d_it3; ++d_it3)
									{
										if ((*d_it3) != (*d_it1) && (*d_it3) != (*d_it2))
										{
										distance2 = heavy_atoms_S2[(*d_it1)].getSquareDistance(heavy_atoms_S2[(*d_it3)]);
										if (distance2 < square_upper && distance2 > square_lower)
										{
										distance3 = heavy_atoms_S2[(*d_it2)].getSquareDistance(heavy_atoms_S2[(*d_it3)]);
										if (distance3 < square_upper && distance3 > square_lower)
										{
										distance1 = sqrt(distance1);
										distance2 = sqrt(distance2);
										distance3 = sqrt(distance3);
										distance_vector.set(distance1, distance2, distance3);
										index_vector.set((*d_it1), (*d_it2), (*d_it3));
										triangles_S2.insert(distance_vector, index_vector);
										}
										}
										}
									}
								}
							}
						}
					}
				}
			}
		}

	/// calculate all triangles between heavy atoms of S1 and
	/// search similar triangles between heavy atoms of S2 stored in triangles_S2
		HashGridBox3 < TVector3 < Position > >::BoxIterator b_it4;
		HashGridBox3 < TVector3 < Position > >::DataIterator d_it4;
		HashGridBox3 < TVector3 < Position > >*box;
		HashGridBox3 < Position > *ibox;
		HashGridBox3 < Position >::BoxIterator ibox_it;
		HashGridBox3 < Position >::DataIterator id_it;

		Matrix4x4 T_best;
		const float square_tolerance = tolerance * tolerance;
		no_matched_heavy_atoms = 0;

		for (b_it1 = grid_S1.beginBox(); +b_it1; ++b_it1)
		{
			for (d_it1 = (*b_it1).beginData(); +d_it1; ++d_it1)
			{
				for (b_it2 = (*b_it1).beginBox(); +b_it2; ++b_it2)
				{
					for (d_it2 = (*b_it2).beginData(); +d_it2; ++d_it2)
					{
						if ((*d_it2) != (*d_it1))
						{
							distance1 = heavy_atoms_S1[(*d_it1)].getSquareDistance(heavy_atoms_S1[(*d_it2)]);
							if (distance1 < square_upper && distance1 > square_lower)
							{
								distance1 = sqrt(distance1);
								for (b_it3 = (*b_it1).beginBox(); +b_it3; ++b_it3)
								{
									for (d_it3 = (*b_it3).beginData(); +d_it3; ++d_it3)
									{
										if ((*d_it3) != (*d_it1) && (*d_it3) != (*d_it2))
										{
										distance2 = heavy_atoms_S1[*d_it1].getSquareDistance(heavy_atoms_S1[*d_it3]);
										if (distance2 < square_upper && distance2 > square_lower)
										{
										distance2 = sqrt(distance2);
										distance3 = heavy_atoms_S1[*d_it2].getSquareDistance(heavy_atoms_S1[*d_it3]);
										if (distance3 < square_upper && distance3 > square_lower)
										{
										distance3 = sqrt(distance3);
										distance_vector.set(distance1, distance2, distance3);
										index_vector.set(*d_it1, *d_it2, *d_it3);     // the positions of the atoms of S1 that are part of this triangle
										box = triangles_S2.getBox(distance_vector);

				/// iterate over all triangles of S2 that have similar edge lengths
										for (b_it4 = box->beginBox(); +b_it4; ++b_it4) // iterate over all buckets...
										{
										for (d_it4 = (*b_it4).beginData(); +d_it4; ++d_it4)
										{
					//   if (index_heavy_S1[(*d_it1)] == index_heavy_S2[(*d_it4).x] && index_heavy_S1[(*d_it2)] == index_heavy_S2[(*d_it4).y] && index_heavy_S1[(*d_it3)] == index_heavy_S2[(*d_it4).z])
										{
										Matrix4x4 T = StructureMapper::matchPoints(
												heavy_atoms_S1[(*d_it1)],
												heavy_atoms_S1[(*d_it2)],
												heavy_atoms_S1[(*d_it3)],
												heavy_atoms_S2[(*d_it4).x],
												heavy_atoms_S2[(*d_it4).y],
												heavy_atoms_S2[(*d_it4).z]
										);

										size_t matched_heavy_atoms = 0u;
										float current_rmsd = 0.0f;

						/// iterate over all heavy atoms of S1 and define them as matched if there is an atom of S2 within square_tolerance;
						/// calculate RMSD out of all matched atoms
										for (Size i = 0; i < no_heavy_S1; i++)
										{
										Vector3 v = T * heavy_atoms_S1[i];
										ibox = fine_grid_S2.getBox(v);

										if (ibox != 0)
										{
										bool matched = false;

										for (ibox_it = ibox->beginBox(); +ibox_it && !matched; ++ibox_it)
										{
										for (id_it = (*ibox_it).beginData(); +id_it && !matched; ++id_it)
										{
										float squared_atom_dist = v.getSquareDistance(heavy_atoms_S2[(*id_it)]);
										if (squared_atom_dist <= square_tolerance)
										{
										matched_heavy_atoms++;
										matched = true;
										current_rmsd += squared_atom_dist;
										}
										}
										}
										}
										}

										if (matched_heavy_atoms >= no_matched_heavy_atoms)
										{
										current_rmsd = sqrt(current_rmsd / matched_heavy_atoms);
										if (matched_heavy_atoms == no_matched_heavy_atoms)
										{
										if (current_rmsd < rmsd)
										{
										T_best = T;
										rmsd = current_rmsd;
										}
										}
										else // if more atoms could be mapped, always use new
										{	// transformation
										T_best = T;
										rmsd = current_rmsd;
										}
										no_matched_heavy_atoms = matched_heavy_atoms;
										}
										}
										}
										}
										}
										}
										}
									}
								}
							}
						}
					}
				}
			}
		}

		return T_best;
	}

	const AtomContainer* DockingAlgorithm::getLigand()
	{
		return ligand_;
	}

	const AtomContainer* DockingAlgorithm::getReferenceLigand()
	{
		return reference_ligand_;
	}
}

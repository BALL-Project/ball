// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// A small program for assigning positions based on one structure to another

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/STRUCTURE/geometricProperties.h>

#include <Eigen/Core>
#include <Eigen/Eigenvalues>


using namespace std;
using namespace BALL;

int main(int argc, char** argv)
{
	if ((argc < 6))
	{
		Log << "Usage:" << argv[0] << " <PDB infile to place> <chain to place> <PDB infile template> <chain template> <PDB outfile> " << endl;
		return 1;
	}

	Log << "Loading " << argv[1] << "...";
	PDBFile infile(argv[1]);
	if (!infile)
	{
		// if file does not exist: complain and abort
		Log.error() << "error opening " << argv[1] << " for input." << endl;
		return 2;
	}

	System system;
	infile >> system;
	Log << "  done." << endl;

	Log << "Loading " << argv[3] << "...";
	PDBFile template_infile(argv[3]);
	if (!template_infile)
	{
		// if file does not exist: complain and abort
		Log.error() << "error opening " << argv[3] << " for input." << endl;
		return 2;
	}

	System template_system;
	template_infile >> template_system;
	Log << "  done." << endl;

	// check if both chain are present
	Chain* chain_to_place = NULL;
	Chain* template_chain = NULL;

	// find a matching between chain_to_place and template_chain
	BALL::ChainIterator ch_it = system.beginChain();
	for (; +ch_it; ++ch_it)
	{
	  if (ch_it->getName() == String(argv[2]))
		{
			chain_to_place = &*ch_it;
			break;
		}
	}
	if (!chain_to_place)
	{
		Log.error() << "could not find chain " << argv[2] << " in file " << argv[1] << "." << endl;
			return 2;
	}

	ch_it = template_system.beginChain();
	for (; +ch_it; ++ch_it)
	{
		if (ch_it->getName() == String(argv[4]))
		{
			template_chain = &*ch_it;
			break;
		}
	}
	if (!template_chain)
	{
		Log.error() << "could not find chain " << argv[4] << " in file " << argv[3] << "." << endl;
			return 2;
	}
	
	// map chains onto each other to find the best transformation
	// TODO: we have to handle amino acids differently from small molecules	
	Log << "Mapping ";

	AtomBijection bijection;

	bool is_peptide = (chain_to_place->beginResidue() != chain_to_place->endResidue());
	ResidueIterator resit = chain_to_place->beginResidue();
	for (; +resit ; ++resit)
	{
		 if (!resit->isAminoAcid())
			 is_peptide = false;
	}

	if (is_peptide)
	{
		Log << " peptides ";

		StructureMapper mapper(*chain_to_place, *template_chain);
		vector<Fragment*> frags_chain_p;
		vector<Fragment*> frags_chain_t;
		for (ResidueIterator resit = chain_to_place->beginResidue(); +resit ; ++resit)
		{
			frags_chain_p.push_back(&*resit);
		}
		for (ResidueIterator resit = template_chain->beginResidue(); +resit ; ++resit)
		{
			frags_chain_t.push_back(&*resit);
		}

		// compute the transformation
		// TODO is the orientation correct?
		Matrix4x4 mat =  Matrix4x4::getIdentity();
		mapper.mapFragments(frags_chain_p, frags_chain_t, &mat, 3., 5.);
		bijection = mapper.getBijection();

		// transform
		chain_to_place->apply(mapper);

		Log <<  bijection.size() << " of " << chain_to_place->countAtoms() << " atoms in chain " << argv[2] << " are matched." << endl;
	}
	else
	{
		Log << " non peptides ";

		// lets apply a heuristic
		//   idea: use covariance matrices to compute eigenvectors
		//         to determine the rotation matrix

		// first map the center of mass
		GeometricCenterProcessor center;
		chain_to_place->apply(center);
		Vector3 toOrigin = center.getCenter();

		// create the translation
		TranslationProcessor translation;
		translation.setTranslation(toOrigin.negate());

		// translate the molecule
		chain_to_place->apply(translation);

		// store the vector to the templates center
		template_chain->apply(center);
		Vector3 origin_to_template = center.getCenter();

		// move the template into the center as well
		translation.setTranslation(origin_to_template.negate());
		template_chain->apply(translation);

		// compute both covariance matrices
		Eigen::Matrix3f cov_original(Eigen::Matrix3f::Zero());
		Eigen::Matrix3f cov_new(Eigen::Matrix3f::Zero());

		for (AtomIterator at_it = template_chain->beginAtom(); +at_it; ++at_it)
		{
			if (at_it->getElement().getSymbol() != "H")
			{
				Vector3& pos = at_it->getPosition();
				cov_original.selfadjointView<Eigen::Lower>().rankUpdate(Eigen::Vector3f(pos.x, pos.y, pos.z));
			}
		}

		// compute the eigenvectors
		Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> eigen_solver_original(cov_original, Eigen::ComputeEigenvectors);

		for (AtomIterator at_it = chain_to_place->beginAtom(); +at_it; ++at_it)
		{
			if (at_it->getElement().getSymbol() != "H")
			{
				Vector3& pos = at_it->getPosition();
				cov_new.selfadjointView<Eigen::Lower>().rankUpdate(Eigen::Vector3f(pos.x, pos.y, pos.z));
			}
		}

		// compute the eigenvectors
		Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> eigen_solver_new(cov_new, Eigen::ComputeEigenvectors);

		// and compute the final rotation matrix
		Eigen::Matrix3f rot = eigen_solver_original.eigenvectors() * eigen_solver_new.eigenvectors().transpose();

		// convert it into a BALL transformation
		Matrix4x4 trans_mat(
			rot(0,0), rot(0,1), rot(0,2), -origin_to_template.x,
			rot(1,0), rot(1,1), rot(1,2), -origin_to_template.y,
			rot(2,0), rot(2,1), rot(2,2), -origin_to_template.z,
			       0,        0,        0,        1
		);
		TransformationProcessor transform(trans_mat);

		// move the molecule back to its the center position of the template
		chain_to_place->apply(transform);
	}
	Log << "  done." << endl;

	Log << "Writing " << argv[5] << "...";
	PDBFile outfile(argv[5], std::ios::out);
	outfile << system;
	Log << "  done." << endl;

	return 0;
}


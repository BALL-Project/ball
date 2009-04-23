
#include <BALL/XRAY/crystalGenerator.h>
#include <BALL/KERNEL/system.h>

using namespace std;


namespace BALL
{
	CrystalGenerator::CrystalGenerator()
		:	system_(0),
			asu_(0),
			unitcell_(0)
	{
	}
	
	CrystalGenerator::CrystalGenerator(const CrystalGenerator& cg)
		:	system_(0),
			asu_(0),
			unitcell_(0)
	{
	}

	CrystalGenerator::CrystalGenerator(System* system)
		:	system_(system),
			asu_(0),
			unitcell_(0)
	{
		setSystem(system);
	}
	
	CrystalGenerator::~CrystalGenerator()
	{
		system_ = 0;
		asu_ = 0;
		unitcell_ = 0;
		ci_ptr_ = 0;
	}
	//bool CrystalGenerator::buildASU_()
	//{
	//	System* ncs_component;
	//	for(Position n = 0; n < ci_.getNumberOfSymOps(); n++)
	//	{
	//		
	//		
	//	}
	//	return true;
	//}
	void CrystalGenerator::setCrystalInfo(CrystalInfo& ci)
	{

	}

	void CrystalGenerator::setSystem(System* system_ptr)
	{
		
		if (system_ptr->countProteins() == 0)
		{
			Log.warn() << "System does not contain any proteins" << std::endl;
		}
		else
		{
			//Iterate over all proteins in the system and look for the first CrystalInfo object
			for (ProteinIterator pit = system_ptr->beginProtein(); !pit.isEnd(); ++pit) 		
			{
				if (pit->hasProperty("CRYSTALINFO"))
				{
					Log.info() << "CrystalInfo found in Protein " << pit->getName() << std::endl;
					ci_ptr_ = dynamic_cast<CrystalInfo*>(pit->getProperty("CRYSTALINFO").getObject());	
					break;
				}
			}
		}
		if ( ci_ptr_ == 0)
		{
			Log.warn() << "No CrystalInfo object found, generating default CrystalInfo" << std::endl;
			ci_ptr_ = new CrystalInfo();
		}
		//Protein* prot = dynamic_cast<Protein*>(system->getMolecule(0));
		//if( prot != 0)
		//{
		//	if (prot->hasProperty("CRYSTALINFO"))
		//	{
		//		cout << "hasProperty" << endl;
		//		ci_ptr_ = dynamic_cast<CrystalInfo*>(prot->getProperty("CRYSTALINFO").getObject());	
		//	}
		//	else	
		//	{
		//		cout << "hasNoProperty" << endl;
		//		ci_ptr_ = new CrystalInfo();
		//	}
		//}
		//else
		//{
		//	cout << "hasNoProperty and contains no protein" << endl;
		//	ci_ptr_ = new CrystalInfo();
		//}

		if (buildASU_())
		{
			cout << "ASU successfully build" << endl;
			if (buildUnitCell_())
			{
				cout << "UnitCell successfully build" << endl;
			}

		}


	}

	//void CrystalGenerator::setDensity(RegularData3D* dens_ptr)
	//{

	//}


	System* CrystalGenerator::generatePacking(Index a, Index b, Index c)
	{
		//System* crystal = new System();
		//crystal->setName("CRYSTAL");
		//for (Index a = 0; a <= )
		return 0;
	}

	System* CrystalGenerator::generateUnitCell(Index a, Index b, Index c)
	{
		Matrix4x4 transmatrix;
		transmatrix.setIdentity();
		System* unitcell = new System(*unitcell_);
		unitcell->setName("UNITCELL (" + String(a) + "|" + String(b) + "|" + String(c) +")");
		transformer_.setTransformation(ci_ptr_->getCart2Frac());
		unitcell->apply(transformer_);
		transmatrix.setTranslation(Vector3(a,b,c));
		transformer_.setTransformation(transmatrix);
		unitcell->apply(transformer_);
		transformer_.setTransformation(ci_ptr_->getFrac2Cart());
		unitcell->apply(transformer_);
		return unitcell;
	}
	
	System* CrystalGenerator::generateUnitCell()
	{
		return generateUnitCell(0,0,0);
	}

	System* CrystalGenerator::generateAsymmetricUnit()
	{
		cout << unitcell_ << endl;	
		return asu_;
	}
	
	System* CrystalGenerator::generateSymMoleculesWithinDistance(float angstrom)
	{
		return 0;
	}
	
	Box3 CrystalGenerator::getUnitCellBox(Index a, Index b, Index c)
	{
		Vector3 origin = ci_ptr_->getFrac2Cart() * Vector3(a,b,c);
		Vector3 right = ci_ptr_->getFrac2Cart() * Vector3(1,0,0);
		Vector3 height = ci_ptr_->getFrac2Cart() * Vector3(0,1,0);
		Vector3 depth  = ci_ptr_->getFrac2Cart() * Vector3(0,0,1);
		Box3 cell(origin, right, height, depth);
		return cell;
	}

	bool CrystalGenerator::buildUnitCell_()
	{
		unitcell_ = new System();
		unitcell_->setName("UNITCELL (0|0|0)");
		Size symops_size = ci_ptr_->getNumberOfSymOps();

		cout << "symops_size " << symops_size << endl;
		//System* current_asu;

		for (Position i = 0; i < symops_size; i++)
		{
			cout << "symop " << i;
			System* current_asu = new System(*asu_);
			transformer_.setTransformation(ci_ptr_->getCart2Frac());
			current_asu->apply(transformer_);
			transformer_.setTransformation(ci_ptr_->getSymOp(i));
			current_asu->apply(transformer_);
			transformer_.setTransformation(ci_ptr_->getFrac2Cart());
			current_asu->apply(transformer_);
			
			unitcell_->spliceAfter(*current_asu);

			cout << endl;
		}

		MoleculeIterator mi;
		Matrix4x4 corr_matrix;
		Vector3 center;
		Vector3 corr_trans;
		for(mi = unitcell_->beginMolecule(); mi != unitcell_->endMolecule(); ++mi)
		{
			corr_matrix.setIdentity();
			corr_trans = Vector3(0,0,0);
			transformer_.setTransformation(ci_ptr_->getCart2Frac());
			mi->apply(transformer_);
			mi->apply(center_processor_);
			center = center_processor_.getCenter();
			for (int k=0; k<3; k++)
			{
				if (center[k] < 0) corr_trans[k] = 1;
				else if (center[k] >= 1) corr_trans[k] = -1;
			}
			//cout << center << endl;
			//cout << corr_trans << endl;
			corr_matrix.setTranslation(corr_trans);
			transformer_.setTransformation(corr_matrix);
			mi->apply(transformer_);
			transformer_.setTransformation(ci_ptr_->getFrac2Cart());
			mi->apply(transformer_);
		}

		cout << "uc_cP" << unitcell_->countProteins() << endl;
		cout << "uc_cM" << unitcell_->countMolecules() << endl;
		return true;
	}

	bool CrystalGenerator::buildASU_()
	{
		asu_ = new System();
		Size ncs_symops_size = ci_ptr_->getNumberOfNCSSymOps();
		cout << "ncs_symops_size" << ncs_symops_size << endl;
		System* current_ncs;

		current_ncs = new System(*system_);
		asu_->spliceAfter(*current_ncs);
		for (Position i = 0; i < ncs_symops_size; i++)
		{
			cout << "ncs " << i;
			current_ncs = new System(*system_);
			if (!ci_ptr_->isgivenNCS(i))
			{
				transformer_.setTransformation(ci_ptr_->getNCS(i));
				current_ncs->apply(transformer_);
				asu_->spliceAfter(*current_ncs);
			}
			else
			{
				cout << " isgiven ";
			}
			cout << endl;
		}

		cout << "asu_cP" << asu_->countProteins() << endl;
		cout << "asu_cM" << asu_->countMolecules() << endl;
		return true;
	}


}

// namespace BALL

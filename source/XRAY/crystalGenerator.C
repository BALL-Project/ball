#include <BALL/XRAY/crystalGenerator.h>
#include <BALL/KERNEL/system.h>

using namespace std;


namespace BALL
{

	const string CrystalGenerator::Option::SPACE_GROUP_FILE= "filename";
	const string CrystalGenerator::Default::SPACE_GROUP_FILE= "XRAY/spacegroups-details.ini";
	
	CrystalGenerator::CrystalGenerator()
		: center_processor_(),
			transformer_(),
			system_(0),
			asu_(0),
			unitcell_(0),
			filename_(Default::SPACE_GROUP_FILE)
	{
	}
	
	CrystalGenerator::CrystalGenerator(const CrystalGenerator& cg)
		:	center_processor_(cg.center_processor_),
			transformer_(cg.transformer_),
			system_(cg.system_),
			asu_(cg.asu_),
			unitcell_(cg.unitcell_),
			filename_(cg.filename_)
	{
	}

	CrystalGenerator::~CrystalGenerator()
	{
		system_ = 0;
		asu_ = 0;
		unitcell_ = 0;
	}
	
	void CrystalGenerator::setCrystalInfo(boost::shared_ptr<CrystalInfo> ci_ptr)
	{
		ci_ptr_ = ci_ptr;
		if (buildASU_())
		{
			cout << "ASU successfully build" << endl;
			if (buildUnitCell_())
			{
				cout << "UnitCell successfully build" << endl;
			}

		}
	}

	void CrystalGenerator::setSystem(System* system_ptr)
	{
			
		system_ = system_ptr;	
		ci_ptr_.reset();
		
		if (system_->countProteins() == 0)
		{
			Log.warn() << "System does not contain any proteins" << std::endl;
		}
		else
		{
			//Iterate over all proteins in the system and look for the first CrystalInfo object
			for (ProteinIterator pit = system_->beginProtein(); !pit.isEnd(); ++pit) 		
			{
				if (pit->hasProperty("CRYSTALINFO"))
				{
					Log.info() << "CrystalInfo found in Protein " << pit->getName() << std::endl;
					ci_ptr_ = boost::dynamic_pointer_cast<CrystalInfo>(pit->getProperty("CRYSTALINFO").getSmartObject());	
					break;
				}
			}
		}
		
		if (!ci_ptr_)
		{
			Log.warn() << "No CrystalInfo object found, generating default CrystalInfo" << std::endl;
			boost::shared_ptr<CrystalInfo> tmp_ptr(new CrystalInfo());
			ci_ptr_ = tmp_ptr;
		}

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


	std::list<System*> CrystalGenerator::generatePacking(Index a_loweridx, Index a_upperidx, Index b_loweridx, Index b_upperidx, Index c_loweridx, Index c_upperidx)
	{
		if (a_loweridx > a_upperidx) std::swap(a_loweridx, a_upperidx);		
		if (b_loweridx > b_upperidx) std::swap(b_loweridx, b_upperidx);		
		if (c_loweridx > c_upperidx) std::swap(c_loweridx, c_upperidx);		

		std::list<System*> crystal;	
		for (Index a = a_loweridx; a <= a_upperidx; a++)
		{
			for (Index b = b_loweridx; b <= b_upperidx; b++)
			{
				for (Index c = c_loweridx; c <= c_upperidx; c++)
				{
					crystal.push_back(generateUnitCell(a,b,c));	
				}
			}
		}
		return crystal;
	}

	System* CrystalGenerator::generateUnitCell(Index a, Index b, Index c)
	{

		System* unitcell = new System(*unitcell_);
		unitcell->setName("UNITCELL (" + String(a) + "|" + String(b) + "|" + String(c) +")");
		
		// set CELLTYPE to UNITCELL = 2, TODO: switch to an enum later
		unitcell->setProperty("CELLTYPE", (unsigned int)2 );	
		unitcell->setProperty("UC_IDX_A", (int)a );	
		unitcell->setProperty("UC_IDX_B", (int)b );	
		unitcell->setProperty("UC_IDX_C", (int)c );	
		
		// define a global transformation matrix
		Matrix4x4 transmatrix;
		transmatrix.setIdentity();
		
		Matrix4x4 translationmatrix;
		transmatrix.setIdentity();
		translationmatrix.setTranslation(Vector3(a,b,c));
		
		//to apply the transformation correctly, be aware that the order of multiplications is reversed
		// assume two linear mappings:
		//  		f: Cart -> Frac
		//  		g: Cart -> Frac
		// composition of both means for matrix multiplications:
		// 			g x f
		transmatrix *= ci_ptr_->getFrac2Cart();
		transmatrix *= translationmatrix;
		transmatrix *= ci_ptr_->getCart2Frac();
		transformer_.setTransformation(transmatrix);
		unitcell->apply(transformer_);
			
		//Set Unit Cell index for all Molecules in the unit cell
		MoleculeIterator mit = unitcell->beginMolecule();
		for(; mit != unitcell->endMolecule(); ++mit)
		{
			mit->setProperty("UC_IDX_A", (int)a );	
			mit->setProperty("UC_IDX_B", (int)b );	
			mit->setProperty("UC_IDX_C", (int)c );	
		}

		return unitcell;
	}
	
	System* CrystalGenerator::generateUnitCell()
	{
		return generateUnitCell(0,0,0);
	}

	System* CrystalGenerator::generateAsymmetricUnit()
	{
		return asu_;
	}
	
	std::list<System*> CrystalGenerator::generateSymMoleculesWithinDistance(float angstrom)
	{
		// As we calculate the bounding boxes in fractional space
		// Scale the distance for every axis
		Vector3 distance = Vector3(angstrom, angstrom, angstrom);
		distance = ci_ptr_->getCart2Frac() * distance;
		
		transformer_.setTransformation(ci_ptr_->getCart2Frac());
		system_->apply(transformer_);
		
		// calulate the bounding box of the system, extended about the distance
		BoundingBoxProcessor bbp;	
		system_->apply(bbp);
		
		Vector3 sys_lo =  bbp.getLower();
    Vector3 sys_up =  bbp.getUpper();
    sys_lo =  sys_lo - distance;
    sys_up =  sys_up + distance;
		
		// calculate bounding box for every molecule in the unit cell	
		// and store them in a vector
		vector<SimpleBox3> boundingboxes;
		MoleculeIterator mit = unitcell_->beginMolecule();
		for (; mit != unitcell_->endMolecule(); ++mit)
		{
			transformer_.setTransformation(ci_ptr_->getCart2Frac());
			mit->apply(transformer_);
			mit->apply(bbp);
			boundingboxes.push_back(bbp.getBox());
			transformer_.setTransformation(ci_ptr_->getFrac2Cart());
			mit->apply(transformer_);
		}
		
		//for every surrounding unit cell 
		Vector3 lo, up;
		vector<BitVector> boundingbits_vector;		
		for (Index i = -1; i <= 1; i++)
		{
			for (Index j = -1; j <= 1; j++)
			{
				for (Index k = -1; k <= 1; k++)
				{
					//	check if any of the bounding boxes reaches into the extended bounding box of the origin system 
					BitVector boundingbits;
					boundingbits.setSize(boundingboxes.size());
					for (vector<SimpleBox3>::iterator it = boundingboxes.begin(); it != boundingboxes.end(); ++it)
					{
						it->get(lo, up);

						bool bbit =(	(		((lo.x + i > sys_lo.x) && (lo.x +i <= sys_up.x)) 
													&&	((lo.y + j > sys_lo.y) && (lo.y +j <= sys_up.y))
													&&	((lo.z + k > sys_lo.z) && (lo.z +k <= sys_up.z)) )
												||
													(		((up.x + i <= sys_up.x) && (up.x +i > sys_lo.x))
                          &&	((up.y + j <= sys_up.y) && (up.y +j > sys_lo.y))
													&&	((up.z + k <= sys_up.z) && (up.z +k > sys_lo.z)) ));

						// store clashing information in a bitvector for every unitcell 
						boundingbits.setBit((it - boundingboxes.begin()), bbit);
					}
					boundingbits_vector.push_back(boundingbits);
				}
			}
		}


		cout << ci_ptr_->getFrac2Cart() * sys_lo << endl;
		cout << ci_ptr_->getFrac2Cart() * sys_up << endl;
		
		transformer_.setTransformation(ci_ptr_->getFrac2Cart());
		system_->apply(transformer_);
		
		// generate surrounding unitcell, if there are clashing asyymetric units and delete delete non-clashing
		std::list<System*> crystal;	
		System* current_unitcell = 0;
		vector<BitVector>::iterator bit = boundingbits_vector.begin();
		for (Index a = -1; a <= 1; a++)
		{
			for (Index b = -1; b <= 1; b++)
			{
				for (Index c = -1; c <= 1; c++)
				{
					if (bit->isAnyBit(true))
					{
						current_unitcell = generateUnitCell(a,b,c);	
						MoleculeIterator mit = current_unitcell->beginMolecule();
						MoleculeIterator temp_mit;
						while ( mit != current_unitcell->endMolecule())
						{
							if (!(bit->getBit(mit->getProperty("ASU_IDX").getUnsignedInt())))
							{
								temp_mit = mit;
								++temp_mit;
								(*mit).destroy();
								mit = temp_mit;
							}
							else ++mit;
						}
						crystal.push_back(current_unitcell);
					}

					++bit;
				}
			}
		}
		return crystal;
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

		// set a name and specify the cell type of this system
		unitcell_->setName("UNITCELL (0|0|0)");
		unitcell_->setProperty("CELLTYPE", (unsigned int)2 );	

		Size symops_size = ci_ptr_->getNumberOfSymOps();

		cout << "symops_size " << symops_size << endl;
		//System* current_asu;
		Matrix4x4 transmatrix;
		for (Position i = 0; i < symops_size; i++)
		{
			transmatrix.setIdentity();
			cout << "symop " << i;
			System* current_asu = new System(*asu_);

			transmatrix *= ci_ptr_->getFrac2Cart();
			transmatrix *= ci_ptr_->getSymOp(i);
			transmatrix *= ci_ptr_->getCart2Frac();
			transformer_.setTransformation(transmatrix);
			current_asu->apply(transformer_);


			//	Postprocess Molecules in Unit Cell
			MoleculeIterator mit = current_asu->beginMolecule();
			for(; mit != current_asu->endMolecule(); ++mit)
			{
				// set cell type and asymmetric unit index for all top level atom
				// containers of the current asymmetric unit
				mit->setProperty("CELLTYPE", (unsigned int)1 );	
				mit->setProperty("ASU_IDX", (unsigned int)i );	

			}

			unitcell_->spliceAfter(*current_asu);
			cout << endl;
		}
		
		//correct positions of the asu's
		correctASUPositions_(unitcell_);
		
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
			// TODO add if for isIdentity
			if (!ci_ptr_->isgivenNCS(i))
			{
				current_ncs = new System(*system_);
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

	bool CrystalGenerator::correctASUPositions_(System* raw_cell)
	{
			//	Postprocess Molecules in Unit Cell
			Matrix4x4 corr_matrix;
			Vector3 center;
			Vector3 corr_trans;
			MoleculeIterator mit = raw_cell->beginMolecule();
			for(; mit != raw_cell->endMolecule(); ++mit)
			{
				//	Shift the molecules into unit cell if their geometric center is
				//		- necessary because symop transformations do not distinguish application
				//		order of  rotations and translations
				corr_matrix.setIdentity();
				corr_trans = Vector3(0,0,0);
				transformer_.setTransformation(ci_ptr_->getCart2Frac());
				mit->apply(transformer_);
				mit->apply(center_processor_);
				center = center_processor_.getCenter();
				for (int k=0; k<3; k++)
				{
					if (center[k] < 0) corr_trans[k] = 1;
					else if (center[k] >= 1) corr_trans[k] = -1;
				}

				corr_matrix.setTranslation(corr_trans);
				corr_matrix = ci_ptr_->getFrac2Cart() * corr_matrix;
				transformer_.setTransformation(corr_matrix);
				mit->apply(transformer_);

			}
			return true;
	}


}

// namespace BALL

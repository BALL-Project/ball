// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/SYSTEM/path.h>
#include <BALL/XRAY/crystalInfo.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/FORMAT/parameters.h>
#include <BALL/FORMAT/parameterSection.h>

using namespace std;

namespace BALL
{
	const string CrystalInfo::Option::SPACE_GROUP_FILE = "filename";
	const string CrystalInfo::Default::SPACE_GROUP_FILE = "XRAY/spacegroups-details.dat";

	CrystalInfo::CrystalInfo()
		:	options(),
			space_group_("P 1"),
			cell_dimensions_(1.0, 1.0, 1.0),
			alpha_(90, false),
			beta_(90, false),
			gamma_(90, false),
			z_score_(1),
			filename_(Default::SPACE_GROUP_FILE),
			cart2frac_(),
			frac2cart_(),
			ncs_symops_(),
			ncs_isgiven_(),
			sg_symops_()
	{
		calculateMatrices_();
		retrieveSymOps_(space_group_);
	}

	CrystalInfo::CrystalInfo(const CrystalInfo& ci)
		:	options(ci.options),
			space_group_(ci.space_group_),
			cell_dimensions_(ci.cell_dimensions_),
			alpha_(ci.alpha_),
			beta_(ci.beta_),
			gamma_(ci.gamma_),
			z_score_(ci.z_score_),
			filename_(ci.filename_),
			cart2frac_(),
			frac2cart_(),
			ncs_symops_(),
			ncs_isgiven_(),
			sg_symops_()
	{
		calculateMatrices_();
		retrieveSymOps_(space_group_);
	}
	
	CrystalInfo::CrystalInfo(String group, Vector3 dim, Angle alpha, Angle beta, Angle gamma)
		:	options(),
			space_group_(group),
			cell_dimensions_(dim.x, dim.y, dim.z),
			alpha_(alpha),
			beta_(beta),
			gamma_(gamma),
			filename_(Default::SPACE_GROUP_FILE),
			cart2frac_(),
			frac2cart_(),
			ncs_symops_(),
			ncs_isgiven_(),
			sg_symops_()
	{
		calculateMatrices_();
		retrieveSymOps_(space_group_);
	}
	
	CrystalInfo::~CrystalInfo()
	{
	}
	
	
	bool CrystalInfo::setSpaceGroup(const String& sg)
	{
		if (retrieveSymOps_(sg))
		{
			space_group_=sg;
			return true;
		}
		else return false;
		
	}

	
	const String& CrystalInfo::getSpaceGroup() const
	{
		return space_group_;
	}

	
	void CrystalInfo::setCellDimensions(const Vector3& dim)
	{
		cell_dimensions_.x = dim.x;
		cell_dimensions_.y = dim.y;
		cell_dimensions_.z = dim.z;
		calculateMatrices_();
	}

	
	void CrystalInfo::setCellEdgeLengthA(const float& a) 
	{
		cell_dimensions_.x = a;
		calculateMatrices_();
	}

	const float& CrystalInfo::getCellEdgeLengthA() const
	{
		return cell_dimensions_.x;
	}
	
	void CrystalInfo::setCellEdgeLengthB(const float& b) 
	{
		cell_dimensions_.y = b;
		calculateMatrices_();
	}
	
	
	const float& CrystalInfo::getCellEdgeLengthB() const
	{
		return cell_dimensions_.y;
	}
	
	void CrystalInfo::setCellEdgeLengthC(const float& c) 
	{
		cell_dimensions_.z = c;
		calculateMatrices_();
	}

	const float& CrystalInfo::getCellEdgeLengthC() const
	{
		return cell_dimensions_.z;
	}
	
	
	void CrystalInfo::setCellAngles(const Angle& alpha, const Angle& beta, const Angle& gamma)
	{
		alpha_ = alpha;
		beta_ = beta;
		gamma_ = gamma;
		calculateMatrices_();
	}

	
	void CrystalInfo::setCellAngleAlpha(const Angle& alpha)
	{
		alpha_ = alpha;
		calculateMatrices_();
	}

	const Angle& CrystalInfo::getCellAngleAlpha() const
	{
		return alpha_; 
	}
	
	void CrystalInfo::setCellAngleBeta(const Angle& beta)
	{
		beta_ = beta;
		calculateMatrices_();
	}
	
	const Angle& CrystalInfo::getCellAngleBeta() const
	{
		return beta_; 
	}
	
	void CrystalInfo::setCellAngleGamma(const Angle& gamma)
	{
		gamma_ = gamma;
		calculateMatrices_();
	}
	
	const Angle& CrystalInfo::getCellAngleGamma() const
	{
		return gamma_; 
	}
	
	Size CrystalInfo::getNumberOfSymOps() const
	{
		return (Size)sg_symops_.size();
	}
	
	void CrystalInfo::setZScore(const int& zscore)
	{
		z_score_ = zscore;	
	}
	
	const int& CrystalInfo::getZScore() const
	{
		return z_score_;	
	}
	
	const Matrix4x4& CrystalInfo::getSymOp(Position p) const
	{
		return sg_symops_[p];				
	}


	Size CrystalInfo::getNumberOfNCSSymOps() const 
	{
		return (Size)ncs_symops_.size();
	}

	const Matrix4x4& CrystalInfo::getNCS(Position p) const
	{
		if (p >= (ncs_symops_.size()))
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, p, ncs_symops_.size());
		}
		return ncs_symops_[p];				
	}
	
	Matrix4x4& CrystalInfo::getNCS(Position p)
	{
		if (p >= (ncs_symops_.size()))
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, p, ncs_symops_.size());
		}
		return ncs_symops_[p];				
	}
	
	bool CrystalInfo::isgivenNCS(Position p) const
	{
		if (p >= (ncs_symops_.size()))
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, p, ncs_symops_.size());
		}
		return ncs_isgiven_[p];				
	}
	
	bool CrystalInfo::insertNCS(Position p, Matrix4x4 ncsm, bool is_given)
	{
		vector<Matrix4x4>::iterator ita = ncs_symops_.begin();
		vector<bool>::iterator itb = ncs_isgiven_.begin();
		if (p > (ncs_symops_.size()))
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, p, ncs_symops_.size());
		}
		ncs_symops_.insert(ita+p, ncsm);
		ncs_isgiven_.insert(itb+p, is_given);
		return true;
	}
	
	//pushbackNCS	
	void CrystalInfo::pushbackNCS(Matrix4x4 ncsm, bool is_given)
	{
		ncs_symops_.push_back(ncsm);
		ncs_isgiven_.push_back(is_given);
	}
	
	bool CrystalInfo::eraseNCS(Position p)
	{
		vector<Matrix4x4>::iterator ita = ncs_symops_.begin();
		vector<bool>::iterator itb = ncs_isgiven_.begin();
		if (p > (ncs_symops_.size()))
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, p, ncs_symops_.size());
		}
		ncs_symops_.erase(ita+p);
		ncs_isgiven_.erase(itb+p);
		return true;
	}

	const Matrix4x4& CrystalInfo::getCart2Frac() const
	{
		return cart2frac_;
	}
	
	
	const Matrix4x4& CrystalInfo::getFrac2Cart() const
	{
		return frac2cart_;
	}

	void CrystalInfo::setDefaultOptions()
	{
		options.setDefault(CrystalInfo::Option::SPACE_GROUP_FILE,
												CrystalInfo::Default::SPACE_GROUP_FILE);
	}

	
	void CrystalInfo::calculateMatrices_()
	{
		float cos_alpha_star = ((cos(beta_.toRadian())*cos(gamma_.toRadian())- cos(alpha_.toRadian()))/( sin(beta_.toRadian()) *  sin(gamma_.toRadian())));
		float sin_alpha_star = (1.0 - (cos_alpha_star * cos_alpha_star));
		frac2cart_(0,0) = cell_dimensions_.x;
		frac2cart_(0,1) = cell_dimensions_.y*cos(gamma_.toRadian());
		frac2cart_(0,2) = cell_dimensions_.z*cos(beta_.toRadian());
		frac2cart_(1,0) = 0;
		frac2cart_(1,1) = cell_dimensions_.y*sin(gamma_.toRadian());
		frac2cart_(1,2) = (-cell_dimensions_.z*sin(beta_.toRadian())*cos_alpha_star);
		frac2cart_(2,0) = 0;
		frac2cart_(2,1) = 0;
		frac2cart_(2,2) = cell_dimensions_.z*sin(beta_.toRadian())*sin_alpha_star;
		frac2cart_(3,3) = 1;

		cart2frac_ = frac2cart_;
		cart2frac_.invert();
	}
	
	
	bool CrystalInfo::retrieveSymOps_(const String& sg)
	{
		Path path;
		String sg_filename(path.find(filename_));
		if (sg_filename == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename_);
		}
		String trimmed_sg = sg.trim();
		LineBasedFile* groupfile = new LineBasedFile(sg_filename);
		if (groupfile->search(trimmed_sg))
		{
			sg_symops_.clear();
			
			String curr_line = groupfile->getLine();
			Position curr_linenum = groupfile->getLineNumber();

			vector <String> jf_line; // A vector containing one line of the Jones Faithful notation
			curr_line.split(jf_line,"\t",0);
			Position num_symops = (jf_line.back()).toInt();

			Matrix4x4 sym_mtrx;

			for (Position j=1; j<=num_symops; j++)
			{
				sym_mtrx.clear();
				groupfile->gotoLine(curr_linenum + j);
				curr_line = groupfile->getLine();
				curr_line.trim("()");
				curr_line.split(jf_line,",",0);

				int comp, comp_add, sign, sign_add;
				float divid, divis;
				for (Position i=0; i < jf_line.size(); i++)
				{
					comp = 0;
					comp_add = 0;
					sign = 1;
					sign_add = 0;
					divid = 0;
					divis = 1;
					switch(jf_line[i].length())
					{
						case 1:
							comp = int(jf_line[i][0])-88;
							break;

						case 2:
							sign = -(int(jf_line[i][0])-44);
							comp = int(jf_line[i][1])-88;
							break;

						case 3:
							comp = int(jf_line[i][0])-88;
							sign_add = -(int(jf_line[i][1])-44);
							comp_add = int(jf_line[i][2])-88;
							break;

						case 5: 
							if (isdigit(jf_line[i][0]))
							{
								divid = float(jf_line[i][0])-48;
								divis = float(jf_line[i][2])-48;
								sign = -(int(jf_line[i][3])-44);
								comp = int(jf_line[i][4])-88;
							}
							else
							{
								comp = int(jf_line[i][0])-88;
								sign = -(int(jf_line[i][1])-44);
								divid = float(jf_line[i][2])-48;
								divis = float(jf_line[i][4])-48;
							}
							break;

						case 6:
							sign = -(int(jf_line[i][0])-44);
							comp = int(jf_line[i][1])-88;
							divid = float(jf_line[i][3])-48;
							divis = float(jf_line[i][5])-48;
							break;

						default:
							cout << "Wrong Notation" << endl;
					}
					sym_mtrx(i,comp) = sign;
					if (sign_add) sym_mtrx(i,comp_add) = sign_add;
					sym_mtrx(i,3) = divid / divis;
					sym_mtrx.setRow(3,Vector4(0,0,0,1));
				}
				//cout << sym_mtrx << endl;
				sg_symops_.push_back(sym_mtrx);
			}
			z_score_ = sg_symops_.size();
			groupfile->close();
			delete groupfile;
		}
		else
		{
			cout << "Space Group \"" << sg << "\" unknown"  << endl;
			groupfile->close();
			delete groupfile;

			return false;
		}
		return true;
	}

  void CrystalInfo::persistentWrite(PersistenceManager& pm, const char* name) const
  {
		pm.writeObjectHeader(this, name);

			pm.writePrimitive(space_group_, "space_group_");
			pm.writePrimitive(z_score_, "z_score_");
			pm.writePrimitive(filename_, "filename_");

			pm.writeStorableObject(cell_dimensions_, "cell_dimensions_");

			// Serialize the angles
			float alpha; bool alpha_flag = 0;
			alpha_.get(alpha, alpha_flag);
			pm.writePrimitive(alpha, "alpha_");
			pm.writePrimitive(alpha_flag, "alpha_flag_");

			float beta; bool beta_flag = 0;
			beta_.get(beta, beta_flag);
			pm.writePrimitive(beta, "beta_");
			pm.writePrimitive(beta_flag, "beta_flag_");

			float gamma; bool gamma_flag = 0;
			gamma_.get(gamma, gamma_flag);
			pm.writePrimitive(gamma, "gamma_");
			pm.writePrimitive(gamma_flag, "gamma_flag_");

			//Serialize the conversion matrix
			String primitive_identifier = "";
			for (Position i = 1; i<=4; i++)
			{
				for (Position j = 1; j<=4; j++)
				{
					primitive_identifier = "frac2cart_.m" + String(i) + String(j);
					pm.writePrimitive(cart2frac_(i-1,j-1), primitive_identifier.c_str());
				}
			}

			// Serialize the NCS SymOps as Matrix4x4, followed by the isGiven bit
			pm.writePrimitive(getNumberOfNCSSymOps(), "ncs_symops_.size");

			for (Position k = 1; k <= getNumberOfNCSSymOps(); k++)
			{
				for (Position i = 1; i <= 4; i++)
				{
					for (Position j = 1; j <= 4; j++)
					{
						primitive_identifier = "ncs_symops_(" + String(k) + ").m" + String(i) + String(j);
						pm.writePrimitive(getNCS(k-1)(i-1,j-1), primitive_identifier.c_str());
					}
				}

				primitive_identifier = "ncs_isgiven_(" + String(k) + ")";
				pm.writePrimitive(isgivenNCS(k-1), primitive_identifier.c_str());

			}

			// Serialize the Space Group SymOps as Matrix4x4
			//pm.writePrimitive(getNumberOfSymOps(), "sg_symops_.size");
			//
			//for (Position k = 1; k <= getNumberOfSymOps(); k++)
			//{
			//	for (Position i = 1; i <= 4; i++)
			//	{
			//		for (Position j = 1; j <= 4; j++)
			//		{
			//			primitive_identifier = "sg_symops_(" + String(k) + ").m" + String(i) + String(j);
			//			pm.writePrimitive(getSymOp(k-1)(i-1,j-1), primitive_identifier.c_str());
			//		}
			//	}
			//}

		pm.writeObjectTrailer(name);
	}
  
  void CrystalInfo::persistentRead(PersistenceManager& pm)
  {
		String space_group = "";
		pm.readPrimitive(space_group, "space_group_");
		if (!setSpaceGroup(space_group))
		{
			cout << "Could not read space group from project file, setting space group to P 1" << endl;
			setSpaceGroup("P 1");
		}

		pm.readPrimitive(z_score_, "z_score_");
		pm.readPrimitive(filename_, "filename_");

		pm.readStorableObject(cell_dimensions_, "cell_dimensions_");

		// Deserialize the angles
		float alpha; bool alpha_flag = 0;
		pm.readPrimitive(alpha, "alpha_");
		pm.readPrimitive(alpha_flag, "alpha_flag_");
		alpha_.set(alpha, alpha_flag);

		float beta; bool beta_flag = 0;
		pm.readPrimitive(beta, "beta_");
		pm.readPrimitive(beta_flag, "beta_flag_");
		beta_.set(beta, beta_flag);

		float gamma; bool gamma_flag = 0;
		pm.readPrimitive(gamma, "gamma_");
		pm.readPrimitive(gamma_flag, "gamma_flag_");
		gamma_.set(gamma, gamma_flag);

		//Deserialize the conversion matrix
		String primitive_identifier = "";
		cart2frac_ = frac2cart_;
		cart2frac_.invert();
		for (Position i = 1; i<=4; i++)
		{
			for (Position j = 1; j<=4; j++)
			{
				primitive_identifier = "frac2cart_.m" + String(i) + String(j);
				pm.readPrimitive(frac2cart_(i-1,j-1), primitive_identifier.c_str());
			}
		}
		cart2frac_ = frac2cart_;
		cart2frac_.invert();

		// Deserialize the NCS SymOps as Matrix4x4, followed by the isGiven bit
		Position ncs_symop_size;
		pm.readPrimitive(ncs_symop_size, "ncs_symops_.size");

		for (Position k = 1; k <= ncs_symop_size; k++)
		{
			Matrix4x4 temp_ncs = Matrix4x4::getZero();
			bool temp_isgiven = false;

			for (Position i = 1; i <= 4; i++)
			{
				for (Position j = 1; j <= 4; j++)
				{
					primitive_identifier = "ncs_symops_(" + String(k) + ").m" + String(i) + String(j);
					pm.readPrimitive(temp_ncs(i-1,j-1), primitive_identifier.c_str());
				}
			}

			primitive_identifier = "ncs_isgiven_(" + String(k) + ")";
			pm.readPrimitive(temp_isgiven, primitive_identifier.c_str());
			pushbackNCS(temp_ncs, temp_isgiven);

		}

	}
}

// namespace BALL

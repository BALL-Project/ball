// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:

#include <BALL/SYSTEM/path.h>
#include <BALL/XRAY/crystalInfo.h>
#include <BALL/FORMAT/lineBasedFile.h>

using namespace std;

namespace BALL
{
	const char* CrystalInfo::Option::SPACE_GROUP_FILE = "filename";
	const char* CrystalInfo::Default::SPACE_GROUP_FILE = "XRAY/spacegroups.dat";

	CrystalInfo::CrystalInfo()
		:	space_group_("P 1"),
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

	CrystalInfo::CrystalInfo(String group, Vector3 dim, Angle alpha, Angle beta, Angle gamma)
		:	space_group_(group),
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
	
	CrystalInfo::~CrystalInfo() throw()
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
		throw(Exception::IndexOverflow)
	{
		if (p >= (ncs_symops_.size()))
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, p, ncs_symops_.size());
		}
		return ncs_symops_[p];				
	}
	
	Matrix4x4& CrystalInfo::getNCS(Position p)
		throw(Exception::IndexOverflow)
	{
		if (p >= (ncs_symops_.size()))
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, p, ncs_symops_.size());
		}
		return ncs_symops_[p];				
	}
	
	const bool CrystalInfo::isgivenNCS(Position p) const
		throw(Exception::IndexOverflow)
	{
		if (p >= (ncs_symops_.size()))
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, p, ncs_symops_.size());
		}
		return ncs_isgiven_[p];				
	}
	
	bool CrystalInfo::isgivenNCS(Position p)
		throw(Exception::IndexOverflow)
	{
		if (p >= (ncs_symops_.size()))
		{
			throw Exception::IndexOverflow(__FILE__, __LINE__, p, ncs_symops_.size());
		}
		return ncs_isgiven_[p];				
	}
	
	bool CrystalInfo::insertNCS(Position p, Matrix4x4 ncsm, bool is_given)
		throw(Exception::IndexOverflow)
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
		throw(Exception::IndexOverflow)
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
		//cout << "M" << frac2cart_ << endl;
		//cout << "I" << cart2frac_ << endl;
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
			int curr_linenum = groupfile->getLineNumber();
			
			vector <String> tmp;
			curr_line.split(tmp,"\t",0);
			unsigned int num_symops = (tmp.back()).toInt();
			
			Matrix4x4 sym_mtrx;
			
			for (unsigned int j=1; j<=num_symops; j++)
			{
				sym_mtrx.clear();
				groupfile->gotoLine(curr_linenum + j);
				curr_line = groupfile->getLine();
				curr_line.trim("()");
				curr_line.split(tmp,",",0);
				//cout << curr_line << endl;
				int comp, comp_add, sign, sign_add;
				float divid, divis;
				for (unsigned int i=0; i < tmp.size(); i++)
				{
					comp = 0;
					comp_add = 0;
					sign = 1;
					sign_add = 0;
					divid = 0;
					divis = 1;
					switch(tmp[i].length())
					{
						case 1:
							comp = int(tmp[i][0])-88;
							break;

						case 2:
							sign = -(int(tmp[i][0])-44);
							comp = int(tmp[i][1])-88;
							break;

						case 3:
							comp = int(tmp[i][0])-88;
							sign_add = -(int(tmp[i][1])-44);
							comp_add = int(tmp[i][2])-88;
							break;

						case 5: 
							if (isdigit(tmp[i][0]))
							{
								divid = float(tmp[i][0])-48;
								divis = float(tmp[i][2])-48;
								sign = -(int(tmp[i][3])-44);
								comp = int(tmp[i][4])-88;
							}
							else
							{
								comp = int(tmp[i][0])-88;
								sign = -(int(tmp[i][1])-44);
								divid = float(tmp[i][2])-48;
								divis = float(tmp[i][4])-48;
							}
							break;

						case 6:
							sign = -(int(tmp[i][0])-44);
							comp = int(tmp[i][1])-88;
							divid = float(tmp[i][3])-48;
							divis = float(tmp[i][5])-48;
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
			groupfile->close();
		}
		else
		{
			cout << "Space Group \"" << sg << "\" unknown"  << endl;
			groupfile->close();
			return false;
		}
		return true;
	}

}

// namespace BALL

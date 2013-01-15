// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/DOCKING/COMMON/result.h>

namespace BALL
{
	Result::Method Result::getMethod(int i)
	{
		switch(i)
		{
			case 0:
				return Result::RECEPTORIMPORT;
			case 1:
				return Result::LIGANDIMPORT;
			case 2:
				return Result::DOCKING;
			case 3:
				return Result::RESCORING;
			case 4:
				return Result::MDRESCORING;
			case 5:
				return Result::CONSENSUSRESCORING;
			case 6:
				return Result::MOLECULE_CHECK;
		}
		return Result::UNKNOWN;
	}

	Result* Result::makeLigandImportResult()
	{
		Result* res = new Result();
		res->setMethod(LIGANDIMPORT);
		return res;
	}

	Result* Result::makeReceptorImportResult()
	{
		Result* res = new Result();
		res->setMethod(RECEPTORIMPORT);
		return res;
	}

	Result* Result::makeDockingResult()
	{
		Result* res = new Result();
		res->setMethod(DOCKING);
		return res;
	}

	Result* Result::makeRescoringResult()
	{
		Result* res = new Result();
		res->setMethod(RESCORING);
		return res;
	}


	Result::Result()
	{
		description_ = "";
		timestamp_ = "";
		toolinfo_ = "";
	}

	Result::Result(const Result::Method& _method)
	{
		method = _method;
		description_ = "";
		timestamp_ = "";
		toolinfo_ = "";
	}

	Result::Result(const Result& res)
	{
		operator = (res);
	}

	Result::Result(const Result& res, const HashSet<String>& IDs)
	{
		method = res.method;
		result_data_ = res.result_data_;
		description_ = res.description_;

		for (HashMap < String, vector < ResultData > > ::const_iterator it = res.result_data_.begin(); it != res.result_data_.end(); it++)
		{
			if (IDs.has(it->first)) result_data_.insert(*it);
		}

		for (HashMap < String, ResultData > ::const_iterator it = res.result_output_data_.begin(); it != res.result_output_data_.end(); it++)
		{
			if (IDs.has(it->first)) result_output_data_.insert(*it);
		}

		for (vector < String > ::const_iterator it = res.input_conformations_.begin(); it != res.input_conformations_.end(); it++)
		{
			if (IDs.has(*it)) input_conformations_.push_back(*it);
		}

		for (HashSet < String > ::const_iterator it = res.input_conformations_map_.begin(); it != res.input_conformations_map_.end(); it++)
		{
			if (IDs.has(*it)) input_conformations_map_.insert(*it);
		}
	}

	void Result::clear()
	{
		result_data_.clear();
		result_output_data_.clear();
		input_conformations_.clear();
		input_conformations_map_.clear();
	}

	void Result::operator = (const Result& res)
	{
		method = res.method;
		description_ = res.description_;
		result_data_ = res.result_data_;
		result_output_data_ = res.result_output_data_;
		input_conformations_ = res.input_conformations_;
		input_conformations_map_ = res.input_conformations_map_;
	}

	void Result::operator += (const Result& res)
	{
		if (method != res.method)
		{
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Result::operator += ", "Results can only be added to another Result of the same type!");
		}
		for (HashMap < String, vector < ResultData > > ::const_iterator it = res.result_data_.begin(); it != res.result_data_.end(); it++)
		{
			result_data_.insert(*it);
		}
		for (HashMap < String, ResultData > ::const_iterator it = res.result_output_data_.begin(); it != res.result_output_data_.end(); it++)
		{
			result_output_data_.insert(*it);
		}
		for (vector < String > ::const_iterator it = res.input_conformations_.begin(); it != res.input_conformations_.end(); it++)
		{
			input_conformations_.push_back(*it);
		}
		for (HashSet < String > ::const_iterator it = res.input_conformations_map_.begin(); it != res.input_conformations_map_.end(); it++)
		{
			input_conformations_map_.insert(*it);
		}
	}

	Result::~Result()
	{
	}

	Result::Method Result::getMethod()
	{
			return method;
	}

	String Result::getMethodString()
	{
		switch(method)
		{
			case 0:
				return "RECEPTORIMPORT";
			case 1:
				return "LIGANDIMPORT";
			case 2:
				return "DOCKING";
			case 3:
				return "RESCORING";
			case 4:
				return "MDRESCORING";
			case 5:
				return "CONSENSUSCORING";
			case 6:
				return "MOLECULE_CHECK";
			default:
				return "N/A";
		}
	}

	void Result::setMethod(Result::Method _method)
	{
		method = _method;
	}

	void Result::setTimestamp(const String& timestamp)
	{
		timestamp_ = timestamp;
	}

	const String& Result::getTimestamp()
	{
		return timestamp_;
	}

	void Result::setToolInfo(const String& toolinfo)
	{
		toolinfo_ = toolinfo;
	}

	const String& Result::getToolInfo()
	{
		return toolinfo_;
	}

	void Result::setDescription(const String& text)
	{
		description_ = text;
	}

	const String& Result::getDescription()
	{
		return description_;
	}

	const vector < String > * Result::getInputConformations()
	{
		return &input_conformations_;
	}

	Conformation* Result::getInputConformer(Ligand* lig)
			throw(Exception::GeneralException)
	{
		for (Position i = 0; i < lig->getNumberOfConformations(); i++)
		{
			Conformation* conf = lig->getConformation(i);
			String confid = conf->getId();
			if (input_conformations_map_.has(confid))
			{
				return conf;
			}
		}
		throw(Exception::GeneralException(__FILE__, __LINE__, "Result", "no input conformation found for ligand"));
	}

	Conformation* Result::getFirstOutputConformation(Ligand* lig)
			throw(Exception::GeneralException)
	{
		for (Position i = 0; i < lig->getNumberOfConformations(); i++)
		{
			Conformation* conf = lig->getConformation(i);
			String confid = conf->getId();
			if (hasOutputData(confid))
			{
				return conf;
			}
		}
		throw(Exception::GeneralException(__FILE__, __LINE__, "Result", "no first output conformation found for ligand"));
	}

	vector < Result::ResultData > Result::getOutputConformations(Ligand* lig)
	{
		vector<ResultData> ret;
		for (Position i = 0; i < lig->getNumberOfConformations(); i++)
		{
			Conformation* conf = lig->getConformation(i);
			String confid = conf->getId();
			if (hasOutputData(confid))
			{
				Result::ResultData rd = getOutputData(confid);
				rd.setConformation(conf);
				ret.push_back(rd);
			}
		}
		return ret;
	}

	const HashSet<String>* Result::getInputIds()
	{
		return &input_conformations_map_;
	}

	HashSet<String> Result::getOutputIds()
	{
		HashSet<String> ret;
		HashMap<String, vector<ResultData> >::Iterator iter = result_data_.begin();
		for (; iter != result_data_.end(); iter++)
		{
			vector<ResultData>::iterator iter2 = (iter->second).begin();
			for (; iter2 != (iter->second).end(); iter2++)
			{
				ResultData rd = *iter2;
				ret.insert(rd.getLigandConformationId());
			}
		}
		return ret;
	}

	void Result::add(Conformation* lig_conf_in, Conformation* lig_conf_out, int scoringmethod, double energy, Conformation* rec_conf)
	{
		ResultData rd;
		rd.setEnergy(energy);
		rd.setScoringMethod(scoringmethod);
		rd.setLigandConformation(lig_conf_out);
		rd.setReceptorConformation(rec_conf);
		add(lig_conf_in, rd);
	}

	void Result::add(Conformation* lig_conf_in, ResultData &rd)
	{
		add(lig_conf_in->getId(), rd);
	}

	void Result::add(const String& lig_conf_id_in, const String& lig_conf_id_out, int scoringmethod, double energy, const String& rec_conf_id)
	{
		ResultData rd;
		rd.setEnergy(energy);
		rd.setScoringMethod(scoringmethod);
		rd.setLigandConformationId(lig_conf_id_out);
		rd.setReceptorConformationId(rec_conf_id);
		add(lig_conf_id_in, rd);
	}

	void Result::add(String lig_conf_id_in, ResultData &rd)
	{
		if ( input_conformations_map_.has(lig_conf_id_in)
			&& result_output_data_.has(rd.getLigandConformationId()) )
		{
			//cout<<"[Warning:] Result object already has data for given pair of input- and output-conformation! Thus, no new data is added!"<<endl;
			return;
		}

		if (!result_data_.has(lig_conf_id_in))
		{
			input_conformations_.push_back(lig_conf_id_in);
			input_conformations_map_.insert(lig_conf_id_in);
			result_data_[lig_conf_id_in] = vector<ResultData>();
		}
		result_data_[lig_conf_id_in].push_back(rd);
		result_output_data_[rd.getLigandConformationId()] = rd;
	}


	void Result::erase(const FlexibleMolecule* flexmol)
	{
		const vector < Conformation* > & confs = flexmol->getConformations();
		HashSet<String> conf_set;
		for (Size i = 0; i < confs.size(); i++)
		{
			String ID = confs[i]->getId();
			conf_set.insert(ID);
			result_data_.erase(ID);
			result_output_data_.erase(ID);
			input_conformations_map_.erase(ID);
		}
		for (vector < String > ::iterator it = input_conformations_.begin(); it != input_conformations_.end(); )
		{
			if (conf_set.has(*it))
			{
				it = input_conformations_.erase(it);
			}
			else it++;
		}
	}

	void Result::erase(const String& ID)
	{
		result_data_.erase(ID);
		result_output_data_.erase(ID);
		input_conformations_map_.erase(ID);

		for (vector < String > ::iterator it = input_conformations_.begin(); it != input_conformations_.end(); )
		{
			if (*it == ID)
			{
				it = input_conformations_.erase(it);
				break;
			}
			else it++;
		}
	}


	void Result::sort(const list<String>& input_order, list<String>& output_order)
	{
		input_conformations_.clear();
		output_order.clear();
		for (list < String > ::const_iterator it = input_order.begin(); it != input_order.end(); it++)
		{
			HashMap<String, vector<ResultData> >::iterator it2 = result_data_.find(*it);
			if (it2 != result_data_.end())
			{
				input_conformations_.push_back(*it);
				for (Size i = 0; i < it2->second.size(); i++)
				{
					output_order.push_back(it2->second[i].getLigandConformationId());
				}
			}
		}
	}


	bool Result::hasOutputData(String id)
	{
		return (result_output_data_.has(id));
	}

	Result::ResultData Result::getOutputData(String id)
	{
		if (result_output_data_.has(id))
		{
			return result_output_data_[id];
		}
		else
		{
			//throw(Exception::IndexOverflow(__FILE__, __LINE__, id, 1));
			throw(Exception::InvalidArgument(__FILE__, __LINE__, "supplied id not found"));
		}
	}

	const vector<Result::ResultData>* Result::get(String inpose_id)
	{
		return &result_data_[inpose_id];
	}

	String Result::toString(const ResultData &rd)
	{
		String ret="";
		ret += " method: "+ String(rd.getScoringMethod());
		ret += " energy: "+ String(rd.getEnergy());
		ret += " lig_id: "+ rd.getLigandConformationId();
		ret += " rec_id: "+ rd.getReceptorConformationId();
		ret += " ";
		return ret;
	}

	Result::ResultData::ResultData()
		: scoringmethod(UNKNOWN)
	{
		has_lig_id  = false;
		has_rec_id  = false;
		has_energy  = false;
		has_method  = false;
		resulttype  = -1;
	}

	Result::ResultData::ResultData(int result_type)
		: scoringmethod(UNKNOWN)
	{
		has_lig_id  = false;
		has_rec_id  = false;
		has_energy  = false;
		has_method  = false;
		resulttype  = result_type;
	}

	void Result::ResultData::setReceptorConformationId(String id)
	{
		rec_conf_id = id;
		has_rec_id  = true;
	}

	void Result::ResultData::setReceptorConformation(Conformation* conf)
	{
		rec_conf_id = conf->getId();
		has_rec_id  = true;
	}

	String  Result::ResultData::getReceptorConformationId() const
	{
		return rec_conf_id;
	}

	void Result::ResultData::setLigandConformationId(String id)
	{
		has_lig_id  = true;
		lig_conf_id = id;
	}

	void Result::ResultData::setLigandConformation(Conformation* conf)
	{
		has_lig_id  = true;
		lig_conf_id = conf->getId();
	}

	String  Result::ResultData::getLigandConformationId() const
	{
		return lig_conf_id;
	}

	void Result::ResultData::setScoringMethod(int _method)
	{
		scoringmethod = _method;
		has_method = true;
	}

	int  Result::ResultData::getScoringMethod() const
	{
		return scoringmethod;
	}

	void Result::ResultData::setEnergy(double e)
	{
		energy = e;
		has_energy = true;
	}

	double Result::ResultData::getEnergy() const
	{
		return energy;
	}

	bool Result::ResultData::hasReceptorConformationId() const
	{
		if (scoringmethod == LIGANDIMPORT || scoringmethod == RECEPTORIMPORT)
		{
			return false;
		}
		return has_rec_id;
	}

	bool Result::ResultData::hasLigandConformationId() const
	{
		return has_lig_id;
	}

	bool Result::ResultData::hasEnergy() const
	{
		if (scoringmethod == LIGANDIMPORT || scoringmethod == RECEPTORIMPORT)
		{
			return false;
		}
		return has_energy;
	}

	bool Result::ResultData::hasScoringMethod() const
	{
		if (scoringmethod == LIGANDIMPORT || scoringmethod == RECEPTORIMPORT)
		{
			return false;
		}
		return has_method;
	}

	bool Result::ResultData::check() const
	{
		switch(resulttype)
		{
			case Result::DOCKING:
				if (has_energy && has_lig_id && has_rec_id && has_method)
				{
					return true;
				}
				return false;
				break;
			case Result::LIGANDIMPORT:
				if (has_lig_id)
					return true;
				return false;
				break;
			default:
				return true;
		}
	}

	void Result::ResultData::setConformation(Conformation* conf)
	{
		conformation = conf;
	}

	Conformation* Result::ResultData::getConformation()
	{
		return conformation;
	}

	const HashMap<String, vector<Result::ResultData> >* Result::getData()
	{
		return &result_data_;
	}
}

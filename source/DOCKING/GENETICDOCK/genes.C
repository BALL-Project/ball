// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Jan Fuhrmann, Marcel Schumann $
// ----------------------------------------------------

#include <BALL/DOCKING/GENETICDOCK/genes.h>
#include <iostream>

namespace BALL
{
	GenericGene::GenericGene()
		: rng_(),
		  dist_(0., 1.)
	{
		std::random_device rd;
		rng_.seed(rd());
	}

	double GenericGene::randomGeneValue()
	{
		return dist_(rng_);
	}

	int GenericGene::randomGenePosition(int from, int to)
	{
		std::uniform_int_distribution<int> dist(from, to);
		return dist(rng_);
	}

	DoubleGene::DoubleGene()
		: GenericGene()
	{

	}

	DoubleGene::~DoubleGene()
	{

	}

	DoubleGene::DoubleGene(Size i)
	{
		/** new values vector of size i
		 */
		values_ = vector<double>(i);

		/** randomize values
		 */
		randomize();
	}

	void DoubleGene::randomize()
	{
		/** assign random number to each double variable
		 */
		for (Size x = 0; x < values_.size(); ++x)
			values_[x] = randomGeneValue();
	}

	GenericGene *DoubleGene::mate(GenericGene *gg)
	{
		/** dynamic cast tests mating partners type
		 */
		DoubleGene *partner = dynamic_cast<DoubleGene *>(gg);

		/** partner not a double gene
		 */
		if (partner == 0)
		{
			std::cerr << "gene mismatch while mating" << std::endl;
			exit(1);
		}

		/** test if parents share the same number of variables
		 */
		if (values_.size() != partner->values_.size())
		{
			std::cerr << "vector mismatch while mating double gene" << std::endl;
			exit(1);
		}

		/** create offspring gene
		 */
		DoubleGene *offspring = new DoubleGene;

		/** offspring with same number of variables as parents
		 */
		offspring->values_.resize(values_.size());

		for (Size x = 0; x < values_.size(); x++)
		{
			double v1 = values_[x];
			double v2 = partner->values_[x];

			/** v1 always smaller than v2
			 */
			if (v1 > v2)
			{
				double c = v1;
				v1 = v2;
				v2 = c;
			}

			/** distance is never larger than 0.5
			 */
			if (v2 - v1 > 0.5)
			{
				v1 += 1;
				double c = v1;
				v1 = v2;
				v2 = c;
			}

			// caluclate distance
			double d = v2 - v1;

			/** blend two values with respect to a random number
			 */
			offspring->values_[x] = v1 - 0.5 * d + 2 * d * randomGeneValue();

			if (offspring->values_[x] < 0)
				offspring->values_[x] += 1;

			if (offspring->values_[x] > 1)
				offspring->values_[x] -= 1;
		}

		return offspring;
	}


	void DoubleGene::mutate()
	{
		/** set random variable to random number
		 */
		values_[randomGenePosition(0, values_.size() - 1)] = randomGeneValue();
	}


	vector<double> DoubleGene::getValues()
	{
		return values_;
	}


	void DoubleGene::setValue(vector<double> v)
	{
		values_ = v;
	}

	GenericGene *DoubleGene::clone()
	{
		DoubleGene *g = new DoubleGene;
		g->values_ = values_;
		return g;
	}

	QuaternionGene::QuaternionGene()
		: GenericGene()
	{
		randomize();
	}

	void QuaternionGene::randomize()
	{
		/** evenly distributed random orientation of ligand
		 */
		double pi = 3.14159;

		double x = randomGeneValue();

		double o = sqrt(1 - x);
		double b = sqrt(x);
		double c = 2 * pi * randomGeneValue();
		double d = 2 * pi * randomGeneValue();

		Vector3 axis(cos(d) * b, sin(c) * o, cos(c) * o);
		double angle = sin(d) * b;
		quat_.fromAxisAngle(axis, angle);
	}

	QuaternionGene::~QuaternionGene()
	{

	}

	GenericGene *QuaternionGene::mate(GenericGene *gg)
	{
		/** linear interpolation of parent values
		 */
		if (QuaternionGene * qg = dynamic_cast < QuaternionGene * > (gg))
		{
			if ((quat_.i() * qg->quat_.i() + quat_.j() * qg->quat_.j() + quat_.k() * qg->quat_.k() +
				 quat_.getAngle() * qg->quat_.getAngle()) < 0)
			{
				Vector3 axis(-qg->quat_.i(), -qg->quat_.j(), -qg->quat_.k());
				double angle = -qg->quat_.getAngle();
				quat_.fromAxisAngle(axis, angle);
			}

			/** create offspring gene
			 */
			QuaternionGene *result = new QuaternionGene;

			double i = fabs(qg->quat_.i() - quat_.i());
			double j = fabs(qg->quat_.j() - quat_.j());
			double k = fabs(qg->quat_.k() - quat_.k());
			double angle = fabs(qg->quat_.getAngle() - quat_.getAngle());

			i = quat_.i() - 0.5 * i + 2 * i * randomGeneValue();
			j = quat_.j() - 0.5 * j + 2 * j * randomGeneValue();
			k = quat_.k() - 0.5 * k + 2 * k * randomGeneValue();
			angle = quat_.getAngle() - 0.5 * angle + 2 * angle * randomGeneValue();

			double t = sqrt(i * i + j * j + k * k + angle * angle);

			/** normalize quaternion
			 */
			i /= t;
			j /= t;
			k /= t;
			angle /= t;
			Vector3 axis(i, j, k);
			quat_.fromAxisAngle(axis, angle);

			return result;

		} else
		{
			std::cerr << "mating failed" << std::endl;
			exit(1);
		}
	}

	void QuaternionGene::mutate()
	{
		/** mutation not implemented
		 */
	}

	Quaternion QuaternionGene::getValue()
	{
		return quat_;
	}

	GenericGene *QuaternionGene::clone()
	{
		QuaternionGene *g = new QuaternionGene;
		g->quat_ = quat_;
		return g;
	}


}

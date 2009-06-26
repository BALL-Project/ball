// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2 sw=2
//
// $Id: randomNumberGenerator.C,v 1.1 2005/11/25 14:20:55 anker Exp $
//

#include <BALL/MATHS/randomNumberGenerator.h>
#include <BALL/common.h>

namespace BALL
{

	RandomNumberGenerator::RandomNumberGenerator()	
	: valid_(false)
	{
		setup();
	}

	RandomNumberGenerator::RandomNumberGenerator(const RandomNumberGenerator&
	rng)
		:	u_(rng.u_),
			c_(rng.c_),
			cd_(rng.cd_),
			cm_(rng.cm_),
			i97_(rng.i97_),
			j97_(rng.j97_),
			valid_(rng.valid_)
	{
	}

	RandomNumberGenerator::~RandomNumberGenerator()
	{
	}

	void RandomNumberGenerator::setup(int ij, int kl)
	{
		double s,t;
		int ii,i,j,k,l,jj,m;

		// allocate space for u_
		u_.resize(97);

		/*
			 Handle the seed range errors
			 First random number seed must be between 0 and 31328
			 Second seed must have a value between 0 and 30081
		 */
		if (ij < 0 || ij > 31328 || kl < 0 || kl > 30081)
		{
			Log.warn() << "Warning: Random number generator initialized with too large (ij > 31328 || kl > 30081) or negative values!\n" <<
			              "         Using default values instead."<< std::endl;
			ij = 1802;
			kl = 9373;
		}

		i = (ij / 177) % 177 + 2;
		j = (ij % 177)       + 2;
		k = (kl / 169) % 178 + 1;
		l = (kl % 169);

		for (ii=0; ii<97; ii++) 
		{
			s = 0.0;
			t = 0.5;
			for (jj=0; jj<24; jj++) 
			{
				m = (((i * j) % 179) * k) % 179;
				i = j;
				j = k;
				k = m;
				l = (53 * l + 1) % 169;
				if (((l * m % 64)) >= 32)
					s += t;
				t *= 0.5;
			}
			u_[ii] = s;
		}

		c_    = 362436.0 / 16777216.0;
		cd_   = 7654321.0 / 16777216.0;
		cm_   = 16777213.0 / 16777216.0;
		i97_  = 97;
		j97_  = 33;
		valid_ = true;
	}

	double RandomNumberGenerator::randomUniform()
	{
		if (!isValid())
		{
			Log.warn() << "RandomNumberGenerator::randomUniform(): "
				<< "instance not valid, returning 0" << std::endl;
			return(0.0);
		}

		double uni;

		uni = u_[i97_-1] - u_[j97_-1];
		if (uni <= 0.0) uni++;

		u_[i97_-1] = uni;
		i97_--;

		if (i97_ == 0) i97_ = 97;
		j97_--;

		if (j97_ == 0) j97_ = 97;
		c_ -= cd_;

		if (c_ < 0.0) c_ += cm_;
		uni -= c_;

		if (uni < 0.0) uni++;

		return(uni);

	}

	double RandomNumberGenerator::randomGaussian(double mean, double stddev)
	{
		double q,u,v,x,y;

		/*  
				Generate P = (u,v) uniform in rect. enclosing acceptance region 
				Make sure that any random numbers <= 0 are rejected, since
				gaussian() requires uniforms > 0, but RandomUniform() delivers >= 0.
		 */
		do {
			u = randomUniform();
			v = randomUniform();
			if (u <= 0.0 || v <= 0.0) 
			{
				u = 1.0;
				v = 1.0;
			}
			v = 1.7156 * (v - 0.5);

			/*  Evaluate the quadratic form */
			x = u - 0.449871;
			y = fabs(v) + 0.386595;
			q = x * x + y * (0.19600 * y - 0.25472 * x);

			/* Accept P if inside inner ellipse */
			if (q < 0.27597) break;

			/*  Reject P if outside outer ellipse, or outside acceptance region */
		} while ((q > 0.27846) || (v * v > -4.0 * log(u) * u * u));

		/*  Return ratio of P's coordinates as the normal deviate */
		return (mean + stddev * v / u);
	}

	int RandomNumberGenerator::randomInteger(int lower, int upper)
	{
		 return((int)(randomUniform() * (upper - lower + 1)) + lower);
	}

	double RandomNumberGenerator::randomDouble(double lower, double upper)	
	{
		 return((upper - lower) * randomUniform() + lower);
	}
}


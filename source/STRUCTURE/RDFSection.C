// $Id: RDFSection.C,v 1.7 2000/10/30 00:19:58 amoll Exp $

#include <BALL/STRUCTURE/RDFSection.h>
#include <BALL/FORMAT/parameters.h>

using namespace std;

namespace BALL
{

	RDFSection::RDFSection() throw()
		:	ParameterSection(),
		 	rdf_()
	{
	}


	RDFSection::RDFSection(const RDFSection& rdf_section) throw()
		:	ParameterSection(),
			rdf_(rdf_section.rdf_)
	{
	}


	RDFSection::~RDFSection() throw()
	{
		clear();
		valid_ = false;
	}


	void RDFSection::clear() throw()
	{
		rdf_.clear();

		ParameterSection::clear();
	}


	const RDFSection& RDFSection::operator = (const RDFSection& rdf_section)
		throw()
	{
		ParameterSection::operator = (rdf_section);
		rdf_ = rdf_section.rdf_;

		return *this;
	}


	const RadialDistributionFunction& RDFSection::getRDF() const throw()
	{
		return rdf_;
	}

	
	bool RDFSection::operator == (const RDFSection& section) const throw()
	{
		return (ParameterSection::operator == (section)
						&& (rdf_ == section.rdf_));
	}


	bool RDFSection::extractSection(Parameters& parameters,
			const String& section_name) throw()
	{
		if (!parameters.isValid())
		{
			return false;
		}

		int type;

		// extract the basis information
		ParameterSection::extractSection(parameters, section_name);

		if (options.has("type"))
		{
			if (options.get("type") == "piecewise_polynomial")
			{
				type = PIECEWISE_POLYNOMIAL;
			}
			else
			{
				Log.error() << "RDFSection::extractSection(): "
					<< "Unknown type.";
				return false;
			}
		}
		else
		{
			Log.warn() << "RDFSection::extractSection(): "
				<< "no type given, assuming piecewise_polynomial." << endl;
			type = PIECEWISE_POLYNOMIAL;
		}

		PiecewisePolynomial poly;
		Interval interval;
		std::vector<Interval> intervals;
		Size number_of_intervals;
		Size degree;
		Coefficients coeffs;
		std::vector<Coefficients> coefficients;
		String upper_limit;

		switch(type) 
		{

			case PIECEWISE_POLYNOMIAL:
			
				if (options.has("degree"))
				{
					degree = options.get("degree").toInt();
				}
				else
				{
					Log.warn() << "RDFSection::extractSection(): "
										 << "No degree given, assuming 4." << endl;
					degree = 4;
				}
				coeffs.resize(degree);

				number_of_intervals = getNumberOfKeys();
				intervals.resize(number_of_intervals);
				coefficients.resize(number_of_intervals);

				for (Size i = 0; i < number_of_intervals; ++i)
				{
					interval.first = getValue(i, 0).toFloat();
					// special case: an upper limit can be infinity
					upper_limit = getValue(i, 1);
					if (upper_limit == "inf")
					{
						interval.second = INFINITY;
					}
					else 
					{
						interval.second = getValue(i, 1).toFloat();
					}
					for (Size col = 0; col < degree; ++col)
					{
						coeffs[col] = getValue(i, col + 2).toFloat();
					}
					intervals[i] = interval;
					coefficients[i] = coeffs;
				}

				poly.set(degree, intervals, coefficients);
				rdf_ = RadialDistributionFunction(poly);
				return true;

			default:		
				Log.error() << "RDFSection::extractSection(): "
										<< "Unknown type." << endl;
				return false;
		}

	}


} // namespace BALL

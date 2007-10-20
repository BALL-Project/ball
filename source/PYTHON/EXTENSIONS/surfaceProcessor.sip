// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: surfaceProcessor.sip,v 1.1.30.1 2007/03/25 21:38:55 oliver Exp $
//

class  SurfaceProcessor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/surfaceProcessor.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

		enum SurfaceType
		{
			SOLVENT_EXCLUDED_SURFACE,
			SOLVENT_ACCESSIBLE_SURFACE
		};
		SurfaceProcessor();
		bool start();
		bool finish();
 		//Processor::Result operator () (Atom&);

		//const Surface& getSurface() const ;
		Surface& getSurface();

		void setProbeRadius(double);
		double getProbeRadius() const;
		void setDensity(double);
		double getDensity() const;
		//std::vector<TSphere3<double> >& getSpheres();
		void setType(SurfaceType);
		SurfaceType getType() const;
};

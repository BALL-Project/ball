// $Id: numericalSAS.C,v 1.6 2000/02/12 19:35:20 oliver Exp $

#include <BALL/STRUCTURE/numericalSAS.h>
#include <BALL/KERNEL/atom.h>

using namespace std;

namespace BALL
{

#define FLAG_DOTS       01
#define FLAG_VOLUME     02
#define FLAG_ATOM_AREA  04

	// forward
	int nsc_(double*, double*, int, int, int, double*, double**, double*, double**, int*);

	void calculateNumericalSASAtomAreas
		(HashMap<Atom*,float>& aareas,
			const Composite& composite, float probe_radius, int number_of_dots)
	{
		// extract all atoms: iterate over all composites and
		// check whether they are Atoms
		vector<Atom*>	atoms;
		Composite::SubcompositeIterator	it = composite.beginSubcomposite();
		for (; it != composite.endSubcomposite(); ++it)
		{
			if (RTTI::isKindOf<Atom>(*it))
			{
				Atom* atom = RTTI::castTo<Atom>(*it);
				if (atom->getRadius() != 0.0)
				{
					atoms.push_back(atom);
				}
			}
		}
		
		// if no atoms are found, return zero
		if (atoms.size() == 0)
		{
			return;
		}
				
		// create the field required by nsc and fill it with the atom coordinates
		double* coordinates = new double[atoms.size() * 3];
		double* radii = new double[atoms.size()];
		for (Size i = 0; i < atoms.size(); i++)
		{
			float tmp[3];
			atoms[i]->getPosition().get(tmp);
			coordinates[i * 3]			= (double)tmp[0];
			coordinates[i * 3 + 1]	= (double)tmp[1];
			coordinates[i * 3 + 2]	= (double)tmp[2];
			radii[i] = atoms[i]->getRadius() + probe_radius;
		}

		double area;
		double volume;
		int number_of_surface_dots;
		double* atom_areas = 0;
		double* surface_dots = 0;

		// call nsc
		nsc_(coordinates, radii, (int)atoms.size(),
				 number_of_dots, FLAG_ATOM_AREA, 
				 &area, &atom_areas, &volume, 
				 &surface_dots, &number_of_surface_dots);


		// clear the hash map
		aareas.clear();
		
		// iterate over all atoms and insert them into the
		// hash map aareas
		it = composite.beginSubcomposite();
		for (Size j = 0; it != composite.endSubcomposite(); ++it)
		{
			if (RTTI::isKindOf<Atom>(*it))
			{
				aareas.insert(pair<Atom*,float>(RTTI::castTo<Atom>(*it), (float)atom_areas[j]));
				j++;
			}
		}

		// free arrays (if created)
		if (atom_areas != 0)
		{
			free(atom_areas);
		}
		if (surface_dots != 0)
		{
			free(surface_dots);
		}

		// free the input fields
		delete [] coordinates;
		delete [] radii;

		return;
	}


	float calculateNumericalSASArea	
		(const Composite& composite, float probe_radius, int number_of_dots)
	{
		// extract all atoms: iterate over all composites and
		// check whether they are Atoms
		vector<Atom*>	atoms;
		Composite::SubcompositeIterator	it = composite.beginSubcomposite();
		for (; it != composite.endSubcomposite(); ++it)
		{
			if (RTTI::isKindOf<Atom>(*it))
			{
				Atom* atom = RTTI::castTo<Atom>(*it);
				if (atom->getRadius() != 0.0)
				{
					atoms.push_back(atom);
				}
			}
		}
		
		// if no atoms are found, return zero
		if (atoms.size() == 0)
		{
			return 0;
		}
				
		// create the field required by nsc and fill it with the atom coordinates
		double* coordinates = new double[atoms.size() * 3];
		double* radii = new double[atoms.size()];
		for (Size i = 0; i < atoms.size(); i++)
		{
			float tmp[3];
			atoms[i]->getPosition().get(tmp);
			coordinates[i * 3]			= (double)tmp[0];
			coordinates[i * 3 + 1]	= (double)tmp[1];
			coordinates[i * 3 + 2]	= (double)tmp[2];
			radii[i] = atoms[i]->getRadius() + probe_radius;
		}

		double area;
		double volume;
		int number_of_surface_dots;
		double* atom_areas = 0;
		double* surface_dots = 0;

		// call nsc
		nsc_(coordinates, radii, (int)atoms.size(),
				 number_of_dots, FLAG_ATOM_AREA, 
				 &area, &atom_areas, &volume, 
				 &surface_dots, &number_of_surface_dots);

		// free arrays (if created)
		if (atom_areas != 0)
		{
			free(atom_areas);
		}
		if (surface_dots != 0)
		{
			free(surface_dots);
		}

		// free the input fields
		delete [] coordinates;
		delete [] radii;

		return area;
	}

	/*
	 *
	 *  program NSC
	 *  version 2.0  (April 1995)
	 *
	 *  Author: Frank Eisenhaber
	 *
	 *  For user notes see file nsc.h and readme.nsc !!
	 *
	 *  Copyright Notice:
	 *  All rights reserved, whether the whole or part of the program is
	 *  concerned. The software may not be used without specific, prior 
	 *  written permission of the author. 
	 *
	 *  An academic licence agreement for the package ASC/GM or its parts
	 *  is granted if you make the following commitments:
	 *  1) In using this software, the user will respect the interests of 
	 *     the author.
	 *  2) The use of the software in commercial activities is not allowed 
	 *     without a prior written commercial licence agreement.
	 *  3) Other interested research groups will be redirected
	 *     to the author. The user will not redistribute the code outside
	 *     his immediate research group.
	 *  4) The copyright messages will not be modified or suppressed.
	 *  5) The reference given below will be cited in any publication
	 *     of scientific results based in part or completely on use of the
	 *     program.
	 *  6) Bugs will be reported to the author.
	 *
	 *  Permission to use, copy, and modify this software and
	 *  its documentation is hereby granted without fee for 
	 *  academic use, provided
	 *  that the above copyright notices and this permission notice appear in
	 *  all copies of the software and related documentation.
	 *
	 *  THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF
	 *  ANY KIND,
	 *  EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
	 *  WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
	 *
	 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
	 *  ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
	 *  OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
	 *  WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
	 *  LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
	 *  OF THIS SOFTWARE.
	 *
	 * 
	 *  contact address :    European Molecular Biology Laboratory
	 *                       Biocomputing Unit
	 *                       Meyerhofstr. 1
	 *                       Postfach 10.2209
	 *                       D-69012 Heidelberg
	 *                       Federal Republic of Germany
	 *
	 *
	 *
	 *  E-mail : IN%"EISENHABER@EMBL-Heidelberg.DE"
	 *  Please send your contact address to get information on updates and
	 *  new features. Questions will be answered as soon as possible.
	 *
	 *
	 *  references :
	 *  1.F.Eisenhaber, P.Lijnzaad, P.Argos, M.Scharf
	 *    "The Double Cubic Lattice Method: Efficient Approaches to
	 *    Numerical Integration of Surface Area and Volume and to Dot
	 *    Surface Contouring of Molecular Assemblies"
	 *    Journal of Computational Chemistry (1995) v.16, N3, pp.273-284
	 *  2.F.Eisenhaber, P.Argos
	 *    "Improved Strategy in Analytic Surface Calculation for Molecular
	 *    Systems: Handling of Singularities and Computational Efficiency"
	 *    Journal of Computational Chemistry (1993) v.14, N11, pp.1272-1280
	 *
	 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>

#define TEST_NSC 0

#define TEST_ARC 0
#define TEST_DOD 0
#define TEST_CUBE 0

#define UNSP_ICO_DOD      9
#define UNSP_ICO_ARC     10

#define NSC_WARNING\
	Log.level(LogStream::WARNING) << "NSC: error in " << __FILE__\
		<< ":" << __LINE__ << ": " 

#define NSC_ERROR\
	Log.level(LogStream::ERROR) << "NSC: error in " << __FILE__\
		<< ":" << __LINE__ << ": " 

typedef double * point_double;
	typedef int    * point_int;
	point_double xpunsp=NULL;
	double       del_cube;
	point_int    ico_wk=NULL, ico_pt=NULL;
	int          n_dot, ico_cube, last_n_dot=0, last_densit=0, last_unsp=0;
	int          last_cubus=0;

#define FOURPI (4.*M_PI)
#define TORAD(A)     ((A)*0.017453293)
#define DP_TOL     0.001
#define MAXIMUM(A, B)  (((A) > (B) ? (A) : (B)))
#define MINIMUM(A, B)  (((A) < (B) ? (A) : (B)))


#define ASIN safe_asin
	double safe_asin(double f) {
		if ( (fabs(f) < 1.00) ) return( asin(f) );
		if ( (fabs(f) - 1.00)  <= DP_TOL ) 
		{
			NSC_WARNING << "ASIN : invalid argument" << f << endl;
		}
		return(M_PI_2);
		}

#define CALLOC(n, size) mycalloc(__FILE__,__LINE__, n, size)
	void * mycalloc(const char * filename, const int linenr,
									size_t nelem, size_t elsize) {
		int * ip;
		ip = (int *) calloc(nelem, elsize);
		if(ip == NULL)
		{
			NSC_ERROR << "CALLOC : failed in file " << filename << " at line " << linenr << endl;
		}
		return(ip);
		}

#define REALLOC(ptr, size) myrealloc(__FILE__,__LINE__,  ptr, size)
	void * myrealloc(const char * filename, const int linenr,
									 void * ptr, size_t size) {
		int * ip;
		ip = (int *) realloc(ptr, size);
		if(ip == NULL)
		{
			NSC_ERROR << "REALLOC : failed in file " << filename << " at line " << linenr << endl;
		}
		return(ip);
		}


	/* routines for dot distributions on the surface of the unit sphere */
	double rg, rh;

	void icosaeder_vertices(double *xus) {
		rh = sqrt(1.-2.*cos(TORAD(72.)))/(1.-cos(TORAD(72.)));
		rg = cos(TORAD(72.))/(1.-cos(TORAD(72.)));
	/* icosaeder vertices */
		xus[ 0] = 0.;                  xus[ 1] = 0.;                  xus[ 2] = 1.;
		xus[ 3] = rh*cos(TORAD(72.));  xus[ 4] = rh*sin(TORAD(72.));  xus[ 5] = rg;
		xus[ 6] = rh*cos(TORAD(144.)); xus[ 7] = rh*sin(TORAD(144.)); xus[ 8] = rg;
		xus[ 9] = rh*cos(TORAD(216.)); xus[10] = rh*sin(TORAD(216.)); xus[11] = rg;
		xus[12] = rh*cos(TORAD(288.)); xus[13] = rh*sin(TORAD(288.)); xus[14] = rg;
		xus[15] = rh;                  xus[16] = 0;                   xus[17] = rg;
		xus[18] = rh*cos(TORAD(36.));  xus[19] = rh*sin(TORAD(36.));  xus[20] = -rg;
		xus[21] = rh*cos(TORAD(108.)); xus[22] = rh*sin(TORAD(108.)); xus[23] = -rg;
		xus[24] = -rh;                 xus[25] = 0;                   xus[26] = -rg;
		xus[27] = rh*cos(TORAD(252.)); xus[28] = rh*sin(TORAD(252.)); xus[29] = -rg;
		xus[30] = rh*cos(TORAD(324.)); xus[31] = rh*sin(TORAD(324.)); xus[32] = -rg;
		xus[33] = 0.;                  xus[34] = 0.;                  xus[35] = -1.;
		}


	void divarc(double x1, double y1, double z1,
							double x2, double y2, double z2,
							int div1, int div2, double *xr, double *yr, double *zr) {

		double xd, yd, zd, dd, d1, d2, s, x, y, z;
		double phi, sphi, cphi;

		xd = y1*z2-y2*z1;
		yd = z1*x2-z2*x1;
		zd = x1*y2-x2*y1;
		dd = sqrt(xd*xd+yd*yd+zd*zd);
		if (dd < DP_TOL)
		{
			NSC_ERROR << "divarc: rotation axis of length " << dd << endl;
		}

		d1 = x1*x1+y1*y1+z1*z1;
		if (d1 < 0.5)
		{
			NSC_ERROR << "divarc: vector 1 of sq.length " << d1 << endl;
		}
		d2 = x2*x2+y2*y2+z2*z2;
		if (d2 < 0.5)
		{
			NSC_ERROR << "divarc: vector 2 of sq.length " << d2 << endl;
		}

		phi = ASIN(dd/sqrt(d1*d2));
		phi = phi*((double)div1)/((double)div2);
		sphi = sin(phi); cphi = cos(phi);
		s  = (x1*xd+y1*yd+z1*zd)/dd;

		x = xd*s*(1.-cphi)/dd + x1 * cphi + (yd*z1-y1*zd)*sphi/dd;
		y = yd*s*(1.-cphi)/dd + y1 * cphi + (zd*x1-z1*xd)*sphi/dd;
		z = zd*s*(1.-cphi)/dd + z1 * cphi + (xd*y1-x1*yd)*sphi/dd;
		dd = sqrt(x*x+y*y+z*z);
		*xr = x/dd; *yr = y/dd; *zr = z/dd;
		}

	int ico_dot_arc(int densit) { /* densit...required dots per unit sphere */
	/* dot distribution on a unit sphere based on an icosaeder *
	 * great circle average refining of icosahedral face       */

		int i, j, k, tl, tl2, tn, tess;
		double a, d, x, y, z, x2, y2, z2, x3, y3, z3;
		double xij, yij, zij, xji, yji, zji, xik, yik, zik, xki, yki, zki,
			xjk, yjk, zjk, xkj, ykj, zkj;
		point_double xus=NULL;

	/* calculate tessalation level */
		a = sqrt((((double) densit)-2.)/10.);
		tess = (int) ceil(a);
		n_dot = 10*tess*tess+2;
		if (n_dot < densit) 
		{
			NSC_ERROR << "ico_dot_arc: error in formula for tessalation level (" << tess 
				<< "->" << n_dot << ", " << densit << ")" << endl;
		}

		xus = (double *) CALLOC(3*n_dot, sizeof(double));
		xpunsp = xus;
		icosaeder_vertices(xus);

		if (tess > 1) {
			tn = 12;
			a = rh*rh*2.*(1.-cos(TORAD(72.)));
	/* calculate tessalation of icosaeder edges */
			for (i=0; i<11; i++) {
				for (j=i+1; j<12; j++) {
					x = xus[3*i]-xus[3*j];
					y = xus[1+3*i]-xus[1+3*j]; z = xus[2+3*i]-xus[2+3*j];
					d = x*x+y*y+z*z;
					if (fabs(a-d) > DP_TOL) continue;
					for (tl=1; tl<tess; tl++) {
						if (tn >= n_dot) 
						{ 
							NSC_ERROR << "ico_dot: tn exceeds dimension of xus" << endl;
						}
						divarc(xus[3*i], xus[1+3*i], xus[2+3*i],
									 xus[3*j], xus[1+3*j], xus[2+3*j],
							tl, tess, &xus[3*tn], &xus[1+3*tn], &xus[2+3*tn]);
						tn++;
						}
					}
				}
	/* calculate tessalation of icosaeder faces */
			for (i=0; i<10; i++) {
				for (j=i+1; j<11; j++) {
					x = xus[3*i]-xus[3*j];
					y = xus[1+3*i]-xus[1+3*j]; z = xus[2+3*i]-xus[2+3*j];
					d = x*x+y*y+z*z;
					if (fabs(a-d) > DP_TOL) continue;

					for (k=j+1; k<12; k++) {
						x = xus[3*i]-xus[3*k];
						y = xus[1+3*i]-xus[1+3*k]; z = xus[2+3*i]-xus[2+3*k];
						d = x*x+y*y+z*z;
						if (fabs(a-d) > DP_TOL) continue;
						x = xus[3*j]-xus[3*k];
						y = xus[1+3*j]-xus[1+3*k]; z = xus[2+3*j]-xus[2+3*k];
						d = x*x+y*y+z*z;
						if (fabs(a-d) > DP_TOL) continue;
						for (tl=1; tl<tess-1; tl++) {
							divarc(xus[3*j], xus[1+3*j], xus[2+3*j],
										 xus[3*i], xus[1+3*i], xus[2+3*i],
								tl, tess, &xji, &yji, &zji);
							divarc(xus[3*k], xus[1+3*k], xus[2+3*k],
										 xus[3*i], xus[1+3*i], xus[2+3*i],
								tl, tess, &xki, &yki, &zki);

							for (tl2=1; tl2<tess-tl; tl2++) {
								divarc(xus[3*i], xus[1+3*i], xus[2+3*i],
											 xus[3*j], xus[1+3*j], xus[2+3*j],
									tl2, tess, &xij, &yij, &zij);
								divarc(xus[3*k], xus[1+3*k], xus[2+3*k],
											 xus[3*j], xus[1+3*j], xus[2+3*j],
									tl2, tess, &xkj, &ykj, &zkj);
								divarc(xus[3*i], xus[1+3*i], xus[2+3*i],
											 xus[3*k], xus[1+3*k], xus[2+3*k],
									tess-tl-tl2, tess, &xik, &yik, &zik);
								divarc(xus[3*j], xus[1+3*j], xus[2+3*j],
											 xus[3*k], xus[1+3*k], xus[2+3*k],
									tess-tl-tl2, tess, &xjk, &yjk, &zjk);
								if (tn >= n_dot)
								{
									NSC_ERROR << "ico_dot: tn exceeds dimension of xus" << endl;
								}
								divarc(xki, yki, zki, xji, yji, zji, tl2, tess-tl,
									&x, &y, &z);
								divarc(xkj, ykj, zkj, xij, yij, zij, tl, tess-tl2,
									&x2, &y2, &z2);
								divarc(xjk, yjk, zjk, xik, yik, zik, tl, tl+tl2,
									&x3, &y3, &z3);
								x = x+x2+x3; y = y+y2+y3; z = z+z2+z3;
								d = sqrt(x*x+y*y+z*z);
								xus[3*tn] = x/d;
								xus[1+3*tn] = y/d;
								xus[2+3*tn] = z/d;
								tn++;
								}		/* cycle tl2 */
							}		/* cycle tl */
						}		/* cycle k */
					}		/* cycle j */
				}			/* cycle i */
			if (n_dot != tn) 
			{
				NSC_ERROR << "ico_dot: n_dot(" << n_dot << ") and tn(" << tn << ") differ" << endl;
			}
		}		/* end of if (tess > 1) */
	return n_dot;
	}		/* end of routine ico_dot_arc */

	int ico_dot_dod(int densit) { /* densit...required dots per unit sphere */
	/* dot distribution on a unit sphere based on an icosaeder *
	 * great circle average refining of icosahedral face       */

		int i, j, k, tl, tl2, tn, tess, j1, j2;
		double a, d, x, y, z, x2, y2, z2, x3, y3, z3, ai_d, adod;
		double xij, yij, zij, xji, yji, zji, xik, yik, zik, xki, yki, zki,
			xjk, yjk, zjk, xkj, ykj, zkj;
		point_double xus=NULL;
	/* calculate tesselation level */
		a = sqrt((((double) densit)-2.)/30.);
		tess = MAXIMUM((int) ceil(a), 1);
		n_dot = 30*tess*tess+2;
		if (n_dot < densit) 
		{
			NSC_ERROR << "ico_dot_dod: error in formula for tessalation level (" << tess << "->" 
				<< n_dot << ", " << densit << ")" << endl;
		}

		xus = (double *) CALLOC(3*n_dot, sizeof(double));
		xpunsp = xus;
		icosaeder_vertices(xus);

		tn=12;
	/* square of the edge of an icosaeder */
			a = rh*rh*2.*(1.-cos(TORAD(72.)));
	/* dodecaeder vertices */
		for (i=0; i<10; i++) {
			for (j=i+1; j<11; j++) {
				x = xus[3*i]-xus[3*j];
				y = xus[1+3*i]-xus[1+3*j]; z = xus[2+3*i]-xus[2+3*j];
				d = x*x+y*y+z*z;
				if (fabs(a-d) > DP_TOL) continue;
				for (k=j+1; k<12; k++) {
					x = xus[3*i]-xus[3*k];
					y = xus[1+3*i]-xus[1+3*k]; z = xus[2+3*i]-xus[2+3*k];
					d = x*x+y*y+z*z;
					if (fabs(a-d) > DP_TOL) continue;
					x = xus[3*j]-xus[3*k];
					y = xus[1+3*j]-xus[1+3*k]; z = xus[2+3*j]-xus[2+3*k];
					d = x*x+y*y+z*z;
					if (fabs(a-d) > DP_TOL) continue;
					x = xus[  3*i]+xus[  3*j]+xus[  3*k];
					y = xus[1+3*i]+xus[1+3*j]+xus[1+3*k];
					z = xus[2+3*i]+xus[2+3*j]+xus[2+3*k];
					d = sqrt(x*x+y*y+z*z);
					xus[3*tn]=x/d; xus[1+3*tn]=y/d; xus[2+3*tn]=z/d;
					tn++;
					}
				}
			}

		if (tess > 1) {
			tn = 32;
	/* square of the edge of an dodecaeder */
			adod = 4.*(cos(TORAD(108.))-cos(TORAD(120.)))/(1.-cos(TORAD(120.)));
	/* square of the distance of two adjacent vertices of ico- and dodecaeder */
			ai_d = 2.*(1.-sqrt(1.-a/3.));

	/* calculate tessalation of mixed edges */
			for (i=0; i<31; i++) {
				j1 = 12; j2 = 32; a = ai_d;
				if (i>=12) { j1=i+1; a = adod; }
				for (j=j1; j<j2; j++) {
					x = xus[3*i]-xus[3*j];
					y = xus[1+3*i]-xus[1+3*j]; z = xus[2+3*i]-xus[2+3*j];
					d = x*x+y*y+z*z;
					if (fabs(a-d) > DP_TOL) continue;
					for (tl=1; tl<tess; tl++) {
						if (tn >= n_dot) 
						{
							NSC_ERROR << "ico_dot: tn exceeds dimension of xus" << endl;
						}
						divarc(xus[3*i], xus[1+3*i], xus[2+3*i],
									 xus[3*j], xus[1+3*j], xus[2+3*j],
							tl, tess, &xus[3*tn], &xus[1+3*tn], &xus[2+3*tn]);
						tn++;
						}
					}
				}
	/* calculate tessalation of pentakisdodecahedron faces */
			for (i=0; i<12; i++) {
				for (j=12; j<31; j++) {
					x = xus[3*i]-xus[3*j];
					y = xus[1+3*i]-xus[1+3*j]; z = xus[2+3*i]-xus[2+3*j];
					d = x*x+y*y+z*z;
					if (fabs(ai_d-d) > DP_TOL) continue;

					for (k=j+1; k<32; k++) {
						x = xus[3*i]-xus[3*k];
						y = xus[1+3*i]-xus[1+3*k]; z = xus[2+3*i]-xus[2+3*k];
						d = x*x+y*y+z*z;
						if (fabs(ai_d-d) > DP_TOL) continue;
						x = xus[3*j]-xus[3*k];
						y = xus[1+3*j]-xus[1+3*k]; z = xus[2+3*j]-xus[2+3*k];
						d = x*x+y*y+z*z;
						if (fabs(adod-d) > DP_TOL) continue;
						for (tl=1; tl<tess-1; tl++) {
							divarc(xus[3*j], xus[1+3*j], xus[2+3*j],
										 xus[3*i], xus[1+3*i], xus[2+3*i],
								tl, tess, &xji, &yji, &zji);
							divarc(xus[3*k], xus[1+3*k], xus[2+3*k],
										 xus[3*i], xus[1+3*i], xus[2+3*i],
								tl, tess, &xki, &yki, &zki);

							for (tl2=1; tl2<tess-tl; tl2++) {
								divarc(xus[3*i], xus[1+3*i], xus[2+3*i],
											 xus[3*j], xus[1+3*j], xus[2+3*j],
									tl2, tess, &xij, &yij, &zij);
								divarc(xus[3*k], xus[1+3*k], xus[2+3*k],
											 xus[3*j], xus[1+3*j], xus[2+3*j],
									tl2, tess, &xkj, &ykj, &zkj);
								divarc(xus[3*i], xus[1+3*i], xus[2+3*i],
											 xus[3*k], xus[1+3*k], xus[2+3*k],
									tess-tl-tl2, tess, &xik, &yik, &zik);
								divarc(xus[3*j], xus[1+3*j], xus[2+3*j],
											 xus[3*k], xus[1+3*k], xus[2+3*k],
									tess-tl-tl2, tess, &xjk, &yjk, &zjk);
								if (tn >= n_dot) 
								{
									NSC_ERROR << "ico_dot: tn exceeds dimension of xus" << endl;
								}
								divarc(xki, yki, zki, xji, yji, zji, tl2, tess-tl,
									&x, &y, &z);
								divarc(xkj, ykj, zkj, xij, yij, zij, tl, tess-tl2,
									&x2, &y2, &z2);
								divarc(xjk, yjk, zjk, xik, yik, zik, tl, tl+tl2,
									&x3, &y3, &z3);
								x = x+x2+x3; y = y+y2+y3; z = z+z2+z3;
								d = sqrt(x*x+y*y+z*z);
								xus[3*tn] = x/d;
								xus[1+3*tn] = y/d;
								xus[2+3*tn] = z/d;
								tn++;
								}		/* cycle tl2 */
							}		/* cycle tl */
						}		/* cycle k */
					}		/* cycle j */
				}			/* cycle i */
			if (n_dot != tn) 
			{
				NSC_ERROR << "ico_dot: n_dot(" << n_dot << ") and tn(" << tn << ") differ" << endl;
			}
			}		/* end of if (tess > 1) */
		return n_dot;
		}		/* end of routine ico_dot_dod */

	int unsp_type(int densit) {
		int i1, i2;
		i1 = 1;
		while (10*i1*i1+2 < densit) i1++;
		i2 = 1;
		while (30*i2*i2+2 < densit) i2++;
		if (10*i1*i1-2 < 30*i2*i2-2) return UNSP_ICO_ARC;
		else return UNSP_ICO_DOD;
		}

	int make_unsp(int densit, int mode, int * num_dot, int cubus) {
		int ndot, ico_cube_cb, i, j, k, l, ijk, tn, tl, tl2;
		point_double xus;
		point_int    work;
		double x, y, z;

		if (xpunsp) free(xpunsp); if (ico_wk) free(ico_wk);

		k=1; if (mode < 0) { k=0; mode = -mode; }
		if (mode == UNSP_ICO_ARC)      { ndot = ico_dot_arc(densit); }
		else if (mode == UNSP_ICO_DOD)      { ndot = ico_dot_dod(densit); }
		else {
			NSC_WARNING << "make_unsp: mode " << ((k)?'+':'-') << (int)mode << " not allowed" << endl;
			return 1;
			}

		last_n_dot = ndot; last_densit = densit; last_unsp = mode;
		*num_dot=ndot; if (k) return 0;

	/* in the following the dots of the unit sphere may be resorted */
		last_unsp = -last_unsp;

	/* determine distribution of points in elementary cubes */
		if (cubus) {
			ico_cube = cubus;
			}
		else {
			last_cubus = 0;
			i=1;
			while (i*i*i*2 < ndot) i++;
			ico_cube = MAXIMUM(i-1, 0);
			}
		ico_cube_cb = ico_cube*ico_cube*ico_cube;
		del_cube=2./((double)ico_cube);
		work = (int *) CALLOC(ndot, sizeof(int));
		xus = xpunsp;
		for (l=0; l<ndot; l++) {
			i = MAXIMUM((int) floor((1.+xus[3*l])/del_cube), 0);
			if (i>=ico_cube) i = ico_cube-1;
			j = MAXIMUM((int) floor((1.+xus[1+3*l])/del_cube), 0);
			if (j>=ico_cube) j = ico_cube-1;
			k = MAXIMUM((int) floor((1.+xus[2+3*l])/del_cube), 0);
			if (k>=ico_cube) k = ico_cube-1;
			ijk = i+j*ico_cube+k*ico_cube*ico_cube;
			work[l] = ijk;
			}

		ico_wk = (int *) CALLOC(2*ico_cube_cb+1, sizeof(int));
		ico_pt = ico_wk+ico_cube_cb;
		for (l=0; l<ndot; l++) {
			ico_wk[work[l]]++;   /* dots per elementary cube */
			}

	/* reordering of the coordinate array in accordance with box number */
		tn=0;
		for (i=0; i<ico_cube; i++) {
			for (j=0; j<ico_cube; j++) {
				for (k=0; k<ico_cube; k++) {
					tl=0;
					tl2 = tn;
					ijk = i+ico_cube*j+ico_cube*ico_cube*k;
					*(ico_pt+ijk) = tn;
					for (l=tl2; l<ndot; l++) {
						if (ijk == work[l]) {
							x = xus[3*l]; y = xus[1+3*l]; z = xus[2+3*l];
							xus[3*l] = xus[3*tn];
							xus[1+3*l] = xus[1+3*tn]; xus[2+3*l] = xus[2+3*tn];
							xus[3*tn] = x; xus[1+3*tn] = y; xus[2+3*tn] = z;
							ijk = work[l]; work[l]=work[tn]; work[tn]=ijk;
							tn++; tl++;
							}
						}
					*(ico_wk+ijk) = tl;
					}		/* cycle k */
				}			/* cycle j */
			}			/* cycle i */
		free(work); return 0;
		}


	typedef struct _stwknb {
		double x;
		double y;
		double z;
		double dot;
		} Neighb;

	int nsc_
		(double *co, double *radius, int nat, int densit, int mode,
		 double *value_of_area, double **at_area, double *value_of_vol,
		 double **lidots, int *nu_dots) 
	{

		int iat, i, ii, iii, ix, iy, iz, ixe, ixs, iye, iys, ize, izs, i_ac;
		int jat, j, jj, jjj, jx, jy, jz;
		int distribution;
		int l;
		int maxnei, nnei, last, maxdots;
		point_int wkdot=NULL, wkbox=NULL, wkat1=NULL, wkatm=NULL;
		Neighb  *wknb, *ctnb;
		int iii1, iii2, iiat, lfnr, i_at, j_at;
		double dx, dy, dz, dd, ai, aisq, ajsq, aj, as, a;
		double xi, yi, zi, xs=0., ys=0., zs=0.;
		double dotarea, area, vol=0.;
		point_double xus, dots=NULL, atom_area=NULL;

		int    nxbox, nybox, nzbox, nxy, nxyz;
		double xmin, ymin, zmin, xmax, ymax, zmax, ra2max, d, *pco;

		distribution = unsp_type(densit);
		if (distribution != -last_unsp || last_cubus != 4 ||
			 (densit != last_densit && densit != last_n_dot)) {
			if (make_unsp(densit, (-distribution), &n_dot, 4)) return 1;
			}
		xus = xpunsp;

		dotarea = FOURPI/(double) n_dot;
		area = 0.;

	#if TEST_CUBE 
		printf("nsc_dclm: n_dot=%5d %9.3f\n", n_dot, dotarea);
	#endif

	/* start with neighbour list */
	/* calculate neighbour list with the box algorithm */
		if (nat==0) 
		{
			NSC_WARNING << "nsc_dclm: no surface atoms selected" << endl;
			return 1;
		}
		if (mode & FLAG_VOLUME) vol=0.;
		if (mode & FLAG_DOTS) {
			maxdots = 3*n_dot*nat/10;
			dots = (double *) CALLOC(maxdots, sizeof(double));
			lfnr=0;
			}
		if (mode & FLAG_ATOM_AREA) {
			atom_area = (double *) CALLOC(nat, sizeof(double));
			}

	/* dimensions of atomic set, cell edge is 2*ra_max */
		xmin = co[0]; xmax = xmin; xs=xmin;
		ymin = co[1]; ymax = ymin; ys=ymin;
		zmin = co[2]; zmax = zmin; zs=zmin;
		ra2max = radius[0];

		for (iat=1; iat<nat; iat++) {
			pco = co+3*iat;
			xmin = MINIMUM(xmin, *pco);     xmax = MAXIMUM(xmax, *pco);
			ymin = MINIMUM(ymin, *(pco+1)); ymax = MAXIMUM(ymax, *(pco+1));
			zmin = MINIMUM(zmin, *(pco+2)); zmax = MAXIMUM(zmax, *(pco+2));
			xs= xs+ *pco; ys = ys+ *(pco+1); zs= zs+ *(pco+2);
			ra2max = MAXIMUM(ra2max, radius[iat]);
			}
		xs = xs/ (double) nat;
		ys = ys/ (double) nat;
		zs = zs/ (double) nat;
		ra2max = 2.*ra2max;
	#if TEST_CUBE
		printf("nsc_dclm: n_dot=%5d ra2max=%9.3f %9.3f\n", n_dot, ra2max, dotarea);
	#endif

		d = xmax-xmin; nxbox = (int) MAXIMUM(ceil(d/ra2max), 1.);
		d = (((double)nxbox)*ra2max-d)/2.;
		xmin = xmin-d; xmax = xmax+d;
		d = ymax-ymin; nybox = (int) MAXIMUM(ceil(d/ra2max), 1.);
		d = (((double)nybox)*ra2max-d)/2.;
		ymin = ymin-d; ymax = ymax+d;
		d = zmax-zmin; nzbox = (int) MAXIMUM(ceil(d/ra2max), 1.);
		d = (((double)nzbox)*ra2max-d)/2.;
		zmin = zmin-d; zmax = zmax+d;
		nxy = nxbox*nybox;
		nxyz = nxy*nzbox;

	/* box number of atoms */
		wkatm = (int *) CALLOC(3*nat, sizeof(int));
		wkat1 = wkatm+nat;
		wkdot = (int *) CALLOC(n_dot+nxyz+1, sizeof(int));
		wkbox = wkdot+n_dot;

		for (iat=0; iat<nat; iat++) {
			pco = co+3*iat;
			i = (int) MAXIMUM(floor((  *pco  -xmin)/ra2max), 0); i = MINIMUM(i,nxbox-1);
			j = (int) MAXIMUM(floor((*(pco+1)-ymin)/ra2max), 0); j = MINIMUM(j,nybox-1);
			l = (int) MAXIMUM(floor((*(pco+2)-zmin)/ra2max), 0); l = MINIMUM(l,nzbox-1);
			i = i+j*nxbox+l*nxy;
			wkat1[iat] = i; wkbox[i]++;
			}

	/* sorting of atoms in accordance with box numbers */
		j = wkbox[0]; for (i=1; i<nxyz; i++) j= MAXIMUM(wkbox[i], j);
		for (i=1; i<=nxyz; i++) wkbox[i] += wkbox[i-1];
	/*
		maxnei = (int) floor(ra2max*ra2max*ra2max*0.5);
	*/
		maxnei = MINIMUM(nat, 27*j);
		wknb = (Neighb *) CALLOC(maxnei, sizeof(Neighb));
		for (iat=0; iat<nat; iat++) {
			wkatm[--wkbox[wkat1[iat]]] = iat;
	#if TEST_CUBE
			printf("atom %5d on place %5d\n", iat, wkbox[wkat1[iat]]);
	#endif
			}
	#if TEST_CUBE
		printf("nsc_dclm: n_dot=%5d ra2max=%9.3f %9.3f\n", n_dot, ra2max, dotarea);
		printf("neighbour list calculated/box(xyz):%d %d %d\n", nxbox, nybox, nzbox);
		for (i=0; i<nxyz; i++) printf("box %6d : atoms %4d-%4d    %5d\n",
			i, wkbox[i], wkbox[i+1]-1, wkbox[i+1]-wkbox[i]);
		for (i=0; i<nat; i++) {
			printf("list place %5d by atom %7d\n", i, wkatm[i]);
			}
	#endif

	/* calculate surface for all atoms, step cube-wise */
		for (iz=0; iz<nzbox; iz++) {
			iii = iz*nxy;
			izs = MAXIMUM(iz-1,0); ize = MINIMUM(iz+2, nzbox);
		for (iy=0; iy<nybox; iy++) {
			ii = iy*nxbox+iii;
			iys = MAXIMUM(iy-1,0); iye = MINIMUM(iy+2, nybox);
		for (ix=0; ix<nxbox; ix++) {
			i = ii+ix;
			iii1=wkbox[i]; iii2=wkbox[i+1];
			if (iii1 >= iii2) continue;
			ixs = MAXIMUM(ix-1,0); ixe = MINIMUM(ix+2, nxbox);

			iiat = 0;
	/* make intermediate atom list */
			for (jz=izs; jz<ize; jz++) {
				jjj = jz*nxy;
			for (jy=iys; jy<iye; jy++) {
				jj = jy*nxbox+jjj;
			for (jx=ixs; jx<ixe; jx++) {
				j = jj+jx;
				for (jat=wkbox[j]; jat<wkbox[j+1]; jat++) {
					wkat1[iiat] = wkatm[jat]; iiat++;
					}     /* end of cycle "jat" */
				}       /* end of cycle "jx" */
				}       /* end of cycle "jy" */
				}       /* end of cycle "jz" */
			for (iat=iii1; iat<iii2; iat++) {
				i_at = wkatm[iat];
				ai = radius[i_at]; aisq = ai*ai;
				pco = co+3*i_at;
				xi = *pco; yi = *(pco+1); zi = *(pco+2);
				for (i=0; i<n_dot; i++) *(wkdot+i)=0;

				ctnb = wknb; nnei = 0;
				for (j=0; j<iiat; j++) {
					j_at = *(wkat1+j);
					if (j_at == i_at) continue;

					aj = radius[j_at]; ajsq = aj*aj;
					pco = co+3*j_at;
					dx = *pco-xi; dy = *(pco+1)-yi; dz = *(pco+2)-zi;
					dd = dx*dx+dy*dy+dz*dz;

					as = ai+aj; if (dd > as*as) continue;
					nnei++;
					ctnb->x = dx; ctnb->y = dy; ctnb->z = dz;
					ctnb->dot = (dd+aisq-ajsq)/(2.*ai); /* reference dot product */
					ctnb++;
					}

	/* check points on accessibility */
				if (nnei) {
					last = 0; i_ac = 0;
					for (l=0; l<n_dot; l++) {
						if (xus[3*l]*(wknb+last)->x+
								xus[1+3*l]*(wknb+last)->y+
								xus[2+3*l]*(wknb+last)->z <= (wknb+last)->dot) {
							for (j=0; j<nnei; j++) {
								if (xus[3*l]*(wknb+j)->x+xus[1+3*l]*(wknb+j)->y+
										xus[2+3*l]*(wknb+j)->z > (wknb+j)->dot) {
									last = j; break;
									}
								}
							if (j >= nnei) { i_ac++; wkdot[l] = 1; }
							}     /* end of cycle j */
						}       /* end of cycle l */
					}
				else {
					i_ac  = n_dot;
					for (l=0; l < n_dot; l++) wkdot[l] = 1;
					}

	#if TEST_CUBE
				printf("i_ac=%d, dotarea=%8.3f, aisq=%8.3f\n", i_ac, dotarea, aisq);
	#endif
				a = aisq*dotarea* (double) i_ac;
				area = area + a;
				if (mode & FLAG_ATOM_AREA) {
					atom_area[i_at] = a;
					}
				if (mode & FLAG_DOTS) {
					for (l=0; l<n_dot; l++) {
						if (wkdot[l]) {
							lfnr++;
							if (maxdots <= 3*lfnr+1) {
								maxdots = maxdots+n_dot*3;
								dots = (double *) REALLOC(dots, maxdots*sizeof(double));
								}
							dots[3*lfnr-3] = ai*xus[3*l]+xi;
							dots[3*lfnr-2] = ai*xus[1+3*l]+yi;
							dots[3*lfnr-1] = ai*xus[2+3*l]+zi;
							}
						}
					}
				if (mode & FLAG_VOLUME) {
					dx=0.; dy=0.; dz=0.;
					for (l=0; l<n_dot; l++) {
						if (wkdot[l]) {
							dx=dx+xus[3*l];
							dy=dy+xus[1+3*l];
							dz=dz+xus[2+3*l];
							}
						}
					vol = vol+aisq*(dx*(xi-xs)+dy*(yi-ys)+dz*(zi-zs)+ai* (double) i_ac);
					}

				}         /* end of cycle "iat" */
			}           /* end of cycle "ix" */
			}           /* end of cycle "iy" */
			}           /* end of cycle "iz" */

		free(wkatm); free(wkdot); free(wknb);

		if (mode & FLAG_VOLUME) {
			vol = vol*FOURPI/(3.* (double) n_dot);
			*value_of_vol = vol;
			}
		if (mode & FLAG_DOTS) {
			*nu_dots = lfnr;
			*lidots = dots;
			}
		if (mode & FLAG_ATOM_AREA) {
			*at_area = atom_area;
			}
		*value_of_area = area;

	#if TEST_CUBE
		printf("area=%8.3f\n", area);
	#endif
		return 0;
		}


#undef FLAG_DOTS
#undef FLAG_VOLUME
#undef FLAG_ATOM_AREA
 

} // namespace BALL

// $Id: connolly.C,v 1.7 2000/06/27 14:19:50 oliver Exp $

#include <math.h>
#include <algorithm>
#include <iostream>

using std::min;
using std::max;
using std::cerr;
using std::endl;

namespace	BALL
{

/* Common Block Declarations */

	struct
	{
		double x[5000], y[5000], z__[5000];
		int n;
	}
	atoms_;

#define atoms_1 atoms_

	struct
	{
		double a[15000] /* was [3][5000] */ , ar[5000], pr;
		int na;
	}
	face01_;

#define face01_1 face01_

	struct
	{
		bool skip[5000], nosurf[5000], afree[5000], abur[5000];
	}
	face02_;

#define face02_1 face02_

	struct
	{
		int anbr[20000] /* was [2][5000] */ , nbr[500000], nbrt[500000];
	}
	face03_;

#define face03_1 face03_

	struct
	{
		int ntt, tta[500000] /* was [2][250000] */ , ttfe[250000], ttle[250000], enext[50000];
		bool ttbur[250000], ttfree[250000];
	}
	face04_;

#define face04_1 face04_

	struct
	{
		double t[45000] /* was [3][15000] */ , tr[15000], tax[45000]	/* was [3][15000] */ ;
		int nt, ta[30000] /* was [2][15000] */ , tfe[15000];
		bool tfree[15000];
	}
	face05_;

#define face05_1 face05_

	struct
	{
		double p[30000] /* was [3][10000] */ ;
		int np, pa[30000] /* was [3][10000] */ ;
	}
	face06_;

#define face06_1 face06_

	struct
	{
		double v[75000] /* was [3][25000] */ ;
		int nv, va[25000], vp[25000];
	}
	face07_;

#define face07_1 face07_

	struct
	{
		int nen, env[50000] /* was [2][25000] */ , nfn, fnen[30000]	/* was [3][10000] */ ;
	}
	face08_;

#define face08_1 face08_

	struct
	{
		double c__[75000] /* was [3][25000] */ , cr[25000];
		int nc, ca[25000], ct[25000];
	}
	face09_;

#define face09_1 face09_

	struct
	{
		int nep, epc[25000], epv[50000] /* was [2][25000] */ , afe[5000], ale[5000], epnext[25000];
	}
	face10_;

#define face10_1 face10_

	struct
	{
		int nfs, fsen[30000] /* was [2][15000] */ , fsep[30000]	/* was [2][15000] */ ;
	}
	face11_;

#define face11_1 face11_

	struct
	{
		int ncy, cynep[5000], cyep[150000] /* was [30][5000] */ ;
	}
	face12_;

#define face12_1 face12_

	struct
	{
		int nfp, fpa[5000], fpcy[50000] /* was [10][5000] */ , fpncy[5000];
	}
	face13_;

#define face13_1 face13_

/* Table of constant values */

	static double c_b94 = 0.0;
	static double c_b157 = -1.0;
	static double c_b158 = 1.0;


	inline double dist2_ (double* x, double* y)
	{
		return ((x[0] - y[0]) * (x[0] - y[0]) + (x[1] - y[1]) * (x[1] - y[1]) + (x[2] - y[2]) * (x[2] - y[2]));
	}

	int vcross_ (double *x, double *y, double *z__);
	int vnorm_ (double *x, double *xn);
	double anorm_ (double *x);
	int gettor_ (int *ia, int *ja, bool * ttok, double *torcen, double *torad, double *torax);
	int getprb_ (int *ia, int *ja, int *ka, bool * prbok, bool * tb, double *bijk, double *hijk, double *uijk);
	int vam_ (double *volume, double *area, double* atom_areas);
	bool ptincy_ (double *pnt, double *unvect, int *icy);
	double rotang_ (double *epu, int *nedge, double *unvect);
	double vecang_ (double *v1, double *v2, double *axis, double *hand);
	double depth_ (int *ip, double *alt);
	int neighbor_ ();
	int torus_ ();
	int place_ ();
	int compress_ ();
	int saddles_ ();
	int contact_ ();
	int inedge_ (int *ien, int *itt);
	int ipedge_ (int *iep, int *ia);
	int measpm_ (int *ifn, double *prism);
	int measfp_ (int *ifp, double *areap, double *volp);
	int measfs_ (int *ifs, double *areas, double *vols, double *areasp, double *volsp, int* a1, int* a2);
	int measfn_ (int *ifn, double *arean, double *voln, int* a1, int* a2, int* a3);
	int projct_ (double *pnt, double *unvect, int *icy, int *ia, double *spv, int *nedge, bool * fail);
	int epuclc_ (double *spv, int *nedge, double *epu);
	int cirpln_ (double *circen, double *cirrad, double *cirvec,
							 double *plncen, double *plnvec, bool * cinsp, bool * cintp, double *xpnt1, double *xpnt2);
	int gendot_ (int *ndots, double *dots, double *radius, double *xcenter, double *ycenter, double *zcenter);
	double dot_ (double *x, double *y);
	double triple_ (double *x, double *y, double *z__);

	int connolly_ (int number_of_atoms, double *coordinates, double *radius,
								 double *volume, double *area, double probe, double exclude, double* atom_areas)
	{
		/*     set the probe radius and the number of atoms */

		/* Parameter adjustments */
		--radius;

		/* Function Body */
		face01_1.pr = probe;
		face01_1.na = number_of_atoms;

		/*     set atom coordinates and radii, the excluded buffer */

		/*     radius ("exclude") is added to atomic radii */

		for (int i = 1; i <= face01_1.na; ++i)
		{
			face01_1.a[i * 3 - 3] = coordinates[3 * i - 3];
			face01_1.a[i * 3 - 2] = coordinates[3 * i - 2];
			face01_1.a[i * 3 - 1] = coordinates[3 * i - 1];
			face01_1.ar[i - 1] = radius[i];
			if (face01_1.ar[i - 1] == 0.0)
			{
				face02_1.skip[i - 1] = true;
			}
			else
			{
				face01_1.ar[i - 1] += exclude;
				face02_1.skip[i - 1] = false;
			}
		}

		/*     find the analytical volume and surface area */
		neighbor_();
		torus_();
		place_();
		compress_();
		saddles_();
		contact_();
		vam_(volume, area, atom_areas);

		return 0;
	}	/* connolly_ */


	/*     ################################################################## */
	/*     ##                                                              ## */
	/*     ##  subroutine getprb  --  test probe site between three atoms  ## */
	/*     ##                                                              ## */
	/*     ################################################################## */


	/*     "getprb" tests for a possible probe position at the interface */
	/*     between three neighboring atoms */


	int getprb_ (int *ia, int *ja, int *ka, bool * prbok, bool * tb, double *bijk, double *hijk, double *uijk)
	{
		/* System generated locals */
		double d__1, d__2;

		/* Local variables */
		static double fact, wijk;
		static int k;
		static double tijik[3], swijk, dotut, dotijk;
		static double dba, rad, rij;
		static double rik, uij[3], uik[3], utb[3], tij[3], tik[3];
		static bool tok;
		static double rad2, dat2, rip2;




		/*     initialize, then check torus over atoms "ia" and "ja" */
		/* Parameter adjustments */
		--uijk;
		--bijk;

		/* Function Body */
		*prbok = false;
		*tb = false;
		gettor_ (ia, ja, &tok, tij, &rij, uij);
		if (!tok)
		{
			return 0;
		}
		dat2 =
		dist2_ (&face01_1.a[*ka * 3 - 3], tij);

		d__1 = face01_1.ar[*ka - 1] + face01_1.pr;

		d__2 = rij;
		rad2 = d__1 * d__1 - d__2 * d__2;

		/*     if "ka" less than "ja", then all we care about */
		/*     is whether the torus is buried */

		if (*ka < *ja)
		{
			if (rad2 <= 0.0)
			{
				return 0;
			}
			if (dat2 > rad2)
			{
				return 0;
			}
		}
		gettor_ (ia, ka, &tok, tik, &rik, uik);
		if (!tok)
		{
			return 0;
		}
		dotijk = dot_ (uij, uik);
		if (dotijk > 1.)
		{
			dotijk = 1.;
		}
		if (dotijk < -1.)
		{
			dotijk = -1.;
		}
		wijk = acos (dotijk);
		swijk = sin (wijk);

		/*     if the three atoms are colinear, then there is no */
		/*     probe placement; but we still care whether the torus */
		/*     is buried by atom "k" */

		if (swijk == 0.0)
		{
			*tb = rad2 > 0.0 && dat2 <= rad2;
			return 0;
		}
		vcross_ (uij, uik, &uijk[1]);
		for (k = 1; k <= 3; ++k)
		{
			uijk[k] /= swijk;
		}
		vcross_ (&uijk[1], uij, utb);
		for (k = 1; k <= 3; ++k)
		{
			tijik[k - 1] = tik[k - 1] - tij[k - 1];
		}
		dotut = dot_ (uik, tijik);
		fact = dotut / swijk;
		for (k = 1; k <= 3; ++k)
		{
			bijk[k] = tij[k - 1] + utb[k - 1] * fact;
		}
		dba = dist2_ (&face01_1.a[*ia * 3 - 3], &bijk[1]);

		d__1 = face01_1.ar[*ia - 1] + face01_1.pr;
		rip2 = d__1 * d__1;
		rad = rip2 - dba;
		if (rad < 0.0)
		{
			*tb = rad2 > 0.0 && dat2 <= rad2;
		}
		else
		{
			*prbok = true;
			*hijk = sqrt (rad);
		}
		return 0;
	}	/* getprb_ */





	/*     ################################################################ */
	/*     ##                                                            ## */
	/*     ##  subroutine gettor  --  test torus site between two atoms  ## */
	/*     ##                                                            ## */
	/*     ################################################################ */


	/*     "gettor" tests for a possible torus position at the interface */
	/*     between two atoms, and finds the torus radius, center and axis */


	int gettor_ (int *ia, int *ja, bool * ttok, double *torcen, double *torad, double *torax)
	{
		/* System generated locals */
		double
			d__1, d__2, d__3;

		/* Local variables */
		static double temp;
		static double temp1, temp2;
		static int k;
		static double bij[3], dij, uij[3], vij[3];

		/*     get the distance between the two atoms */
		/* Parameter adjustments */
		--torax;
		--torcen;

		/* Function Body */
		*ttok = false;
		dij = sqrt (dist2_ (&face01_1.a[*ia * 3 - 3], &face01_1.a[*ja * 3 - 3]));

		/*     find a unit vector along interatomic (torus) axis */

		for (k = 1; k <= 3; ++k)
		{
			vij[k - 1] = face01_1.a[k + *ja * 3 - 4] - face01_1.a[k + *ia * 3 - 4];
			uij[k - 1] = vij[k - 1] / dij;
		}

		/*     find coordinates of the center of the torus */


		d__1 = face01_1.ar[*ia - 1] + face01_1.pr;
		d__2 = face01_1.ar[*ja - 1] + face01_1.pr;
		d__3 = dij;
		temp = (d__1 * d__1 - d__2 * d__2) / (d__3 * d__3) + 1.;
		for (k = 1; k <= 3; ++k)
		{
			bij[k - 1] = face01_1.a[k + *ia * 3 - 4] + vij[k - 1] * .5 * temp;
		}

		/*     skip if atoms too far apart (should not happen) */


		d__1 = face01_1.ar[*ia - 1] + face01_1.ar[*ja - 1] + face01_1.pr * 2.;
		d__2 = dij;
		temp1 = d__1 * d__1 - d__2 * d__2;
		if (temp1 >= 0.0)
		{

			/*     skip if one atom is inside the other */

			d__1 = dij;
			d__2 = face01_1.ar[*ia - 1] - face01_1.ar[*ja - 1];
			temp2 = d__1 * d__1 - d__2 * d__2;
			if (temp2 >= 0.0)
			{

				/*     store the torus radius, center and axis */
				*ttok = true;
				*torad = sqrt (temp1 * temp2) / (dij * 2.);
				for (k = 1; k <= 3; ++k)
				{
					torcen[k] = bij[k - 1];
					torax[k] = uij[k - 1];
				}
			}
		}
		return 0;
	}





	/*     ############################################################### */
	/*     ##                                                           ## */
	/*     ##  subroutine neighbor  --  list of neighboring atom pairs  ## */
	/*     ##                                                           ## */
	/*     ############################################################### */


	/*     "neighbor" finds all of the neighbors of each atom */
	/*     local variables : */
	/*     ico      int cube coordinates */
	/*     icuptr   pointer to next atom in cube */
	/*     comin    minimum atomic coordinates (cube corner) */
	/*     icube    pointer to first atom in list for cube */
	/*     scube    true if cube contains active atoms */
	/*     sscube   true if cube or adjacent cubes have active atoms */
	/*     itnl     temporary neighbor list, before sorting */


	int
	neighbor_ ()
	{
		/* System generated locals */
		int i__1, i__2, i__3, i__4, i__5, i__6;
		double d__1, d__2, d__3;

		/* Local variables */
		static int nbra[1000], jnbr, jmin, nnbr, iuse, itnl[1000], iptr;
		static double sumi, vect1, vect2, vect3;
		static int i__, j, k, m, icube[64000] /* was [40][40][40] */ , nnbra;
		static bool scube[64000] /* was [40][40][40] */ ;
		static int jmold;
		static double comin[3];
		static int iatom, jatom;
		static double width, d2;
		static int i1, j1, k1;
		static double r2, radmax;
		static int jminbr;
		static bool sscube[64000] /* was [40][40][40] */ ;
		static int icuptr[5000], ici, icj, ick, jci, jcj, jck, ico[15000] /* was [3][5000] */ ;
		static double sum;


		/*     ignore all atoms that are completely inside another atom; */

		/*     may give nonsense results if this step is not taken */

		i__1 = face01_1.na - 1;
		for (i__ = 1; i__ <= i__1; ++i__)
		{
			if (!face02_1.skip[i__ - 1])
			{
				i__2 = face01_1.na;
				for (j = i__ + 1; j <= i__2; ++j)
				{
					d2 = dist2_ (&face01_1.a[i__ * 3 - 3], &face01_1.a[j * 3 - 3]);

					d__1 = face01_1.ar[i__ - 1] - face01_1.ar[j - 1];
					r2 = d__1 * d__1;
					if (!face02_1.skip[j - 1] && d2 < r2)
					{
						if (face01_1.ar[i__ - 1] < face01_1.ar[j - 1])
						{
							face02_1.skip[i__ - 1] = true;
						}
						else
						{
							face02_1.skip[j - 1] = true;
						}
					}
				}
			}
		}

/*     check for new coordinate minima and radii maxima */

		radmax = 0.0;
		for (k = 1; k <= 3; ++k)
		{
			comin[k - 1] = face01_1.a[k - 1];
		}
		i__1 = face01_1.na;
		for (i__ = 1; i__ <= i__1; ++i__)
		{
			for (k = 1; k <= 3; ++k)
			{
				if (face01_1.a[k + i__ * 3 - 4] < comin[k - 1])
				{
					comin[k - 1] = face01_1.a[k + i__ * 3 - 4];
				}
			}
			if (face01_1.ar[i__ - 1] > radmax)
			{
				radmax = face01_1.ar[i__ - 1];
			}
		}

/*     calculate width of cube from maximum */

/*     atom radius and probe radius */

		width = (radmax + face01_1.pr) * 2.;

/*     set up cube arrays; first the int coordinate arrays */

		i__1 = face01_1.na;
		for (i__ = 1; i__ <= i__1; ++i__)
		{
			for (k = 1; k <= 3; ++k)
			{
				ico[k + i__ * 3 - 4] = (int) ((face01_1.a[k + i__ * 3 - 4] - comin[k - 1]) / width + 1);
				if (ico[k + i__ * 3 - 4] < 1)
				{
					cerr << "Cube Coordinate Too Small" << endl;
				}
				else if (ico[k + i__ * 3 - 4] > 40)
				{
					cerr << "Cube Coordinate Too Large" << endl;
				}
			}
		}

/*     initialize head pointer and srn=2 arrays */

		for (i__ = 1; i__ <= 40; ++i__)
		{
			for (j = 1; j <= 40; ++j)
			{
				for (k = 1; k <= 40; ++k)
				{
					icube[i__ + (j + k * 40) * 40 - 1641] = 0;
					scube[i__ + (j + k * 40) * 40 - 1641] = false;
					sscube[i__ + (j + k * 40) * 40 - 1641] = false;
				}
			}
		}

/*     initialize linked list pointers */

		i__1 = face01_1.na;
		for (i__ = 1; i__ <= i__1; ++i__)
		{
			icuptr[i__ - 1] = 0;
		}

/*     set up head and later pointers for each atom */

		i__1 = face01_1.na;
		for (iatom = 1; iatom <= i__1; ++iatom)
		{

/*     skip atoms with surface request numbers of zero */

			if (face02_1.skip[iatom - 1])
			{
				goto L30;
			}
			i__ = ico[iatom * 3 - 3];
			j = ico[iatom * 3 - 2];
			k = ico[iatom * 3 - 1];
			if (icube[i__ + (j + k * 40) * 40 - 1641] <= 0)
			{

/*     first atom in this cube */

				icube[i__ + (j + k * 40) * 40 - 1641] = iatom;
			}
			else
			{

/*     add to end of linked list */

				iptr = icube[i__ + (j + k * 40) * 40 - 1641];
			L10:

/*     check for duplicate atoms, turn off one of them */

				if (dist2_ (&face01_1.a[iatom * 3 - 3], &face01_1.a[iptr * 3 - 3]) <= 0.0)
				{
					face02_1.skip[iatom - 1] = true;
					goto L30;
				}

/*     move on down the list */

				if (icuptr[iptr - 1] <= 0)
				{
					goto L20;
				}
				iptr = icuptr[iptr - 1];
				goto L10;
			L20:

/*     store atom number */

				icuptr[iptr - 1] = iatom;
			}

/*     check for surfaced atom */

			if (!face02_1.skip[iatom - 1])
			{
				scube[i__ + (j + k * 40) * 40 - 1641] = true;
			}
		L30:
			;
		}

/*     check if this cube or any adjacent cube has active atoms */

		for (k = 1; k <= 40; ++k)
		{
			for (j = 1; j <= 40; ++j)
			{
				for (i__ = 1; i__ <= 40; ++i__)
				{
					if (icube[i__ + (j + k * 40) * 40 - 1641] != 0)
					{

/* Computing MAX */
						i__1 = k - 1;

/* Computing MIN */
						i__3 = k + 1;
						i__2 = std::min (i__3, 40);
						for (k1 = max (i__1, 1); k1 <= i__2; ++k1)
						{

/* Computing MAX */
							i__1 = j - 1;

/* Computing MIN */
							i__4 = j + 1;
							i__3 = min (i__4, 40);
							for (j1 = max (i__1, 1); j1 <= i__3; ++j1)
							{

/* Computing MAX */
								i__1 = i__ - 1;

/* Computing MIN */
								i__5 = i__ + 1;
								i__4 = min (i__5, 40);
								for (i1 = max (i__1, 1); i1 <= i__4; ++i1)
								{
									if (scube[i1 + (j1 + k1 * 40) * 40 - 1641])
									{
										sscube[i__ + (j + k * 40) * 40 - 1641] = true;
									}
								}
							}
						}
					}
				}
			}
		}
		nnbr = 0;

/*     zero pointers for atom and find its cube */

		i__2 = face01_1.na;
		for (i__ = 1; i__ <= i__2; ++i__)
		{
			nnbra = 0;
			face02_1.nosurf[i__ - 1] = face02_1.skip[i__ - 1];
			face03_1.anbr[(i__ << 1) - 2] = 0;
			face03_1.anbr[(i__ << 1) - 1] = 0;
			if (face02_1.skip[i__ - 1])
			{
				goto L70;
			}
			ici = ico[i__ * 3 - 3];
			icj = ico[i__ * 3 - 2];
			ick = ico[i__ * 3 - 1];

/*     skip iatom if its cube and adjoining */

/*     cubes contain only blockers */

			if (!sscube[ici + (icj + ick * 40) * 40 - 1641])
			{
				goto L70;
			}
			sumi = face01_1.pr * 2. + face01_1.ar[i__ - 1];

/*     check iatom cube and adjacent cubes for neighboring atoms */

/* Computing MAX */
			i__3 = ick - 1;

/* Computing MIN */
			i__1 = ick + 1;
			i__4 = min (i__1, 40);
			for (jck = max (i__3, 1); jck <= i__4; ++jck)
			{

/* Computing MAX */
				i__3 = icj - 1;

/* Computing MIN */
				i__5 = icj + 1;
				i__1 = min (i__5, 40);
				for (jcj = max (i__3, 1); jcj <= i__1; ++jcj)
				{

/* Computing MAX */
					i__3 = ici - 1;

/* Computing MIN */
					i__6 = ici + 1;
					i__5 = min (i__6, 40);
					for (jci = max (i__3, 1); jci <= i__5; ++jci)
					{
						j = icube[jci + (jcj + jck * 40) * 40 - 1641];
					L40:

/*     check for end of linked list for this cube */

						if (j <= 0)
						{
							goto L60;
						}
						if (i__ == j)
						{
							goto L50;
						}
						if (face02_1.skip[j - 1])
						{
							goto L50;
						}

/*     distance check */

						sum = sumi + face01_1.ar[j - 1];
						vect1 = (d__1 = face01_1.a[j * 3 - 3] - face01_1.a[i__ * 3 - 3], fabs (d__1));
						if (vect1 >= sum)
						{
							goto L50;
						}
						vect2 = (d__1 = face01_1.a[j * 3 - 2] - face01_1.a[i__ * 3 - 2], fabs (d__1));
						if (vect2 >= sum)
						{
							goto L50;
						}
						vect3 = (d__1 = face01_1.a[j * 3 - 1] - face01_1.a[i__ * 3 - 1], fabs (d__1));
						if (vect3 >= sum)
						{
							goto L50;
						}

						d__1 = vect1;

						d__2 = vect2;

						d__3 = vect3;
						d2 = d__1 * d__1 + d__2 * d__2 + d__3 * d__3;

						d__1 = sum;
						if (d2 >= d__1 * d__1)
						{
							goto L50;
						}

/*     atoms are neighbors, save atom number in temporary 
   array */

						if (!face02_1.skip[j - 1])
						{
							face02_1.nosurf[i__ - 1] = false;
						}
						++nnbra;
						if (nnbra > 1000)
						{
							cerr << "Too many Neighbors for Atom" << endl;
						}
						itnl[nnbra - 1] = j;
					L50:

/*     get number of next atom in cube */

						j = icuptr[j - 1];
						goto L40;
					L60:
						;
					}
				}
			}
			if (face02_1.nosurf[i__ - 1])
			{
				goto L70;
			}

/*     set up neighbors arrays with jatom in increasing order */

			jmold = 0;
			i__4 = nnbra;
			for (iuse = 1; iuse <= i__4; ++iuse)
			{
				jmin = face01_1.na + 1;
				i__1 = nnbra;
				for (jnbr = 1; jnbr <= i__1; ++jnbr)
				{

/*     don't use ones already sorted */

					if (itnl[jnbr - 1] > jmold)
					{
						if (itnl[jnbr - 1] < jmin)
						{
							jmin = itnl[jnbr - 1];
							jminbr = jnbr;
						}
					}
				}
				jmold = jmin;
				jnbr = jminbr;
				jatom = itnl[jnbr - 1];
				nbra[iuse - 1] = jatom;
			}

/*     set up pointers to first and last neighbors of atom */

			if (nnbra > 0)
			{
				face03_1.anbr[(i__ << 1) - 2] = nnbr + 1;
				i__4 = nnbra;
				for (m = 1; m <= i__4; ++m)
				{
					++nnbr;
					if (nnbr > 500000)
					{
						cerr << "Too many Neighboring Atom Pairs" << endl;
					}
					face03_1.nbr[nnbr - 1] = nbra[m - 1];
				}
				face03_1.anbr[(i__ << 1) - 1] = nnbr;
			}
		L70:
			;
		}
		return 0;
	}	/* neighbor_ */



/*     ############################################################## */

/*     ##                                                          ## */

/*     ##  subroutine torus  --  position of each temporary torus  ## */

/*     ##                                                          ## */

/*     ############################################################## */


/*     "torus" sets a list of all of the temporary torus positions */

/*     by testing for a torus between each atom and its neighbors */


	int
	torus_ ()
	{
		/* System generated locals */
		int
			i__1, i__2;

		/* Local variables */
		static int
			ibeg, iend;
		static double
			ttax[3];
		static bool
			ttok;
		static int
			ia, ja, jn;
		static double
			tt[3];
		static double
			ttr;


/*     no torus is possible if there is only one atom */

		face04_1.ntt = 0;
		i__1 = face01_1.na;
		for (ia = 1; ia <= i__1; ++ia)
		{
			face02_1.afree[ia - 1] = true;
		}
		if (face01_1.na <= 1)
		{
			return 0;
		}

/*     get begin and end pointers to neighbors of this atom */

		i__1 = face01_1.na;
		for (ia = 1; ia <= i__1; ++ia)
		{
			if (!face02_1.nosurf[ia - 1])
			{
				ibeg = face03_1.anbr[(ia << 1) - 2];
				iend = face03_1.anbr[(ia << 1) - 1];

/*     check for no neighbors */

				if (ibeg > 0)
				{
					i__2 = iend;
					for (jn = ibeg; jn <= i__2; ++jn)
					{

/*     clear pointer from neighbor to torus */

						face03_1.nbrt[jn - 1] = 0;

/*     get atom number of neighbor */

						ja = face03_1.nbr[jn - 1];

/*     don't create torus twice */

						if (ja >= ia)
						{

/*     do some solid geometry */

							gettor_ (&ia, &ja, &ttok, tt, &ttr, ttax);
							if (ttok)
							{

								//     we have a temporary torus, set up variables

								++face04_1.ntt;
								if (face04_1.ntt > 250000)
								{
									cerr << "Too many Temporary Tori" << endl;
								}

								//     mark both atoms not free

								face02_1.afree[ia - 1] = false;
								face02_1.afree[ja - 1] = false;
								face04_1.tta[(face04_1.ntt << 1) - 2] = ia;
								face04_1.tta[(face04_1.ntt << 1) - 1] = ja;

								//   pointer from neighbor to torus

								face03_1.nbrt[jn - 1] = face04_1.ntt;

								//     initialize torus as both free and buried

								face04_1.ttfree[face04_1.ntt - 1] = true;
								face04_1.ttbur[face04_1.ntt - 1] = true;

								//     clear pointers from torus to first and last concave edges

								face04_1.ttfe[face04_1.ntt - 1] = 0;
								face04_1.ttle[face04_1.ntt - 1] = 0;
							}
						}
					}
				}
			}
		}
		return 0;
	}	/* torus_ */



/*     ################################################################# */

/*     ##                                                             ## */

/*     ##  subroutine place  --  locate positions of the probe sites  ## */

/*     ##                                                             ## */

/*     ################################################################# */


/*     "place" finds the probe sites by putting the probe sphere */

/*     tangent to each triple of neighboring atoms */


	int
	place_ ()
	{
		/* System generated locals */
		int
			i__1, i__2, i__3;
		double
			d__1;

		/* Local variables */
		static double
			aijk[3];
		static int
			iend, jend;
		static double
			bijk[3], hijk, pijk[3], uijk[3];
		static int
			iptr, jptr;
		static bool
			ttok;
		static int
			nmut;
		static int
			k, l, lknbr[250000];
		static bool
			prbok;
		static double
			d2;
		static double
			tempv[3];
		static int
			l1, l2, ia, ja, ka, la, ke, ik, ip, jk, km, lm;
		static bool
			tb;
		static int
			kv;
		static double
			disnbr[500];
		static double
			sumnbr[500], bij[3], hij;
		static int
			lkf;
		static double
			det;
		static int
			ikt[500], jkt[500];
		static double
			uij[3];
		static int
			itt, mut[500];




/*     no possible placement if there are no temporary tori */

		face06_1.np = 0;
		face08_1.nfn = 0;
		face08_1.nen = 0;
		face07_1.nv = 0;
		if (face04_1.ntt <= 0)
		{
			return 0;
		}

/*     consider each torus in turn */

		i__1 = face04_1.ntt;
		for (itt = 1; itt <= i__1; ++itt)
		{

/*     get atom numbers */

			ia = face04_1.tta[(itt << 1) - 2];
			ja = face04_1.tta[(itt << 1) - 1];

/*     form mutual neighbor list; clear number */

/*     of mutual neighbors of atoms ia and ja */

			nmut = 0;

/*     get begin and end pointers for each atom's neighbor list */

			iptr = face03_1.anbr[(ia << 1) - 2];
			jptr = face03_1.anbr[(ja << 1) - 2];
			if (iptr <= 0 || jptr <= 0)
			{
				goto L130;
			}
			iend = face03_1.anbr[(ia << 1) - 1];
			jend = face03_1.anbr[(ja << 1) - 1];

/*     collect mutual neighbors */

		L10:

/*     check for end of loop */

			if (iptr > iend)
			{
				goto L40;
			}
			if (jptr > jend)
			{
				goto L40;
			}

/*     go move the lagging pointer */

			if (face03_1.nbr[iptr - 1] < face03_1.nbr[jptr - 1])
			{
				goto L20;
			}
			if (face03_1.nbr[jptr - 1] < face03_1.nbr[iptr - 1])
			{
				goto L30;
			}

/*     both point at same neighbor */

/*     one more mutual neighbor */

/*     save atom number of mutual neighbor */

			++nmut;
			if (nmut > 500)
			{
				cerr << "Too many Mutual Neighbors" << endl;
			}
			mut[nmut - 1] = face03_1.nbr[iptr - 1];

/*     save pointers to second and third tori */

			ikt[nmut - 1] = face03_1.nbrt[iptr - 1];
			jkt[nmut - 1] = face03_1.nbrt[jptr - 1];
		L20:

/*     increment pointer to ia atom neighbors */

			++iptr;
			goto L10;
		L30:

/*     increment pointer to ja atom neighbors */

			++jptr;
			goto L10;
		L40:

/*     we have all the mutual neighbors of ia and ja */

/*     if no mutual neighbors, skip to end of loop */

			if (nmut <= 0)
			{
				face04_1.ttbur[itt - 1] = false;
				goto L130;
			}
			gettor_ (&ia, &ja, &ttok, bij, &hij, uij);
			i__2 = nmut;
			for (km = 1; km <= i__2; ++km)
			{
				ka = mut[km - 1];
				disnbr[km - 1] = dist2_ (bij, &face01_1.a[ka * 3 - 3]);

				d__1 = face01_1.pr + face01_1.ar[ka - 1];
				sumnbr[km - 1] = d__1 * d__1;

/*     initialize link to next farthest out neighbor */

				lknbr[km - 1] = 0;
			}

/*     set up a linked list of neighbors in order of */

/*     increasing distance from ia-ja torus center */

			lkf = 1;
			if (nmut <= 1)
			{
				goto L70;
			}

/*     put remaining neighbors in linked list at proper position */

			i__2 = nmut;
			for (l = 2; l <= i__2; ++l)
			{
				l1 = 0;
				l2 = lkf;
			L50:
				if (disnbr[l - 1] < disnbr[l2 - 1])
				{
					goto L60;
				}
				l1 = l2;
				l2 = lknbr[l2 - 1];
				if (l2 != 0)
				{
					goto L50;
				}
			L60:

/*     add to list */

				if (l1 == 0)
				{
					lkf = l;
					lknbr[l - 1] = l2;
				}
				else
				{
					lknbr[l1 - 1] = l;
					lknbr[l - 1] = l2;
				}
			}
		L70:

/*     loop thru mutual neighbors */

			i__2 = nmut;
			for (km = 1; km <= i__2; ++km)
			{

/*     get atom number of neighbors */

				ka = mut[km - 1];
				if (face02_1.skip[ia - 1] && face02_1.skip[ja - 1] && face02_1.skip[ka - 1])
				{
					goto L120;
				}

/*     get tori numbers for neighbor */

				ik = ikt[km - 1];
				jk = jkt[km - 1];

/*     possible new triple, do some geometry to */

/*     retrieve saddle center, axis and radius */

				getprb_ (&ia, &ja, &ka, &prbok, &tb, bijk, &hijk, uijk);
				if (tb)
				{
					face04_1.ttbur[itt - 1] = true;
					face04_1.ttfree[itt - 1] = false;
					goto L120;
				}

/*     no duplicate triples */

				if (ka < ja)
				{
					goto L120;
				}

/*     check whether any possible probe positions */

				if (!prbok)
				{
					goto L120;
				}

/*     altitude vector */

				for (k = 1; k <= 3; ++k)
				{
					aijk[k - 1] = hijk * uijk[k - 1];
				}

/*     we try two probe placements */

				for (ip = 1; ip <= 2; ++ip)
				{
					for (k = 1; k <= 3; ++k)
					{
						if (ip == 1)
						{
							pijk[k - 1] = bijk[k - 1] + aijk[k - 1];
						}
						else
						{
							pijk[k - 1] = bijk[k - 1] - aijk[k - 1];
						}
					}

/*     mark three tori not free */

					face04_1.ttfree[itt - 1] = false;
					face04_1.ttfree[ik - 1] = false;
					face04_1.ttfree[jk - 1] = false;

/*     check for collisions */

					lm = lkf;
				L80:
					if (lm <= 0)
					{
						goto L100;
					}

/*     get atom number of mutual neighbor */

					la = mut[lm - 1];

/*     must not equal third atom */

					if (la == ka)
					{
						goto L90;
					}

/*     compare distance to sum of radii */

					d2 = dist2_ (pijk, &face01_1.a[la * 3 - 3]);
					if (d2 <= sumnbr[lm - 1])
					{
						goto L110;
					}
				L90:
					lm = lknbr[lm - 1];
					goto L80;
				L100:

					/*     we have a new probe position */

					++face06_1.np;
					if (face06_1.np > 10000)
					{
						cerr << "Too many Probe Positions" << endl;
					}

					/*     mark three tori not buried */

					face04_1.ttbur[itt - 1] = false;
					face04_1.ttbur[ik - 1] = false;
					face04_1.ttbur[jk - 1] = false;

					/*     store probe center */

					for (k = 1; k <= 3; ++k)
					{
						face06_1.p[k + face06_1.np * 3 - 4] = pijk[k - 1];
					}

					/*     calculate vectors from probe to atom centers */

					if (face07_1.nv + 3 > 25000)
					{
						cerr << "Too many Vertices" << endl;
					}
					for (k = 1; k <= 3; ++k)
					{
						face07_1.v[k + (face07_1.nv + 1) * 3 - 4] = face01_1.a[k
																																	 + ia * 3 - 4] - face06_1.p[k + face06_1.np * 3 - 4];
						face07_1.v[k + (face07_1.nv + 2) * 3 - 4] = face01_1.a[k
																																	 + ja * 3 - 4] - face06_1.p[k + face06_1.np * 3 - 4];
						face07_1.v[k + (face07_1.nv + 3) * 3 - 4] = face01_1.a[k
																																	 + ka * 3 - 4] - face06_1.p[k + face06_1.np * 3 - 4];
					}

/*     calculate determinant of vectors defining triangle */

					det =
						face07_1.v[(face07_1.nv + 1) * 3 - 3] * face07_1.v[(face07_1.nv + 2) * 3 -
																															 2] * face07_1.v[(face07_1.nv + 3) * 3 - 1] +
						face07_1.v[(face07_1.nv + 2) * 3 - 3] * face07_1.v[(face07_1.nv + 3) * 3 -
																															 2] * face07_1.v[(face07_1.nv + 1) * 3 - 1] +
						face07_1.v[(face07_1.nv + 3) * 3 - 3] * face07_1.v[(face07_1.nv + 1) * 3 -
																															 2] * face07_1.v[(face07_1.nv + 2) * 3 - 1] -
						face07_1.v[(face07_1.nv + 3) * 3 - 3] * face07_1.v[(face07_1.nv + 2) * 3 -
																															 2] * face07_1.v[(face07_1.nv + 1) * 3 - 1] -
						face07_1.v[(face07_1.nv + 2) * 3 - 3] * face07_1.v[(face07_1.nv + 1) * 3 -
																															 2] * face07_1.v[(face07_1.nv + 3) * 3 - 1] -
						face07_1.v[(face07_1.nv + 1) * 3 - 3] * face07_1.v[(face07_1.nv + 3) * 3 -
																															 2] * face07_1.v[(face07_1.nv + 2) * 3 - 1];

/*     now add probe coordinates to vertices */

					for (k = 1; k <= 3; ++k)
					{
						face07_1.v[k + (face07_1.nv + 1) * 3 - 4] = face06_1.p[k
																																	 + face06_1.np * 3 - 4] + face07_1.v[k +
																																																			 (face07_1.nv +
																																																				1) * 3 -
																																																			 4] *
							face01_1.pr / (face01_1.ar[ia - 1] + face01_1.pr);
						face07_1.v[k + (face07_1.nv + 2) * 3 - 4] =
							face06_1.p[k + face06_1.np * 3 - 4] + face07_1.v[k + (face07_1.nv + 2) * 3 -
																															 4] * face01_1.pr / (face01_1.ar[ja - 1] + face01_1.pr);
						face07_1.v[k + (face07_1.nv + 3) * 3 - 4] =
							face06_1.p[k + face06_1.np * 3 - 4] + face07_1.v[k + (face07_1.nv + 3) * 3 -
																															 4] * face01_1.pr / (face01_1.ar[ka - 1] + face01_1.pr);
					}

/*     want the concave face to have counter-clockwise orienta
   tion */

					if (det > 0.0)
					{

						/*     swap second and third vertices */

						for (k = 1; k <= 3; ++k)
						{
							tempv[k - 1] = face07_1.v[k + (face07_1.nv + 2) * 3 - 4];
							face07_1.v[k + (face07_1.nv + 2) * 3 - 4] = face07_1.v[k + (face07_1.nv + 3) * 3 - 4];
							face07_1.v[k + (face07_1.nv + 3) * 3 - 4] = tempv[k - 1];
						}

						/*     set up pointers from probe to atoms */

						face06_1.pa[face06_1.np * 3 - 3] = ia;
						face06_1.pa[face06_1.np * 3 - 2] = ka;
						face06_1.pa[face06_1.np * 3 - 1] = ja;

						/*     set up pointers from vertices to atoms */

						face07_1.va[face07_1.nv] = ia;
						face07_1.va[face07_1.nv + 1] = ka;
						face07_1.va[face07_1.nv + 2] = ja;

						/*     insert concave edges into linked lists for appropriate tori */

						i__3 = face08_1.nen + 1;
						inedge_ (&i__3, &ik);
						i__3 = face08_1.nen + 2;
						inedge_ (&i__3, &jk);
						i__3 = face08_1.nen + 3;
						inedge_ (&i__3, &itt);
					}
					else
					{

/*     similarly, if face already counter clockwise */

						face06_1.pa[face06_1.np * 3 - 3] = ia;
						face06_1.pa[face06_1.np * 3 - 2] = ja;
						face06_1.pa[face06_1.np * 3 - 1] = ka;
						face07_1.va[face07_1.nv] = ia;
						face07_1.va[face07_1.nv + 1] = ja;
						face07_1.va[face07_1.nv + 2] = ka;
						i__3 = face08_1.nen + 1;
						inedge_ (&i__3, &itt);
						i__3 = face08_1.nen + 2;
						inedge_ (&i__3, &jk);
						i__3 = face08_1.nen + 3;
						inedge_ (&i__3, &ik);
					}

/*     set up pointers from vertices to probe */

					for (kv = 1; kv <= 3; ++kv)
					{
						face07_1.vp[face07_1.nv + kv - 1] = face06_1.np;
					}

/*     set up concave edges and concave face */

					if (face08_1.nen + 3 > 25000)
					{
						cerr << "Too many Concave Edges" << endl;
					}

/*     edges point to vertices */

					face08_1.env[(face08_1.nen + 1 << 1) - 2] = face07_1.nv + 1;
					face08_1.env[(face08_1.nen + 1 << 1) - 1] = face07_1.nv + 2;
					face08_1.env[(face08_1.nen + 2 << 1) - 2] = face07_1.nv + 2;
					face08_1.env[(face08_1.nen + 2 << 1) - 1] = face07_1.nv + 3;
					face08_1.env[(face08_1.nen + 3 << 1) - 2] = face07_1.nv + 3;
					face08_1.env[(face08_1.nen + 3 << 1) - 1] = face07_1.nv + 1;
					if (face08_1.nfn + 1 > 10000)
					{
						cerr << "Too many Concave Faces" << endl;
					}

/*     face points to edges */

					for (ke = 1; ke <= 3; ++ke)
					{
						face08_1.fnen[ke + (face08_1.nfn + 1) * 3 - 4] = face08_1.nen + ke;
					}

/*     increment counters for number of faces, edges and verti
   ces */

					++face08_1.nfn;
					face08_1.nen += 3;
					face07_1.nv += 3;
				L110:
					;
				}
			L120:
				;
			}
		L130:
			;
		}
		return 0;
	}	/* place_ */



/*     ################################################################ */

/*     ##                                                            ## */

/*     ##  subroutine inedge  --  manage linked list of torus edges  ## */

/*     ##                                                            ## */

/*     ################################################################ */


/*     "inedge" inserts a concave edge into the */

/*     linked list for its temporary torus */


	int
	inedge_ (int *ien, int *itt)
	{
		static int
			iepen;




/*     check for a serious error in the calling arguments */

		if (*ien <= 0)
		{
			cerr << "Bad Edge Number in INEDGE" << endl;
		}
		if (*itt <= 0)
		{
			cerr << "Bad Torus Number in INEDGE" << endl;
		}

/*     set beginning of list or add to end */

		if (face04_1.ttfe[*itt - 1] == 0)
		{
			face04_1.ttfe[*itt - 1] = *ien;
			face04_1.enext[*ien - 1] = 0;
			face04_1.ttle[*itt - 1] = *ien;
		}
		else
		{
			iepen = face04_1.ttle[*itt - 1];
			face04_1.enext[iepen - 1] = *ien;
			face04_1.enext[*ien - 1] = 0;
			face04_1.ttle[*itt - 1] = *ien;
		}
		return 0;
	}	/* inedge_ */



/*     ################################################################# */

/*     ##                                                             ## */

/*     ##  subroutine compress  --  condense temporary to final tori  ## */

/*     ##                                                             ## */

/*     ################################################################# */


/*     "compress" transfers only the non-buried tori from */

/*     the temporary tori arrays to the final tori arrays */


	int
	compress_ ()
	{
		/* System generated locals */
		int
			i__1;

		/* Local variables */
		static int
			iptr;
		static bool
			ttok;
		static int
			ia, ja;
		static int
			ned, itt;



		/*     initialize the number of nonburied tori */

		face05_1.nt = 0;
		if (face04_1.ntt <= 0)
		{
			return 0;
		}

		/*     if torus is free, then it is not buried; */
		/*     skip to end of loop if buried torus */

		i__1 = face04_1.ntt;
		for (itt = 1; itt <= i__1; ++itt)
		{
			if (face04_1.ttfree[itt - 1])
			{
				face04_1.ttbur[itt - 1] = false;
			}
			if (!face04_1.ttbur[itt - 1])
			{

				/*     first, transfer information */

				++face05_1.nt;
				if (face05_1.nt > 15000)
				{
					cerr << "Too many NonBuried Tori" << endl;
				}
				ia = face04_1.tta[(itt << 1) - 2];
				ja = face04_1.tta[(itt << 1) - 1];
				gettor_ (&ia, &ja, &ttok, &face05_1.t[face05_1.nt * 3 - 3], &face05_1.tr[face05_1.nt - 1],
								 &face05_1.tax[face05_1.nt * 3 - 3]);
				face05_1.ta[(face05_1.nt << 1) - 2] = ia;
				face05_1.ta[(face05_1.nt << 1) - 1] = ja;
				face05_1.tfree[face05_1.nt - 1] = face04_1.ttfree[itt - 1];
				face05_1.tfe[face05_1.nt - 1] = face04_1.ttfe[itt - 1];

				/*     special check for inconsistent probes */

				iptr = face05_1.tfe[face05_1.nt - 1];
				ned = 0;
				while (iptr != 0)
				{
					++ned;
					iptr = face04_1.enext[iptr - 1];
				}
				if (ned % 2 != 0)
				{
					iptr = face05_1.tfe[face05_1.nt - 1];
					while (iptr != 0)
					{
						cerr << "Odd Torus for Probes IP1 and IP2" << endl;
						iptr = face04_1.enext[iptr - 1];
					}
				}
			}
		}
		return 0;
	}	/* compress_ */



/*     ############################################################## */

/*     ##                                                          ## */

/*     ##  subroutine saddles  --  builds saddle pieces from tori  ## */

/*     ##                                                          ## */

/*     ############################################################## */


/*     "saddles" constructs circles, convex edges and saddle faces */


	int
	saddles_ ()
	{
		/* System generated locals */
		int
			i__1, i__2;
		double
			d__1;

		/* Local variables */
		static int
			ient;
		static double
			dtev;
		static int
			nent, itwo, k;
		static double
			teang[500];
		static int
			l1, l2, m1, n1, ia, in, ip;
		static double
			dt;
		static int
			it, iv;
		static double
			factor, atvect[3];
		static int
			nxtang[500];
		static bool
			sdstrt[500];
		static int
			ien, ten[500];
		static double
			tev[1500] /* was [3][500] */ ;


/*     zero the number of circles, convex edges and saddle faces */

		face09_1.nc = 0;
		face10_1.nep = 0;
		face11_1.nfs = 0;
		i__1 = face01_1.na;
		for (ia = 1; ia <= i__1; ++ia)
		{
			face10_1.afe[ia - 1] = 0;
			face10_1.ale[ia - 1] = 0;
			face02_1.abur[ia - 1] = true;
		}

/*     no saddle faces if no tori */

		if (face05_1.nt < 1)
		{
			return 0;
		}

/*     cycle through tori */

		i__1 = face05_1.nt;
		for (it = 1; it <= i__1; ++it)
		{
			if (face02_1.skip[face05_1.ta[(it << 1) - 2] - 1] && face02_1.skip[face05_1.ta[(it << 1) - 1] - 1])
			{
				goto L80;
			}

/*     set up two circles */

			for (in = 1; in <= 2; ++in)
			{
				ia = face05_1.ta[in + (it << 1) - 3];

/*     mark atom not buried */

				face02_1.abur[ia - 1] = false;

/*     vector from atom to torus center */

				for (k = 1; k <= 3; ++k)
				{
					atvect[k - 1] = face05_1.t[k + it * 3 - 4] - face01_1.a[k + ia * 3 - 4];
				}
				factor = face01_1.ar[ia - 1] / (face01_1.ar[ia - 1] + face01_1.pr);

/*     one more circle */

				++face09_1.nc;
				if (face09_1.nc > 25000)
				{
					cerr << "Too many Circles" << endl;
				}

/*     circle center */

				for (k = 1; k <= 3; ++k)
				{
					face09_1.c__[k + face09_1.nc * 3 - 4] = face01_1.a[k + ia * 3 - 4] + factor * atvect[k - 1];
				}

/*     pointer from circle to atom */

				face09_1.ca[face09_1.nc - 1] = ia;

/*     pointer from circle to torus */

				face09_1.ct[face09_1.nc - 1] = it;

/*     circle radius */

				face09_1.cr[face09_1.nc - 1] = factor * face05_1.tr[it - 1];
			}

/*     skip to special code if free torus */

			if (face05_1.tfree[it - 1])
			{
				goto L70;
			}

/*     now we collect all the concave edges for this torus; */

/*     for each concave edge, calculate vector from torus center */

/*     thru probe center and the angle relative to first such vector 
 */

/*     clear the number of concave edges for torus */

			nent = 0;

/*     pointer to start of linked list */

			ien = face05_1.tfe[it - 1];
		L10:

/*     finished if concave edge pointer is zero */

			if (ien <= 0)
			{
				goto L20;
			}

/*     one more concave edge */

			++nent;
			if (nent > 500)
			{
				cerr << "Too many Edges for Torus" << endl;
			}

/*     first vertex of edge */

			iv = face08_1.env[(ien << 1) - 2];

/*     probe number of vertex */

			ip = face07_1.vp[iv - 1];
			for (k = 1; k <= 3; ++k)
			{
				tev[k + nent * 3 - 4] = face06_1.p[k + ip * 3 - 4] - face05_1.t[k + it * 3 - 4];
			}
			dtev = 0.0;
			for (k = 1; k <= 3; ++k)
			{

				d__1 = tev[k + nent * 3 - 4];
				dtev += d__1 * d__1;
			}
			if (dtev <= 0.0)
			{
				cerr << "Probe on Torus Axis" << endl;
			}
			dtev = sqrt (dtev);
			for (k = 1; k <= 3; ++k)
			{
				tev[k + nent * 3 - 4] /= dtev;
			}

/*     store concave edge number */

			ten[nent - 1] = ien;
			if (nent > 1)
			{

/*     calculate angle between this vector and first vector */

				dt = 0.0;
				for (k = 1; k <= 3; ++k)
				{
					dt += tev[k - 1] * tev[k + nent * 3 - 4];
				}

/*     be careful */

				if (dt > 1.)
				{
					dt = 1.;
				}
				if (dt < -1.)
				{
					dt = -1.;
				}

/*     store angle */

				teang[nent - 1] = acos (dt);

/*     get the sign right */

				if (triple_ (tev, &tev[nent * 3 - 3], &face05_1.tax[it * 3 - 3]) < 0.0)
				{
					teang[nent - 1] = 2 * M_PI - teang[nent - 1];
				}
			}
			else
			{
				teang[0] = 0.0;
			}

/*     saddle face starts with this edge if it points parallel */

/*     to torus axis vector (which goes from first to second atom) */

			sdstrt[nent - 1] = face07_1.va[iv - 1] == face05_1.ta[(it << 1) - 2];

/*     next edge in list */

			ien = face04_1.enext[ien - 1];
			goto L10;
		L20:
			if (nent <= 0)
			{
				cerr << "No Edges for Non-free Torus" << endl;
			}
			itwo = 2;
			if (nent % itwo != 0)
			{
				cerr << "Odd Number of Edges for Torus" << endl;
			}

/*     set up linked list of concave edges in order */

/*     of increasing angle around the torus axis; */

/*     clear second linked (angle-ordered) list pointers */

			i__2 = nent;
			for (ient = 1; ient <= i__2; ++ient)
			{
				nxtang[ient - 1] = 0;
			}
			i__2 = nent;
			for (ient = 2; ient <= i__2; ++ient)
			{

				/*     we have an entry to put into linked list */

				/*     search for place to put it */

				l1 = 0;
				l2 = 1;
			L30:
				if (teang[ient - 1] < teang[l2 - 1])
				{
					goto L40;
				}

				/*     not yet, move along */

				l1 = l2;
				l2 = nxtang[l2 - 1];
				if (l2 != 0)
				{
					goto L30;
				}
			L40:

				/*     we are at end of linked list or between l1 and l2; */

				/*     insert edge */

				if (l1 <= 0)
				{
					cerr << "Logic Error in SADDLES" << endl;
				}
				nxtang[l1 - 1] = ient;
				nxtang[ient - 1] = l2;
			}

			/*     collect pairs of concave edges into saddles */

			/*     create convex edges while you're at it */

			l1 = 1;
		L50:
			if (l1 <= 0)
			{
				goto L60;
			}

			/*     check for start of saddle */

			if (sdstrt[l1 - 1])
			{

				/*     one more saddle face */

				++face11_1.nfs;
				if (face11_1.nfs > 15000)
				{
					cerr << "Too many Saddle Faces" << endl;
				}

				/*     get edge number */

				ien = ten[l1 - 1];

				/*     first concave edge of saddle */

				face11_1.fsen[(face11_1.nfs << 1) - 2] = ien;

				/*     one more convex edge */

				++face10_1.nep;
				if (face10_1.nep > 25000)
				{
					cerr << "Too many Convex Edges" << endl;
				}

				/*     first convex edge points to second circle */

				face10_1.epc[face10_1.nep - 1] = face09_1.nc;

				/*     atom circle lies on */

				ia = face09_1.ca[face09_1.nc - 1];

				/*     insert convex edge into linked list for atom */

				ipedge_ (&face10_1.nep, &ia);

				/*     first vertex of convex edge is second vertex of concave edge */

				face10_1.epv[(face10_1.nep << 1) - 2] = face08_1.env[(ien << 1) - 1];

				/*     first convex edge of saddle */

				face11_1.fsep[(face11_1.nfs << 1) - 2] = face10_1.nep;

				/*     one more convex edge */

				++face10_1.nep;
				if (face10_1.nep > 25000)
				{
					cerr << "Too many Convex Edges" << endl;
				}

				/*     second convex edge points to first circle */

				face10_1.epc[face10_1.nep - 1] = face09_1.nc - 1;
				ia = face09_1.ca[face09_1.nc - 2];

				/*     insert convex edge into linked list for atom */

				ipedge_ (&face10_1.nep, &ia);

				/*     second vertex of second convex edge */

				/*     is first vertex of first concave edge */

				face10_1.epv[(face10_1.nep << 1) - 1] = face08_1.env[(ien << 1) - 2];
				l1 = nxtang[l1 - 1];

				/*     wrap around */

				if (l1 <= 0)
				{
					l1 = 1;
				}
				if (sdstrt[l1 - 1])
				{
					m1 = nxtang[l1 - 1];
					if (m1 <= 0)
					{
						m1 = 1;
					}
					if (sdstrt[m1 - 1])
					{
						cerr << "Three Starts in a Row" << endl;
					}
					n1 = nxtang[m1 - 1];

					/*     the old switcheroo */

					nxtang[l1 - 1] = n1;
					nxtang[m1 - 1] = l1;
					l1 = m1;
				}
				ien = ten[l1 - 1];

				/*     second concave edge for saddle face */

				face11_1.fsen[(face11_1.nfs << 1) - 1] = ien;

				/*     second vertex of first convex edge is */

				/*     first vertex of second concave edge */

				face10_1.epv[(face10_1.nep - 1 << 1) - 1] = face08_1.env[(ien << 1) - 2];

				/*     first vertex of second convex edge is */

				/*     second vertex of second concave edge */

				face10_1.epv[(face10_1.nep << 1) - 2] = face08_1.env[(ien << 1) - 1];
				face11_1.fsep[(face11_1.nfs << 1) - 1] = face10_1.nep;

/*     quit if we have wrapped around to first edge */

				if (l1 == 1)
				{
					goto L60;
				}
			}

/*     next concave edge */

			l1 = nxtang[l1 - 1];
			goto L50;
		L60:
			goto L80;

/*     free torus */

		L70:

/*     set up entire circles as convex edges for new saddle surface; 
 */

/*     one more saddle face */

			++face11_1.nfs;
			if (face11_1.nfs > 15000)
			{
				cerr << "Too many Saddle Faces" << endl;
			}

/*     no concave edges for saddle */

			face11_1.fsen[(face11_1.nfs << 1) - 2] = 0;
			face11_1.fsen[(face11_1.nfs << 1) - 1] = 0;

/*     one more convex edge */

			++face10_1.nep;
			ia = face09_1.ca[face09_1.nc - 1];

/*     insert convex edge into linked list for atom */

			ipedge_ (&face10_1.nep, &ia);

/*     no vertices for convex edge */

			face10_1.epv[(face10_1.nep << 1) - 2] = 0;
			face10_1.epv[(face10_1.nep << 1) - 1] = 0;

/*     pointer from convex edge to second circle */

			face10_1.epc[face10_1.nep - 1] = face09_1.nc;

/*     first convex edge for saddle face */

			face11_1.fsep[(face11_1.nfs << 1) - 2] = face10_1.nep;

			/*     one more convex edge */

			++face10_1.nep;
			ia = face09_1.ca[face09_1.nc - 2];

			/*     insert second convex edge into linked list */

			ipedge_ (&face10_1.nep, &ia);

			/*     no vertices for convex edge */

			face10_1.epv[(face10_1.nep << 1) - 2] = 0;
			face10_1.epv[(face10_1.nep << 1) - 1] = 0;

			/*     convex edge points to first circle */

			face10_1.epc[face10_1.nep - 1] = face09_1.nc - 1;

			/*     second convex edge for saddle face */

			face11_1.fsep[(face11_1.nfs << 1) - 1] = face10_1.nep;

			/*     buried torus; do nothing with it */

		L80:
			;
		}
		return 0;
	}	/* saddles_ */







	/*     ################################################################# */
	/*     ##                                                             ## */
	/*     ##  subroutine ipedge  --  manage linked list of convex edges  ## */
	/*     ##                                                             ## */
	/*     ################################################################# */
	/*     "ipedge" inserts convex edge into linked list for atom */


	int ipedge_ (int *iep, int *ia)
	{
		static int
			iepen;


/*     first, check for an error condition */

		if (*iep <= 0)
		{
			cerr << "Bad Edge Number in IPEDGE" << endl;
		}
		if (*ia <= 0)
		{
			cerr << "Bad Atom Number in IPEDGE" << endl;
		}

/*     set beginning of list or add to end */

		if (face10_1.afe[*ia - 1] == 0)
		{
			face10_1.afe[*ia - 1] = *iep;
			face10_1.epnext[*iep - 1] = 0;
			face10_1.ale[*ia - 1] = *iep;
		}
		else
		{
			iepen = face10_1.ale[*ia - 1];
			face10_1.epnext[iepen - 1] = *iep;
			face10_1.epnext[*iep - 1] = 0;
			face10_1.ale[*ia - 1] = *iep;
		}
		return 0;
	}	/* ipedge_ */



	/*     ############################################################### */
	/*     ##                                                           ## */
	/*     ##  subroutine contact  --  builds exposed contact surfaces  ## */
	/*     ##                                                           ## */
	/*     ############################################################### */
	/*     "contact" constructs the contact surface, cycles and convex faces */


	int contact_ ()
	{
		/* System generated locals */
		int
			i__1, i__2, i__3, i__4, i__5;

		/* Local variables */
		static double
			anaa;
		static int
			iepa, jepa, nepa, icya, jcya, kcya;
		static double
			pole[3];
		static bool
			cycy[10000] /* was [100][100] */ ;
		static int
			i__, k;
		static bool
			samef[10000] /* was [100][100] */ ;
		static int
			cyepa[3000] /* was [30][100] */ , ncypa, icyep, ncyep, jcyep, nused;

		static int
			lookv, ia, ic, jc, av[600] /* was [2][300] */ , it;
		static double
			aavect[900] /* was [3][300] */ , factor;
		static int
			ncyepa[100];
		static bool
			epused[300];
		static int
			ncyold, ia2;
		static bool
			cyused[100];
		static double
			unvect[3];
		static int
			aic[300], aep[300];
		static int
			iep, jcy;


/*     zero out the number of cycles and convex faces */

		face12_1.ncy = 0;
		face13_1.nfp = 0;

/*     mark all free atoms not buried */

		i__1 = face01_1.na;
		for (ia = 1; ia <= i__1; ++ia)
		{
			if (face02_1.afree[ia - 1])
			{
				face02_1.abur[ia - 1] = false;
			}
		}

/*     go through all atoms */

		i__1 = face01_1.na;
		for (ia = 1; ia <= i__1; ++ia)
		{
			if (face02_1.skip[ia - 1])
			{
				goto L130;
			}

/*     skip to end of loop if buried atom */

			if (face02_1.abur[ia - 1])
			{
				goto L130;
			}

/*     special code for completely solvent-accessible atom */

			if (face02_1.afree[ia - 1])
			{
				goto L120;
			}

/*     gather convex edges for atom */

/*     clear number of convex edges for atom */

			nepa = 0;

/*     pointer to first edge */

			iep = face10_1.afe[ia - 1];
		L10:

/*     check whether finished gathering */

			if (iep <= 0)
			{
				goto L20;
			}

/*     one more edge */

			++nepa;
			if (nepa > 300)
			{
				cerr << "Too many Convex Edges for Atom" << endl;
			}

/*      store vertices of edge */

			av[(nepa << 1) - 2] = face10_1.epv[(iep << 1) - 2];
			av[(nepa << 1) - 1] = face10_1.epv[(iep << 1) - 1];

/*     store convex edge number */

			aep[nepa - 1] = iep;
			ic = face10_1.epc[iep - 1];

/*     store circle number */

			aic[nepa - 1] = ic;

/*     get neighboring atom */

			it = face09_1.ct[ic - 1];
			if (face05_1.ta[(it << 1) - 2] == ia)
			{
				ia2 = face05_1.ta[(it << 1) - 1];
			}
			else
			{
				ia2 = face05_1.ta[(it << 1) - 2];
			}


/*     vector from atom to circle center; also */

/*     vector from atom to center of neighboring atom */

/*     sometimes we use one vector, sometimes the other */

			for (k = 1; k <= 3; ++k)
			{
				aavect[k + nepa * 3 - 4] = face01_1.a[k + ia2 * 3 - 4] - face01_1.a[k + ia * 3 - 4];
			}

/*     pointer to next edge */

			iep = face10_1.epnext[iep - 1];
			goto L10;
		L20:
			if (nepa <= 0)
			{
				cerr << "No Edges for Non-buried, Non-free Atom" << endl;
			}


/*     form cycles; initialize all the */

/*     convex edges as not used in cycle */

			i__2 = nepa;
			for (iepa = 1; iepa <= i__2; ++iepa)
			{
				epused[iepa - 1] = false;
			}

/*     save old number of cycles */

			ncyold = face12_1.ncy;
			nused = 0;
			ncypa = 0;
		L30:

/*     look for starting edge */

			i__2 = nepa;
			for (iepa = 1; iepa <= i__2; ++iepa)
			{
				if (!epused[iepa - 1])
				{
					goto L40;
				}
			}

/*     cannot find starting edge, finished */

			goto L80;
		L40:

/*     pointer to edge */

			iep = aep[iepa - 1];

/*     one edge so far for this cycle */

			ncyep = 1;

/*     one more cycle for atom */

			++ncypa;
			if (ncypa > 100)
			{
				cerr << "Too many Cycles per Atom" << endl;
			}

/*     mark edge used in cycle */

			epused[iepa - 1] = true;
			++nused;

/*     one more cycle for molecule */

			++face12_1.ncy;
			if (face12_1.ncy > 5000)
			{
				cerr << "Too many Cycles" << endl;
			}

/*     index of edge in atom cycle array */

			cyepa[ncyep + ncypa * 30 - 31] = iepa;

/*     store in molecule cycle array a pointer to edge */

			face12_1.cyep[ncyep + face12_1.ncy * 30 - 31] = iep;

/*     second vertex of this edge is the vertex to look */

/*     for next as the first vertex of another edge */

			lookv = av[(iepa << 1) - 1];

/*     if no vertex, this cycle is finished */

			if (lookv <= 0)
			{
				goto L70;
			}
		L50:

/*     look for next connected edge */

			i__2 = nepa;
			for (jepa = 1; jepa <= i__2; ++jepa)
			{
				if (epused[jepa - 1])
				{
					goto L60;
				}

/*     check second vertex of iepa versus first vertex of jepa */

				if (av[(jepa << 1) - 2] != lookv)
				{
					goto L60;
				}

/*     edges are connected */

/*     pointer to edge */

				iep = aep[jepa - 1];

/*     one more edge for this cycle */

				++ncyep;
				if (ncyep > 30)
				{
					cerr << "Too many Edges per Cycle" << endl;
				}
				epused[jepa - 1] = true;
				++nused;

/*     store index in local edge array */

				cyepa[ncyep + ncypa * 30 - 31] = jepa;

/*     store pointer to edge */

				face12_1.cyep[ncyep + face12_1.ncy * 30 - 31] = iep;

/*     new vertex to look for */

				lookv = av[(jepa << 1) - 1];

/*     if no vertex, this cycle is in trouble */

				if (lookv <= 0)
				{
					cerr << "Pointer Error in Cycle" << endl;
				}
				goto L50;
			L60:
				;
			}

/*     it better connect to first edge of cycle */

			if (lookv != av[(iepa << 1) - 2])
			{
				cerr << "Cycle does not Close" << endl;
			}
		L70:

/*     this cycle is finished */

/*     store number of edges in cycle */

			ncyepa[ncypa - 1] = ncyep;
			face12_1.cynep[face12_1.ncy - 1] = ncyep;
			if (nused >= nepa)
			{
				goto L80;
			}

/*     look for more cycles */

			goto L30;
		L80:

/*     compare cycles for inside/outside relation; */

/*     check to see if cycle i is inside cycle j */

			i__2 = ncypa;
			for (icya = 1; icya <= i__2; ++icya)
			{
				i__3 = ncypa;
				for (jcya = 1; jcya <= i__3; ++jcya)
				{
					jcy = ncyold + jcya;

/*     initialize */

					cycy[icya + jcya * 100 - 101] = true;

/*     check for same cycle */

					if (icya == jcya)
					{
						goto L90;
					}

/*     if cycle j has two or fewer edges, nothing can */

/*     lie in its exterior; i is therefore inside j */

					if (ncyepa[jcya - 1] <= 2)
					{
						goto L90;
					}

/*     if cycles i and j have a pair of edges belonging */

/*     to the same circle, then they are outside each other */

					i__4 = ncyepa[icya - 1];
					for (icyep = 1; icyep <= i__4; ++icyep)
					{
						iepa = cyepa[icyep + icya * 30 - 31];
						ic = aic[iepa - 1];
						i__5 = ncyepa[jcya - 1];
						for (jcyep = 1; jcyep <= i__5; ++jcyep)
						{
							jepa = cyepa[jcyep + jcya * 30 - 31];
							jc = aic[jepa - 1];
							if (ic == jc)
							{
								cycy[icya + jcya * 100 - 101] = false;
								goto L90;
							}
						}
					}
					iepa = cyepa[icya * 30 - 30];
					anaa = anorm_ (&aavect[iepa * 3 - 3]);
					factor = face01_1.ar[ia - 1] / anaa;

/*     north pole and unit vector pointing south */

					for (k = 1; k <= 3; ++k)
					{
						pole[k - 1] = factor * aavect[k + iepa * 3 - 4] + face01_1.a[k + ia * 3 - 4];
						unvect[k - 1] = -aavect[k + iepa * 3 - 4] / anaa;
					}
					cycy[icya + jcya * 100 - 101] = ptincy_ (pole, unvect, &jcy);
				L90:
					;
				}
			}

/*     group cycles into faces; direct comparison for i and j */

			i__2 = ncypa;
			for (icya = 1; icya <= i__2; ++icya)
			{
				i__3 = ncypa;
				for (jcya = 1; jcya <= i__3; ++jcya)
				{

/*     tentatively say that cycles i and j bound */

/*     the same face if they are inside each other */

					samef[icya + jcya * 100 - 101] = cycy[icya + jcya * 100 - 101] && cycy[jcya + icya * 100 - 101];
				}
			}

/*     if i is in exterior of k, and k is in interior of */

/*     i and j, then i and j do not bound the same face */

			i__2 = ncypa;
			for (icya = 1; icya <= i__2; ++icya)
			{
				i__3 = ncypa;
				for (jcya = 1; jcya <= i__3; ++jcya)
				{
					if (icya != jcya)
					{
						i__4 = ncypa;
						for (kcya = 1; kcya <= i__4; ++kcya)
						{
							if (kcya != icya && kcya != jcya)
							{
								if (cycy[kcya + icya * 100 - 101] && cycy[kcya + jcya * 100 - 101] && !cycy[icya + kcya * 100 - 101])
								{
									samef[icya + jcya * 100 - 101] = false;
									samef[jcya + icya * 100 - 101] = false;
								}
							}
						}
					}
				}
			}

/*     fill gaps so that "samef" falls into complete blocks */

			i__2 = ncypa - 2;
			for (icya = 1; icya <= i__2; ++icya)
			{
				i__3 = ncypa - 1;
				for (jcya = icya + 1; jcya <= i__3; ++jcya)
				{
					if (samef[icya + jcya * 100 - 101])
					{
						i__4 = ncypa;
						for (kcya = jcya + 1; kcya <= i__4; ++kcya)
						{
							if (samef[jcya + kcya * 100 - 101])
							{
								samef[icya + kcya * 100 - 101] = true;
								samef[kcya + icya * 100 - 101] = true;
							}
						}
					}
				}
			}

/*     group cycles belonging to the same face */

			i__2 = ncypa;
			for (icya = 1; icya <= i__2; ++icya)
			{
				cyused[icya - 1] = false;
			}

/*     clear number of cycles used in bounding faces */

			nused = 0;
			i__2 = ncypa;
			for (icya = 1; icya <= i__2; ++icya)
			{

/*     check for already used */

				if (cyused[icya - 1])
				{
					goto L110;
				}

/*     one more convex face */

				++face13_1.nfp;
				if (face13_1.nfp > 5000)
				{
					cerr << "Too many Convex Faces" << endl;
				}

/*     clear number of cycles for face */

				face13_1.fpncy[face13_1.nfp - 1] = 0;

/*     pointer from face to atom */

				face13_1.fpa[face13_1.nfp - 1] = ia;

/*     look for all other cycles belonging to same face */

				i__3 = ncypa;
				for (jcya = 1; jcya <= i__3; ++jcya)
				{

/*     check for cycle already used in another face */

					if (cyused[jcya - 1])
					{
						goto L100;
					}

/*     cycles i and j belonging to same face */

					if (!samef[icya + jcya * 100 - 101])
					{
						goto L100;
					}

/*     mark cycle used */

					cyused[jcya - 1] = true;
					++nused;

/*     one more cycle for face */

					++face13_1.fpncy[face13_1.nfp - 1];
					if (face13_1.fpncy[face13_1.nfp - 1] > 10)
					{
						cerr << "Too many Cycles bounding Convex Face" << endl;
					}
					i__ = face13_1.fpncy[face13_1.nfp - 1];

/*     store cycle number */

					face13_1.fpcy[i__ + face13_1.nfp * 10 - 11] = ncyold + jcya;

/*     check for finished */

					if (nused >= ncypa)
					{
						goto L130;
					}
				L100:
					;
				}
			L110:
				;
			}

/*     should not fall through end of do loops */

			cerr << "Not all Cycles grouped into Convex Faces" << endl;
		L120:

/*     one face for free atom; no cycles */

			++face13_1.nfp;
			if (face13_1.nfp > 5000)
			{
				cerr << "Too many Convex Faces" << endl;
			}
			face13_1.fpa[face13_1.nfp - 1] = ia;
			face13_1.fpncy[face13_1.nfp - 1] = 0;
		L130:
			;
		}
		return 0;
	}	/* contact_ */



/*     ########################################################## */
/*     ##                                                      ## */
/*     ##  subroutine vam  --  volumes and areas of molecules  ## */
/*     ##                                                      ## */
/*     ########################################################## */


/*     "vam" takes the analytical molecular surface defined */
/*     as a collection of spherical and toroidal polygons   */
/*     and uses it to compute the volume and surface area   */


	int vam_ (double *volume, double *area, double* atom_areas)
	{
		/* System generated locals */
		int i__1, i__2, i__3;
		double d__1, d__2, d__3;

		/* Local variables */
		static bool badt[10000], alli, allj;
		static double cora[10000];
		static int nate, neat, nlap[10000], enfs[25000], idot;
		static double dota, alts[30000] /* was [3][10000] */ , corv[10000],
					 sdot[3], dotv[20], voln, dots[3000] /* was [3][1000] */ , vint, volp;
		static bool anyi;
		static double vols;
		static int nspt[30000] /* was [3][10000] */ ;
		static bool anyj, case1, case2;
		static double vpyr, vect1[3], vect2[3], vect3[3];
		static double vect4[3], vect5[3], vect6[3], vect7[3], vect8[3], xpnt1[3];
		static int k;
		static double xpnt2[3];
		static bool badav[10000];
		static double arean, areap, areas, fncen[30000] /* was [3][10000] */ ;
		static double scinc, alens, coran;
		static int ifnop[100];
		static double vcone;
		static double totan, voldo;
		static int ndots;
		static double vlens, totap, totas, prism, sumsc, corvn, vects[9]
						/* was [3][3] */ , tdots[3000] /* was [3][1000] */ ;
		static bool cinsp;
		static double volsp;
		static bool cintp;
		static double totvn, totvp, totvs;
		static int ispnd2[3], ia, ic, ke, ip;
		static double areado, dt;
		static int it, iv;
		static double rm;
		static int kv;
		static double uc[3], uq[3];
		static double areasp;
		static double hedron, alensn, sigmaq[3];
		static double fnvect[90000] /* was [3][3][10000] */ ;
		static int ispind[3];
		static double alenst, depths[10000];
		static int neatmx;
		static double sumlam;
		static double thetaq[3];
		static bool spindl;
		static int ke2;
		static bool fntrev[30000] /* was [3][10000] */ ;
		static double vlensn, ds2, totasp, vlenst, sumsig;
		static bool usenum;
		static int iv1, iv2;
		static double totvsp;
		static bool ate[100];
		static int ien, ifn, iep, ifp, ifs, isc, jfn, nop, iop, ivs[3], fnt[30000] /* was [3][10000] */ ;
		static double dpp, rat, rsc, rho, stq, tau[3], ppm[3], qij[3], upp[3], umq[3], upq[3], uij[3];
		static double dij2;


		/*     compute the volume of the interior polyhedron */

		hedron = 0.0;
		i__1 = face08_1.nfn;
		for (ifn = 1; ifn <= i__1; ++ifn)
		{
			measpm_ (&ifn, &prism);
			hedron += prism;
		}

		/*     compute the area and volume due to convex faces */
		/*     as well as the area partitioned among the atoms */

		totap = 0.0;
		totvp = 0.0;
		for (ia = 1; ia <= face01_1.na; ++ia)
		{
			atom_areas[ia - 1] = 0.0;
		}
		for (ifp = 1; ifp <= face13_1.nfp; ++ifp)
		{
			measfp_ (&ifp, &areap, &volp);
			ia = face13_1.fpa[ifp - 1];
			atom_areas[ia - 1] += areap;
			totap += areap;
			totvp += volp;
		}

		/*     compute the area and volume due to saddle faces */
		/*     as well as the spindle correction value */

		totas = 0.0;
		totvs = 0.0;
		totasp = 0.0;
		totvsp = 0.0;
		i__1 = face11_1.nfs;
		for (ifs = 1; ifs <= i__1; ++ifs)
		{
			for (k = 1; k <= 2; ++k)
			{
				ien = face11_1.fsen[k + (ifs << 1) - 3];
				if (ien > 0)
				{
					enfs[ien - 1] = ifs;
				}
			}
			// atom indices of the saddle face
			int a1, a2;
			measfs_ (&ifs, &areas, &vols, &areasp, &volsp, &a1, &a2);
			totas += areas;
			totvs += vols;
			totasp += areasp;
			totvsp += volsp;
			atom_areas[a1 - 1] += (areas + areasp) / 2.0;
			atom_areas[a2 - 1] += (areas + areasp) / 2.0;
			if (areas - areasp < 0.0)
			{
				cerr << "Negative Area for Saddle Face" << endl;
			}
		}

		/* compute the area and volume due to concave faces */
		totan = 0.0;
		totvn = 0.0;
		i__1 = face08_1.nfn;
		for (ifn = 1; ifn <= i__1; ++ifn)
		{
			// the indices of the three atoms
			int a1, a2, a3;
			measfn_ (&ifn, &arean, &voln, &a1, &a2, &a3);
			atom_areas[a1 - 1] += arean / 3.0;
			atom_areas[a2 - 1] += arean / 3.0;
			atom_areas[a3 - 1] += arean / 3.0;
			totan += arean;
			totvn += voln;
		}

		/*     compute the area and volume lens correction values */
		alenst = 0.0;
		alensn = 0.0;
		vlenst = 0.0;
		vlensn = 0.0;
		if (face01_1.pr <= 0.0)
		{
			goto L140;
		}
		ndots = 1000;
		gendot_ (&ndots, dots, &face01_1.pr, &c_b94, &c_b94, &c_b94);

		d__1 = face01_1.pr;
		dota = d__1 * d__1 * 12.566370614359172 / ndots;
		i__1 = face08_1.nfn;
		for (ifn = 1; ifn <= i__1; ++ifn)
		{
			nlap[ifn - 1] = 0;
			cora[ifn - 1] = 0.0;
			corv[ifn - 1] = 0.0;
			badav[ifn - 1] = false;
			badt[ifn - 1] = false;
			for (k = 1; k <= 3; ++k)
			{
				nspt[k + ifn * 3 - 4] = 0;
			}
			ien = face08_1.fnen[ifn * 3 - 3];
			iv = face08_1.env[(ien << 1) - 2];
			ip = face07_1.vp[iv - 1];
			depths[ifn - 1] = depth_ (&ip, &alts[ifn * 3 - 3]);
			for (k = 1; k <= 3; ++k)
			{
				fncen[k + ifn * 3 - 4] = face06_1.p[k + ip * 3 - 4];
			}
			ia = face07_1.va[iv - 1];

			/*     get vertices and vectors */

			for (ke = 1; ke <= 3; ++ke)
			{
				ien = face08_1.fnen[ke + ifn * 3 - 4];
				ivs[ke - 1] = face08_1.env[(ien << 1) - 2];
				ia = face07_1.va[ivs[ke - 1] - 1];
				ifs = enfs[ien - 1];
				iep = face11_1.fsep[(ifs << 1) - 2];
				ic = face10_1.epc[iep - 1];
				it = face09_1.ct[ic - 1];
				fnt[ke + ifn * 3 - 4] = it;
				fntrev[ke + ifn * 3 - 4] = face05_1.ta[(it << 1) - 2] != ia;
			}
			for (ke = 1; ke <= 3; ++ke)
			{
				for (k = 1; k <= 3; ++k)
				{
					vects[k + ke * 3 - 4] = face07_1.v[k + ivs[ke - 1] * 3 - 4] - face06_1.p[k + ip * 3 - 4];
				}
			}

			/*     calculate normal vectors for the three planes */
			/*     that cut out the geodesic triangle */

			vcross_ (vects, &vects[3], &fnvect[(ifn * 3 + 1) * 3 - 12]);
			vnorm_ (&fnvect[(ifn * 3 + 1) * 3 - 12], &fnvect[(ifn * 3 + 1) * 3 - 12]);
			vcross_ (&vects[3], &vects[6], &fnvect[(ifn * 3 + 2) * 3 - 12]);
			vnorm_ (&fnvect[(ifn * 3 + 2) * 3 - 12], &fnvect[(ifn * 3 + 2) * 3 - 12]);
			vcross_ (&vects[6], vects, &fnvect[(ifn * 3 + 3) * 3 - 12]);
			vnorm_ (&fnvect[(ifn * 3 + 3) * 3 - 12], &fnvect[(ifn * 3 + 3) * 3 - 12]);
		}
		i__1 = face08_1.nfn - 1;
		for (ifn = 1; ifn <= i__1; ++ifn)
		{
			i__2 = face08_1.nfn;
			for (jfn = ifn + 1; jfn <= i__2; ++jfn)
			{
				dij2 = dist2_ (&fncen[ifn * 3 - 3], &fncen[jfn * 3 - 3]);

				d__1 = face01_1.pr;
				if (dij2 > d__1 * d__1 * 4.)
				{
					goto L90;
				}
				if (depths[ifn - 1] > face01_1.pr && depths[jfn - 1] > face01_1.pr)
				{
					goto L90;
				}

				/*     these two probes may have intersecting surfaces */

				dpp = sqrt (dist2_ (&fncen[ifn * 3 - 3], &fncen[jfn * 3 - 3]));

				/*     compute the midpoint */

				for (k = 1; k <= 3; ++k)
				{
					ppm[k - 1] = (fncen[k + ifn * 3 - 4] + fncen[k + jfn * 3 - 4]) / 2.;
					upp[k - 1] = (fncen[k + jfn * 3 - 4] - fncen[k + ifn * 3 - 4]) / dpp;
				}

				d__1 = face01_1.pr;

				d__2 = dpp / 2.;
				rm = d__1 * d__1 - d__2 * d__2;
				if (rm < 0.0)
				{
					rm = 0.0;
				}
				rm = sqrt (rm);
				rat = dpp / (face01_1.pr * 2.);
				if (rat > 1.)
				{
					rat = 1.;
				}
				if (rat < -1.)
				{
					rat = -1.;
				}
				rho = asin (rat);

				/*     use circle-plane intersection routine */

				alli = true;
				anyi = false;
				spindl = false;
				for (k = 1; k <= 3; ++k)
				{
					ispind[k - 1] = 0;
					ispnd2[k - 1] = 0;
				}
				for (ke = 1; ke <= 3; ++ke)
				{
					thetaq[ke - 1] = 0.0;
					sigmaq[ke - 1] = 0.0;
					tau[ke - 1] = 0.0;
					cirpln_ (ppm, &rm, upp, &fncen[ifn * 3 - 3], &fnvect[(ke + ifn * 3) * 3 - 12], &cinsp, &cintp, xpnt1, xpnt2);
					if (!cinsp)
					{
						alli = false;
					}
					if (cintp)
					{
						anyi = true;
					}
					if (!cintp)
					{
						goto L10;
					}
					it = fnt[ke + ifn * 3 - 4];
					if (face05_1.tr[it - 1] > face01_1.pr)
					{
						goto L10;
					}
					for (ke2 = 1; ke2 <= 3; ++ke2)
					{
						if (it == fnt[ke2 + jfn * 3 - 4])
						{
							ispind[ke - 1] = it;
							++nspt[ke + ifn * 3 - 4];
							ispnd2[ke2 - 1] = it;
							++nspt[ke2 + jfn * 3 - 4];
							spindl = true;
						}
					}
					if (ispind[ke - 1] == 0)
					{
						goto L10;
					}

/*     check that the two ways of calculating */

/*     intersection points match */

					rat = face05_1.tr[it - 1] / face01_1.pr;
					if (rat > 1.)
					{
						rat = 1.;
					}
					if (rat < -1.)
					{
						rat = -1.;
					}
					thetaq[ke - 1] = acos (rat);
					stq = sin (thetaq[ke - 1]);
					if (fntrev[ke + ifn * 3 - 4])
					{
						for (k = 1; k <= 3; ++k)
						{
							uij[k - 1] = -face05_1.tax[k + it * 3 - 4];
						}
					}
					else
					{
						for (k = 1; k <= 3; ++k)
						{
							uij[k - 1] = face05_1.tax[k + it * 3 - 4];
						}
					}
					for (k = 1; k <= 3; ++k)
					{
						qij[k - 1] = face05_1.t[k + it * 3 - 4] - stq * face01_1.pr * uij[k - 1];
					}
					for (k = 1; k <= 3; ++k)
					{
						umq[k - 1] = (qij[k - 1] - ppm[k - 1]) / rm;
						upq[k - 1] = (qij[k - 1] - fncen[k + ifn * 3 - 4]) / face01_1.pr;
					}
					vcross_ (uij, upp, vect1);
					dt = dot_ (umq, vect1);
					if (dt > 1.)
					{
						dt = 1.;
					}
					if (dt < -1.)
					{
						dt = -1.;
					}
					sigmaq[ke - 1] = acos (dt);
					vcross_ (upq, &fnvect[(ke + ifn * 3) * 3 - 12], vect1);
					vnorm_ (vect1, uc);
					vcross_ (upp, upq, vect1);
					vnorm_ (vect1, uq);
					dt = dot_ (uc, uq);
					if (dt > 1.)
					{
						dt = 1.;
					}
					if (dt < -1.)
					{
						dt = -1.;
					}
					tau[ke - 1] = M_PI - acos (dt);
				L10:
					;
				}
				allj = true;
				anyj = false;
				for (ke = 1; ke <= 3; ++ke)
				{
					cirpln_ (ppm, &rm, upp, &fncen[jfn * 3 - 3], &fnvect[(ke + jfn * 3) * 3 - 12], &cinsp, &cintp, xpnt1, xpnt2);
					if (!cinsp)
					{
						allj = false;
					}
					if (cintp)
					{
						anyj = true;
					}
				}
				case1 = alli && allj && !anyi && !anyj;
				case2 = anyi && anyj && spindl;
				if (!case1 && !case2)
				{
					goto L90;
				}

/*     this kind of overlap can be handled */

				++nlap[ifn - 1];
				++nlap[jfn - 1];
				for (ke = 1; ke <= 3; ++ke)
				{
					ien = face08_1.fnen[ke + ifn * 3 - 4];
					iv1 = face08_1.env[(ien << 1) - 2];
					iv2 = face08_1.env[(ien << 1) - 1];
					for (k = 1; k <= 3; ++k)
					{
						vect3[k - 1] = face07_1.v[k + iv1 * 3 - 4] - fncen[k + ifn * 3 - 4];
						vect4[k - 1] = face07_1.v[k + iv2 * 3 - 4] - fncen[k + ifn * 3 - 4];
					}
					for (ke2 = 1; ke2 <= 3; ++ke2)
					{
						if (ispind[ke - 1] == ispnd2[ke2 - 1])
						{
							goto L40;
						}
						if (ispind[ke - 1] == 0)
						{
							goto L40;
						}
						cirpln_ (&fncen[ifn * 3 - 3], &face01_1.pr, &fnvect[(ke +
																																 ifn * 3) * 3 - 12], &fncen[jfn * 3 - 3],
										 &fnvect[(ke2 + jfn * 3) * 3 - 12], &cinsp, &cintp, xpnt1, xpnt2);
						if (!cintp)
						{
							goto L40;
						}
						ien = face08_1.fnen[ke2 + jfn * 3 - 4];
						iv1 = face08_1.env[(ien << 1) - 2];
						iv2 = face08_1.env[(ien << 1) - 1];
						for (k = 1; k <= 3; ++k)
						{
							vect7[k - 1] = face07_1.v[k + iv1 * 3 - 4] - fncen[k + jfn * 3 - 4];
							vect8[k - 1] = face07_1.v[k + iv2 * 3 - 4] - fncen[k + jfn * 3 - 4];
						}

/*     check whether point lies on spindle arc */

						for (k = 1; k <= 3; ++k)
						{
							vect1[k - 1] = xpnt1[k - 1] - fncen[k + ifn * 3 - 4];
							vect2[k - 1] = xpnt2[k - 1] - fncen[k + ifn * 3 - 4];
							vect5[k - 1] = xpnt1[k - 1] - fncen[k + jfn * 3 - 4];
							vect6[k - 1] = xpnt2[k - 1] - fncen[k + jfn * 3 - 4];
						}
						if (triple_ (vect3, vect1, &fnvect[(ke + ifn * 3) * 3 - 12]) < 0.0)
						{
							goto L20;
						}
						if (triple_ (vect1, vect4, &fnvect[(ke + ifn * 3) * 3 - 12]) < 0.0)
						{
							goto L20;
						}
						if (triple_ (vect7, vect5, &fnvect[(ke2 + jfn * 3) * 3 - 12]) < 0.0)
						{
							goto L20;
						}
						if (triple_ (vect5, vect8, &fnvect[(ke2 + jfn * 3) * 3 - 12]) < 0.0)
						{
							goto L20;
						}
						goto L30;
					L20:
						if (triple_ (vect3, vect2, &fnvect[(ke + ifn * 3) * 3 - 12]) < 0.0)
						{
							goto L40;
						}
						if (triple_ (vect2, vect4, &fnvect[(ke + ifn * 3) * 3 - 12]) < 0.0)
						{
							goto L40;
						}
						if (triple_ (vect7, vect6, &fnvect[(ke2 + jfn * 3) * 3 - 12]) < 0.0)
						{
							goto L40;
						}
						if (triple_ (vect6, vect8, &fnvect[(ke2 + jfn * 3) * 3 - 12]) < 0.0)
						{
							goto L40;
						}
					L30:
						badav[ifn - 1] = true;
					L40:
						;
					}
				}
				for (ke = 1; ke <= 3; ++ke)
				{
					ien = face08_1.fnen[ke + ifn * 3 - 4];
					iv1 = face08_1.env[(ien << 1) - 2];
					iv2 = face08_1.env[(ien << 1) - 1];
					for (k = 1; k <= 3; ++k)
					{
						vect3[k - 1] = face07_1.v[k + iv1 * 3 - 4] - fncen[k + ifn * 3 - 4];
						vect4[k - 1] = face07_1.v[k + iv2 * 3 - 4] - fncen[k + ifn * 3 - 4];
					}
					for (ke2 = 1; ke2 <= 3; ++ke2)
					{
						if (ispind[ke - 1] == ispnd2[ke2 - 1])
						{
							goto L70;
						}
						if (ispnd2[ke2 - 1] == 0)
						{
							goto L70;
						}
						cirpln_ (&fncen[jfn * 3 - 3], &face01_1.pr, &fnvect[(ke2 +
																																 jfn * 3) * 3 - 12], &fncen[ifn * 3 - 3],
										 &fnvect[(ke + ifn * 3) * 3 - 12], &cinsp, &cintp, xpnt1, xpnt2);
						if (!cintp)
						{
							goto L70;
						}
						ien = face08_1.fnen[ke2 + jfn * 3 - 4];
						iv1 = face08_1.env[(ien << 1) - 2];
						iv2 = face08_1.env[(ien << 1) - 1];
						for (k = 1; k <= 3; ++k)
						{
							vect7[k - 1] = face07_1.v[k + iv1 * 3 - 4] - fncen[k + jfn * 3 - 4];
							vect8[k - 1] = face07_1.v[k + iv2 * 3 - 4] - fncen[k + jfn * 3 - 4];
						}

/*     check whether point lies on spindle arc */

						for (k = 1; k <= 3; ++k)
						{
							vect1[k - 1] = xpnt1[k - 1] - fncen[k + ifn * 3 - 4];
							vect2[k - 1] = xpnt2[k - 1] - fncen[k + ifn * 3 - 4];
							vect5[k - 1] = xpnt1[k - 1] - fncen[k + jfn * 3 - 4];
							vect6[k - 1] = xpnt2[k - 1] - fncen[k + jfn * 3 - 4];
						}
						if (triple_ (vect3, vect1, &fnvect[(ke + ifn * 3) * 3 - 12]) < 0.0)
						{
							goto L50;
						}
						if (triple_ (vect1, vect4, &fnvect[(ke + ifn * 3) * 3 - 12]) < 0.0)
						{
							goto L50;
						}
						if (triple_ (vect7, vect5, &fnvect[(ke2 + jfn * 3) * 3 - 12]) < 0.0)
						{
							goto L50;
						}
						if (triple_ (vect5, vect8, &fnvect[(ke2 + jfn * 3) * 3 - 12]) < 0.0)
						{
							goto L50;
						}
						goto L60;
					L50:
						if (triple_ (vect3, vect2, &fnvect[(ke + ifn * 3) * 3 - 12]) < 0.0)
						{
							goto L70;
						}
						if (triple_ (vect2, vect4, &fnvect[(ke + ifn * 3) * 3 - 12]) < 0.0)
						{
							goto L70;
						}
						if (triple_ (vect7, vect6, &fnvect[(ke2 + jfn * 3) * 3 - 12]) < 0.0)
						{
							goto L70;
						}
						if (triple_ (vect6, vect8, &fnvect[(ke2 + jfn * 3) * 3 - 12]) < 0.0)
						{
							goto L70;
						}
					L60:
						badav[jfn - 1] = true;
					L70:
						;
					}
				}
				sumlam = 0.0;
				sumsig = 0.0;
				sumsc = 0.0;
				for (ke = 1; ke <= 3; ++ke)
				{
					if (ispind[ke - 1] != 0)
					{
						sumlam = sumlam + M_PI - tau[ke - 1];
						sumsig = sumsig + sigmaq[ke - 1] - M_PI;
						sumsc += sin (sigmaq[ke - 1]) * cos (sigmaq[ke - 1]);
					}
				}

				d__1 = face01_1.pr;
				alens = d__1 * d__1 * 2. * (M_PI - sumlam - sin (rho) * (sumsig + M_PI));
				vint = alens * face01_1.pr / 3.;

				d__1 = rm;
				vcone = face01_1.pr * (d__1 * d__1) * sin (rho) * (sumsig + M_PI) / 3.;

				d__1 = rm;
				vpyr = face01_1.pr * (d__1 * d__1) * sin (rho) * sumsc / 3.;
				vlens = vint - vcone + vpyr;
				cora[ifn - 1] += alens;
				cora[jfn - 1] += alens;
				corv[ifn - 1] += vlens;
				corv[jfn - 1] += vlens;

				/*     check for vertex on opposing probe in face */

				for (kv = 1; kv <= 3; ++kv)
				{
					ien = face08_1.fnen[kv + jfn * 3 - 4];
					iv = face08_1.env[(ien << 1) - 2];
					for (k = 1; k <= 3; ++k)
					{
						vect1[k - 1] = face07_1.v[k + iv * 3 - 4] - fncen[k + ifn * 3 - 4];
					}
					vnorm_ (vect1, vect1);
					for (ke = 1; ke <= 3; ++ke)
					{
						dt = dot_ (&fnvect[(ke + ifn * 3) * 3 - 12], &face07_1.v[iv * 3 - 3]);
						if (dt > 0.0)
						{
							goto L80;
						}
					}
				L80:
					;
				}
			L90:
				;
			}
		}
		i__1 = face08_1.nfn;
		for (ifn = 1; ifn <= i__1; ++ifn)
		{
			for (ke = 1; ke <= 3; ++ke)
			{
				if (nspt[ke + ifn * 3 - 4] > 1)
				{
					badt[ifn - 1] = true;
				}
			}
		}
		i__1 = face08_1.nfn;
		for (ifn = 1; ifn <= i__1; ++ifn)
		{
			if (nlap[ifn - 1] <= 0)
			{
				goto L130;
			}

			/*     gather all overlapping probes */
			nop = 0;
			i__2 = face08_1.nfn;
			for (jfn = 1; jfn <= i__2; ++jfn)
			{
				if (ifn != jfn)
				{
					dij2 = dist2_ (&fncen[ifn * 3 - 3], &fncen[jfn * 3 - 3]);

					d__1 = face01_1.pr;
					if (dij2 <= d__1 * d__1 * 4.)
					{
						if (depths[jfn - 1] <= face01_1.pr)
						{
							++nop;
							if (nop > 100)
							{
								cerr << "NOP Overflow in VAM" << endl;
							}
							ifnop[nop - 1] = jfn;
						}
					}
				}
			}

			/*     numerical calculation of the correction */

			areado = 0.0;
			voldo = 0.0;
			scinc = 0.050000000000000003;
			for (isc = 1; isc <= 20; ++isc)
			{
				rsc = isc - 0.5;

				d__1 = rsc;
				d__2 = scinc, d__3 = d__2;
				dotv[isc - 1] = face01_1.pr * dota * (d__1 * d__1) * (d__3 * (d__2 * d__2));
			}
			i__2 = nop;
			for (iop = 1; iop <= i__2; ++iop)
			{
				ate[iop - 1] = false;
			}
			neatmx = 0;
			i__2 = ndots;
			for (idot = 1; idot <= i__2; ++idot)
			{
				for (ke = 1; ke <= 3; ++ke)
				{
					dt = dot_ (&fnvect[(ke + ifn * 3) * 3 - 12], &dots[idot * 3 - 3]);
					if (dt > 0.0)
					{
						goto L120;
					}
				}
				for (k = 1; k <= 3; ++k)
				{
					tdots[k + idot * 3 - 4] = fncen[k + ifn * 3 - 4] + dots[k + idot * 3 - 4];
				}
				i__3 = nop;
				for (iop = 1; iop <= i__3; ++iop)
				{
					jfn = ifnop[iop - 1];
					ds2 = dist2_ (&tdots[idot * 3 - 3], &fncen[jfn * 3 - 3]);

					d__1 = face01_1.pr;
					if (ds2 < d__1 * d__1)
					{
						areado += dota;
						goto L100;
					}
				}
			L100:
				for (isc = 1; isc <= 20; ++isc)
				{
					rsc = isc - .5;
					for (k = 1; k <= 3; ++k)
					{
						sdot[k - 1] = fncen[k + ifn * 3 - 4] + rsc * scinc * dots[k + idot * 3 - 4];
					}
					neat = 0;
					i__3 = nop;
					for (iop = 1; iop <= i__3; ++iop)
					{
						jfn = ifnop[iop - 1];
						ds2 = dist2_ (sdot, &fncen[jfn * 3 - 3]);

						d__1 = face01_1.pr;
						if (ds2 < d__1 * d__1)
						{
							for (k = 1; k <= 3; ++k)
							{
								vect1[k - 1] = sdot[k - 1] - fncen[k + jfn * 3 - 4];
							}
							for (ke = 1; ke <= 3; ++ke)
							{
								dt = dot_ (&fnvect[(ke + jfn * 3) * 3 - 12], vect1);
								if (dt > 0.0)
								{
									goto L110;
								}
							}
							++neat;
							ate[iop - 1] = true;
						L110:
							;
						}
					}
					if (neat > neatmx)
					{
						neatmx = neat;
					}
					if (neat > 0)
					{
						voldo += dotv[isc - 1] * (neat / (neat + 1.));
					}
				}
			L120:
				;
			}
			coran = areado;
			corvn = voldo;
			nate = 0;
			i__2 = nop;
			for (iop = 1; iop <= i__2; ++iop)
			{
				if (ate[iop - 1])
				{
					++nate;
				}
			}

/*     use either the analytical or numerical correction */

			usenum = nate > nlap[ifn - 1] || neatmx > 1 || badt[ifn - 1];
			if (usenum)
			{
				cora[ifn - 1] = coran;
				corv[ifn - 1] = corvn;
				alensn += cora[ifn - 1];
				vlensn += corv[ifn - 1];
			}
			else if (badav[ifn - 1])
			{
				corv[ifn - 1] = corvn;
				vlensn += corv[ifn - 1];
			}
			alenst += cora[ifn - 1];
			vlenst += corv[ifn - 1];
		L130:
			;
		}
	L140:

		/*     finally, compute the total area and total volume */

		*area = totap + totas + totan - totasp - alenst;
		*volume = totvp + totvs + totvn + hedron - totvsp + vlenst;

		/*     print out the decomposition of the area and volume */

		return 0;
	}	/* vam_ */



/*     ###################### */

/*     ##                  ## */

/*     ##  function depth  ## */

/*     ##                  ## */

/*     ###################### */


	double
	depth_ (int *ip, double *alt)
	{
		/* System generated locals */
		double
			ret_val;

		/* Local variables */
		static double
			vect1[3], vect2[3], vect3[3], vect4[3];
		static int
			k;
		static int
			ia1, ia2, ia3;
		/* Parameter adjustments */
		--alt;

		/* Function Body */
		ia1 = face06_1.pa[*ip * 3 - 3];
		ia2 = face06_1.pa[*ip * 3 - 2];
		ia3 = face06_1.pa[*ip * 3 - 1];
		for (k = 1; k <= 3; ++k)
		{
			vect1[k - 1] = face01_1.a[k + ia1 * 3 - 4] - face01_1.a[k + ia3 * 3 - 4];
			vect2[k - 1] = face01_1.a[k + ia2 * 3 - 4] - face01_1.a[k + ia3 * 3 - 4];
			vect3[k - 1] = face06_1.p[k + *ip * 3 - 4] - face01_1.a[k + ia3 * 3 - 4];
		}
		vcross_ (vect1, vect2, vect4);
		vnorm_ (vect4, vect4);
		ret_val = dot_ (vect4, vect3);
		for (k = 1; k <= 3; ++k)
		{
			alt[k] = vect4[k - 1];
		}
		return ret_val;
	}	/* depth_ */



/*     ############################################################ */

/*     ##                                                        ## */

/*     ##  subroutine measpm  --  volume of interior polyhedron  ## */

/*     ##                                                        ## */

/*     ############################################################ */


/*     "measpm" computes the volume of a single prism section of */

/*     the full interior polyhedron */


	int
	measpm_ (int *ifn, double *prism)
	{
		static double
			vect1[3], vect2[3], vect3[3];
		static int
			k, ia, ke, ip, iv;
		static double
			height;
		static int
			ien;
		static double
			pav[9] /* was [3][3] */ ;

		height = 0.0;
		for (ke = 1; ke <= 3; ++ke)
		{
			ien = face08_1.fnen[ke + *ifn * 3 - 4];
			iv = face08_1.env[(ien << 1) - 2];
			ia = face07_1.va[iv - 1];
			height += face01_1.a[ia * 3 - 1];
			ip = face07_1.vp[iv - 1];
			for (k = 1; k <= 3; ++k)
			{
				pav[k + ke * 3 - 4] = face01_1.a[k + ia * 3 - 4] - face06_1.p[k + ip * 3 - 4];
			}
		}
		height /=
			3.;
		for (k = 1; k <= 3; ++k)
		{
			vect1[k - 1] = pav[k + 2] - pav[k - 1];
			vect2[k - 1] = pav[k + 5] - pav[k - 1];
		}
		vcross_ (vect1, vect2, vect3);
		*prism = height * vect3[2] / 2.;
		return 0;
	}	/* measpm_ */



/*     ######################### */

/*     ##                     ## */

/*     ##  subroutine measfp  ## */

/*     ##                     ## */

/*     ######################### */


	int
	measfp_ (int *ifp, double *areap, double *volp)
	{
		/* System generated locals */
		int
			i__1, i__2;
		double
			d__1;

		/* Local variables */
		static double
			tanv[180] /* was [3][2][30] */ , vect1[3], vect2[3];
		static int
			k, nedge;
		static double
			angle, gauss;
		static int
			ia, ic, ke;
		static double
			dt, radial[90] /* was [3][30] */ ;
		static int
			it;
		static double
			acvect[3], aavect[3];
		static int
			ncycle, ieuler;
		static double
			gcurve;
		static int
			ia2;
		static double
			pcurve;
		static int
			icyptr;
		static int
			iv1, iv2;
		static double
			geo;
		static int
			iep, icy;

		ia = face13_1.fpa[*ifp - 1];
		pcurve = 0.0;
		gcurve = 0.0;
		ncycle = face13_1.fpncy[*ifp - 1];
		if (ncycle > 0)
		{
			ieuler = 2 - ncycle;
		}
		else
		{
			ieuler = 2;
		}
		i__1 = ncycle;
		for (icyptr = 1; icyptr <= i__1; ++icyptr)
		{
			icy = face13_1.fpcy[icyptr + *ifp * 10 - 11];
			nedge = face12_1.cynep[icy - 1];
			i__2 = nedge;
			for (ke = 1; ke <= i__2; ++ke)
			{
				iep = face12_1.cyep[ke + icy * 30 - 31];
				ic = face10_1.epc[iep - 1];
				it = face09_1.ct[ic - 1];
				if (ia == face05_1.ta[(it << 1) - 2])
				{
					ia2 = face05_1.ta[(it << 1) - 1];
				}
				else
				{
					ia2 = face05_1.ta[(it << 1) - 2];
				}
				for (k = 1; k <= 3; ++k)
				{
					acvect[k - 1] = face09_1.c__[k + ic * 3 - 4] - face01_1.a[k + ia * 3 - 4];
					aavect[k - 1] = face01_1.a[k + ia2 * 3 - 4] - face01_1.a[k + ia * 3 - 4];
				}
				vnorm_ (aavect, aavect);
				dt = dot_ (acvect, aavect);
				geo = -dt / (face01_1.ar[ia - 1] * face09_1.cr[ic - 1]);
				iv1 = face10_1.epv[(iep << 1) - 2];
				iv2 = face10_1.epv[(iep << 1) - 1];
				if (iv1 == 0 || iv2 == 0)
				{
					angle = 2 * M_PI;
				}
				else
				{
					for (k = 1; k <= 3; ++k)
					{
						vect1[k - 1] = face07_1.v[k + iv1 * 3 - 4] - face09_1.c__[k + ic * 3 - 4];
						vect2[k - 1] = face07_1.v[k + iv2 * 3 - 4] - face09_1.c__[k + ic * 3 - 4];
						radial[k + ke * 3 - 4] = face07_1.v[k + iv1 * 3 - 4] - face01_1.a[k + ia * 3 - 4];
					}
					vnorm_ (&radial[ke * 3 - 3], &radial[ke * 3 - 3]);
					vcross_ (vect1, aavect, &tanv[((ke << 1) + 1) * 3 - 9]);
					vnorm_ (&tanv[((ke << 1) + 1) * 3 - 9], &tanv[((ke << 1) + 1) * 3 - 9]);
					vcross_ (vect2, aavect, &tanv[((ke << 1) + 2) * 3 - 9]);
					vnorm_ (&tanv[((ke << 1) + 2) * 3 - 9], &tanv[((ke << 1) + 2) * 3 - 9]);
					angle = vecang_ (vect1, vect2, aavect, &c_b157);
				}
				gcurve += face09_1.cr[ic - 1] * angle * geo;
				if (nedge != 1)
				{
					if (ke > 1)
					{
						angle = vecang_ (&tanv[((ke - 1 << 1) + 2) * 3 - 9], &tanv[
																																			 ((ke << 1) + 1) * 3 - 9], &radial[ke * 3 - 3],
														 &c_b158);
						if (angle < 0.0)
						{
							cerr << "Negative Angle in MEASFP" << endl;
						}
						pcurve += angle;
					}
				}
			}
			if (nedge > 1)
			{
				angle = vecang_ (&tanv[((nedge << 1) + 2) * 3 - 9], tanv, radial, &c_b158);
				if (angle < 0.0)
				{
					cerr << "Negative Angle in MEASFP" << endl;
				}
				pcurve += angle;
			}
		}
		gauss = ieuler * 2 * M_PI - pcurve - gcurve;

		d__1 = face01_1.ar[ia - 1];
		*areap = gauss * (d__1 * d__1);
		*volp = *areap * face01_1.ar[ia - 1] / 3.;
		return 0;
	}	/* measfp_ */



	/*     ######################### */
	/*     ##                     ## */
	/*     ##  subroutine measfs  ## */
	/*     ##                     ## */
	/*	   ######################### */


	int measfs_ (int *ifs, double *areas, double *vols, double *areasp, double *volsp, int* a1, int* a2)
	{
		/* System generated locals */
		double d__1, d__2;


		/* Local variables */
		static double spin;
		static bool cusp;
		static double volt, cone1, cone2, vect1[3], vect2[3], term1, term2, term3;
		static int k;
		static double d1, d2;
		static double w1, w2, theta1, theta2;
		static int ic, it;
		static double aavect[3], thetaq;
		static int ia1, ia2, ic1, ic2, iv1, iv2, iep;
		static double phi;
		static double rat;



		iep = face11_1.fsep[(*ifs << 1) - 2];
		ic = face10_1.epc[iep - 1];
		it = face09_1.ct[ic - 1];
		*a1 = ia1 = face05_1.ta[(it << 1) - 2];
		*a2 = ia2 = face05_1.ta[(it << 1) - 1];
		for (k = 1; k <= 3; ++k)
		{
			aavect[k - 1] = face01_1.a[k + ia2 * 3 - 4] - face01_1.a[k + ia1 * 3 - 4];
		}
		vnorm_ (aavect, aavect);
		iv1 = face10_1.epv[(iep << 1) - 2];
		iv2 = face10_1.epv[(iep << 1) - 1];
		if (iv1 == 0 || iv2 == 0)
		{
			phi = 2 * M_PI;
		}
		else
		{
			for (k = 1; k <= 3; ++k)
			{
				vect1[k - 1] = face07_1.v[k + iv1 * 3 - 4] - face09_1.c__[k + ic * 3 - 4];
				vect2[k - 1] = face07_1.v[k + iv2 * 3 - 4] - face09_1.c__[k + ic * 3 - 4];
			}
			phi = vecang_ (vect1, vect2, aavect, &c_b158);
		}
		for (k = 1; k <= 3; ++k)
		{
			vect1[k - 1] = face01_1.a[k + ia1 * 3 - 4] - face05_1.t[k + it * 3 - 4];
			vect2[k - 1] = face01_1.a[k + ia2 * 3 - 4] - face05_1.t[k + it * 3 - 4];
		}
		d1 = -dot_ (vect1, aavect);
		d2 = dot_ (vect2, aavect);
		theta1 = atan2 (d1, face05_1.tr[it - 1]);
		theta2 = atan2 (d2, face05_1.tr[it - 1]);

/*     check for cusps */

		if (face05_1.tr[it - 1] < face01_1.pr && theta1 > 0.0 && theta2 > 0.0)
		{
			cusp = true;
			rat = face05_1.tr[it - 1] / face01_1.pr;
			if (rat > 1.)
			{
				rat = 1.;
			}
			if (rat < -1.)
			{
				rat = -1.;
			}
			thetaq = acos (rat);
		}
		else
		{
			cusp = false;
			thetaq = 0.0;
			*areasp = 0.0;
			*volsp = 0.0;
		}
		term1 = face05_1.tr[it - 1] * face01_1.pr * (theta1 + theta2);

		d__1 = face01_1.pr;
		term2 = d__1 * d__1 * (sin (theta1) + sin (theta2));
		*areas = phi * (term1 - term2);
		if (cusp)
		{

			d__1 = face01_1.pr;
			spin = face05_1.tr[it - 1] * face01_1.pr * thetaq - d__1 * d__1 * sin (thetaq);
			*areasp = phi * 2. * spin;
		}

		iep = face11_1.fsep[(*ifs << 1) - 2];
		ic2 = face10_1.epc[iep - 1];
		iep = face11_1.fsep[(*ifs << 1) - 1];
		ic1 = face10_1.epc[iep - 1];
		if (face09_1.ca[ic1 - 1] != ia1)
		{
			cerr << "IA1 Inconsistency in MEASFS" << endl;
		}
		for (k = 1; k <= 3; ++k)
		{
			vect1[k - 1] = face09_1.c__[k + ic1 * 3 - 4] - face01_1.a[k + ia1 * 3 - 4];
			vect2[k - 1] = face09_1.c__[k + ic2 * 3 - 4] - face01_1.a[k + ia2 * 3 - 4];
		}
		w1 = dot_ (vect1, aavect);
		w2 = -dot_ (vect2, aavect);

		d__1 = face09_1.cr[ic1 - 1];
		cone1 = phi * (w1 * (d__1 * d__1)) / 6.;

		d__1 = face09_1.cr[ic2 - 1];
		cone2 = phi * (w2 * (d__1 * d__1)) / 6.;

		d__1 = face05_1.tr[it - 1];
		term1 = d__1 * d__1 * face01_1.pr * (sin (theta1) + sin (theta2));
		term2 = sin (theta1) * cos (theta1) + theta1 + sin (theta2) * cos (theta2) + theta2;

		d__1 = face01_1.pr;
		term2 = face05_1.tr[it - 1] * (d__1 * d__1) * term2;

		d__1 = cos (theta1);

		d__2 = cos (theta2);
		term3 = sin (theta1) * (d__1 * d__1) + sin (theta1) * 2. + sin (theta2) * (d__2 * d__2) + sin (theta2) * 2.;

/* Computing 3rd power */
		d__1 = face01_1.pr, d__2 = d__1;
		term3 = d__2 * (d__1 * d__1) / 3. * term3;
		volt = phi / 2. * (term1 - term2 + term3);
		*vols = volt + cone1 + cone2;
		if (cusp)
		{

			d__1 = face05_1.tr[it - 1];
			term1 = d__1 * d__1 * face01_1.pr * sin (thetaq);
			term2 = sin (thetaq) * cos (thetaq) + thetaq;

			d__1 = face01_1.pr;
			term2 = face05_1.tr[it - 1] * (d__1 * d__1) * term2;

			d__1 = cos (thetaq);
			term3 = sin (thetaq) * (d__1 * d__1) + sin (thetaq) * 2.;

			d__1 = face01_1.pr, d__2 = d__1;
			term3 = d__2 * (d__1 * d__1) / 3. * term3;
			*volsp = phi * (term1 - term2 + term3);
		}
		return 0;
	}	/* measfs_ */



	/*     ######################### */
	/*     ##                     ## */
	/*     ##  subroutine measfn  ## */
	/*     ##                     ## */
	/*     ######################### */

	// calculate concave surface area/volume
	int measfn_ (int *ifn, double *arean, double *voln, int* a1, int* a2, int* a3)
	{
		/* System generated locals */
		double d__1;

		/* Local variables */
		static int k;
		static double angle[3];
		static int ia, ke, je, ip;
		static double defect;
		static int iv;
		static double planev[9] /* was [3][3] */ ;
		static double simplx;
		static int ien;
		static double pav[9] /* was [3][3] */ , pvv[9]	/* was [3][3] */ ;

		for (ke = 1; ke <= 3; ++ke)
		{
			ien = face08_1.fnen[ke + *ifn * 3 - 4];
			iv = face08_1.env[(ien << 1) - 2];
			ia = face07_1.va[iv - 1];
			// store the atom indices
			switch (ke)
			{	
				case 1: *a1 = ia ; break;
				case 2: *a2 = ia ; break;
				case 3: *a3 = ia ; break;
			}
			ip = face07_1.vp[iv - 1];
			for (k = 1; k <= 3; ++k)
			{
				pvv[k + ke * 3 - 4] = face07_1.v[k + iv * 3 - 4] - face06_1.p[k + ip * 3 - 4];
				pav[k + ke * 3 - 4] = face01_1.a[k + ia * 3 - 4] - face06_1.p[k + ip * 3 - 4];
			}
			if (face01_1.pr > 0.0)
			{
				vnorm_ (&pvv[ke * 3 - 3], &pvv[ke * 3 - 3]);
			}
		}
		if (face01_1.pr <= 0.0)
		{
			*arean = 0.0;
		}
		else
		{
			for (ke = 1; ke <= 3; ++ke)
			{
				je = ke + 1;
				if (je > 3)
				{
					je = 1;
				}
				vcross_ (&pvv[ke * 3 - 3], &pvv[je * 3 - 3], &planev[ke * 3 - 3]);
				vnorm_ (&planev[ke * 3 - 3], &planev[ke * 3 - 3]);
			}
			for (ke = 1; ke <= 3; ++ke)
			{
				je = ke - 1;
				if (je < 1)
				{
					je = 3;
				}
				angle[ke - 1] = vecang_ (&planev[je * 3 - 3], &planev[ke * 3 - 3], &pvv[ke * 3 - 3], &c_b157);
				if (angle[ke - 1] < 0.0)
				{
					cerr << "Negative Angle in MEASFN" << endl;
				}
			}
			defect = 2 * M_PI - (angle[0] + angle[1] + angle[2]);

			d__1 = face01_1.pr;
			*arean = d__1 * d__1 * defect;
		}
		simplx = -triple_ (pav, &pav[3], &pav[6]) / 6.;
		*voln = simplx - *arean * face01_1.pr / 3.;
		return 0;
	}	/* measfn_ */



/*     ######################### */

/*     ##                     ## */

/*     ##  subroutine projct  ## */

/*     ##                     ## */

/*     ######################### */


	int
	projct_ (double *pnt, double *unvect, int *icy, int *ia, double *spv, int *nedge, bool * fail)
	{
		/* System generated locals */
		int
			i__1;

		/* Local variables */
		static double
			f;
		static int
			k;
		static double
			polev[3];
		static int
			ke;
		static double
			dt;
		static int
			iv, iep;



		/* Parameter adjustments */
		spv -= 4;
		--unvect;
		--pnt;

		/* Function Body */
		*fail = false;
		*nedge = face12_1.cynep[*icy - 1];
		i__1 = face12_1.cynep[*icy - 1];
		for (ke = 1; ke <= i__1; ++ke)
		{

/*     vertex number (use first vertex of edge) */

			iep = face12_1.cyep[ke + *icy * 30 - 31];
			iv = face10_1.epv[(iep << 1) - 2];
			if (iv != 0)
			{

/*     vector from north pole to vertex */

				for (k = 1; k <= 3; ++k)
				{
					polev[k - 1] = face07_1.v[k + iv * 3 - 4] - pnt[k];
				}

/*     calculate multiplication factor */

				dt =
				dot_ (polev, &unvect[1]);
				if (dt == 0.0)
				{
					*fail = true;
					return 0;
				}
				f = face01_1.ar[*ia - 1] * 2 / dt;
				if (f < 1.)
				{
					*fail = true;
					return 0;
				}

/*     projected vertex for this convex edge */

				for (k = 1; k <= 3; ++k)
				{
					spv[k + ke * 3] = pnt[k] + f * polev[k - 1];
				}
			}
		}
		return 0;
	}	/* projct_ */



/*     ####################### */

/*     ##                   ## */

/*     ##  function ptincy  ## */

/*     ##                   ## */

/*     ####################### */


	bool
	ptincy_ (double *pnt, double *unvect, int *icy)
	{
		/* System generated locals */
		int
			i__1;
		bool
			ret_val;

		/* Local variables */
		static bool
			fail;
		static int
			k, nedge, iaoth, iatom, ic, ke, it;
		static double
			acvect[3];
		static double
			cpvect[3];
		static double
			totang;
		static int
			iep;
		static double
			epu[90] /* was [3][30] */ , spv[90]	/* was [3][30]
																					 */ ;




/*     check for eaten by neighbor */

		/* Parameter adjustments */
		--unvect;
		--pnt;

		/* Function Body */
		i__1 = face12_1.cynep[*icy - 1];
		for (ke = 1; ke <= i__1; ++ke)
		{
			iep = face12_1.cyep[ke + *icy * 30 - 31];
			ic = face10_1.epc[iep - 1];
			it = face09_1.ct[ic - 1];
			iatom = face09_1.ca[ic - 1];
			if (face05_1.ta[(it << 1) - 2] == iatom)
			{
				iaoth = face05_1.ta[(it << 1) - 1];
			}
			else
			{
				iaoth = face05_1.ta[(it << 1) - 2];
			}
			for (k = 1; k <= 3; ++k)
			{
				acvect[k - 1] = face01_1.a[k + iaoth * 3 - 4] - face01_1.a[k + iatom * 3 - 4];
				cpvect[k - 1] = pnt[k] - face09_1.c__[k + ic * 3 - 4];
			}
			if (dot_ (acvect, cpvect) >= 0.0)
			{
				ret_val = false;
				return ret_val;
			}
		}
		if (face12_1.cynep[*icy - 1] <= 2)
		{
			ret_val = true;
			return ret_val;
		}
		projct_ (&pnt[1], &unvect[1], icy, &iatom, spv, &nedge, &fail);
		if (fail)
		{
			ret_val = true;
			return ret_val;
		}
		epuclc_ (spv, &nedge, epu);
		totang = rotang_ (epu, &nedge, &unvect[1]);
		ret_val = totang > 0.0;
		return ret_val;
	}	/* ptincy_ */



/*     ######################### */

/*     ##                     ## */

/*     ##  subroutine epuclc  ## */

/*     ##                     ## */

/*     ######################### */


	int
	epuclc_ (double *spv, int *nedge, double *epu)
	{
		/* System generated locals */
		int
			i__1;

		/* Local variables */
		static double
			epun;
		static int
			k;
		static int
			ke, le, ke2;



/*     calculate unit vectors along edges */

		/* Parameter adjustments */
		epu -= 4;
		spv -= 4;

		/* Function Body */
		i__1 = *nedge;
		for (ke = 1; ke <= i__1; ++ke)
		{

/*     get index of second edge of corner */

			if (ke < *nedge)
			{
				ke2 = ke + 1;
			}
			else
			{
				ke2 = 1;
			}

			/*     unit vector along edge of cycle */

			for (k = 1; k <= 3; ++k)
			{
				epu[k + ke * 3] = spv[k + ke2 * 3] - spv[k + ke * 3];
			}
			epun = anorm_ (&epu[ke * 3 + 1]);

			/*        if (epun .le. 0.0d0)  call error ('Null Edge in Cycle') */

			/*     normalize */

			if (epun > 0.0)
			{
				for (k = 1; k <= 3; ++k)
				{
					epu[k + ke * 3] /= epun;
				}
			}
			else
			{
				for (k = 1; k <= 3; ++k)
				{
					epu[k + ke * 3] = 0.0;
				}
			}
		}

/*     vectors for null edges come from following or preceding edges */

		i__1 = *nedge;
		for (ke = 1; ke <= i__1; ++ke)
		{
			if (anorm_ (&epu[ke * 3 + 1]) <= 0.0)
			{
				le = ke - 1;
				if (le <= 0)
				{
					le = *nedge;
				}
				for (k = 1; k <= 3; ++k)
				{
					epu[k + ke * 3] = epu[k + le * 3];
				}
			}
		}
		return 0;
	}	/* epuclc_ */



/*     ####################### */

/*     ##                   ## */

/*     ##  function rotang  ## */

/*     ##                   ## */

/*     ####################### */


	double
	rotang_ (double *epu, int *nedge, double *unvect)
	{
		/* System generated locals */
		int
			i__1;
		double
			ret_val;

		/* Local variables */
		static int
			ke;
		static double
			dt, totang;
		static double
			ang;
		static double
			crs[3];



		/* Parameter adjustments */
		epu -= 4;
		--unvect;

		/* Function Body */
		totang = 0.0;

/*     sum angles at vertices of cycle */

		i__1 = *nedge;
		for (ke = 1; ke <= i__1; ++ke)
		{
			if (ke < *nedge)
			{
				dt = dot_ (&epu[ke * 3 + 1], &epu[(ke + 1) * 3 + 1]);
				vcross_ (&epu[ke * 3 + 1], &epu[(ke + 1) * 3 + 1], crs);
			}
			else
			{

/*     closing edge of cycle */

				dt = dot_ (&epu[ke * 3 + 1], &epu[4]);
				vcross_ (&epu[ke * 3 + 1], &epu[4], crs);
			}
			if (dt < -1.)
			{
				dt = -1.;
			}
			if (dt > 1.)
			{
				dt = 1.;
			}
			ang = acos (dt);
			if (dot_ (crs, &unvect[1]) > 0.0)
			{
				ang = -ang;
			}

/*     add to total for cycle */

			totang += ang;
		}
		ret_val = totang;
		return ret_val;
	}	/* rotang_ */



/*     ######################### */

/*     ##                     ## */

/*     ##  subroutine vcross  ## */

/*     ##                     ## */

/*     ######################### */


/*     "vcross" finds the cross product of two vectors */


	int
	vcross_ (double *x, double *y, double *z__)
	{


		/* Parameter adjustments */
		--z__;
		--y;
		--x;

		/* Function Body */
		z__[1] = x[2] * y[3] - x[3] * y[2];
		z__[2] = x[3] * y[1] - x[1] * y[3];
		z__[3] = x[1] * y[2] - x[2] * y[1];
		return 0;
	}															/* vcross_ */



/*     #################### */

/*     ##                ## */

/*     ##  function dot  ## */

/*     ##                ## */

/*     #################### */


/*     "dot" finds the dot product of two vectors */


	double
	dot_ (double *x, double *y)
	{
		/* System generated locals */
		double
			ret_val;



		/* Parameter adjustments */
		--y;
		--x;

		/* Function Body */
		ret_val = x[1] * y[1] + x[2] * y[2] + x[3] * y[3];
		return ret_val;
	}															/* dot_ */



/*     ####################################################### */

/*     ##                                                   ## */

/*     ##  function anorm  --  find the length of a vector  ## */

/*     ##                                                   ## */

/*     ####################################################### */


/*     "anorm" finds the norm (length) of a vector; used as a */

/*     service routine by the Connolly surface area and volume */

/*     computation */


	double
	anorm_ (double *x)
	{
		/* System generated locals */
		double
			ret_val, d__1, d__2, d__3;




		/* Parameter adjustments */
		--x;

		/* Function Body */

		d__1 = x[1];

		d__2 = x[2];

		d__3 = x[3];
		ret_val = d__1 * d__1 + d__2 * d__2 + d__3 * d__3;
		if (ret_val < 0.0)
		{
			ret_val = 0.0;
		}
		ret_val =
		sqrt (ret_val);
		return ret_val;
	}	/* anorm_ */



/*     ############################################################### */

/*     ##                                                           ## */

/*     ##  subroutine vnorm  --  normalize a vector to unit length  ## */

/*     ##                                                           ## */

/*     ############################################################### */


/*     "vnorm" normalizes a vector to unit length; used as a */

/*     service routine by the Connolly surface area and volume */

/*     computation */


	int
	vnorm_ (double *x, double *xn)
	{
		static int
			k;
		static double
			ax;



		/* Parameter adjustments */
		--xn;
		--x;

		/* Function Body */
		ax = anorm_ (&x[1]);
		for (k = 1; k <= 3; ++k)
		{
			xn[k] = x[k] / ax;
		}
		return
			0;
	}	/* vnorm_ */



/*     ################################################################# */
/*     ##                                                             ## */
/*     ##  function triple  --  form triple product of three vectors  ## */
/*     ##                                                             ## */
/*     ################################################################# */

/*     "triple" finds the triple product of three vectors; used as */
/*     a service routine by the Connolly surface area and volume */
/*     computation */


	double triple_ (double *x, double *y, double *z__)
	{
		/* System generated locals */
		double
			ret_val;

		/* Local variables */
		static double
			xy[3];



		/* Parameter adjustments */
		--z__;
		--y;
		--x;

		/* Function Body */
		vcross_ (&x[1], &y[1], xy);
		ret_val = dot_ (xy, &z__[1]);
		return ret_val;
	}															/* triple_ */



	/*     ################################################################ */
	/*     ##                                                            ## */
	/*     ##  function vecang  --  finds the angle between two vectors  ## */
	/*     ##                                                            ## */
	/*     ################################################################ */
	/*     "vecang" finds the angle between two vectors handed with respect */
	/*     to a coordinate axis; returns an angle in the range [0,2#pi] */


	double vecang_ (double *v1, double *v2, double *axis, double *hand)
	{
		/* System generated locals */
		double
			ret_val;

		/* Local variables */
		static double
			angle;
		static double
			a1, a2, a12, dt;



		/* Parameter adjustments */
		--axis;
		--v2;
		--v1;

		/* Function Body */
		a1 = anorm_ (&v1[1]);
		a2 = anorm_ (&v2[1]);
		dt = dot_ (&v1[1], &v2[1]);
		a12 = a1 * a2;
		if (fabs (a12) != 0.0)
		{
			dt /= a12;
		}
		if (dt < -1.)
		{
			dt = -1.;
		}
		if (dt > 1.)
		{
			dt = 1.;
		}
		angle = acos (dt);
		if (*hand * triple_ (&v1[1], &v2[1], &axis[1]) < 0.0)
		{
			ret_val = 2 * M_PI - angle;
		}
		else
		{
			ret_val = angle;
		}
		return ret_val;
	}	/* vecang_ */



	/*     ######################### */
	/*     ##                     ## */
	/*     ##  subroutine cirpln  ## */
	/*     ##                     ## */
	/*     ######################### */


	int cirpln_ (double *circen, double *cirrad, double *cirvec,
							 double *plncen, double *plnvec, bool * cinsp, bool * cintp, double *xpnt1, double *xpnt2)
	{
		/* System generated locals */
		double
			d__1, d__2;

		/* Local variables */
		static double
			rlen, vect1[3], vect2[3];
		static int
			k;
		static double
			ratio;
		static double
			uvect1[3], uvect2[3], cpvect[3];
		static double
			dcp, dir;
		static double
			pnt1[3];



		/* Parameter adjustments */
		--xpnt2;
		--xpnt1;
		--plnvec;
		--plncen;
		--cirvec;
		--circen;

		/* Function Body */
		for (k = 1; k <= 3; ++k)
		{
			cpvect[k - 1] = plncen[k] - circen[k];
		}
		dcp =
		dot_ (cpvect, &plnvec[1]);
		*cinsp = dcp > 0.0;
		vcross_ (&plnvec[1], &cirvec[1], vect1);
		if (anorm_ (vect1) > 0.0)
		{
			vnorm_ (vect1, uvect1);
			vcross_ (&cirvec[1], uvect1, vect2);
			if (anorm_ (vect2) > 0.0)
			{
				vnorm_ (vect2, uvect2);
				dir = dot_ (uvect2, &plnvec[1]);
				if (dir != 0.0)
				{
					ratio = dcp / dir;
					if (fabs (ratio) <= *cirrad)
					{
						for (k = 1; k <= 3; ++k)
						{
							pnt1[k - 1] = circen[k] + ratio * uvect2[k - 1];
						}

						d__1 = *cirrad;

						d__2 = ratio;
						rlen = d__1 * d__1 - d__2 * d__2;
						if (rlen < 0.0)
						{
							rlen = 0.0;
						}
						rlen = sqrt (rlen);
						for (k = 1; k <= 3; ++k)
						{
							xpnt1[k] = pnt1[k - 1] - rlen * uvect1[k - 1];
							xpnt2[k] = pnt1[k - 1] + rlen * uvect1[k - 1];
						}
						*cintp = true;
						return 0;
					}
				}
			}
		}
		*cintp = false;
		return 0;
	}	/* cirpln_ */



	/*     ################################################################# */
	/*     ##                                                             ## */
	/*     ##  subroutine gendot  --  find surface points on unit sphere  ## */
	/*     ##                                                             ## */
	/*     ################################################################# */

	/*     "gendot" finds the coordinates of a specified number of surface */
	/*     points for a sphere with the input radius and coordinate center */


	int gendot_ (int *ndots, double *dots, double *radius, double *xcenter, double *ycenter, double *zcenter)
	{
		/* System generated locals */
		int i__1, i__2;

		/* Local variables */
		static int
			i__, j, k, nvert;
		static double
			fi, fj, xa, ya, za, xy;
		static int
			nequat, nhoriz;


		/* Parameter adjustments */
		dots -= 4;

		/* Function Body */
		nequat = (int) sqrt ((double) (*ndots) * M_PI);
		nvert = (int) (nequat * .5);
		if (nvert < 1)
		{
			nvert = 1;
		}
		k =
			0;
		i__1 = nvert;
		for (i__ = 0; i__ <= i__1; ++i__)
		{
			fi = (double) i__ *
			M_PI / (double) nvert;
			za = cos (fi);
			xy = sin (fi);
			nhoriz = (int) (nequat * xy);
			if (nhoriz < 1)
			{
				nhoriz = 1;
			}
			i__2 = nhoriz - 1;
			for (j = 0; j <= i__2; ++j)
			{
				fj = (double) j *
				2 * M_PI / (double) nhoriz;
				xa = cos (fj) * xy;
				ya = sin (fj) * xy;
				++k;
				dots[k * 3 + 1] = xa * *radius + *xcenter;
				dots[k * 3 + 2] = ya * *radius + *ycenter;
				dots[k * 3 + 3] = za * *radius + *zcenter;
				if (k >= *ndots)
				{
					goto L10;
				}
			}
		}
	L10:
		*ndots = k;
		return 0;
	}	/* gendot_ */


}	// namespace BALL

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: fieldLineCreator.C,v 1.1.4.2 2007/04/11 11:55:53 amoll Exp $

#include <BALL/VIEW/MODELS/fieldLineCreator.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/atom.h>

#include <BALL/VIEW/PRIMITIVES/multiLine.h>
#include <BALL/VIEW/PRIMITIVES/point.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>

using namespace std;

namespace BALL
{
	namespace VIEW
	{

		FieldLineCreator::FieldLineCreator()
			:	tolerance_(0.0001),
				max_steps_(10000),
				interpolation_steps_(2),
				icosaeder_steps_(1),
				atom_distance_(1),
				field_line_errors_(0),
				use_atoms_(false),
				monte_carlo_nr_lines_(500),
				potential_grid_(0),
				vector_grid_(0),
				ac_(0),
				use_potential_grid_(false)
		{
		}

		FieldLineCreator::FieldLineCreator(const FieldLineCreator& )
		{
		}

		FieldLineCreator::~FieldLineCreator()
		{
			#ifdef BALL_VIEW_DEBUG
				Log.error() << "Destructing object " << (void *)this 
										<< " of class " << RTTI::getName<FieldLineCreator>() << std::endl;
			#endif 
		}

		Representation* FieldLineCreator::createLines()
		{
			if (vector_grid_ == 0) 
			{
				BALLVIEW_DEBUG
				return 0;
			}

			field_line_errors_ = 0;

			TRegularData3D<Vector3>::CoordinateType spacing = vector_grid_->getSpacing();
			TRegularData3D<Vector3>::IndexType         size = vector_grid_->getSize();
			if (Maths::isZero(spacing.x) ||
					Maths::isZero(spacing.y) ||
					Maths::isZero(spacing.z))
			{
				Log.error() << "Aborting, since vector grid has a spacing of 0!" << std::endl;
				return 0;
			}

			if (size.x == 0 ||
					size.y == 0 ||
					size.z == 0)
			{
				Log.error() << "Aborting, since vector grid has a size of 0!" << std::endl;
				return 0;
			}


			if (use_atoms_ && ac_ == 0) 
			{
				Log.error() << "No System available for this gradient grid, aborting field line calculation!" << std::endl;
				return 0;
			}

			Representation* rep = new Representation();
			rep->setModelType(MODEL_FIELD_LINES);

			if (use_atoms_)
			{
				// seed points from spheres around atoms:
				vector<Vector3> start_diffs;
				if (icosaeder_steps_ == 0) 
				{
					start_diffs.push_back(Vector3(0,0,0));
				}
				else
				{
					start_diffs = VIEW::createSphere(icosaeder_steps_ - 1);
				}
				AtomIterator ait = ac_->beginAtom();
				for (; +ait; ++ait)
				{
					for (Position p = 0; p < start_diffs.size(); p++)
					{
						const Vector3& point = ait->getPosition();
						const Vector3& diff = start_diffs[p];
						createFieldLine_(point + diff * atom_distance_, *rep);
					}
				}
			}
			else
			{
				// method from "Fast Display of Multi Field Lines"
				// from Stalling, Zaeckler, Hege; 1997
				// Monte Carlo Approach in relation to potential strenght at the individual points

				if (use_potential_grid_)
				{
					if (potential_grid_ == 0) 
					{
						delete rep;
						Log.error() << "No potential grid loaded, aborting..." << std::endl;
						return 0;
					}

					TRegularData3D<Vector3>::CoordinateType pspacing = potential_grid_->getSpacing();
					TRegularData3D<float>::IndexType        psize = potential_grid_->getSize();
					if (Maths::isZero(pspacing.x) ||
							Maths::isZero(pspacing.y) ||
							Maths::isZero(pspacing.z))
					{
						delete rep;
						Log.error() << "Aborting, since grid has a spacing of 0!" << std::endl;
						return 0;
					}

					if (psize.x == 0 ||
							psize.y == 0 ||
							psize.z == 0)
					{
						delete rep;
						Log.error() << "Aborting, since grid has a size of 0!" << std::endl;
						return 0;
					}
				}

				Vector3 origin = vector_grid_->getOrigin();
				Vector3 dimension = vector_grid_->getDimension();
				VectorGrid::IndexType size = vector_grid_->getSize();

				Size sx = (Size)(size.x / 2.0 + 1);
				Size sy = (Size)(size.y / 2.0 + 1);
				Size sz = (Size)(size.z / 2.0 + 1);
				RegularData3D::IndexType st(sx, sy, sz);
				Vector3 diff = Vector3(0.001);

				RegularData3D new_grid(st, origin - diff, vector_grid_->getDimension() + diff * 2.);
				const Size new_grid_size = sx * sy * sz;
				for (Position p = 0; p < new_grid_size; p++)
				{
					new_grid[p] = 0;
				}

				// use strength of potential grid?
				if (use_potential_grid_)
				{
					if (potential_grid_->getOrigin() != vector_grid_->getOrigin() ||
							potential_grid_->getDimension() != vector_grid_->getDimension())
					{
						delete rep;
						Log.error() << "Potential and vector grid have different sizes, aborting..." << std::endl;
						return 0;
					}

				
					const vector<float>& values =  potential_grid_->getData();
					for (Position p = 0; p < values.size(); p++)
					{
						new_grid.getClosestValue((potential_grid_->getCoordinates(p))) += BALL_ABS(values[p]);
					}
				}
				else
				{
					const vector<Vector3>& values =  vector_grid_->getData();
					for (Position p = 0; p < values.size(); p++)
					{
						new_grid.getClosestValue((vector_grid_->getCoordinates(p))) += values[p].getLength();
					}

					// use strength of vector field
				}

				vector<Vector3> result_points;
				calculateRandomPoints(new_grid, monte_carlo_nr_lines_, result_points);

				for (Position p = 0; p < monte_carlo_nr_lines_; p++)
				{
					createFieldLine_(result_points[p], *rep);
								
					/*
					Sphere* s = new Sphere();
					s->setPosition(point);
					s->setRadius(0.05);
					s->setColor(ColorRGBA(0.,0.1,0));
					rep->insert(*s);
					*/
				} // all lines
			}
			
			return rep;
		}


		void FieldLineCreator::createFieldLine_(const Vector3& point, Representation& rep)
		{
			for (Size backwards = 0; backwards < 2; backwards++)
			{
				MultiLine* line = new MultiLine;
				vector<Vector3> points;

				calculateLinePoints_(point, points, (backwards == 0) ? 1. : -1.);

				// somethis way may run over the grid's borders:
				Index p = 0;
				try
				{
					for (; p < (Index)points.size(); p++)
					{
						vector_grid_->getClosestIndex(points[p]);
					}
				}
				catch(...)
				{
					p--;
				}

				if (p < 3)
				{
					field_line_errors_ ++;
					delete line;
					return;
				}

				// throw away errorous points:
				points.resize(p);

				// take only points that are at least 0.05 A apart:
				Size nrp = points.size();
				vector<Vector3>& points_ok = line->vertices;
				points_ok.push_back(points[0]);
				float min_d = 0.05 * 0.05;
				float d = 0;
				for (Position p = 1; p < nrp; p++)
				{
					d += points[p].getSquareDistance(points[p - 1]);
					if (d > min_d)
					{
						points_ok.push_back(points[p]);
						d = 0;
					}
				}

				nrp = points_ok.size();
				if (nrp < 3)
				{
					field_line_errors_ ++;
					delete line;
					return;
				}

				line->tangents.resize(nrp);

				for (Position v = 0; v < nrp - 1; v++)
				{
					(*line).tangents[v] = points_ok[v+1] - points_ok[v];
				}
				(*line).tangents[nrp -1] = (*line).tangents[nrp -2];

				(*line).colors.push_back(ColorRGBA(0.,0.,1.));
				rep.insert(*line);
			}
		}

		/** Compute a field line using a Runge-Kutta of fourth order with adaptive step
		 *  size control. factor can be used to iterate _against_ the gradient, i.e. backwards in time.
		 */
		void FieldLineCreator::calculateLinePoints_(Vector3 point, vector<Vector3>& points, float factor)
		{
			TRegularData3D<Vector3>::CoordinateType spacing = vector_grid_->getSpacing();
			TRegularData3D<Vector3>::IndexType         size = vector_grid_->getSize();
			Vector3 k1, k2, k3, k4, k5, k6;
			Vector3 		p2, p3, p4, p5, p6;
			Vector3 error_estimate_vector;
			Vector3 scaling;
			float error_estimate = 0.;

			float min_spacing = std::min(std::min(spacing.x, spacing.y), spacing.z);
			float rho = 0.9; // chose sensible values
			float lower_limit = min_spacing * 0.00001;
			float h = min_spacing;// * 0.1;

			// use interpolation_steps interpolation points
			std::vector<Vector3> interpolated_values(interpolation_steps_);

			Vector3 rk_estimate;

			Vector3 grad_current = (*vector_grid_)(point) * factor;
			Vector3 grad_old     = grad_current;
			
			// Runge - Kutta of order 4 with adaptive step size and
			// error control as described in Schwarz: "Numerische Mathematik"
			// with step size control taken from Numerical Recipes
			for (Size i = 0; i < max_steps_; i++)
			{
				// compute scaling values for the step size computation (see Numerical Recipes)
				scaling.x = fabs(point.x) + fabs(grad_current.x*h) + 1e-30;
				scaling.y = fabs(point.y) + fabs(grad_current.y*h) + 1e-30;
				scaling.z = fabs(point.z) + fabs(grad_current.z*h) + 1e-30;

				if (Maths::isZero(tolerance_))
				{
					logString("Value 0 for tolerance of field line! Aborting...\n");
					return;
				}

				if (Maths::isZero(h))
				{
					logString("Value 0 for h of field line! Aborting...\n");
					return;
				}


				if (Maths::isZero(scaling.x) ||
						Maths::isZero(scaling.y) ||
						Maths::isZero(scaling.z))
				{
					return;
				}

				// repeat the Runge-Kutta until the step size is either accepted or completely rejected
				bool accepted = false;

				while (!accepted)
				{
					try
					{
						k1 = h*grad_current;
						
						p2 = point + k1*2./9.;
						k2 = h*vector_grid_->getInterpolatedValue(p2)*factor;

						p3 = point + k1*1./12. + k2*1./4.;
						k3 = h*vector_grid_->getInterpolatedValue(p3)*factor;

						p4 = point + k1*69./128. - k2*243./128. + k3*135./64.;
						k4 = h*vector_grid_->getInterpolatedValue(p4)*factor;

						p5 = point - k1*17./12. + k2*27./4. - k3*27./5.+ k4*16./15.;
						k5 = h*vector_grid_->getInterpolatedValue(p5)*factor;

						p6 = point + k1*65./432. - k2*5./16. + k3 * 13./16. + k4*4./27. + k5*5./144.;
						k6 = h*vector_grid_->getInterpolatedValue(p6)*factor;

						// let's see if the steps have become that small that we don't proceed at all...
						if (p6 == point)
						{
							return;
						}

						rk_estimate = (k1 / 9. + k3 * 9./20. + k4*16./45. + k5 / 12.);

						error_estimate_vector = (-k1*2. + k3*9. - k4*64. - k5*15. + k6*72.) / 300.;
						error_estimate = std::max(fabs(error_estimate_vector.x/scaling.x), 
														 std::max(fabs(error_estimate_vector.y/scaling.y), 
																			fabs(error_estimate_vector.z/scaling.z)));

						error_estimate /= tolerance_;

						if (error_estimate > 1.0)
						{
							// update h using the error estimate
							double h_new = h * rho * pow((double)error_estimate, (double)-0.25);

							h = (h >= 0) ? std::max(h_new, 0.1*h) : std::min(h_new, 0.1*h);
						}
						else 
						{
							accepted = true;
						}
					} catch (Exception::OutOfGrid& e)
					{
						h /= 2.;
						// horrible heuristic... :-)
						if (fabs(h) < 1e-7)
						{
							return;
						}
					}
				}

				// compute a step size for the next step (the magic numbers are taken from Numerical Recipes)
				if (error_estimate > 1.89e-4) h = rho * h * pow((double)error_estimate, (double)-0.2);
				else 												  h = 5.*h;

				grad_old = grad_current;
				grad_current = (*vector_grid_)(point+rk_estimate)*factor;

				cubicInterpolation(point, point+rk_estimate, grad_old, grad_current, interpolated_values);	
				points.push_back(point);

				for (Position p = 0; p < interpolated_values.size(); p++)
				{
					points.push_back(interpolated_values[p]);
				}

				point += rk_estimate;

				if (rk_estimate.getLength() < lower_limit) break;
			}
		}
	
	} // namespace VIEW
} // namespace BALL

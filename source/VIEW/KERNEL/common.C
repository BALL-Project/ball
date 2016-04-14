// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>
#include <BALL/VIEW/WIDGETS/scene.h>

#include <BALL/SYSTEM/directory.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/SYSTEM/fileSystem.h>
#include <BALL/SYSTEM/path.h>

#include <BALL/STRUCTURE/geometricProperties.h>

#include <QtWidgets/QApplication>
#include <QtCore/QUrl>
#include <QtCore/QMimeData>

#include <BALL/COMMON/init.h>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>

namespace BALL
{
	namespace VIEW
	{

		float SurfaceDrawingPrecisions[4] = 
		{
			1.5,
			3.5,
			6.5,
			12
		};


		Composite composite_to_be_ignored_for_colorprocessors_;

		String vector3ToString(const Vector3& v)
		{
			return String("(") + String(v.x) + "," + 
													 String(v.y) + "," +
													 String(v.z) + ") ";
		}


		String createFloatString(float value, Size precision)
		{
			std::ostringstream stream;
			stream << std::setprecision(precision);
			stream.imbue(std::locale("C"));
			stream << value;

			return stream.str();
		}

		bool stringToVector3(const String& data, Vector3& v)
		{
			try
			{
				vector<String> fields;
				if (data.split(fields, ",()") != 3) return false;
				v.x = fields[0].toFloat();
				v.y = fields[1].toFloat();
				v.z = fields[2].toFloat();
				return true;
			}
			catch(...)
			{
			}

			return false;
		}

		MainControl* getMainControl()
		{
			MainControl* mc = 0;
			mc = MainControl::getInstance(0);

			#ifdef BALL_VIEW_DEBUG
				if (mc == 0)
				{
					throw (Exception::NullPointer(__FILE__, __LINE__));
				}
			#endif

			return mc; 
		}


		String createTemporaryFilename()
		{
			Directory org;
			Directory::changeToUserHomeDir();
			String filename;
			File::createTemporaryFilename(filename);
			filename = Directory::getUserHomeDir() + FileSystem::PATH_SEPARATOR + filename;
			org.setCurrent();
			return filename;
		}
			

		Vector3 getNormal(const Vector3& v)
		{
			Vector3 n = v % Vector3(1,0,0);
			if (Maths::isZero(n.getSquareLength())) 
			{ 
				n = v % Vector3(0,1,0);
				if (Maths::isZero(n.getSquareLength())) 
				{
					n = v % Vector3(0,0,1);
				}
			}
			n.normalize();

			return n;
		}

		void logString(const String& data)
		{
			if (MainControl::getInstance(0) == 0) 
			{
				Log.error() << data << std::endl;
				return;
			}

			LogEvent* su = new LogEvent;
			su->setMessage(data);
			su->setShowOnlyInLogView(true);
			qApp->postEvent((QObject*)MainControl::getInstance(0), su);  // Qt will delete it when done
		}


		LogEvent::LogEvent()
			: QEvent((QEvent::Type)LOG_EVENT),
				important_(false),
				only_log_(false)
		{
		}

		ColorRGBA getColor(const QLabel* label) 
		{
			QPalette pal(label->palette());
			QColor qcolor = pal.color(label->backgroundRole());
			return ColorRGBA(qcolor);
		}

		void setColor(const QLabel* label, const ColorRGBA& color)
		{
			QPalette pal(label->palette());
			pal.setColor(label->backgroundRole(), color.getQColor());
			pal.setColor(QPalette::Window, color.getQColor());
			((QLabel*)label)->setAutoFillBackground(true);
			((QLabel*)label)->setPalette(pal);
		}

		void setTextColor(QLabel* label, const ColorRGBA& color)
		{
			QPalette pal(label->palette());
			pal.setColor(label->foregroundRole(), color.getQColor());
			((QLabel*)label)->setPalette(pal);
		}

		QColor chooseColor(QLabel* label)
		{
			QColor qcolor;
			QPalette pal(label->palette());
			qcolor = pal.color(QPalette::Window);
			QColor qcolor2 = QColorDialog::getColor(qcolor);
			if (!qcolor2.isValid()) return qcolor;
			pal.setColor(QPalette::Window, qcolor2);
			label->setPalette(pal);
			label->setAutoFillBackground(true);
			return qcolor2;
		}

		void processDropEvent(QDropEvent* e)
		{
			if (!e->mimeData()->hasUrls())
			{
				e->ignore();
				return;
			}

			QList<QUrl> urls = e->mimeData()->urls();
			e->acceptProposedAction();

			QList<QUrl>::iterator it = urls.begin();
			for (; it != urls.end(); it++)
			{
				QString filename = (*it).path();
				getMainControl()->openFile(ascii(filename));
			}
		}

		Camera focusCamera(const list<Vector3>& points)
		{
			// use processor for calculating the center
			GeometricCenterProcessor center;
			center.start();

			list<Vector3>::const_iterator vit = points.begin();
			for (; vit != points.end(); ++vit)
			{
				center.operator()(*vit);
			}

			center.finish();

			const Vector3 look_at_point = center.getCenter();

			Vector3 max_distance_point;
			float max_square_distance = -1;

			vit = points.begin();
			for (; vit != points.end(); ++vit)
			{
				float sd = (*vit - look_at_point).getSquareLength();
				if (sd > max_square_distance)
				{
					max_square_distance = sd;
					max_distance_point = *vit;
				}
			}

			Vector3 max_distance_vector(max_distance_point - look_at_point);
			if (Maths::isZero(max_distance_vector.getSquareLength()))
				max_distance_vector.set(0,1,0);

			Vector3 up_vector = Vector3(1,0,0);
			Vector3 view_vector = up_vector % max_distance_vector;
			if (Maths::isZero(view_vector.getSquareLength())) 
			{
				up_vector = Vector3(0,1,0);
				view_vector = up_vector % max_distance_vector;
			}

			if (Maths::isZero(view_vector.getSquareLength()))
			{
				up_vector = Vector3(0,0,1);
				view_vector = up_vector % max_distance_vector;
			}

			if (Maths::isZero(view_vector.getSquareLength()))
			{
				view_vector = Vector3(1,0,0);
			}

			if (!Maths::isZero(view_vector.getSquareLength())) view_vector.normalize();

			float distance = max_distance_vector.getLength() / tan(Angle(31, false).toRadian());
			if (distance < 5) 	distance = 5;
			if (distance > 150) distance = 150;

			view_vector *= distance;

			// update scene
			Camera camera(look_at_point - view_vector, look_at_point, up_vector);

			MainControl* mc = getMainControl();
			if (mc)
			{
				SceneMessage *scene_message = new SceneMessage(SceneMessage::UPDATE_CAMERA);
				scene_message->setStage(*(Scene::getInstance(0)->getStage()));
				camera.setProjectionMode(scene_message->getStage().getCamera().getProjectionMode());
				scene_message->getStage().setCamera(camera);
				mc->sendMessage(*scene_message);
			}

			return camera;
		}

		Camera focusCamera(Composite* composite)
		{
			Composite* to_center_on = composite;
			
			if (to_center_on == 0)
			{
				MainControl* mc = getMainControl();

				if (mc == 0 || mc->getMolecularControlSelection().size() == 0)
				{
					return Camera();
				}

				to_center_on = *mc->getMolecularControlSelection().begin();
			}

			list<Vector3> positions;

			AtomContainer* ai = dynamic_cast<AtomContainer*>(to_center_on);
			if (ai != 0)
			{
				AtomIterator ait = ai->beginAtom();
				for (; ait != ai->endAtom(); ait++)
				{
					positions.push_back((*ait).getPosition());
				}
			}
			else
			{
				const Atom* atom = dynamic_cast<const Atom*>(to_center_on);
				if (atom == 0) return Camera();
				positions.push_back(atom->getPosition());
			}

			return focusCamera(positions);
		}

		String ascii(const QString& str)
		{
			return str.toStdString();
		}

		/** Uses the de-Casteljou algorithm to evalute a cubic Hermite interpolation
		 *  polynomial at interpolated_values.size() equidistant values.
		 */
	  void cubicInterpolation(const Vector3& a, const Vector3& b,
													  const Vector3& tangent_a, const Vector3& tangent_b,
														std::vector<Vector3>& interpolated_values)
		{
			// compute the Bezier points
			Vector3 bezier[9];
			bezier[0] = a;
			bezier[3] = b;

			float interval_length_2 = (b-a).getSquareLength();

			float f = (tangent_a * (b-a)) * 3.;
			if (!Maths::isZero(f))
			{
				bezier[1] = a + tangent_a *interval_length_2 / f;
			}
			else
			{
				bezier[1] = a;
			}

			f = (tangent_b * (b-a)) * 3.;
			if (!Maths::isZero(f))
			{
				bezier[2] = b - tangent_b *interval_length_2 / f;
			}
			else
			{
				bezier[2] = b;
			}

			// compute the step size
			float step_size = 1./(interpolated_values.size()+1);
			Index i = 0;

			for (float evaluation_point = step_size; evaluation_point < 1.; evaluation_point += step_size)
			{
				bezier[4] = (bezier[1] - bezier[0]) * evaluation_point + bezier[0];
				bezier[5] = (bezier[2] - bezier[1]) * evaluation_point + bezier[1];
				bezier[6] = (bezier[3] - bezier[2]) * evaluation_point + bezier[2];

				bezier[7] = (bezier[5] - bezier[4]) * evaluation_point + bezier[4];
				bezier[8] = (bezier[6] - bezier[5]) * evaluation_point + bezier[5];

				interpolated_values[i] = (bezier[8] - bezier[7]) * evaluation_point + bezier[7];
				i++;
			}	
		}

		#define SPHERE_X .525731112119133606
		#define SPHERE_Z .850650808352039932

		float icosaeder_vertices[12][3] =
		{
			{-SPHERE_X, 0.0, SPHERE_Z},
			{ SPHERE_X, 0.0, SPHERE_Z},
			{-SPHERE_X, 0.0,-SPHERE_Z},
			{ SPHERE_X, 0.0,-SPHERE_Z},
			{ 0.0, SPHERE_Z, SPHERE_X},
			{ 0.0, SPHERE_Z,-SPHERE_X},
			{ 0.0,-SPHERE_Z, SPHERE_X},
			{ 0.0,-SPHERE_Z,-SPHERE_X},
			{ SPHERE_Z, SPHERE_X, 0.0},
			{-SPHERE_Z, SPHERE_X, 0.0},
			{ SPHERE_Z,-SPHERE_X, 0.0},
			{-SPHERE_Z,-SPHERE_X, 0.0}
		};

		Position icosaeder_indices[20][3] =
		{
			{ 0, 4, 1}, { 0, 9, 4}, { 9, 5, 4}, { 4, 5, 8}, { 4, 8, 1},
			{ 8,10, 1}, { 8, 3,10}, { 5, 3, 8}, { 5, 2, 3}, { 2, 7, 3},
			{ 7,10, 3}, { 7, 6,10}, { 7,11, 6}, {11, 0, 6}, { 0, 1, 6},
			{ 6, 1,10}, { 9, 0,11}, { 9,11, 2}, { 9, 2, 5}, { 7, 2,11}
		};

		void subdivideTriangle(vector<Vector3>& results, Vector3& v1, Vector3& v2, Vector3& v3, Size precision)
		{
			if (precision == 0)
			{
				Vector3 result = v1 + v2 + v3;
				result.normalize();
				results.push_back(result);
				return;
			}

			Vector3 v12 = v1 + v2;
			Vector3 v23 = v2 + v3;
			Vector3 v31 = v3 + v1;
			
			v12.normalize();
			v23.normalize();
			v31.normalize();

			subdivideTriangle(results, v1, v12, v31, precision - 1);
			subdivideTriangle(results, v2, v23, v12, precision - 1);
			subdivideTriangle(results, v3, v31, v23, precision - 1);
			subdivideTriangle(results, v12, v23, v31, precision - 1);
		}

		vector<Vector3> createSphere(Size precision)
		{
			vector<Vector3> results;
			for (int i = 0; i < 20; ++i)
			{
				Vector3 v1(icosaeder_vertices[icosaeder_indices[i][0]][0],
									 icosaeder_vertices[icosaeder_indices[i][0]][1],
									 icosaeder_vertices[icosaeder_indices[i][0]][2]);
				
				Vector3 v2(icosaeder_vertices[icosaeder_indices[i][1]][0],
									 icosaeder_vertices[icosaeder_indices[i][1]][1],
									 icosaeder_vertices[icosaeder_indices[i][1]][2]);
				
				Vector3 v3(icosaeder_vertices[icosaeder_indices[i][2]][0],
									 icosaeder_vertices[icosaeder_indices[i][2]][1],
									 icosaeder_vertices[icosaeder_indices[i][2]][2]);
				
				subdivideTriangle(results, v1, v2, v3, precision);
			}
			return results;
		}

		void calculateHistogramEqualization(const vector<float>& values, 
																				vector<float>& normalized_values, 
																				bool use_absolute_values)
		{
			vector<float> sorted;
			float size = (float) values.size();
			normalized_values.resize((Size)size);
 			std::map<float, float> map;

			if (!use_absolute_values)
			{
				sorted = values;
				sort(sorted.begin(), sorted.end());
			}
			else
			{
				vector<float> temp(values);
				for (Position p = 0; p < size; p++)
				{
					temp[p] = BALL_ABS(temp[p]);
				}
				sorted = temp;
				sort(sorted.begin(), sorted.end());
			}

			for (Position p = 0; p < size; p++)
			{
				map[sorted[p]] = p / size;
			}

			for (Position p = 0; p < size; p++)
			{
				normalized_values[p] = map[values[p]];
			}
		}


		void calculateRandomPoints(const RegularData3D& grid, Size nr_points, 
															 vector<Vector3>& resulting_points)
		{
			PreciseTime pt;
			boost::mt19937 ran_gen(pt.now().getMicroSeconds());

			Vector3 point;

			vector<float> normalized_values;
			calculateHistogramEqualization(grid.getData(), normalized_values, true);

			float current = 0;
			for (Position p = 0; p < normalized_values.size(); p++)
			{
				current += normalized_values[p];
				normalized_values[p] = current;
			}

			const Vector3 spacing = grid.getSpacing();
			Vector3 off = Vector3(spacing.x / 2.0, spacing.y / 2.0, spacing.z / 2.0);
			float xd  =  spacing.x;
			float yd  =  spacing.y;
			float zd  =  spacing.z;
			float xdm = -xd;
			float ydm = -yd;
			float zdm = -zd;

			float x;
			Index max_max = grid.getData().size();
			Index hh = (int) (grid.getData().size() / 2.0);
			Index h, hmin, hmax;

			boost::uniform_real<double> rng(0, current);
			boost::uniform_real<double> rng_xd(xdm, xd);
			boost::uniform_real<double> rng_yd(ydm, yd);
			boost::uniform_real<double> rng_zd(zdm, zd);

			for (Position i = 0; i < nr_points; i++)
			{
				x = rng(ran_gen);

				try
				{
					hmin = 0;
					hmax = max_max;
					h = hh;

					while (hmax - hmin > 1)
					{
						if (normalized_values[h] < x) 
						{
							hmin = h + 1;
						}
						else if (normalized_values[h] > x) 
						{
							hmax = h - 1;
						}
						else
						{
							break;
						}

						h = (Index)((hmax - hmin) / 2.0 + hmin);
					}

					if (hmax - hmin == 1)
					{
						if (normalized_values[hmin] < x) h = hmax;
						else h = hmin;
					}

					Vector3 point = grid.getCoordinates(h) + off +
													Vector3(rng_xd(ran_gen), rng_yd(ran_gen), rng_zd(ran_gen));

					grid.getClosestIndex(point);

					resulting_points.push_back(point);
				}
				catch(...)
				{
					i--;
				}
			}
		}

	// just to be on the safe side, create a BALL initializer object here as well,
	// so that libVIEW is also covered
	GlobalInitializer* gi = GlobalInitializer::init();

	} // namespace VIEW
} //namespace BALL

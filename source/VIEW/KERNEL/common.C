#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>

#include <BALL/VIEW/DATATYPE/colorExtension2.h>
#include <BALL/VIEW/PRIMITIVES/line.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/point.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/box.h>
#include <BALL/VIEW/PRIMITIVES/illuminatedLine.h>

#include <BALL/SYSTEM/directory.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/SYSTEM/fileSystem.h>
#include <BALL/SYSTEM/path.h>

#include <BALL/STRUCTURE/geometricProperties.h>

#include <QtGui/qapplication.h>
#include <QtCore/QUrl>

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

		String getModelName(ModelType type) 
			throw()
		{
			switch (type)
			{
				case MODEL_LINES:
					return "Line";
				case MODEL_STICK:
					return "Stick";
				case MODEL_BALL_AND_STICK:
					return "Ball and Stick";
				case MODEL_VDW:
					return "VDW";
				case MODEL_SE_SURFACE:
					return "SES";
				case MODEL_SA_SURFACE:
					return "SAS";
				case MODEL_CONTOUR_SURFACE:
					return "Contour Surface";
				case MODEL_GRID_SLICE:
					return "Grid Slice";
				case MODEL_GRID_VOLUME:
					return "Grid Volume";
				case MODEL_BACKBONE:
					return "Backbone";
				case MODEL_CARTOON:
					return "Cartoon";
				case MODEL_HBONDS:
					return "H-Bonds";
				case MODEL_FORCES:
					return "Forces";
				case MODEL_LABEL:
					return "Label";
				case MODEL_UNKNOWN:
					return "Unknown";
			}
			return "Unknown";
		}

		String getColoringName(ColoringMethod type) 
			throw()
		{
			switch (type)
			{
				case COLORING_ELEMENT:
					return "by element";
				case COLORING_RESIDUE_INDEX:
					return "by residue index";
				case COLORING_RESIDUE_NAME:
					return "by residue name";
				case COLORING_SECONDARY_STRUCTURE:
					return "by secondary structure";
				case COLORING_ATOM_CHARGE:
					return "by atom charge";
				case COLORING_DISTANCE:
					return "by atom distance";
				case COLORING_TEMPERATURE_FACTOR:
					return "by temperature factor";
				case COLORING_OCCUPANCY:
					return "by occupancy";
				case COLORING_FORCES:
					return "by forces";
				case COLORING_RESIDUE_TYPE:
					return "by residue type";
				case COLORING_CHAIN:
					return "by chain";
				case COLORING_MOLECULE:
					return "by molecule";
				case COLORING_CUSTOM:
					return "custom";
				case COLORING_UNKNOWN:
					return "Unknown";
			}
			return "Unknown";
		}


		bool isSurfaceModel(ModelType type)
			throw()
		{
			if (type == MODEL_SE_SURFACE || 
					type == MODEL_SA_SURFACE ||
					type == MODEL_CONTOUR_SURFACE)
			{
				return true;
			}

			return false;
		}


		bool modelMustBeRebuild(ModelType type)
			throw()
		{
			return (type == MODEL_SE_SURFACE ||
							type == MODEL_SA_SURFACE ||
							type == MODEL_BACKBONE 	||
							type == MODEL_FORCES    ||
							type == MODEL_BALL_AND_STICK||
							type == MODEL_CARTOON   ||
							type == MODEL_LABEL);
		}

		bool modelMuteableByDisplayProperties(ModelType type)
			throw()
		{ 
			return type < MODEL_LABEL;
		}

		// ===============================================
		String getTypeName(GeometricObjectType type)
		{
			switch(type)
			{
				case TYPE__LINE:
					return "Line";

				case TYPE__SPHERE:
					return "Sphere";
					
				case TYPE__TUBE:
					return "Tube";
					
				case TYPE__POINT:
					return "Point";
					
				case TYPE__MESH:
					return "Mesh";
					
				case TYPE__BOX:
					return "Box";

				default:
					return "unknown GeometricObject";
			}
		}


		GeometricObjectType getGeometricObjectType(const GeometricObject& object)
		{
			if (RTTI::isKindOf<Line>(object))
			{
				return TYPE__LINE;
			}	
			if (RTTI::isKindOf<Sphere>(object))
			{
				return TYPE__SPHERE;
			}
			if (RTTI::isKindOf<Tube>(object))
			{
				return TYPE__TUBE;
			}
			if (RTTI::isKindOf<Point>(object))
			{
				return TYPE__POINT;
			}	
			if (RTTI::isKindOf<Mesh>(object))
			{
				return TYPE__MESH;
			}	
			if (RTTI::isKindOf<Box>(object))
			{
				return TYPE__BOX;
			}	

			return TYPE__UNKNOWN;
		}

		String vector3ToString(const Vector3& v)
			throw()
		{
			return String("(") + String(v.x) + "," + 
													 String(v.y) + "," +
													 String(v.z) + ") ";
		}


		String createFloatString(float value, Size precision)
			throw()
		{
			String data(value);
			for (Position p = 0; p < data.size(); p++)
			{
				if (data[p] == '.')
				{
					data = data.left(p + precision + 1);
					data.trimRight("0");
					if (data == "-0.") data = "0.";
					if (data[data.size() - 1] == '.') 
					{
						data = data(0, data.size() - 1);
					}
					return data;
				}
			}

			return data;
		}

		bool stringToVector3(const String& data, Vector3& v)
			throw()
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
			throw()
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
			throw()
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
			throw()
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
			((QLabel*)label)->setAutoFillBackground(true);
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

		void focusCamera(const List<Vector3>& points)
		{
			// use processor for calculating the center
			GeometricCenterProcessor center;
			center.start();

			List<Vector3>::const_iterator vit = points.begin();
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
			SceneMessage *scene_message = new SceneMessage(SceneMessage::UPDATE_CAMERA);
			scene_message->getStage().getCamera().setLookAtPosition(look_at_point);
			scene_message->getStage().getCamera().setViewPoint(look_at_point- view_vector);
			scene_message->getStage().getCamera().setLookUpVector(up_vector);
			getMainControl()->sendMessage(*scene_message);
		}

		String getDataPath()
		{
			// we set the base directory to the first stored data path in the Path class
			// this should be equal to the BALLVIEW_DATA_PATH environment variable
			// (if it was set, otherwise the compiled data path)
			Path path;
			String dir1 = path.getDataPath();

			if (dir1.has('\n')) dir1 = dir1.before(String('\n'));

			// sort out double slashes
			String dir = "";
			dir += dir1[0];

			for (Position p = 1; p < dir1.size(); p++)
			{
				if (dir1[p - 1] == FileSystem::PATH_SEPARATOR &&
						dir1[p] 		 == FileSystem::PATH_SEPARATOR)
				{
					continue;
				}

				dir += dir1[p];
			}

			if (!dir.hasSuffix(String(FileSystem::PATH_SEPARATOR)))
			{
				dir += FileSystem::PATH_SEPARATOR;
			}
				
			return dir;
		}

		String ascii(const QString& str)
		{
			return str.toAscii().constData();
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

			bezier[1] = a + tangent_a *interval_length_2 / ((tangent_a * (b-a)) * 3.);
			bezier[2] = b - tangent_b *interval_length_2 / ((tangent_b * (b-a)) * 3.);

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
			throw()
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

			if (!use_absolute_values)
			{
				sorted = values;
				sort(sorted.begin(), sorted.end());

				Position x = 0;
				for (Position p = 0; p < size; p++)
				{
					while (values[p] > sorted[x] && x < (Size)size)
					{
						x++;
					}
				
					normalized_values[p] = (float)x / size;
				}
				return;
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

				Position x = 0;
				for (Position p = 0; p < size; p++)
				{
					while (temp[p] > sorted[x] && x < (Size)size)
					{
						x++;
					}
				
					normalized_values[p] = (float)x / size;
				}
			}
		}

		void getColors(const GeometricObject& object, HashSet<String>& colors)
		{
			String c;
			object.getColor().get(c);
			colors.insert(c);

			if (RTTI::isKindOf<ColorExtension2>(object))
			{
				(dynamic_cast<const ColorExtension2*>(&object))->getColor2().get(c);
				colors.insert(c);
				return;
			}

			if (RTTI::isKindOf<Mesh>(object))
			{
				const Mesh& mesh = *dynamic_cast<const Mesh*>(&object);
				for (Position p = 0; p < mesh.colors.size(); p++)
				{
					mesh.colors[p].get(c);
					colors.insert(c);
				}
				return;
			}

			if (RTTI::isKindOf<IlluminatedLine>(object))
			{
				const IlluminatedLine& line = *dynamic_cast<const IlluminatedLine*>(&object);
				for (Position p = 0; p < line.colors.size(); p++)
				{
					line.colors[p].get(c);
					colors.insert(c);
				}
				return;
			}
		}

	} // namespace VIEW
} //namespace BALL

#include <BALL/VIEW/KERNEL/common.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/message.h>

#include <BALL/VIEW/PRIMITIVES/line.h>
#include <BALL/VIEW/PRIMITIVES/sphere.h>
#include <BALL/VIEW/PRIMITIVES/tube.h>
#include <BALL/VIEW/PRIMITIVES/point.h>
#include <BALL/VIEW/PRIMITIVES/mesh.h>
#include <BALL/VIEW/PRIMITIVES/box.h>

#include <BALL/SYSTEM/directory.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/SYSTEM/fileSystem.h>
#include <BALL/SYSTEM/path.h>

#include <BALL/STRUCTURE/geometricProperties.h>

#ifdef BALL_PLATFORM_WINDOWS
 #include <qapplication.h>
#endif

#include <qdragobject.h>
#include <qdir.h>
#include <qwhatsthis.h>

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


		bool BALL_VIEW_DOCKWINDOWS_SHOW_LABELS = true;

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
			#ifdef BALL_PLATFORM_WINDOWS
				mc = dynamic_cast<MainControl*>(qApp->mainWidget());
			#else
				mc = MainControl::getInstance(0);
			#endif

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
			qApp->postEvent(MainControl::getInstance(0), su);  // Qt will delete it when done
		}


		LogEvent::LogEvent()
			: QCustomEvent(LOG_EVENT),
				important_(false),
				only_log_(false)
		{
		}

		QColor chooseColor(QLabel* label)
		{
			QColor qcolor = QColorDialog::getColor(label->backgroundColor());
			if (!qcolor.isValid()) return label->backgroundColor();
			label->setBackgroundColor(qcolor);
			return qcolor;
		}


		void processDropEvent(QDropEvent* e)
		{
			if (!QUriDrag::canDecode(e)) 
			{
				e->ignore();
				return;
			}

			QStrList lst;
			QUriDrag::decode(e, lst);
			e->accept();

			for (Position i = 0; i < lst.count(); ++i )
			{
				QString filename = QDir::convertSeparators(QUriDrag::uriToLocalFile(lst.at(i)));
				getMainControl()->openFile(filename.ascii());
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
	} // namespace VIEW
} //namespace BALL

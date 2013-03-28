#include <BALL/VIEW/RENDERING/sceneExporter.h>

#include <BALL/SYSTEM/file.h>

#include <QtCore/QString>

namespace BALL
{
	namespace VIEW
	{
		SceneExporter::SceneExporter(std::ostream* ostrm)
			: ostrm_(ostrm),
			  destroy_ostrm_(false)
		{
		}

		SceneExporter::SceneExporter(const QString& filename)
			: ostrm_(new File(filename.toStdString(), std::ios::out)),
			  destroy_ostrm_(true)
		{
		}

		SceneExporter::SceneExporter(const String& filename)
			: ostrm_(new File(filename, std::ios::out)),
			  destroy_ostrm_(true)
		{
		}

		SceneExporter::~SceneExporter()
		{
			if(destroy_ostrm_) {
				delete ostrm_;
			}
		}

		void SceneExporter::setOutputStream(std::ostream* strm)
		{
			if(destroy_ostrm_) {
				delete ostrm_;
			}

			ostrm_ = strm;
			destroy_ostrm_ = false;
		}

		bool SceneExporter::finish()
		{
			if(!finishImpl_()) {
				return false;
			}

			File* f = dynamic_cast<File*>(ostrm_);
			if(f) {
				f->close();
			}

			return true;
		}
	}
}

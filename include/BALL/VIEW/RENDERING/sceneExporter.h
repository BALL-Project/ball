#ifndef BALL_VIEW_RENDERING_SCENEEXPORTER_H
#define BALL_VIEW_RENDERING_SCENEEXPORTER_H

#include <BALL/COMMON/global.h>

#include <iostream>

class QString;

namespace BALL
{
	class String;

	namespace VIEW
	{
		// Forward declarations
		class Representation;
		class Stage;

		/**
		 * This is an abstract base class for objects that
		 * want to export the scene geometry to a file.
		 *
		 * For exporting image data it is recommended to instead create a subclass of Renderer.
		 */
		class BALL_VIEW_EXPORT SceneExporter
		{
			public:
				/**
				 * Default constructor.
				 *
				 * @param ostrm An ostrm object that should be used for output. If not specified std::cout is used.
				 *              The exporter is free to modify any properties of the stream.
				 */
				explicit SceneExporter(std::ostream* ostrm = &std::cout);

				/**
				 * Detailed constructor. This constructor allows to pass a filename that should be written to.
				 *
				 * @param ostrm An ostrm object that should be used for output. If not specified std::cout is used.
				 */
				explicit SceneExporter(const QString& filename);

				/**
				 * Detailed constructor. This constructor allows to pass a filename that should be written to.
				 *
				 * @param ostrm An ostrm object that should be used for output. If not specified std::cout is used.
				 */
				explicit SceneExporter(const String& filename);

				/**
				 * Destructor
				 *
				 * This destructor deletes the internal file handle if necessary
				 */
				virtual ~SceneExporter();

				/**
				 * Initialises the SceneExporter with a stage object as well as an optional
				 * width and height parameter that can be used as a hint for external rendering software.
				 *
				 * If the exporter was in use before, this call should clear all previous data.
				 *
				 * @param stage A pointer to a stage object
				 * @param height The height a rendered image should have. This parameter can be safely ignored.
				 * @param width The height a rendered image should have. This parameter can be safely ignored.
				 *
				 * @return false on error
				 */
				virtual bool init(const Stage* stage, float width, float height) = 0;

				/**
				 * Passes a representation that the SceneExporter should write to file.
				 * The exporter is free to buffer the representation without actually writing to file.
				 *
				 * @param rep The representation that should be exported.
				 * @return false on error
				 */
				virtual bool exportOneRepresentation(const Representation* rep) = 0;

				/**
				 * Tells the exporter that all representations have been passed and that
				 * it should write its output to file. Subclasses should implement
				 * finishImpl_() to do the heavy lifting.
				 *
				 * If the output is a File the file will be closed.
				 *
				 * @return false on error
				 */
				bool finish();

				/**
				 * Sets the output stream that should be written to. The exporter is free to modify
				 * any properties of the stream.
				 *
				 * @param ostrm a valid pointer to a std::ostream object.
				 */
				void setOutputStream(std::ostream* ostrm);

			protected:
				virtual bool finishImpl_() = 0;

				std::ostream* ostrm_;

			private:
				bool destroy_ostrm_;
		};
	}
}

#endif //BALL_VIEW_RENDERING_SCENEEXPORTER_H
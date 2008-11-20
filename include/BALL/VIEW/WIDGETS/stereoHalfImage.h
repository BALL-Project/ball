// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: genericControl.h,v 1.14.16.1 2007/03/25 21:26:20 oliver Exp $

#ifndef BALL_VIEW_WIDGETS_STEREOHALFIMAGE_H
#define BALL_VIEW_WIDGETS_STEREOHALFIMAGE_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_VIEW_RENDERING_GLRENDERER_H
# include <BALL/VIEW/RENDERING/glRenderer.h>
#endif

#include <QtOpenGL/qgl.h>
#include <QtGui/QApplication>

namespace BALL
{
	namespace VIEW
	{
		class Scene;

		class StereoHalfImage : public QGLWidget
		{
			Q_OBJECT

			public:
				StereoHalfImage(Index display_number, bool left_eye = true);
				StereoHalfImage(Stage* stage, Scene* scene, Index display_number, bool left_eye = true);
				~StereoHalfImage() {};

				void init(Stage* stage, Scene* scene);

				QSize minimumSizeHint() const;
				QSize sizeHint() const;

				void setRenderMode(int mode) {mode_ = mode;}
			protected:
				void initializeGL();
				void paintGL();

				void renderRepresentations_(int mode);
				void render_(const Representation& repr, int mode);

				GLRenderer* gl_renderer_;
				Stage* stage_;
				Scene* scene_;

				float prefactor_;

				int mode_;
		};
	}
}

#endif // BALL_VIEW_WIDGETS_STEREOHALFIMAGE_H

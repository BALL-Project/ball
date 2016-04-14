// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#include <BALL/VIEW/DIALOGS/stereoSettingsDialog.h>
#include <BALL/VIEW/WIDGETS/scene.h>
#include <BALL/VIEW/KERNEL/mainControl.h>
#include <BALL/VIEW/KERNEL/stage.h>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>

namespace BALL
{
	namespace VIEW
	{

StereoSettingsDialog::StereoSettingsDialog( QWidget* parent,  const char* name)
	: QDialog(parent),
		Ui_StereoSettingsDialogData(),
		ModularWidget(name)
{
	setupUi(this);

  // signals and slots connections
  connect( ok_button, SIGNAL( pressed() ), this, SLOT( okPressed() ) );
  connect( cancel_button, SIGNAL( pressed() ), this, SLOT( close() ) );
	connect( apply_button, SIGNAL( pressed() ), this, SLOT( apply() )  );
	connect( reset_to_standard_button, SIGNAL( pressed() ), this, SLOT( computeSettingsFromModelDistance() ) );

	setModal(false);
	setObjectName(name);

	//TODO
	screen_height->setText(String("200").c_str());
	screen_width->setText(String("300").c_str());
	viewer_distance_from_screen->setText(String("100").c_str());
	viewer_height->setText(String("170").c_str());

	ModularWidget::registerWidget(this);
	hide();
}

StereoSettingsDialog::~StereoSettingsDialog()
{
}

void StereoSettingsDialog::show()
{
	if (!Scene::getInstance(0))
	{
		QDialog::show();
		return;
	}

	Stage* stage = Scene::getInstance(0)->getStage();
	if (!stage)
	{
		QDialog::show();
		return;
	}

	focal_distance->setText(String(stage->getFocalDistance()).c_str());
	eye_separation->setText(String(stage->getEyeDistance()).c_str());

	QDialog::show();
}

void StereoSettingsDialog::initializeWidget(MainControl&)
{
	insertMenuEntry(MainControl::DISPLAY_STEREO, tr("Stereo Settings"), this, 
	                SLOT(show()), "Shortcut|Display|Stereo|Stereo_Settings", QKeySequence(),
									tr(""), UIOperationMode::MODE_ADVANCED);
}

void StereoSettingsDialog::computeSettingsFromModelDistance()
{
	// try to compute sensible stereo settings
	Camera& camera = Scene::getInstance(0)->getStage()->getCamera();
	Vector3 view_vector = camera.getViewVector();
	view_vector.normalize();
	const Vector3& view_point = camera.getViewPoint();

	RepresentationManager& r = MainControl::getInstance(0)->getRepresentationManager();
	RepresentationList::const_iterator rit = r.getRepresentations().begin();

	float min_separation = 600;

	for (; rit != r.getRepresentations().end(); ++rit)
	{
		list<GeometricObject*>::const_iterator it = (*rit)->getGeometricObjects().begin();

		vector<Vector3> positions;
		for (; it != (*rit)->getGeometricObjects().end(); ++it)
		{
			(*it)->getVertices(positions);
			// iterate over them and find the closest point along the view vector
			for (Position i=0; i<positions.size(); i++)
			{
				float separation = (positions[i] - view_point)*view_vector;
				min_separation = std::min(std::max(separation, 0.f), min_separation);
			}
			positions.clear();
		}
	}
	focal_distance->setText(String(2.1*min_separation).c_str());
	eye_separation->setText(String(1./20.*2.1*min_separation).c_str());
}	
	
void StereoSettingsDialog::apply()
{
	Stage* stage = Scene::getInstance(0)->getStage();
	stage->setFocalDistance((focal_distance->text()).toFloat());
	stage->setEyeDistance((eye_separation->text()).toFloat());
	Scene::getInstance(0)->update();
}

void StereoSettingsDialog::okPressed()
{
	apply();
	accept();
	return;

	MainControl *main_control = MainControl::getMainControl(parentWidget());

	float v_screen_height, v_screen_width, v_viewer_distance_from_screen, v_viewer_height;

	try
	{	
		v_screen_height = (screen_height->text()).toFloat();
		v_screen_width = (screen_width->text()).toFloat();
		v_viewer_distance_from_screen = (viewer_distance_from_screen->text()).toFloat();
		v_viewer_height = (viewer_height->text()).toFloat();
	}
	catch(...)
	{
		main_control->setStatusbarText((String)tr("Invalid Values!"));
		return;
	}

	if (   (v_screen_height < 0) || (v_screen_width < 0) 
			|| (v_viewer_distance_from_screen < 0)
			|| (v_viewer_height < 0))
	{
		Log.error() << (String)tr("Invalid values for StereoSettings: negative value(s).") << std::endl;
		return;
	}
	
	//TODO
	//compute the focal plane and eye distance

	//Camera& camera = Scene::getInstance(0)->getStage()->getCamera();
	//camera.setViewPoint(vp);
	//camera.setLookAtPosition(lp);
	accept();
}

// NAMESPACE
} 
}

#include <modernGLConfigDialog.h>
#include <ui_modernGLConfigDialog.h>


ModernGLConfigDialog::ModernGLConfigDialog(QWidget* parent, Qt::WindowFlags f)
	: BALL::VIEW::ConfigDialog(parent, f),
	  renderer_(0)
{
	ui_ = new Ui::OpenGLSettings;
	ui_->setupUi(this);
	registerWidgets_();
	setINIFileSectionName("ModernGLRendererPlugin");
}

ModernGLConfigDialog::~ModernGLConfigDialog()
{
	delete ui_;
}

void ModernGLConfigDialog::setVersion(QString version)
{
	ui_->version_label->setText(version);
}

void ModernGLConfigDialog::setGraphicsCard(QString graphics_card)
{
	ui_->renderer_label->setText(graphics_card);
}

void ModernGLConfigDialog::setDescription(QString description)
{
	ui_->description_label->setText(description);
}

void ModernGLConfigDialog::setVendor(QString vendor)
{
	ui_->vendor_label->setText(vendor);
}

void ModernGLConfigDialog::setRenderer(ModernGLRenderer* renderer)
{
	if(!renderer)
	{
		return;
	}

	renderer_ = renderer;
	connect(this, SIGNAL(ssaoParametersChanged(SSAOParameters)),
	        renderer, SLOT(ssaoParametersChanged(SSAOParameters)));
	renderer->ssaoParametersChanged(getSSAOParameters());

	connect(this, SIGNAL(transparentParametersChanged(TransparentParameters)),
	        renderer, SLOT(transparentParametersChanged(TransparentParameters)));
	renderer->transparentParametersChanged(getTransparentParameters());
}

void ModernGLConfigDialog::readPreferenceEntries(const BALL::INIFile& ini)
{
	PreferencesEntry::readPreferenceEntries(ini);

	emit ssaoParametersChanged(getSSAOParameters());
	emit transparentParametersChanged(getTransparentParameters());
}

SSAOParameters ModernGLConfigDialog::getSSAOParameters()
{
	SSAOParameters p;

	float sample_radius_scale = (float) ui_->sampleRadiusMax->text().toFloat() / (float) ui_->sampleRadiusSlider->maximum();
	float intensity_scale     = (float) ui_->intensityMax->text().toFloat()    / (float) ui_->intensitySlider->maximum();
	float scale_scale         = (float) ui_->scaleMax->text().toFloat()        / (float) ui_->scaleSlider->maximum();
	float bias_scale          = (float) ui_->biasMax->text().toFloat()         / (float) ui_->biasSlider->maximum();
	float downsampling_scale  = (float) ui_->downsamplingMax->text().toFloat() / (float) ui_->downsamplingSlider->maximum();

	p.useAO         = ui_->use_AO->isChecked();
	p.sample_radius = (float) ui_->sampleRadiusSlider->value() * sample_radius_scale;
	p.intensity     = (float) ui_->intensitySlider->value()    * intensity_scale;
	p.scale         = (float) ui_->scaleSlider->value()        * scale_scale;
	p.bias          = (float) ui_->biasSlider->value()         * bias_scale;
	p.downsampling  = (float) ui_->downsamplingSlider->value() * downsampling_scale;

	return p;
}

TransparentParameters ModernGLConfigDialog::getTransparentParameters()
{
	TransparentParameters u;

	u.useOQ = ui_->useOQCheckbox->isChecked();
	u.numPasses = ui_->numPassesSlider->value();

	return u;
}

void ModernGLConfigDialog::sampleRadiusSliderChanged()
{
	float sample_radius_scale = (float) ui_->sampleRadiusMax->text().toFloat() / (float) ui_->sampleRadiusSlider->maximum();
	float sample_radius = (float) ui_->sampleRadiusSlider->value() * sample_radius_scale;

	ui_->sampleRadiusValue->setNum(sample_radius);
}

void ModernGLConfigDialog::intensitySliderChanged()
{
	float intensity_scale = (float) ui_->intensityMax->text().toFloat() / (float) ui_->intensitySlider->maximum();
	float intensity = (float) ui_->intensitySlider->value()    * intensity_scale;

	ui_->intensityValue->setNum(intensity);
}

void ModernGLConfigDialog::scaleSliderChanged()
{
	float scale_scale = (float) ui_->scaleMax->text().toFloat() / (float) ui_->scaleSlider->maximum();
	float scale = (float) ui_->scaleSlider->value()    * scale_scale;

	ui_->scaleValue->setNum(scale);
}

void ModernGLConfigDialog::biasSliderChanged()
{
	float bias_scale = (float) ui_->biasMax->text().toFloat() / (float) ui_->biasSlider->maximum();
	float bias = (float) ui_->biasSlider->value()    * bias_scale;

	ui_->biasValue->setNum(bias);
}

void ModernGLConfigDialog::downsamplingSliderChanged()
{
	float downsampling_scale = (float) ui_->downsamplingMax->text().toFloat() / (float) ui_->downsamplingSlider->maximum();
	float downsampling = (float) ui_->downsamplingSlider->value()    * downsampling_scale;
	ui_->downsamplingValue->setNum(downsampling);
}

void ModernGLConfigDialog::numPassesSliderChanged()
{
	ui_->numPasses->setNum(ui_->numPassesSlider->value());
}


void ModernGLConfigDialog::storeValues()
{
	SSAOParameters p = getSSAOParameters();
	emit ssaoParametersChanged(p);

	TransparentParameters t = getTransparentParameters();
	emit transparentParametersChanged(t);
}


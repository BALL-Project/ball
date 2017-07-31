class LightSettings
	: Embeddable
{
%TypeHeaderCode
	#include <BALL/VIEW/DIALOGS/lightSettings.h>
%End
	public:

	LightSettings();
	static Size countInstances();
	static LightSettings* getInstance(Position);

	void update();
	void updateFromStage();
	void apply();
	virtual void restoreDefaultValues(bool /*all*/ = false);
	void restoreValues(bool all);

	public slots:
	virtual void addLightPressed();
	virtual void colorPressed();
	virtual void defaultsPressed();
	virtual void removeLightPressed();
 	virtual void typeSelected();
	virtual void intensityChanged();
	virtual void positionTypeChanged();

	private:
	LightSettings(const LightSettings& ls);
};

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

class LightSettings 
	: Embeddable
{ 
%TypeHeaderCode
	#include <BALL/VIEW/DIALOGS/lightSettings.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:

	LightSettings();

	// methods implemented by the BALL_EMBEDDED macro
	static Size countInstances();
	static LightSettings* getInstance(Position);

	/// Update the display of all fields
	void update();

	/// Get the values for lighting from the stage
	void updateFromStage();

	/// Apply the new values to the stage
	void apply();

	/// Called when defaults is pressed in Preferences, calls setDefaults
	virtual void restoreDefaultValues(bool /*all*/ = false);

	///
	void restoreValues(bool all);
	
	public slots:

	/// Slot for the AddLight button
  virtual void addLightPressed();

	/// Slot for the EditColor button, opens a color dialog
  virtual void colorPressed();
 	
	/// Resets the lighting
	virtual void defaultsPressed();
	
	/// Slot for selection in the listbox with lights
//	virtual void lightSelected(QListWidgetItem* current_item, QListWidgetItem* previous_item);
	
	/// Slot for the RemoveLight button
	virtual void removeLightPressed();

	/// Slot for type selection changed
 	virtual void typeSelected();

	/// Slot for changes of the intensity slider
	virtual void intensityChanged();

	///
	virtual void positionTypeChanged();

	private:
	LightSettings(const LightSettings& ls);
};

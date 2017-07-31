class DisplayProperties
	: ModularWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/DIALOGS/displayProperties.h>
%End
	public:

	static Size countInstances();
	static DisplayProperties* getInstance(Position);

	~DisplayProperties() throw();
	void createRepresentationMode();
	void modifyRepresentationMode(Representation*);
	Representation* createRepresentation(const PyCompositeList);
	bool getSettingsFromString(const String&) throw();
	void enableCreationForNewMolecules(bool) throw();
	void selectModel(int);
	void selectMode(int);
	void selectColoringMethod(int);
	void setSurfaceDrawingPrecision(float);
	void setDrawingPrecision(int);
	void setTransparency(int);
	void setCustomColor(const ColorRGBA&);
	void apply();
	void coloringOptionsPressed();
	void modelOptionsPressed();
	void precisionBoxChanged(int);
	void precisionSliderChanged();
	void coloringUpdatesChanged();
	void modelUpdatesChanged();

	private:
	DisplayProperties(const DisplayProperties&);
};

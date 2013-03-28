class Scene
	: Embeddable
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/scene.h>
%End
	public:
	static Size countInstances();
	static Scene* getInstance(Position);

	Scene();
	Scene(const Scene&);
	~Scene();

	void setCursor(String c);
	void setElementCursor(int);

	virtual void onNotify(Message*);
	virtual bool exportScene(SceneExporter&) const;
	virtual void initializeWidget(MainControl&);
	virtual void fetchPreferences(INIFile&);
	virtual void writePreferences(INIFile&);
	virtual void checkMenu(MainControl&);
	virtual bool isValid() const;
	Stage* getStage();
	Vector2 map3DToViewport(const Vector3& );
	Vector3 mapViewportTo3D(const Vector2& );
	Vector3 mapViewportTo3D(int , int );
	void merge(Composite* , Composite* );
	void deselect(bool );

	void setCamera(const Camera&);
	static void setAnimationSmoothness(float);
	static float getAnimationSmoothness();
	void setDefaultLighting(bool /*update_GL*/ = true);
	void initializePreferencesTab(Preferences&);
	void finalizePreferencesTab(Preferences&);
	virtual void applyPreferences();

	bool exportPNG(const String&);
	virtual void setWidgetVisible(bool);
	void setOffScreenRendering(bool, Size);
	void rotate(float /*degree_right*/, float /*degree_up*/);
	void rotateClockwise(float /*degree*/);
	void move(Vector3);
	void moveComposites(PyCompositeList, Vector3);
	void rotateComposites(PyCompositeList, float /*degree_right*/, float /*degree_up*/, float de/*gree_clockwise*/ = 0);
	bool isAnimationRunning() const throw();
	void setTurnPoint(const Vector3&);
	const Vector3& getTurnPoint() const;
	void setFullScreen(bool);
	void setFPSEnabled(bool);
	void showText(const String&, Size /*font_size*/ = 20);

	public Q_SLOTS:

	void createCoordinateSystem();
	void createCoordinateSystemAtOrigin();
	String exportPNG();
	void exportPOVRay();
	void exportNextPOVRay();
	void printScene();
	void showExportPNGDialog();
	void showExportVRMLDialog();
	virtual void switchShowWidget();
	void exitStereo();
	void enterStereo();
	void enterActiveStereo();
	void enterDualStereo();
	void clearRecordedAnimation();
	void startAnimation();
	void stopAnimation();
	void switchToLastMode();
	void switchShowGrid();
	void showInfos();
	void setupViewVolume();
	void createNewMolecule();
	void saturateWithHydrogens();
	void optimizeStructure();
	void setEditElementType(int element_number);
	int getEditElementType();
	void toggleContinuousLoop();
	void startContinuousLoop();
	void stopContinuousLoop();
	void applyStereoDefaults();
};

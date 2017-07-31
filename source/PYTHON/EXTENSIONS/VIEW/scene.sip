class Scene
	: Embeddable
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/scene.h>
%End
	private:
	Scene(const Scene&);

	public:

	static Size countInstances();
	static Scene* getInstance(Position);

	Scene() throw();
	~Scene() throw();

	void setCursor(String c);
	void setElementCursor(int);

	virtual void onNotify(Message*) throw();
	virtual bool exportScene(Renderer&) const throw();
	virtual void initializeWidget(MainControl&) throw();
	virtual void fetchPreferences(INIFile&) throw();
	virtual void writePreferences(INIFile&) throw();
	virtual void checkMenu(MainControl&) throw();
	virtual bool isValid() const throw();
	Stage* getStage() throw();
	Vector2 map3DToViewport(const Vector3& );
	Vector3 mapViewportTo3D(const Vector2& );
	Vector3 mapViewportTo3D(int , int );
	void merge(Composite* , Composite* );
	void deselect(bool );

	void setCamera(const Camera&) throw();
	static void setAnimationSmoothness(float) throw();
	static float getAnimationSmoothness() throw();
	void setDefaultLighting(bool /*update_GL*/ = true) throw();
	void initializePreferencesTab(Preferences&) throw();
	void finalizePreferencesTab(Preferences&) throw();
	virtual void applyPreferences() throw();

	bool exportPNG(const String&);
	virtual void setWidgetVisible(bool);
	void setOffScreenRendering(bool, Size);
	void rotate(float /*degree_right*/, float /*degree_up*/);
	void rotateClockwise(float /*degree*/);
	void move(Vector3);
	void moveComposites(PyCompositeList, Vector3);
	void rotateComposites(PyCompositeList, float /*degree_right*/,
		float /*degree_up*/, float de/*gree_clockwise*/ = 0);
	bool isAnimationRunning() const throw();
	void setTurnPoint(const Vector3&);
	const Vector3& getTurnPoint() const;
	void setFullScreen(bool);
	void setFPSEnabled(bool);
	void showText(const String&, Size /*font_size*/ = 20);

	public slots:

	void createCoordinateSystem() throw();
	void createCoordinateSystemAtOrigin() throw();
	String exportPNG();
	void exportPOVRay();
	void exportNextPOVRay();
	void printScene();
	void showExportPNGDialog();
	void showExportVRMLDialog();
	virtual void switchShowWidget() throw();
	void exitStereo() throw();
	void enterStereo();
	void enterActiveStereo() throw();
	void enterDualStereo() throw();
	void clearRecordedAnimation() throw();
	void startAnimation() throw();
	void stopAnimation() throw();
	void switchToLastMode() throw();
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

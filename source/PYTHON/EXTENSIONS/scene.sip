// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: scene.sip,v 1.2 2004/06/08 15:29:10 amoll Exp $
//

class Scene 
  : Embeddable
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/scene.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
//  BALL_EMBEDDABLE(Scene,);

// methods implemented by the BALL_EMBEDDED macro
static Size countInstances();
static Scene* getInstance(Position /index/);

  Scene() throw();
//  Scene(QWidget*, const char*, WFlags);
  Scene(const Scene&);
  Stage* getStage() throw();
  bool isValid() const throw();
//  const Scene& operator =  (const Scene&) throw();
  const Stage* getStage() const throw();
  static float getMouseSensitivity() throw();
  static void setMouseSensitivity(float) throw();
//  void applyPreferences(Preferences&) throw();
//  void cancelPreferences(Preferences&) throw();
//  void checkMenu(MainControl&) throw();
//  void clear() throw();
//  void defaultPreferences(Preferences&) throw();
		void exportPNG();
  	void exportScene(Renderer&) const throw();
//  void fetchPreferences(INIFile&) throw();
//  void finalizePreferencesTab(Preferences &preferences) throw();
//  void finalizeWidget(MainControl&) throw();
//  void initializePreferencesTab(Preferences &preferences) throw();
//  void initializeWidget(MainControl&) throw();
//  void onNotify(Message *message) throw();
//  void set(const Scene&) throw();
  void setDefaultLighting(bool);
  void update(bool);
	void enterDualStereo();
	void enterActiveStereo();
	void exitStereo();
//  void writePreferences(INIFile&) throw();
  ~Scene() throw();
};

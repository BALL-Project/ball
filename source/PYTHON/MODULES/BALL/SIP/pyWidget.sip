// $Id: pyWidget.sip,v 1.1 2000/07/18 21:28:20 oliver Exp $

class PyWidget
  // : public QMultiLineEdit,
  : ModularWidget
{
%HeaderCode
	#include <BALL/VIEW/OPENGL/WIDGETS/pyWidget.h>
	using namespace BALL;
	using namespace VIEW;
%End
 	// Q_OBJECT
  public:
	// BALL_EMBEDDABLE(PyWidget)
	// is expanded to:
	static Size countInstances();
	static PyWidget* getInstance(const String& /identifier/);
	static PyWidget* getInstance(Position /index/);
	
  // PyWidget(QWidget* /parent/ = 0, const char* /name/ = 0);
	PyWidget(const PyWidget& /widget/);
 	~PyWidget();
	
	// public slots:
	public:
	virtual void startInterpreter();
	virtual void stopInterpreter();
	virtual void initializeWidget(MainControl& /main_control/);
	virtual void finalizeWidget(MainControl& /main_control/);

	public:
  virtual void cursorUp(bool /mark/ = false);
  virtual void cursorDown(bool /mark/ = false);
 	virtual void newLine();
	virtual void cursorLeft(bool /mark/ = false, bool /wrap/ = true);
	virtual void backspace();
	virtual void cursorRight(bool /mark/ = false, bool /wrap/ = true);
	// virtual void mousePressEvent(QMouseEvent* m);
}; 

// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: mainControl.sip,v 1.5 2003/10/31 23:15:49 oliver Exp $
//

class MainControl
//   :  QMainWindow,
//    public ConnectionObject
	: Embeddable
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/mainControl.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
	// Q_OBJECT
	public:
	// typedef List<CompositeDescriptor *>::iterator DescriptorIterator;
	enum PopUpID
	{
		FILE = 10001,
		EDIT,
		BUILD,
		DISPLAY,
		TOOLS,
		HELP,
		USER
	};
	//MainControl(QWidget* /parent/ = 0, const char* /name/ = 0 , String /option_filename/ = ".BALL.options");
	MainControl(const MainControl&) throw();
	~MainControl() throw();
	virtual void clear() throw();
	//virtual void destroy() throw();
	// class OptionsFileError
	//	: public Exception::GeneralException
	// {
	//  	public:
	// 	OptionsFileError(const char* file, int line, const string& data);
	// };
	// MainControl& operator = (const MainControl& main_control);
	// void get
	// 	(MainControl& main_control, bool deep = true) const;
	// void swap(MainControl& main_control);
	// void insert_
	//	(Composite* composite,
	// const String& name = "unkown");
	bool update(const Composite& /composite/);
	void updateAll();
	// virtual Options* getOptions();
	//	virtual void onNotify(Message *message);
	virtual bool isValid() const throw();
	// virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
	// virtual void read(std::istream& s);
	// virtual void write(std::ostream& s) const;
	// static MainControl* getMainControl(const QWidget* widget);
	//static int insertMenuEntry
	//	(int ID, const String& name, const QWidget* mc_child, const QObject* receiver, 
	//  const char* slot, int accel = 0, int entry_ID = -1);
	//int insertMenuEntry
	//	(int ID, const String& name, const QObject* receiver, 
	// 	 const char* slot, int accel = 0, int entry_ID = -1);
	// int getNextID();
	// virtual QPopupMenu* initPopupMenu(int ID);


	// methods implemented by the BALL_EMBEDDED macro
	static Size countInstances();
	static MainControl* getInstance(Position /index/);
};

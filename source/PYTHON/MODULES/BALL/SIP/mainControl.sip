// $Id: mainControl.sip,v 1.4 2001/05/07 00:58:11 oliver Exp $

class MainControl
//   :  QMainWindow,
//    public ConnectionObject
	: Embeddable
{
%HeaderCode
	#include <BALL/VIEW/GUI/KERNEL/mainControl.h>
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
	MainControl(const MainControl&);
	~MainControl();
	virtual void clear();
	virtual void destroy();
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
	CompositeDescriptor* insert
		(const Composite& /composite/,
		 const String& /name/ = "unkown",
			const Vector3& /center/ = Vector3(0.0, 0.0, 0.0));
	bool remove(const Composite& /composite/);
	bool remove(const CompositeDescriptor& /composite_descriptor/);
 	// List<CompositeDescriptor*>& getDescriptorList();
 	PyCompositeDescriptorList getDescriptorList();
	// const List<CompositeDescriptor*>& getDescriptorList() const;
	bool setName
		(const Composite& /composite/, const String& /name/);
	String* getName(const Composite& /composite/);
	const String* getName(const Composite& /composite/) const;
	CompositeDescriptor* getDescriptor(const String& /name/);
	const CompositeDescriptor* getDescriptor(const String& /name/) const;
	bool setCenter
		(const Composite& /composite/, const Vector3& /v/);
	Vector3* getCenter(const Composite& /composite/);
	const Vector3* getCenter(const Composite& /composite/) const;
	CompositeDescriptor* getDescriptor(const Vector3& /center/);
	const CompositeDescriptor* getDescriptor(const Vector3& /center/) const;
	bool update(const Composite& /composite/);
	void updateAll();
	// virtual Options* getOptions();
	//	virtual void onNotify(Message *message);
	bool isInserted(const Composite& /composite/) const;
	bool isInserted(const CompositeDescriptor& /compositeDescriptor/) const;
	virtual bool isValid() const;
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

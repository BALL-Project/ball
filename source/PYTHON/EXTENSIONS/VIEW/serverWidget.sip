class ServerWidget
	: ModularWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/serverWidget.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL;
	using namespace BALL::VIEW;
	using namespace BALL::Exception;
%End
  public:

	// methods implemented by the BALL_EMBEDDED macro
	static Size countInstances();
	static ModularWidget* getInstance(Position /index/);

	//ServerWidget(QWidget* parent = 0, const char* name = 0) throw();
	~ServerWidget() throw();
	virtual void clear() throw();

	class NotCompositeObject
		:	GeneralException
	{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/serverWidget.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
		public:

			NotCompositeObject(const char*, int) throw();
	};

	void activate()	throw();
	void deactivate() throw();
	void setPort(const int) throw();
	int getPort() const throw();
	//void registerObjectCreator(const ObjectCreator& ) throw();
	//void unregisterObjectCreator() throw();
	virtual void initializeWidget(MainControl&) throw();
	virtual void finalizeWidget(MainControl&) throw();
	virtual void initializePreferencesTab(Preferences&) throw();
	virtual void finalizePreferencesTab(Preferences&) throw();
	virtual void applyPreferences() throw();
	virtual bool isValid() const throw();
	//virtual void dump(std::ostream& s = std::cout, Size = 0) const throw();

	private:
		void operator = (const ServerWidget&);
};

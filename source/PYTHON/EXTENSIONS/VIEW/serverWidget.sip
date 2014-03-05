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
	static ModularWidget* getInstance(Position);

	//ServerWidget(QWidget* parent = 0, const char* name = 0) throw();
	~ServerWidget() throw();
	virtual void clear() throw();

	%Exception ServerWidget::NotCompositeObject(GeneralException) /PyName=ServerWidget_NotCompositeObject/
	{
%TypeHeaderCode
#include <BALL/DATATYPE/string.h>
#include <BALL/COMMON/exception.h>
#include <BALL/VIEW/KERNEL/serverWidget.h>
using namespace BALL;
using namespace BALL::VIEW;
using namespace BALL::Exception;
%End
%RaiseCode
		String detail = 
			(String)(sipExceptionRef.getName()) + " in line " + String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
																				 + ": " + sipExceptionRef.getMessage();

    SIP_BLOCK_THREADS
    PyErr_SetString(sipException_ServerWidget_NotCompositeObject, detail.c_str());
    SIP_UNBLOCK_THREADS
%End
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
		ServerWidget(const ServerWidget&);
};

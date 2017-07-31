class ServerWidget
	: ModularWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/KERNEL/serverWidget.h>
%End
	public:

	static Size countInstances();
	static ModularWidget* getInstance(Position);

	~ServerWidget() throw();
	virtual void clear() throw();

	%Exception ServerWidget::NotCompositeObject(SIP_Exception) /PyName=ServerWidget_NotCompositeObject/
	{
%TypeHeaderCode
		#include <BALL/VIEW/KERNEL/serverWidget.h>
%End
%RaiseCode
		String detail =
			(String)(sipExceptionRef.getName()) + " in line "
			+ String(sipExceptionRef.getLine()) + " of " + sipExceptionRef.getFile()
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
	virtual void initializeWidget(MainControl&) throw();
	virtual void finalizeWidget(MainControl&) throw();
	virtual void initializePreferencesTab(Preferences&) throw();
	virtual void finalizePreferencesTab(Preferences&) throw();
	virtual void applyPreferences() throw();
	virtual bool isValid() const throw();

	private:
	ServerWidget(const ServerWidget&);
};

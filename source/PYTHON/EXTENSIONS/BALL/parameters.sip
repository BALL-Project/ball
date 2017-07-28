class Parameters
{
%TypeHeaderCode
	#include <BALL/FORMAT/parameters.h>
%End
	public:
	Parameters();
	Parameters(const String&);
	Parameters(const Parameters&);
	virtual ~Parameters();
	virtual void clear() throw();

	void setFilename(const String&);
	const String& getFilename() const;
	INIFile& getParameterFile();
	bool init();
	virtual bool isValid() const;
	bool operator == (const Parameters&) const;
};

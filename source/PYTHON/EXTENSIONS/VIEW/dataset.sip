class Dataset
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/dataset.h>
%End
	public:
	Dataset();
	Dataset(const Dataset&);
	virtual ~Dataset();
	virtual void clear();
	void set(const Dataset&);
	void setName(String name);
	String getName() const;
	void setType(String type);
	String getType() const;
	void setComposite(Composite*);
	Composite* getComposite() const;
};

class DatasetController
	: Embeddable
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/dataset.h>
%End
	public:
	static Size countInstances();
	static DatasetController* getInstance(Position);
	DatasetController();
	DatasetController(DatasetController& );
	virtual ~DatasetController() throw();
	void setDatasetControl(DatasetControl* );
	DatasetControl* getDatasetControl();
	virtual bool insertDataset(Dataset* /Transfer/);
	virtual bool deleteDataset(Dataset* );
	virtual bool createMenuEntries();
	PyDatasetVector getDatasets();
	PyDatasetVector getSelectedDatasets();
	Dataset* getSelectedDataset();
	bool hasDataset(Dataset* );
	String getType();
	virtual void checkMenu(MainControl&);

	public Q_SLOTS:
	virtual bool write();
	virtual bool open();
	virtual bool deleteDatasets();
	virtual bool deleteDataset();
};

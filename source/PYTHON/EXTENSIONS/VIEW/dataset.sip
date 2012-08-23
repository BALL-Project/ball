// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Dataset
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/dataset.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
	Dataset();
	Dataset(const Dataset&);
	virtual ~Dataset();
	virtual void clear();
	void set(const Dataset&);
	// const Dataset& operator = (const Dataset&);
	void setName(String name);
	String getName() const;
	void setType(String type);
	String getType() const;
	void setComposite(Composite*);
	Composite* getComposite() const;
	//void dump(std::ostream& s, Size depth) const;
};

class DatasetController
	: Embeddable
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/dataset.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
	
	public:

	static Size countInstances();
	static DatasetController* getInstance(Position);

	DatasetController();
	DatasetController(DatasetController& );
	virtual ~DatasetController() throw();
	void setDatasetControl(DatasetControl* );
	DatasetControl* getDatasetControl();
	//virtual bool handle(DatasetMessage* );
	virtual bool insertDataset(Dataset* /Transfer/);
	virtual bool deleteDataset(Dataset* );
	virtual bool createMenuEntries();
	//virtual QMenu* buildContextMenu(QTreeWidgetItem* );
	PyDatasetVector getDatasets();
	PyDatasetVector getSelectedDatasets();
	Dataset* getSelectedDataset();
	bool hasDataset(Dataset* );
	// bool hasItem(QTreeWidgetItem* );
	// Dataset* getDataset(QTreeWidgetItem* );
	String getType();
	virtual void checkMenu(MainControl&);
	//vector<String> getSupportedFileFormats();
	public slots:
	virtual bool write();
	virtual bool open();
	virtual bool deleteDatasets();
	virtual bool deleteDataset();
};


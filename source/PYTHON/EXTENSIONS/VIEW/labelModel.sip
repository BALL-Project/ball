class LabelModel
	: ModelProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/labelModel.h>
%End
	public:

	enum Mode
	{
		ALL_ITEMS,
		ALL_ATOMS,
		ALL_RESIDUES,
		ONE_LABEL
	};

	LabelModel() throw();
	LabelModel(const LabelModel&) throw();
	~LabelModel() throw();
	void setColor(const ColorRGBA&) throw();
	const ColorRGBA& getColor() const throw();
	void setMode(Mode) throw();
	Mode getType() const throw();
	void setText(const String&);
	const String& getText() const;
	virtual bool createGeometricObjects()  throw();
};

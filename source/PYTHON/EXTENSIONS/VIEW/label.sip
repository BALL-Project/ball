class Label
	: GeometricObject,
	  Vertex
{
%TypeHeaderCode
	#include <BALL/VIEW/PRIMITIVES/label.h>
%End
	public:
	Label() throw();
	Label(const Label&) throw();
	String getText() const throw();
	bool isValid() const throw();
	void clear() throw();
	void set(const Label&) throw();
	void setText(const String&) throw();
	void swap(Label&) throw();
	void setFontSize(Size);
	~Label() throw();
};

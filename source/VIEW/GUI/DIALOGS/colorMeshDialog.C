#include <BALL/VIEW/GUI/DIALOGS/colorMeshDialog.h>
#include <BALL/DATATYPE/regularData3D.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/VIEW/DATATYPE/colorTable.h>
#include <BALL/VIEW/GUI/KERNEL/mainControl.h>
#include <BALL/SYSTEM/path.h>

#include <qlineedit.h>
#include <qfiledialog.h>
#include <qspinbox.h>

using namespace BALL;
using namespace VIEW;

/* 
 *  Constructs a ColorMeshDialog which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ColorMeshDialog::ColorMeshDialog( QWidget* parent,  const char* name, bool modal, WFlags fl )
    : colorMeshDialogData( parent, name, modal, fl )
{
	connect((QObject*)Compute_, SIGNAL(clicked()), this, SLOT(colorMesh()));
	connect((QObject*)Browseload, SIGNAL(clicked()), this, SLOT(browseLoadFiles()));
}

ColorMeshDialog::~ColorMeshDialog()
{
 // no need to delete child widgets, Qt does it all for us
}

void ColorMeshDialog::colorMesh()
{
	String fileName(Loadfile_->text().latin1());

	if (fileName.isEmpty())
	{
		return;
	}

	RegularData3D dat;
	File infile(fileName, File::IN);
	infile >> dat;
	infile.close();

	// now do the colorizing stuff...
	mesh->colorList.resize(mesh->vertex.size());
	ColorRGBA list[3];
	list[0] = ColorRGBA(0.,0.,1.,1.);
	list[1] = ColorRGBA(0.,1.,1.,1.);
	list[2] = ColorRGBA(1.,1.,0.,1.);

	ColorTable table(list, 3);
	table.setNumberOfColors(numberOfColors->value());
	table.createTable();
	table.setRange(atof(MinVal->text().latin1()), atof(MaxVal->text().latin1()));

	try 
	{
		for (Position i=0; i<mesh->colorList.size(); i++)
		{
			mesh->colorList[i] = table.map(dat[mesh->vertex[i]]);
		}
	}	
	catch (Exception::OutOfGrid)
	{
		Log.error() << "Error! There is a point contained in the surface that is not "
								<< "inside the grid! Aborting the coloring..." << std::endl;
		return;
	}
	
	// update of the scene and the composites needed
	MainControl::getMainControl(this)->updateAll();
}

void ColorMeshDialog::browseLoadFiles()
{
	// Look up the full path of the file
	Path p;
	String filename = p.find(Loadfile_->text().latin1());
	if (filename == "")
	{
		filename = Loadfile_->text().latin1();
	}
	QString result = QFileDialog::getOpenFileName(QString(filename.c_str()), "*", 0, "Select a RegularData file");
	if (!result.isEmpty())
	{
		// store the new filename in the lineedit field
		Loadfile_->setText(result);
	}
}

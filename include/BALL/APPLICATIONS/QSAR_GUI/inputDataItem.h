#ifndef INPUTDATAITEM_H
#define INPUTDATAITEM_H

#include <BALL/APPLICATIONS/QSAR_GUI/dataItem.h>
#include <BALL/QSAR/QSARData.h>

#include <QtGui/QGraphicsSceneMouseEvent>


using namespace BALL::QSAR;

namespace BALL
{
	namespace VIEW
	{
		class InputPlotter;
		
		/** @class InputDataItem
		* @brief class for the representation of input data
		*
		* @todo
		*/
		class InputDataItem : public DataItem
		{
			public:
				/** @name Constructors and Destructors
				*/
				virtual ~InputDataItem();

				InputDataItem(QString filename, bool center_data, bool center_y, DataItemView* view);
				//InputDataItem(QString filename, QString name, DataItemView* view);
				InputDataItem(QString filename, DataItemView* view);
				InputDataItem(InputDataItem& item);
				InputDataItem();

				/** @name Accessors
				*/
				
				/** Returns a pointer to the data saved in the item*/
				QSARData* data();
			//	enum { Type = UserType + 2 };
		
				/** Returns the type of the item as an int. This type information is used by qgraphicsitem_cast() to distinguish between types. */
				int type() const { return Type; }

				QString filename();
				bool centerData();
				bool centerY();
				void setCenterDataFlag(bool cd);
				void setCenterResponseFlag(bool cr);
				void setData(QSARData* data);
				void setFilename(String filename) { filename_=filename.c_str();}
				void setAppend(bool append);
				bool append();
				
				void showPlotter();
				
				/** load preprocessed input data from a file */
				void loadFromFile(String file);

				virtual bool checkForDiscreteY() = 0;

			protected:
				/** @name Attributes
				*/
		
				/**pointer to an instance of QSARData that holds the input data read from a file */
				QSARData* data_;
				QString filename_;
				bool center_data_;
				bool center_y_;
				
				/** is the data of this InputDataItem append to another InputDataItem?? */
				bool append_;
				
				InputPlotter* input_plotter_;
		};
	}
}	

#endif

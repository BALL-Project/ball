/* dataItemView.h
 * 
 * Copyright (C) 2009 Marcel Schumann
 * 
 * This file is part of QuEasy -- A Toolbox for Automated QSAR Model
 * Construction and Validation.
 * QuEasy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * QuEasy is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DATAITEMVIEW_H
#define DATAITEMVIEW_H

#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsScene>

#include <BALL/APPLICATIONS/QuEasyViz/dataItemScene.h>




namespace BALL
{
	namespace VIEW
	{

		class MainWindow;
		class DataItem;
		class Edge;
		
		/** @class DataItemView
		* @brief class for viewing all DataItems
		*
		* @todo
		*/
		class DataItemView : public QGraphicsView
		{
		Q_OBJECT
		
		public:
			/** @name Constructors and Destructors */

			/** constructor
			* @param scene the scene connected to this view which contains the items
			* @param mw the main windwo to which this View belongs
			*/
			DataItemView(DataItemScene* scene, MainWindow* mw);


			/** @name Attributes */

			/** the scene*/
			DataItemScene* data_scene;

			/** name of the widget for which this object is used,i.e. "view", "model_list" or "source_list" */
			std::string name; 

			void addDropSite();
			
			void scaleView(qreal scaleFactor);
		

		
		protected:

			/** @name Event Handlers 
			* reimplemented event handlers of this class
			*/

			/** event handler for mouse scroll events 
			* needed for scaling the view
			*/
			void wheelEvent(QWheelEvent* event);

			/** event handler for keyboard key events 
			* used for moving the items on the view
			*/
			void keyPressEvent(QKeyEvent* event);
			
		
		private:
			int timer_id_;
		
		};
	}
}	

 #endif


#include <BALL/VIEW/WIDGETS/SDWidget.h>

#include <BALL/VIEW/KERNEL/iconLoader.h>

#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/STRUCTURE/sdGenerator.h>

#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtGui/QStyleOptionFocusRect>
#include <QtGui/QAction>

#include <QtGui/QFileDialog>
#include <QtGui/QImageWriter>

#include <set>

namespace BALL
{
	namespace VIEW
	{
		const char* SDWidget::Option::SHOW_HYDROGENS = "sd_widget_show_hydrogens";
		const bool  SDWidget::Default::SHOW_HYDROGENS = false;

		SDWidget::SDWidget(QWidget *parent, bool show_hydrogens)
			: QWidget(parent)
		{
			setup_();

			options[SDWidget::Option::SHOW_HYDROGENS] = show_hydrogens;
		}

		SDWidget::SDWidget(const System& system, QWidget *parent)
			: QWidget(parent)
		{
			setup_();

			plot(system);
		}

		void SDWidget::setup_()
		{
			upper_ = Vector3( 5.0f);
			lower_ = Vector3(-5.0f);
			setDefaultOptions();
			setBackgroundRole(QPalette::Base);

			//Todo: Add a nice icon
			QAction* export_image = new QAction(tr("Export image"), this);

			export_image->setIcon(IconLoader::instance().getIcon("actions/document-save"));

			addAction(export_image);
			connect(export_image, SIGNAL(triggered()), this, SLOT(exportImage_()));
		}

		SDWidget::~SDWidget()
		{}
		
		void SDWidget::plot(const System& system, bool create_sd)
		{
			system_ = system;

			if (create_sd)
			{
				SDGenerator sdg;
				sdg.generateSD(system_);
			}

			update();
		}

		void SDWidget::paintEvent(QPaintEvent *)
		{
			drawFrame_();
			renderSD_(this);
		}

		void SDWidget::drawFrame_()
		{
			QPainter p(this);

			QStyleOptionFrame opt;
			opt.initFrom(this);
			opt.state |= QStyle::State_Sunken;
			QRect erase_area = rect();

			int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth, &opt, this) + 1;
			erase_area.adjust(frameWidth, frameWidth, -frameWidth, -frameWidth);
			p.eraseRect(erase_area);
			style()->drawPrimitive(QStyle::PE_Frame, &opt, &p, this);

			p.end();
		}

		void SDWidget::renderSD_(QPaintDevice* pd)
		{
			if (!pd)
			{
				return;
			}

			BoundingBoxProcessor bp;
			system_.apply(bp);
			upper_ = bp.getUpper()*1.1;
			lower_ = bp.getLower()*1.1;

			GeometricCenterProcessor gcp;
			system_.apply(gcp);
			Vector3 center = gcp.getCenter();

			float xscale = pd->width()  / (upper_.x - lower_.x);
			float yscale = pd->height() / (upper_.y - lower_.y);

			xscale = yscale = std::min(xscale, yscale);

			QPainter painter(pd);

			QPen pen(palette().foreground().color(), 3, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
			painter.setPen(pen);
			painter.setRenderHint(QPainter::Antialiasing, true);
			painter.translate(pd->width()/2, pd->height()/2);

		  QFont newFont = font();
 		  newFont.setPixelSize(12);
			painter.setFont(newFont);
 
			QFontMetrics fontMetrics(newFont);
			
			// check if hydrogen atoms should be shown!
			bool show_H = options.getBool(Option::SHOW_HYDROGENS);

			AtomIterator at_it = system_.beginAtom();
			std::set<Atom*> already_seen;
			for (; +at_it; ++at_it)
			{
				if ((at_it->getElement() == PTE[Element::H]) && !show_H) continue;
				already_seen.insert(&(*at_it));	

				Atom::BondIterator b_it = at_it->beginBond();

				Vector3 from3d = at_it->getPosition() - center;
				QPointF from2d(from3d.x*xscale, from3d.y*yscale);

				for (; +b_it; ++b_it)
				{
					Atom* partner = b_it->getPartner(*at_it);
					
					if (
								 // treat each bond only once
								 (std::find(already_seen.begin(), already_seen.end(), partner) == already_seen.end()) 
								 // we don't draw hydrogens
							&& !((partner->getElement() == PTE[Element::H]) && !show_H)
						 )
					{
						Vector3 to3d = partner->getPosition() - center;
						QPointF to2d(to3d.x*xscale, to3d.y*yscale);
						QPointF shifted_from = from2d;

						// do we need to draw a character for the "to" atom?
						if (partner->getElement() != PTE[Element::C])
						{
							QRect br = fontMetrics.boundingRect(partner->getElement().getSymbol().c_str());
							QPointF label_pos(to2d.x() - br.width()/2, to2d.y() + br.height()/2 - 4);

							painter.drawText(label_pos, partner->getElement().getSymbol().c_str());	

							QRectF bounding_box(to2d.x() - br.width()/2  - 3, 
																	to2d.y() - br.height()/2 - 4, 
																	br.width() + 6, br.height() + 6);
							to2d = getEndpoint_(bounding_box, to2d, from2d, false);

						}

						// and do we need to draw a character for the "from" atom?
						if (at_it->getElement() != PTE[Element::C])
						{
							QRect br = fontMetrics.boundingRect(at_it->getElement().getSymbol().c_str());

							QRectF bounding_box(from2d.x() - br.width()/2  - 3, 
																	from2d.y() - br.height()/2 - 4, 
																	br.width() + 6, br.height() + 6);
							shifted_from = getEndpoint_(bounding_box, to2d, from2d, true);
						}

						// don't draw if the points are too close to each other (Qt tends to crash if this happens...)
						if ((to2d - shifted_from).toPoint().manhattanLength() == 0)
							continue;

						// do we need to draw a double bond?
						if (b_it->getOrder() == Bond::ORDER__DOUBLE)
						{
							// compute two lines, slightly shifted wrt the normal of this line
							QLineF bond_line  = QLineF(shifted_from, to2d);
							QLineF normal     = bond_line.normalVector();
							normal = normal.unitVector();

							QPointF shift;
							shift.setX(2*(normal.p2().x() - bond_line.p1().x()));
							shift.setY(2*(normal.p2().y() - bond_line.p1().y()));

							QLineF first_line = bond_line;
							first_line.translate(shift);

							shift.setX(-1.*shift.x());
							shift.setY(-1.*shift.y());

							QLineF second_line = bond_line;
							second_line.translate(shift);

							pen.setWidth(2);
							painter.setPen(pen);
							painter.drawLine(first_line);	
							painter.drawLine(second_line);	
							pen.setWidth(3);
							painter.setPen(pen);
						}
						else if (b_it->getOrder() == Bond::ORDER__TRIPLE)
						{
							// compute two lines, slightly shifted wrt the normal of this line
							QLineF bond_line  = QLineF(shifted_from, to2d);
							QLineF normal     = bond_line.normalVector();
							normal = normal.unitVector();

							QPointF shift;
							shift.setX(3*(normal.p2().x() - bond_line.p1().x()));
							shift.setY(3*(normal.p2().y() - bond_line.p1().y()));

							QLineF first_line = bond_line;
							first_line.translate(shift);

							shift.setX(-1.*shift.x());
							shift.setY(-1.*shift.y());

							QLineF second_line = bond_line;
							second_line.translate(shift);

							pen.setWidth(2);
							painter.setPen(pen);
							painter.drawLine(first_line);	
							painter.drawLine(bond_line);
							painter.drawLine(second_line);	
							pen.setWidth(3);
							painter.setPen(pen);
						}
						else // assume it's a single bond...
							painter.drawLine(shifted_from, to2d);
					}
				}
				// do we need to draw a character for the "from" atom?
				if (at_it->getElement() != PTE[Element::C])
				{
					QRect br = fontMetrics.boundingRect(at_it->getElement().getSymbol().c_str());
					QPointF label_pos(from2d.x() - br.width()/2, from2d.y() + br.height()/2 - 4);

					painter.drawText(label_pos, at_it->getElement().getSymbol().c_str());	
				}	
			}
		}

		QPointF SDWidget::getEndpoint_(QRectF& character_boundary, QPointF from, QPointF to, bool character_is_from)
		{
			// compute all relevant lines...

			// the line from "from" to "to" :-)
			QLineF line_from_to(from, to);

			// the upper part of the box
			QLineF upper(character_boundary.topLeft(), character_boundary.topRight());
			// the right part of the box
			QLineF right(character_boundary.topRight(), character_boundary.bottomRight());
			// the lower part of the box
			QLineF lower(character_boundary.bottomRight(), character_boundary.bottomLeft());
			// and finally the left part
			QLineF left(character_boundary.bottomLeft(), character_boundary.topLeft());

			// intersect them
			QPointF intersection_point;
			if (line_from_to.intersect(upper, &intersection_point) == QLineF::BoundedIntersection)
				return intersection_point;
			if (line_from_to.intersect(right, &intersection_point) == QLineF::BoundedIntersection)
				return intersection_point;
			if (line_from_to.intersect(lower, &intersection_point) == QLineF::BoundedIntersection)
				return intersection_point;
			if (line_from_to.intersect(left, &intersection_point) == QLineF::BoundedIntersection)
				return intersection_point;

			//If to and from are too close, the computed intersection is incorrect.
			//In this case just return the point not having the character
			return character_is_from ? from : to;
		}

		void SDWidget::clear()
		{
			upper_ = Vector3( 5.0f);
			lower_ = Vector3(-5.0f);
			system_.clear();
			update();
		}

		QSize SDWidget::sizeHint() const
		{
			Vector3 diff = upper_ - lower_;
			return QSize(diff.x * 10, diff.y * 10);
		}

		void SDWidget::setDefaultOptions()
		{
	 		options.setDefaultBool(SDWidget::Option::SHOW_HYDROGENS,
	 												 	 SDWidget::Default::SHOW_HYDROGENS);
		}

		void SDWidget::exportImage_()
		{
			QString file = QFileDialog::getSaveFileName(this, tr("Export image"), QString(), "Images (*.png *.xpm *.jpg *.bmp *.gif)");

			if(file != QString::null)
			{
				QImage image(width(), height(), QImage::Format_ARGB32);
				image.fill(0);
				renderSD_(&image);

				image.save(file);
			}
		}
	}
}

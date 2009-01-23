/********************************************************************************
** Form generated from reading ui file 'raytraceableContourSurfaceDialog.ui'
**
** Created: Fri Jan 23 16:04:01 2009
**      by: Qt User Interface Compiler version 4.4.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef RAYTRACEABLECONTOURSURFACEDIALOGDATA_H
#define RAYTRACEABLECONTOURSURFACEDIALOGDATA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>

QT_BEGIN_NAMESPACE

class Ui_RaytraceableContourSurfaceDialogData
{
public:
    QGroupBox *groupBoxValue;
    QSlider *ContourSurfaceThresholdSlider;
    QLineEdit *ContourSurfaceThresholdLineEdit;
    QPushButton *add_button;
    QPushButton *OK_button;
    QPushButton *cancel_button;
    QGroupBox *groupBoxValue_2;
    QLabel *RangeLabel_min;
    QLabel *RangeLabel_max;
    QLabel *RangeLabel_steps;
    QPushButton *autoRangeButton;
    QLineEdit *RangeLineEdit_steps;
    QLineEdit *RangeLineEdit_max;
    QLineEdit *RangeLineEdit_min;

    void setupUi(QDialog *RaytraceableContourSurfaceDialogData)
    {
    if (RaytraceableContourSurfaceDialogData->objectName().isEmpty())
        RaytraceableContourSurfaceDialogData->setObjectName(QString::fromUtf8("RaytraceableContourSurfaceDialogData"));
    RaytraceableContourSurfaceDialogData->setEnabled(true);
    RaytraceableContourSurfaceDialogData->resize(372, 249);
    groupBoxValue = new QGroupBox(RaytraceableContourSurfaceDialogData);
    groupBoxValue->setObjectName(QString::fromUtf8("groupBoxValue"));
    groupBoxValue->setGeometry(QRect(10, 10, 351, 141));
    groupBoxValue->setAlignment(Qt::AlignHCenter);
    ContourSurfaceThresholdSlider = new QSlider(groupBoxValue);
    ContourSurfaceThresholdSlider->setObjectName(QString::fromUtf8("ContourSurfaceThresholdSlider"));
    ContourSurfaceThresholdSlider->setGeometry(QRect(20, 50, 201, 20));
    ContourSurfaceThresholdSlider->setOrientation(Qt::Horizontal);
    ContourSurfaceThresholdLineEdit = new QLineEdit(groupBoxValue);
    ContourSurfaceThresholdLineEdit->setObjectName(QString::fromUtf8("ContourSurfaceThresholdLineEdit"));
    ContourSurfaceThresholdLineEdit->setGeometry(QRect(250, 50, 51, 24));
    add_button = new QPushButton(groupBoxValue);
    add_button->setObjectName(QString::fromUtf8("add_button"));
    add_button->setWindowModality(Qt::WindowModal);
    add_button->setGeometry(QRect(14, 100, 91, 26));
    OK_button = new QPushButton(groupBoxValue);
    OK_button->setObjectName(QString::fromUtf8("OK_button"));
    OK_button->setWindowModality(Qt::WindowModal);
    OK_button->setGeometry(QRect(130, 100, 91, 26));
    cancel_button = new QPushButton(groupBoxValue);
    cancel_button->setObjectName(QString::fromUtf8("cancel_button"));
    cancel_button->setWindowModality(Qt::WindowModal);
    cancel_button->setGeometry(QRect(250, 100, 91, 26));
    groupBoxValue_2 = new QGroupBox(RaytraceableContourSurfaceDialogData);
    groupBoxValue_2->setObjectName(QString::fromUtf8("groupBoxValue_2"));
    groupBoxValue_2->setGeometry(QRect(10, 160, 351, 81));
    groupBoxValue_2->setAlignment(Qt::AlignHCenter);
    RangeLabel_min = new QLabel(groupBoxValue_2);
    RangeLabel_min->setObjectName(QString::fromUtf8("RangeLabel_min"));
    RangeLabel_min->setGeometry(QRect(30, 20, 31, 20));
    RangeLabel_max = new QLabel(groupBoxValue_2);
    RangeLabel_max->setObjectName(QString::fromUtf8("RangeLabel_max"));
    RangeLabel_max->setGeometry(QRect(110, 20, 31, 20));
    RangeLabel_steps = new QLabel(groupBoxValue_2);
    RangeLabel_steps->setObjectName(QString::fromUtf8("RangeLabel_steps"));
    RangeLabel_steps->setGeometry(QRect(190, 20, 31, 20));
    autoRangeButton = new QPushButton(groupBoxValue_2);
    autoRangeButton->setObjectName(QString::fromUtf8("autoRangeButton"));
    autoRangeButton->setWindowModality(Qt::WindowModal);
    autoRangeButton->setGeometry(QRect(260, 40, 75, 26));
    RangeLineEdit_steps = new QLineEdit(groupBoxValue_2);
    RangeLineEdit_steps->setObjectName(QString::fromUtf8("RangeLineEdit_steps"));
    RangeLineEdit_steps->setGeometry(QRect(180, 40, 51, 24));
    RangeLineEdit_max = new QLineEdit(groupBoxValue_2);
    RangeLineEdit_max->setObjectName(QString::fromUtf8("RangeLineEdit_max"));
    RangeLineEdit_max->setGeometry(QRect(100, 40, 51, 24));
    RangeLineEdit_min = new QLineEdit(groupBoxValue_2);
    RangeLineEdit_min->setObjectName(QString::fromUtf8("RangeLineEdit_min"));
    RangeLineEdit_min->setGeometry(QRect(20, 40, 51, 24));

    retranslateUi(RaytraceableContourSurfaceDialogData);

    QMetaObject::connectSlotsByName(RaytraceableContourSurfaceDialogData);
    } // setupUi

    void retranslateUi(QDialog *RaytraceableContourSurfaceDialogData)
    {
    RaytraceableContourSurfaceDialogData->setWindowTitle(QApplication::translate("RaytraceableContourSurfaceDialogData", "Ray-traced Contour Surface Options", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    RaytraceableContourSurfaceDialogData->setToolTip(QString());
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    groupBoxValue->setToolTip(QApplication::translate("RaytraceableContourSurfaceDialogData", "add contour surface", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    groupBoxValue->setTitle(QApplication::translate("RaytraceableContourSurfaceDialogData", "Contour Threshold Value", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    ContourSurfaceThresholdSlider->setToolTip(QApplication::translate("RaytraceableContourSurfaceDialogData", "choose a contour Surface threshold of value", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_WHATSTHIS
    ContourSurfaceThresholdSlider->setWhatsThis(QApplication::translate("RaytraceableContourSurfaceDialogData", "slider to chose a threshold of a new contour surface", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS


#ifndef QT_NO_ACCESSIBILITY
    ContourSurfaceThresholdSlider->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY


#ifndef QT_NO_ACCESSIBILITY
    ContourSurfaceThresholdSlider->setAccessibleDescription(QString());
#endif // QT_NO_ACCESSIBILITY


#ifndef QT_NO_TOOLTIP
    ContourSurfaceThresholdLineEdit->setToolTip(QApplication::translate("RaytraceableContourSurfaceDialogData", "contour surface threshold value", "contour surface threshold value", QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    add_button->setToolTip(QString());
#endif // QT_NO_TOOLTIP

    add_button->setText(QApplication::translate("RaytraceableContourSurfaceDialogData", "add", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    OK_button->setToolTip(QApplication::translate("RaytraceableContourSurfaceDialogData", "add contour surface and close", "add a contour", QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    OK_button->setText(QApplication::translate("RaytraceableContourSurfaceDialogData", "OK", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    cancel_button->setToolTip(QApplication::translate("RaytraceableContourSurfaceDialogData", "cancel dialog", "cancel dialog", QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    cancel_button->setText(QApplication::translate("RaytraceableContourSurfaceDialogData", "cancel", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    groupBoxValue_2->setToolTip(QApplication::translate("RaytraceableContourSurfaceDialogData", "add contour surface", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    groupBoxValue_2->setTitle(QApplication::translate("RaytraceableContourSurfaceDialogData", "Contour Threshold Range", 0, QApplication::UnicodeUTF8));
    RangeLabel_min->setText(QApplication::translate("RaytraceableContourSurfaceDialogData", "min", 0, QApplication::UnicodeUTF8));
    RangeLabel_max->setText(QApplication::translate("RaytraceableContourSurfaceDialogData", "max", 0, QApplication::UnicodeUTF8));
    RangeLabel_steps->setText(QApplication::translate("RaytraceableContourSurfaceDialogData", "steps", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    autoRangeButton->setToolTip(QApplication::translate("RaytraceableContourSurfaceDialogData", "set the contour threshold range to default values", "set the range values to default values", QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    autoRangeButton->setText(QApplication::translate("RaytraceableContourSurfaceDialogData", "auto", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    RangeLineEdit_steps->setToolTip(QApplication::translate("RaytraceableContourSurfaceDialogData", "set the number of steps", "range steps", QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    RangeLineEdit_max->setToolTip(QApplication::translate("RaytraceableContourSurfaceDialogData", "set the maximum theshold", "maximum threshold", QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP


#ifndef QT_NO_TOOLTIP
    RangeLineEdit_min->setToolTip(QApplication::translate("RaytraceableContourSurfaceDialogData", "set the minimum threshold", "minimum threshold", QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    Q_UNUSED(RaytraceableContourSurfaceDialogData);
    } // retranslateUi

};

namespace Ui {
    class RaytraceableContourSurfaceDialogData: public Ui_RaytraceableContourSurfaceDialogData {};
} // namespace Ui

QT_END_NAMESPACE

#endif // RAYTRACEABLECONTOURSURFACEDIALOGDATA_H

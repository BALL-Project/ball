/********************************************************************************
** Form generated from reading ui file 'raytraceableContourSurfaceDialog.ui'
**
** Created: Wed Aug 29 16:30:45 2007
**      by: Qt User Interface Compiler version 4.2.2
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

class Ui_RaytraceableContourSurfaceDialogData
{
public:
    QGroupBox *groupBox;
    QLabel *ContouThresholdLabel;
    QSlider *ContourSurfaceThresholdSlider;
    QLineEdit *ContourSurfaceThresholdLineEdit;
    QLabel *AngstroemLabel;
    QPushButton *add_button;
    QPushButton *cancel_button;
    QPushButton *OK_button;

    void setupUi(QDialog *RaytraceableContourSurfaceDialogData)
    {
    RaytraceableContourSurfaceDialogData->setObjectName(QString::fromUtf8("RaytraceableContourSurfaceDialogData"));
    RaytraceableContourSurfaceDialogData->setEnabled(true);
    RaytraceableContourSurfaceDialogData->setWindowIcon(QIcon());
    groupBox = new QGroupBox(RaytraceableContourSurfaceDialogData);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    groupBox->setGeometry(QRect(20, 10, 281, 141));
    groupBox->setAlignment(Qt::AlignHCenter);
    ContouThresholdLabel = new QLabel(groupBox);
    ContouThresholdLabel->setObjectName(QString::fromUtf8("ContouThresholdLabel"));
    ContouThresholdLabel->setGeometry(QRect(10, 30, 151, 20));
    ContourSurfaceThresholdSlider = new QSlider(groupBox);
    ContourSurfaceThresholdSlider->setObjectName(QString::fromUtf8("ContourSurfaceThresholdSlider"));
    ContourSurfaceThresholdSlider->setGeometry(QRect(10, 60, 160, 16));
    ContourSurfaceThresholdSlider->setOrientation(Qt::Horizontal);
    ContourSurfaceThresholdLineEdit = new QLineEdit(groupBox);
    ContourSurfaceThresholdLineEdit->setObjectName(QString::fromUtf8("ContourSurfaceThresholdLineEdit"));
    ContourSurfaceThresholdLineEdit->setGeometry(QRect(190, 50, 51, 24));
    AngstroemLabel = new QLabel(groupBox);
    AngstroemLabel->setObjectName(QString::fromUtf8("AngstroemLabel"));
    AngstroemLabel->setGeometry(QRect(250, 50, 21, 20));
    add_button = new QPushButton(groupBox);
    add_button->setObjectName(QString::fromUtf8("add_button"));
    add_button->setWindowModality(Qt::WindowModal);
    add_button->setGeometry(QRect(20, 100, 75, 26));
    cancel_button = new QPushButton(groupBox);
    cancel_button->setObjectName(QString::fromUtf8("cancel_button"));
    cancel_button->setWindowModality(Qt::WindowModal);
    cancel_button->setGeometry(QRect(190, 100, 75, 26));
    OK_button = new QPushButton(groupBox);
    OK_button->setObjectName(QString::fromUtf8("OK_button"));
    OK_button->setWindowModality(Qt::WindowModal);
    OK_button->setGeometry(QRect(110, 100, 75, 26));

    retranslateUi(RaytraceableContourSurfaceDialogData);

    QSize size(319, 166);
    size = size.expandedTo(RaytraceableContourSurfaceDialogData->minimumSizeHint());
    RaytraceableContourSurfaceDialogData->resize(size);


    QMetaObject::connectSlotsByName(RaytraceableContourSurfaceDialogData);
    } // setupUi

    void retranslateUi(QDialog *RaytraceableContourSurfaceDialogData)
    {
    RaytraceableContourSurfaceDialogData->setWindowTitle(QApplication::translate("RaytraceableContourSurfaceDialogData", "Ray-traced Contour Surface Options", 0, QApplication::UnicodeUTF8));
    RaytraceableContourSurfaceDialogData->setToolTip(QString());
    groupBox->setToolTip(QApplication::translate("RaytraceableContourSurfaceDialogData", "add contour surface", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("RaytraceableContourSurfaceDialogData", "Contour Surface Options", 0, QApplication::UnicodeUTF8));
    ContouThresholdLabel->setText(QApplication::translate("RaytraceableContourSurfaceDialogData", "Contour Threshold", 0, QApplication::UnicodeUTF8));
    ContourSurfaceThresholdSlider->setToolTip(QApplication::translate("RaytraceableContourSurfaceDialogData", "choose a threshold of a new contour surface", 0, QApplication::UnicodeUTF8));
    ContourSurfaceThresholdSlider->setWhatsThis(QApplication::translate("RaytraceableContourSurfaceDialogData", "slider to chose a threshold of a new contour surface", 0, QApplication::UnicodeUTF8));
    ContourSurfaceThresholdSlider->setAccessibleName(QString());
    ContourSurfaceThresholdSlider->setAccessibleDescription(QString());
    AngstroemLabel->setText(QApplication::translate("RaytraceableContourSurfaceDialogData", "A", 0, QApplication::UnicodeUTF8));
    add_button->setToolTip(QString());
    add_button->setText(QApplication::translate("RaytraceableContourSurfaceDialogData", "add", 0, QApplication::UnicodeUTF8));
    cancel_button->setToolTip(QApplication::translate("RaytraceableContourSurfaceDialogData", "cancel dialog", "add a contour", QApplication::UnicodeUTF8));
    cancel_button->setText(QApplication::translate("RaytraceableContourSurfaceDialogData", "cancel", 0, QApplication::UnicodeUTF8));
    OK_button->setToolTip(QApplication::translate("RaytraceableContourSurfaceDialogData", "add contour surface and close", "add a contour", QApplication::UnicodeUTF8));
    OK_button->setText(QApplication::translate("RaytraceableContourSurfaceDialogData", "OK", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(RaytraceableContourSurfaceDialogData);
    } // retranslateUi

};

namespace Ui {
    class RaytraceableContourSurfaceDialogData: public Ui_RaytraceableContourSurfaceDialogData {};
} // namespace Ui

#endif // RAYTRACEABLECONTOURSURFACEDIALOGDATA_H

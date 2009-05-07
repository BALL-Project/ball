/********************************************************************************
** Form generated from reading ui file 'multitouchSettings.ui'
**
** Created: Mon May 4 20:12:34 2009
**      by: Qt User Interface Compiler version 4.4.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef MULTITOUCHSETTINGSDATA_H
#define MULTITOUCHSETTINGSDATA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_multitouchSettingsData
{
public:
    QDialogButtonBox *buttonBox;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *host_edit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLineEdit *port_edit;

    void setupUi(QDialog *multitouchSettingsData)
    {
    if (multitouchSettingsData->objectName().isEmpty())
        multitouchSettingsData->setObjectName(QString::fromUtf8("multitouchSettingsData"));
    multitouchSettingsData->resize(400, 163);
    buttonBox = new QDialogButtonBox(multitouchSettingsData);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setGeometry(QRect(40, 120, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    layoutWidget = new QWidget(multitouchSettingsData);
    layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
    layoutWidget->setGeometry(QRect(23, 24, 351, 64));
    verticalLayout = new QVBoxLayout(layoutWidget);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
    label = new QLabel(layoutWidget);
    label->setObjectName(QString::fromUtf8("label"));

    horizontalLayout_3->addWidget(label);

    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout_3->addItem(horizontalSpacer_2);

    host_edit = new QLineEdit(layoutWidget);
    host_edit->setObjectName(QString::fromUtf8("host_edit"));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(host_edit->sizePolicy().hasHeightForWidth());
    host_edit->setSizePolicy(sizePolicy);
    host_edit->setMinimumSize(QSize(250, 25));
    host_edit->setMaximumSize(QSize(80, 500));

    horizontalLayout_3->addWidget(host_edit);


    verticalLayout->addLayout(horizontalLayout_3);

    horizontalLayout_2 = new QHBoxLayout();
    horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    label_2 = new QLabel(layoutWidget);
    label_2->setObjectName(QString::fromUtf8("label_2"));

    horizontalLayout_2->addWidget(label_2);

    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    horizontalSpacer = new QSpacerItem(158, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    horizontalLayout->addItem(horizontalSpacer);

    port_edit = new QLineEdit(layoutWidget);
    port_edit->setObjectName(QString::fromUtf8("port_edit"));
    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(port_edit->sizePolicy().hasHeightForWidth());
    port_edit->setSizePolicy(sizePolicy1);
    port_edit->setMinimumSize(QSize(30, 25));
    port_edit->setMaximumSize(QSize(80, 500));

    horizontalLayout->addWidget(port_edit);


    horizontalLayout_2->addLayout(horizontalLayout);


    verticalLayout->addLayout(horizontalLayout_2);


    retranslateUi(multitouchSettingsData);
    QObject::connect(buttonBox, SIGNAL(accepted()), multitouchSettingsData, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), multitouchSettingsData, SLOT(reject()));

    QMetaObject::connectSlotsByName(multitouchSettingsData);
    } // setupUi

    void retranslateUi(QDialog *multitouchSettingsData)
    {
    multitouchSettingsData->setWindowTitle(QApplication::translate("multitouchSettingsData", "Multitouch Settings", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("multitouchSettingsData", "Host", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    host_edit->setToolTip(QApplication::translate("multitouchSettingsData", "Position x", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    label_2->setText(QApplication::translate("multitouchSettingsData", "Port", 0, QApplication::UnicodeUTF8));

#ifndef QT_NO_TOOLTIP
    port_edit->setToolTip(QApplication::translate("multitouchSettingsData", "Position x", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP

    Q_UNUSED(multitouchSettingsData);
    } // retranslateUi

};

namespace Ui {
    class multitouchSettingsData: public Ui_multitouchSettingsData {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MULTITOUCHSETTINGSDATA_H

/********************************************************************************
** Form generated from reading UI file 'helpframe.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HELPFRAME_H
#define UI_HELPFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_HelpFrame
{
public:
    QGridLayout *gridLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_2;
    QTextBrowser *textBrowser;
    QWidget *tab_2;
    QGridLayout *gridLayout_3;
    QTextBrowser *textBrowser_2;
    QWidget *tab_3;
    QGridLayout *gridLayout_4;
    QTextBrowser *textBrowser_3;

    void setupUi(QFrame *HelpFrame)
    {
        if (HelpFrame->objectName().isEmpty())
            HelpFrame->setObjectName(QString::fromUtf8("HelpFrame"));
        HelpFrame->resize(721, 506);
        gridLayout = new QGridLayout(HelpFrame);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        tabWidget = new QTabWidget(HelpFrame);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setTabBarAutoHide(false);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_2 = new QGridLayout(tab);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        textBrowser = new QTextBrowser(tab);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));

        gridLayout_2->addWidget(textBrowser, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_3 = new QGridLayout(tab_2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        textBrowser_2 = new QTextBrowser(tab_2);
        textBrowser_2->setObjectName(QString::fromUtf8("textBrowser_2"));

        gridLayout_3->addWidget(textBrowser_2, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        gridLayout_4 = new QGridLayout(tab_3);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        textBrowser_3 = new QTextBrowser(tab_3);
        textBrowser_3->setObjectName(QString::fromUtf8("textBrowser_3"));

        gridLayout_4->addWidget(textBrowser_3, 0, 0, 1, 1);

        tabWidget->addTab(tab_3, QString());

        gridLayout->addWidget(tabWidget, 1, 0, 1, 1);


        retranslateUi(HelpFrame);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(HelpFrame);
    } // setupUi

    void retranslateUi(QFrame *HelpFrame)
    {
        HelpFrame->setWindowTitle(QCoreApplication::translate("HelpFrame", "Frame", nullptr));
        textBrowser->setHtml(QCoreApplication::translate("HelpFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:22pt; font-weight:600;\">\345\225\206\344\270\232\350\264\267\346\254\276\344\270\216\345\205\254\347\247\257\351\207\221\350\264\267\346\254\276\347\232\204\345\214\272\345\210\253</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:14pt;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'\344\273"
                        "\277\345\256\213'; font-size:14pt; font-weight:600; color:#333333;\">1\357\274\216\350\264\267\346\254\276\345\210\251\347\216\207\344\270\215\345\220\214\357\274\232 </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'\344\273\277\345\256\213'; font-size:14pt; color:#333333;\">\345\205\254\347\247\257\351\207\221\350\264\267\346\254\276\357\274\2323.25%\343\200\202 \345\225\206\344\270\232\350\264\267\346\254\276\357\274\2324.65%.\357\274\2102020\345\271\2644\346\234\21020\346\227\245\346\234\200\346\226\260LPR\357\274\211 </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'Times New Roman'; font-size:14pt; font-weight:600; color:#333333;\"> </span><span style=\" font-family:'\344\273\277\345\256\213'; font-size:14pt; font-weight:600; color:#333333;\">2\357\274\216\350\264\267\346\254"
                        "\276\346\265\201\347\250\213\344\270\215\345\220\214\357\274\232</span><span style=\" font-size:14pt; font-weight:600;\"> </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'\344\273\277\345\256\213'; font-size:14pt; color:#333333;\">\347\224\263\350\257\267\345\225\206\344\270\232\350\264\267\346\254\276\350\246\201\345\234\250\345\212\236\347\220\206\350\277\207\346\210\267\344\271\213\345\211\215\345\256\241\346\240\270\350\264\267\346\254\276\357\274\214\345\205\254\347\247\257\351\207\221\350\264\267\346\254\276\346\230\257\345\234\250\345\212\236\347\220\206\350\277\207\346\210\267\344\271\213\345\220\216\345\256\241\346\240\270\350\264\267\346\254\276\343\200\202 </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'\344\273\277\345\256\213'; font-size:14pt; font-weight:600; colo"
                        "r:#333333;\">3\343\200\201\347\224\250\351\200\224\344\270\215\345\220\214\357\274\232</span><span style=\" font-size:14pt; font-weight:600;\"> </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'\344\273\277\345\256\213'; font-size:14pt; color:#333333;\">\345\225\206\344\270\232\350\264\267\346\254\276\345\217\257\344\273\245\347\224\250\346\235\245\344\271\260\344\275\217\345\256\205\343\200\201\345\225\206\344\275\217\343\200\202 \345\205\254\347\247\257\351\207\221\350\264\267\346\254\276\344\270\215\350\203\275\347\224\250\344\272\216\350\264\255\344\271\260\345\225\206\344\275\217\357\274\214\345\217\252\350\203\275\347\224\250\344\272\216\344\271\260\344\275\217\345\256\205\343\200\201\350\243\205\344\277\256\347\255\211\343\200\202</span><span style=\" font-size:14pt;\"> </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; "
                        "text-indent:0px;\"><span style=\" font-family:'\344\273\277\345\256\213'; font-size:14pt; font-weight:600; color:#333333;\">4\343\200\201\351\235\242\345\220\221\345\257\271\350\261\241\344\270\215\345\220\214\357\274\232</span><span style=\" font-size:14pt; font-weight:600;\"> </span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'\344\273\277\345\256\213'; font-size:14pt; color:#333333;\">\345\225\206\344\270\232\350\264\267\346\254\276\351\222\210\345\257\271\346\211\200\346\234\211\347\254\246\345\220\210\346\235\241\344\273\266\347\232\204\347\244\276\344\274\232\345\205\254\344\274\227\345\274\200\346\224\276\357\274\214\350\200\214\345\205\254\347\247\257\351\207\221\350\264\267\346\254\276\345\210\231\345\217\252\345\257\271\347\274\264\347\272\263\345\205\254\347\247\257\351\207\221\347\232\204\350\201\214\345\267\245\345\274\200\346\224\276\343\200\202</span><span style=\" font-size:14pt;\""
                        "> </span></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("HelpFrame", "\350\264\267\346\254\276\347\261\273\345\236\213", nullptr));
        textBrowser_2->setHtml(QCoreApplication::translate("HelpFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'arial','tahoma','Microsoft Yahei','\345\256\213\344\275\223','sans-serif'; font-size:22pt; color:#000000; background-color:#ffffff;\">\350\264\267\346\254\276\345\270\202\345\234\272\346\212\245\344\273\267\345\210\251\347\216\207\357\274\210LPR\357\274\211</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; mar"
                        "gin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Helvetica','Arial','sans-serif'; font-size:12pt; color:#555555;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'\344\273\277\345\256\213'; font-size:14pt; color:#333333;\">   \344\273\2162020\345\271\2643\346\234\2101\346\227\245\350\265\267\357\274\214\351\207\221\350\236\215\346\234\272\346\236\204\344\270\216\345\255\230\351\207\217\346\265\256\345\212\250\345\210\251\347\216\207\350\264\267\346\254\276\345\256\242\346\210\267\345\260\261\345\256\232\344\273\267\345\237\272\345\207\206\350\275\254\346\215\242\346\235\241\346\254\276\350\277\233\350\241\214\345\215\217\345\225\206\357\274\214\345\260\206\345\216\237\345\220\210\345\220\214\347\272\246\345\256\232\347\232\204\345\210\251\347\216\207\345\256\232\344\273\267\346\226\271\345\274\217\350\275\254\346\215\242\344\270\272\344\273\245LPR\344\270\272\345\256\232\344\273\267"
                        "\345\237\272\345\207\206\345\212\240\347\202\271\345\275\242\346\210\220\357\274\210\345\212\240\347\202\271\345\217\257\344\270\272\350\264\237\345\200\274\357\274\211\357\274\214\345\212\240\347\202\271\346\225\260\345\200\274\345\234\250\345\220\210\345\220\214\345\211\251\344\275\231\346\234\237\351\231\220\345\206\205\345\233\272\345\256\232\344\270\215\345\217\230\357\274\233\344\271\237\345\217\257\350\275\254\346\215\242\344\270\272\345\233\272\345\256\232\345\210\251\347\216\207\343\200\202</span><span style=\" font-size:14pt;\"> </span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:14pt;\"><br /></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'\344\273\277\345\256\213'; font-size:14pt; color:#333333;\">   \344\271\237\345\260\261\346\230\257\350\257\264\344\273\2162020\345\271"
                        "\2643\346\234\2101\346\227\245\350\265\267\357\274\214\345\205\250\351\203\250\347\232\204\346\210\277\350\264\267\351\231\244\345\267\262\345\217\221\346\224\276\347\232\204\346\210\277\350\264\267\345\244\226\357\274\214\351\203\275\345\260\206\345\256\236\350\241\214LPR+\345\237\272\347\202\271\347\232\204\346\250\241\345\274\217\350\277\233\350\241\214\350\264\267\346\254\276\357\274\214\345\271\266\344\270\224LPR\345\210\251\347\216\207\347\224\261\345\244\256\350\241\214\346\257\217\346\234\210\350\260\203\346\225\264\344\270\200\346\254\241\357\274\214\346\211\200\344\273\245\345\277\205\351\241\273\346\257\217\346\234\210\345\205\263\346\263\250LPR\345\210\251\347\216\207\343\200\202</span><span style=\" font-size:14pt;\"> </span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-family:'Helvetica','Arial','sans-serif'; font-size:12pt; color:#555555;\"><br /></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("HelpFrame", "LPR", nullptr));
        textBrowser_3->setHtml(QCoreApplication::translate("HelpFrame", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:22pt; font-weight:600;\">\350\256\241\347\256\227\346\226\271\345\274\217\345\214\272\345\210\253</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'\344\273\277\345\256\213'; font-size:12pt; font-weight:600; color:#333333;\">\344\270\200\343\200\201\347\255\211\351\242\235\346\234\254\346\201\257\350\277\230\346\254\276\357\274\232</span><span style=\" font-family:'\344\273\277\345\256\213'; font-size:12pt; "
                        "color:#333333;\">\346\212\212\346\214\211\346\217\255\350\264\267\346\254\276\347\232\204\346\234\254\351\207\221\346\200\273\351\242\235\344\270\216\345\210\251\346\201\257\346\200\273\351\242\235\347\233\270\345\212\240\357\274\214\347\204\266\345\220\216\345\271\263\345\235\207\345\210\206\346\221\212\345\210\260\350\277\230\346\254\276\346\234\237\351\231\220\347\232\204\346\257\217\344\270\252\346\234\210\344\270\255\343\200\202\344\275\234\344\270\272\350\277\230\346\254\276\344\272\272\357\274\214\346\257\217\344\270\252\346\234\210\350\277\230\347\273\231\351\223\266\350\241\214\345\233\272\345\256\232\351\207\221\351\242\235\357\274\214\344\275\206\346\257\217\346\234\210\350\277\230\346\254\276\351\242\235\344\270\255\347\232\204\346\234\254\351\207\221\346\257\224\351\207\215\351\200\220\346\234\210\351\200\222\345\242\236\343\200\201\345\210\251\346\201\257\346\257\224\351\207\215\351\200\220\346\234\210\351\200\222\345\207\217\343\200\202</span> <span style=\" font-family:'\344\273\277\345\256\213"
                        "'; font-size:12pt; color:#333333;\">\345\215\263\345\200\237\346\254\276\344\272\272\346\257\217\346\234\210\346\214\211\347\205\247\345\233\272\345\256\232\351\207\221\351\242\235\350\277\230\346\254\276\343\200\202</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'\344\273\277\345\256\213'; font-size:12pt; color:#333333;\">\350\256\241\347\256\227\345\205\254\345\274\217\357\274\232</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'\344\273\277\345\256\213'; font-size:12pt; color:#333333;\">\346\257\217\346\234\210\350\277\230\346\254\276\351\207\221\351\242\235=\343\200\220\350\264\267\346\254\276\346\234\254\351\207\221\303\227\346\234\210\345\210\251\347\216\207\303\227\357\274\2101+\346\234\210\345\210\251\347\216\207\357\274\211^\350\277\230\346\254\276\346\234\210\346\225\260"
                        "\343\200\221\303\267\343\200\220\357\274\2101+\346\234\210\345\210\251\347\216\207\357\274\211^\350\277\230\346\254\276\346\234\210\346\225\260-1\343\200\221\343\200\202</span> </p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'\344\273\277\345\256\213'; font-size:12pt; font-weight:600; color:#333333;\">\344\272\214\343\200\201\347\255\211\351\242\235\346\234\254\351\207\221\350\277\230\346\254\276\357\274\232</span><span style=\" font-family:'\344\273\277\345\256\213'; font-size:12pt; color:#333333;\">\345\260\206\346\234\254\351\207\221\345\210\206\346\221\212\345\210\260\346\257\217\344\270\252\346\234\210\345\206\205,\345\220\214\346\227\266\344\273\230\346\270\205\344\270\212\344\270\200\344\272\244\346\230\223\346\227\245\350\207\263\346\234\254\346\254\241\350\277\230\346\254\276\346\227\245\344\271\213\351\227\264\347\232\204\345\210\251\346\201\257\343\200\202\350\277\231\347\247\215\350\277\230"
                        "\346\254\276\346\226\271\345\274\217\347\233\270\345\257\271\347\255\211\351\242\235\346\234\254\346\201\257\350\200\214\350\250\200,\346\200\273\347\232\204\345\210\251\346\201\257\346\224\257\345\207\272\350\276\203\344\275\216,\344\275\206\346\230\257\345\211\215\346\234\237\346\224\257\344\273\230\347\232\204\346\234\254\351\207\221\345\222\214\345\210\251\346\201\257\350\276\203\345\244\232,\350\277\230\346\254\276\350\264\237\346\213\205\351\200\220\346\234\210\351\200\222\345\207\217\343\200\202</span> <span style=\" font-family:'\344\273\277\345\256\213'; font-size:12pt; color:#333333;\">\346\214\207\345\200\237\346\254\276\344\272\272\346\257\217\346\234\210\345\201\277\350\277\230\346\234\254\351\207\221\344\270\215\345\217\230\357\274\214\345\210\251\346\201\257\351\232\217\347\235\200\350\264\267\346\254\276\347\232\204\345\201\277\350\277\230\350\266\212\346\235\245\350\266\212\345\260\221\343\200\202</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px"
                        "; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'\344\273\277\345\256\213'; font-size:12pt; color:#333333;\">\350\256\241\347\256\227\345\205\254\345\274\217\357\274\232</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'\344\273\277\345\256\213'; font-size:12pt; color:#333333;\">\346\257\217\346\234\210\350\277\230\346\254\276\351\207\221\351\242\235=\343\200\220\350\264\267\346\254\276\346\234\254\351\207\221/\350\277\230\346\254\276\346\234\210\346\225\260\343\200\221+\343\200\220\346\234\254\351\207\221-\345\267\262\345\275\222\350\277\230\346\234\254\351\207\221\347\264\257\350\256\241\351\242\235\343\200\221\303\227\346\257\217\346\234\210\345\210\251\347\216\207\343\200\202</span> </p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("HelpFrame", "\350\277\230\346\254\276\346\226\271\345\274\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HelpFrame: public Ui_HelpFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HELPFRAME_H

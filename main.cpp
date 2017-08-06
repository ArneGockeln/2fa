#include <iostream>
#include <QApplication>
#include <QTranslator>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  // Retina Display Support for Mac OS and X11
  app.setAttribute(Qt::AA_UseHighDpiPixmaps);

  // Translate
  QTranslator translator;
  translator.load(QString(":/2fa_") + QLocale::system().name());
  app.installTranslator(&translator);

  QIcon appIcon;
  appIcon.addFile(":/Icons/AppIcon240");
  app.setWindowIcon(appIcon);

  // prevent closing the application if mainwindow is closed
  QApplication::setQuitOnLastWindowClosed(false);

  MainWindow window;
  return app.exec();
}
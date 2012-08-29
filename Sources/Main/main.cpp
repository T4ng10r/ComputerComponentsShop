#include <QtGui/QApplication>
#include <GUI/MainWindow.h>

#include <QtGui/QTreeView>

int main(int argc, char *argv[])
{
      //Q_INIT_RESOURCE(application);
      QApplication app(argc, argv);
	  app.setApplicationName("ComputerConfigurationPriceChecker");
	  app.setOrganizationName("T4ng10r");

#ifdef USE_LOG4QT
	  void createLoggers();
	  createLoggers();
#endif
      MainWindow * ptMainWindow = new MainWindow;
      ptMainWindow->show();

	  //////////////////////////////////////////////////////////////////////////
	  int iReturn = app.exec();
	  delete ptMainWindow;
#ifdef USE_LOG4QT
	  void destroyLoggers();
	  destroyLoggers();
#endif
	  return iReturn;
}

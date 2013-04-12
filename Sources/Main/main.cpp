#include <QtGui/QApplication>
#include <GUI/MainWindow.h>

#include <QtGui/QTreeView>
#include <Tools/loggers.h>

int main(int argc, char *argv[])
{
      //Q_INIT_RESOURCE(application);
      QApplication app(argc, argv);
	  app.setApplicationName("ComputerConfigurationPriceChecker");
	  app.setOrganizationName("T4ng10r");

	  createLoggers();
      MainWindow * ptMainWindow = new MainWindow;
      ptMainWindow->show();

	  //////////////////////////////////////////////////////////////////////////
	  int iReturn = app.exec();
	  delete ptMainWindow;
	  destroyLoggers();
	  return iReturn;
}

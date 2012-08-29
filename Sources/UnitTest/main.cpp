#include "CComputerConfModelTest.h"
#include "CCustomHeadersTest.h"
#include <PluginsTests/ShopPluginNetworkTest.h>
#include <PluginsTests/CProlineParserTest.h>
#include <PluginsTests/CProlinePluginTest.h>
#include <PluginsTests/CKomputronikParserTest.h>
#include <PluginsTests/CKomputronikPluginTest.h>
#include <PluginsTests/CArestPluginTest.h>
#include <PluginsTests/CArestParserTest.h>

#include <QtWebKit/QWebPage>
#include <QtWebKit/QWebView>
#include <QtWebKit/QWebFrame>
#include <QtNetwork/QNetworkProxy>
void setProxy();

void deleteLogsDir()
{
	#define LOG_DIR QDir::currentPath()+QDir::separator()+QString("logs")+QDir::separator()

	QDir appDir;
	QString strAppPath = QString("logs")+QDir::separator();
	appDir = QDir(strAppPath);

	foreach (QString fileName, appDir.entryList(QDir::Files)) 
	{
		appDir.remove(fileName);
	}
	appDir.rmdir(strAppPath);
}

int main(int argc, char *argv[]) 
{ 
	QApplication app(argc, argv);
	setProxy();

	deleteLogsDir();
	
	CComputerConfModelTest cComputerConfModelTest;
	if (QTest::qExec(&cComputerConfModelTest, argc, argv)!=0)
		return -1;  

	CCustomHeadersTest cCustomHeadersTest;
	if (QTest::qExec(&cCustomHeadersTest, argc, argv)!=0)
		return -1;  
	 
	//ShopPluginNetworkTest   cShopPluginNetworkTest;
	//if (QTest::qExec(&cShopPluginNetworkTest, argc, argv)!=0)
	//	return -1;  

	//CProlineParserTest cProlineParserTest;
	//if (QTest::qExec(&cProlineParserTest, argc, argv)==0)
	//{
	//	CProlinePluginTest cProlinePluginTest; 
	//	//if (QTest::qExec(&cProlinePluginTest, argc, argv)!=0)
	//	//	return -1;  
	//}


	//CKomputronikParserTest cKomputronikParserTest;
	//if (QTest::qExec(&cKomputronikParserTest, argc, argv)==0)
	//{
	//	//CKomputronikPluginTest cKomputronikPluginTest;
	//	//if (QTest::qExec(&cKomputronikPluginTest, argc, argv)!=0)
	//	//	return -1;  
	//}

	CArestParserTest cArestParserTest;
	if (QTest::qExec(&cArestParserTest, argc, argv)==0)
	{
		CArestPluginTest cArestPluginTest; 
		if (QTest::qExec(&cArestPluginTest, argc, argv)!=0)
			return -1;  
	}
		
}

#include <QtTest/QtTest>
#include <QtCore/QString>
#include <Plugins/ArestShop.h>

class CArestParserTest : public QObject
{
	Q_OBJECT
public:
	void performHTMLCleanup(QString & strHTML );
	private slots:
		void initTestCase(); //start setting proxy
		void testPluginParsing_ProductPage();
		void testPluginParsing_NotFoundPage();
		void testPluginParsing_SingleSearchPage();
};

class CArestPluginExtraTest: public ArestShopPlugin
{
	friend class CArestParserTest;
public:
	void resetParser();
	bool isNotFoundPage();
	bool isSingleProductPage();
	bool isSearchProductPage();
	bool isRefreshNeeded();
	bool isLoadingNextSearchPage();
};

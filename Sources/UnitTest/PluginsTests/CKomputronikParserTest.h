#include <QtTest/QtTest>
#include <QtCore/QString>
#include <Plugins/KomputronikShop.h>

class CKomputronikParserTest : public QObject
{
    Q_OBJECT
public:
	void performHTMLCleanup(QString & strHTML );
private slots:
	void initTestCase(); //start setting proxy
	void testPluginParsing_Refresh();
	void testPluginParsing_ProductPage();
	void testPluginParsing_NotFoundPage();
	void testPluginParsing_SingleSearchPage();
};

class CKomputronikPluginExtraTest: public KomputronikShopPlugin
{
	friend class CKomputronikParserTest;
public:
	void resetParser();
	bool isNotFoundPage();
	bool isSingleProductPage();
	bool isSearchProductPage();
	bool isRefreshNeeded();
	bool isLoadingNextSearchPage();
};

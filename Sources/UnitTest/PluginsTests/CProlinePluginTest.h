#include <QtTest/QtTest>
#include <Plugins/ProlineShop.h>


class CProlinePluginTest : public QObject
{
    Q_OBJECT
public:
private slots:
	void initTestCase();
	void testPluginName();
	void testPluginIcon();
	void testPluginURL();
	void testPluginFindItem_WithOldCorrectURL();
	void testPluginFindItem_SearchPageWithOnlyOneResult();
	void testPluginFindItem_ParsingOneSearchSide();
	void testPluginFindItem_NotFound();
	void testPluginFindItem_NotFoundButSearched();
	void testPluginFindItem_SearchRasburoGAPProduct();
};

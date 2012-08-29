#include <QtTest/QtTest>

class CArestPluginTest : public QObject
{
    Q_OBJECT
public:
private slots:
	void initTestCase(); //start setting proxy
	void testPluginName();
	void testPluginIcon();
	void testPluginURL();
	void testPluginFindItem_Test1();
	void testPluginFindItem_WithoutParsingSearchPages();
	void testPluginFindItem_ParsingOneSearchSide();
	void testPluginFindItem_NotFound();
	void testPluginFindItem_NotFoundButSearched();
	void testPluginFindItem_FoundSearchedMultiplePages();
};

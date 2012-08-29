#include <QtTest/QtTest>
#include <QtCore/QString>

class CKomputronikPluginTest : public QObject
{
    Q_OBJECT
private slots:
	void initTestCase(); //start setting proxy
	void testPluginName();
	void testPluginIcon();
	void testPluginURL();
	void testPluginFindItem_LoadingQNAP_TS212();
	void testPluginFindItem_WithoutParsingSearchPages();
	void testPluginFindItem_ParsingOneSearchSide();
	void testPluginFindItem_NotFound();
	void testPluginFindItem_NotFoundButSearched();
	void testPluginFindItem_FoundSearchedMultiplePages();
};


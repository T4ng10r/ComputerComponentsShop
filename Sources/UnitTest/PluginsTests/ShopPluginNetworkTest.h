#include <QtTest/QtTest>
#include <QtCore/QSharedPointer>

class ShopPluginNetwork;

class ShopPluginNetworkTest : public QObject
{
	Q_OBJECT
public:
	QSharedPointer<ShopPluginNetwork> m_ptrShopPluginNetwork;
private slots:
	void init();
	void cleanup();
	void testSimplePageLoad();
	void testProlineIntelPageLoad();
	void testNotExistingPageLoad();
};

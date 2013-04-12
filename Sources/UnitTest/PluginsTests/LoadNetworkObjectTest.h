#include <QtTest/QtTest>
#include <QtCore/QSharedPointer>

class LoadNetworkObject;

class LoadNetworkObjectTest : public QObject
{
	Q_OBJECT
public:
	QSharedPointer<LoadNetworkObject> m_ptrShopPluginNetwork;
private slots:
	void init();
	void cleanup();
	void testSimplePageLoad();
	void testProlineIntelPageLoad();
	void testNotExistingPageLoad();
};

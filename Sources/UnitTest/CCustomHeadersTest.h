#include <QtTest/QtTest>

class CComputerConfModel;
class CShopHeaderView;
class CComponentHeaderView;

class CCustomHeadersTest : public QObject
{
	Q_OBJECT
public:
	CComputerConfModel * m_ptrModel;
	CShopHeaderView * m_ptrShopHeader;
	CComponentHeaderView * m_ptrComponentHeaderView;
private slots:
	void init();
	void cleanup();
	void test_insertRowColumn_1();
	void test_insertRowColumn_2();
};

#include "CCustomHeadersTest.h" 
#include <Main/Data/ComputerConfModel.h>
#include <Main/GUI/ShopHeaderView.h>
#include <Main/GUI/ComponentHeaderView.h>
#include <Main/GUI/FrozenTableView.h>
#include <Main/Data/Enumerations.h>
#include <QMetaType>
#include <string>
#include <QtGui/QStandardItemModel>
#include <QtGui/QTableView>

const unsigned int ciSingleTimeout(1000); //in s
const unsigned int ciTimeoutsCount(5); //in s
Q_DECLARE_METATYPE(Qt::Orientation)
Q_DECLARE_METATYPE(QModelIndex)

void CCustomHeadersTest::init()
{
	m_ptrModel = new CComputerConfModel;
	m_ptrComponentHeaderView = new CComponentHeaderView();
}
void CCustomHeadersTest::cleanup()
{
	delete m_ptrModel;
	//delete m_ptrShopHeader;
	delete m_ptrComponentHeaderView;
}
void CCustomHeadersTest::test_insertRowColumn_1()
{
	QTableView stTableView;
	m_ptrShopHeader = new CShopHeaderView();
	stTableView.setHorizontalHeader(m_ptrShopHeader);
	stTableView.setModel(m_ptrModel);
	stTableView.show();
	//////////////////////////////////////////////////////////////////////////
	QCOMPARE(m_ptrModel->columnCount(),1);
	QCOMPARE(m_ptrModel->rowCount(),1);

	int iColumnCount = m_ptrModel->columnCount();
	QVERIFY(m_ptrModel->insertColumn(iColumnCount));
	QVERIFY(m_ptrModel->insertRow(m_ptrModel->rowCount()));
	m_ptrModel->setHeaderData(iColumnCount,Qt::Horizontal,true,Role_ShopColumnActive);

	QVERIFY(m_ptrModel->insertColumn(m_ptrModel->columnCount()));
	QVERIFY(m_ptrModel->insertRow(m_ptrModel->rowCount()));
}
void CCustomHeadersTest::test_insertRowColumn_2()
{
	FrozenTableView stTableView;
	stTableView.show();
	stTableView.setModel(m_ptrModel);
	m_ptrModel->reset();
	//////////////////////////////////////////////////////////////////////////
	QCOMPARE(m_ptrModel->columnCount(),1);
	QCOMPARE(m_ptrModel->rowCount(),1);

	int iColumnCount = m_ptrModel->columnCount();
	QVERIFY(m_ptrModel->insertColumn(iColumnCount));
	QVERIFY(m_ptrModel->insertRow(m_ptrModel->rowCount()));
	m_ptrModel->setHeaderData(iColumnCount,Qt::Horizontal,true,Role_ShopColumnActive);

	QVERIFY(m_ptrModel->insertColumn(m_ptrModel->columnCount()));
	QVERIFY(m_ptrModel->insertRow(m_ptrModel->rowCount()));
}

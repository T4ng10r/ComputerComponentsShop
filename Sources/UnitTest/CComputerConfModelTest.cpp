#include "CComputerConfModelTest.h" 
#include <Main/Data/ComputerConfModel.h>
#include <QMetaType>
#include <string>
#include <QtGui/QStandardItemModel>
#include <QtGui/QTableView>
#include <Main/GUI/ShopHeaderView.h>

const unsigned int ciSingleTimeout(1000); //in s
const unsigned int ciTimeoutsCount(5); //in s
Q_DECLARE_METATYPE(Qt::Orientation)
Q_DECLARE_METATYPE(QModelIndex)

void CComputerConfModelTest::init()
{
	m_ptrModel = new CComputerConfModel;
}
void CComputerConfModelTest::cleanup()
{
	delete m_ptrModel;
}
void CComputerConfModelTest::test_insertColumn_ColumnCount()
{
	QCOMPARE(m_ptrModel->columnCount(),1);
	QVERIFY(m_ptrModel->insertColumn(0));
	QCOMPARE(m_ptrModel->columnCount(),2);
	QVERIFY(m_ptrModel->insertColumn(m_ptrModel->columnCount()));
	QCOMPARE(m_ptrModel->columnCount(),3);
	QVERIFY(m_ptrModel->insertColumns(1,4));
	QCOMPARE(m_ptrModel->columnCount(),7);
	QCOMPARE(m_ptrModel->insertColumns(8,4),false);
	QCOMPARE(m_ptrModel->columnCount(),7);
}
void CComputerConfModelTest::test_insertColumn_columnsInserted()
{
	QCOMPARE(m_ptrModel->columnCount(),1);
	int iCount =0;
	qRegisterMetaType<QModelIndex>("QModelIndex");
	QSignalSpy spy(m_ptrModel, SIGNAL(columnsInserted (QModelIndex,int,int)));
	m_ptrModel->insertColumn(0);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for columnsInserted signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->columnCount(),2);
	QList<QVariant> firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 0);
	QCOMPARE(firstSignal.at(2).toInt(), 0);
	//////////////////////////////////////////////////////////////////////////
	spy.clear();
	m_ptrModel->insertColumns(1,3);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for columnsInserted signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->columnCount(),5);
	firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 1);
	QCOMPARE(firstSignal.at(2).toInt(), 3);
}
void CComputerConfModelTest::test_insertColumn_TableView()
{
	QCOMPARE(m_ptrModel->columnCount(),1);
	QTableView stTableView;
	stTableView.setModel(m_ptrModel);
	int iCount =0;
	qRegisterMetaType<QModelIndex>("QModelIndex");
	QSignalSpy spy(m_ptrModel, SIGNAL(columnsInserted (QModelIndex,int,int)));
	m_ptrModel->insertColumn(0);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for columnsInserted signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->columnCount(),2);
	QList<QVariant> firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 0);
	QCOMPARE(firstSignal.at(2).toInt(), 0);
	//////////////////////////////////////////////////////////////////////////
	iCount =0;
	spy.clear();
	m_ptrModel->insertColumn(m_ptrModel->columnCount());
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for columnsInserted signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->columnCount(),3);
	firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 2);
	QCOMPARE(firstSignal.at(2).toInt(), 2);
	//////////////////////////////////////////////////////////////////////////
	iCount =0;
	spy.clear();
	m_ptrModel->insertColumns(1,3);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for columnsInserted signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->columnCount(),6);
	firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 1);
	QCOMPARE(firstSignal.at(2).toInt(), 3);
}
void CComputerConfModelTest::test_removeColumn_ColumnCount()
{
	QCOMPARE(m_ptrModel->columnCount(),1);
	QVERIFY(m_ptrModel->insertColumns(0,5));
	QCOMPARE(m_ptrModel->columnCount(),6);
	QVERIFY(m_ptrModel->removeColumns(0,1));
	QCOMPARE(m_ptrModel->columnCount(),5);
	QVERIFY(m_ptrModel->removeColumns(2,1));
	QCOMPARE(m_ptrModel->columnCount(),4);
	m_ptrModel->removeColumns(0,m_ptrModel->columnCount()+5);
	QCOMPARE(m_ptrModel->columnCount(),1);
}
void CComputerConfModelTest::test_removeColumn_columnsRemoved()
{
	QCOMPARE(m_ptrModel->columnCount(),1);
	m_ptrModel->insertColumns(0,9);
	QCOMPARE(m_ptrModel->columnCount(),10);
	int iCount =0;
	qRegisterMetaType<QModelIndex>("QModelIndex");
	QSignalSpy spy(m_ptrModel, SIGNAL(columnsRemoved(QModelIndex,int,int)));
	m_ptrModel->removeColumn(0);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for columnsRemoved signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->columnCount(),9);
	QList<QVariant> firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 0);
	QCOMPARE(firstSignal.at(2).toInt(), 0);
	//////////////////////////////////////////////////////////////////////////
	spy.clear();
	m_ptrModel->removeColumn(3);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for columnsRemoved signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->columnCount(),8);
	firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 3);
	QCOMPARE(firstSignal.at(2).toInt(), 3);
	//////////////////////////////////////////////////////////////////////////
	spy.clear();
	m_ptrModel->removeColumns(3,3);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for columnsRemoved signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->columnCount(),5);
	firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 3);
	QCOMPARE(firstSignal.at(2).toInt(), 5);
}
void CComputerConfModelTest::test_removeColumn_TableView()
{
	QCOMPARE(m_ptrModel->columnCount(),1);
	m_ptrModel->insertColumns(0,9);
	QCOMPARE(m_ptrModel->columnCount(),10);
	QTableView stTableView;
	stTableView.setModel(m_ptrModel);
	int iCount =0;
	qRegisterMetaType<QModelIndex>("QModelIndex");
	QSignalSpy spy(m_ptrModel, SIGNAL(columnsRemoved(QModelIndex,int,int)));
	m_ptrModel->removeColumn(0);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for columnsRemoved signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->columnCount(),9);
	QList<QVariant> firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 0);
	QCOMPARE(firstSignal.at(2).toInt(), 0);
	//////////////////////////////////////////////////////////////////////////
	spy.clear();
	m_ptrModel->removeColumn(3);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for columnsRemoved signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->columnCount(),8);
	firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 3);
	QCOMPARE(firstSignal.at(2).toInt(), 3);
	//////////////////////////////////////////////////////////////////////////
	spy.clear();
	m_ptrModel->removeColumns(3,3);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for columnsRemoved signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->columnCount(),5);
	firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 3);
	QCOMPARE(firstSignal.at(2).toInt(), 5);
}
void CComputerConfModelTest::test_insertRow_RowCount()
{
	QCOMPARE(m_ptrModel->rowCount(),1);
	QVERIFY(m_ptrModel->insertRow(0));
	QCOMPARE(m_ptrModel->rowCount(),2);
	QVERIFY(m_ptrModel->insertRow(m_ptrModel->rowCount()));
	QCOMPARE(m_ptrModel->rowCount(),3);
	QVERIFY(m_ptrModel->insertRows(0,5));
	QCOMPARE(m_ptrModel->rowCount(),8);
}
void CComputerConfModelTest::test_insertRow_rowsInserted()
{
	QCOMPARE(m_ptrModel->rowCount(),1);
	int iCount =0;
	qRegisterMetaType<QModelIndex>("QModelIndex");
	QSignalSpy spy(m_ptrModel, SIGNAL(rowsInserted(QModelIndex,int,int)));
	m_ptrModel->insertRow(0);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for rowsInserted signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->rowCount(),2);
	QList<QVariant> firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 0);
	QCOMPARE(firstSignal.at(2).toInt(), 0);
	//////////////////////////////////////////////////////////////////////////
	spy.clear();
	m_ptrModel->insertRows(2,4);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for rowsInserted signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->rowCount(),6);
	firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 2);
	QCOMPARE(firstSignal.at(2).toInt(), 5);
}

void CComputerConfModelTest::test_insertRow_TableView()
{
	QCOMPARE(m_ptrModel->rowCount(),1);
	int iCount =0;
	QTableView stTableView;
	stTableView.setModel(m_ptrModel);
	qRegisterMetaType<QModelIndex>("QModelIndex");
	QSignalSpy spy(m_ptrModel, SIGNAL(rowsInserted(QModelIndex,int,int)));
	m_ptrModel->insertRow(0);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for rowsInserted signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->rowCount(),2);
	QList<QVariant> firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 0);
	QCOMPARE(firstSignal.at(2).toInt(), 0);
	//////////////////////////////////////////////////////////////////////////
	spy.clear();
	m_ptrModel->insertRows(2,4);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for rowsInserted signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->rowCount(),6);
	firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 2);
	QCOMPARE(firstSignal.at(2).toInt(), 5);
}
void CComputerConfModelTest::test_removalRow_RowCount()
{
	QCOMPARE(m_ptrModel->rowCount(),1);
	m_ptrModel->insertRow(m_ptrModel->rowCount());
	m_ptrModel->insertRow(m_ptrModel->rowCount());
	m_ptrModel->insertRow(m_ptrModel->rowCount());
	m_ptrModel->insertRow(m_ptrModel->rowCount());
	QCOMPARE(m_ptrModel->rowCount(),5);
	m_ptrModel->removeRows(0,1);
	QCOMPARE(m_ptrModel->rowCount(),4);
	m_ptrModel->removeRows(2,1);
	QCOMPARE(m_ptrModel->rowCount(),3);
	m_ptrModel->removeRows(0,m_ptrModel->rowCount()+5);
	QCOMPARE(m_ptrModel->rowCount(),1);
}
void CComputerConfModelTest::test_removalRow_rowRemoved()
{
	QCOMPARE(m_ptrModel->rowCount(),1);
	m_ptrModel->insertRows(0,9);
	QCOMPARE(m_ptrModel->rowCount(),10);
	int iCount =0;
	qRegisterMetaType<QModelIndex>("QModelIndex");
	QSignalSpy spy(m_ptrModel, SIGNAL(rowsRemoved(QModelIndex,int,int)));
	m_ptrModel->removeRow(0);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for rowRemoved signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->rowCount(),9);
	QList<QVariant> firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 0);
	QCOMPARE(firstSignal.at(2).toInt(), 0);
	//////////////////////////////////////////////////////////////////////////
	spy.clear();
	m_ptrModel->removeRow(3);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for rowRemoved signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->rowCount(),8);
	firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 3);
	QCOMPARE(firstSignal.at(2).toInt(), 3);
	//////////////////////////////////////////////////////////////////////////
	spy.clear();
	m_ptrModel->removeRows(3,3);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for rowRemoved signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->rowCount(),5);
	firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 3);
	QCOMPARE(firstSignal.at(2).toInt(), 5);
	//////////////////////////////////////////////////////////////////////////
	spy.clear();
	m_ptrModel->removeRows(0,m_ptrModel->rowCount());
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for rowRemoved signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->rowCount(),1);
	firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 0);
	QCOMPARE(firstSignal.at(2).toInt(), 3);
}
void CComputerConfModelTest::test_removalRow_TableView()
{
	QCOMPARE(m_ptrModel->rowCount(),1);
	m_ptrModel->insertRows(0,9);
	QCOMPARE(m_ptrModel->rowCount(),10);
	QTableView stTableView;
	stTableView.setModel(m_ptrModel);
	int iCount =0;
	qRegisterMetaType<QModelIndex>("QModelIndex");
	QSignalSpy spy(m_ptrModel, SIGNAL(rowsRemoved(QModelIndex,int,int)));
	m_ptrModel->removeRow(0);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for rowRemoved signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->rowCount(),9);
	QList<QVariant> firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 0);
	QCOMPARE(firstSignal.at(2).toInt(), 0);
	//////////////////////////////////////////////////////////////////////////
	spy.clear();
	m_ptrModel->removeRow(3);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for rowRemoved signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->rowCount(),8);
	firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 3);
	QCOMPARE(firstSignal.at(2).toInt(), 3);
	//////////////////////////////////////////////////////////////////////////
	spy.clear();
	m_ptrModel->removeRows(3,3);
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for rowRemoved signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->rowCount(),5);
	firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 3);
	QCOMPARE(firstSignal.at(2).toInt(), 5);
	//////////////////////////////////////////////////////////////////////////
	spy.clear();
	m_ptrModel->removeRows(0,m_ptrModel->rowCount());
	while (spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for rowRemoved signal");
			break;
		}
	}
	QCOMPARE(m_ptrModel->rowCount(),1);
	firstSignal = spy.takeFirst();
	QCOMPARE(firstSignal.at(1).toInt(), 0);
	QCOMPARE(firstSignal.at(2).toInt(), 3);
}
void CComputerConfModelTest::test_ModelClear()
{
	m_ptrModel->insertRows(0,2);
	m_ptrModel->insertColumns(0,2);

	QCOMPARE(m_ptrModel->columnCount(),3);
	QCOMPARE(m_ptrModel->rowCount(),3);
	//////////////////////////////////////////////////////////////////////////
	m_ptrModel->removeColumns(0,m_ptrModel->columnCount());
	m_ptrModel->removeRows(0,m_ptrModel->rowCount());
	QCOMPARE(m_ptrModel->columnCount(),1);
	QCOMPARE(m_ptrModel->rowCount(),1);
}
//////////////////////////////////////////////////////////////////////////
void CComputerConfModelTest::test_DataSetting_onFrozenRowColumn()
{
	m_ptrModel->insertRows(0,2);
	m_ptrModel->insertColumns(0,2);
	QCOMPARE(m_ptrModel->columnCount(),3);
	QCOMPARE(m_ptrModel->rowCount(),3);
	//////////////////////////////////////////////////////////////////////////
	QModelIndex stIndex = m_ptrModel->index(10,4);
	QVERIFY(false==stIndex.isValid());
	stIndex = m_ptrModel->index(0,0);
	QVERIFY(false==m_ptrModel->setData(stIndex,"aa"));
	stIndex = m_ptrModel->index(1,0);
	QVERIFY(stIndex.isValid());
	QVERIFY(false==m_ptrModel->setData(stIndex,"aa"));
	stIndex = m_ptrModel->index(2,0);
	QVERIFY(stIndex.isValid());
	QVERIFY(false==m_ptrModel->setData(stIndex,"aa"));
	stIndex = m_ptrModel->index(2,1);
	QVERIFY(stIndex.isValid());
	QVERIFY(m_ptrModel->setData(stIndex,"aa"));
	stIndex = m_ptrModel->index(2,2);
	QVERIFY(stIndex.isValid());
	QVERIFY(m_ptrModel->setData(stIndex,"aa"));
}
void CComputerConfModelTest::test_standaItemModel_signalsTest()
{
	QTableView ptrTableView;
	CShopHeaderView * shopHeaderView = new CShopHeaderView();
	ptrTableView.setHorizontalHeader(shopHeaderView);

	//QStandardItemModel stModel;

	//QStandardItemModel * m_ptrModel = &stModel;
	ptrTableView.setModel(m_ptrModel);
	ptrTableView.show();
	m_ptrModel->insertColumns(0,5);
	m_ptrModel->insertRows(0,5);
	QCOMPARE(m_ptrModel->rowCount(),6);
	QCOMPARE(m_ptrModel->columnCount(),6);
	//////////////////////////////////////////////////////////////////////////
	int iCount =0;
	//qRegisterMetaType<QModelIndex>("QModelIndex");
	//qRegisterMetaType<Qt::Orientation>("Qt::Orientation");
	//QSignalSpy spy(&ptrTableView, SIGNAL(quit()));
	//m_ptrModel->removeRow(3);
	//m_ptrModel->removeRows(0,m_ptrModel->rowCount());
	//m_ptrModel->insertRow(0);
	//m_ptrModel->insertRow(0);
	//m_ptrModel->insertRow(0);
	//m_ptrModel->removeRows(0,m_ptrModel->rowCount());
	//m_ptrModel->insertRow(m_ptrModel->rowCount());
	//while (spy.count() == 0)
	//{
	//	QTest::qWait(5000);
	//	++iCount;
	//	if (iCount>20)
	//	{
	//		QVERIFY2(false,"Timeout waiting for rowsRemoved signal");
	//		break;
	//	}
	//}
	//QCOMPARE(m_ptrModel->rowCount(),4);
	//QList<QVariant> firstSignal = spy.takeFirst();
	//QCOMPARE(firstSignal.at(1).toInt(), 0);
	//QCOMPARE(firstSignal.at(2).toInt(), 0);
	//////////////////////////////////////////////////////////////////////////
	//QTest::qWait(5000);
}

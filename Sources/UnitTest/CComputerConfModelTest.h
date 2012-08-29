#include <QtTest/QtTest>

class CComputerConfModel;

class CComputerConfModelTest : public QObject
{
    Q_OBJECT
public:
	CComputerConfModel * m_ptrModel;
private slots:
	void init();
	void cleanup();
	void test_insertColumn_ColumnCount();
	void test_insertColumn_columnsInserted();
	void test_insertColumn_TableView();
	void test_removeColumn_ColumnCount();
	void test_removeColumn_columnsRemoved();
	void test_removeColumn_TableView();
	void test_insertRow_RowCount();
	void test_insertRow_rowsInserted();
	void test_insertRow_TableView();
	void test_removalRow_RowCount();
	void test_removalRow_rowRemoved();
	void test_removalRow_TableView();
	void test_ModelClear();
	void test_DataSetting_onFrozenRowColumn();
	void test_standaItemModel_signalsTest();
};

#include <QtTest/QtTest>

class RefreshProgressModel;

class CRefreshProgressModelTest : public QObject
{
    Q_OBJECT
public:
	RefreshProgressModel * m_ptrModel;
private slots:
	void init();
	void cleanup();
	void testRowCount();
	void testRowRemoval();
	void testColumnCount();
	void testColumnRemoval();
	//void testModelClear();
	//void testDataSetting();
};

#include "Mocks/ShopPluginBaseInheriterMock.h"

class LoadNetworkObjectMock;

class ShopPluginBaseTest : public ::testing::Test {
public:
	ShopPluginBaseTest();
	virtual void SetUp();
	void prepareSelectorsMocks();
	LoadNetworkObjectMock* getShopPluginBaseMock();
protected:

protected:
	ShopPluginBaseInheriterMock		stShopPluginBase;
};
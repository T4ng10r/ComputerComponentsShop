#include "Mocks/LoadNetworkObjectMock.h"
#include "Mocks/ShopPluginBaseInheriterMock.h"

class ShopPluginBaseTest : public ::testing::Test {
public:
	virtual void SetUp();
protected:

protected:
	ShopPluginBaseInheriterMock		stShopPluginBase;
	LoadNetworkObjectMock			stLoadNetworkObjectMock;

};
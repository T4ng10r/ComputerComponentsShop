#include <QtGui/QApplication>
#include <QtTest/QtTest>
#include <Tests/ShopPluginBaseTest.h>
//#include <TestData/LoadNetworkObjectTest.h>


TEST(ShopPluginBaseTest, Test1) {
	//LoadNetworkObjectMock turtle;                          // #2
	//set LoadNetworkObj mock inside ShopPluginBase
	
	//EXPECT_CALL(turtle, PenDown())              // #3
	//	.Times(AtLeast(1));

	//Painter painter(&turtle);                   // #4

	//EXPECT_TRUE(painter.DrawCircle(0, 0, 10));
}  

int main(int argc, char *argv[]) 
{ 
	QApplication app(argc, argv);

	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}

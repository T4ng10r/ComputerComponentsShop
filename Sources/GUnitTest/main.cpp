#include <QtGui/QApplication>
#include "gmock/gmock.h" 
#include "gtest/gtest.h" 

int main(int argc, char *argv[]) 
{ 
	QApplication app(argc, argv);

	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}

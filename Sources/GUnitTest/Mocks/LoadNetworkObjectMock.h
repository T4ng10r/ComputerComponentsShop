#include "gmock/gmock.h" 
#include <Network/LoadNetworkObject.h>
#ifndef _LOAD_NETWORK_OBJECT_MOCK_
#define _LOAD_NETWORK_OBJECT_MOCK_


class LoadNetworkObjectMock : public LoadNetworkObject
{
public:
	MOCK_METHOD1(loadNetworkObject, void(const QString &strObjectUrl));
	MOCK_METHOD2(loadPostNetworkObject, void(QUrl stUrl, const std::map<QString, QString> &mPostData));
};
#endif

#include "Mocks/ShopPluginBaseInheriterMock.h"
#include <ShopPluginBase_p.h>


ShopPluginBaseInheriterMock::ShopPluginBaseInheriterMock()
{
	m_ptrPriv->m_ptrNetworkObjectLoader.reset(new LoadNetworkObjectMock);
}

#ifndef _CCS_C_PROXY_SETTINGS_INCLUDE_
#define _CCS_C_PROXY_SETTINGS_INCLUDE_

#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkProxy>
#include <Data/ProxySettings.h>

class CProxySetting
{
public:
	CProxySetting();
	void loadProxySettings();
	void setNetworkProxy();
	const ProxySettings & getSettings();
	void setSettings(const ProxySettings &);
public:
	QNetworkProxy			m_stNetworkProxy;
	ProxySettings			m_currentSettings;
	ProxySettings			m_DefaultSettings;	//default system setttings
};
#endif //_CCS_PROXY_SETTINGS_INCLUDE_

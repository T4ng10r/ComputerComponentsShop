#include "CProxySettings.h"
#include <QtCore/QUrl>
#include <QtCore/QSettings>

CProxySetting::CProxySetting()
{
	
}

void CProxySetting::loadProxySettings()
{
	QSettings stSettings("T4ng10r","ComputerConfigurationPriceChecker");
	m_currentSettings.m_eType = (QNetworkProxy::ProxyType)stSettings.value("Proxy/Type").toInt();
	m_currentSettings.m_ProxyIpAddress = QHostAddress(stSettings.value("Proxy/Address").toString());
	m_currentSettings.m_iPort = stSettings.value("Proxy/Port").toInt();
	m_currentSettings.m_bUserAutentication = stSettings.value("Proxy/UserAuth").toBool();
	m_currentSettings.m_strUser = stSettings.value("Proxy/User").toString();
	m_currentSettings.m_strPassword = stSettings.value("Proxy/Pass").toString();
	// default system proxy
	QUrl stUrl("http:\\www.onet.pl");
	QNetworkProxyQuery q;
	q.setQueryType(QNetworkProxyQuery::UrlRequest);
	q.setProtocolTag("http");
	q.setUrl(stUrl);

	QList<QNetworkProxy> proxies = QNetworkProxyFactory::systemProxyForQuery(q);
	if( proxies.size() > 0 && proxies[0].type() != QNetworkProxy::NoProxy )
	{
		//QNetworkProxy::setApplicationProxy(proxies[0]);
		m_DefaultSettings.m_eType = proxies[0].type();
		m_DefaultSettings.m_ProxyIpAddress = proxies[0].hostName();
		m_DefaultSettings.m_iPort = proxies[0].port();
		m_DefaultSettings.m_bUserAutentication = proxies[0].user().isEmpty() ||  proxies[0].password().isEmpty()?false:true;
		m_DefaultSettings.m_strUser = proxies[0].user();
		m_DefaultSettings.m_strPassword = proxies[0].password();
	}
	if (m_currentSettings.m_eType==QNetworkProxy::DefaultProxy)
	{
		m_currentSettings = m_DefaultSettings;
	}
}
void CProxySetting::setNetworkProxy()
{
	m_stNetworkProxy.setType(m_currentSettings.m_eType);
	if (m_currentSettings.m_eType==QNetworkProxy::DefaultProxy ||
		m_currentSettings.m_eType==QNetworkProxy::NoProxy)
	{
		m_stNetworkProxy.setHostName("");
		m_stNetworkProxy.setPort(0);
		m_stNetworkProxy.setUser("");
		m_stNetworkProxy.setPassword("");
	}
	else
	{
		m_stNetworkProxy.setHostName(m_currentSettings.m_ProxyIpAddress.toString());
		m_stNetworkProxy.setPort(m_currentSettings.m_iPort);
		if (m_currentSettings.m_bUserAutentication)
		{
			m_stNetworkProxy.setUser(m_currentSettings.m_strUser);
			m_stNetworkProxy.setPassword(m_currentSettings.m_strPassword);
		}
	}
	QNetworkProxy::setApplicationProxy(m_stNetworkProxy);
}

const ProxySettings & CProxySetting::getSettings()
{
	return m_currentSettings;
}
void CProxySetting::setSettings(const ProxySettings & stProxySettings)
{
	QSettings stSettings("T4ng10r","ComputerConfigurationPriceChecker");
	stSettings.beginGroup("Proxy");
	stSettings.setValue("Type",stProxySettings.m_eType);
	stSettings.setValue("Address",stProxySettings.m_ProxyIpAddress.toString());
	stSettings.setValue("Port",stProxySettings.m_iPort);
	stSettings.setValue("UserAuth",stProxySettings.m_bUserAutentication);
	stSettings.setValue("User",stProxySettings.m_strUser);
	stSettings.setValue("Pass",stProxySettings.m_strPassword);
	stSettings.endGroup();
	m_currentSettings = stProxySettings;
	//////////////////////////////////////////////////////////////////////////
	setNetworkProxy();
}

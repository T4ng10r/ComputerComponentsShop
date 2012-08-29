#include <QtNetwork/QNetworkProxy>
#include <QtCore/QSettings>

void setProxy()
{
	QSettings stSettings("T4ng10r","ComputerConfigurationPriceChecker");
	QNetworkProxy::ProxyType m_eType = (QNetworkProxy::ProxyType)stSettings.value("Proxy/Type").toInt();
	QHostAddress m_ProxyIpAddress = QHostAddress(stSettings.value("Proxy/Address").toString());
	int m_iPort = stSettings.value("Proxy/Port").toInt();
	bool m_bUserAutentication = stSettings.value("Proxy/UserAuth").toBool();
	QString m_strUser = stSettings.value("Proxy/User").toString();
	QString m_strPassword = stSettings.value("Proxy/Pass").toString();

	QNetworkProxy					m_stNetworkProxy;
	{
		m_stNetworkProxy.setType(m_eType);
		if (m_eType==QNetworkProxy::DefaultProxy ||
			m_eType==QNetworkProxy::NoProxy)
		{
			m_stNetworkProxy.setHostName("");
			m_stNetworkProxy.setPort(0);
			m_stNetworkProxy.setUser("");
			m_stNetworkProxy.setPassword("");
		}
		else
		{
			QString strProxyHost = m_ProxyIpAddress.toString();
			//m_stNetworkProxy.setHostName(strProxyHost);
			m_stNetworkProxy.setHostName("localhost");
			m_stNetworkProxy.setPort(m_iPort);
			if (m_bUserAutentication)
			{
				m_stNetworkProxy.setUser(m_strUser);
				m_stNetworkProxy.setPassword(m_strPassword);
			}
			else
			{
				m_stNetworkProxy.setUser("");
				m_stNetworkProxy.setPassword("");
			}
		}
		QNetworkProxy::setApplicationProxy(m_stNetworkProxy);
	}
}
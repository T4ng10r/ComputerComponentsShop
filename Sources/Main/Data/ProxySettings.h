#ifndef _CCS_PROXY_SETTINGS_INCLUDE_
#define _CCS_PROXY_SETTINGS_INCLUDE_

#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkProxy>

struct ProxySettings
{
	ProxySettings():m_eType(QNetworkProxy::NoProxy),
		m_bUserAutentication(false){}
	QNetworkProxy::ProxyType  m_eType;
	QHostAddress	m_ProxyIpAddress;
	int				m_iPort;
	bool			m_bUserAutentication;
	QString			m_strUser;
	QString			m_strPassword;
};
#endif //_CCS_PROXY_SETTINGS_INCLUDE_

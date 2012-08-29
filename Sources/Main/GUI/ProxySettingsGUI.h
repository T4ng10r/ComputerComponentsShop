#ifndef _CCS_PROXY_SETTINGS_GUI_INCLUDE_
#define _CCS_PROXY_SETTINGS_GUI_INCLUDE_

#include <Data/ProxySettings.h>
#include <QtGui/QWidget>

class ProxySettingsGUIPrivate;

class ProxySettingsGUI : public QWidget
{
	Q_OBJECT
	friend class ProxySettingsGUIPrivate;
public:
	ProxySettingsGUI(QWidget * ptrParent = 0);
	~ProxySettingsGUI();
public slots:
	void onSaveProxy();
protected slots:
	void onProxyChanged(int);
protected:
	void	showEvent ( QShowEvent * event );
	ProxySettingsGUIPrivate *	m_ptrPriv;
};
#endif //_CCS_PROXY_SETTINGS_GUI_INCLUDE_ 

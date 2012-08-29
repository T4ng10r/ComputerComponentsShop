#include <GUI/ProxySettingsGUI.h>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QGroupBox>
#include <QtGui/QComboBox>
#include <QtGui/QBoxLayout>
#include <QtGui/QFormLayout>
#include <QtGui/QPushButton>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QIntValidator>
#include <QtGui/QRegExpValidator>
#include <QtNetwork/QNetworkProxy>
#include <Data/CDataThread.h>

void printGUICreationLog(const QString & strLog);
void printSlotsConnectionLog(const QString & strLog);

class IP4Validator : public QValidator {
public:
	IP4Validator(QObject *parent=0) : QValidator(parent){}
	void fixup(QString &input) const {}
	State validate(QString &input, int &pos) const {
		if(input.isEmpty()) return Acceptable;
		QStringList slist = input.split(".");
		int s = slist.size();
		if(s>4) return Invalid;
		bool emptyGroup = false;
		for(int i=0;i<s;i++){
			bool ok;
			if(slist[i].isEmpty()){
				emptyGroup = true;
				continue;
			}
			int val = slist[i].toInt(&ok);
			if(!ok || val<0 || val>255) return Invalid;
		}
		if(s<4 || emptyGroup) return Intermediate;
		return Acceptable;
	}
};

class ProxySettingsGUIPrivate
{
public:
	ProxySettingsGUIPrivate(ProxySettingsGUI *	ptrPublic);
	void setupUI();
	void setConnections();
	void retranslateUI();
	void fillCombos(); 
	void setProxySettings();
public:
	QComboBox *		m_ptrProxyType;
	QLabel *		m_ptrHostLabel;
	QLineEdit *		m_ptrHostEdit;
	QLabel *		m_ptrPortLabel;
	QLineEdit *		m_ptrPortEdit;
	QLabel *		m_ptrUserNameLabel;
	QLineEdit *		m_ptrUserEdit;
	QLabel *		m_ptrPasswordLabel;
	QLineEdit *		m_ptrPasswordEdit;
	QGroupBox *		m_ptrHostData;
	QGroupBox *		m_ptrUserData;
	QDialogButtonBox * m_ptrButtonBox;
	QPushButton *	m_ptrSaveButton;
	QPushButton *	m_ptrCancelButton;

	ProxySettingsGUI *		m_ptrPublic;
	bool			m_bSettingProxy;
	IP4Validator	m_stHostValidator;
	//QRegExpValidator	m_stHostValidator;
	QIntValidator	m_stPortValidator;
};
ProxySettingsGUIPrivate::ProxySettingsGUIPrivate(ProxySettingsGUI *	ptrPublic):m_bSettingProxy(false)
{
	m_ptrPublic = ptrPublic;
	m_ptrPublic->m_ptrPriv = this;
	
	//m_stHostValidator.setRegExp();
	setupUI();
	retranslateUI();
	fillCombos();
	setConnections();
	setProxySettings();

}
void ProxySettingsGUIPrivate::setupUI()
{
	QVBoxLayout * ptrVMainLayout = new QVBoxLayout(m_ptrPublic);
	m_ptrPublic->setLayout(ptrVMainLayout);
	
	m_ptrProxyType = new QComboBox(m_ptrPublic);
	ptrVMainLayout->addWidget(m_ptrProxyType);
	//////////////////////////////////////////////////////////////////////////
	m_ptrHostData = new QGroupBox(m_ptrProxyType);
	ptrVMainLayout->addWidget(m_ptrHostData);

	QVBoxLayout * ptrVHostLayout = new QVBoxLayout;
	//////////////////////////////////////////////////////////////////////////
	QHBoxLayout * ptrHHostLayout = new QHBoxLayout;

	m_ptrHostLabel = new QLabel;
	ptrHHostLayout->addWidget(m_ptrHostLabel);
	m_ptrHostEdit = new QLineEdit;
	m_ptrHostEdit->setValidator(&m_stHostValidator);
	ptrHHostLayout->addWidget(m_ptrHostEdit);
	m_ptrPortLabel = new QLabel;
	ptrHHostLayout->addWidget(m_ptrPortLabel);
	m_ptrPortEdit = new QLineEdit;
	m_ptrPortEdit->setValidator(&m_stPortValidator);
	ptrHHostLayout->addWidget(m_ptrPortEdit);
	ptrVHostLayout->addLayout(ptrHHostLayout);
	//////////////////////////////////////////////////////////////////////////
	m_ptrUserData = new QGroupBox(m_ptrHostData);
	m_ptrUserData->setCheckable(true);
	m_ptrUserData->setChecked(false);
	ptrVHostLayout->addWidget(m_ptrUserData);

	QFormLayout *ptrVPassLayout = new QFormLayout ;

	m_ptrUserNameLabel = new QLabel;
	m_ptrUserEdit = new QLineEdit;

	ptrVPassLayout->addRow(m_ptrUserNameLabel,m_ptrUserEdit);
	m_ptrPasswordLabel = new QLabel;
	m_ptrPasswordEdit = new QLineEdit;
	m_ptrPasswordEdit->setEchoMode(QLineEdit::Password);
	ptrVPassLayout->addRow(m_ptrPasswordLabel,m_ptrPasswordEdit);

	m_ptrUserData->setLayout(ptrVPassLayout);
	//////////////////////////////////////////////////////////////////////////
	m_ptrHostData->setLayout(ptrVHostLayout);

	m_ptrButtonBox = new QDialogButtonBox;
	ptrVMainLayout->addWidget(m_ptrButtonBox);

	m_ptrSaveButton = new QPushButton;
	QObject::connect(m_ptrSaveButton,SIGNAL(clicked(bool)),m_ptrPublic,SLOT(onSaveProxy()));
	
	m_ptrCancelButton = new QPushButton;
	QObject::connect(m_ptrCancelButton,SIGNAL(clicked(bool)),m_ptrPublic,SLOT(hide()));

	m_ptrButtonBox->addButton(m_ptrSaveButton, QDialogButtonBox::ActionRole);
	m_ptrButtonBox->addButton(m_ptrCancelButton, QDialogButtonBox::RejectRole);
	//////////////////////////////////////////////////////////////////////////
	QWidget::setTabOrder(m_ptrProxyType,m_ptrHostLabel);
	QWidget::setTabOrder(m_ptrHostLabel,m_ptrPortEdit);
	QWidget::setTabOrder(m_ptrPortEdit,m_ptrUserData);
	QWidget::setTabOrder(m_ptrUserData,m_ptrUserEdit);
	QWidget::setTabOrder(m_ptrUserEdit,m_ptrPasswordEdit);
	QWidget::setTabOrder(m_ptrPasswordEdit,m_ptrSaveButton);
	QWidget::setTabOrder(m_ptrSaveButton,m_ptrCancelButton);
}
void ProxySettingsGUIPrivate::retranslateUI()
{
	m_ptrHostLabel->setText("Host");;
	m_ptrPortLabel->setText("Port");;
	m_ptrUserNameLabel->setText("Username");
	m_ptrPasswordLabel->setText("Password");
	m_ptrUserData->setTitle("Use autentication");
	m_ptrSaveButton->setText("Save");
	m_ptrCancelButton->setText("Cancel");
}
void ProxySettingsGUIPrivate::setConnections()
{
	QObject::connect(m_ptrProxyType,SIGNAL(activated ( int )),
		m_ptrPublic, SLOT(onProxyChanged(int)));
}
void ProxySettingsGUIPrivate::fillCombos()
{
	m_ptrProxyType->clear();
	m_ptrProxyType->addItem("NoProxy",QVariant(QNetworkProxy::NoProxy));
	m_ptrProxyType->addItem("DefaultProxy",QVariant(QNetworkProxy::DefaultProxy));
	m_ptrProxyType->addItem("HttpProxy",QVariant(QNetworkProxy::HttpProxy));
	m_ptrProxyType->addItem("Socks5",QVariant(QNetworkProxy::Socks5Proxy));
	m_ptrProxyType->setCurrentIndex(0);
}
void ProxySettingsGUIPrivate::setProxySettings()
{
	const ProxySettings & stProxySett = CDataThread::getInstance()->getProxySettings();
	for(int i=0;i<m_ptrProxyType->count();++i)
	{
		QNetworkProxy::ProxyType eType = (QNetworkProxy::ProxyType )m_ptrProxyType->itemData(i).toInt();;
		if (eType==stProxySett.m_eType)
		{
			m_ptrProxyType->setCurrentIndex(i);
			m_ptrPublic->onProxyChanged(i);
			break;
		}
	}
	m_ptrHostEdit->setText(stProxySett.m_ProxyIpAddress.toString());
	m_ptrPortEdit->setText(QString::number(stProxySett.m_iPort));
	m_ptrUserEdit->setText(stProxySett.m_strUser);
	m_ptrPasswordEdit->setText(stProxySett.m_strPassword);
	m_ptrUserData->setChecked(stProxySett.m_bUserAutentication);
}
//////////////////////////////////////////////////////////////////////////
ProxySettingsGUI::ProxySettingsGUI(QWidget * ptrParent) : QWidget(ptrParent)
{
	m_ptrPriv = new ProxySettingsGUIPrivate(this);
}
ProxySettingsGUI::~ProxySettingsGUI()
{
	delete m_ptrPriv;
}
void ProxySettingsGUI::onProxyChanged(int iIndex)
{
	QNetworkProxy::ProxyType eType = (QNetworkProxy::ProxyType )m_ptrPriv->m_ptrProxyType->itemData(iIndex).toInt();;
	switch(eType)
	{
	case QNetworkProxy::Socks5Proxy:
	case QNetworkProxy::HttpProxy:
	case QNetworkProxy::DefaultProxy:
		m_ptrPriv->m_ptrHostEdit->setEnabled(true);
		m_ptrPriv->m_ptrPortEdit->setEnabled(true);
		m_ptrPriv->m_ptrHostData->setEnabled(true);
		m_ptrPriv->m_ptrUserData->setEnabled(true);
		break;
	case QNetworkProxy::NoProxy:
		m_ptrPriv->m_ptrHostEdit->setEnabled(false);
		m_ptrPriv->m_ptrPortEdit->setEnabled(false);
		m_ptrPriv->m_ptrHostData->setEnabled(false);
		m_ptrPriv->m_ptrUserData->setChecked(false);
		break;
	default:
		break;
	}
}

void ProxySettingsGUI::showEvent ( QShowEvent * event )
{
	m_ptrPriv->setProxySettings();
}
void ProxySettingsGUI::onSaveProxy()
{
	ProxySettings stProxySett;

	stProxySett.m_eType = (QNetworkProxy::ProxyType )m_ptrPriv->m_ptrProxyType->itemData(m_ptrPriv->m_ptrProxyType->currentIndex()).toInt();
	stProxySett.m_ProxyIpAddress = m_ptrPriv->m_ptrHostEdit->text();
	stProxySett.m_iPort = m_ptrPriv->m_ptrPortEdit->text().toInt();
	stProxySett.m_bUserAutentication = m_ptrPriv->m_ptrUserData->isChecked();
	if (stProxySett.m_bUserAutentication)
	{
		stProxySett.m_strUser = m_ptrPriv->m_ptrUserEdit->text();
		stProxySett.m_strPassword = m_ptrPriv->m_ptrPasswordEdit->text();;
	}
	else
	{
		stProxySett.m_strUser = "";
		stProxySett.m_strPassword = "";
	}
	//////////////////////////////////////////////////////////////////////////
	CDataThread::getInstance()->setProxySettings(stProxySett);
	hide();
}

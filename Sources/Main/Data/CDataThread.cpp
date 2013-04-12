#include <Data/CDataThread.h>
#include <Data/CProxySettings.h>
#include <QtCore/QAbstractItemModel>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include <QtCore/QUrl>
#include <QtGui/QApplication>
#include <Data/ComputerConfModel.h>
#include <Data/ComponentConfXMLReader.h>
#include <Data/Enumerations.h>
#include <Data/CRefreshData.h>
#include <Plugins/ShopInterface.h>
#include <QtGui/QDesktopServices>
#include <QtGui/QFileDialog>
#include <QtCore/QSettings>

#include <list>
#include <tools/loggers.h>

CDataThread* CDataThread::pInstance_=0;
const QString cstrConfFile("ComputerConfShop.xml");
const short int iSpacesPerLevel=2;
enum { MaxRecentFiles = 5 };

class CDataThreadPrivate
{
public:
	CDataThreadPrivate(CDataThread * ptrPublic);
	~CDataThreadPrivate();
	void loadPlugins();
	void setConnections();
	void setNetworkProxy();
	void loadProxySettings();
	void processComp();
	void saveConf(QString strPath);
	void setCurrentFile(const QString &fileName);
public:
	QString							m_strCurrentFile;
	CDataThread *					m_ptrPublic;
	CComputerConfModel *			m_ptrConfigurationModel;
	QStandardItemModel *			m_ptrRefreshModel;
	std::vector<ShopInterface *>	m_ptrPlugins;
	CProxySetting					m_ProxySettings;
	ComputerConfXMLReader *			m_ptrConfReader;
	CRefreshData *					m_ptrRefreshData;
	QStringList						m_recentFilesList;
};

CDataThreadPrivate::CDataThreadPrivate(CDataThread * ptrPublic):m_ptrPublic(ptrPublic),m_strCurrentFile("")
{
	m_ptrConfigurationModel = new CComputerConfModel();
	m_ptrRefreshModel = new QStandardItemModel();
	m_ptrConfReader = new ComputerConfXMLReader();
	loadPlugins();
	m_ProxySettings.loadProxySettings();
	m_ProxySettings.setNetworkProxy();
	m_ptrRefreshData = new CRefreshData;

	QSettings stSettings("T4ng10r","ComputerConfigurationPriceChecker");
	QStringList recentFilesList = stSettings.value("recentFileList").toStringList();
}
CDataThreadPrivate::~CDataThreadPrivate()
{
	printLog(eDebugLogLevel,eDebug,QString("Enter %1").arg(__FUNCTION__));
	delete m_ptrConfigurationModel;
	delete m_ptrRefreshModel;
	delete m_ptrConfReader;
	printLog(eDebugLogLevel,eDebug,QString("Leave %1").arg(__FUNCTION__));
}
void CDataThreadPrivate::loadPlugins()
{
	QDir pluginsDir;
	pluginsDir = QDir(qApp->applicationDirPath());

#if defined(Q_OS_WIN)
	if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
		pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
	if (pluginsDir.dirName() == "MacOS") {
		pluginsDir.cdUp();
		pluginsDir.cdUp();
		pluginsDir.cdUp();
	}
#endif
	pluginsDir.cd("plugins");

	foreach (QString fileName, pluginsDir.entryList(QDir::Files)) 
	{
		QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
		QObject *plugin = loader.instance();
		if (plugin) 
		{
			ShopInterface * iShop = qobject_cast<ShopInterface *>(plugin);
			if (iShop) 
			{
				m_ptrPlugins.push_back(iShop);
				//m_mPluginName2Plugin[iShop->shopName()]=iShop;
				QString strLog(QString("Plugin '%1' loaded").arg(iShop->pluginName()));
				printLog(eInfoLogLevel, eDebug, strLog);
			}
		}
	}
	QString strLog(QString("Plugins loading finished (%1 loaded)").arg(m_ptrPlugins.size()));
	printLog(eInfoLogLevel, eDebug, strLog);
	for(std::vector<ShopInterface *>::const_iterator iterPlug = m_ptrPlugins.begin();
		iterPlug != m_ptrPlugins.end();++iterPlug)
	{
		m_ptrConfigurationModel->addShopPlugin(*iterPlug);
	}
}
void CDataThreadPrivate::saveConf(QString strPath)
{
	short int iIntendLevel=0;
	QFile stFile(strPath);
	if (false==stFile.open(QIODevice::WriteOnly | QIODevice::Truncate /*| QIODevice::Text*/))
	{
		printLog(eErrorLogLevel,eDebug,QString("Opening configuration save file '%1' failed").arg(strPath));
		return;
	}
	stFile.write(QString("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n").toAscii());
	stFile.write("<CONFIGURATION>\n");
	iIntendLevel++;
	stFile.write(QString("%1<SHOPS>\n").arg(QString(iIntendLevel*iSpacesPerLevel,' ')).toAscii());
	iIntendLevel++;
	for(int iCol=0;iCol<m_ptrConfigurationModel->columnCount();++iCol)
	{
		QVariant stShopName = m_ptrConfigurationModel->headerData(iCol,Qt::Horizontal,Role_ShopName);
		if (stShopName.isNull())
			continue;
		QVariant stShopActive = m_ptrConfigurationModel->headerData(iCol,Qt::Horizontal,Role_ShopColumnActive);
		stFile.write(QString("%3<SHOP active=\"%1\">%2</SHOP>\n")
			.arg( stShopActive.toString() ).arg( stShopName.toString() )
			.arg( QString(iIntendLevel*iSpacesPerLevel,' ') ).toAscii()
			);
	}
	iIntendLevel--;
	stFile.write(QString("%1</SHOPS>\n").arg(QString(iIntendLevel*iSpacesPerLevel,' ')).toAscii());
	stFile.flush();
	stFile.write(QString("%1<COMPONENTS>\n").arg(QString(iIntendLevel*iSpacesPerLevel,' ')).toAscii());
	iIntendLevel++;
	for(int iRow=0;iRow<m_ptrConfigurationModel->rowCount();++iRow)
	{
		QVariant stCompName = m_ptrConfigurationModel->headerData(iRow,Qt::Vertical,Role_ComponentName);
		if (stCompName.isNull())
			continue;
		QVariant stCompActive = m_ptrConfigurationModel->headerData(iRow,Qt::Vertical,Role_RowActive);

		QChar sChar;
		if (stCompName.toString().indexOf("\"")>=0)
			sChar=QString("'").at(0);
		else
			sChar='"';
		stFile.write(QString("%3<COMPONENT name=%4%1%4 active=\"%2\">\n")
			.arg(stCompName.toString()).arg(stCompActive.toString())
			.arg( QString(iIntendLevel*iSpacesPerLevel,' '))
			.arg(sChar)
			.toAscii());

		iIntendLevel++;
		for(int iCol=0;iCol<m_ptrConfigurationModel->columnCount();++iCol)
		{
			QVariant stShopName = m_ptrConfigurationModel->headerData(iCol,Qt::Horizontal,Role_ShopName);
			if (stShopName.isNull())
				continue;
			QModelIndex stIndex = m_ptrConfigurationModel->index(iRow,iCol);
			QVariant stPrice = m_ptrConfigurationModel->data(stIndex,Qt::DisplayRole);
			QVariant stURL = m_ptrConfigurationModel->data(stIndex,Role_CompShopURL);
			QVariant stShopActive = m_ptrConfigurationModel->headerData(iCol,Qt::Horizontal,Role_ShopColumnActive);
			if (stShopActive.isNull() || stURL.toString().trimmed().isEmpty() )
				stFile.write(QString("%3<DATA last_price =\"%1\"/>\n").arg(stPrice.toDouble())
				.arg( QString(iIntendLevel*iSpacesPerLevel,' ') )
				.toAscii());
			else
			{
				stFile.write(QString("%3<DATA last_price =\"%1\">%2</DATA>\n")
				.arg(stPrice.toDouble()).arg(stURL.toString())
				.arg( QString(iIntendLevel*iSpacesPerLevel,' ') )
				.toAscii());
			}
			stFile.flush();
		}
		iIntendLevel--;
		stFile.write(QString("%1</COMPONENT>\n").arg(QString(iIntendLevel*iSpacesPerLevel,' ')).toAscii());
	}
	iIntendLevel--;
	stFile.write(QString("%1</COMPONENTS>\n").arg(QString(iIntendLevel*iSpacesPerLevel,' ')).toAscii());
	iIntendLevel--;
	stFile.write(QString("%1</CONFIGURATION>\n").arg(QString(iIntendLevel*iSpacesPerLevel,' ')).toAscii());
	stFile.close();
}
void CDataThreadPrivate::setCurrentFile(const QString &fileName)
{
	m_strCurrentFile = fileName;

	QSettings settings;
	QStringList files = settings.value("recentFileList").toStringList();
	files.removeAll(fileName);
	files.prepend(fileName);
	while (files.size() > MaxRecentFiles)
		files.removeLast();

	settings.setValue("recentFileList", files);
	emit m_ptrPublic->updateRecentFilesAction(files);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
CDataThread::CDataThread(void):m_ptrPriv(new CDataThreadPrivate(this))
{
	connect(m_ptrPriv->m_ptrRefreshData,SIGNAL(refreshingFinished()),this,SIGNAL(refreshingFinished()));
}
CDataThread* CDataThread::getInstance()
{
	if(!pInstance_)
	{
		if(!pInstance_)
		{
			CDataThread * p = new CDataThread();
			pInstance_ = p;
		}
	}
	return pInstance_;
}
CDataThread::~CDataThread(void)
{
	printLog(eDebugLogLevel,eDebug,QString("Enter %1").arg(__FUNCTION__));
	pInstance_ = NULL;
	printLog(eDebugLogLevel,eDebug,QString("Leave %1").arg(__FUNCTION__));
}
void CDataThread::run()
{
	bool bResult = false;
	exec();
}
CComputerConfModel * CDataThread::getConfigurationModel()
{
	return m_ptrPriv->m_ptrConfigurationModel;
}
QStandardItemModel * CDataThread::getRefreshModel()
{
	return m_ptrPriv->m_ptrRefreshModel;
}
std::vector<ShopInterface *> CDataThread::getPlugins()
{
	return m_ptrPriv->m_ptrPlugins;
}
const ProxySettings & CDataThread::getProxySettings()
{
	return m_ptrPriv->m_ProxySettings.getSettings();
}
void CDataThread::setProxySettings(const ProxySettings & stProxySettings)
{
	return m_ptrPriv->m_ProxySettings.setSettings(stProxySettings);
}
QString CDataThread::getCurrentFile()
{
	return m_ptrPriv->m_strCurrentFile;
}
//////////////////////////////////////////////////////////////////////////
void CDataThread::onNewConf()
{
	m_ptrPriv->m_strCurrentFile.clear();
	m_ptrPriv->m_ptrConfigurationModel->reset();
}
void CDataThread::onRefreshConf()
{
	m_ptrPriv->m_ptrRefreshData->onRefreshConf();
}
void CDataThread::onStopRefreshingConf()
{
	m_ptrPriv->m_ptrRefreshData->onStopRefreshConf();
}
void CDataThread::onAddShop()
{
	m_ptrPriv->m_ptrConfigurationModel->insertColumn(m_ptrPriv->m_ptrConfigurationModel->columnCount());
}
void CDataThread::onAddComp()
{
	m_ptrPriv->m_ptrConfigurationModel->insertRow(m_ptrPriv->m_ptrConfigurationModel->rowCount());
}
void CDataThread::onRemComp(int iRow)
{
	m_ptrPriv->m_ptrConfigurationModel->removeRow(iRow);
}
void CDataThread::onRemShop(int iCol)
{
	m_ptrPriv->m_ptrConfigurationModel->removeColumn(iCol);
}
void CDataThread::onOpenConf(const QString instrFilename)
{
	QString strFilename;
	onNewConf();
	strFilename=instrFilename;
	if (strFilename.isEmpty())
	{
		strFilename = QFileDialog::getOpenFileName (/*const_cast<CDataThread*>(this)*/ 0, tr("Save configuration"), "", tr("Configuration Files (*.xml)"));
	}
	if (!strFilename.isEmpty())
	{
		if (false==m_ptrPriv->m_ptrConfReader->loadXMLFile(strFilename,m_ptrPriv->m_ptrConfigurationModel))
		{
			printLog(eErrorLogLevel,eDebug,QString("Configuration file loading FAILED."));
		}
		else
		{
			m_ptrPriv->setCurrentFile(strFilename);
		}
	}
}
void CDataThread::onSaveAsConf()
{
	QString fileName;
	fileName = QFileDialog::getSaveFileName( 0,  
		tr("Open configuration"), "", tr("Configuration Files (*.xml)"));
	m_ptrPriv->m_strCurrentFile = fileName;
	m_ptrPriv->saveConf(fileName);
	m_ptrPriv->setCurrentFile(cstrConfFile);
}
void CDataThread::onSaveConf()
{
	QString fileName;
	if (m_ptrPriv->m_strCurrentFile.isEmpty())
	{
		fileName = QFileDialog::getSaveFileName( 0, 
			tr("Open configuration"), "", tr("Configuration Files (*.xml)"));
	}
	else
	{
		fileName=m_ptrPriv->m_strCurrentFile;
	}
	m_ptrPriv->saveConf(fileName);
	m_ptrPriv->setCurrentFile(cstrConfFile);
}
void CDataThread::onOpenLinkInExternalBrowser(const QModelIndex & stIndex)
{
	if (!stIndex.isValid())
		return;

	QString strUrl = m_ptrPriv->m_ptrConfigurationModel->data(stIndex,Role_CompShopURL).toString();
	if (strUrl.isEmpty())
	{
		emit onStatusBarMsg("Lack of ProductURL for this cell");
		return;
	}
	QUrl sUrl(strUrl);
	QDesktopServices::openUrl(sUrl);
}
void CDataThread::onOpenSearchInExternalBrowser(const QModelIndex & stIndex)
{
	if (!stIndex.isValid())
		return;

	QString strUrl = m_ptrPriv->m_ptrConfigurationModel->data(stIndex,Role_CompShopSearchURL).toString();
	if (strUrl.isEmpty())
	{
		emit onStatusBarMsg("Lack of SearchProductURL for this cell");
		return;
	}
	QUrl sUrl(strUrl);
	QDesktopServices::openUrl(sUrl);
}
void CDataThread::onResetFieldCache(const QModelIndex & stIndex)
{
	if (!stIndex.isValid())
		return;
	m_ptrPriv->m_ptrConfigurationModel->setData(stIndex,false,Role_CellIsRefreshing);
	m_ptrPriv->m_ptrConfigurationModel->setData(stIndex,0,Qt::DisplayRole);
	m_ptrPriv->m_ptrConfigurationModel->setData(stIndex,SR_NONE,Role_CompShopSearchResult);
	m_ptrPriv->m_ptrConfigurationModel->setData(stIndex,"",Role_CompShopURL);
	m_ptrPriv->m_ptrConfigurationModel->setData(stIndex,"",Role_CompShopSearchURL);
	emit onStatusBarMsg("Field cache reset");
}

void CDataThread::onUpdateRecentFileActions()
{
	QSettings settings;
	QStringList files = settings.value("recentFileList").toStringList();
	emit updateRecentFilesAction(files);
}

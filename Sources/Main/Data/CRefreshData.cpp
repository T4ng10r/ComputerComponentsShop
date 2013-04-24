#include <Data/CRefreshData.h>
#include <Data/CDataThread.h>
#include <Data/Enumerations.h>
#include <Data/ComputerConfModel.h>
#include <tools/loggers.h>
#include <list>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>


boost::mutex  resultMutex;

#define LOG_REFRESHING

const int ciMaxRefreshingThreads(1);

static void logConnection(QString strConnDesc,bool bResult)
{
	QString strDebug = "Connection "+strConnDesc;
	if (bResult)	strDebug+=" SUCCESS";	else	strDebug+=" FAIL";
	printLog(eDebugLogLevel,eSlots,strDebug);
	Q_ASSERT_X(bResult==true,"CDataThread::setConnections",QString("Connect %1").arg(strConnDesc).toAscii());
}
//////////////////////////////////////////////////////////////////////////

//component date for single shop
struct CompData
{
	CompData():bBeingProcess(false),bProcessingFinished(false){}
	bool	bBeingProcess;	//is this item being currently processed
	bool	bProcessingFinished;	//is this item has been already
	QString strCompName;
	QString strCompUrl;
	int		iRow;
	int		iCol;
};
typedef std::map<QString, CompData >  MapOfShopComponents;
struct ShopData
{
	ShopData():m_iCount(false){}
	int					m_iCount;
	bool				m_bShopThreadNotFinished;
	MapOfShopComponents	m_Components;
};
//////////////////////////////////////////////////////////////////////////
typedef std::map<ShopInterface *, ShopData >  MapOfAllShops;
class CRefreshDataPrivate
{
public:
	CRefreshDataPrivate(CRefreshData * ptrPublic);
	~CRefreshDataPrivate();
	void processComp();
	void stopProcessComp();

	//make sure that all shop plugins are in internal map
	//with added slots connections to refresh procedure
	void prepareNewRefreshing();
	QStandardItem * addShopToModel(QString strShopName);
	QStandardItem * addComponentToModel(QStandardItem * pShopItem,QString strComponentName);
public:
	CRefreshData *						m_ptrPublic;
	CComputerConfModel *				m_ptrConfigurationModel;
	std::map<QString, ShopInterface *>	m_mPluginName2Plugin;
	//map indexed by 'shop interfaces' and containing map of components indexed by their names
	MapOfAllShops							m_vShopCompData;
	bool								m_bStoppingRefreshing;
};

CRefreshDataPrivate::CRefreshDataPrivate(CRefreshData * ptrPublic):m_ptrPublic(ptrPublic),
m_ptrConfigurationModel(NULL),m_bStoppingRefreshing(false)
{}
CRefreshDataPrivate::~CRefreshDataPrivate(){}
void CRefreshDataPrivate::prepareNewRefreshing()
{
	if (!m_ptrConfigurationModel)
		m_ptrConfigurationModel = CDataThread::getInstance()->getConfigurationModel();
	std::vector<ShopInterface *>		ptrPlugins = CDataThread::getInstance()->getPlugins();
	for(std::vector<ShopInterface *>::iterator iterShop = ptrPlugins.begin();
		iterShop != ptrPlugins.end();iterShop++)
	{
		//if plugin already present in map - stop
		if (m_mPluginName2Plugin.find((*iterShop)->shopName())!=m_mPluginName2Plugin.end())
			break;
		m_mPluginName2Plugin[(*iterShop)->shopName()]=*iterShop;
		bool bResult = QObject::connect(*iterShop, SIGNAL(priceSearchedFinished(CompPriceData)), m_ptrPublic, SLOT(onPriceSearchedFinished(CompPriceData)));
		QString strConnDesc = QString("'%1::priceSearchedFinished' with 'CRefreshData::onPriceSearchedFinished'").arg((*iterShop)->shopName());
		logConnection(strConnDesc,bResult);
		//////////////////////////////////////////////////////////////////////////
		bResult = QObject::connect(*iterShop, SIGNAL(priceSearchProgress(QString ,ESearchProgress, int )), m_ptrPublic, SLOT(onPriceSearchProgress(QString ,ESearchProgress, int )));
		strConnDesc = QString("'%1::priceSearchProgress' with 'CRefreshData::onPriceSearchProgress'").arg((*iterShop)->shopName());
		logConnection(strConnDesc,bResult);
		//////////////////////////////////////////////////////////////////////////
		bResult = QObject::connect(*iterShop, SIGNAL(finished()), m_ptrPublic, SLOT(onShopPluginTerminated()));
		strConnDesc = QString("'%1::terminated' with 'CRefreshData::onShopPluginTerminated'").arg((*iterShop)->shopName());
		logConnection(strConnDesc,bResult);
	}
	m_ptrConfigurationModel->resetWorseBestFlags();
	if (m_vShopCompData.size())
	{
		printLog(eWarningLogLevel,eDebug,QString("Components refreshing data from last Refresh run are NOT CLEARED(size=%1)").arg(m_vShopCompData.size()));
	}
	m_vShopCompData.clear();
	m_bStoppingRefreshing = false;
}
void CRefreshDataPrivate::stopProcessComp()
{
	MapOfAllShops::iterator iterShopsMap = m_vShopCompData.begin();
	MapOfAllShops::iterator iterEndShopsMap = m_vShopCompData.end();
	MapOfAllShops::iterator iterNextMap;
	while(iterShopsMap!=iterEndShopsMap)
	//for(;iterShopsMap!=iterEndShopsMap;iterShopsMap++)
	{
		ShopInterface * pShopPlugin = iterShopsMap->first;
		MapOfShopComponents & lComponentsMap = iterShopsMap->second.m_Components;
		MapOfShopComponents::iterator iterComp = lComponentsMap.begin();
		MapOfShopComponents::iterator iterEndComp = lComponentsMap.end();
		while(iterComp!=iterEndComp)
		{
			if (iterComp->second.bBeingProcess)
			{
				++iterComp;
				continue;
			}
			CompData & stCompData = iterComp->second;
			QModelIndex stIndex = m_ptrConfigurationModel->index(stCompData.iRow,stCompData.iCol);
			m_ptrConfigurationModel->setData(stIndex,false,Role_CellIsRefreshing);
			lComponentsMap.erase(iterComp++);
		}
		if (lComponentsMap.size())
		{
			pShopPlugin->quit();
		}
		else
		{
			//QObject::disconnect(pShopPlugin, 0, 0, 0);
			m_vShopCompData.erase(iterShopsMap);
		}
		if (m_vShopCompData.size()==0)
			break;
		iterShopsMap++;
	}
	if (m_vShopCompData.size()==0)
	{
		printLog(eDebugLogLevel,eDebug,QString("----- Refreshing components DATA FINISHED -----"));
		emit m_ptrPublic->refreshingFinished();
	}
}
void CRefreshDataPrivate::processComp()
{
	bool bSingleShopCompFinished(true);
	MapOfAllShops::iterator iterShopsMap = m_vShopCompData.begin();
	MapOfAllShops::iterator iterEndShopsMap = m_vShopCompData.end();
	for(;iterShopsMap!=iterEndShopsMap;iterShopsMap++)
	{
		ShopData & stSingleShop = iterShopsMap->second;
		if (stSingleShop.m_iCount>=ciMaxRefreshingThreads)
			continue;
		MapOfShopComponents & lComponentsMap = stSingleShop.m_Components;
		ShopInterface * pShopPlugin = iterShopsMap->first;
		if (pShopPlugin->isRunning())
			continue;
		MapOfShopComponents::iterator iterComp = lComponentsMap.begin();
		MapOfShopComponents::iterator iterEndComp = lComponentsMap.end();
		for(;iterComp!=iterEndComp;iterComp++)
		{
			CompData & stCompData = iterComp->second;
			if (stCompData.bBeingProcess)
			{
				bSingleShopCompFinished=false;
				continue;
			}
			else if (stCompData.bProcessingFinished) 	
			{
				continue;
			}
			else
			{
				stCompData.bBeingProcess=true;
				if (stCompData.strCompName.isEmpty())
				{
					int a=1;
				}
				if (stCompData.strCompUrl.isEmpty())
					pShopPlugin->getProductData(stCompData.strCompName);
				else
					pShopPlugin->getProductData(stCompData.strCompName,stCompData.strCompUrl);
				bSingleShopCompFinished=false;
				stSingleShop.m_iCount++;
				if (stSingleShop.m_iCount>=ciMaxRefreshingThreads)
					break;
			}
		}
		MapOfAllShops::iterator iterNextMap;
		if (bSingleShopCompFinished)
		{
			//QObject::disconnect(pShopPlugin, 0, 0, 0);
			iterNextMap=iterShopsMap;
			iterNextMap++;
			m_vShopCompData.erase(iterShopsMap);
			iterShopsMap=iterNextMap;
		}
		if (iterShopsMap==iterEndShopsMap)
			break;
	}
	if (m_vShopCompData.size()==0)
	{
		printLog(eInfoLogLevel, eDebug, QString("----- Refreshing components DATA FINISHED -----"));
		emit m_ptrPublic->refreshingFinished();
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CRefreshData::CRefreshData(void)
{
	m_ptrPriv = new CRefreshDataPrivate(this);
}
CRefreshData::~CRefreshData(void)
{
	delete m_ptrPriv;
}
//////////////////////////////////////////////////////////////////////////
void CRefreshData::onRefreshConf()
{
	m_ptrPriv->prepareNewRefreshing();
	int iColumnCount = m_ptrPriv->m_ptrConfigurationModel->columnCount();
	int iComponentCount = m_ptrPriv->m_ptrConfigurationModel->rowCount();
	printLog(eInfoLogLevel, eDebug, QString("----- Refreshing components DATA STARTED -----"));
	for(int iShopIndex=0;iShopIndex<iColumnCount;++iShopIndex)
	{
		//deactivated row aren't refreshed
		if (false==m_ptrPriv->m_ptrConfigurationModel->headerData(iShopIndex,Qt::Horizontal,Role_ShopColumnActive).toBool())
			continue;
		QString strShopName = m_ptrPriv->m_ptrConfigurationModel->headerData(iShopIndex,Qt::Horizontal,Role_ShopName).toString();
		std::map<QString, ShopInterface *>::iterator iterFindShop = m_ptrPriv->m_mPluginName2Plugin.find(strShopName);
		if (iterFindShop == m_ptrPriv->m_mPluginName2Plugin.end())	
		{
			printLog(eErrorLogLevel,eDebug,QString("%1:Couldn't find shop plugin for name %2").arg(__FUNCTION__).arg(strShopName));
			continue;
		}
		if (m_ptrPriv->m_vShopCompData.find(iterFindShop->second)==m_ptrPriv->m_vShopCompData.end())
		{
			ShopData stSingleShop;
			m_ptrPriv->m_vShopCompData[iterFindShop->second]=stSingleShop;
		}
#ifdef LOG_REFRESHING
		printLog(eDebugLogLevel,eDebug,QString("Shop '%1' prepared for refreshing").arg(strShopName));
#endif
		ShopData & stSingleShop = m_ptrPriv->m_vShopCompData[iterFindShop->second];
		//storing pointer to refresh view item
		MapOfShopComponents & stComponentsList = stSingleShop.m_Components;
		//////////////////////////////////////////////////////////////////////////
		for(int iCompIndex=1;iCompIndex<iComponentCount;++iCompIndex)
		{
			if (false==m_ptrPriv->m_ptrConfigurationModel->headerData(iCompIndex,Qt::Vertical,Role_RowActive).toBool())
				continue;
			
			CompData stCompData;
			stCompData.iRow=iCompIndex;
			stCompData.iCol=iShopIndex;
			stCompData.strCompName = m_ptrPriv->m_ptrConfigurationModel->headerData(iCompIndex,Qt::Vertical,Role_ComponentName).toString();

			QModelIndex stIndex = m_ptrPriv->m_ptrConfigurationModel->index(iCompIndex,iShopIndex);
			m_ptrPriv->m_ptrConfigurationModel->setData(stIndex,true,Role_CellIsRefreshing);
			m_ptrPriv->m_ptrConfigurationModel->setData(stIndex,0,Role_CellRefreshProgress);
			stCompData.strCompUrl = m_ptrPriv->m_ptrConfigurationModel->data(stIndex,Role_CompShopURL).toString().trimmed();
			stComponentsList[stCompData.strCompName]=stCompData;
#ifdef LOG_REFRESHING
			printLog(eDebugLogLevel,eDebug,QString("    Component '%1' (%2,%3) URL='%4'")
				.arg(stCompData.strCompName).arg(stCompData.iCol)
				.arg(stCompData.iRow).arg(stCompData.strCompUrl) );
#endif
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	printLog(eInfoLogLevel, eDebug, QString("----- Refreshing components IN PROGRESS -----"));
	m_ptrPriv->processComp();
}
void CRefreshData::onPriceSearchedFinished(CompPriceData stData)
{
	boost::lock_guard<boost::mutex>  mutexGuard(resultMutex);
	ShopInterface * iShopPluginFinishedProcessing = qobject_cast<ShopInterface *>(sender());
	if (m_ptrPriv->m_vShopCompData.find(iShopPluginFinishedProcessing)==m_ptrPriv->m_vShopCompData.end())
	{
		printLog(eErrorLogLevel,eDebug,QString("%1:Couldn't find shop plugin for name %2, component %3").arg(__FUNCTION__).arg(iShopPluginFinishedProcessing->shopName())
			.arg(stData.strName));
		return;
	}
	ShopData & stSingleShop = m_ptrPriv->m_vShopCompData[iShopPluginFinishedProcessing];
	MapOfShopComponents & stComponentsList = stSingleShop.m_Components;
	if (stComponentsList.find(stData.strName)==stComponentsList.end())
	{
		printLog(eErrorLogLevel,eDebug,QString("%1:Not found component '%2'in '%3's components").arg(__FUNCTION__)
			.arg(stData.strName).arg(iShopPluginFinishedProcessing->shopName()));
		return;
	}
	if (stSingleShop.m_iCount!=0)
		stSingleShop.m_iCount--;
	CompData & stCompData = stComponentsList[stData.strName];
	stCompData.bBeingProcess=false;
	stCompData.bProcessingFinished=true;
	//////////////////////////////////////////////////////////////////////////
	QModelIndex stIndex = m_ptrPriv->m_ptrConfigurationModel->index(stCompData.iRow,stCompData.iCol);
	m_ptrPriv->m_ptrConfigurationModel->setData(stIndex,false,Role_CellIsRefreshing);
	m_ptrPriv->m_ptrConfigurationModel->setData(stIndex,stData.dPrice,Qt::DisplayRole);
	m_ptrPriv->m_ptrConfigurationModel->setData(stIndex,stData.eSearchResult,Role_CompShopSearchResult);
	m_ptrPriv->m_ptrConfigurationModel->setData(stIndex,stData.strCompURL,Role_CompShopURL);
	m_ptrPriv->m_ptrConfigurationModel->setData(stIndex,stData.strCompSearchURL,Role_CompShopSearchURL);
	//remove from list
	stComponentsList.erase(stData.strName);
	iShopPluginFinishedProcessing->quit();
	//CompPriceData stData
	
	printLog(eDebugLogLevel,eDebug,QString(" -- Component download finished '%1'").arg(stData.strName));
}
void CRefreshData::onPriceSearchProgress(QString strCompName,ESearchProgress eProgress, int iProgress)
{
	ShopInterface * iShopPluginTerminated = qobject_cast<ShopInterface *>(sender());
	if (m_ptrPriv->m_vShopCompData.find(iShopPluginTerminated)!=m_ptrPriv->m_vShopCompData.end())
	{
		QString strComponentName = iShopPluginTerminated->getCurrentlyProcessedComponent();
		MapOfShopComponents & lComponentsMap = m_ptrPriv->m_vShopCompData.find(iShopPluginTerminated)->second.m_Components;
		if (lComponentsMap.find(strComponentName)!=lComponentsMap.end())
		{
			CompData & stCompData = lComponentsMap.find(strComponentName)->second;
			//////////////////////////////////////////////////////////////////////////
			//calculate phase of refreshing
			QModelIndex stIndex = m_ptrPriv->m_ptrConfigurationModel->index(stCompData.iRow,stCompData.iCol);
			int progress=0;
			switch(eProgress)
			{
			case ETEST_OLD_URL: progress=2;break;
			case ESEARCHING_PRODUCT: progress=20;break;
			case ELOAD_PAGE: progress=80;break;
			}
			progress+=iProgress;
			if (eProgress!=ELOAD_PAGE)
			{
				progress=qMin(progress,90);
			}
			m_ptrPriv->m_ptrConfigurationModel->setData(stIndex,progress,Role_CellRefreshProgress);
		}
	}
}
void CRefreshData::onShopPluginTerminated()
{
	if (false==m_ptrPriv->m_bStoppingRefreshing)
		m_ptrPriv->processComp();
	else
	{
		ShopInterface * iShopPluginTerminated = qobject_cast<ShopInterface *>(sender());
		if (m_ptrPriv->m_vShopCompData.find(iShopPluginTerminated)!=m_ptrPriv->m_vShopCompData.end())
		{
			QString strComponentName = iShopPluginTerminated->getCurrentlyProcessedComponent();
			MapOfShopComponents & lComponentsMap = m_ptrPriv->m_vShopCompData.find(iShopPluginTerminated)->second.m_Components;
			if (lComponentsMap.find(strComponentName)!=lComponentsMap.end())
			{
				CompData & stCompData = lComponentsMap.find(strComponentName)->second;
				stCompData.bBeingProcess=false;
			}
		}
		m_ptrPriv->stopProcessComp();
	}
}
void CRefreshData::onStopRefreshConf()
{
	printLog(eDebugLogLevel,eDebug,QString("STOP refreshing components"));
	m_ptrPriv->m_bStoppingRefreshing = true;
	m_ptrPriv->stopProcessComp();
}

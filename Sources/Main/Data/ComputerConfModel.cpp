#include <Data/ComputerConfModel.h>
#include <Data/Enumerations.h>
#include <QtCore/QLocale>
#include <QtGui/QPalette>
#include <QtGui/QBrush>
#include <QtGui/QApplication>
#include <QtGui/QFont>
#include <log4qt/logger.h>
#include <vector>
#include <Plugins/ShopInterface.h>

const double ciBestPriceDefault(20000); 
const double ciWorsePriceDefault(0); 

QColor ccBestPriceColor(Qt::green);
QColor ccDefaultColor(Qt::red);
QColor ccWorstColor(Qt::red);
QColor ccRefreshError(Qt::darkRed);
QColor ccDisableColor(QColor(160,160,10));
QColor ccFrozenRowColumnColor(QColor(85,255,255));
QColor ccFrozenDisRowColumnColor(QColor(170,255,255));

#ifdef VIEW_WITH_FROZEN_COLUMN
#define FROZEN_COLUMN_FIX 1
#else
#define FROZEN_COLUMN_FIX 0
#endif
#ifdef VIEW_WITH_FROZEN_ROW
#define FROZEN_ROW_FIX 1
#else
#define FROZEN_ROW_FIX 0
#endif

//row data for single component
struct ComponentEntry
{
	ComponentEntry():dBestPrice(0){}
	bool			bActive;
	QString			strName;
	QString			strCode;
	double			dBestPrice;
};

struct ShopEntry
{
	ShopEntry():dComponentPriceSum(0){}
	bool			bActive;
	QString			strName;
	QString			strShopURL;
	QStringList		lstrAvailableShopsName;
	QVariantList	lShopIconsList;
	double			dComponentPriceSum;
	void			initShopsList(QStringList strShopsList,QVariantList	 inlShopIconsList)
	{
		lstrAvailableShopsName=(QStringList()<<""<<strShopsList);
		lShopIconsList=inlShopIconsList;
	};
};

struct CompShopPriceEntry
{
	CompShopPriceEntry():strURL(), dPrice(0),bActive(true), bBestPrice(0),
		bWorsePrice(0),eSearchResult(SR_NONE),bCellIsRefreshing(false),iRefreshProgress(0){}
	QString				strURL;
	QString				strSearchPageURL;
	QString				strToolTip;
	double				dPrice;
	bool				bActive;
	bool				bBestPrice;
	bool				bWorsePrice;
	bool				bCellIsRefreshing;
	int					iRefreshProgress;
	SearchResultType	eSearchResult;
};

class CComputerConfModelPrivate
{
public:
	CComputerConfModelPrivate(CComputerConfModel * ptrPublic);
	~CComputerConfModelPrivate();
	//calculate which cells are active/inactive
	void calculateCellsState();
	void calculateCellsValues();
	void updateShopsList();
	void processHeaders();
	void calculateCellsTooltips();
public:
	//list of shops names (all available)
	std::vector<ShopEntry>							vShopsList;
	//list of rows
	std::vector<ComponentEntry>						vComponentsList;
	//[row][column] or [components][shops]
	std::vector<std::vector<CompShopPriceEntry> >	vvPricesMatrix;
	//////////////////////////////////////////////////////////////////////////
	std::map<QString, ShopInterface *>				mPlugins;
	CComputerConfModel *							m_ptrPublic;
	//int												m_iLastCol;
	//int												m_iLastRow;
	double											m_iBestPricesSum;
	QLocale											m_Locale;
	QFont											m_currentFont;
	QFont											m_BoldFont;
	QStringList										m_strShopsList;
	QVariantList									m_lShopIconsList;
	QIcon				cErrorIcon;
	QIcon				cBestIcon;
	QIcon				cWorstIcon;
	QIcon				cSearchingIcon;
	QIcon				cTimeoutIcon;
};

CComputerConfModelPrivate::CComputerConfModelPrivate(CComputerConfModel * ptrPublic)
:m_iBestPricesSum(0),m_Locale(QLocale::Polish)
{
	m_ptrPublic = ptrPublic;
	m_currentFont = QApplication::font();
	m_BoldFont = m_currentFont;
	m_BoldFont.setBold(true);

	cErrorIcon = QIcon(":/images/error.png");
	cBestIcon = QIcon(":/images/good.png");
	cWorstIcon = QIcon(":/images/worst.png");
	cSearchingIcon = QIcon(":/images/search.png");
	cTimeoutIcon = QIcon(":/images/clock.png");

	QPalette stPalette(QApplication::palette());
	ccDisableColor = stPalette.color(QPalette::Disabled,QPalette::Base);
	ccDefaultColor = stPalette.color(QPalette::Active,QPalette::Base);
}
CComputerConfModelPrivate::~CComputerConfModelPrivate()
{
	for(std::map<QString, ShopInterface *>::iterator iterPlugins = mPlugins.begin();iterPlugins != mPlugins.end();iterPlugins++)
	{
		delete iterPlugins->second;
	}
}
//sets for each cell its activity state
void CComputerConfModelPrivate::calculateCellsState()
{
	for(unsigned int iRow=0;iRow<vComponentsList.size();++iRow)
	{
		for(unsigned int iColumn=0;iColumn<vShopsList.size();++iColumn)
		{
			vvPricesMatrix[iRow][iColumn].bActive=vShopsList[iColumn].bActive && vComponentsList[iRow].bActive;
		}
	}
}
void CComputerConfModelPrivate::calculateCellsValues()
{
	int iBestColumn(-1);
	int iWorseColumn(-1);
	m_iBestPricesSum=0;
	for(unsigned int iColumn=0;iColumn<vShopsList.size();++iColumn)
		vShopsList[iColumn].dComponentPriceSum=0;
	for(unsigned int iRow=0;iRow<vComponentsList.size();++iRow)
	{
		double iBestPrice(ciBestPriceDefault);
		double iWorsePrice(ciWorsePriceDefault);
		vComponentsList[iRow].dBestPrice=0;
		for(unsigned int iColumn=0;iColumn<vShopsList.size();++iColumn)
		{
			vvPricesMatrix[iRow][iColumn].bBestPrice=false;
			vvPricesMatrix[iRow][iColumn].bWorsePrice=false;
			double dPrice(vvPricesMatrix[iRow][iColumn].dPrice);
			if (vvPricesMatrix[iRow][iColumn].bCellIsRefreshing)
				continue;
			if (dPrice==0)
				continue;
			if (false==vvPricesMatrix[iRow][iColumn].bActive)
				continue;
			vShopsList[iColumn].dComponentPriceSum+=dPrice;
			if (dPrice>iWorsePrice)
			{
				iWorsePrice=dPrice;
				iWorseColumn=iColumn;
			}
			if (dPrice<iBestPrice)
			{
				iBestPrice=dPrice;
				iBestColumn=iColumn;
			}
		}
		if (iBestColumn>=0 && iBestPrice!=ciBestPriceDefault)
		{
			vvPricesMatrix[iRow][iBestColumn].bBestPrice=true;
			vComponentsList[iRow].dBestPrice=iBestPrice;
			m_iBestPricesSum+=iBestPrice;
		}
		if (iWorseColumn>=0 && iWorsePrice!=ciWorsePriceDefault)
		{
			vvPricesMatrix[iRow][iWorseColumn].bWorsePrice=true;
		}
	}
}
void CComputerConfModelPrivate::calculateCellsTooltips()
{

}
void CComputerConfModelPrivate::updateShopsList()
{
	for(std::vector<ShopEntry>::iterator iterShop = vShopsList.begin();
		iterShop != vShopsList.end();++iterShop)
	{
		iterShop->initShopsList(m_strShopsList,m_lShopIconsList);
		iterShop->lShopIconsList.clear();
		//remove from all other shops entries name and icon of this selected shop
		for(std::vector<ShopEntry>::iterator iterShop2 = vShopsList.begin();
			iterShop2 != vShopsList.end();++iterShop2)
		{
			QString strOtherShopSelectedName = iterShop2->strName;
			if (iterShop2==iterShop || strOtherShopSelectedName.isEmpty())
				continue;
			int iIndex = iterShop->lstrAvailableShopsName.indexOf(strOtherShopSelectedName);
			if (iIndex>=0)
			{
				iterShop->lstrAvailableShopsName.removeAt(iIndex);
				//iterShop->lShopIconsList.removeAt(iIndex);
			}
		}
		//////////////////////////////////////////////////////////////////////////
		int a3 =  iterShop->lstrAvailableShopsName.size();
		for(QStringList::const_iterator iterShop3 = iterShop->lstrAvailableShopsName.begin();
			iterShop3!=iterShop->lstrAvailableShopsName.end();++iterShop3)
		{
			QString strName = *iterShop3;
			if ((*iterShop3).isEmpty()) continue;
			iterShop->lShopIconsList.append(mPlugins[*iterShop3]->getShopIcon());
		}
	}
}
void CComputerConfModelPrivate::processHeaders()
{
	calculateCellsState();
	calculateCellsValues();
	calculateCellsTooltips();
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CComputerConfModel::CComputerConfModel(void)
{
	m_ptrPriv = new CComputerConfModelPrivate(this);
}
CComputerConfModel::~CComputerConfModel(void)
{
	delete m_ptrPriv;
}
void CComputerConfModel::reset()
{
	removeColumns(0,columnCount());
	removeRows(0,rowCount());
	m_ptrPriv->m_iBestPricesSum=0;
}
Qt::ItemFlags CComputerConfModel::flags ( const QModelIndex & stIndex ) const
{
	Qt::ItemFlags stEmptyFlags;
	Qt::ItemFlags stFlagsSel = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	if (!stIndex.isValid())
		return stEmptyFlags;
	int iRow = stIndex.row();
	int iColumn = stIndex.column();

	if (iRow>rowCount()-FROZEN_ROW_FIX || iColumn>columnCount()-FROZEN_COLUMN_FIX)
		return stEmptyFlags;
#ifdef VIEW_WITH_FROZEN_COLUMN
	if (iColumn==0)
		return Qt::ItemIsEnabled;
#endif
#ifdef VIEW_WITH_FROZEN_ROW
	if (iRow==0)
		return Qt::ItemIsEnabled;
#endif
	if (m_ptrPriv->vvPricesMatrix[iRow-FROZEN_ROW_FIX][iColumn-FROZEN_COLUMN_FIX].bActive)
	{
		return stFlagsSel;
	}
	return stEmptyFlags;
}
int CComputerConfModel::rowCount ( const QModelIndex & parent ) const
{
	return m_ptrPriv->vComponentsList.size()+FROZEN_ROW_FIX;
}
int CComputerConfModel::columnCount(const QModelIndex & parent) const
{
	return m_ptrPriv->vShopsList.size()+FROZEN_COLUMN_FIX;
}
QVariant CComputerConfModel::data(const QModelIndex &stIndex, int role) const
{
	if (!stIndex.isValid())
		return QVariant();
	int iRow = stIndex.row();
	int iColumn = stIndex.column();
	if (iRow>rowCount()-1 || iColumn>columnCount()-1)
		return QVariant();
	bool bCellActive(true);
	if (false){}
#ifdef VIEW_WITH_FROZEN_COLUMN
	else if (iColumn==0 && iRow!=0)
		bCellActive=m_ptrPriv->vComponentsList[iRow-FROZEN_ROW_FIX].bActive;
#endif
#ifdef VIEW_WITH_FROZEN_ROW
	else if (iColumn!=0 && iRow==0)
		bCellActive=m_ptrPriv->vShopsList[iColumn-FROZEN_COLUMN_FIX].bActive;
#endif
#if defined(VIEW_WITH_FROZEN_COLUMN) && defined(VIEW_WITH_FROZEN_ROW)
	else if (iColumn==0 && iRow==0)
		bCellActive=true;
#endif
	else 
		bCellActive=m_ptrPriv->vvPricesMatrix[iRow-FROZEN_ROW_FIX][iColumn-FROZEN_COLUMN_FIX].bActive;
	switch(role)
	{
	case Qt::TextAlignmentRole:
		return Qt::AlignCenter;
	case Qt::FontRole:
		#ifdef VIEW_WITH_FROZEN_COLUMN
			if (iColumn==0 && bCellActive)
			{
				return m_ptrPriv->m_BoldFont;
			}
		#endif
		#ifdef VIEW_WITH_FROZEN_ROW
			if (iRow==0 && bCellActive)
			{
				return m_ptrPriv->m_BoldFont;
			}
		#endif
		return m_ptrPriv->m_currentFont;
	case Qt::BackgroundRole:
		{
		#ifdef VIEW_WITH_FROZEN_COLUMN
			if (iColumn==0)
			{
				if (bCellActive)
					return QBrush(ccFrozenRowColumnColor);
				else
					return QBrush(ccFrozenDisRowColumnColor);
			}
		#endif
		#ifdef VIEW_WITH_FROZEN_ROW
			if (iRow==0)
			{
				if (bCellActive)
					return QBrush(ccFrozenRowColumnColor);
				else
					return QBrush(ccFrozenDisRowColumnColor);
			}
		#endif
			if (bCellActive)
			{
				const CompShopPriceEntry & eEntry = m_ptrPriv->vvPricesMatrix[iRow-FROZEN_ROW_FIX][iColumn-FROZEN_COLUMN_FIX];
				if (eEntry.bBestPrice)
					return QBrush(ccBestPriceColor);
				else if (eEntry.bWorsePrice)
					return QBrush(ccWorstColor);
				else if (eEntry.eSearchResult!=SR_NONE && eEntry.eSearchResult!=SR_COMPFOUND)
					return QBrush(ccRefreshError);
				else
					return QBrush(ccDefaultColor);
			}
			return QBrush(ccDisableColor);
		}
	case Qt::DecorationRole:
#if defined(VIEW_WITH_FROZEN_COLUMN)
		if (iColumn==0)	return QVariant();
#endif
#if defined(VIEW_WITH_FROZEN_ROW)
		if (iRow==0)	return QVariant();
#endif
		{
			const CompShopPriceEntry & eEntry = m_ptrPriv->vvPricesMatrix[iRow-FROZEN_ROW_FIX][iColumn-FROZEN_COLUMN_FIX];
			if (eEntry.bBestPrice)
				return m_ptrPriv->cBestIcon;
			if (eEntry.bWorsePrice)
				return m_ptrPriv->cWorstIcon;
			if (eEntry.eSearchResult==SR_LOADINGTIMEOUT)
				return m_ptrPriv->cTimeoutIcon;
			if (eEntry.eSearchResult!=SR_NONE && eEntry.eSearchResult!=SR_COMPFOUND)
				return m_ptrPriv->cErrorIcon;
			if (eEntry.bCellIsRefreshing)
				return m_ptrPriv->cSearchingIcon;
			break;
		}
	case Role_CompShopPrice:
	case Qt::DisplayRole:
	case Qt::EditRole:
		double dPrice;
#ifdef VIEW_WITH_FROZEN_COLUMN
		if (iColumn==0 && iRow!=0)
		{
			dPrice = m_ptrPriv->vComponentsList[iRow-FROZEN_ROW_FIX].dBestPrice;
			return m_ptrPriv->m_Locale.toString(dPrice,'f',0);
		}
#endif
#ifdef VIEW_WITH_FROZEN_ROW
		if (iRow==0 && iColumn!=0)
		{
			dPrice = m_ptrPriv->vShopsList[iColumn-FROZEN_COLUMN_FIX].dComponentPriceSum;
			return m_ptrPriv->m_Locale.toString(dPrice,'f',0);
		}
#endif
#ifdef VIEW_WITH_FROZEN_ROW
#ifdef VIEW_WITH_FROZEN_COLUMN
		if (iRow==0 && iColumn==0)
		{
			return m_ptrPriv->m_Locale.toString(m_ptrPriv->m_iBestPricesSum,'f',0);
		}
#endif
#endif
		dPrice = m_ptrPriv->vvPricesMatrix[iRow-FROZEN_ROW_FIX][iColumn-FROZEN_COLUMN_FIX].dPrice;
		return m_ptrPriv->m_Locale.toString(dPrice,'f',0);
	case Role_CompShopURL:
#if defined(VIEW_WITH_FROZEN_COLUMN)
		if (iColumn==0)	return QVariant();
#endif
#if defined(VIEW_WITH_FROZEN_ROW)
		if (iRow==0)	return QVariant();
#endif
		return m_ptrPriv->vvPricesMatrix[iRow-FROZEN_ROW_FIX][iColumn-FROZEN_COLUMN_FIX].strURL;
	case Role_CompShopSearchURL:
#if defined(VIEW_WITH_FROZEN_COLUMN)
		if (iColumn==0)	return QVariant();
#endif
#if defined(VIEW_WITH_FROZEN_ROW)
		if (iRow==0)	return QVariant();
#endif
		{
			QString strSearchURL = m_ptrPriv->vvPricesMatrix[iRow-FROZEN_ROW_FIX][iColumn-FROZEN_COLUMN_FIX].strSearchPageURL;
			if (strSearchURL.isEmpty())
			{
				QString strShopName = m_ptrPriv->vShopsList[iColumn-FROZEN_COLUMN_FIX].strName;
				strSearchURL= m_ptrPriv->mPlugins[strShopName]->getSearchProductPageURL(m_ptrPriv->vComponentsList[iRow-FROZEN_ROW_FIX].strName);
			}
			return strSearchURL;
		}
	case Role_CellIsRefreshing:
#if defined(VIEW_WITH_FROZEN_COLUMN)
		if (iColumn==0)	return QVariant();
#endif
#if defined(VIEW_WITH_FROZEN_ROW)
		if (iRow==0)	return QVariant();
#endif
		return m_ptrPriv->vvPricesMatrix[iRow-FROZEN_ROW_FIX][iColumn-FROZEN_COLUMN_FIX].bCellIsRefreshing;
	case Role_CellRefreshProgress:
#if defined(VIEW_WITH_FROZEN_COLUMN)
		if (iColumn==0)	return QVariant();
#endif
#if defined(VIEW_WITH_FROZEN_ROW)
		if (iRow==0)	return QVariant();
#endif
		return m_ptrPriv->vvPricesMatrix[iRow-FROZEN_ROW_FIX][iColumn-FROZEN_COLUMN_FIX].iRefreshProgress;
		break;
	}
	return QVariant();
}
bool CComputerConfModel::setData ( const QModelIndex & stIndex, const QVariant & value, int role )
{
	//test index correctness
	bool bRefreshAll = false;
	if (!stIndex.isValid())
		return false;
	int iRow = stIndex.row();
	int iColumn = stIndex.column();
	if (iRow>rowCount()-1 || iColumn>columnCount()-1)
		return false;
	#ifdef VIEW_WITH_FROZEN_COLUMN
		if (iColumn == 0) return false;
	#endif
	#ifdef VIEW_WITH_FROZEN_ROW
		if (iRow == 0)	return false;
	#endif
	switch(role)
	{
	case Qt::EditRole:
	case Qt::DisplayRole:
		m_ptrPriv->vvPricesMatrix[iRow-FROZEN_ROW_FIX][iColumn-FROZEN_COLUMN_FIX].dPrice = value.toDouble();
		bRefreshAll = true;
		break;
	case Role_CompShopURL:
		m_ptrPriv->vvPricesMatrix[iRow-FROZEN_ROW_FIX][iColumn-FROZEN_COLUMN_FIX].strURL = value.toString();
		break;
	case Role_CompShopSearchURL:
		m_ptrPriv->vvPricesMatrix[iRow-FROZEN_ROW_FIX][iColumn-FROZEN_COLUMN_FIX].strSearchPageURL = value.toString();
		break;
	case Role_CompShopSearchResult:
		m_ptrPriv->vvPricesMatrix[iRow-FROZEN_ROW_FIX][iColumn-FROZEN_COLUMN_FIX].eSearchResult = (SearchResultType)value.toInt();
		break;
	case Role_CellIsRefreshing:
		m_ptrPriv->vvPricesMatrix[iRow-FROZEN_ROW_FIX][iColumn-FROZEN_COLUMN_FIX].bCellIsRefreshing = value.toBool();
		break;
	case Role_CellRefreshProgress:
		m_ptrPriv->vvPricesMatrix[iRow-FROZEN_ROW_FIX][iColumn-FROZEN_COLUMN_FIX].iRefreshProgress = value.toInt();
		break;
	case Role_CellIcon:
		m_ptrPriv->vvPricesMatrix[iRow-FROZEN_ROW_FIX][iColumn-FROZEN_COLUMN_FIX].iRefreshProgress = value.toInt();
		break;
	//////////////////////////////////////////////////////////////////////////

	default:
		return false;
	}
	m_ptrPriv->processHeaders();
	if (bRefreshAll)
		emit dataChanged(index(0,0),index(rowCount()-1,columnCount()-1));
	else
		emit dataChanged(stIndex,stIndex);
	return true;
}
QVariant CComputerConfModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
	if (orientation==Qt::Horizontal)
	{
		if (section < 0 || section >= columnCount())
			return QVariant();
#ifdef VIEW_WITH_FROZEN_COLUMN
		if (section < 1 && role != Role_ShopColumnActive)
			return QVariant();
#endif
		if (role == Role_ShopColumnActive)
		{
			if (section==0)
			{
				bool bActive=true;
				for(size_t i=0;i<m_ptrPriv->vShopsList.size();i++)
					if (false==m_ptrPriv->vShopsList[i].bActive)
					{
						bActive=false;
						break;
					}
				return bActive;
			}	
			else
				return m_ptrPriv->vShopsList[section-FROZEN_COLUMN_FIX].bActive;
		}
		else if (role == Role_ShopNamesList)
			return m_ptrPriv->vShopsList[section-FROZEN_COLUMN_FIX].lstrAvailableShopsName;
		else if (role == Role_ShopNamesIconsList)
			return m_ptrPriv->vShopsList[section-FROZEN_COLUMN_FIX].lShopIconsList;
		else if (role == Role_ShopName)
			return m_ptrPriv->vShopsList[section-FROZEN_COLUMN_FIX].strName;
	}
	else
	{
		if (section < 0 || section >= rowCount())
			return QVariant();
	#ifdef VIEW_WITH_FROZEN_ROW
		if (section < 1 && role != Role_RowActive)
			return QVariant();
	#endif
		if (role == Role_ComponentName)
			return m_ptrPriv->vComponentsList[section-FROZEN_ROW_FIX].strName;
		else if (role == Role_ComponentCode)
			return m_ptrPriv->vComponentsList[section-FROZEN_ROW_FIX].strCode;
		else if (role == Role_RowBestPrice)
			return m_ptrPriv->vComponentsList[section-FROZEN_ROW_FIX].dBestPrice;
		else if (role == Role_RowActive)
		{
			if (section==0)
			{
				bool bActive=true;
				for(size_t i=0;i<m_ptrPriv->vComponentsList.size();i++)
					if (false==m_ptrPriv->vComponentsList[i].bActive)
					{
						bActive=false;
						break;
					}
					return bActive;
			}	
			else
				return m_ptrPriv->vComponentsList[section-FROZEN_ROW_FIX].bActive;
		}
	}
	return QVariant();
}
bool CComputerConfModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role /* = Qt::EditRole */)
{
	bool bResult = false;
	if (orientation==Qt::Horizontal)
	{
		if (section < 0 || section >= columnCount())
			return false;
#ifdef VIEW_WITH_FROZEN_COLUMN
		if (section < 1 && role != Role_ShopColumnActive)
			return false;
#endif
		if (role == Role_ShopColumnActive)
		{
			if (section==0)
			{
				for(size_t i=0;i<m_ptrPriv->vShopsList.size();i++)
					m_ptrPriv->vShopsList[i].bActive = value.toBool();
			}
			else
				m_ptrPriv->vShopsList[section-FROZEN_COLUMN_FIX].bActive = value.toBool();
			bResult = true;
		}
		else if (role == Role_ShopName)
		{
			m_ptrPriv->vShopsList[section-FROZEN_COLUMN_FIX].strName = value.toString();
			bResult = true;
			m_ptrPriv->updateShopsList();
		}
	}
	else
	{
		if (section < 0 || section >= rowCount())
			return false;
#ifdef VIEW_WITH_FROZEN_ROW
		if (section < 1 && role != Role_RowActive)
			return false;
#endif
		if (role == Role_ComponentName)
		{
			m_ptrPriv->vComponentsList[section-FROZEN_ROW_FIX].strName = value.toString();
			for(size_t iCol=1;iCol<m_ptrPriv->vShopsList.size();iCol++)
				m_ptrPriv->vvPricesMatrix[section-FROZEN_ROW_FIX][iCol].strSearchPageURL="";

			bResult = true;
		}
		else if (role == Role_ComponentCode)
		{
			m_ptrPriv->vComponentsList[section-FROZEN_ROW_FIX].strCode = value.toString();
			bResult = true;
		}
		else if (role == Role_RowBestPrice)
		{
			m_ptrPriv->vComponentsList[section-FROZEN_ROW_FIX].dBestPrice = value.toDouble();
			bResult = true;
		}
		else if (role == Role_RowActive)
		{
			if (section==0)
			{
				for(size_t i=0;i<m_ptrPriv->vComponentsList.size();i++)
					m_ptrPriv->vComponentsList[i].bActive = value.toBool();
			}
			else
				m_ptrPriv->vComponentsList[section-FROZEN_ROW_FIX].bActive = value.toBool();
			bResult = true;
		}
	}
	if (bResult)
	{
		m_ptrPriv->processHeaders();
		if (orientation==Qt::Horizontal)
			emit headerDataChanged(Qt::Horizontal,0,columnCount()-1);
		else
			emit headerDataChanged(Qt::Vertical,0,rowCount()-1);
	}
	return bResult;
}
bool CComputerConfModel::insertRows(int iStartRow, int iCount, const QModelIndex &parent)
{
	if (iStartRow<0 || iStartRow > rowCount())
		return false;
	if (iCount==1)
		beginInsertRows(QModelIndex(), iStartRow, iStartRow);
	else
		beginInsertRows(QModelIndex(), iStartRow, iStartRow+iCount-1);
	ComponentEntry	compItem;
	std::vector<ComponentEntry>::iterator iterComp;
	if (iStartRow==rowCount())
	{
		iterComp = m_ptrPriv->vComponentsList.end();
	}
	else
	{
		iterComp = m_ptrPriv->vComponentsList.begin(); 
		std::vector<ComponentEntry>::iterator iterComp1 = m_ptrPriv->vComponentsList.begin();
		for(int i=0;i<iStartRow;++i)
			++iterComp;
		iterComp1 = iterComp1+iStartRow;
	}
	m_ptrPriv->vComponentsList.insert(iterComp,iCount, compItem);
	
	m_ptrPriv->vvPricesMatrix.resize(rowCount()-FROZEN_ROW_FIX);
	for(unsigned int i=0;i<m_ptrPriv->vvPricesMatrix.size();++i)
		m_ptrPriv->vvPricesMatrix[i].resize(columnCount()-FROZEN_COLUMN_FIX);
	endInsertRows();
	return true;
}
bool CComputerConfModel::insertColumns(int iColumn, int iCount, const QModelIndex &parent)
{
	if (iColumn<0 || iColumn > columnCount())
		return false;
	if (iCount==1)
		beginInsertColumns(QModelIndex(), iColumn, iColumn);
	else
		beginInsertColumns(QModelIndex(), iColumn, iColumn+iCount-1);
	ShopEntry	shopItem;
	std::vector<ShopEntry>::iterator iterShop;
	if (iColumn==columnCount())
	{
		iterShop = m_ptrPriv->vShopsList.end();
	}
	else
	{
		iterShop = m_ptrPriv->vShopsList.begin();
		for(int i=0;i<iColumn;++i)
			++iterShop;
	}
	m_ptrPriv->vShopsList.insert(iterShop,iCount,shopItem);

	m_ptrPriv->vvPricesMatrix.resize(rowCount());
	for(unsigned int i=0;i<m_ptrPriv->vvPricesMatrix.size();++i)
		m_ptrPriv->vvPricesMatrix[i].resize(columnCount()-FROZEN_COLUMN_FIX);
	m_ptrPriv->updateShopsList();
	endInsertColumns();
	return true;
}
bool CComputerConfModel::removeColumns ( int column, int count, const QModelIndex & parent)
{
	if (!count || column>=columnCount())	return false;
	if ((column+count)>=columnCount()) 
	{
		count=columnCount()-FROZEN_COLUMN_FIX-column;
		if (count<=0)	
			return false;
	}
	beginRemoveColumns(QModelIndex(),column,column+count-1);
	std::vector<ShopEntry>::iterator iterEndShop = m_ptrPriv->vShopsList.end();
	std::vector<ShopEntry>::iterator iterStartShop = m_ptrPriv->vShopsList.begin();
	for(int i=1;i<column;++i)
		if (iterStartShop!=iterEndShop)
			++iterStartShop;
	//////////////////////////////////////////////////////////////////////////
	std::vector<ShopEntry>::iterator iterLastShop = iterStartShop;
	for(int i=0;i<count;++i)
		if (iterLastShop!=iterEndShop)
			++iterLastShop;
	m_ptrPriv->vShopsList.erase(iterStartShop,iterLastShop);

	m_ptrPriv->vvPricesMatrix.resize(rowCount()-FROZEN_ROW_FIX);
	for(unsigned int i=0;i<m_ptrPriv->vvPricesMatrix.size();++i)
	{
		std::vector<CompShopPriceEntry>::iterator iterCol = m_ptrPriv->vvPricesMatrix[i].begin();
		for(int j=0;j<column;++j)
			++iterCol;
		m_ptrPriv->vvPricesMatrix[i].erase(iterCol);
	}
	m_ptrPriv->updateShopsList();
	endRemoveColumns();
	return true;
}
bool CComputerConfModel::removeRows ( int iStartRow, int count, const QModelIndex & parent )
{
	if (!count || iStartRow>=rowCount() )	return false;
	if ((iStartRow+count)>=rowCount()) 
	{
		count=rowCount()-FROZEN_ROW_FIX-iStartRow;
		if (count<=0)	
			return false;
	}
	beginRemoveRows(QModelIndex(),iStartRow,iStartRow+count-1);
	std::vector<ComponentEntry>::iterator iterEndComp = m_ptrPriv->vComponentsList.end();
	std::vector<ComponentEntry>::iterator iterStartComp = m_ptrPriv->vComponentsList.begin();
	for(int i=1;i<iStartRow;++i)
	{
		if (iterStartComp!=iterEndComp)
			++iterStartComp;
	}
	std::vector<ComponentEntry>::iterator iterLastComp = iterStartComp;
	for(int i=0;i<count;++i)
		if (iterLastComp!=iterEndComp)
			++iterLastComp;
	m_ptrPriv->vComponentsList.erase(iterStartComp,iterLastComp);

	std::vector<std::vector<CompShopPriceEntry> >::iterator iterRow = m_ptrPriv->vvPricesMatrix.begin();
	std::vector<std::vector<CompShopPriceEntry> >::iterator iterEndRow = m_ptrPriv->vvPricesMatrix.end();
	for(int j=0;j<iStartRow;++j)
		if (iterRow!=iterRow)
			++iterRow;
	if (iterRow!=iterEndRow)
		m_ptrPriv->vvPricesMatrix.erase(iterRow);
	endRemoveRows();
	return true;
}
QModelIndex CComputerConfModel::index(int iRow, int iColumn, const QModelIndex &parent) const
{
	if (iRow<0 || iRow>=rowCount() || iColumn<0 || iColumn>=columnCount())
		return QModelIndex();
	return createIndex(iRow, iColumn);
}
QModelIndex CComputerConfModel::parent(const QModelIndex &child) const
{
	return QModelIndex();
}
//////////////////////////////////////////////////////////////////////////
void CComputerConfModel::addShopPlugin(ShopInterface * ptrPlugin)
{
	if (!ptrPlugin)	return;
	m_ptrPriv->mPlugins.insert(std::make_pair(ptrPlugin->shopName(),ptrPlugin));
	m_ptrPriv->m_strShopsList.append(ptrPlugin->shopName());
	m_ptrPriv->m_lShopIconsList.append(ptrPlugin->getShopIcon());
	m_ptrPriv->processHeaders();
	QModelIndex stTopLeft = index(0,0);
	QModelIndex stBottomRight = index(rowCount()-1,columnCount()-1);
	emit dataChanged(stTopLeft,stBottomRight);
}
void CComputerConfModel::removeShopPlugins()
{
	m_ptrPriv->mPlugins.clear();
	m_ptrPriv->m_strShopsList.clear();
	m_ptrPriv->m_lShopIconsList.clear();
}
void CComputerConfModel::resetWorseBestFlags()
{
	for(size_t iRow = 0; iRow<m_ptrPriv->vvPricesMatrix.size();++iRow)
		for(size_t iCol = 0;iCol<m_ptrPriv->vvPricesMatrix[iRow].size();++iCol)
		{
			m_ptrPriv->vvPricesMatrix[iRow][iCol].bBestPrice=false;
			m_ptrPriv->vvPricesMatrix[iRow][iCol].bWorsePrice=false;
		}
	QModelIndex stTopLeft = index(0,0);
	QModelIndex stBottomRight = index(rowCount()-1,columnCount()-1);
	emit dataChanged(stTopLeft,stBottomRight);
}

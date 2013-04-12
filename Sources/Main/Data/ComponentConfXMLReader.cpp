#include <Data/ComponentConfXMLReader.h>
#include <Data/Enumerations.h>
#include <Data/ComputerConfModel.h>
#include <QtXml/QXmlDefaultHandler>
#include <iostream>
#include <QtCore/QtDebug>
#include <QtCore/QDir>
#include <tools/loggers.h>

class ComputerConfXMLReaderPrivate : public QXmlDefaultHandler
{
public:
	ComputerConfXMLReaderPrivate(ComputerConfXMLReader * ptrPublic);
	//void startReadingShipComponents();
	//void startReadingResearch();
	//void startReadingData();

	//void parseMinerals(MineralRequired &stMinerals);

	bool startElement( const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts );
	bool endElement ( const QString & namespaceURI, const QString & localName, const QString & qName );
	bool characters ( const QString & ch );
	//////////////////////////////////////////////////////////////////////////
	//bool parseShipComponets(const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts);
	//bool parseResearch(const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts);
	//bool parseStartRaceFractions(const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts);
public:
	///Path to directory containing xml data files 
	QString					m_strDataDirectory;
	CComputerConfModel *	m_ptrModel;
	QString					strCurrentValue;
	bool					m_bShopActive;
	int						m_iDataCellIndex;
	int						m_iCompIndex; //component/row index
	ComputerConfXMLReader * m_ptrPublic;
	int						iShopCounts;
	int						iComponentsCounts;

};
ComputerConfXMLReaderPrivate::ComputerConfXMLReaderPrivate(ComputerConfXMLReader * ptrPublic):m_ptrPublic(ptrPublic),
m_iDataCellIndex(0)
{}
bool ComputerConfXMLReaderPrivate::startElement( const QString &, const QString & localName, const QString & qName, const QXmlAttributes & atts )
{
	if (qName =="CONFIGURATION")
	{
		iComponentsCounts=0;
		iShopCounts=0;
		return true;
	}
	if (qName=="SHOPS" || qName=="COMPONENTS" )
	{
#ifdef VIEW_WITH_FROZEN_ROW
		m_iCompIndex=1;
#else
		m_iCompIndex=0;
#endif
		return true;
	}
	if (qName == "SHOP")
	{
		m_bShopActive = atts.value("active")=="true"?true:false;
		return true;
	}
	if (qName == "COMPONENT")
	{
		int iComp=m_ptrModel->rowCount();
		m_ptrModel->insertRow(iComp);

		bool bActive = atts.value("active")=="true"?true:false;
		QString strCompName = atts.value("name");
		m_ptrModel->setHeaderData(m_iCompIndex,Qt::Vertical,bActive,Role_RowActive);
		m_ptrModel->setHeaderData(m_iCompIndex,Qt::Vertical,strCompName,Role_ComponentName);
		m_iDataCellIndex=1;
		return true;
	}
	if (qName == "DATA")
	{
		QModelIndex stIndex = m_ptrModel->index(m_iCompIndex,m_iDataCellIndex);
		double dPrice = atts.value("last_price").toDouble();
		m_ptrModel->setData(stIndex,dPrice,Qt::DisplayRole);
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	return false;
}
bool ComputerConfXMLReaderPrivate::endElement ( const QString & namespaceURI, const QString & localName, const QString & qName )
{
	if (qName =="CONFIGURATION" || qName=="SHOPS" || qName=="COMPONENTS" )
	{
		return true;
	}
	if (qName=="SHOP")
	{
		int iShop = m_ptrModel->columnCount();
		m_ptrModel->insertColumn(iShop);
		m_ptrModel->setHeaderData(iShop,Qt::Horizontal,m_bShopActive,Role_ShopColumnActive);
		m_ptrModel->setHeaderData(iShop,Qt::Horizontal,strCurrentValue,Role_ShopName);
		iShopCounts++;
		return true;
	}
	if (qName=="COMPONENT")
	{
		++m_iCompIndex;
		iComponentsCounts++;
		return true;
	}
	if (qName=="DATA")
	{
		QModelIndex stIndex = m_ptrModel->index(m_iCompIndex,m_iDataCellIndex);
		if (m_iCompIndex==0 && m_iDataCellIndex == 1)
		{
			//m_ptrModel->setData(stIndex,strCurrentValue,Role_CompShopURL);
		}
		m_ptrModel->setData(stIndex,strCurrentValue,Role_CompShopURL);
		++m_iDataCellIndex;
		return true;
	}
	////////////
	return false;
}
bool ComputerConfXMLReaderPrivate::characters ( const QString & ch )
{
	strCurrentValue = ch;
	return true;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
ComputerConfXMLReader::ComputerConfXMLReader(void)
{
	m_ptrPriv = new ComputerConfXMLReaderPrivate(this);
}
ComputerConfXMLReader::~ComputerConfXMLReader(void)
{
	delete m_ptrPriv;
};
bool ComputerConfXMLReader::loadXMLFile( QString strPath,CComputerConfModel * ptrModel )
{
	//m_ptrPriv->startReadingFile();
	QXmlSimpleReader reader;
	reader.setContentHandler(m_ptrPriv);
	reader.setErrorHandler(m_ptrPriv);  
	m_ptrPriv->m_ptrModel = ptrModel;
	printLog(eDebugLogLevel,eDebug,QString("Start loading saved configuration"));

	QFile file(strPath);
	if (!file.open(QIODevice::ReadOnly))
	{
		printLog(eErrorLogLevel,eDebug,QString("Error during file opening (%1): %2").arg(strPath).arg(file.errorString()));
		return false;
	}
	QXmlInputSource xmlInputSource(&file);

	if (!reader.parse(&xmlInputSource)) {
		printLog(eErrorLogLevel,eDebug,QString("Parse failed during %1  parsing. (%2)").arg(strPath).arg(reader.errorHandler()->errorString().toLatin1().data()));
		return false;
	}
	printLog(eDebugLogLevel,eDebug,QString("Configuration loading finished. Shops: %1, Components: %2").arg(m_ptrPriv->iShopCounts).arg(m_ptrPriv->iComponentsCounts));
	return true;
}
void ComputerConfXMLReader::saveXMLFile(QString strPath,CComputerConfModel * ptrModel)
{

}
//////////////////////////////////////////////////////////////////////////

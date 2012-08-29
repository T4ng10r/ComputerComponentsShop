#pragma once
#include <QtCore/QString>

class CComputerConfModel;
class ComputerConfXMLReaderPrivate;

//This thread will read 
class ComputerConfXMLReader
{
public:
	ComputerConfXMLReader(void);
	~ComputerConfXMLReader(void);
	bool loadXMLFile(QString strPath,CComputerConfModel * ptrModel);
	void saveXMLFile(QString strPath,CComputerConfModel * ptrModel);
protected:
	ComputerConfXMLReaderPrivate *		m_ptrPriv;
};
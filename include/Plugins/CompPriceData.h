#ifndef _CCS_COMP_PRICE_DATA_INCLUDE_
#define _CCS_COMP_PRICE_DATA_INCLUDE_
#include <QtCore/QMetaType>

enum SearchResultType
{
	SR_NONE,
	SR_COMPFOUND,
	SR_COMPNOTFOUND,
	SR_LOADINGFAIL,
	SR_LOADINGTIMEOUT
};

class CompPriceData
{
public:
	CompPriceData():strName(),dPrice(0),eSearchResult(SR_NONE){}
	CompPriceData(const CompPriceData & src):strName(src.strName),dPrice(src.dPrice),
		eSearchResult(src.eSearchResult),strCompURL(src.strCompURL),strCompSearchURL(src.strCompSearchURL){}
	void reset()
	{
		strName="";
		dPrice=0;
		eSearchResult=SR_NONE;
		strFailReason="";
		strCompURL="";
		strCompSearchURL="";
	}
	//CompPriceData(const CompPriceData * src):strName(src->strName),dPrice(src->dPrice),eSearchResult(src->eSearchResult){}
	QString				strName;
	double				dPrice;
	SearchResultType	eSearchResult;
	QString				strFailReason;
	QString				strCompURL;
	QString				strCompSearchURL;
};
Q_DECLARE_METATYPE(CompPriceData);

#endif //_CCS_COMP_PRICE_DATA_INCLUDE_

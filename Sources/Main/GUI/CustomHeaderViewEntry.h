#ifndef _CCS_CUSTOM_HEADER_VIEW_ENTRY_INCLUDE_
#define _CCS_CUSTOM_HEADER_VIEW_ENTRY_INCLUDE_
#include <QtCore/QAbstractItemModel>
#include <QtGui/QWidget>

class CCustomHeaderViewEntry : public QWidget
{
	Q_OBJECT
public:
	CCustomHeaderViewEntry(QWidget *parent = 0):QWidget(parent){};
	void setModel(QAbstractItemModel *model)
	{
		m_ptrModel = model;
	};
	virtual void updateData()=0;
	virtual void setSectionIndex(int iSection){};
	//virtual void retranslateUI();
signals:
	void removeEntry(int iSection);
protected slots:
	void onActiveClicked(bool){};
	void onRemoveClicked(bool){};
protected:
	QAbstractItemModel * m_ptrModel;
};
#endif //_CCS_CUSTOM_HEADER_VIEW_ENTRY_INCLUDE_

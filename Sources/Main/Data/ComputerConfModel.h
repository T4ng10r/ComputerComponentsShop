#ifndef _CCS_COMP_CONF_MODEL_INCLUDE_
#define _CCS_COMP_CONF_MODEL_INCLUDE_
#include <QtCore/QAbstractItemModel>

class CComputerConfModelPrivate;
class ShopInterface;

class CComputerConfModel : public QAbstractItemModel
{
	Q_OBJECT
	friend class CComputerConfModelPrivate;
public:
	CComputerConfModel(void);
	~CComputerConfModel(void);
	void reset();
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;
	virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;

	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const;
	virtual QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole );

	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

	virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
	virtual bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex());
	virtual bool removeColumns ( int column, int count, const QModelIndex & parent = QModelIndex() );
	virtual bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
	void addShopPlugin(ShopInterface * ptrPlugin);
	void removeShopPlugins();
	void getProductsToRefresh();
	void resetWorseBestFlags();
protected:
	CComputerConfModelPrivate *		m_ptrPriv;
};
#endif //_CCS_COMP_CONF_MODEL_INCLUDE_

#ifndef TILECACHE_H
#define TILECACHE_H
#include <QMap>
#include <QImage>
#include <QThread>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
class TileCache : public QThread
{
	Q_OBJECT
public:
	TileCache();
	void add(int x, int y, int z, QImage img);
	void getTile(int x, int y, int z);
	bool contains(int x, int y, int z);
	void zoomLevelChanged();
private:
	int Random(int low, int high);
	class TileReq
	{
	public:
		int x;
		int y;
		int z;
	};
	QList<TileReq> m_tileQueue;
	QMutex m_tileMutex;
	QMutex m_networkQueueMutex;
	QMap<int,QMap<int,QMap<int,bool> > > m_tileExistsMap;
	QString m_cacheLocation;
	void run();
	QNetworkAccessManager *m_nam;
	QMap<QNetworkReply*,QPair<QPair<int, int>, int> > m_tileIdList;
	QByteArray UserAgent;
signals:
	void tileRecv(int x,int y, int z, QImage tile);
	void localTileUpdate(int count);
	void networkTileUpdate(int count);
private slots:
	void networkFinished();
};

#endif // TILECACHE_H

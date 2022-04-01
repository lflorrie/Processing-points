#ifndef IMAGEITEM_H
#define IMAGEITEM_H
#include <QQuickPaintedItem>
#include <QQuickItem>
#include <QPainter>
#include <QImage>

class ImageItem : public QQuickPaintedItem
{
Q_OBJECT
	Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
	Q_PROPERTY(QString source READ source WRITE setsource NOTIFY sourceChanged)
public:
	ImageItem(QQuickItem *parent = nullptr);
	Q_INVOKABLE void setImage(const QImage &image);
	void paint(QPainter *painter);
	QImage image() const;
	const QString &source() const;
	void setsource(const QString &newSource);

signals:
	void imageChanged();
	void sourceChanged();

private:
	QImage current_image;
	QString m_source;
};
#endif // IMAGEITEM_H

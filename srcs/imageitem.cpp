#include "imageitem.h"

ImageItem::ImageItem(QQuickItem *parent) : QQuickPaintedItem(parent)
{
	this->current_image = QImage("C:/Users/lflor/Desktop/GreenPoints/Screenshot_4.jpg");
}

void ImageItem::paint(QPainter *painter)
{
	QRectF bounding_rect = boundingRect();
	QImage scaled = this->current_image.scaledToHeight(bounding_rect.height());
	QPointF center = bounding_rect.center() - scaled.rect().center();

//	if(center.x() < 0)
//		center.setX(0);
//	if(center.y() < 0)
//		center.setY(0);
   painter->drawImage(center, scaled);
}

QImage ImageItem::image() const
{    return this->current_image;
}

void ImageItem::setImage(const QImage &image)
{
	this->current_image = image;
	update();
}

const QString &ImageItem::source() const
{
	return m_source;
}

void ImageItem::setsource(const QString &newSource)
{
	if (m_source == newSource)
		return;
	m_source = newSource;
	this->current_image = QImage(newSource);
	emit sourceChanged();
}

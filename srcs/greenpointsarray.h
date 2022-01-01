#ifndef GREENPOINTSARRAY_H
#define GREENPOINTSARRAY_H

#include <QObject>
#include <greenpoints.h>
#include <QDir>

class GreenPointsArray : public QObject
{
	Q_OBJECT
public:
	explicit GreenPointsArray(QDir dir);
	GreenPointsArray() = default;
	~GreenPointsArray();

	QVector<GreenPoints*>	getValues() const;
	void					clear();
public slots:
	void					setValues(const QDir &dir);
	void					setCoef(double coef);

signals:
	void					progressChangedArr(float progress);
private:
	QVector<GreenPoints*> points_file;
	float	progress;
	double	coef = 1;
};

#endif // GREENPOINTSARRAY_H

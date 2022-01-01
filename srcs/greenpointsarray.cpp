#include "greenpointsarray.h"
#include <QAbstractEventDispatcher>
#include <QDebug>

GreenPointsArray::GreenPointsArray(QDir dir)
{
	setValues(dir);
}

GreenPointsArray::~GreenPointsArray()
{
	this->clear();
}

void GreenPointsArray::setValues(const QDir &dir)
{
	if (dir.exists())
	{
		QStringList files =  dir.entryList({"*.bmp"});
		for (int i = 0; i < files.count(); ++i) {
			points_file.push_back(new GreenPoints{ dir.path() + "\\" + files[i] });
			points_file[i]->setCoef(coef);
			points_file[i]->processing_image();
			progress = (i + 1.0) / files.count() * 100;
			emit progressChangedArr(progress);
			this->thread()->eventDispatcher()->processEvents(QEventLoop::AllEvents);
		}
	}
}

void GreenPointsArray::setCoef(double coef)
{
	this->coef = coef;
}

QVector<GreenPoints *> GreenPointsArray::getValues() const
{
	return points_file;
}

void GreenPointsArray::clear()
{
	for (auto now : points_file) {
		delete now;
	}
	points_file.clear();
}

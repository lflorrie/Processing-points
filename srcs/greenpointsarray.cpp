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
	QString fileName;

	cv::Mat background;

	if (useBackgroundSubtraction && !pathToBackground.isEmpty())
		background = cv::imread(cv::samples::findFile(pathToBackground.toStdString()));
	if (dir.exists())
	{
//		QStringList files =  dir.entryList(QStringList({"*.bmp"}));
		QStringList files =  dir.entryList(QStringList({{"*.bmp"}, {"*.jpg"}}));
		for (int i = 0; i < files.count(); ++i) {
			fileName = dir.path() + "/" + files[i];
			points_file.push_back(new GreenPoints{ fileName });
			points_file[i]->setFileName(fileName);
			points_file[i]->setCoef(coef);
			points_file[i]->setSensivity(Sensivity);
			points_file[i]->processing_image(background);
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

void GreenPointsArray::setSensivity(double Sensivity)
{
	this->Sensivity = Sensivity;
}

void GreenPointsArray::setBackgroundPath(const QString &newPathToBackground)
{
	pathToBackground = newPathToBackground;
}

void GreenPointsArray::setUseBackgroundSub(bool newUseBackgroundSubtraction)
{
	useBackgroundSubtraction = newUseBackgroundSubtraction;
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

#include "greenpointsproject.h"
#include <QDebug>
#include <QtMath>

GreenPointsProject::GreenPointsProject(QObject *parent) : QObject(parent)
{
	connect(&points_array, SIGNAL(progressChangedArr(float)), this, SLOT(progressChanged(float)));

	QTextStream stream(&config);
	if (!config.exists()) {
		config.open(QIODevice::ReadWrite);
		config_map["path_to_open"] = "";
		config_map["path_to_save"] = "";
		config_map["area_mm"] = "25";
		config_map["area_pix"] = "256";
		config_map["showOnlyMaxResult"] = "0";
		config_map["sensivity"] = "13";
		config_map["unitsArea"] = "";
		config_map["path_to_background"] = "";
		config_map["useBackgroundSubtraction"] = "0";

		for (const auto &i : config_map) {
			stream << i.first << " " << i.second << "\n";
		}
	} else {
		config.open(QIODevice::ReadWrite);
		QString key;
		QString value;
		QString line;

		while (!(line = stream.readLine()).isNull()) {
			QTextStream l(&line);
			l >> key >> value;
			config_map[key] = value;
		}
	}
	dir.setPath(config_map["path_to_open"]);
	points_array.setBackgroundPath(config_map["path_to_background"]);
	points_array.setUseBackgroundSub(config_map["useBackgroundSubtraction"] == "1");
	config.close();
}

GreenPointsProject::~GreenPointsProject()
{
	QTextStream stream(&config);
	config.open(QIODevice::WriteOnly);
	for (const auto &i : config_map) {
		stream << i.first << " " << i.second << "\n";
	}
	config.close();
}

void GreenPointsProject::procGreenPoints()
{
	points_array.clear();
	points_array.setCoef(getAreaMM() / getAreaPixels());
	points_array.setSensivity(getSensivity());
	points_array.setValues(dir);
}

QString GreenPointsProject::path() const
{
	return dir.path();
}

void GreenPointsProject::setPath(const QString& path)
{
	if (path == dir.path())
		return;
	dir.setPath(path);
	emit pathChanged(dir.path());
}

QString GreenPointsProject::pathSave() const
{
	return config_map.at("path_to_save");
}

void GreenPointsProject::setPathSave(const QString &path)
{
	if (path == pathSave())
		return;
	config_map["path_to_save"] = path;
	emit pathSaveChanged(path);
}

float GreenPointsProject::getProgress() const
{
	return progress;
}

void GreenPointsProject::setAreaMM(double value)
{
	config_map["area_mm"] = QString::number(value);
}

void GreenPointsProject::setAreaPixels(double value)
{
	config_map["area_pix"] = QString::number(value);
}

double GreenPointsProject::getAreaMM() const
{
	return config_map.at("area_mm").toDouble();
}

double GreenPointsProject::getAreaPixels() const
{
	return config_map.at("area_pix").toDouble();
}

void GreenPointsProject::receiveFromQml()
{
	s.clear();
	config_map["path_to_open"] = dir.path();
	procGreenPoints();
	QTextStream text(&s);

	text.setFieldAlignment(QTextStream::AlignLeft);
	maxGreenPoints = nullptr;
	if (getShowOnlyMaxResult()) {
		for (const GreenPoints *now : points_array.getValues()) {
			if (maxGreenPoints == nullptr || now->get_count_points() > maxGreenPoints->get_count_points()) {
				maxGreenPoints = now;
			}
		}
	}

	for (const GreenPoints *now : points_array.getValues()) {
		if (getShowOnlyMaxResult() && maxGreenPoints != nullptr)
			now = maxGreenPoints;
		if (now->get_count_points() > 0) {
			text.setFieldWidth(0);
			text << "FileName: " << now->getFileName() << "\n";
			text << "Filling frame: " << now->getFillingFrame() << "\n";
			text << "Count of points: " << now->get_count_points() << "\n";

			for (size_t i = 0; i < now->get_count_points(); ++i) {
				QString area = QString::number(now->get_contours_area()[i]);
				QString avgs = QString::number(now->get_cont_avgs()[i]);
				QString S = QString("S = %1").arg(area) + " " + getUnitsArea();
				QString I = QString("I = %1").arg(avgs);
				text.setFieldWidth(25);
				text << S << "\t";
				text << I;
				text.setFieldWidth(0);
				text << "\n";
			}
			if (getShowOnlyMaxResult()) {
				emit	sendToQml(s);
				return;
			}
		}
	}
	emit	sendToQml(s);
}

void GreenPointsProject::progressChanged(float progress)
{
	this->progress = progress;
	emit processingChanged(progress);
}

void GreenPointsProject::saveFile(const QString& name, const QString& filename)
{
	QFile file(filename.toLocal8Bit().data());
	config_map["path_to_save"] = filename;
	if ( file.open(QIODevice::ReadWrite) )
	{
		QTextStream stream(&file);
		stream << name << Qt::endl;
		stream << s << Qt::endl;
	}
}


bool GreenPointsProject::getShowOnlyMaxResult() const
{
	return config_map.at("showOnlyMaxResult") == "1";
}

void GreenPointsProject::setShowOnlyMaxResult(bool newShowOnlyMaxResult)
{
	if (getShowOnlyMaxResult() == newShowOnlyMaxResult)
		return;
	config_map["showOnlyMaxResult"] = QString::number(newShowOnlyMaxResult);
	emit showOnlyMaxResultChanged();
}

double GreenPointsProject::getSensivity() const
{
	return config_map.at("sensivity").toDouble();
}

void GreenPointsProject::setSensivity(double newSensivity)
{
	if (qFuzzyCompare(getSensivity(), newSensivity))
		return;
	config_map["sensivity"] = QString::number(newSensivity);
	emit sensivityChanged();
}

const QString &GreenPointsProject::getUnitsArea() const
{
	return config_map.at("unitsArea");
}

void GreenPointsProject::setunitsArea(const QString &newUnitsArea)
{
	if (getUnitsArea() == newUnitsArea)
		return;
	config_map["unitsArea"] = newUnitsArea;
	emit unitsAreaChanged();
}

const QString GreenPointsProject::getFileNameMaxGP() const
{
	return maxGreenPoints->getFileName();
}

const QImage GreenPointsProject::getContour() const
{
	return maxGreenPoints->getContour();
}

const QString &GreenPointsProject::getPathToBackground() const
{
	return config_map.at("path_to_background");;
}

void GreenPointsProject::setPathToBackground(const QString &newPathToBackground)
{
	if (newPathToBackground == getPathToBackground())
		return;
	points_array.setBackgroundPath(newPathToBackground);
	config_map["path_to_background"] = newPathToBackground;

	emit pathToBackgroundChanged();
}

bool GreenPointsProject::getUseBackgroundSubtraction() const
{
	return config_map.at("useBackgroundSubtraction") == "1";
}

void GreenPointsProject::setUseBackgroundSubtraction(bool newUseBackgroundSubtraction)
{
	if (getUseBackgroundSubtraction() == newUseBackgroundSubtraction)
		return;
	config_map["useBackgroundSubtraction"] = QString::number(newUseBackgroundSubtraction);
	points_array.setUseBackgroundSub(newUseBackgroundSubtraction);
	emit useBackgroundSubtractionChanged();
}

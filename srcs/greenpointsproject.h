#ifndef GREENPOINTSPROJECT_H
#define GREENPOINTSPROJECT_H

#include <QObject>
#include <QString>
#include <QDir>
#include "greenpoints.h"
#include <QTextStream>
#include <QQmlContext>
#include <QQmlProperty>
#include <QThread>
#include "greenpointsarray.h"
#include <QFile>
#include <map>

class GreenPointsProject : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
	Q_PROPERTY(float progress READ getProgress NOTIFY processingChanged)
	Q_PROPERTY(QString path_save READ pathSave WRITE setPathSave NOTIFY pathSaveChanged)
	Q_PROPERTY(double area_pixels READ getAreaPixels WRITE setAreaPixels NOTIFY areaPixelsChanged)
	Q_PROPERTY(double area_mm READ getAreaMM WRITE setAreaMM NOTIFY areaMMChanged)
	Q_PROPERTY(double sensivity READ getSensivity WRITE setSensivity NOTIFY sensivityChanged)
	Q_PROPERTY(bool showOnlyMaxResult READ getShowOnlyMaxResult WRITE setShowOnlyMaxResult NOTIFY showOnlyMaxResultChanged)
	Q_PROPERTY(bool useBackgroundSubtraction READ getUseBackgroundSubtraction WRITE setUseBackgroundSubtraction NOTIFY useBackgroundSubtractionChanged)
	Q_PROPERTY(QString unitsArea READ getUnitsArea WRITE setunitsArea NOTIFY unitsAreaChanged)
	Q_PROPERTY(QString fileNameMaxGP READ getFileNameMaxGP NOTIFY fileNameMaxGPChanged)
	Q_PROPERTY(QImage contour READ getContour NOTIFY contourChanged)
	Q_PROPERTY(QString pathToBackground READ getPathToBackground WRITE setPathToBackground NOTIFY pathToBackgroundChanged)


public:
	explicit GreenPointsProject(QObject *parent = nullptr);
	~GreenPointsProject();

	Q_INVOKABLE void	procGreenPoints();
	QString	path() const;
	void	setPath(const QString& path);
	QString	pathSave() const;
	void	setPathSave(const QString& path);
	float	getProgress() const;

	void	setAreaMM(double value);
	void	setAreaPixels(double value);

	double	getAreaMM() const;
	double	getAreaPixels() const;

	bool	getShowOnlyMaxResult() const;
	void	setShowOnlyMaxResult(bool newShowOnlyMaxResult);

	double getSensivity() const;
	void setSensivity(double newSensivity);

	const QString &getUnitsArea() const;
	void setunitsArea(const QString &newUnitsArea);

	const QString getFileNameMaxGP() const;

	const QImage getContour() const;

	const QString &getPathToBackground() const;
	void setPathToBackground(const QString &newPathToBackground);

	bool getUseBackgroundSubtraction() const;
	void setUseBackgroundSubtraction(bool newUseBackgroundSubtraction);

signals:
	void	sendToQml(QString text_stream);
	void	processingChanged(float progress);
	void	pathChanged(QString path);
	void	pathSaveChanged(QString path);
	void	areaMMChanged(double value);
	void	areaPixelsChanged(double value);

	void	showOnlyMaxResultChanged();

	void sensivityChanged();

	void unitsAreaChanged();

	void fileNameMaxGPChanged();

	void contourChanged();

	void pathToBackgroundChanged();

	void useBackgroundSubtractionChanged();

public slots:
	void	receiveFromQml();
	void	progressChanged(float progress);
	void	saveFile(const QString& name, const QString& filename);
private:
	void	parseConfig();

private:
	float	progress;
	QDir	dir;
	GreenPointsArray	points_array;
	const GreenPoints *maxGreenPoints = nullptr;

	QString s; // data experiment text


private: // config
	QFile	config{"GpConifg.txt"};
	std::map<QString, QString>	config_map;
};

#endif // GREENPOINTSPROJECT_H

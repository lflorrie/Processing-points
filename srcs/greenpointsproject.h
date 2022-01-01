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

signals:
	void	sendToQml(QString text_stream);
	void	processingChanged(float progress);
	void	pathChanged(QString path);
	void	pathSaveChanged(QString path);
	void	areaMMChanged(double value);
	void	areaPixelsChanged(double value);

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
	QString s; // data experiment text


private: // config
	QFile	config{"GpConifg.txt"};
	std::map<QString, QString>	config_map;
};

#endif // GREENPOINTSPROJECT_H

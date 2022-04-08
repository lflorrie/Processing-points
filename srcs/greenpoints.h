#ifndef GREENPOINTS_H
#define GREENPOINTS_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <sstream>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QThread>
#include <QImage>
#include <QPixmap>
#include <QDebug>

class GreenPoints : public QObject
{
	Q_OBJECT

public:
	GreenPoints();
	GreenPoints(const QString& path);

	void	processing_image(cv::Mat background);
	void	thresh_callback();

	size_t				get_count_points() const;
	std::vector<double>	get_contours_area() const;
	std::vector<double>	get_cont_avgs() const;
	const QImage		getContour() const;
	double				getFillingFrame() const;

	void	setCoef(double coef);
	void	setSensivity(double Sensivity);

	QString					getFileName() const;
	void					setFileName(const QString &newFileName);

signals:

	void	result_ready();

public slots:

	bool	write(const QString& source, const QString& data);

private:

	/* a = a - b */
	void	imageSubtraction(cv::Mat &a, const cv::Mat &b);

	double	calculateAreaContour(const cv::Mat &img, const std::vector<cv::Point> &contour);
	double	calculateMeanIntensivity(const cv::Mat &img, const std::vector<cv::Point> &contour);

private:

	QString path;
	QString	fileName;

	double thresh = 13;

	double coef = 1;

	cv::Mat src;
	cv::Mat src_gray;
	cv::Mat drawing;

	double	filling_frame = 0;
	size_t	count_points = 0;
	std::vector<double> contours_area;
	std::vector<double> cont_avgs;

};

#endif // GREENPOINTS_H

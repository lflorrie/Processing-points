#ifndef GREENPOINTS_H
#define GREENPOINTS_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <algorithm>
#include "opencv2/imgproc.hpp"
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <sstream>
#include <QString>
#include <QThread>

class GreenPoints : public QObject
{
	Q_OBJECT

public:
	GreenPoints();
	GreenPoints(const QString& path);
	void	processing_image();
	void	thresh_callback(int, void*);

	size_t	get_count_points() const;
	std::vector<double>	get_contours_area() const;
	std::vector<double>	get_cont_avgs() const;
	void	setCoef(double coef);
signals:
	void	result_ready();
public slots:
	bool write(const QString& source, const QString& data);
private:

	QString path;
	QString text_stream;

	int thresh = 29;
	cv::RNG rng{12345};
	double coef = 1;
	bool it = false;
	cv::Mat src;
	cv::Mat src_gray;

	size_t count_points = 0;
	std::vector<double> contours_area;
	std::vector<double> cont_avgs;

};

#endif // GREENPOINTS_H

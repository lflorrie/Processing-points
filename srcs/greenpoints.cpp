#include "greenpoints.h"
#include <QDebug>
using namespace cv;
using namespace std;

GreenPoints::GreenPoints()
{
}

GreenPoints::GreenPoints(const QString &path) : QObject(nullptr), path(path)
{
}

void GreenPoints::processing_image()
{
	if (path.isEmpty()) {
		cout << "Path is empty!\n";
		return;
	}
	src = imread(samples::findFile(path.toStdString()));
	if (src.empty())
	{
	   cout << "Could not open or find the image!\n" << endl;
	   exit(1);
	}
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	blur(src_gray, src_gray, Size(3, 3));
	thresh_callback(0, 0);
	emit result_ready();
}

void GreenPoints::thresh_callback(int, void *)
{
	Mat canny_output;

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	count_points = 0;
	Canny(src_gray, canny_output, thresh, thresh * 2);
	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC1);

	for (size_t i = 1; i < contours.size(); i+=2)
	{
	   cv::drawContours(drawing, contours, i, cv::Scalar(i), cv::FILLED);
	   contours_area.push_back(std::max(contourArea(contours[i - 1]), contourArea(contours[i])) * coef);
	   cv::Rect roi = cv::boundingRect(contours[i]);
	   cv::Scalar mean;
	   mean = cv::mean(src(roi), drawing(roi) == i);
	   cont_avgs.push_back((mean[0] + mean[1] + mean[2] + mean[3]));
	}
	count_points = contours.size() / 2;
}

size_t GreenPoints::get_count_points() const
{
	return count_points;
}

std::vector<double> GreenPoints::get_contours_area() const
{
	return std::move(contours_area);
}

std::vector<double> GreenPoints::get_cont_avgs() const
{
	return std::move(cont_avgs);
}

void GreenPoints::setCoef(double coef)
{
	this->coef = coef;
}

bool GreenPoints::write(const QString &source, const QString &data)
{
	if (source.isEmpty())
		return false;

	QFile file(source);
	if (!file.open(QFile::WriteOnly | QFile::Truncate))
		return false;

	QTextStream out(&file);
	out << data;
	file.close();
	return true;
}

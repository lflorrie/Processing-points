#include "greenpoints.h"

using namespace cv;
using namespace std;

GreenPoints::GreenPoints()
{
}

GreenPoints::GreenPoints(const QString &path) : QObject(nullptr), path(path)
{
}

void GreenPoints::imageSubtraction(cv::Mat &a, const cv::Mat &b)
{
	for (int i = 0; i < a.rows && i < b.rows; i++)
	{
		for (int j = 0; j < a.cols && j < b.cols; j++)
		{
			int diff1 = abs(a.at<Vec3b>(i, j)[0] - b.at<Vec3b>(i, j)[0]);
			int diff2 = abs(a.at<Vec3b>(i, j)[1] - b.at<Vec3b>(i, j)[1]);
			int diff3 = abs(a.at<Vec3b>(i, j)[2] - b.at<Vec3b>(i, j)[2]);

			if ((diff1 < thresh || diff2 < thresh || diff3 < thresh ))
			{
				a.at<Vec3b>(i, j)[0] = 0;
				a.at<Vec3b>(i, j)[1] = 0;
				a.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}
}

double GreenPoints::calculateAreaContour(const cv::Mat &img, const std::vector<cv::Point> &contour)
{
	int S = 0;

	auto [min_x, max_x] = std::minmax_element(contour.begin(), contour.end(),
			[](const Point &a, const Point &b) { return a.x < b.x; });
	auto [min_y, max_y] = std::minmax_element(contour.begin(), contour.end(),
			[](const Point &a, const Point &b) { return a.y < b.y; });

	for (int i = min_x->x; i <=  max_x->x; ++i)
	{
		for (int j = min_y->y; j <= max_y->y; ++j)
		{
			if (img.at<uchar>(j, i))
			{
				S++;
			}
		}
	}
	return S;
}

double GreenPoints::calculateMeanIntensivity(const cv::Mat &img, const std::vector<cv::Point> &contour)
{
	double I = 0;
	double countPxs = 0;

	auto [min_x, max_x] = std::minmax_element(contour.begin(), contour.end(),
			[](const Point &a, const Point &b) { return a.x < b.x; });
	auto [min_y, max_y] = std::minmax_element(contour.begin(), contour.end(),
			[](const Point &a, const Point &b) { return a.y < b.y; });

	for (int i = min_x->x; i <=  max_x->x; ++i)
	{
		for (int j = min_y->y; j <= max_y->y; ++j)
		{
			int pixelColor = img.at<uchar>(j, i);
			if (pixelColor > thresh)
			{
				countPxs++;
				I += pixelColor;
			}
		}
	}
	return I / countPxs;
}

void GreenPoints::processing_image(cv::Mat background)
{
	if (path.isEmpty()) {
		cout << "Path is empty!\n";
		return;
	}
	src = imread(samples::findFile(path.toLocal8Bit().data()));
	if (src.empty())
	{
	   cout << "Could not open or find the image!\n";
	   return;
	}
	if (!background.empty())
	{
		imageSubtraction(src, background);
	}
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	thresh_callback();
	emit result_ready();
}

void GreenPoints::thresh_callback()
{
	Mat						tempImg;
	vector<vector<Point>>	contours;
	vector<Vec4i>			hierarchy;

	threshold(src_gray, tempImg, thresh, 255, cv::THRESH_BINARY);
	findContours(tempImg, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	drawing = Mat::zeros(tempImg.size(), CV_8UC1);
	tempImg = Mat::zeros(tempImg.size(), CV_8UC1);

	for (size_t i = 0; i < contours.size(); i++)
	{
		cv::drawContours(tempImg, contours, i, 255, cv::FILLED);
		// areaContour
		contours_area.push_back(calculateAreaContour(tempImg, contours[i]) * coef);
		// meanContour
		cont_avgs.push_back(calculateMeanIntensivity(src_gray, contours[i]));
	}
	filling_frame = 0;
	for (int i = 0; i < src_gray.rows; ++i)
	{
		for (int j = 0; j < src_gray.cols; ++j)
		{
			if (src_gray.at<uchar>(i,j) >= thresh)
				++filling_frame;
		}
	}
	drawing = tempImg;
	count_points = contours.size();
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

void GreenPoints::setSensivity(double Sensivity)
{
	this->thresh = Sensivity;
}

QString GreenPoints::getFileName() const
{
	return fileName;
}

void GreenPoints::setFileName(const QString &newFileName)
{
	fileName = newFileName;
}

const QImage	GreenPoints::getContour() const
{
	return QImage(drawing.data, drawing.cols, drawing.rows, QImage::Format_Grayscale8);
}

double GreenPoints::getFillingFrame() const
{
	return filling_frame;
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

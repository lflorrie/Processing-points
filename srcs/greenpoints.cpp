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

			if ((diff1 < 20 || diff2 < 20 || diff3 < 20))
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

void GreenPoints::processing_image(cv::Mat background)
{
	if (path.isEmpty()) {
		cout << "Path is empty!\n";
		return;
	}
	src = imread(samples::findFile(path.toStdString()));
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
		cv::drawContours(drawing, contours, i, Scalar(i), cv::FILLED); // for mean

		// areaContour
		contours_area.push_back(calculateAreaContour(tempImg, contours[i]) * coef);

		// meanContour
		cv::Rect roi = cv::boundingRect(contours[i]);
		cv::Scalar mean;
		mean = cv::mean(src(roi), drawing(roi) == i);
		cont_avgs.push_back((mean[0] + mean[1] + mean[2] + mean[3]));
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

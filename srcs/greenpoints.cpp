#include "greenpoints.h"
#include <QDebug>
#include <algorithm>
using namespace cv;
using namespace std;

GreenPoints::GreenPoints()
{
}

GreenPoints::GreenPoints(const QString &path) : QObject(nullptr), path(path)
{
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
	   cout << "Could not open or find the image!\n" << endl;
	   exit(1);
	}
	if (!background.empty()) {
		for (int i = 0; i < src.rows && i < background.rows; i++) {
				for (int j = 0; j < src.cols && j < background.cols; j++) {
					int a = abs(src.at<Vec3b>(i, j)[0] - background.at<Vec3b>(i, j)[0]);
					int b = abs(src.at<Vec3b>(i, j)[1] - background.at<Vec3b>(i, j)[1]);
					int c = abs(src.at<Vec3b>(i, j)[2] - background.at<Vec3b>(i, j)[2]);

					if ((a < 20 || b < 20 || c < 20))
					{
						src.at<Vec3b>(i, j)[0] = 0;
						src.at<Vec3b>(i, j)[1] = 0;
						src.at<Vec3b>(i, j)[2] = 0;
					}
				}
		}
	}
	cvtColor(src, src_gray, COLOR_BGR2GRAY);
	blur(src_gray, src_gray, Size(1, 1));
//	drawing = src_gray;
//	drawing = src_gray;

	thresh_callback(0, 0);
	emit result_ready();
}

void GreenPoints::thresh_callback(int, void *)
{
	Mat						canny_output;
	vector<vector<Point>>	contours;
	vector<Vec4i>			hierarchy;

	count_points = 0;
//	Canny(src_gray, canny_output, 0, 255);
//	Canny(src_gray, canny_output,);

	threshold(src_gray, canny_output, 30, 255, cv::THRESH_BINARY);
//	findContours(drawing, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	findContours(canny_output, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	//	threshold(src_gray, canny_output, 0, 255, cv::THRESH_BINARY);
//	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	drawing = Mat::zeros(canny_output.size(), CV_8UC1); //CV_8UC1
	canny_output = Mat::zeros(canny_output.size(), CV_8UC1); //CV_8UC1
//	drawing = canny_output;
	Mat contour_now = Mat::zeros(canny_output.size(), CV_8UC1); //CV_8UC1

	for (size_t i = 0; i < contours.size(); i+=1)
	{

		cv::drawContours(canny_output, contours, i, 255, cv::FILLED);

		cv::drawContours(drawing, contours, i, Scalar(i), cv::FILLED);

		auto [min_x, max_x] = std::minmax_element(contours[i].begin(), contours[i].end(), [](const Point &a, const Point &b) {
			return a.x < b.x;
		});
		auto [min_y, max_y] = std::minmax_element(contours[i].begin(), contours[i].end(), [](const Point &a, const Point &b) {
			return a.y < b.y;
		});

			int S = 0;
			for (int i = min_x->x; i <=  max_x->x; ++i) {
				for (int j = min_y->y; j <= max_y->y; ++j) {
					if (canny_output.at<uchar>(j, i))
						S++;
				}
			}
//		auto max_x = std::max_element(contours[i].begin(), contours[i].end(), [](const Point &a, const Point &b) {
//			return a.x < b.x;
//		});
//		auto min_x = std::min_element(contours[i].begin(), contours[i].end(), [](const Point &a, const Point &b) {
//			return a.x < b.x;
//		});
//		qDebug() << "Min_x = " << min_x->x << " Max_x = " << max_x->x;
//		qDebug() << "Min_y = " << min_y->y << " Max_y = " << max_y->y;

//		std::max(contours[i].begin(), contours[i].end())->y;
//	Mat contour_now = Mat::zeros(, CV_8UC1);


		contours_area.push_back(S * coef);
//	   contours_area.push_back(contourArea(contours[i]) * coef);
//	   contours_area.push_back(std::max(contourArea(contours[i - 1]), contourArea(contours[i])) * coef);
	   cv::Rect roi = cv::boundingRect(contours[i]);

//	   cv::drawContours(drawing, roi);
//	   rectangle(drawing, roi.tl(), roi.br(), 255);
//	   contours_area.push_back(roi.area() * coef);
	   cv::Scalar mean;

	   mean = cv::mean(src(roi), drawing(roi) == i);


	   cont_avgs.push_back((mean[0] + mean[1] + mean[2] + mean[3]));

	}
	drawing = canny_output;
//	drawing = Mat::zeros(canny_output.size());
//	cvtColor(drawing, drawing, COLOR_)
	// src = src - background;
	// src -> src_gray
//	drawing = src_gray;
//	drawing = src;
//		drawing = canny_output;

	count_points = contours.size();

	// new

	//image_lbl->setPixmap(QPixmap::fromImage(QImage(src.data, src.cols, src.rows, src.step, QImage::Format_RGB888)));
	//imshow( source_window, src_gray );
	//const int max_thresh = 255;
	//createTrackbar( "Canny thresh:", source_window, &thresh, max_thresh, thresh_callback );
	//thresh_callback( 0, 0 );
	//waitKey();

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
	if (drawing.empty())
		qDebug() << "const QImage	GreenPoints::getContour() const... drawing.empty() =  EMPTY!";
	return QImage(drawing.data, drawing.cols, drawing.rows, QImage::Format_Grayscale8);
//	return QImage(drawing.data, drawing.cols, drawing.rows, QImage::Format_BGR888);
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

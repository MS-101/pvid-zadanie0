#include <iostream>
#include <opencv2/opencv.hpp>


void countOranges() {
	// input

	cv::Mat input = cv::imread("assets\\oranges.jpg");
	cv::imshow("input", input);

	// preprocessing

	cv::Mat grayscale;
	cv::cvtColor(input, grayscale, cv::COLOR_BGR2GRAY);
	cv::imshow("grayscale", grayscale);

	// binarization

	cv::Mat threshold;
	cv::threshold(grayscale, threshold, 0, 255, cv::THRESH_OTSU);
	cv::imshow("threshold", threshold);

	// postprocessing

	cv::Mat erode;
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(30, 30));
	cv::erode(threshold, erode, kernel);
	cv::imshow("erode", erode);

	// contour detection

	cv::Mat output = input.clone();
	int objectCount = 0;

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(erode, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	
	for (int i = 0; i < contours.size(); ++i) {
		std::vector<cv::Point> points = contours[i];

		if (cv::contourArea(points) > 100) {
			objectCount++;

			double centerX = 0.0;
			double centerY = 0.0;

			for (cv::Point point : points) {
				centerX += point.x;
				centerY += point.y;
			}

			centerX /= points.size();
			centerY /= points.size();

			int fontFace = cv::FONT_HERSHEY_PLAIN;
			int fontScale = 1;

			cv::Point fontPoint = cv::Point(centerX, centerY);
			cv::Scalar color = cv::Scalar(0, 255, 0);

			cv::putText(output, std::to_string(objectCount), fontPoint, fontFace, fontScale, color);
			cv::drawContours(output, contours, i, color);
		}
	}

	// output

	cv::imshow("output", output);
	std::cout << "Detected " << objectCount << " oranges." << std::endl;
	cv::waitKey();
}

void countBananas() {
	// input
	
	cv::Mat input = cv::imread("assets\\bananas.jpg");
	cv::imshow("input", input);

	// preprocessing

	cv::Mat grayscale;
	cv::cvtColor(input, grayscale, cv::COLOR_BGR2GRAY);
	cv::imshow("grayscale", grayscale);

	// binarization

	cv::Mat canny;
	cv::Canny(grayscale, canny, 50, 100);
	cv::imshow("canny", canny);

	// postprocessing

	cv::Mat dilate;
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10));
	cv::dilate(canny, dilate, kernel);
	cv::imshow("dilate", dilate);

	cv::Mat bananas;
	cv::bitwise_not(dilate, bananas);
	cv::imshow("bananas", bananas);

	cv::Mat background;
	cv::threshold(grayscale, background, 240, 255, cv::THRESH_BINARY);
	cv::imshow("background", background);
	
	cv::Mat bananasWithoutBackground = bananas.clone();
	bananasWithoutBackground.setTo(0, background);
	cv::imshow("bananas without background", bananasWithoutBackground);

	// contour detection

	cv::Mat output = input.clone();
	int objectCount = 0;

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(bananas, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); ++i) {
		std::vector<cv::Point> points = contours[i];

		if (cv::contourArea(points) > 100) {
			objectCount++;

			double centerX = 0.0;
			double centerY = 0.0;

			for (cv::Point point : points) {
				centerX += point.x;
				centerY += point.y;
			}

			centerX /= points.size();
			centerY /= points.size();

			int fontFace = cv::FONT_HERSHEY_PLAIN;
			int fontScale = 1;

			cv::Point fontPoint = cv::Point(centerX, centerY);
			cv::Scalar color = cv::Scalar(0, 0, 255);

			cv::putText(output, std::to_string(objectCount), fontPoint, fontFace, fontScale, color);
			cv::drawContours(output, contours, i, color);
		}
	}

	// output

	cv::imshow("output", output);
	std::cout << "Detected " << objectCount << " bananas." << std::endl;
	cv::waitKey();
}

int main()
{
	countOranges();
	countBananas();

	return 0;
}

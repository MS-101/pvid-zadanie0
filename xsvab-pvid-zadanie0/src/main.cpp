#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat preprocessImage(cv::Mat image) {
	cv::Mat grayscale;
	cv::cvtColor(image, grayscale, cv::COLOR_BGR2GRAY);

	cv::Mat median;
	cv::medianBlur(grayscale, median, 7);

	return median;
}

cv::Mat binarizeImage(cv::Mat image) {
	cv::Mat threshold;
	cv::threshold(image, threshold, 110, 255, cv::THRESH_BINARY);

	return threshold;
}

std::vector<std::vector<cv::Point>> getContours(cv::Mat image) {
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(image, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

	return contours;
}

bool isContourCircular(const std::vector<cv::Point>& contour) {
	double epsilon = 0.02 * cv::arcLength(contour, true);
	std::vector<cv::Point> approx;
	cv::approxPolyDP(contour, approx, epsilon, true);

	double circularity = 0.0;
	if (approx.size() >= 5) {
		cv::RotatedRect ellipse = cv::fitEllipse(approx);
		double area = cv::contourArea(contour);
		double ellipseArea = CV_PI * ellipse.size.width * ellipse.size.height / 4.0;
		circularity = area / ellipseArea;
	}

	return circularity > 0.6;
}

void countOranges() {
	cv::Mat image = cv::imread("assets\\oranges.jpg");
	cv::Mat preprocessed = preprocessImage(image);
	cv::Mat binarized = binarizeImage(preprocessed);
	std::vector<std::vector<cv::Point>> contours = getContours(binarized);

	int objectCount = 0;
	cv::Mat output = binarized;
	output.setTo(0);
	
	for (int i = 0; i < contours.size(); ++i) {
		if (cv::contourArea(contours[i]) > 10000 && isContourCircular(contours[i])) {
			cv::drawContours(output, contours, i, cv::Scalar(255), -1);
			objectCount++;
		}
	}

	cv::imshow("Oranges", output);
	std::cout << "Detected " << objectCount << " oranges." << std::endl;

	cv::waitKey();
}

void countBananas() {
	cv::Mat image = cv::imread("assets\\bananas.jpg");
	cv::Mat preprocessed = preprocessImage(image);
	cv::Mat binarized = binarizeImage(preprocessed);
	std::vector<std::vector<cv::Point>> contours = getContours(binarized);

	int objectCount = 0;
	cv::Mat output = binarized;
	output.setTo(0);

	for (int i = 0; i < contours.size(); ++i) {
		if (cv::contourArea(contours[i]) > 10000) {
			cv::drawContours(output, contours, i, cv::Scalar(255), -1);
			objectCount++;
		}
	}

	cv::imshow("Bananas", output);
	std::cout << "Detected " << objectCount << " bananas." << std::endl;

	cv::waitKey();
}

int main()
{
	countOranges();
	//countBananas();

	return 0;
}

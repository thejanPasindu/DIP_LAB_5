#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"

using namespace cv;
using namespace std;

void insertionSort(int arr[], int n)
{
	int i, key, j;
	for (i = 1; i < n; i++)
	{
		key = arr[i];
		j = i - 1;

		while (j >= 0 && arr[j] > key)
		{
			arr[j + 1] = arr[j];
			j = j - 1;
		}
		arr[j + 1] = key;
	}
}


int main(int argc, char* argv[])
{
	if (argc != 3) // check all command line arguments are given 
	{
		cout << "Some argument are missing" << endl;
		cout << "exe_file_name image_name kernal_size" << endl;
		return -1;
	}

	Mat img = imread(argv[1], IMREAD_GRAYSCALE); // Load the color image as Gray Image
	if (!img.data) // check image is available
	{
		cout << "Could not load image file" << endl;
		return -1;
	}

	// image measures 
	int height = img.rows;
	int width = img.cols;
	int widthstep = img.step;
	int ch = img.channels();
	printf("height: %d, widht: %d, width step: %d, chanels: %d\n", height, width, widthstep, ch);

	// create Copy of the image
	Mat img_copy_3x3 = img.clone();
	Mat img_copy_5x5 = img.clone();

	Mat img_copy_for_median = img.clone();

	// memory alocarion for sum and average of 3x3 kernel
	int sum;
	int avg_3x3 = 0;

	//  Neighbourhood Averaging

	// calculate Neighbourhood Averaging 3x3
	for (int i = 1; i < height-1; i++) {
		for (int j = 1; j < width-1; j++) {
			sum = 0;
			for (int k_h = i-1; k_h <= i+1; k_h++){
				for (int k_w = j-1; k_w <= j+1; k_w++){
					sum = sum + (int)img.at<uchar>(k_h, k_w);
				}
			}
			//sum_3x3 = (int)img.at<uchar>(i-1, j-1) + (int)img.at<uchar>(i-1, j) + (int)img.at<uchar>(i-1, j+1) + (int)img.at<uchar>(i, j-1) + (int)img.at<uchar>(i, j) + (int)img.at<uchar>(i, j+1) + (int)img.at<uchar>(i+1, j-1) + (int)img.at<uchar>(i+1, j) + (int)img.at<uchar>(i+1, j+1);
			avg_3x3 = sum / 9;
			img_copy_3x3.at<uchar>(i, j) = (uchar)avg_3x3;
		}
	}
	
	// calculate Neighbourhood Averaging 5x5
	for (int i = 2; i < height - 2; i++) {
		for (int j = 2; j < width - 2; j++) {
			sum = 0;
			for (int k_h = i - 2; k_h <= i + 2; k_h++) {
				for (int k_w = j - 2; k_w <= j + 2; k_w++) {
					sum = sum + (int)img.at<uchar>(k_h, k_w);
				}
			}
			avg_3x3 = sum / 25;
			img_copy_5x5.at<uchar>(i, j) = (uchar)avg_3x3;
		}
	}

	//Median Filtering
	
	int kernal_size = atoi(argv[2]);
	int adj = kernal_size / 2;

	int size = kernal_size * kernal_size;
	int* arr = new int[size];
	int kernal_index = 0;
	int middle = 0;

	for (int i = adj; i < height - adj; i++) {
		for (int j = adj; j < width - adj; j++) {
			kernal_index = 0;
			for (int k_h = i - adj; k_h <= i + adj; k_h++) {
				for (int k_w = j - adj; k_w <= j + adj; k_w++) {
					arr[kernal_index] = (int)img.at<uchar>(k_h, k_w);
					kernal_index++;
				}
			}
			insertionSort(arr, size);
			middle = size / 2;
			img_copy_for_median.at<uchar>(i, j) = (uchar)arr[middle];
		}
	}

	//original image
	namedWindow("Gray"); // Create the window
	imshow("Gray", img); // load and show image

	//Enhanced_Image_3x3
	namedWindow("Mean_Enhanced_Image_3x3"); // Create the window
	imshow("Mean_Enhanced_Image_3x3", img_copy_3x3); // load and show image

	//Enhanced_Image_5x5
	namedWindow("Mean_Enhanced_Image_5x5"); // Create the window
	imshow("Mean_Enhanced_Image_5x5", img_copy_5x5); // load and show image

	//Enhanced_Image_5x5
	namedWindow("Medean_Enhanced_Image"); // Create the window
	imshow("Medean_Enhanced_Image", img_copy_for_median); // load and show image

	waitKey(0); // wait to close

	return 0;

}
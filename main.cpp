#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <Windows.h>
using namespace cv;

constexpr auto default_ratio_threshold = 0.125;
constexpr auto windowName = "Reminder";

int main(int argc, const char** argv)
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE); //to disable the command window in the release version.
	CommandLineParser parser(argc, argv,
		"{help h||}"
		"{face_cascade|haarcascade_frontalface_alt2.xml|Path to face cascade.}"
		"{camera|0|Camera device number.}");

	CascadeClassifier face_cascade;
	if (!face_cascade.load(parser.get<String>("face_cascade")))
	{
		std::cerr << "Error loading face cascade\n";
		parser.printMessage();
		return -1;
	};

	VideoCapture capture;
	capture.open(parser.get<int>("camera"));
	if (!capture.isOpened())
	{
		std::cerr << "Error opening video capture\n";
		parser.printMessage();
		return -1;
	}

	double threshold = default_ratio_threshold;
	Mat frame;
	Mat frame_gray;
	std::vector<Rect> faces;
	while (capture.read(frame))
	{
		if (frame.empty())
		{
			std::cerr << "No captured frame, Break!\n";
			break;
		}
		cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
		pyrDown(frame_gray, frame_gray); //for perfomance

		face_cascade.detectMultiScale(frame_gray, faces);
		auto maxFace = std::max_element(faces.begin(), faces.end(), [](const Rect& left, const Rect& right)
			{
				return (left.area() < right.area());
			});

		if (maxFace != faces.end() && (double(maxFace->area()) / (frame_gray.rows * frame_gray.cols) > threshold))
		{
			//Make sound to notify user
			std::cout << '\a' << std::endl;
			//Pop up a window to notify user at the same time
			auto current_ratio = double(maxFace->area()) / (frame_gray.rows * frame_gray.cols);
			putText(frame, std::to_string(current_ratio) + " > " + std::to_string(threshold), { 0,30 },
				FONT_HERSHEY_COMPLEX, 1,
				Scalar(0, 0, 255), 2, LINE_AA);
			putText(frame, "  Sit Back ", { 100,100 },
				FONT_HERSHEY_COMPLEX, 2.1,
				Scalar(0, 0, 255), 2, LINE_AA);
			imshow(windowName, frame);
			setWindowProperty(windowName, WND_PROP_TOPMOST, 1);
			//Change the threshold via keyboard if needed.
			auto key = waitKey(1000);
			if (key == 's')
				threshold = current_ratio;  //the key "S" for set the threshold
			if (key == 'r')
				threshold = default_ratio_threshold;      //the key "R" for reset the threshold to default
			if (getWindowProperty(windowName, WND_PROP_VISIBLE) < 1) //check whether closed by user.
			{
				std::cout << "User close window, normal exit.\n";
				break;
			}
		}
		else
		{   //close window, because user's pos become good
			if (getWindowProperty(windowName, WND_PROP_VISIBLE) >= 1)
			{
				destroyWindow(windowName);
			}
			Sleep(10000);
		}
	}
	return 0;
}

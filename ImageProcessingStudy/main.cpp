#include "stdafx.h"

#define MAINVER 1
#define SUBVER1 0
#define SUBVER2 1

#define STR(s)     #s 
#define VERSION(a,b,c)  "System V" STR(a) "." STR(b) "." STR(c) " "__DATE__

using std::string;
using std::cout;
using std::endl;
using std::setprecision;
using std::fixed;

using namespace cv;

IplImage* hBitmapToIpl(HBITMAP hBmp);

int vision(void);

int main(int argc, char* argv[])
{
	/*
	Mount&Blade Warband
	HWND hpWnd = NULL;
	hpWnd = WindowFromPoint(pt);
	if (IsWindow(hpWnd))
	hcWnd = ::ChildWindowFromPointEx(hpWnd, pt, CWP_ALL);
	if (IsWindow(hcWnd) == FALSE)
	hcWnd = hpWnd;
	*/
	
	HWND hcWnd = NULL;
	HDC hDc, memDc;
	hcWnd = FindWindow(NULL, "Task Manager");//ͨ����������ȡ���
	hDc = GetDC(hcWnd);//ȡ�������豸 hdc  
	memDc = CreateCompatibleDC(hDc);//������HDC�����棩���ݵ��豸memDc  
	HBITMAP hBitMap;
	//HWND hWnd = GetDesktopWindow();//�õ�������  
	RECT rect;
	int width = 0, hight = 0, prevw = 0, prevh = 0;

	cout << VERSION(MAINVER, SUBVER1, SUBVER2) << endl;//û����
	cout << "\** successfully load program **/" << endl;
	while (waitKey(50) != 27 && IsWindow(hcWnd))//������ESC����Ӧ�ô����˳�ʱ�˳�����  
	{
		GetWindowRect(hcWnd, &rect);//����ľ��� 
		width = rect.right - rect.left;
		hight = rect.bottom - rect.top;
		if (prevw != width || prevh != hight)//�����ڴ�С�ı�ʱ��ӡ�´�С
		{
			cout << "width:" << width << " " << "hight:" << hight << endl;
			prevw = width;
			prevh = hight;
		}
		hBitMap = CreateCompatibleBitmap(hDc, width, hight);//������������ݵĻ���  
		SelectObject(memDc, hBitMap);//�����豸�ͻ���  
		BitBlt(memDc, 0, 0, width, hight, hDc, 0, 0, SRCCOPY);//�������������豸�︴��  
		IplImage* src = hBitmapToIpl(hBitMap);
		cvNamedWindow("Fetched Window");
		cvShowImage("Fetched Window", src);
		cvReleaseImage(&src);
		DeleteObject(hBitMap);
	}

	cout << "\** program terminated **/" << endl;
	destroyAllWindows();
	// getchar();
	return EXIT_SUCCESS;
}

IplImage* hBitmapToIpl(HBITMAP hBmp)
{
	BITMAP bmp;

	GetObject(hBmp, sizeof(BITMAP), &bmp);

	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;

	int depth = bmp.bmBitsPixel == 1 ? IPL_DEPTH_1U : IPL_DEPTH_8U;

	IplImage* img = cvCreateImage(cvSize(bmp.bmWidth, bmp.bmHeight), depth, nChannels);

	BYTE *pBuffer = new BYTE[bmp.bmHeight*bmp.bmWidth*nChannels];

	GetBitmapBits(hBmp, bmp.bmHeight*bmp.bmWidth*nChannels, pBuffer);

	memcpy(img->imageData, pBuffer, bmp.bmHeight*bmp.bmWidth*nChannels);
	delete pBuffer;

	IplImage *dst = cvCreateImage(cvGetSize(img), img->depth, 3);

	cvCvtColor(img, dst, CV_BGRA2BGR);
	cvReleaseImage(&img);

	return dst;
}

int vision(void)
{

	//initialize and allocate memory to load the video stream from camera  
	VideoCapture camera1(0), camera2(1);
	int cam_count;
	double fps;
	char buff[10];  // ���ڴ��֡�ʵ��ַ���
	double t = 0;
	int prevw1 = 0, prevw2 = 0, prevh1 = 0, prevh2 = 0;
	//camera1.set(CV_CAP_PROP_FRAME_WIDTH, 1080); //�޷�������Ĭ��ΪVideoCapture  640*480
	//camera1.set(CV_CAP_PROP_FRAME_HEIGHT, 960);
	//camera2.set(CV_CAP_PROP_FRAME_WIDTH, 1080);
	//camera2.set(CV_CAP_PROP_FRAME_HEIGHT, 960);

	while (1)
	{
		Mat3b frame1, frame2;
		t = (double)getTickCount();
		if (waitKey(1) == 1) { break; }
		if (camera1.isOpened() || camera2.isOpened())
		{
			//grab and retrieve each frames of the video sequentially 
			camera1 >> frame1;
			camera2 >> frame2;

			t = ((double)getTickCount() - t) / getTickFrequency();
			fps = 1.0 / t;
			sprintf_s(buff, "%.2f", fps, 100);      // ֡�ʱ�����λС��
			string fpsString("FPS:");
			fpsString += buff;                    // ��"FPS:"�����֡����ֵ�ַ���
			if (prevw1 != frame1.cols || prevw2 != frame1.cols || prevh1 != frame1.rows || prevh2 != frame1.rows)
			{
				//printf("fps: %.2f width:%d height:%d fps:%.2f\n", fps, frame.cols, frame.rows, capture.get(CV_CAP_PROP_FPS));
				cout << "camera1 fps:" << fixed << setprecision(2) << fps << " width: " << frame1.cols << " height: " << frame1.rows << " cvfps: " << camera1.get(CV_CAP_PROP_FPS) << endl; //(CV_CAP_PROP_FPS ֻ��ʾ0��ԭ��δ֪)
																																															//<< " MaxWidth " << camera1.get(CV_CAP_PROP_FRAME_WIDTH) << " MAxHeight " << camera1.get(CV_CAP_PROP_FRAME_HEIGHT) << endl; //�޷�������Ƿֱ���
				cout << "camera2 fps:" << fixed << setprecision(2) << fps << " width: " << frame2.cols << " height: " << frame2.rows << " cvfps: " << camera2.get(CV_CAP_PROP_FPS) << endl;
				//<< " MaxWidth " << camera2.get(CV_CAP_PROP_FRAME_WIDTH) << " MAxHeight " << camera2.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
				prevw1 = frame1.cols; prevw2 = frame1.cols; prevh1 = frame1.rows; prevh2 = frame1.rows;
			}


			putText(frame1, fpsString, Point(5, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0));
			namedWindow("Camera1 Test", CV_WINDOW_AUTOSIZE);//CV_WINDOW_NORMAL CV_WINDOW_AUTOSIZE
			imshow("Camera1 Test", frame1);
			putText(frame2, fpsString, Point(5, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0));
			namedWindow("Camera2 Test", CV_WINDOW_AUTOSIZE);//CV_WINDOW_NORMAL CV_WINDOW_AUTOSIZE
			imshow("Camera2 Test", frame2);
		}
		else
		{
			cout << "No Camera Input!" << std::endl;
			break;
		}
	}
	return EXIT_SUCCESS;
}
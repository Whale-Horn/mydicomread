#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkImageCast.h>
#include <vtkDICOMImageReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkImageData.h>
#include <vtkCoordinate.h>

using namespace cv;
using namespace std;

// 读入一个CT图，返回它的像素矩阵，使用OpenCV的Mat类型返回
void dicomread(string inputFilename, Mat& img, vtkSmartPointer<vtkDICOMImageReader>& reader)
{
    vtkSmartPointer<vtkImageCast> imageCast =
        vtkSmartPointer<vtkImageCast>::New();

    reader->SetFileName(inputFilename.c_str());
    reader->Update();

    imageCast->SetInputConnection(reader->GetOutputPort());
    imageCast->SetOutputScalarTypeToInt();
    imageCast->Update();

    vtkImageData* imageData = imageCast->GetOutput();
    int dims[3];
    imageData->GetDimensions(dims);

    img.create(dims[1], dims[0], CV_32SC1);

    for (int j = 0; j < dims[1]; j++)
    {
        for (int i = 0; i < dims[0]; i++)
        {
            int* pixel =
                (int*)(imageData->GetScalarPointer(i, j, 0)); // 修改此处，z轴设置为0，因此只读取一个二维平面的数据
            img.at<int>(j, i) = (*pixel);
        }
    }
}

void showdicom(Mat I)
{
    // 将图像归一化到0到255的范围
    double minVal, maxVal;
    minMaxLoc(I, &minVal, &maxVal);
    I = (I - minVal) * 255.0 / (maxVal - minVal);

    // 将图像转换为8位灰度图像
    I.convertTo(I, CV_8UC1);

    // 显示图像
    imshow("DICOM Image", I);
    waitKey(0);
}

int main()
{
    string filename;
    cout << "请输入DICOM图像的文件路径：";
    cin >> filename;

    Mat I1;

    vtkSmartPointer<vtkDICOMImageReader> reader =
        vtkSmartPointer<vtkDICOMImageReader>::New();

    // 读入dicom图
    dicomread(filename, I1, reader);
    flip(I1, I1, 0); // 可以根据需要翻转图像
    showdicom(I1);
}

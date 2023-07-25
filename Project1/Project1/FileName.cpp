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

// ����һ��CTͼ�������������ؾ���ʹ��OpenCV��Mat���ͷ���
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
                (int*)(imageData->GetScalarPointer(i, j, 0)); // �޸Ĵ˴���z������Ϊ0�����ֻ��ȡһ����άƽ�������
            img.at<int>(j, i) = (*pixel);
        }
    }
}

void showdicom(Mat I)
{
    // ��ͼ���һ����0��255�ķ�Χ
    double minVal, maxVal;
    minMaxLoc(I, &minVal, &maxVal);
    I = (I - minVal) * 255.0 / (maxVal - minVal);

    // ��ͼ��ת��Ϊ8λ�Ҷ�ͼ��
    I.convertTo(I, CV_8UC1);

    // ��ʾͼ��
    imshow("DICOM Image", I);
    waitKey(0);
}

int main()
{
    string filename;
    cout << "������DICOMͼ����ļ�·����";
    cin >> filename;

    Mat I1;

    vtkSmartPointer<vtkDICOMImageReader> reader =
        vtkSmartPointer<vtkDICOMImageReader>::New();

    // ����dicomͼ
    dicomread(filename, I1, reader);
    flip(I1, I1, 0); // ���Ը�����Ҫ��תͼ��
    showdicom(I1);
}

#include "pch.h"
#include "Image.h"
#include <wx/splitter.h>
#include <wx/graphics.h>
#include <opencv2/opencv.hpp>
#include "FaceRecognition.h"
#include "../ImageResourcesLib/convertmattowxbmp.h"
#include "FaceRecognitionView.h"
using namespace cv;

const std::string faceCascadePath = "C:/Program Files/opencv/sources/data/haarcascades/haarcascade_frontalface_default.xml";

/**
 * Image Constructor
 */
Image::Image(const wxString& filename, FaceRecognitionView* parent, FaceRecognition* facrec):Item(filename,parent, facrec)
{
 
}

/**
* Process the image to customize it to as needed
*/
void Image::Process()
{
    // Load the image from the provided path
     mImage = wxImage(mPath, wxBITMAP_TYPE_ANY);

    if (!mImage.IsOk()) {
        // Handle image loading error
        wxLogError("Failed to load image: %s", mPath);
    }
    DetectFaces();
}


void Image::Draw(std::shared_ptr<wxGraphicsContext> graphics)
{
    // Get the dimensions of the graphics context
    double contextWidth, contextHeight;
    graphics->GetSize(&contextWidth, &contextHeight);

    // half-width of window
    int halfWidth = static_cast<int>(contextWidth / 2.0);
    int halfHeight = static_cast<int>(contextHeight / 2.0);

    // Calculate the scaling factors to fit the image within the context while maintaining aspect ratio
    double scaleX = halfWidth / mImage.GetWidth();
    double scaleY = contextHeight / mImage.GetHeight();
    double scaleFactor = std::min(scaleX, scaleY);

    int newWidth, newHeight;
    // Calculate the new dimensions
    if (scaleFactor == 0) {
        //if the the image dimension are very large extending the window size
        newWidth = halfWidth * 0.98;
        newHeight = contextHeight;
    }
    else {
        newWidth = static_cast<int>(mImage.GetWidth() * scaleFactor);
        newHeight = static_cast<int>(mImage.GetHeight() * scaleFactor);
    }

    // Calculate the X,Y position for the first image to center it
    int image1X = (halfWidth - newWidth) / 2;
    int image1Y = (halfHeight - newHeight) ;

    // Rescale the image with a specific quality option (you can choose the one you prefer)
    //wxImage scaledImage = mImage.Rescale(newWidth, newHeight, wxIMAGE_QUALITY_HIGH);

    // Create a bitmap from the scaled image
    auto bitmap = graphics->CreateBitmapFromImage(mImage);
    graphics->DrawBitmap(bitmap, image1X, image1Y, newWidth, newHeight);

    graphics->DrawBitmap(mImageDetected, image1X+halfWidth, image1Y, newWidth, newHeight);

}


/**
*   Detect the faces in the image
*/
void Image::DetectFaces()
{
    // Load an image
    cv::Mat image = cv::imread(mPath.ToStdString());
    if (image.empty()) {
        std::cerr << "Image not found. Make sure to provide a valid image path." << std::endl;
        return;
    }
    
    // Initialize face detection classifier
    cv::CascadeClassifier face_cascade;
    // Load pre-trained XML classifier for face detection
    if (!face_cascade.load(faceCascadePath)) {
        std::cerr << "Error loading face cascade." << std::endl;
        return ;
    }

    // Convert the image to grayscale for detection
    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray_image, gray_image);

    // Detect faces
    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(gray_image, faces, 1.1, 3, 0);

    // Extract and display detected faces
    for (size_t i = 0; i < faces.size(); i++) {
        // Crop the detected face region
        cv::Mat face = image(faces[i]);
        mFaceRecognition->AddDetectedFace(face);
    }

    // Loop through the detected faces and draw bounding boxes
    for (const cv::Rect& face : faces) {
        cv::rectangle(image, face, cv::Scalar(0, 0, 255), 3);  // Draw a red rectangle around the face
    }

    mImageDetected = wxBitmap(image.cols, image.rows, 24);
    ConvertMatBitmapTowxBitmap(image, mImageDetected);
}

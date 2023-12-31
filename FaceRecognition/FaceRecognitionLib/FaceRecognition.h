#pragma once

#include "Item.h"
class Observer;
class FaceRecognitionView;

/**
** 
*/
class FaceRecognition
{	
private:

	//item we want implement face recogniton on
	std::unique_ptr<Item> mItem;

	/// The observers of this picture
	std::vector<Observer*> mObservers;

	std::vector<wxBitmap> mDetectedFaces;

public:
	void OnDraw(std::shared_ptr<wxGraphicsContext> graphics);
	FaceRecognition();
	void Save(const wxString& filename);
	void LoadImage(FaceRecognitionView* parent,const wxString& filename);
	void LoadVideo(FaceRecognitionView* parent, const wxString& filename);
	void Clear();
	void AddObserver(Observer* observer);
	void RemoveObserver(Observer* observer);
	void UpdateObservers();
	/**
   * Add the face that has been detected
  */
	virtual void AddDetectedFace(cv::Mat faceImage);

	/**
	* Clear the vector
	*/
	virtual void ClearDetectedFaces();
	std::vector<wxBitmap> GetDetectedFaces();

	void Update();

	int GetNumofDetectedFaces() { return mDetectedFaces.size(); }
};
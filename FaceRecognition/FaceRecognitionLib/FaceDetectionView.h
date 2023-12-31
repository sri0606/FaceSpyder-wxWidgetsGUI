#pragma once

#include "Observer.h"
/**
 * View class for the FaceDetectionView area of the screen.
 */
class FaceDetectionView final : public wxScrolledCanvas, public Observer {
private:

     virtual void OnPaint(wxPaintEvent& event) override;

     std::pair<int, int> GetNumofRowsCols(int contextWidth, int contextHeight);

    

public:
    static const int Height = 90;      ///< Height to make this window


    FaceDetectionView(wxPanel* parent);

    void UpdateObserver() override;

    virtual void OnImageOpen(wxCommandEvent& event) override;
    virtual void OnVideoOpen(wxCommandEvent& event) override;
    virtual void OnFileSaveAs(wxCommandEvent& event) override;
};

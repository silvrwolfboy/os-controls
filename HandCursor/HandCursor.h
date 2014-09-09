#pragma once

#include "LeapListener.h"
#include "Primitives.h"
#include "Color.h"

class HandCursor : public PrimitiveBase {
public:
  HandCursor();

  void Update(const Leap::Hand& hand);
  void InitChildren();

  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

protected:

  virtual void DrawContents(RenderState& renderState) const override;

private:
  static const int NUM_FINGERS = 5;
  
  float averageFingerBend(Leap::Finger finger, int startBone = 3, int endBone = 4) const;
  
  void formatFinger(const Leap::Finger& finger, float distance, bool isLeft = false);

  Color m_OutlineColor;
  Color m_FillColor;

  float m_PalmOutlineRadius;
  float m_PalmOutlineThickness;

  std::shared_ptr<PartialDisk> m_PalmOutline;
  std::shared_ptr<Disk> m_PalmCenter;
  std::shared_ptr<Disk> m_Fingers[5];

};

#pragma once
#include "Leap.h"
#include "TimeRecognizer.h"

enum class HandPose;

struct HandPinch {
  bool isPinching;
  float pinchStrength;
  float pinchDeltaPerSecond;
};

struct HandGrab {
  bool isGrabbing;
  float grabStrength;
  float grabDeltaPerSecond;
};

class HandActivationRecognizer {
public:
  HandActivationRecognizer();
  virtual~HandActivationRecognizer(){};
  
  void AutoFilter(const Leap::Hand& hand, const FrameTime& frameTime, HandGrab& handGrab, HandPinch& handPinch);
private:
  const float MIN_GRAB_START = 0.7;
  const float MIN_GRAB_CONTINUE = 0.6;
  const float MIN_PINCH_START = 0.75;
  const float MIN_PINCH_CONTINUE = 0.65;
  
  float getCustomPinchStrength(const Leap::Hand& hand) const;
  float calcHandPinchVelocity(const Leap::Finger& index, const Leap::Finger& thumb) const;
  
  bool m_wasPinching;
  float m_lastPinchStrength;
  
  bool m_wasGrabbing;
  float m_lastGrabStrength;
};
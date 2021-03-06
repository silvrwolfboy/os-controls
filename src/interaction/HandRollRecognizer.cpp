#include "stdafx.h"
#include "HandRollRecognizer.h"

HandRollRecognizer::HandRollRecognizer(void):
  m_hasLast(false),
  m_lastRoll(0.0),
  m_deltaRoll(0.0f,0.99f)
{
}

HandRollRecognizer::~HandRollRecognizer(void)
{
}

void HandRollRecognizer::AutoFilter(const Leap::Hand& hand, const FrameTime& frameTime, HandRoll& handRoll) {
  // Compute the roll amount, decide whether to floor it down to zero
  float roll = -hand.palmNormal().roll();
  
  handRoll.dTheta = roll - m_lastRoll;
  m_deltaRoll.SetGoal(roll - m_lastRoll);
  
  // TODO: this delta time is in microseconds
  // should convert to seconds before passing into Smoothed and adjust strength accordingly
  m_deltaRoll.Update(static_cast<float>(frameTime.deltaTime));
  
  // Need to keep track of what the last roll was, now
  m_lastRoll = roll;

  // Zeroize theta if we don't have a prior roll value
  if(!m_hasLast) {
    m_deltaRoll.SetImmediate(0.0);
    m_hasLast = true;
  }
  
  handRoll.dTheta = m_deltaRoll.Value();
  
  handRoll.absoluteRoll = roll;
}
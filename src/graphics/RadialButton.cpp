#include "stdafx.h"
#include "RadialButton.h"
#include "GLShaderLoader.h"
#include "RenderState.h"

#include <iostream>

RadialButton::RadialButton(const Vector3& center, float innerRadius, float width, float startAngle, float endAngle, const Vector3& offset) {
  Translation() = center + offset;

  m_partialDisk.SetDiffuseColor(Color(0.5f, 0.5f, 0.5f, 0.7f));
  m_partialDisk.SetAmbientFactor(0.9f);
  
  m_innerRadius = innerRadius;
  m_outerRadius = innerRadius + width;
  
  m_partialDisk.SetInnerRadius(m_innerRadius);
  m_partialDisk.SetOuterRadius(m_outerRadius);
  m_partialDisk.SetStartAngle(startAngle);
  m_partialDisk.SetEndAngle(endAngle);
}

RadialButton::~RadialButton() {
}

void RadialButton::nudge(float offset) {
  m_partialDisk.SetInnerRadius(m_innerRadius + offset);
  m_partialDisk.SetOuterRadius(m_outerRadius + offset);
}

void RadialButton::Render(const RenderFrame& frame) const {
  // draw primitives
  m_partialDisk.Draw(frame.renderState);
}
#include "AnimationLayer.h"

#include <cmath>

#include "GLBuffer.h"
#include "GLController.h"
#include "GLShader.h"
#include "GLShaderLoader.h"
#include "GLTexture2.h"
#include "GLTexture2Loader.h"
#include "Resource.h"

AnimationLayer::AnimationLayer()
:
m_Width(640),
m_Height(480),
m_Sphere1Translation(EigenTypes::Vector3::Zero()),
m_Sphere2Translation(EigenTypes::Vector3::Zero()),
m_Sphere3Translation(EigenTypes::Vector3::Zero()),
m_Sphere4Translation(EigenTypes::Vector3::Zero()),
m_time(0)
{
  m_shader = Resource<GLShader>("material");
  m_shader->CheckForTypedAttribute("position", GL_FLOAT_VEC3, VariableIs::OPTIONAL_BUT_WARN);
  m_shader->CheckForTypedAttribute("normal", GL_FLOAT_VEC3, VariableIs::OPTIONAL_BUT_WARN);
  m_shader->CheckForTypedAttribute("tex_coord", GL_FLOAT_VEC2, VariableIs::OPTIONAL_BUT_WARN);

  const Color color1(1.0f, 0.5f, 0.3f, 1.0f);
  m_Sphere1.SetRadius(6);
  m_Sphere1.Material().SetDiffuseLightColor(color1);
  m_Sphere1.Material().SetAmbientLightColor(color1);
  m_Sphere1.Material().SetAmbientLightingProportion(0.3f);
  m_Sphere1Translation.SetGoal(EigenTypes::Vector3::Zero());
  m_Sphere1Translation.SetSmoothStrength(0.95f);

  const Color color2(0.9f, 0.9f, 0.4f, 1.0f);
  m_Sphere2.SetRadius(5);
  m_Sphere2.Material().SetDiffuseLightColor(color2);
  m_Sphere2.Material().SetAmbientLightColor(color2);
  m_Sphere2.Material().SetAmbientLightingProportion(0.3f);
  m_Sphere2Translation.SetGoal(EigenTypes::Vector3::Zero());
  m_Sphere2Translation.SetSmoothStrength(0.85f);

  const Color color3(0.3f, 1.0f, 0.5f, 1.0f);
  m_Sphere3.SetRadius(4);
  m_Sphere3.Material().SetDiffuseLightColor(color3);
  m_Sphere3.Material().SetAmbientLightColor(color3);
  m_Sphere3.Material().SetAmbientLightingProportion(0.3f);
  m_Sphere3Translation.SetGoal(EigenTypes::Vector3::Zero());
  m_Sphere3Translation.SetSmoothStrength(0.75f);

  const Color color4(0.4f, 0.7f, 1.0f, 1.0f);
  m_Sphere4.SetRadius(3);
  m_Sphere4.Material().SetDiffuseLightColor(color4);
  m_Sphere4.Material().SetAmbientLightColor(color4);
  m_Sphere4.Material().SetAmbientLightingProportion(0.3f);
  m_Sphere4Translation.SetGoal(EigenTypes::Vector3::Zero());
  m_Sphere4Translation.SetSmoothStrength(0.65f);
}

AnimationLayer::~AnimationLayer() {

}

void AnimationLayer::Update(TimeDelta real_time_delta) {
  m_time += real_time_delta;

  const float dart1 = static_cast<float>(rand())/static_cast<float>(RAND_MAX);
  const float dart2 = static_cast<float>(rand())/static_cast<float>(RAND_MAX);
  const float dart3 = static_cast<float>(rand())/static_cast<float>(RAND_MAX);
  const float dart4 = static_cast<float>(rand())/static_cast<float>(RAND_MAX);

  if (dart1 < 0.01f) {
    m_Sphere1Translation.SetGoal(25*EigenTypes::Vector3::Random());
  }
  if (dart2 < 0.03f) {
    m_Sphere2Translation.SetGoal(25*EigenTypes::Vector3::Random());
  }
  if (dart3 < 0.06f) {
    m_Sphere3Translation.SetGoal(25*EigenTypes::Vector3::Random());
  }
  if (dart4 < 0.1f) {
    m_Sphere4Translation.SetGoal(25*EigenTypes::Vector3::Random());
  }

  m_Sphere1Translation.Update(static_cast<float>(real_time_delta));
  m_Sphere2Translation.Update(static_cast<float>(real_time_delta));
  m_Sphere3Translation.Update(static_cast<float>(real_time_delta));
  m_Sphere4Translation.Update(static_cast<float>(real_time_delta));

  m_Sphere1.Translation() = (EigenTypes::Vector3)m_Sphere1Translation;
  m_Sphere2.Translation() = (EigenTypes::Vector3)m_Sphere2Translation;
  m_Sphere3.Translation() = (EigenTypes::Vector3)m_Sphere3Translation;
  m_Sphere4.Translation() = (EigenTypes::Vector3)m_Sphere4Translation;
}

void AnimationLayer::Render(TimeDelta real_time_delta) const {
  glEnable(GL_DEPTH_TEST);

  // set renderer projection matrix
  const double fovRadians = (M_PI / 180.0) * 60;
  const double widthOverHeight = static_cast<double>(m_Width)/static_cast<double>(m_Height);
  const double nearClip = 1.0;
  const double farClip = 10000.0;
  m_Renderer.GetProjection().Perspective(fovRadians, widthOverHeight, nearClip, farClip);

  // set renderer modelview matrix
  const EigenTypes::Vector3 eyePos = 100*EigenTypes::Vector3::UnitZ();
  const EigenTypes::Vector3 lookAtPoint = EigenTypes::Vector3::Zero();
  const EigenTypes::Vector3 upVector = EigenTypes::Vector3::UnitY();
  //m_Renderer.SetShader(m_shader);
  m_Renderer.GetModelView().Reset();
  m_Renderer.GetModelView().LookAt(eyePos, lookAtPoint, upVector);

  m_shader->Bind();

  // set light position
  const EigenTypes::Vector3f desiredLightPos(0, 10, 10);
  const EigenTypes::Vector3f lightPos = desiredLightPos - eyePos.cast<float>();
  m_shader->SetUniformf("light_position", lightPos);

  // draw primitives
  PrimitiveBase::DrawSceneGraph(m_Sphere1, m_Renderer);
  PrimitiveBase::DrawSceneGraph(m_Sphere2, m_Renderer);
  PrimitiveBase::DrawSceneGraph(m_Sphere3, m_Renderer);
  PrimitiveBase::DrawSceneGraph(m_Sphere4, m_Renderer);

  m_shader->Unbind();
}

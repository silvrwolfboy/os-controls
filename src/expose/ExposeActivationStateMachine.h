#pragma once
#include "graphics/Renderable.h"
#include "graphics/RenderEngine.h"
#include "interaction/HandDataCombiner.h"
#include "uievents/OSCDomain.h"

#include "Primitives.h"
#include "Animation.h"
#include <autowiring/Autowiring.h>

class ExposeActivationStateMachine  :
public std::enable_shared_from_this<ExposeActivationStateMachine>,
public Renderable
{
public:
  ExposeActivationStateMachine();
  ~ExposeActivationStateMachine();
  
  void AutoInit();
  
  void AutoFilter(OSCState appState, const HandData& handData, const FrameTime& frameTime);
  
  void AnimationUpdate(const RenderFrame& renderFrame) override;
  void Render(const RenderFrame& renderFrame) const override;
  
private:
  const Color UNSELECTED_COLOR = Color(0.4f, 0.425f, 0.45f, 0.75f);
  const Color SELECTED_COLOR = Color(0.505f, 0.831f, 0.114f, 0.95f);
  const Color GOAL_COLOR = Color(0.505f, 0.831f, 0.114f, 1.0f);
  const float GOAL_BOTTOM_Y = 30.0f;
  const float PUSHER_BOTTOM_Y = 150.0f;
  
  void transitionToInactive();
  void resolveSelection();
  Color blendColor(Color c1, Color c2, float amnt);
  
  enum class State {
    
    //Media View is created but not focused.
    INACTIVE,
    
    //Taking user input, fading in, etc
    ACTIVE,
    
    //Wait for animation to fade out
    COMPLETE,
    
    //Tear everything down.
    FINAL
  };
  
  State m_state;
  
  std::shared_ptr<RectanglePrim> m_goalStrip;
  std::shared_ptr<RectanglePrim> m_pusherBar;
  
  bool m_armed;
  
  Smoothed<float> m_goalBottomY;
  Smoothed<float> m_pusherBottomY;
  
  Autowired<sf::RenderWindow> m_renderWindow; // Grabbing this for layout purposes.
  Autowired<RenderEngine> m_rootNode;
  AutoFired<OSCStateChangeEvent> m_stateChangeEvent;
};
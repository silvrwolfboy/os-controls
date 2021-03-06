#pragma once

/// <summary>
/// An event type that is raised when an event happens originating with the user
/// </summary>
class MediaViewEventListener {
public:
  virtual ~MediaViewEventListener(void);

  virtual void OnUserPlayPause(void) {};
  virtual void OnUserNextTrack(void) {};
  virtual void OnUserPrevTrack(void) {};
  virtual void OnInitializeVolume(void) {};
  virtual void OnUserChangedVolume(float volume) {};
};

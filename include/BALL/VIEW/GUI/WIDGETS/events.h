// $Id: events.h,v 1.5 2001/06/29 06:16:07 sturm Exp $

////////////////////////////////////////////////////////////////////////////////
//
//    !!! do not compile this class !!!
//
//		!!! class will be included in scene !!!
//
////////////////////////////////////////////////////////////////////////////////

#ifndef BALL_VIEW_GUI_WIDGETS_EVENTS_H
#define BALL_VIEW_GUI_WIDGETS_EVENTS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_NOTIFICATION_H
#	include <BALL/CONCEPT/notification.h>
#endif

#ifndef BALL_MATHS_QUATERNION_H
#	include <BALL/MATHS/quaternion.h>
#endif

#include <iostream>


#define BALL_VIEW_DECLARE_OPERATOR(event_a, event_b) \
friend _##event_a##event_b##Event &operator & \
  (const _##event_a##Event &__r_##event_a##Event, \
   const _##event_b##Event &__r_##event_b##Event) \
  throw(Events::EventCombinationNotAllowed); \
\
friend _##event_a##event_b##Event &operator & \
  (const _##event_b##Event &__r_##event_b##Event, \
   const _##event_a##Event &__r_##event_a##Event) \
  throw(Events::EventCombinationNotAllowed)



#define BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE(event_a, event_b, returntype) \
friend _##returntype##Event &operator & \
  (const _##event_a##Event &__r_##event_a##Event, \
   const _##event_b##Event &__r_##event_b##Event) \
  throw(Events::EventCombinationNotAllowed); \
\
friend _##returntype##Event &operator & \
  (const _##event_b##Event &__r_##event_b##Event, \
   const _##event_a##Event &__r_##event_a##Event) \
  throw(Events::EventCombinationNotAllowed)



#define BALL_VIEW_IMPLEMENT_OPERATOR(event_a, event_b) \
Events::_##event_a##event_b##Event & \
operator & \
  (const Events::_##event_a##Event &__r_##event_a##Event, \
   const Events::_##event_b##Event &__r_##event_b##Event) \
  throw(Events::EventCombinationNotAllowed) \
{ \
  if (__r_##event_a##Event.getScene() \
      != __r_##event_b##Event.getScene()) \
  { \
    throw Events::EventCombinationNotAllowed(__FILE__, __LINE__); \
  } \
\
  return __r_##event_a##Event.getEvent()->##event_a##event_b; \
} \
\
Events::_##event_a##event_b##Event & \
operator & \
  (const Events::_##event_b##Event &__r_##event_b##Event, \
   const Events::_##event_a##Event &__r_##event_a##Event) \
  throw(Events::EventCombinationNotAllowed) \
{ \
  return operator &(__r_##event_a##Event, __r_##event_b##Event); \
}



#define BALL_VIEW_IMPLEMENT_OPERATOR_WITH_RETURN_TYPE(event_a, event_b, returntype) \
Events::_##returntype##Event & \
operator & \
  (const Events::_##event_a##Event &__r_##event_a##Event, \
   const Events::_##event_b##Event &__r_##event_b##Event) \
  throw(Events::EventCombinationNotAllowed) \
{ \
  if (__r_##event_a##Event.getScene() \
      != __r_##event_b##Event.getScene()) \
  { \
    throw Events::EventCombinationNotAllowed(__FILE__, __LINE__); \
  } \
\
  return __r_##event_a##Event.getEvent()->##returntype; \
} \
\
Events::_##returntype##Event & \
operator & \
  (const Events::_##event_b##Event &__r_##event_b##Event, \
   const Events::_##event_a##Event &__r_##event_a##Event) \
  throw(Events::EventCombinationNotAllowed) \
{ \
  return operator &(__r_##event_a##Event, __r_##event_b##Event); \
}




#define BALL_VIEW_DECLARE_EVENT_CLASS(name) \
class _##name##Event \
{ \
  public: \
\
    _##name##Event \
      () \
      throw() \
	: \
        __mpEvents_(0) \
    { \
    } \
\
    _##name##Event \
      (Events *__pEvents) \
      throw() \
	: \
        __mpEvents_(__pEvents) \
    { \
    } \
\
    ~_##name##Event \
      () \
      throw() \
    { \
    } \
\
    Scene *getScene \
      () const\
      throw() \
    { \
      return __mpEvents_->getScene(); \
    } \
\
    Events *getEvent \
      () const\
      throw() \
    { \
      return __mpEvents_; \
    } \
\
    Events *__mpEvents_; \
} name




#define BALL_VIEW_DECLARE_PRESSED_MOVED_EVENT_CLASS(name) \
class _##name##Event: \
  public NotificationTarget<_MouseLeftButtonPressedMouseMovedEvent>, \
  public NotificationTarget<_MouseLeftButtonPressedShiftKeyPressedMouseMovedEvent>, \
  public NotificationTarget<_MouseLeftButtonPressedControlKeyPressedMouseMovedEvent>, \
  public NotificationTarget<_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent>, \
  public NotificationTarget<_MouseMiddleButtonPressedMouseMovedEvent>, \
  public NotificationTarget<_MouseMiddleButtonPressedShiftKeyPressedMouseMovedEvent>, \
  public NotificationTarget<_MouseMiddleButtonPressedControlKeyPressedMouseMovedEvent>, \
  public NotificationTarget<_MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent>, \
  public NotificationTarget<_MouseRightButtonPressedMouseMovedEvent>, \
  public NotificationTarget<_MouseRightButtonPressedShiftKeyPressedMouseMovedEvent>, \
  public NotificationTarget<_MouseRightButtonPressedControlKeyPressedMouseMovedEvent>, \
  public NotificationTarget<_MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent>, \
  public NotificationTarget<_##name##Event> \
{ \
  public: \
\
    _##name##Event \
      () \
      throw() \
      : \
      __mfp(0), \
      __mpScene_(0), \
      transmitter__mpScene_(0) \
    { \
    } \
\
    _##name##Event \
      (Scene *__pScene) \
      throw() \
      : \
      __mfp(0), \
      __mpScene_(__pScene), \
      transmitter__mpScene_(__pScene) \
    { \
    } \
\
    virtual ~_##name##Event \
      () \
      throw() \
    { \
    } \
\
    Scene *getScene \
      () const \
      throw(); \
\
    Scene *getTransmitterScene \
      () const \
      throw(); \
\
    bool onNotify \
      (_MouseLeftButtonPressedMouseMovedEvent & \
       __r_MouseLeftButtonPressedMouseMovedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseLeftButtonPressedShiftKeyPressedMouseMovedEvent & \
       __r_MouseLeftButtonPressedShiftKeyPressedMouseMovedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseLeftButtonPressedControlKeyPressedMouseMovedEvent & \
       __r_MouseLeftButtonPressedControlKeyPressedMouseMovedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent & \
       __r_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseMiddleButtonPressedMouseMovedEvent & \
       __r_MouseMiddleButtonPressedMouseMovedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseMiddleButtonPressedShiftKeyPressedMouseMovedEvent & \
       __r_MouseMiddleButtonPressedShiftKeyPressedMouseMovedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseMiddleButtonPressedControlKeyPressedMouseMovedEvent & \
       __r_MouseMiddleButtonPressedControlKeyPressedMouseMovedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent & \
       __r_MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseRightButtonPressedMouseMovedEvent & \
       __r_MouseRightButtonPressedMouseMovedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseRightButtonPressedShiftKeyPressedMouseMovedEvent & \
       __r_MouseRightButtonPressedShiftKeyPressedMouseMovedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseRightButtonPressedControlKeyPressedMouseMovedEvent & \
       __r_MouseRightButtonPressedControlKeyPressedMouseMovedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent & \
       __r_MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent) \
      throw(); \
\
    bool onNotify \
      (_##name##Event & \
       __r_##name##Event) \
      throw(); \
\
    void register##name## \
      (void (Scene::*__fp)(Scene *__pScene)) \
      throw(); \
\
 private: \
\
    bool _onNotify \
      (Scene *__pScene) \
      throw(); \
\
    void (Scene::*__mfp)(Scene *__pScene); \
\
    Scene *__mpScene_; \
    Scene *transmitter__mpScene_; \
} name



#define BALL_VIEW_IMPLEMENT_INLINE_PRESSED_MOVED_EVENT_CLASS(name) \
BALL_INLINE \
Scene * \
Events::_##name##Event::getScene \
  () const \
  throw() \
{ \
  return __mpScene_; \
} \
\
BALL_INLINE \
Scene * \
Events::_##name##Event::getTransmitterScene \
  () const \
  throw() \
{ \
  return transmitter__mpScene_; \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonPressedMouseMovedEvent & \
   __r_MouseLeftButtonPressedMouseMovedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseLeftButtonPressedMouseMovedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonPressedShiftKeyPressedMouseMovedEvent & \
   __r_MouseLeftButtonPressedShiftKeyPressedMouseMovedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseLeftButtonPressedShiftKeyPressedMouseMovedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonPressedControlKeyPressedMouseMovedEvent & \
   __r_MouseLeftButtonPressedControlKeyPressedMouseMovedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseLeftButtonPressedControlKeyPressedMouseMovedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent & \
   __r_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonPressedMouseMovedEvent & \
   __r_MouseMiddleButtonPressedMouseMovedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseMiddleButtonPressedMouseMovedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonPressedShiftKeyPressedMouseMovedEvent & \
   __r_MouseMiddleButtonPressedShiftKeyPressedMouseMovedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseMiddleButtonPressedShiftKeyPressedMouseMovedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonPressedControlKeyPressedMouseMovedEvent & \
   __r_MouseMiddleButtonPressedControlKeyPressedMouseMovedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseMiddleButtonPressedControlKeyPressedMouseMovedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent & \
   __r_MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent. \
		   getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonPressedMouseMovedEvent & \
   __r_MouseRightButtonPressedMouseMovedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseRightButtonPressedMouseMovedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonPressedShiftKeyPressedMouseMovedEvent & \
   __r_MouseRightButtonPressedShiftKeyPressedMouseMovedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseRightButtonPressedShiftKeyPressedMouseMovedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonPressedControlKeyPressedMouseMovedEvent & \
   __r_MouseRightButtonPressedControlKeyPressedMouseMovedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseRightButtonPressedControlKeyPressedMouseMovedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent & \
   __r_MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent. \
		   getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_##name##Event & \
   __r_##name##Event) \
  throw() \
{ \
  if (__r_##name##Event.getTransmitterScene()  \
      == transmitter__mpScene_) \
  { \
    return true; \
  } \
 \
  return _onNotify(__r_##name##Event.getTransmitterScene()); \
} \
\
BALL_INLINE \
void \
Events::_##name##Event::register##name## \
  (void (Scene::*__fp)(Scene *__pScene)) \
  throw() \
{ \
  __mfp = __fp; \
}
    


#define BALL_VIEW_DECLARE_PRESSED_EVENT_CLASS(name) \
class _##name##Event: \
  public NotificationTarget<_MouseLeftButtonPressedEvent>, \
  public NotificationTarget<_MouseLeftButtonPressedShiftKeyPressedEvent>, \
  public NotificationTarget<_MouseLeftButtonPressedControlKeyPressedEvent>, \
  public NotificationTarget<_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedEvent>, \
  public NotificationTarget<_MouseMiddleButtonPressedShiftKeyPressedEvent>, \
  public NotificationTarget<_MouseMiddleButtonPressedControlKeyPressedEvent>, \
  public NotificationTarget<_MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedEvent>, \
  public NotificationTarget<_MouseRightButtonPressedEvent>, \
  public NotificationTarget<_MouseRightButtonPressedShiftKeyPressedEvent>, \
  public NotificationTarget<_MouseRightButtonPressedControlKeyPressedEvent>, \
  public NotificationTarget<_MouseRightButtonPressedShiftKeyPressedControlKeyPressedEvent>, \
  public NotificationTarget<_##name##Event> \
{ \
  public: \
\
    _##name##Event \
      () \
      throw() \
      : \
      __mfp(0), \
      __mpScene_(0), \
      transmitter__mpScene_(0) \
    { \
    } \
\
    _##name##Event \
      (Scene *__pScene) \
      throw() \
      : \
      __mfp(0), \
      __mpScene_(__pScene), \
      transmitter__mpScene_(__pScene) \
    { \
    } \
\
    virtual ~_##name##Event \
      () \
      throw() \
    { \
    } \
\
    Scene *getScene \
      () const \
      throw(); \
\
    Scene *getTransmitterScene \
      () const \
      throw(); \
\
    bool onNotify \
      (_MouseLeftButtonPressedEvent & \
       __r_MouseLeftButtonPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseLeftButtonPressedShiftKeyPressedEvent & \
       __r_MouseLeftButtonPressedShiftKeyPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseLeftButtonPressedControlKeyPressedEvent & \
       __r_MouseLeftButtonPressedControlKeyPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedEvent & \
       __r_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseMiddleButtonPressedEvent & \
       __r_MouseMiddleButtonPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseMiddleButtonPressedShiftKeyPressedEvent & \
       __r_MouseMiddleButtonPressedShiftKeyPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseMiddleButtonPressedControlKeyPressedEvent & \
       __r_MouseMiddleButtonPressedControlKeyPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedEvent & \
       __r_MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseRightButtonPressedEvent & \
       __r_MouseRightButtonPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseRightButtonPressedShiftKeyPressedEvent & \
       __r_MouseRightButtonPressedShiftKeyPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseRightButtonPressedControlKeyPressedEvent & \
       __r_MouseRightButtonPressedControlKeyPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseRightButtonPressedShiftKeyPressedControlKeyPressedEvent & \
       __r_MouseRightButtonPressedShiftKeyPressedControlKeyPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_##name##Event & \
       __r_##name##Event) \
      throw(); \
\
    void register##name## \
      (void (Scene::*__fp)(Scene *__pScene)) \
      throw(); \
\
 private: \
\
    bool _onNotify \
      (Scene *__pScene) \
      throw(); \
\
    void (Scene::*__mfp)(Scene *__pScene); \
\
    Scene *__mpScene_; \
    Scene *transmitter__mpScene_; \
} name



#define BALL_VIEW_IMPLEMENT_INLINE_PRESSED_EVENT_CLASS(name) \
BALL_INLINE \
Scene * \
Events::_##name##Event::getScene \
  () const \
  throw() \
{ \
  return __mpScene_; \
} \
\
BALL_INLINE \
Scene * \
Events::_##name##Event::getTransmitterScene \
  () const \
  throw() \
{ \
  return transmitter__mpScene_; \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonPressedEvent & \
   __r_MouseLeftButtonPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseLeftButtonPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonPressedShiftKeyPressedEvent & \
   __r_MouseLeftButtonPressedShiftKeyPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseLeftButtonPressedShiftKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonPressedControlKeyPressedEvent & \
   __r_MouseLeftButtonPressedControlKeyPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseLeftButtonPressedControlKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedEvent & \
   __r_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonPressedEvent & \
   __r_MouseMiddleButtonPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseMiddleButtonPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonPressedShiftKeyPressedEvent & \
   __r_MouseMiddleButtonPressedShiftKeyPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseMiddleButtonPressedShiftKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonPressedControlKeyPressedEvent & \
   __r_MouseMiddleButtonPressedControlKeyPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseMiddleButtonPressedControlKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedEvent & \
   __r_MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedEvent. \
		   getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonPressedEvent & \
   __r_MouseRightButtonPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseRightButtonPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonPressedShiftKeyPressedEvent & \
   __r_MouseRightButtonPressedShiftKeyPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseRightButtonPressedShiftKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonPressedControlKeyPressedEvent & \
   __r_MouseRightButtonPressedControlKeyPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseRightButtonPressedControlKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonPressedShiftKeyPressedControlKeyPressedEvent & \
   __r_MouseRightButtonPressedShiftKeyPressedControlKeyPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseRightButtonPressedShiftKeyPressedControlKeyPressedEvent. \
		   getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_##name##Event & \
   __r_##name##Event) \
  throw() \
{ \
  if (__r_##name##Event.getTransmitterScene()  \
      == transmitter__mpScene_) \
  { \
    return true; \
  } \
 \
  return _onNotify(__r_##name##Event.getTransmitterScene()); \
} \
\
BALL_INLINE \
void \
Events::_##name##Event::register##name## \
  (void (Scene::*__fp)(Scene *__pScene)) \
  throw() \
{ \
  __mfp = __fp; \
}
    


#define BALL_VIEW_DECLARE_RELEASED_EVENT_CLASS(name) \
class _##name##Event: \
  public NotificationTarget<_MouseLeftButtonReleasedEvent>, \
  public NotificationTarget<_MouseLeftButtonReleasedShiftKeyPressedEvent>, \
  public NotificationTarget<_MouseLeftButtonReleasedControlKeyPressedEvent>, \
  public NotificationTarget<_MouseLeftButtonReleasedShiftKeyPressedControlKeyPressedEvent>, \
  public NotificationTarget<_MouseMiddleButtonReleasedShiftKeyPressedEvent>, \
  public NotificationTarget<_MouseMiddleButtonReleasedControlKeyPressedEvent>, \
  public NotificationTarget<_MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressedEvent>, \
  public NotificationTarget<_MouseRightButtonReleasedEvent>, \
  public NotificationTarget<_MouseRightButtonReleasedShiftKeyPressedEvent>, \
  public NotificationTarget<_MouseRightButtonReleasedControlKeyPressedEvent>, \
  public NotificationTarget<_MouseRightButtonReleasedShiftKeyPressedControlKeyPressedEvent>, \
  public NotificationTarget<_##name##Event> \
{ \
  public: \
\
    _##name##Event \
      () \
      throw() \
      : \
      __mfp(0), \
      __mpScene_(0), \
      transmitter__mpScene_(0) \
    { \
    } \
\
    _##name##Event \
      (Scene *__pScene) \
      throw() \
      : \
      __mfp(0), \
      __mpScene_(__pScene), \
      transmitter__mpScene_(__pScene) \
    { \
    } \
\
    virtual ~_##name##Event \
      () \
      throw() \
    { \
    } \
\
    Scene *getScene \
      () const \
      throw(); \
\
    Scene *getTransmitterScene \
      () const \
      throw(); \
\
    bool onNotify \
      (_MouseLeftButtonReleasedEvent & \
       __r_MouseLeftButtonReleasedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseLeftButtonReleasedShiftKeyPressedEvent & \
       __r_MouseLeftButtonReleasedShiftKeyPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseLeftButtonReleasedControlKeyPressedEvent & \
       __r_MouseLeftButtonReleasedControlKeyPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseLeftButtonReleasedShiftKeyPressedControlKeyPressedEvent & \
       __r_MouseLeftButtonReleasedShiftKeyPressedControlKeyPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseMiddleButtonReleasedEvent & \
       __r_MouseMiddleButtonReleasedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseMiddleButtonReleasedShiftKeyPressedEvent & \
       __r_MouseMiddleButtonReleasedShiftKeyPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseMiddleButtonReleasedControlKeyPressedEvent & \
       __r_MouseMiddleButtonReleasedControlKeyPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressedEvent & \
       __r_MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseRightButtonReleasedEvent & \
       __r_MouseRightButtonReleasedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseRightButtonReleasedShiftKeyPressedEvent & \
       __r_MouseRightButtonReleasedShiftKeyPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseRightButtonReleasedControlKeyPressedEvent & \
       __r_MouseRightButtonReleasedControlKeyPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_MouseRightButtonReleasedShiftKeyPressedControlKeyPressedEvent & \
       __r_MouseRightButtonReleasedShiftKeyPressedControlKeyPressedEvent) \
      throw(); \
\
    bool onNotify \
      (_##name##Event & \
       __r_##name##Event) \
      throw(); \
\
    void register##name## \
      (void (Scene::*__fp)(Scene *__pScene)) \
      throw(); \
\
 private: \
\
    bool _onNotify \
      (Scene *__pScene) \
      throw(); \
\
    void (Scene::*__mfp)(Scene *__pScene); \
\
    Scene *__mpScene_; \
    Scene *transmitter__mpScene_; \
} name



#define BALL_VIEW_IMPLEMENT_INLINE_RELEASED_EVENT_CLASS(name) \
BALL_INLINE \
Scene * \
Events::_##name##Event::getScene \
  () const \
  throw() \
{ \
  return __mpScene_; \
} \
\
BALL_INLINE \
Scene * \
Events::_##name##Event::getTransmitterScene \
  () const \
  throw() \
{ \
  return transmitter__mpScene_; \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonReleasedEvent & \
   __r_MouseLeftButtonReleasedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseLeftButtonReleasedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonReleasedShiftKeyPressedEvent & \
   __r_MouseLeftButtonReleasedShiftKeyPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseLeftButtonReleasedShiftKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonReleasedControlKeyPressedEvent & \
   __r_MouseLeftButtonReleasedControlKeyPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseLeftButtonReleasedControlKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonReleasedShiftKeyPressedControlKeyPressedEvent & \
   __r_MouseLeftButtonReleasedShiftKeyPressedControlKeyPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseLeftButtonReleasedShiftKeyPressedControlKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonReleasedEvent & \
   __r_MouseMiddleButtonReleasedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseMiddleButtonReleasedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonReleasedShiftKeyPressedEvent & \
   __r_MouseMiddleButtonReleasedShiftKeyPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseMiddleButtonReleasedShiftKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonReleasedControlKeyPressedEvent & \
   __r_MouseMiddleButtonReleasedControlKeyPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseMiddleButtonReleasedControlKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressedEvent & \
   __r_MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressedEvent. \
		   getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonReleasedEvent & \
   __r_MouseRightButtonReleasedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseRightButtonReleasedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonReleasedShiftKeyPressedEvent & \
   __r_MouseRightButtonReleasedShiftKeyPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseRightButtonReleasedShiftKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonReleasedControlKeyPressedEvent & \
   __r_MouseRightButtonReleasedControlKeyPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseRightButtonReleasedControlKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonReleasedShiftKeyPressedControlKeyPressedEvent & \
   __r_MouseRightButtonReleasedShiftKeyPressedControlKeyPressedEvent) \
  throw() \
{ \
  return _onNotify(__r_MouseRightButtonReleasedShiftKeyPressedControlKeyPressedEvent. \
		   getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_##name##Event & \
   __r_##name##Event) \
  throw() \
{ \
  if (__r_##name##Event.getTransmitterScene()  \
      == transmitter__mpScene_) \
  { \
    return true; \
  } \
 \
  return _onNotify(__r_##name##Event.getTransmitterScene()); \
} \
\
BALL_INLINE \
void \
Events::_##name##Event::register##name## \
  (void (Scene::*__fp)(Scene *__pScene)) \
  throw() \
{ \
  __mfp = __fp; \
}
    


#define BALL_VIEW_IMPLEMENT_NOTIFY_FUNCTION_FOR_EVENT_CLASS(name) \
bool \
Events::_##name##Event::_onNotify \
  (Scene *__pScene) \
  throw() \
{ \
  transmitter__mpScene_ = __pScene; \
\
  if (__mfp != 0) \
    (__mpScene_->*__mfp)(__pScene); \
\
  Notify(*this); \
\
  transmitter__mpScene_ = __mpScene_; \
\
  return true;\
}



namespace BALL
{

	namespace VIEW
	{

		/* forward */
		class Scene;


		/*
    !!! do not compile this class !!!
		!!! class will be included in scene !!!
		*/


		/**	The Events class.
				{\bf Framework:} BALL/VIEW/GUI/WIDGETS\\
				{\bf Definition:} \URL{BALL/VIEW/GUI/WIDGETS/events.h}\\ \\
				The class Events declares some event classes, the appropriate receiver classes
				and event combination operators to handle, process and combine events.
				Macros are used to declare these classes.  The \Ref{Notification} concept of
				BALL is used for the receivers classes. The class \Ref{Scene} binds its own
				methods to the receivers classes of {\em *this} events. Further the \Ref{Scene}
				object uses the combination operators to combine different events together and
				sent them to other receiving event classes prio registered with the 
				\Ref{Notification} mechanism of BALL.
				The names of the methods may seems a bit too long and unreadable but they are only
				event names and will used and created automatically for the \Ref{Scene} object.
				@memo    Events class (BALL VIEW gui widgets framework)
				@author  $Author: sturm $
				@version $Revision: 1.5 $
				@date    $Date: 2001/06/29 06:16:07 $
		*/
		class Events
		{
			public:

			/**	@name Exceptions
			*/
			//@{
			/** EventCombinationNotAllowed exception class.
					This exception is thrown if events of different \Ref{Scene} objects are
					combined.
					@see GeneralException
					@see Scene
			*/
 			class EventCombinationNotAllowed: public Exception::GeneralException
			{
  			public:
	   			EventCombinationNotAllowed(const char* file, int line)
						throw();
			};
			//@}

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new events.
					Initialize all own event classes.
					@param      scene the pointer to a \Ref{Scene} object to bind onto {\em *this} events
					@return     Events new constructed events
					@see        Scene
			*/
			Events(Scene* scene)
				throw();
		
			//@}

			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} events.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~Events()
				throw();

			/** Explicit default initialization.
					Empty for further purpose.
			*/
			virtual void clear()
				throw();
		
			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();
			//@}
		

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Inspection of the scene.
					Access a pointer to the \Ref{Scene} object of {\em *this} events.
					@return Scene* a pointer to the \Ref{Scene} object
 			*/
			Scene *getScene();
			//@}


			/** @name Event classes declaration
			*/
			//@{
			/** Declare event class ShiftKeyPressed.
					Declare the event class {\em ShiftKeyPressed}. This event will be sent by
					the \Ref{Scene} object whenever the shift key on the keyboard is pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(ShiftKeyPressed); 

			/** Declare event class ShiftKeyPressedMouseMoved.
					This event will be
					sent by the \Ref{Scene} object whenever the shift key on the keyboard is pressed
					and at the same moment the mouse is moved.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(ShiftKeyPressedMouseMoved);
			
			/** Declare event class ControlKeyPressed.
					This event will be sent by
					the \Ref{Scene} object whenever the control key on the keyboard is pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(ControlKeyPressed);
			
			/** Declare event class ControlKeyPressedMouseMoved.
					This event will be
					sent by the \Ref{Scene} object whenever the control key on the keyboard is pressed
					and at the same moment the mouse is moved.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(ControlKeyPressedMouseMoved);
			
			/** Declare event class ShiftKeyPressedControlKeyPressed.
					This event will be
					sent by the \Ref{Scene} object whenever the shift key on the keyboard is pressed
					and at the same moment the control key is pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(ShiftKeyPressedControlKeyPressed);
			
			/** Declare event class ShiftKeyPressedControlKeyPressedMouseMoved.
					This event will be sent by the \Ref{Scene} object whenever the shift key and
					the control key on the keyboard are pressed and at the same moment the 
					mouse is  moved.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(ShiftKeyPressedControlKeyPressedMouseMoved);
			
			/** Declare event class MouseLeftButtonPressed.
					This event will be sent by
					the \Ref{Scene} object whenever the left button of the mouse is pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonPressed);
			
			/** Declare event class MouseLeftButtonPressedShiftKeyPressed.
					This event will be sent by the \Ref{Scene} object whenever 
					the left button of the mouse is pressed
					and at the same moment the 
					shift key on the keyboard is pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonPressedShiftKeyPressed);
			
			/** Declare event class MouseLeftButtonPressedControlKeyPressed.
					This event will be sent by the \Ref{Scene} object whenever 
					the left button of the mouse is pressed
					and at the same moment the 
					control key on the keyboard is pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonPressedControlKeyPressed);
			
			/** Declare event class MouseLeftButtonPressedShiftKeyPressedControlKeyPressed.
					This event will be sent by the \Ref{Scene} object whenever 
					the left button of the mouse is pressed
					and at the same moment the 
					shift key and the control key on the keyboard are pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonPressedShiftKeyPressedControlKeyPressed);
			
			/** Declare event class MouseLeftButtonPressedMouseMoved.
					This event will be sent by the \Ref{Scene} object whenever 
					the left button of the mouse is pressed
					and at the same moment the 
					the mouse is moved.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonPressedMouseMoved);
			
			/** Declare event class MouseLeftButtonPressedShiftKeyPressedMouseMoved.
					This event will be sent by the \Ref{Scene} object whenever 
					the left button of the mouse is pressed
					and at the same moment the 
					the shift key on the keyboard is pressed and the mouse is moved.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonPressedShiftKeyPressedMouseMoved);
			
			/** Declare event class MouseLeftButtonPressedControlKeyPressedMouseMoved.
					This event will be sent by the \Ref{Scene} object whenever 
					the left button of the mouse is pressed
					and at the same moment the 
					the control key on the keyboard is pressed and the mouse is moved.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonPressedControlKeyPressedMouseMoved);
			
			/** Declare event class MouseLeftButtonPressedControlKeyPressedMouseMoved.
					This event will be sent by the \Ref{Scene} object whenever 
					the left button of the mouse is pressed
					and at the same moment the 
					the shift key and the control key on the keyboard are pressed and the mouse is moved.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonPressedShiftKeyPressedControlKeyPressedMouseMoved);
			
			/** Declare event class MouseLeftButtonReleased.
					This event will be sent by
					the \Ref{Scene} object whenever 
					the left button of the mouse is released.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonReleased);
			
			/** Declare event class MouseLeftButtonReleasedShiftKeyPressed.
					This event will be sent by the \Ref{Scene} object whenever 
					the left button of the mouse is released
					and at the same moment the 
					shift key on the keyboard is pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonReleasedShiftKeyPressed);
			
			/** Declare event class MouseLeftButtonReleasedControlKeyPressed.
					This event will be sent by the \Ref{Scene} object whenever 
					the left button of the mouse is released
					and at the same moment the 
					control key on the keyboard is pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonReleasedControlKeyPressed);
			
			/** Declare event class MouseLeftButtonReleasedShiftKeyPressedControlKeyPressed.
					This event will be sent by the \Ref{Scene} object whenever 
					the left button of the mouse is released
					and at the same moment the 
					shift key and the control key on the keyboard are pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonReleasedShiftKeyPressedControlKeyPressed);
			
			/** Declare event class MouseMiddleButtonPressed.
					This event will be sent by the \Ref{Scene} object whenever 
					the middle button of the mouse is pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonPressed);
			
			/** Declare event class MouseMiddleButtonPressedShiftKeyPressed.
					This event will be sent by the \Ref{Scene} object whenever 
					the middle button of the mouse is pressed
					and at the same moment the 
					shift key on the keyboard is pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonPressedShiftKeyPressed);
			
			/** Declare event class MouseMiddleButtonPressedControlKeyPressed.
					This event will be sent by the \Ref{Scene} object whenever 
					the middle button of the mouse is pressed
					and at the same moment the 
					control key on the keyboard is pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonPressedControlKeyPressed);
			
			/** Declare event class MouseMiddleButtonPressedShiftKeyPressedControlKeyPressed.
					This event will be sent by the \Ref{Scene} object whenever 
					the middle button of the mouse is pressed
					and at the same moment the 
					shift key and the control key on the keyboard are pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonPressedShiftKeyPressedControlKeyPressed);
			
			/** Declare event class MouseMiddleButtonPressedMouseMoved.
					This event will be sent by the \Ref{Scene} object whenever 
					the middle button of the mouse is pressed
					and at the same moment the 
					mouse is moved.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonPressedMouseMoved);
			
			/** Declare event class MouseMiddleButtonPressedShiftKeyPressedMouseMoved.
					This event will be sent by the \Ref{Scene} object whenever 
					the middle button of the mouse is pressed
					and at the same moment the 
					shift key on the keyboard is pressed and the
					mouse is moved.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonPressedShiftKeyPressedMouseMoved);
			
			/** Declare event class MouseMiddleButtonPressedControlKeyPressedMouseMoved.
					This event will be sent by the \Ref{Scene} object whenever 
					the middle button of the mouse is pressed
					and at the same moment the 
					control key on the keyboard is pressed and the
					mouse is moved.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonPressedControlKeyPressedMouseMoved);
			
			/** Declare event class MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMoved.
					This event will be sent by the \Ref{Scene} object whenever 
					the middle button of the mouse is pressed
					and at the same moment the 
					shift key and the control key on the keyboard are pressed and the
					mouse is moved.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMoved);
			
			/** Declare event class MouseMiddleButtonReleased.
					This event will be sent by the \Ref{Scene} object whenever 
					the middle button of the mouse is released.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonReleased);
			
			/** Declare event class MouseMiddleButtonReleasedShiftKeyPressed.
					This event will be sent by the \Ref{Scene} object whenever 
					the middle button of the mouse is released
					and at the same moment the 
					shift key on the keyboard is pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonReleasedShiftKeyPressed);
			
			/** Declare event class MouseMiddleButtonReleasedControlKeyPressed.
					This event will be sent by the \Ref{Scene} object whenever 
					the middle button of the mouse is released
					and at the same moment the 
					control key on the keyboard is pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonReleasedControlKeyPressed);
			
			/** Declare event class MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressed.
					This event will be sent by the \Ref{Scene} object whenever 
					the middle button of the mouse is released
					and at the same moment the 
					shift key and the control key on the keyboard are pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressed);
			
			/** Declare event class MouseRightButtonPressed.
					This event will be sent by the \Ref{Scene} object whenever 
					the right button of the mouse is pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonPressed);
			
			/** Declare event class MouseRightButtonPressedShiftKeyPressed.
					This event will be sent by the \Ref{Scene} object whenever 
					the right button of the mouse is pressed
					and at the same moment the 
					shift key on the keyboard is pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonPressedShiftKeyPressed);
			
			/** Declare event class MouseRightButtonPressedControlKeyPressed.
					This event will be sent by the \Ref{Scene} object whenever 
					the right button of the mouse is pressed
					and at the same moment the 
					control key on the keyboard is pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonPressedControlKeyPressed);
			
			/** Declare event class MouseRightButtonPressedShiftKeyPressedControlKeyPressed.
					This event will be sent by the \Ref{Scene} object whenever 
					the right button of the mouse is pressed
					and at the same moment the 
					shift key and the control key on the keyboard are pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonPressedShiftKeyPressedControlKeyPressed);
			
			/** Declare event class MouseRightButtonPressedMouseMoved.
					This event will be sent by the \Ref{Scene} object whenever 
					the right button of the mouse is pressed
					and at the same moment the 
					mouse is moved.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonPressedMouseMoved);
			
			/** Declare event class MouseRightButtonPressedShiftKeyPressedMouseMoved.
					This event will be sent by the \Ref{Scene} object whenever 
					the right button of the mouse is pressed
					and at the same moment the 
					shift key on the keyboard is pressed and the
					mouse is moved.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonPressedShiftKeyPressedMouseMoved);
			
			/** Declare event class MouseRightButtonPressedControlKeyPressedMouseMoved.
					This event will be sent by the \Ref{Scene} object whenever 
					the right button of the mouse is pressed
					and at the same moment the 
					control key on the keyboard is pressed and the
					mouse is moved.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonPressedControlKeyPressedMouseMoved);
			
			/** Declare event class MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMoved.
					This event will be sent by the \Ref{Scene} object whenever 
					the right button of the mouse is pressed
					and at the same moment the 
					shift key and the control key on the keyboard are pressed and the
					mouse is moved.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMoved);
			
			/** Declare event class MouseRightButtonReleased.
					This event will be sent by the \Ref{Scene} object whenever 
					the right button of the mouse is released.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonReleased);
			
			/** Declare event class MouseRightButtonReleasedShiftKeyPressed.
					This event will be sent by the \Ref{Scene} object whenever 
					the right button of the mouse is released
					and at the same moment the 
					shift key on the keyboard is pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonReleasedShiftKeyPressed);
			
			/** Declare event class MouseRightButtonReleasedControlKeyPressed.
					This event will be sent by the \Ref{Scene} object whenever 
					the right button of the mouse is released
					and at the same moment the 
					control key on the keyboard is pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonReleasedControlKeyPressed);
			
			/** Declare event class MouseRightButtonReleasedShiftKeyPressedControlKeyPressed.
					This event will be sent by the \Ref{Scene} object whenever 
					the right button of the mouse is released
					and at the same moment the 
					shift key and the control key on the keyboard are pressed.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonReleasedShiftKeyPressedControlKeyPressed);
			
			/** Declare event class MouseMoved.
					This event will be sent by the \Ref{Scene} object whenever 
					the mouse is moved.
			*/
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMoved);
			//@}

			/** @name Event receiver classes declaration
			*/
			//@{
			/** Declare event receiver class RotateSystem.
					This event receiver class catches all pressed and moved events and relates
					them to the method registered with {\em registerRotateSystem} available in this
					class. The method registered with {\em registerRotateSystem} must have as
					parameter a pointer to the \Ref{Scene} object. So you have access in this
					method to the	\Ref{Scene} object that has sent the event.
					The method {\em registerRotateSystem} takes as parameter as pointer to
					a method available in the \Ref{Scene} object.
			*/
			BALL_VIEW_DECLARE_PRESSED_MOVED_EVENT_CLASS(RotateSystem);

			/** Declare event receiver class ZoomSystem.
					This event receiver class catches all pressed and moved events and relates
					them to the method registered with {\em registerZoomSystem} available in this
					class. The method registered with {\em registerZoomSystem} must have as
					parameter a pointer to the \Ref{Scene} object. So you have access in this
					method to the	\Ref{Scene} object that has sent the event.
					The method {\em registerZoomSystem} takes as parameter as pointer to
					a method available in the \Ref{Scene} object.
			*/
			BALL_VIEW_DECLARE_PRESSED_MOVED_EVENT_CLASS(ZoomSystem);

			/** Declare event receiver class TranslateSystem.
					This event receiver class catches all pressed and moved events and relates
					them to the method registered with {\em registerTranslateSystem} available in this
					class. The method registered with {\em registerTranslateSystem} must have as
					parameter a pointer to the \Ref{Scene} object. So you have access in this
					method to the	\Ref{Scene} object that has sent the event.
					The method {\em registerTranslateSystem} takes as parameter as pointer to
					a method available in the \Ref{Scene} object.
			*/
			BALL_VIEW_DECLARE_PRESSED_MOVED_EVENT_CLASS(TranslateSystem);

			/** Declare event receiver class SelectionPressedMoved.
					This event receiver class catches all pressed and moved events and relates
					them to the method registered with {\em registerSelectionPressedMoved} available in this
					class. The method registered with {\em registerSelectionPressedMoved} must have as
					parameter a pointer to the \Ref{Scene} object. So you have access in this
					method to the	\Ref{Scene} object that has sent the event.
					The method {\em registerSelectionPressedMoved} takes as parameter as pointer to
					a method available in the \Ref{Scene} object.
			*/
			BALL_VIEW_DECLARE_PRESSED_MOVED_EVENT_CLASS(SelectionPressedMoved);

			/** Declare event receiver class SelectionPressed.
					This event receiver class catches all pressed events and relates
					them to the method registered with {\em registerSelectionPressed} available in this
					class. The method registered with {\em registerSelectionPressed} must have as
					parameter a pointer to the \Ref{Scene} object. So you have access in this
					method to the	\Ref{Scene} object that has sent the event.
					The method {\em registerSelectionPressed} takes as parameter as pointer to
					a method available in the \Ref{Scene} object.
			*/
			BALL_VIEW_DECLARE_PRESSED_EVENT_CLASS(SelectionPressed);

			/** Declare event receiver class SelectionReleased.
					This event receiver class catches all released events and relates
					them to the method registered with {\em registerSelectionReleased} available in this
					class. The method registered with {\em registerSelectionReleased} must have as
					parameter a pointer to the \Ref{Scene} object. So you have access in this
					method to the	\Ref{Scene} object that has sent the event.
					The method {\em registerSelectionReleased} takes as parameter as pointer to
					a method available in the \Ref{Scene} object.
			*/
			BALL_VIEW_DECLARE_RELEASED_EVENT_CLASS(SelectionReleased);

			/** Declare event receiver class DeselectionPressedMoved.
					This event receiver class catches all pressed and moved events and relates
					them to the method registered with {\em registerDeselectionPressedMoved} available in this
					class. The method registered with {\em registerDeselectionPressedMoved} must have as
					parameter a pointer to the \Ref{Scene} object. So you have access in this
					method to the	\Ref{Scene} object that has sent the event.
					The method {\em registerDeselectionPressedMoved} takes as parameter as pointer to
					a method available in the \Ref{Scene} object.
			*/
			BALL_VIEW_DECLARE_PRESSED_MOVED_EVENT_CLASS(DeselectionPressedMoved);

			/** Declare event receiver class DeselectionPressed.
					This event receiver class catches all pressed events and relates
					them to the method registered with {\em registerDeselectionPressed} available in this
					class. The method registered with {\em registerDeselectionPressed} must have as
					parameter a pointer to the \Ref{Scene} object. So you have access in this
					method to the	\Ref{Scene} object that has sent the event.
					The method {\em registerDeselectionPressed} takes as parameter as pointer to
					a method available in the \Ref{Scene} object.
			*/
			BALL_VIEW_DECLARE_PRESSED_EVENT_CLASS(DeselectionPressed);

			/** Declare event receiver class DeselectionReleased.
					This event receiver class catches all released events and relates
					them to the method registered with {\em registerDeselectionReleased} available in this
					class. The method registered with {\em registerDeselectionReleased} must have as
					parameter a pointer to the \Ref{Scene} object. So you have access in this
					method to the	\Ref{Scene} object that has sent the event.
					The method {\em registerDeselectionReleased} takes as parameter as pointer to
					a method available in the \Ref{Scene} object.
			*/
			BALL_VIEW_DECLARE_RELEASED_EVENT_CLASS(DeselectionReleased);
			//@}
			
			/** @name Event combination operators
			*/
			//@{

			/** Declare combine operator for events ShiftKeyPressed and ControlKeyPressed.
					Declare operator & for the two events 
					{\em ShiftKeyPressed} and {\em ControlKeyPressed}.
					The return type of this operator is
					{\em ShiftKeyPressedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(ShiftKeyPressed,
				 ControlKeyPressed);
			
			/** Declare combine operator for events ShiftKeyPressed and MouseMoved.
					Declare operator & for the two events 
					{\em ShiftKeyPressed} and {\em MouseMoved}.
					The return type of this operator is
					{\em ShiftKeyPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(ShiftKeyPressed,
				 MouseMoved);
			
			/** Declare combine operator for events ControlKeyPressed and MouseMoved.
					Declare operator & for the two events 
					{\em ControlKeyPressed} and {\em MouseMoved}.
					The return type of this operator is
					{\em ControlKeyPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(ControlKeyPressed,
				 MouseMoved);
			
			/** Declare combine operator for events ShiftKeyPressed and ControlKeyPressedMouseMoved.
					Declare operator & for the two events 
					{\em ShiftKeyPressed} and {\em ControlKeyPressedMouseMoved}.
					The return type of this operator is
					{\em ShiftKeyPressedControlKeyPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(ShiftKeyPressed,
				 ControlKeyPressedMouseMoved);
			
			/** Declare combine operator for events ControlKeyPressed and ShiftKeyPressedMouseMoved.
					Declare operator & for the two events 
					{\em ControlKeyPressed} and {\em ShiftKeyPressedMouseMoved}.
					This operator changes the order in the return type.
					The return type of this operator is
					{\em ShiftKeyPressedControlKeyPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE
				(ControlKeyPressed,
				 ShiftKeyPressedMouseMoved,
				 ShiftKeyPressedControlKeyPressedMouseMoved);
			
			/** Declare combine operator for events ShiftKeyPressedControlKeyPressed and MouseMoved.
					Declare operator & for the two events 
					{\em ShiftKeyPressedControlKeyPressed} and {\em MouseMoved}.
					The return type of this operator is
					{\em ShiftKeyPressedControlKeyPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(ShiftKeyPressedControlKeyPressed,
				 MouseMoved);
			
			/** Declare combine operator for events MouseLeftButtonPressed and ShiftKeyPressed.
					Declare operator & for the two events 
					{\em MouseLeftButtonPressed} and {\em ShiftKeyPressed}.
					The return type of this operator is
					{\em MouseLeftButtonPressedShiftKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonPressed,
				 ShiftKeyPressed);

			/** Declare combine operator for events MouseLeftButtonPressed and ControlKeyPressed.
					Declare operator & for the two events 
					{\em MouseLeftButtonPressed} and {\em ControlKeyPressed}.
					The return type of this operator is
					{\em MouseLeftButtonPressedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonPressed,
				 ControlKeyPressed);
			
			/** Declare combine operator for events MouseLeftButtonPressed and ShiftKeyPressedControlKeyPressed.
					Declare operator & for the two events 
					{\em MouseLeftButtonPressed} and {\em ShiftKeyPressedControlKeyPressed}.
					The return type of this operator is
					{\em MouseLeftButtonPressedShiftKeyPressedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonPressed,
				 ShiftKeyPressedControlKeyPressed);
			
			/** Declare combine operator for events MouseLeftButtonPressedShiftKeyPressed and ControlKeyPressed.
					Declare operator & for the two events 
					{\em MouseLeftButtonPressedShiftKeyPressed} and {\em ControlKeyPressed}.
					The return type of this operator is
					{\em MouseLeftButtonPressedShiftKeyPressedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonPressedShiftKeyPressed,
				 ControlKeyPressed);
			
			/** Declare combine operator for events MouseLeftButtonPressedControlKeyPressed and ShiftKeyPressed.
					Declare operator & for the two events 
					{\em MouseLeftButtonPressedControlKeyPressed} and {\em ShiftKeyPressed}.
					This operator changes the order in the return type.
					The return type of this operator is
					{\em MouseLeftButtonPressedShiftKeyPressedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE
				(MouseLeftButtonPressedControlKeyPressed,
				 ShiftKeyPressed,
				 MouseLeftButtonPressedShiftKeyPressedControlKeyPressed);
			
			/** Declare combine operator for events MouseLeftButtonPressed and MouseMoved.
					Declare operator & for the two events 
					{\em MouseLeftButtonPressed} and {\em MouseMoved}.
					The return type of this operator is
					{\em MouseLeftButtonPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonPressed,
				 MouseMoved);
			
			/** Declare combine operator for events MouseLeftButtonPressedShiftKeyPressed and MouseMoved.
					Declare operator & for the two events 
					{\em MouseLeftButtonPressedShiftKeyPressed} and {\em MouseMoved}.
					The return type of this operator is
					{\em MouseLeftButtonPressedShiftKeyPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonPressedShiftKeyPressed,
				 MouseMoved);
			
			/** Declare combine operator for events MouseLeftButtonPressedControlKeyPressed and MouseMoved.
					Declare operator & for the two events 
					{\em MouseLeftButtonPressedControlKeyPressed} and {\em MouseMoved}.
					The return type of this operator is
					{\em MouseLeftButtonPressedControlKeyPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonPressedControlKeyPressed,
				 MouseMoved);
			
			/** Declare combine operator for events MouseLeftButtonPressedShiftKeyPressed and ControlKeyPressedMouseMoved.
					Declare operator & for the two events 
					{\em MouseLeftButtonPressedShiftKeyPressed} and {\em ControlKeyPressedMouseMoved}.
					The return type of this operator is
					{\em MouseLeftButtonPressedShiftKeyPressedControlKeyPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonPressedShiftKeyPressed,
				 ControlKeyPressedMouseMoved);
			
			/** Declare combine operator for events MouseLeftButtonPressedControlKeyPressed and ShiftKeyPressedMouseMoved.
					Declare operator & for the two events 
					{\em MouseLeftButtonPressedControlKeyPressed} and {\em ShiftKeyPressedMouseMoved}.
					This operator changes the order in the return type.
					The return type of this operator is
					{\em MouseLeftButtonPressedShiftKeyPressedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE
				(MouseLeftButtonPressedControlKeyPressed,
				 ShiftKeyPressedMouseMoved,
				 MouseLeftButtonPressedShiftKeyPressedControlKeyPressed);
			
			/** Declare combine operator for events MouseLeftButtonPressedShiftKeyPressedControlKeyPressed and MouseMoved.
					Declare operator & for the two events 
					{\em MouseLeftButtonPressedShiftKeyPressedControlKeyPressed} and {\em MouseMoved}.
					The return type of this operator is
					{\em MouseLeftButtonPressedShiftKeyPressedControlKeyPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonPressedShiftKeyPressedControlKeyPressed,
				 MouseMoved);
			
			/** Declare combine operator for events MouseLeftButtonReleased and ShiftKeyPressed.
					Declare operator & for the two events 
					{\em MouseLeftButtonReleased} and {\em ShiftKeyPressed}.
					The return type of this operator is
					{\em MouseLeftButtonReleasedShiftKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonReleased,
				 ShiftKeyPressed);
			
			/** Declare combine operator for events MouseLeftButtonReleased and ControlKeyPressed.
					Declare operator & for the two events 
					{\em MouseLeftButtonReleased} and {\em ControlKeyPressed}.
					The return type of this operator is
					{\em MouseLeftButtonReleasedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonReleased,
				 ControlKeyPressed);
			
			/** Declare combine operator for events MouseLeftButtonReleasedShiftKeyPressed and ControlKeyPressed.
					Declare operator & for the two events 
					{\em MouseLeftButtonReleasedShiftKeyPressed} and {\em ControlKeyPressed}.
					The return type of this operator is
					{\em MouseLeftButtonReleasedShiftKeyPressedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonReleasedShiftKeyPressed,
				 ControlKeyPressed);
			
			/** Declare combine operator for events MouseLeftButtonReleasedControlKeyPressed and ShiftKeyPressed.
					Declare operator & for the two events 
					{\em MouseLeftButtonReleasedControlKeyPressed} and {\em ShiftKeyPressed}.
					This operator changes the order in the return type.
					The return type of this operator is
					{\em MouseLeftButtonReleasedShiftKeyPressedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE
				(MouseLeftButtonReleasedControlKeyPressed,
				 ShiftKeyPressed,
				 MouseLeftButtonReleasedShiftKeyPressedControlKeyPressed);
			
			/** Declare combine operator for events MouseMiddleButtonPressed and ShiftKeyPressed.
					Declare operator & for the two events 
					{\em MouseMiddleButtonPressed} and {\em ShiftKeyPressed}.
					The return type of this operator is
					{\em MouseMiddleButtonPressedShiftKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonPressed,
				 ShiftKeyPressed);
			
			/** Declare combine operator for events MouseMiddleButtonPressed and ControlKeyPressed.
					Declare operator & for the two events 
					{\em MouseMiddleButtonPressed} and {\em ControlKeyPressed}.
					The return type of this operator is
					{\em MouseMiddleButtonPressedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonPressed,
				 ControlKeyPressed);
			
			/** Declare combine operator for events MouseMiddleButtonPressed and ShiftKeyPressedControlKeyPressed.
					Declare operator & for the two events 
					{\em MouseMiddleButtonPressed} and {\em ShiftKeyPressedControlKeyPressed}.
					The return type of this operator is
					{\em MouseMiddleButtonPressedShiftKeyPressedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonPressed,
				 ShiftKeyPressedControlKeyPressed);
			
			/** Declare combine operator for events MouseMiddleButtonPressedShiftKeyPressed and ControlKeyPressed.
					Declare operator & for the two events 
					{\em MouseMiddleButtonPressedShiftKeyPressed} and {\em ControlKeyPressed}.
					The return type of this operator is
					{\em MouseMiddleButtonPressedShiftKeyPressedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonPressedShiftKeyPressed,
				 ControlKeyPressed);
			
			/** Declare combine operator for events MouseMiddleButtonPressedControlKeyPressed and ShiftKeyPressed.
					Declare operator & for the two events 
					{\em MouseMiddleButtonPressedControlKeyPressed} and {\em ShiftKeyPressed}.
					This operator changes the order in the return type.
					The return type of this operator is
					{\em MouseMiddleButtonPressedShiftKeyPressedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE
				(MouseMiddleButtonPressedControlKeyPressed,
				 ShiftKeyPressed,
				 MouseMiddleButtonPressedShiftKeyPressedControlKeyPressed);
			
			/** Declare combine operator for events MouseMiddleButtonPressed and MouseMoved.
					Declare operator & for the two events 
					{\em MouseMiddleButtonPressed} and {\em MouseMoved}.
					The return type of this operator is
					{\em MouseMiddleButtonPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonPressed,
				 MouseMoved);
			
			/** Declare combine operator for events MouseMiddleButtonPressedShiftKeyPressed and MouseMoved.
					Declare operator & for the two events 
					{\em MouseMiddleButtonPressedShiftKeyPressed} and {\em MouseMoved}.
					The return type of this operator is
					{\em MouseMiddleButtonPressedShiftKeyPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonPressedShiftKeyPressed,
				 MouseMoved);
			
			/** Declare combine operator for events MouseMiddleButtonPressedControlKeyPressed and MouseMoved.
					Declare operator & for the two events 
					{\em MouseMiddleButtonPressedControlKeyPressed} and {\em MouseMoved}.
					The return type of this operator is
					{\em MouseMiddleButtonPressedControlKeyPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonPressedControlKeyPressed,
				 MouseMoved);
			
			/** Declare combine operator for events MouseMiddleButtonPressedShiftKeyPressed and ControlKeyPressedMouseMoved.
					Declare operator & for the two events 
					{\em MouseMiddleButtonPressedShiftKeyPressed} and {\em ControlKeyPressedMouseMoved}.
					The return type of this operator is
					{\em MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonPressedShiftKeyPressed,
				 ControlKeyPressedMouseMoved);
			
			/** Declare combine operator for events MouseMiddleButtonPressedControlKeyPressed and ShiftKeyPressedMouseMoved.
					Declare operator & for the two events 
					{\em MouseMiddleButtonPressedControlKeyPressed} and {\em ShiftKeyPressedMouseMoved}.
					This operator changes the order in the return type.
					The return type of this operator is
					{\em MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE
				(MouseMiddleButtonPressedControlKeyPressed,
				 ShiftKeyPressedMouseMoved,
				 MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMoved);
			
			/** Declare combine operator for events MouseMiddleButtonPressedShiftKeyPressedControlKeyPressed and MouseMoved.
					Declare operator & for the two events 
					{\em MouseMiddleButtonPressedShiftKeyPressedControlKeyPressed} and {\em MouseMoved}.
					The return type of this operator is
					{\em MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonPressedShiftKeyPressedControlKeyPressed,
				 MouseMoved);
			
			/** Declare combine operator for events MouseMiddleButtonReleased and ShiftKeyPressed.
					Declare operator & for the two events 
					{\em MouseMiddleButtonReleased} and {\em ShiftKeyPressed}.
					The return type of this operator is
					{\em MouseMiddleButtonReleasedShiftKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonReleased,
				 ShiftKeyPressed);
			
			/** Declare combine operator for events MouseMiddleButtonReleased and ControlKeyPressed.
					Declare operator & for the two events 
					{\em MouseMiddleButtonReleased} and {\em ControlKeyPressed}.
					The return type of this operator is
					{\em MouseMiddleButtonReleasedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonReleased,
				 ControlKeyPressed);
			
			/** Declare combine operator for events MouseMiddleButtonReleasedShiftKeyPressed and ControlKeyPressed.
					Declare operator & for the two events 
					{\em MouseMiddleButtonReleasedShiftKeyPressed} and {\em ControlKeyPressed}.
					The return type of this operator is
					{\em MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonReleasedShiftKeyPressed,
				 ControlKeyPressed);
			
			/** Declare combine operator for events MouseMiddleButtonReleasedControlKeyPressed and ShiftKeyPressed.
					Declare operator & for the two events 
					{\em MouseMiddleButtonReleasedControlKeyPressed} and {\em ShiftKeyPressed}.
					This operator changes the order in the return type.
					The return type of this operator is
					{\em MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE
				(MouseMiddleButtonReleasedControlKeyPressed,
				 ShiftKeyPressed,
				 MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressed);
			
			/** Declare combine operator for events MouseRightButtonPressed and ShiftKeyPressed.
					Declare operator & for the two events 
					{\em MouseRightButtonPressed} and {\em ShiftKeyPressed}.
					The return type of this operator is
					{\em MouseRightButtonPressedShiftKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonPressed,
				 ShiftKeyPressed);
			
			/** Declare combine operator for events MouseRightButtonPressed and ControlKeyPressed.
					Declare operator & for the two events 
					{\em MouseRightButtonPressed} and {\em ControlKeyPressed}.
					The return type of this operator is
					{\em MouseRightButtonPressedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonPressed,
				 ControlKeyPressed);
			
			/** Declare combine operator for events MouseRightButtonPressed and ShiftKeyPressedControlKeyPressed.
					Declare operator & for the two events 
					{\em MouseRightButtonPressed} and {\em ShiftKeyPressedControlKeyPressed}.
					The return type of this operator is
					{\em MouseRightButtonPressedShiftKeyPressedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonPressed,
				 ShiftKeyPressedControlKeyPressed);
			
			/** Declare combine operator for events MouseRightButtonPressedShiftKeyPressed and ControlKeyPressed.
					Declare operator & for the two events 
					{\em MouseRightButtonPressedShiftKeyPressed} and {\em ControlKeyPressed}.
					The return type of this operator is
					{\em MouseRightButtonPressedShiftKeyPressedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonPressedShiftKeyPressed,
				 ControlKeyPressed);
			
			/** Declare combine operator for events MouseRightButtonPressedControlKeyPressed and ShiftKeyPressed.
					Declare operator & for the two events 
					{\em MouseRightButtonPressedControlKeyPressed} and {\em ShiftKeyPressed}.
					This operator changes the order in the return type.
					The return type of this operator is
					{\em MouseRightButtonPressedShiftKeyPressedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE
				(MouseRightButtonPressedControlKeyPressed,
				 ShiftKeyPressed,
				 MouseRightButtonPressedShiftKeyPressedControlKeyPressed);
			
			/** Declare combine operator for events MouseRightButtonPressed and MouseMoved.
					Declare operator & for the two events 
					{\em MouseRightButtonPressed} and {\em MouseMoved}.
					The return type of this operator is
					{\em MouseRightButtonPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonPressed,
				 MouseMoved);
			
			/** Declare combine operator for events MouseRightButtonPressedShiftKeyPressed and MouseMoved.
					Declare operator & for the two events 
					{\em MouseRightButtonPressedShiftKeyPressed} and {\em MouseMoved}.
					The return type of this operator is
					{\em MouseRightButtonPressedShiftKeyPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonPressedShiftKeyPressed,
				 MouseMoved);
			
			/** Declare combine operator for events MouseRightButtonPressedControlKeyPressed and MouseMoved.
					Declare operator & for the two events 
					{\em MouseRightButtonPressedControlKeyPressed} and {\em MouseMoved}.
					The return type of this operator is
					{\em MouseRightButtonPressedControlKeyPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonPressedControlKeyPressed,
				 MouseMoved);
			
			/** Declare combine operator for events MouseRightButtonPressedShiftKeyPressed and ControlKeyPressedMouseMoved.
					Declare operator & for the two events 
					{\em MouseRightButtonPressedShiftKeyPressed} and {\em ControlKeyPressedMouseMoved}.
					The return type of this operator is
					{\em MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonPressedShiftKeyPressed,
				 ControlKeyPressedMouseMoved);
			
			/** Declare combine operator for events MouseRightButtonPressedControlKeyPressed and ShiftKeyPressedMouseMoved.
					Declare operator & for the two events 
					{\em MouseRightButtonPressedControlKeyPressed} and {\em ShiftKeyPressedMouseMoved}.
					This operator changes the order in the return type.
					The return type of this operator is
					{\em MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE
				(MouseRightButtonPressedControlKeyPressed,
				 ShiftKeyPressedMouseMoved,
				 MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMoved);
			
			/** Declare combine operator for events MouseRightButtonPressedShiftKeyPressedControlKeyPressed and MouseMoved.
					Declare operator & for the two events 
					{\em MouseRightButtonPressedShiftKeyPressedControlKeyPressed} and {\em MouseMoved}.
					The return type of this operator is
					{\em MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMoved}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonPressedShiftKeyPressedControlKeyPressed,
				 MouseMoved);
			
			/** Declare combine operator for events MouseRightButtonReleased and ShiftKeyPressed.
					Declare operator & for the two events 
					{\em MouseRightButtonReleased} and {\em ShiftKeyPressed}.
					The return type of this operator is
					{\em MouseRightButtonReleasedShiftKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonReleased,
				 ShiftKeyPressed);
			
			/** Declare combine operator for events MouseRightButtonReleased and ControlKeyPressed.
					Declare operator & for the two events 
					{\em MouseRightButtonReleased} and {\em ControlKeyPressed}.
					The return type of this operator is
					{\em MouseRightButtonReleasedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonReleased,
				 ControlKeyPressed);
			
			/** Declare combine operator for events MouseRightButtonReleasedShiftKeyPressed and ControlKeyPressed.
					Declare operator & for the two events 
					{\em MouseRightButtonReleasedShiftKeyPressed} and {\em ControlKeyPressed}.
					The return type of this operator is
					{\em MouseRightButtonReleasedShiftKeyPressedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonReleasedShiftKeyPressed,
				 ControlKeyPressed);
			
			/** Declare combine operator for events MouseRightButtonReleasedControlKeyPressed and ShiftKeyPressed.
					Declare operator & for the two events 
					{\em MouseRightButtonReleasedControlKeyPressed} and {\em ShiftKeyPressed}.
					This operator changes the order in the return type.
					The return type of this operator is
					{\em MouseRightButtonReleasedShiftKeyPressedControlKeyPressed}.
			*/
			BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE
				(MouseRightButtonReleasedControlKeyPressed,
				 ShiftKeyPressed,
				 MouseRightButtonReleasedShiftKeyPressedControlKeyPressed);
			//@}

			
			private:

			Scene* scene_;
		};

#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/WIDGETS/events.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_WIDGETS_EVENTS_H

////////////////////////////////////////////////////////////////////////////////
//
//    !!! do not compile this class !!!
//
// !!! class will be included in scene !!!
//
////////////////////////////////////////////////////////////////////////////////



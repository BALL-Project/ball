// $Id: events.h,v 1.3 2000/09/23 14:15:06 hekl Exp $

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
   const _##event_b##Event &__r_##event_b##Event); \
\
friend _##event_a##event_b##Event &operator & \
  (const _##event_b##Event &__r_##event_b##Event, \
   const _##event_a##Event &__r_##event_a##Event);



#define BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE(event_a, event_b, returntype) \
friend _##returntype##Event &operator & \
  (const _##event_a##Event &__r_##event_a##Event, \
   const _##event_b##Event &__r_##event_b##Event); \
\
friend _##returntype##Event &operator & \
  (const _##event_b##Event &__r_##event_b##Event, \
   const _##event_a##Event &__r_##event_a##Event);



#define BALL_VIEW_IMPLEMENT_OPERATOR(event_a, event_b) \
Events::_##event_a##event_b##Event & \
operator & \
  (const Events::_##event_a##Event &__r_##event_a##Event, \
   const Events::_##event_b##Event &__r_##event_b##Event) \
{ \
  BALL_PRECONDITION \
    (__r_##event_a##Event.getScene() \
     == __r_##event_b##Event.getScene(), \
     Ball::callErrorHandler \
       (Events::getClass(), \
	NULL, \
	Events::ERROR__CANNOT_COMBINE_EVENTS_FROM_DIFFERENT_SCENES, \
	Events::error_messages_[Events::ERROR__CANNOT_COMBINE_EVENTS_FROM_DIFFERENT_SCENES]);); \
\
  return __r_##event_a##Event.getEvent()->##event_a##event_b; \
} \
\
Events::_##event_a##event_b##Event & \
operator & \
  (const Events::_##event_b##Event &__r_##event_b##Event, \
   const Events::_##event_a##Event &__r_##event_a##Event) \
{ \
  return operator &(__r_##event_a##Event, __r_##event_b##Event); \
}



#define BALL_VIEW_IMPLEMENT_OPERATOR_WITH_RETURN_TYPE(event_a, event_b, returntype) \
Events::_##returntype##Event & \
operator & \
  (const Events::_##event_a##Event &__r_##event_a##Event, \
   const Events::_##event_b##Event &__r_##event_b##Event) \
{ \
  BALL_PRECONDITION \
    (__r_##event_a##Event.getScene() \
     == __r_##event_b##Event.getScene(), \
     Ball::callErrorHandler \
       (Events::getClass(), \
	NULL, \
	Events::ERROR__CANNOT_COMBINE_EVENTS_FROM_DIFFERENT_SCENES, \
	Events::error_messages_[Events::ERROR__CANNOT_COMBINE_EVENTS_FROM_DIFFERENT_SCENES]);); \
\
  return __r_##event_a##Event.getEvent()->##returntype; \
} \
\
Events::_##returntype##Event & \
operator & \
  (const Events::_##event_b##Event &__r_##event_b##Event, \
   const Events::_##event_a##Event &__r_##event_a##Event) \
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
	: \
        __mpEvents_(0) \
    { \
    } \
\
    _##name##Event \
      (Events *__pEvents) \
	: \
        __mpEvents_(__pEvents) \
    { \
    } \
\
    ~_##name##Event \
      () \
    { \
    } \
\
    Scene *getScene \
      () const\
    { \
      return __mpEvents_->getScene(); \
    } \
\
    Events *getEvent \
      () const\
    { \
      return __mpEvents_; \
    } \
\
    Events *__mpEvents_; \
} name;




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
      : \
      __mfp(0), \
      __mpScene_(0), \
      transmitter__mpScene_(0) \
    { \
    } \
\
    _##name##Event \
      (Scene *__pScene) \
      : \
      __mfp(0), \
      __mpScene_(__pScene), \
      transmitter__mpScene_(__pScene) \
    { \
    } \
\
    virtual ~_##name##Event \
      () \
    { \
    } \
\
    Scene *getScene \
      () const; \
\
    Scene *getTransmitterScene \
      () const; \
\
    bool onNotify \
      (_MouseLeftButtonPressedMouseMovedEvent & \
       __r_MouseLeftButtonPressedMouseMovedEvent); \
\
    bool onNotify \
      (_MouseLeftButtonPressedShiftKeyPressedMouseMovedEvent & \
       __r_MouseLeftButtonPressedShiftKeyPressedMouseMovedEvent); \
\
    bool onNotify \
      (_MouseLeftButtonPressedControlKeyPressedMouseMovedEvent & \
       __r_MouseLeftButtonPressedControlKeyPressedMouseMovedEvent); \
\
    bool onNotify \
      (_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent & \
       __r_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent); \
\
    bool onNotify \
      (_MouseMiddleButtonPressedMouseMovedEvent & \
       __r_MouseMiddleButtonPressedMouseMovedEvent); \
\
    bool onNotify \
      (_MouseMiddleButtonPressedShiftKeyPressedMouseMovedEvent & \
       __r_MouseMiddleButtonPressedShiftKeyPressedMouseMovedEvent); \
\
    bool onNotify \
      (_MouseMiddleButtonPressedControlKeyPressedMouseMovedEvent & \
       __r_MouseMiddleButtonPressedControlKeyPressedMouseMovedEvent); \
\
    bool onNotify \
      (_MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent & \
       __r_MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent); \
\
    bool onNotify \
      (_MouseRightButtonPressedMouseMovedEvent & \
       __r_MouseRightButtonPressedMouseMovedEvent); \
\
    bool onNotify \
      (_MouseRightButtonPressedShiftKeyPressedMouseMovedEvent & \
       __r_MouseRightButtonPressedShiftKeyPressedMouseMovedEvent); \
\
    bool onNotify \
      (_MouseRightButtonPressedControlKeyPressedMouseMovedEvent & \
       __r_MouseRightButtonPressedControlKeyPressedMouseMovedEvent); \
\
    bool onNotify \
      (_MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent & \
       __r_MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent); \
\
    bool onNotify \
      (_##name##Event & \
       __r_##name##Event); \
\
    void register##name## \
      (void (Scene::*__fp)(Scene *__pScene)); \
\
 private: \
\
    bool _onNotify \
      (Scene *__pScene); \
\
    void (Scene::*__mfp)(Scene *__pScene); \
\
    Scene *__mpScene_; \
    Scene *transmitter__mpScene_; \
} name;



#define BALL_VIEW_IMPLEMENT_INLINE_PRESSED_MOVED_EVENT_CLASS(name) \
BALL_INLINE \
Scene * \
Events::_##name##Event::getScene \
  () const \
{ \
  return __mpScene_; \
} \
\
BALL_INLINE \
Scene * \
Events::_##name##Event::getTransmitterScene \
  () const \
{ \
  return transmitter__mpScene_; \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonPressedMouseMovedEvent & \
   __r_MouseLeftButtonPressedMouseMovedEvent) \
{ \
  return _onNotify(__r_MouseLeftButtonPressedMouseMovedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonPressedShiftKeyPressedMouseMovedEvent & \
   __r_MouseLeftButtonPressedShiftKeyPressedMouseMovedEvent) \
{ \
  return _onNotify(__r_MouseLeftButtonPressedShiftKeyPressedMouseMovedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonPressedControlKeyPressedMouseMovedEvent & \
   __r_MouseLeftButtonPressedControlKeyPressedMouseMovedEvent) \
{ \
  return _onNotify(__r_MouseLeftButtonPressedControlKeyPressedMouseMovedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent & \
   __r_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent) \
{ \
  return _onNotify(__r_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonPressedMouseMovedEvent & \
   __r_MouseMiddleButtonPressedMouseMovedEvent) \
{ \
  return _onNotify(__r_MouseMiddleButtonPressedMouseMovedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonPressedShiftKeyPressedMouseMovedEvent & \
   __r_MouseMiddleButtonPressedShiftKeyPressedMouseMovedEvent) \
{ \
  return _onNotify(__r_MouseMiddleButtonPressedShiftKeyPressedMouseMovedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonPressedControlKeyPressedMouseMovedEvent & \
   __r_MouseMiddleButtonPressedControlKeyPressedMouseMovedEvent) \
{ \
  return _onNotify(__r_MouseMiddleButtonPressedControlKeyPressedMouseMovedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent & \
   __r_MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent) \
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
{ \
  return _onNotify(__r_MouseRightButtonPressedMouseMovedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonPressedShiftKeyPressedMouseMovedEvent & \
   __r_MouseRightButtonPressedShiftKeyPressedMouseMovedEvent) \
{ \
  return _onNotify(__r_MouseRightButtonPressedShiftKeyPressedMouseMovedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonPressedControlKeyPressedMouseMovedEvent & \
   __r_MouseRightButtonPressedControlKeyPressedMouseMovedEvent) \
{ \
  return _onNotify(__r_MouseRightButtonPressedControlKeyPressedMouseMovedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent & \
   __r_MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMovedEvent) \
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
      : \
      __mfp(0), \
      __mpScene_(0), \
      transmitter__mpScene_(0) \
    { \
    } \
\
    _##name##Event \
      (Scene *__pScene) \
      : \
      __mfp(0), \
      __mpScene_(__pScene), \
      transmitter__mpScene_(__pScene) \
    { \
    } \
\
    virtual ~_##name##Event \
      () \
    { \
    } \
\
    Scene *getScene \
      () const; \
\
    Scene *getTransmitterScene \
      () const; \
\
    bool onNotify \
      (_MouseLeftButtonPressedEvent & \
       __r_MouseLeftButtonPressedEvent); \
\
    bool onNotify \
      (_MouseLeftButtonPressedShiftKeyPressedEvent & \
       __r_MouseLeftButtonPressedShiftKeyPressedEvent); \
\
    bool onNotify \
      (_MouseLeftButtonPressedControlKeyPressedEvent & \
       __r_MouseLeftButtonPressedControlKeyPressedEvent); \
\
    bool onNotify \
      (_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedEvent & \
       __r_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedEvent); \
\
    bool onNotify \
      (_MouseMiddleButtonPressedEvent & \
       __r_MouseMiddleButtonPressedEvent); \
\
    bool onNotify \
      (_MouseMiddleButtonPressedShiftKeyPressedEvent & \
       __r_MouseMiddleButtonPressedShiftKeyPressedEvent); \
\
    bool onNotify \
      (_MouseMiddleButtonPressedControlKeyPressedEvent & \
       __r_MouseMiddleButtonPressedControlKeyPressedEvent); \
\
    bool onNotify \
      (_MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedEvent & \
       __r_MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedEvent); \
\
    bool onNotify \
      (_MouseRightButtonPressedEvent & \
       __r_MouseRightButtonPressedEvent); \
\
    bool onNotify \
      (_MouseRightButtonPressedShiftKeyPressedEvent & \
       __r_MouseRightButtonPressedShiftKeyPressedEvent); \
\
    bool onNotify \
      (_MouseRightButtonPressedControlKeyPressedEvent & \
       __r_MouseRightButtonPressedControlKeyPressedEvent); \
\
    bool onNotify \
      (_MouseRightButtonPressedShiftKeyPressedControlKeyPressedEvent & \
       __r_MouseRightButtonPressedShiftKeyPressedControlKeyPressedEvent); \
\
    bool onNotify \
      (_##name##Event & \
       __r_##name##Event); \
\
    void register##name## \
      (void (Scene::*__fp)(Scene *__pScene)); \
\
 private: \
\
    bool _onNotify \
      (Scene *__pScene); \
\
    void (Scene::*__mfp)(Scene *__pScene); \
\
    Scene *__mpScene_; \
    Scene *transmitter__mpScene_; \
} name;



#define BALL_VIEW_IMPLEMENT_INLINE_PRESSED_EVENT_CLASS(name) \
BALL_INLINE \
Scene * \
Events::_##name##Event::getScene \
  () const \
{ \
  return __mpScene_; \
} \
\
BALL_INLINE \
Scene * \
Events::_##name##Event::getTransmitterScene \
  () const \
{ \
  return transmitter__mpScene_; \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonPressedEvent & \
   __r_MouseLeftButtonPressedEvent) \
{ \
  return _onNotify(__r_MouseLeftButtonPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonPressedShiftKeyPressedEvent & \
   __r_MouseLeftButtonPressedShiftKeyPressedEvent) \
{ \
  return _onNotify(__r_MouseLeftButtonPressedShiftKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonPressedControlKeyPressedEvent & \
   __r_MouseLeftButtonPressedControlKeyPressedEvent) \
{ \
  return _onNotify(__r_MouseLeftButtonPressedControlKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedEvent & \
   __r_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedEvent) \
{ \
  return _onNotify(__r_MouseLeftButtonPressedShiftKeyPressedControlKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonPressedEvent & \
   __r_MouseMiddleButtonPressedEvent) \
{ \
  return _onNotify(__r_MouseMiddleButtonPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonPressedShiftKeyPressedEvent & \
   __r_MouseMiddleButtonPressedShiftKeyPressedEvent) \
{ \
  return _onNotify(__r_MouseMiddleButtonPressedShiftKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonPressedControlKeyPressedEvent & \
   __r_MouseMiddleButtonPressedControlKeyPressedEvent) \
{ \
  return _onNotify(__r_MouseMiddleButtonPressedControlKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedEvent & \
   __r_MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedEvent) \
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
{ \
  return _onNotify(__r_MouseRightButtonPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonPressedShiftKeyPressedEvent & \
   __r_MouseRightButtonPressedShiftKeyPressedEvent) \
{ \
  return _onNotify(__r_MouseRightButtonPressedShiftKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonPressedControlKeyPressedEvent & \
   __r_MouseRightButtonPressedControlKeyPressedEvent) \
{ \
  return _onNotify(__r_MouseRightButtonPressedControlKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonPressedShiftKeyPressedControlKeyPressedEvent & \
   __r_MouseRightButtonPressedShiftKeyPressedControlKeyPressedEvent) \
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
      : \
      __mfp(0), \
      __mpScene_(0), \
      transmitter__mpScene_(0) \
    { \
    } \
\
    _##name##Event \
      (Scene *__pScene) \
      : \
      __mfp(0), \
      __mpScene_(__pScene), \
      transmitter__mpScene_(__pScene) \
    { \
    } \
\
    virtual ~_##name##Event \
      () \
    { \
    } \
\
    Scene *getScene \
      () const; \
\
    Scene *getTransmitterScene \
      () const; \
\
    bool onNotify \
      (_MouseLeftButtonReleasedEvent & \
       __r_MouseLeftButtonReleasedEvent); \
\
    bool onNotify \
      (_MouseLeftButtonReleasedShiftKeyPressedEvent & \
       __r_MouseLeftButtonReleasedShiftKeyPressedEvent); \
\
    bool onNotify \
      (_MouseLeftButtonReleasedControlKeyPressedEvent & \
       __r_MouseLeftButtonReleasedControlKeyPressedEvent); \
\
    bool onNotify \
      (_MouseLeftButtonReleasedShiftKeyPressedControlKeyPressedEvent & \
       __r_MouseLeftButtonReleasedShiftKeyPressedControlKeyPressedEvent); \
\
    bool onNotify \
      (_MouseMiddleButtonReleasedEvent & \
       __r_MouseMiddleButtonReleasedEvent); \
\
    bool onNotify \
      (_MouseMiddleButtonReleasedShiftKeyPressedEvent & \
       __r_MouseMiddleButtonReleasedShiftKeyPressedEvent); \
\
    bool onNotify \
      (_MouseMiddleButtonReleasedControlKeyPressedEvent & \
       __r_MouseMiddleButtonReleasedControlKeyPressedEvent); \
\
    bool onNotify \
      (_MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressedEvent & \
       __r_MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressedEvent); \
\
    bool onNotify \
      (_MouseRightButtonReleasedEvent & \
       __r_MouseRightButtonReleasedEvent); \
\
    bool onNotify \
      (_MouseRightButtonReleasedShiftKeyPressedEvent & \
       __r_MouseRightButtonReleasedShiftKeyPressedEvent); \
\
    bool onNotify \
      (_MouseRightButtonReleasedControlKeyPressedEvent & \
       __r_MouseRightButtonReleasedControlKeyPressedEvent); \
\
    bool onNotify \
      (_MouseRightButtonReleasedShiftKeyPressedControlKeyPressedEvent & \
       __r_MouseRightButtonReleasedShiftKeyPressedControlKeyPressedEvent); \
\
    bool onNotify \
      (_##name##Event & \
       __r_##name##Event); \
\
    void register##name## \
      (void (Scene::*__fp)(Scene *__pScene)); \
\
 private: \
\
    bool _onNotify \
      (Scene *__pScene); \
\
    void (Scene::*__mfp)(Scene *__pScene); \
\
    Scene *__mpScene_; \
    Scene *transmitter__mpScene_; \
} name;



#define BALL_VIEW_IMPLEMENT_INLINE_RELEASED_EVENT_CLASS(name) \
BALL_INLINE \
Scene * \
Events::_##name##Event::getScene \
  () const \
{ \
  return __mpScene_; \
} \
\
BALL_INLINE \
Scene * \
Events::_##name##Event::getTransmitterScene \
  () const \
{ \
  return transmitter__mpScene_; \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonReleasedEvent & \
   __r_MouseLeftButtonReleasedEvent) \
{ \
  return _onNotify(__r_MouseLeftButtonReleasedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonReleasedShiftKeyPressedEvent & \
   __r_MouseLeftButtonReleasedShiftKeyPressedEvent) \
{ \
  return _onNotify(__r_MouseLeftButtonReleasedShiftKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonReleasedControlKeyPressedEvent & \
   __r_MouseLeftButtonReleasedControlKeyPressedEvent) \
{ \
  return _onNotify(__r_MouseLeftButtonReleasedControlKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseLeftButtonReleasedShiftKeyPressedControlKeyPressedEvent & \
   __r_MouseLeftButtonReleasedShiftKeyPressedControlKeyPressedEvent) \
{ \
  return _onNotify(__r_MouseLeftButtonReleasedShiftKeyPressedControlKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonReleasedEvent & \
   __r_MouseMiddleButtonReleasedEvent) \
{ \
  return _onNotify(__r_MouseMiddleButtonReleasedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonReleasedShiftKeyPressedEvent & \
   __r_MouseMiddleButtonReleasedShiftKeyPressedEvent) \
{ \
  return _onNotify(__r_MouseMiddleButtonReleasedShiftKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonReleasedControlKeyPressedEvent & \
   __r_MouseMiddleButtonReleasedControlKeyPressedEvent) \
{ \
  return _onNotify(__r_MouseMiddleButtonReleasedControlKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressedEvent & \
   __r_MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressedEvent) \
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
{ \
  return _onNotify(__r_MouseRightButtonReleasedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonReleasedShiftKeyPressedEvent & \
   __r_MouseRightButtonReleasedShiftKeyPressedEvent) \
{ \
  return _onNotify(__r_MouseRightButtonReleasedShiftKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonReleasedControlKeyPressedEvent & \
   __r_MouseRightButtonReleasedControlKeyPressedEvent) \
{ \
  return _onNotify(__r_MouseRightButtonReleasedControlKeyPressedEvent.getScene()); \
} \
\
BALL_INLINE \
bool \
Events::_##name##Event::onNotify \
  (_MouseRightButtonReleasedShiftKeyPressedControlKeyPressedEvent & \
   __r_MouseRightButtonReleasedShiftKeyPressedControlKeyPressedEvent) \
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
{ \
  __mfp = __fp; \
}
    


#define BALL_VIEW_IMPLEMENT_NOTIFY_FUNCTION_FOR_EVENT_CLASS(name) \
bool \
Events::_##name##Event::_onNotify \
  (Scene *__pScene) \
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



////////////////////////////////////////////////////////////////////////////////
//
//    !!! do not compile this class !!!
//
// !!! class will be included in scene !!!
//
////////////////////////////////////////////////////////////////////////////////

		class Events
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			Events();

			Events(Scene* scene);
		
			Events(const Events& events, bool deep = true);

			virtual ~Events();

			virtual void clear();
		
			virtual void destroy();
			//@}
		
			/**	@name	Assignment
			*/
			//@{
			

			void set(const Events& events, bool deep = true);

			Events& operator = (const Events& events);

			void get(Events& events, bool deep = true) const;

			void swap(Events& events);
			//@}

			// --- EVENT CLASS DECLARATIONS

		 BALL_VIEW_DECLARE_EVENT_CLASS
				(ShiftKeyPressed) 

			BALL_VIEW_DECLARE_EVENT_CLASS
				(ShiftKeyPressedMouseMoved)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(ControlKeyPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(ControlKeyPressedMouseMoved)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(ShiftKeyPressedControlKeyPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(ShiftKeyPressedControlKeyPressedMouseMoved)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonPressedShiftKeyPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonPressedControlKeyPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonPressedShiftKeyPressedControlKeyPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonPressedMouseMoved)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonPressedShiftKeyPressedMouseMoved)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonPressedControlKeyPressedMouseMoved)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonPressedShiftKeyPressedControlKeyPressedMouseMoved)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonReleased)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonReleasedShiftKeyPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonReleasedControlKeyPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseLeftButtonReleasedShiftKeyPressedControlKeyPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonPressedShiftKeyPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonPressedControlKeyPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonPressedShiftKeyPressedControlKeyPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonPressedMouseMoved)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonPressedShiftKeyPressedMouseMoved)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonPressedControlKeyPressedMouseMoved)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMoved)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonReleased)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonReleasedShiftKeyPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonReleasedControlKeyPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonPressedShiftKeyPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonPressedControlKeyPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonPressedShiftKeyPressedControlKeyPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonPressedMouseMoved)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonPressedShiftKeyPressedMouseMoved)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonPressedControlKeyPressedMouseMoved)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMoved)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonReleased)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonReleasedShiftKeyPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonReleasedControlKeyPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseRightButtonReleasedShiftKeyPressedControlKeyPressed)
			
			BALL_VIEW_DECLARE_EVENT_CLASS
				(MouseMoved)
			 
			// --- ACCESSORS: INSPECTORS and MUTATORS
		
			BALL_VIEW_DECLARE_PRESSED_MOVED_EVENT_CLASS(RotateSystem)
			BALL_VIEW_DECLARE_PRESSED_MOVED_EVENT_CLASS(ZoomSystem)
			BALL_VIEW_DECLARE_PRESSED_MOVED_EVENT_CLASS(TranslateSystem)
			BALL_VIEW_DECLARE_PRESSED_MOVED_EVENT_CLASS(SelectionPressedMoved)
			BALL_VIEW_DECLARE_PRESSED_EVENT_CLASS(SelectionPressed)
			BALL_VIEW_DECLARE_RELEASED_EVENT_CLASS(SelectionReleased)
			BALL_VIEW_DECLARE_PRESSED_MOVED_EVENT_CLASS(DeselectionPressedMoved)
			BALL_VIEW_DECLARE_PRESSED_EVENT_CLASS(DeselectionPressed)
			BALL_VIEW_DECLARE_RELEASED_EVENT_CLASS(DeselectionReleased)

			
			// --- OPERATORS

			BALL_VIEW_DECLARE_OPERATOR
				(ShiftKeyPressed,
				 ControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(ShiftKeyPressed,
				 MouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR
				(ControlKeyPressed,
				 MouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR
				(ShiftKeyPressed,
				 ControlKeyPressedMouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE
				(ControlKeyPressed,
				 ShiftKeyPressedMouseMoved,
				 ShiftKeyPressedControlKeyPressedMouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR
				(ShiftKeyPressedControlKeyPressed,
				 MouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonPressed,
				 ShiftKeyPressed)

			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonPressed,
				 ControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonPressed,
				 ShiftKeyPressedControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonPressedShiftKeyPressed,
				 ControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE
				(MouseLeftButtonPressedControlKeyPressed,
				 ShiftKeyPressed,
				 MouseLeftButtonPressedShiftKeyPressedControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonPressed,
				 MouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonPressedShiftKeyPressed,
				 MouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonPressedControlKeyPressed,
				 MouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonPressedShiftKeyPressed,
				 ControlKeyPressedMouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE
				(MouseLeftButtonPressedControlKeyPressed,
				 ShiftKeyPressedMouseMoved,
				 MouseLeftButtonPressedShiftKeyPressedControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonPressedShiftKeyPressedControlKeyPressed,
				 MouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonReleased,
				 ShiftKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonReleased,
				 ControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseLeftButtonReleasedShiftKeyPressed,
				 ControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE
				(MouseLeftButtonReleasedControlKeyPressed,
				 ShiftKeyPressed,
				 MouseLeftButtonReleasedShiftKeyPressedControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonPressed,
				 ShiftKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonPressed,
				 ControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonPressed,
				 ShiftKeyPressedControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonPressedShiftKeyPressed,
				 ControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE
				(MouseMiddleButtonPressedControlKeyPressed,
				 ShiftKeyPressed,
				 MouseMiddleButtonPressedShiftKeyPressedControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonPressed,
				 MouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonPressedShiftKeyPressed,
				 MouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonPressedControlKeyPressed,
				 MouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonPressedShiftKeyPressed,
				 ControlKeyPressedMouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE
				(MouseMiddleButtonPressedControlKeyPressed,
				 ShiftKeyPressedMouseMoved,
				 MouseMiddleButtonPressedShiftKeyPressedControlKeyPressedMouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonPressedShiftKeyPressedControlKeyPressed,
				 MouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonReleased,
				 ShiftKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonReleased,
				 ControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseMiddleButtonReleasedShiftKeyPressed,
				 ControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE
				(MouseMiddleButtonReleasedControlKeyPressed,
				 ShiftKeyPressed,
				 MouseMiddleButtonReleasedShiftKeyPressedControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonPressed,
				 ShiftKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonPressed,
				 ControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonPressed,
				 ShiftKeyPressedControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonPressedShiftKeyPressed,
				 ControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE
				(MouseRightButtonPressedControlKeyPressed,
				 ShiftKeyPressed,
				 MouseRightButtonPressedShiftKeyPressedControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonPressed,
				 MouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonPressedShiftKeyPressed,
				 MouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonPressedControlKeyPressed,
				 MouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonPressedShiftKeyPressed,
				 ControlKeyPressedMouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE
				(MouseRightButtonPressedControlKeyPressed,
				 ShiftKeyPressedMouseMoved,
				 MouseRightButtonPressedShiftKeyPressedControlKeyPressedMouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonPressedShiftKeyPressedControlKeyPressed,
				 MouseMoved)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonReleased,
				 ShiftKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonReleased,
				 ControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR
				(MouseRightButtonReleasedShiftKeyPressed,
				 ControlKeyPressed)
			
			BALL_VIEW_DECLARE_OPERATOR_WITH_RETURN_TYPE
				(MouseRightButtonReleasedControlKeyPressed,
				 ShiftKeyPressed,
				 MouseRightButtonReleasedShiftKeyPressedControlKeyPressed)

			Scene *getScene();
				
			// --- DEBUGGERS and DIAGNOSTICS

			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;

			// --- STORERS

			virtual void read(std::istream& s);

			virtual void write(std::ostream& s) const;

			

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



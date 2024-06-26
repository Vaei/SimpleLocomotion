# Simple Locomotion

Simple locomotion system code functionality to extend from, with respect to updating in worker threads for maximum performance optimization.

This demonstrates best practices for building the functionality side of locomotion systems.

Works with any actor, not only pawns or characters. That means it supports Mover 2.0.

Requires C++ for component implementation.

Does not include an anim blueprint. Extend the USimpleAnimInstance to make your own. Does not include any content.

## Setup

Derive from AActor or any subclass in C++ and implement the `USimpleAnimComponent`. Look at the header to see the comments for each function to understand how to implement them.

Create an animation blueprint with `USimpleAnimInstance` as the parent class.

That's it.

Don't forget to go to your animation blueprint Class Settings and enable `bWarnAboutBlueprintUsage` to get compiler warnings whenever you lose fast path.

Next you will want to either subclass `USimpleAnimInstance` to extend functionality, or extend via the Animation Blueprint. However, animation blueprint running in a virtual machine is a significant performance loss so use C++ if you are capable of doing so.

You will likely run into fast path warnings on your animation state transitions. For this you will want to use ThreadSafeUpdateAnimation to cache the conditions to a boolean and pass that boolean to the transition instead.

## Changelog

### 1.5.0
* Add function for IsSimpleCurrentFloorWalkable() to improve ground/air detection
* Extend FSimpleAnimInstanceProxy printing functionality to not repeat messages
* Extract jumping and falling to virtual function
* Fixed bug where incorrect bool was checked for in air

### 1.4.1
* Make bWantsCardinalsUpdated public
* Add bWantsLeansUpdated property
* Add LeanRateOverride

### 1.4.0
* Add custom anim instance proxy for pushing information out of worker threads

### 1.3.3
* Cache PrevSpeed

### 1.3.2
* Moved FSimpleLandedSignature to SimpleLocomotionTypes for include

### 1.3.1
* Fixed bug where incorrect speed was factored

### 1.3.0
* Disable anim component tick
* Hide anim component properties via details customization for CD0 as they are not needed
* Prevent message log spam
* Add extension point OnAnimNotValidToUpdate

### 1.2.2
* Add SimpleLocomotionEditor module
* Hide common categories from CD0 to make it cleaner for designers
* Mark categories as important from CD0 for same purpose

### 1.2.1
* Add Speed, Speed3D, and Speed2D convenience parameters

### 1.2.0
* Add basic cardinal movement property handling
* Add start and stop gait mode support
* Some properties moved to public instead of protected after anticipating getters

### 1.1.0
* Change to component based approach instead of interface

### 1.0.7
* Don't require GetSimpleOnLandedDelegate(), or GetSimpleIsMoveModeValid() implemented

### 1.0.6
* Implement the GetLocal() conversion (had been forgotten)

### 1.0.5
* Expose `IsAnimValidToUpdate` to BP
* Make `IsAnimValidToUpdate` `const`

### 1.0.4
* Added `TimeToJumpApex`
* Initialized remaining members

### 1.0.3
* Added `bHasVelocity` and `bHasAcceleration`

### 1.0.2
* Added extension points during NativeThreadSafeUpdateAnimation()

### 1.0.1
* Added stance and gait change detection

### 1.0.0
* Release
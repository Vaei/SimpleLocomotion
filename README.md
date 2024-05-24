# Simple Locomotion

Simple locomotion system code functionality to extend from, with respect to updating in worker threads for maximum performance optimization.

This demonstrates best practices for building the functionality side of locomotion systems.

Works with any actor, not only pawns or characters. That means it supports Mover 2.0.

Requires C++ for interface implementation.

Does not include an anim blueprint. Extend the USimpleAnimInstance to make your own. Does not include any content.

## Setup

Derive from AActor or any subclass in C++ and implement the `ISimpleAnimInterface`. Look at the header to see the comments for each function to understand how to implement them.

Create an animation blueprint with `USimpleAnimInstance` as the parent class.

That's it.

Don't forget to go to your animation blueprint Class Settings and enable `bWarnAboutBlueprintUsage` to get compiler warnings whenever you lose fast path.

Next you will want to either subclass `USimpleAnimInstance` to extend functionality, or extend via the Animation Blueprint. However, animation blueprint running in a virtual machine is a significant performance loss so use C++ if you are capable of doing so.

You will likely run into fast path warnings on your animation state transitions. For this you will want to use ThreadSafeUpdateAnimation to cache the conditions to a boolean and pass that boolean to the transition instead.

## Changelog

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
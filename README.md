# Simple Locomotion <img align="right" width=256, height=256 src="https://github.com/Vaei/SimpleLocomotion/blob/main/Resources/Icon256.png">

> [!IMPORTANT]
> A simple locomotion framework to extend from
> <br>Updates in worker threads for maximum performance optimization
> <br>Demonstrates best practices for building the foundation side of locomotion systems

> [!CAUTION]
> This plugin was recently under heavy development, and has now re-released. If you want the old version use the `deprecated` branch.

> [!TIP]
> Suitable for both singleplayer and multiplayer games
> <br>Supports UE5.2+

> [!NOTE]
> Works with any actor, not only pawns or characters -- It supports both CMC and Mover 2.0
> <br>Requires C++ for SimpleAnimComponent implementation to ensure the best possible performance

> [!TIP]
> Does not include an anim blueprint -- Extend the USimpleAnimInstance to make your own
> <br>Does not include any content

## Setup

Derive from AActor or any subclass in C++ and implement the `USimpleAnimComponent`. Look at the header to see the comments for each function to understand how to implement them.

Create an animation blueprint with `USimpleAnimInstance` as the parent class.

That's it.

Don't forget to go to your animation blueprint Class Settings and enable `bWarnAboutBlueprintUsage` to get compiler warnings whenever you lose fast path.

Next you will want to either subclass `USimpleAnimInstance` to extend functionality, or extend via the Animation Blueprint. However, animation blueprint running in a virtual machine is a significant performance loss so use C++ if you are capable of doing so.

You will likely run into fast path warnings on your animation state transitions. For this you will want to use ThreadSafeUpdateAnimation to cache the conditions to a boolean and pass that boolean to the transition instead.

## Changelog

### 1.1.0
* Fix critical bug regression with anim states not applying fallbacks correctly
* Add `FSimpleStateToBlendSpaceSet`

### 1.0.8
* Add `FSimpleStateToAnimSet`

### 1.0.7
* Remove Git LFS

### 1.0.6
* Add structs mimicking blending from Blend Poses by Bool: `USimpleStatics::BlendByBool()`
* Fixed copyright notice

### 1.0.5
* Add `FSimpleStrafeMontageSet`
* Details customization can hide unused properties for directional loco sets (C++ only)

### 1.0.4

_You need to add `DefaultSimpleLocomotion.ini` for redirectors - [Get it here](https://github.com/Vaei/SimpleLocomotion/commit/d5060dd2bb24e15952acb2e6067b0c67e3955791) and add it to the plugin's config folder_

* Refactor Sets based on tag to reduce ambiguity

### 1.0.3
* Query `GaitSpeed` based on speed mid points instead of >= MaxSpeed
* Add gait mode debug drawing `a.SimpleAnim.DrawGaitModes 1`

### 1.0.2
* Add cardinal support for wall movement

### 1.0.1
* Replace native ptr with TObjectPtr

### 1.0.0
* Release

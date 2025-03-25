# Simple Locomotion <img align="right" width=256, height=256 src="https://github.com/Vaei/SimpleLocomotion/blob/main/Resources/Icon256.png">

> [!IMPORTANT]
> A simple locomotion framework to extend from
> <br>Updates in worker threads for maximum performance optimization
> <br>Demonstrates best practices for building the foundation side of locomotion systems

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

### 4.0.1
* Extension point for PreMovement update & IsPlayingNetworkedRootMotionMontage caching

### 4.0.0
* Add Simple.State tag to represent differing AnimStates
* Added nested data types, e.g. per-stance animations can be represented as AnimState:Stance:Anim
* Added further data types to improve overall support
	* Added transition sets with Entry/Loop/Exit animations
* Changed from pointer struct getters to ref struct getters to stop copying structs at the final goal (anim graph)

### 3.4.2
* Support UE5.2, 5.3
* Fix up includes

### 3.4.1
* Expose IsAnyMontagePlaying

### 3.4.0
* Added FSimpleGameplayTags::Simple_Gait_Stroll
	* Primarily intended for NPCs who are not technically idle, but don't really have a purpose, and are wandering or possibly patrolling

### 3.3.0
* Refactor FSimpleGameplayTags::Simple_Mode to FSimpleGameplayTags::Simple_Cardinal_Mode
* Improved readability of input parameters by renaming, so its clearer what tag is expected
* Improved FSimpleGaitSpeed::GetMaxSpeed Gait tag handling and logging

### 3.2.1
* Rename SimpleLocomotionTags.h/cpp to SimpleGameplayTags.h/cpp

### 3.2.0
* Modified USimpleLocomotionStatics function names to prefix Simple
* Added USimpleLocomotionStatics::GetSimpleAnimation to support BP usage

### 3.1.0
* Add further helper functions available in USimpleLocomotionStatics
* Removed redundant functions from USimpleLocomotionStatics
* Marked functions as BlueprintThreadSafe
* GaitSet getters output the CardinalType also
* Fixed bug where cardinals were cached when cached cardinals not requested
* FSimpleCardinals::GetCurrentCardinal aborts if EmptyTag, and warns if failed

### 3.0.0
_Major update, changing to entirely FGameplayTag based approach_
* Cardinal handling
	* FSimpleCardinalMovement refactored to FSimpleCardinals
	* Implemented FGameplayTag based cardinal approach
	* Added delegate for updating cardinals, see USimpleAnimInstance::UpdateCardinal() for usage
* Locomotion set handling
	* Added FSimpleLocomotionSet
	* Added FSimpleGaitSet
	* Refactor "Jog" as "Run"
* Changed FSimpleGaitSpeed to use GameplayTags for gaits to bring it in-line with the gameplay tag approach
* FSimpleCardinalMovement::CalculateDirection reintroduces static_cast as per engine version
* Protected FSimpleCardinalMovement::ThreadSafeUpdate_Internal

### 2.1.0
* Add ESimpleCardinalType enum
	* Refactor FSimpleCardinalMovement properties to match ESimpleCardinalType enum
	* Implement convenience getter in FSimpleCardinalMovement using ESimpleCardinalType

### 2.0.2
* Fixed accidental CVarPrintInvalidGameplayTagStates referencing wrong bool

### 2.0.1
* Build.cs FGameplayTag dependency moved from Public to Private

### 2.0.0
* Switched from enums to FGameplayTag
* Fixed gait change detection and fixed double update
* Added virtual NativeThreadSafeUpdateStance() for overriding
	* Removed prone, no longer necessary to include this thanks to gameplay tags, feel free to extend the system and re-add it
* Added a few missing init properties in USimpleAnimInstance

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

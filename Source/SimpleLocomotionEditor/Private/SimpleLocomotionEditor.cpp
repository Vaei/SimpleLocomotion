#include "SimpleLocomotionEditor.h"

#include "SimpleAnimComponent.h"
#include "SimpleAnimComponentCustomization.h"
#include "SimpleAnimInstanceBase.h"
#include "SimpleAnimInstanceCustomization.h"
#include "SimpleStartLocoSetCustomization.h"
#include "SimpleStrafeLocoSetCustomization.h"
#include "SimpleTurnLocoSetCustomization.h"

#define LOCTEXT_NAMESPACE "FSimpleLocomotionEditorModule"

void FSimpleLocomotionEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	// Anim Instance
	PropertyModule.RegisterCustomClassLayout(
		USimpleAnimInstanceBase::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FSimpleAnimInstanceCustomization::MakeInstance)
	);

	// Anim Component
	PropertyModule.RegisterCustomClassLayout(
		USimpleAnimComponent::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FSimpleAnimComponentCustomization::MakeInstance)
	);

	// FSimpleStrafeLocoSet (SimpleLocomotionTypes)
	PropertyModule.RegisterCustomPropertyTypeLayout(TEXT("SimpleStrafeLocoSet"),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FSimpleStrafeLocoSetCustomization::MakeInstance));

	// FSimpleStartLocoSet (SimpleLocomotionTypes)
	PropertyModule.RegisterCustomPropertyTypeLayout(TEXT("SimpleStartLocoSet"),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FSimpleStartLocoSetCustomization::MakeInstance));
	
	// FSimpleTurnLocoSet (SimpleLocomotionTypes)
	PropertyModule.RegisterCustomPropertyTypeLayout(TEXT("SimpleTurnLocoSet"),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FSimpleTurnLocoSetCustomization::MakeInstance));
}

void FSimpleLocomotionEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule* PropertyModule = FModuleManager::Get().GetModulePtr<FPropertyEditorModule>("PropertyEditor");
		PropertyModule->UnregisterCustomClassLayout(USimpleAnimInstanceBase::StaticClass()->GetFName());
		PropertyModule->UnregisterCustomClassLayout(USimpleAnimComponent::StaticClass()->GetFName());

		PropertyModule->UnregisterCustomPropertyTypeLayout("SimpleStrafeLocoSet");
		PropertyModule->UnregisterCustomPropertyTypeLayout("SimpleStartLocoSet");
		PropertyModule->UnregisterCustomPropertyTypeLayout("SimpleTurnLocoSet");
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FSimpleLocomotionEditorModule, SimpleLocomotionEditor)
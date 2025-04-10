#include "SimpleLocomotionEditor.h"

#include "SimpleAnimComponent.h"
#include "SimpleAnimComponentDetailsCustomization.h"
#include "SimpleAnimInstanceBase.h"
#include "SimpleAnimInstanceDetailsCustomization.h"
#include "SimpleStartLocomotionSetPropertyCustomization.h"
#include "SimpleStrafeLocomotionSetPropertyCustomization.h"
#include "SimpleTurnLocomotionSetPropertyCustomization.h"

#define LOCTEXT_NAMESPACE "FSimpleLocomotionEditorModule"

void FSimpleLocomotionEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	// Anim Instance
	PropertyModule.RegisterCustomClassLayout(
		USimpleAnimInstanceBase::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FSimpleAnimInstanceDetailsCustomization::MakeInstance)
	);

	// Anim Component
	PropertyModule.RegisterCustomClassLayout(
		USimpleAnimComponent::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FSimpleAnimComponentDetailsCustomization::MakeInstance)
	);

	// FSimpleStrafeLocomotionSet (SimpleLocomotionTypes)
	PropertyModule.RegisterCustomPropertyTypeLayout(TEXT("SimpleStrafeLocomotionSet"),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FSimpleStrafeLocomotionSetPropertyCustomization::MakeInstance));

	// FSimpleStartLocomotionSet (SimpleLocomotionTypes)
	PropertyModule.RegisterCustomPropertyTypeLayout(TEXT("SimpleStartLocomotionSet"),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FSimpleStartLocomotionSetPropertyCustomization::MakeInstance));
	
	// FSimpleTurnLocomotionSet (SimpleLocomotionTypes)
	PropertyModule.RegisterCustomPropertyTypeLayout(TEXT("SimpleTurnLocomotionSet"),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FSimpleTurnLocomotionSetPropertyCustomization::MakeInstance));
}

void FSimpleLocomotionEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule* PropertyModule = FModuleManager::Get().GetModulePtr<FPropertyEditorModule>("PropertyEditor");
		PropertyModule->UnregisterCustomClassLayout(USimpleAnimInstanceBase::StaticClass()->GetFName());
		PropertyModule->UnregisterCustomClassLayout(USimpleAnimComponent::StaticClass()->GetFName());

		PropertyModule->UnregisterCustomPropertyTypeLayout("SimpleStrafeLocomotionSet");
		PropertyModule->UnregisterCustomPropertyTypeLayout("SimpleStartLocomotionSet");
		PropertyModule->UnregisterCustomPropertyTypeLayout("SimpleTurnLocomotionSet");
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FSimpleLocomotionEditorModule, SimpleLocomotionEditor)
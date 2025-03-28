#include "SimpleLocomotionEditor.h"

#include "SimpleAnimComponent.h"
#include "SimpleAnimComponentDetailsCustomization.h"
#include "SimpleAnimInstanceBase.h"
#include "SimpleAnimInstanceDetailsCustomization.h"
#include "SimpleLocomotionSetPropertyCustomization.h"

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

	// FSimpleLocomotionSet (SimpleLocomotionTypes)
	PropertyModule.RegisterCustomPropertyTypeLayout(TEXT("SimpleLocomotionSet"),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FSimpleLocomotionSetPropertyCustomization::MakeInstance));
}

void FSimpleLocomotionEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule* PropertyModule = FModuleManager::Get().GetModulePtr<FPropertyEditorModule>("PropertyEditor");
		PropertyModule->UnregisterCustomClassLayout(USimpleAnimInstanceBase::StaticClass()->GetFName());
		PropertyModule->UnregisterCustomClassLayout(USimpleAnimComponent::StaticClass()->GetFName());

		PropertyModule->UnregisterCustomPropertyTypeLayout("SimpleLocomotionSet");
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FSimpleLocomotionEditorModule, SimpleLocomotionEditor)
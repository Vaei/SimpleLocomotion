#include "SimpleLocomotionEditor.h"

#include "SimpleAnimComponent.h"
#include "SimpleAnimComponentDetailsCustomization.h"
#include "SimpleAnimInstance.h"
#include "SimpleAnimInstanceDetailsCustomization.h"

#define LOCTEXT_NAMESPACE "FSimpleLocomotionEditorModule"

void FSimpleLocomotionEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	// Anim Instance
	PropertyModule.RegisterCustomClassLayout(
		USimpleAnimInstance::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FSimpleAnimInstanceDetailsCustomization::MakeInstance)
	);

	// Anim Component
	PropertyModule.RegisterCustomClassLayout(
		USimpleAnimComponent::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FSimpleAnimComponentDetailsCustomization::MakeInstance)
	);
}

void FSimpleLocomotionEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule* PropertyModule = FModuleManager::Get().GetModulePtr<FPropertyEditorModule>("PropertyEditor");
		PropertyModule->UnregisterCustomClassLayout(USimpleAnimInstance::StaticClass()->GetFName());
		PropertyModule->UnregisterCustomClassLayout(USimpleAnimComponent::StaticClass()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FSimpleLocomotionEditorModule, SimpleLocomotionEditor)
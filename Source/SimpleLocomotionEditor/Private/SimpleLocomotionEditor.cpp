#include "SimpleLocomotionEditor.h"

#include "SimpleAnimInstance.h"
#include "SimpleAnimInstanceDetailsCustomization.h"

#define LOCTEXT_NAMESPACE "FSimpleLocomotionEditorModule"

void FSimpleLocomotionEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyModule.RegisterCustomClassLayout(
		USimpleAnimInstance::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FSimpleAnimInstanceDetailsCustomization::MakeInstance)
	);
}

void FSimpleLocomotionEditorModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule* PropertyModule = FModuleManager::Get().GetModulePtr<FPropertyEditorModule>("PropertyEditor");
		PropertyModule->UnregisterCustomClassLayout(USimpleAnimInstance::StaticClass()->GetFName());
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FSimpleLocomotionEditorModule, SimpleLocomotionEditor)
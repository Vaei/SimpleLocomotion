// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleAnimInstanceDetailsCustomization.h"

#include "DetailLayoutBuilder.h"
#include "SimpleAnimInstance.h"


TSharedRef<IDetailCustomization> FSimpleAnimInstanceDetailsCustomization::MakeInstance()
{
	return MakeShared<FSimpleAnimInstanceDetailsCustomization>();
}

void FSimpleAnimInstanceDetailsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> Objects;
	DetailBuilder.GetObjectsBeingCustomized(Objects);

	USimpleAnimInstance* SimpleAnimInstance = Objects.IsValidIndex(0) ? Cast<USimpleAnimInstance>(Objects[0]) : nullptr;
	if (!SimpleAnimInstance)
	{
		return;
	}

	if (SimpleAnimInstance->HasAnyFlags(RF_ClassDefaultObject | RF_Transactional))
	{
		DetailBuilder.EditCategory(TEXT("Editor"), FText::GetEmpty(), ECategoryPriority::Transform);
		for (const FName& Category : SimpleAnimInstance->HideEditorCategories)
		{
			DetailBuilder.HideCategory(Category);
		}
		
		for (const FName& Category : SimpleAnimInstance->ImportantEditorCategories)
		{
			DetailBuilder.EditCategory(Category, FText::GetEmpty(), ECategoryPriority::Important);
		}
	}
	else
	{
		const TSharedPtr<IPropertyHandle> EditorProperty = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(USimpleAnimInstance, HideEditorCategories));
		const TSharedPtr<IPropertyHandle> ImportantProperty = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(USimpleAnimInstance, ImportantEditorCategories));
		DetailBuilder.HideProperty(EditorProperty);
		DetailBuilder.HideProperty(ImportantProperty);
	}
}
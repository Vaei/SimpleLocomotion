// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleAnimInstanceCustomization.h"

#include "DetailLayoutBuilder.h"
#include "SimpleAnimInstanceBase.h"


TSharedRef<IDetailCustomization> FSimpleAnimInstanceCustomization::MakeInstance()
{
	return MakeShared<FSimpleAnimInstanceCustomization>();
}

void FSimpleAnimInstanceCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> Objects;
	DetailBuilder.GetObjectsBeingCustomized(Objects);

	USimpleAnimInstanceBase* SimpleAnimInstance = Objects.IsValidIndex(0) ? Cast<USimpleAnimInstanceBase>(Objects[0]) : nullptr;
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
		const TSharedPtr<IPropertyHandle> EditorProperty = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(USimpleAnimInstanceBase, HideEditorCategories));
		const TSharedPtr<IPropertyHandle> ImportantProperty = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(USimpleAnimInstanceBase, ImportantEditorCategories));
		DetailBuilder.HideProperty(EditorProperty);
		DetailBuilder.HideProperty(ImportantProperty);
	}
}
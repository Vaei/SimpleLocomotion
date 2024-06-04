// Copyright (c) Jared Taylor. All Rights Reserved


#include "SimpleAnimComponentDetailsCustomization.h"

#include "DetailLayoutBuilder.h"
#include "SimpleAnimComponent.h"


TSharedRef<IDetailCustomization> FSimpleAnimComponentDetailsCustomization::MakeInstance()
{
	return MakeShared<FSimpleAnimComponentDetailsCustomization>();
}

void FSimpleAnimComponentDetailsCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> Objects;
	DetailBuilder.GetObjectsBeingCustomized(Objects);

	USimpleAnimComponent* SimpleAnimComponent = Objects.IsValidIndex(0) ? Cast<USimpleAnimComponent>(Objects[0]) : nullptr;
	if (!SimpleAnimComponent)
	{
		return;
	}

	if (SimpleAnimComponent->HasAnyFlags(RF_ClassDefaultObject | RF_Transactional))
	{
		DetailBuilder.EditCategory(TEXT("Editor"), FText::GetEmpty(), ECategoryPriority::Transform);
		for (const FName& Category : SimpleAnimComponent->HideEditorCategories)
		{
			DetailBuilder.HideCategory(Category);
		}
		
		for (const FName& Category : SimpleAnimComponent->ImportantEditorCategories)
		{
			DetailBuilder.EditCategory(Category, FText::GetEmpty(), ECategoryPriority::Important);
		}
	}
	else
	{
		const TSharedPtr<IPropertyHandle> EditorProperty = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(USimpleAnimComponent, HideEditorCategories));
		const TSharedPtr<IPropertyHandle> ImportantProperty = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(USimpleAnimComponent, ImportantEditorCategories));
		DetailBuilder.HideProperty(EditorProperty);
		DetailBuilder.HideProperty(ImportantProperty);
	}
}
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.

#pragma once

#include "CoreMinimal.h"
#include "UI/WW_WidgetController.h"
#include "GameplayTagContainer.h"
#include "ScreenWidgetController.generated.h"



struct FOnAttributeChangeData;
class UWW_UserWidget;


/** Struct for showing message to the screen */
USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UWW_UserWidget> MessageWidget;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//UTexture2D* Image = nullptr;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);


UCLASS(BlueprintType, Blueprintable)
class WAVEWAR_API UScreenWidgetController : public UWW_WidgetController
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable, Category = "HealthAttributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "HealthAttributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Messages")
	FMessageWidgetRowSignature MessageWidgetRow;

	////****	FUNCTIONS	****////

	virtual void BroadcastInitialValues() override;

	virtual void InitBindingAttributes() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	/** Description is in .cpp file */
	//void HealthChanged(const FOnAttributeChangeData& Data) const;
	//void MaxHealthChanged(const FOnAttributeChangeData& Data) const;

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
};

template<typename T>
inline T* UScreenWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	T* Row = DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
	if (Row)
	{
		return Row;
	}

	return nullptr;
}

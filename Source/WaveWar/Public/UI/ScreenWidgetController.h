// Uroš Pörš All Rights Reserved

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


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatSignature, int32, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);


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

	UPROPERTY(BlueprintAssignable, Category = "LevelUp")
	FOnAttributeChangedSignature OnXPPercentChanged;

	UPROPERTY(BlueprintAssignable, Category = "LevelUp")
	FOnPlayerStatSignature OnPlayerLevelChanged;

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

	void OnXPChanged(int32 NewXP);
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

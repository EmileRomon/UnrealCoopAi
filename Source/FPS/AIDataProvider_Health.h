// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataProviders/AIDataProvider_QueryParams.h"
#include "AIDataProvider_Health.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, meta = (DisplayName = "Based on Health"))
class FPS_API UAIDataProvider_Health : public UAIDataProvider_QueryParams
{
	GENERATED_BODY()
	
public:
	UAIDataProvider_Health(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void BindData(const UObject& Owner, int32 RequestId) override;
	virtual FString ToString(FName PropName) const override;

	UPROPERTY(EditAnywhere, Category = AI)
	float ScoringFactor;
};

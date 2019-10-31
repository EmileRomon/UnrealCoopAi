// Fill out your copyright notice in the Description page of Project Settings.


#include "AIDataProvider_Health.h"
#include "FPSCharacter.h"

UAIDataProvider_Health::UAIDataProvider_Health(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	ScoringFactor = 1.0f;
	FloatValue = 1.0f;

}

void UAIDataProvider_Health::BindData(const UObject& Owner, int32 RequestId)
{
	const AFPSCharacter *Querier = Cast<AFPSCharacter>(&Owner);
	if (Querier)
	{
		FloatValue = ((Querier->PercentageHealth - 1.0f) * -1.0f) + 1.0f;
		FloatValue *= ScoringFactor;
	}
}

FString UAIDataProvider_Health::ToString(FName PropName) const
{
	return FString::Printf(TEXT("Remaining Health"));
}

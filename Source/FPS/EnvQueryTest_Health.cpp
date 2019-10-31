// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryTest_Health.h"
#include "FPSCharacter.h"

UEnvQueryTest_Health::UEnvQueryTest_Health(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType::StaticClass();
}
void UEnvQueryTest_Health::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (QueryOwner == nullptr)
	{
		return;
	}

	FloatValueMin.BindData(QueryOwner, QueryInstance.QueryID);
	float MinThresholdValue = FloatValueMin.GetValue();

	FloatValueMax.BindData(QueryOwner, QueryInstance.QueryID);
	float MaxThresholdValue = FloatValueMax.GetValue();

	// loop through all items
	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		AFPSCharacter *Char = Cast<AFPSCharacter>(GetItemActor(QueryInstance, It));
		if (Char)
		{
			It.SetScore(TestPurpose, FilterType, Char->GetHealth(), MinThresholdValue, MaxThresholdValue);
		}
	}
}

FText UEnvQueryTest_Health::GetDescriptionDetails() const
{
	return DescribeFloatTestParams();
}
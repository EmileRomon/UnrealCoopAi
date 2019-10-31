// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryTest_Height.h"

UEnvQueryTest_Height::UEnvQueryTest_Height(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType::StaticClass();
}
void UEnvQueryTest_Height::RunTest(FEnvQueryInstance& QueryInstance) const
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
		FVector ItLocation = GetItemLocation(QueryInstance, It);
		It.SetScore(TestPurpose, FilterType, ItLocation.Z, MinThresholdValue, MaxThresholdValue);
	}
}

FText UEnvQueryTest_Height::GetDescriptionDetails() const
{
	return DescribeFloatTestParams();
}
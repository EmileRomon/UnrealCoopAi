// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryTest_TraceHealthScore.h"
#include "UObject/Package.h"
#include "CollisionQueryParams.h"
#include "WorldCollision.h"
#include "Engine/World.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "EnvironmentQuery/Contexts/EnvQueryContext_Querier.h"
#include "FPSCharacter.h"

UEnvQueryTest_TraceHealthScore::UEnvQueryTest_TraceHealthScore(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Cost = EEnvTestCost::High;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
	SetWorkOnFloatValues(true);
	Context = UEnvQueryContext_Querier::StaticClass();
	TraceData.SetGeometryOnly();

}

void UEnvQueryTest_TraceHealthScore::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* DataOwner = QueryInstance.Owner.Get();
	BoolValue.BindData(DataOwner, QueryInstance.QueryID);
	TraceFromContext.BindData(DataOwner, QueryInstance.QueryID);
	ItemHeightOffset.BindData(DataOwner, QueryInstance.QueryID);
	ContextHeightOffset.BindData(DataOwner, QueryInstance.QueryID);

	bool bWantsHit = BoolValue.GetValue();
	bool bTraceToItem = TraceFromContext.GetValue();
	float ItemZ = ItemHeightOffset.GetValue();
	float ContextZ = ContextHeightOffset.GetValue();

	AFPSCharacter *Querier = Cast<AFPSCharacter>(DataOwner);


	TArray<FVector> ContextLocations;
	if (!QueryInstance.PrepareContext(Context, ContextLocations))
	{
		return;
	}

	TArray<AActor *> ContextActors;
	if (!QueryInstance.PrepareContext(Context, ContextActors))
	{
		return;
	}

	FloatValueMin.BindData(DataOwner, QueryInstance.QueryID);
	float MinThresholdValue = FloatValueMin.GetValue();

	FloatValueMax.BindData(DataOwner, QueryInstance.QueryID);
	float MaxThresholdValue = FloatValueMax.GetValue();

	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(EnvQueryTrace), TraceData.bTraceComplex);

	TArray<AActor*> IgnoredActors;
	if (QueryInstance.PrepareContext(Context, IgnoredActors))
	{
		//Ignore the querier
		if (Querier)
		{
			IgnoredActors.Add(Querier);
		}
		TraceParams.AddIgnoredActors(IgnoredActors);
	}

	ECollisionChannel TraceCollisionChannel = UEngineTypes::ConvertToCollisionChannel(TraceData.TraceChannel);
	FVector TraceExtent(TraceData.ExtentX, TraceData.ExtentY, TraceData.ExtentZ);
	FRunTraceSignature TraceFunc;
	switch (TraceData.TraceShape)
	{
	case EEnvTraceShape::Line:
		TraceFunc.BindUObject(this, bTraceToItem ? &UEnvQueryTest_TraceHealthScore::RunLineTraceTo : &UEnvQueryTest_TraceHealthScore::RunLineTraceFrom);
		break;

	case EEnvTraceShape::Box:
		TraceFunc.BindUObject(this, bTraceToItem ? &UEnvQueryTest_TraceHealthScore::RunBoxTraceTo : &UEnvQueryTest_TraceHealthScore::RunBoxTraceFrom);
		break;

	case EEnvTraceShape::Sphere:
		TraceFunc.BindUObject(this, bTraceToItem ? &UEnvQueryTest_TraceHealthScore::RunSphereTraceTo : &UEnvQueryTest_TraceHealthScore::RunSphereTraceFrom);
		break;

	case EEnvTraceShape::Capsule:
		TraceFunc.BindUObject(this, bTraceToItem ? &UEnvQueryTest_TraceHealthScore::RunCapsuleTraceTo : &UEnvQueryTest_TraceHealthScore::RunCapsuleTraceFrom);
		break;

	default:
		return;
	}

	for (int32 ContextIndex = 0; ContextIndex < ContextLocations.Num(); ContextIndex++)
	{
		ContextLocations[ContextIndex].Z += ContextZ;
	}

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex()) + FVector(0, 0, ItemZ);
		AActor* ItemActor = GetItemActor(QueryInstance, It.GetIndex());

		for (int32 ContextIndex = 0; ContextIndex < ContextLocations.Num(); ContextIndex++)
		{
			const bool bHit = TraceFunc.Execute(ItemLocation, ContextLocations[ContextIndex], ItemActor, QueryInstance.World, TraceCollisionChannel, TraceParams, TraceExtent);

			AFPSCharacter *Char = Cast<AFPSCharacter>(ContextActors[ContextIndex]);
			if (Char && !bHit)
			{
				float Damage = (Char->GetHealth() - 1.0f) * - 1.0f;
				It.SetScore(TestPurpose, FilterType, Damage + 1.0f, MinThresholdValue, MaxThresholdValue);
			}
		}
	}
}

void UEnvQueryTest_TraceHealthScore::PostLoad()
{
	Super::PostLoad();
	TraceData.OnPostLoad();
}

FText UEnvQueryTest_TraceHealthScore::GetDescriptionTitle() const
{
	UEnum* ChannelEnum = StaticEnum<ETraceTypeQuery>();
	FString ChannelDesc = ChannelEnum->GetDisplayNameTextByValue(TraceData.TraceChannel).ToString();

	FString DirectionDesc = TraceFromContext.IsDynamic() ?
		FString::Printf(TEXT("%s, direction: %s"), *UEnvQueryTypes::DescribeContext(Context).ToString(), *TraceFromContext.ToString()) :
		FString::Printf(TEXT("%s %s"), TraceFromContext.DefaultValue ? TEXT("from") : TEXT("to"), *UEnvQueryTypes::DescribeContext(Context).ToString());

	return FText::FromString(FString::Printf(TEXT("%s: %s on %s"),
		*Super::GetDescriptionTitle().ToString(), *DirectionDesc, *ChannelDesc));
}

FText UEnvQueryTest_TraceHealthScore::GetDescriptionDetails() const
{
	return FText::Format(FText::FromString("{0}\n{1}"),
		TraceData.ToText(FEnvTraceData::Detailed), DescribeBoolTestParams("hit"));
}

bool UEnvQueryTest_TraceHealthScore::RunLineTraceTo(const FVector& ItemPos, const FVector& ContextPos, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params, const FVector& Extent)
{
	FCollisionQueryParams TraceParams(Params);
	TraceParams.AddIgnoredActor(ItemActor);

	const bool bHit = World->LineTraceTestByChannel(ContextPos, ItemPos, Channel, TraceParams);
	return bHit;
}

bool UEnvQueryTest_TraceHealthScore::RunLineTraceFrom(const FVector& ItemPos, const FVector& ContextPos, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params, const FVector& Extent)
{
	FCollisionQueryParams TraceParams(Params);
	TraceParams.AddIgnoredActor(ItemActor);

	const bool bHit = World->LineTraceTestByChannel(ItemPos, ContextPos, Channel, TraceParams);
	return bHit;
}

bool UEnvQueryTest_TraceHealthScore::RunBoxTraceTo(const FVector& ItemPos, const FVector& ContextPos, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params, const FVector& Extent)
{
	FCollisionQueryParams TraceParams(Params);
	TraceParams.AddIgnoredActor(ItemActor);

	const bool bHit = World->SweepTestByChannel(ContextPos, ItemPos, FQuat((ItemPos - ContextPos).Rotation()), Channel, FCollisionShape::MakeBox(Extent), TraceParams);
	return bHit;
}

bool UEnvQueryTest_TraceHealthScore::RunBoxTraceFrom(const FVector& ItemPos, const FVector& ContextPos, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params, const FVector& Extent)
{
	FCollisionQueryParams TraceParams(Params);
	TraceParams.AddIgnoredActor(ItemActor);

	const bool bHit = World->SweepTestByChannel(ItemPos, ContextPos, FQuat((ContextPos - ItemPos).Rotation()), Channel, FCollisionShape::MakeBox(Extent), TraceParams);
	return bHit;
}

bool UEnvQueryTest_TraceHealthScore::RunSphereTraceTo(const FVector& ItemPos, const FVector& ContextPos, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params, const FVector& Extent)
{
	FCollisionQueryParams TraceParams(Params);
	TraceParams.AddIgnoredActor(ItemActor);

	const bool bHit = World->SweepTestByChannel(ContextPos, ItemPos, FQuat::Identity, Channel, FCollisionShape::MakeSphere(Extent.X), TraceParams);
	return bHit;
}

bool UEnvQueryTest_TraceHealthScore::RunSphereTraceFrom(const FVector& ItemPos, const FVector& ContextPos, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params, const FVector& Extent)
{
	FCollisionQueryParams TraceParams(Params);
	TraceParams.AddIgnoredActor(ItemActor);

	const bool bHit = World->SweepTestByChannel(ItemPos, ContextPos, FQuat::Identity, Channel, FCollisionShape::MakeSphere(Extent.X), TraceParams);
	return bHit;
}

bool UEnvQueryTest_TraceHealthScore::RunCapsuleTraceTo(const FVector& ItemPos, const FVector& ContextPos, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params, const FVector& Extent)
{
	FCollisionQueryParams TraceParams(Params);
	TraceParams.AddIgnoredActor(ItemActor);

	const bool bHit = World->SweepTestByChannel(ContextPos, ItemPos, FQuat::Identity, Channel, FCollisionShape::MakeCapsule(Extent.X, Extent.Z), TraceParams);
	return bHit;
}

bool UEnvQueryTest_TraceHealthScore::RunCapsuleTraceFrom(const FVector& ItemPos, const FVector& ContextPos, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params, const FVector& Extent)
{
	FCollisionQueryParams TraceParams(Params);
	TraceParams.AddIgnoredActor(ItemActor);

	const bool bHit = World->SweepTestByChannel(ItemPos, ContextPos, FQuat::Identity, Channel, FCollisionShape::MakeCapsule(Extent.X, Extent.Z), TraceParams);
	return bHit;
}
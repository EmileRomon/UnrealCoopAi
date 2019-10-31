// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_TraceHealthScore.generated.h"

/**
 * Copy of UENvQueryTest_Trace since I didn't manage to extend it properly
 */
UCLASS()
class FPS_API UEnvQueryTest_TraceHealthScore : public UEnvQueryTest
{
	GENERATED_BODY()
	
public:

	UEnvQueryTest_TraceHealthScore(const FObjectInitializer& ObjectInitializer);

	/** trace data */
	UPROPERTY(EditDefaultsOnly, Category = Trace)
	FEnvTraceData TraceData;

	/** trace direction */
	UPROPERTY(EditDefaultsOnly, Category = Trace)
	FAIDataProviderBoolValue TraceFromContext;

	/** Z offset from item */
	UPROPERTY(EditDefaultsOnly, Category = Trace, AdvancedDisplay)
	FAIDataProviderFloatValue ItemHeightOffset;

	/** Z offset from querier */
	UPROPERTY(EditDefaultsOnly, Category = Trace, AdvancedDisplay)
	FAIDataProviderFloatValue ContextHeightOffset;

	/** context: other end of trace test */
	UPROPERTY(EditDefaultsOnly, Category = Trace)
		TSubclassOf<UEnvQueryContext> Context;

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

	virtual FText GetDescriptionTitle() const override;
	virtual FText GetDescriptionDetails() const override;

	virtual void PostLoad() override;

protected:

	DECLARE_DELEGATE_RetVal_SevenParams(bool, FRunTraceSignature, const FVector&, const FVector&, AActor*, UWorld*, enum ECollisionChannel, const FCollisionQueryParams&, const FVector&);

	bool RunLineTraceTo(const FVector& ItemPos, const FVector& ContextPos, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params, const FVector& Extent);
	bool RunLineTraceFrom(const FVector& ItemPos, const FVector& ContextPos, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params, const FVector& Extent);
	bool RunBoxTraceTo(const FVector& ItemPos, const FVector& ContextPos, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params, const FVector& Extent);
	bool RunBoxTraceFrom(const FVector& ItemPos, const FVector& ContextPos, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params, const FVector& Extent);
	bool RunSphereTraceTo(const FVector& ItemPos, const FVector& ContextPos, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params, const FVector& Extent);
	bool RunSphereTraceFrom(const FVector& ItemPos, const FVector& ContextPos, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params, const FVector& Extent);
	bool RunCapsuleTraceTo(const FVector& ItemPos, const FVector& ContextPos, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params, const FVector& Extent);
	bool RunCapsuleTraceFrom(const FVector& ItemPos, const FVector& ContextPos, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params, const FVector& Extent);

};

// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryGenerator_FPSCharAround.h"
#include "FPSCharacter.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"


void UEnvQueryGenerator_FPSCharAround::GenerateItems(FEnvQueryInstance & QueryInstance) const
{
	if (SearchedActorClass == nullptr)
	{
		return;
	}

	UObject* QueryOwner = QueryInstance.Owner.Get();
	if (QueryOwner == nullptr)
	{
		return;
	}

	UWorld* World = GEngine->GetWorldFromContextObject(QueryOwner, EGetWorldErrorMode::LogAndReturnNull);
	if (World == nullptr)
	{
		return;
	}

	GenerateOnlyActorsInRadius.BindData(QueryOwner, QueryInstance.QueryID);
	bool bUseRadius = GenerateOnlyActorsInRadius.GetValue();

	TArray<AActor*> MatchingActors;
	if (bUseRadius)
	{
		TArray<FVector> ContextLocations;
		QueryInstance.PrepareContext(SearchCenter, ContextLocations);

		SearchRadius.BindData(QueryOwner, QueryInstance.QueryID);
		const float RadiusValue = SearchRadius.GetValue();
		const float RadiusSq = FMath::Square(RadiusValue);

		for (TActorIterator<AActor> ItActor = TActorIterator<AActor>(World, SearchedActorClass); ItActor; ++ItActor)
		{
			for (int32 ContextIndex = 0; ContextIndex < ContextLocations.Num(); ++ContextIndex)
			{
				if (FVector::DistSquared(ContextLocations[ContextIndex], ItActor->GetActorLocation()) < RadiusSq)
				{
					if (ShouldAddActor(*ItActor, QueryInstance))
					{
						MatchingActors.Add(*ItActor);

					}
					break;
				}
			}
		}
	}
	else
	{	// If radius is not positive, ignore Search Center and Search Radius and just return all actors of class.
		for (TActorIterator<AActor> ItActor = TActorIterator<AActor>(World, SearchedActorClass); ItActor; ++ItActor)
		{
			if (ShouldAddActor(*ItActor, QueryInstance))
			{
				MatchingActors.Add(*ItActor);
			}
		}
	}

	ProcessItems(QueryInstance, MatchingActors);
	QueryInstance.AddItemData<UEnvQueryItemType_Actor>(MatchingActors);
}

bool UEnvQueryGenerator_FPSCharAround::ShouldAddActor(AActor *Actor, FEnvQueryInstance& QueryInstance) const
{
	AFPSCharacter *Char = Cast<AFPSCharacter>(Actor);
	AFPSCharacter *Querier = Cast<AFPSCharacter>(QueryInstance.Owner.Get());
	return Char && Querier && Char != Querier;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "CCWorld.h"
#include "Chunk/ChunkBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACCWorld::ACCWorld()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACCWorld::BeginPlay()
{
	Super::BeginPlay();

	switch (GenerationType)
	{
	case EGenerationType::GT_3D:
		Generate3DWorld();
		break;
	case EGenerationType::GT_2D:
		Generate2DWorld();
		break;
	default:
		throw std::invalid_argument("Invalid Generation Type");
	}
	UE_LOG(LogTemp, Warning, TEXT("% Chunks Created"), ChunkCount);
}

void ACCWorld::Generate3DWorld()
{
	for (int x = - DrawDistance; x <= DrawDistance; x++)
	{
		for (int y = -DrawDistance; y <= DrawDistance; ++y)
		{
			for (int z = -DrawDistance; z <= DrawDistance; ++z)
			{
				auto transform = FTransform(
					FRotator::ZeroRotator,
					FVector(x * Size * 100, y * Size * 100, z * Size * 100),
					FVector::OneVector
				);

				const auto chunk = GetWorld()->SpawnActorDeferred<AChunkBase>(
					ChunkType,
					transform,
					this
				);

				chunk->GenerationType = EGenerationType::GT_3D;
				chunk->Frequency = Frequency;
				chunk->Material = Material;
				chunk->Size = Size;

				UGameplayStatics::FinishSpawningActor(chunk, transform);

				ChunkCount++;
			}
		}
	}
}

void ACCWorld::Generate2DWorld()
{
	for (int x = -DrawDistance; x <= DrawDistance; x++)
	{
		for (int y = -DrawDistance; y <= DrawDistance; ++y)
		{
			auto transform = FTransform(
				FRotator::ZeroRotator,
				FVector(x * Size * 100, y * Size * 100, 0),
				FVector::OneVector
			);

			const auto chunk = GetWorld()->SpawnActorDeferred<AChunkBase>(
				ChunkType,
				transform,
				this
			);

			chunk->GenerationType = EGenerationType::GT_2D;
			chunk->Frequency = Frequency;
			chunk->Material = Material;
			chunk->Size = Size;

			UGameplayStatics::FinishSpawningActor(chunk, transform);

			ChunkCount++;
		}
	}
}
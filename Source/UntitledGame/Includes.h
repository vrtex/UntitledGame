#pragma once

#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Runtime/Engine/Classes/GameFramework/Pawn.h"
#include "Runtime/Engine/Public/DestructibleInterface.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/DamageType.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Components/ActorComponent.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardData.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/Engine/Classes/GameFramework/Character.h"
#include "Runtime/Engine/Classes/Components/ChildActorComponent.h"
#include "Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"



// Teams:
// 0: Neutral
// 1: Generic player
// 2: Generic enemy

#define GenericNeutralTeam uint8(0)
#define GenericPlayerTeam uint8(1)
#define GenericEnemyTeam uint8(2)


// custom trace channels:

// all enemies block this:
#define ECC_Damagable ECC_GameTraceChannel1
#define ECC_CursorTrace ECC_GameTraceChannel2
// all pickup items block this:
#define ECC_Item ECC_GameTraceChannel3
#define ECC_Ground ECC_GameTraceChannel4
#define ECC_NPC ECC_GameTraceChannel5


// Collision presets:
#define GroundCollisionPreset FName("GroundPreset")
#define ItemCollisionPreset FName("ItemPreset")
#define DamagableCollisionPreset FName("DamagablePreset")
#define PlayerCollisionPreset FName("PlayerPreset")
#define NPCCollisionPreset FName("NPCPreset")



// Blackboard keys
// Base enemy:
#define BaseEnemyAttackRangeKey FName("AttackRange")
#define BaseEnemyTargetActorKey FName("TargetActor") 
#define BaseEnemyRandomLocationKey FName("RandomLocation")
#define BaseEnemyIsTargetInRangeKey FName("IsTargetInRange")

// Fill out your copyright notice in the Description page of Project Settings.

#include "Dev/AnimNotifies/EquipFinishedAnimNotify.h"

void UEquipFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    OnNotified.Broadcast(MeshComp);

    Super::Notify(MeshComp, Animation, EventReference);
}

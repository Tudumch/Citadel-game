// Fill out your copyright notice in the Description page of Project Settings.

#include "Dev/AnimNotifies/AnimationFinishedAnimNotify.h"

void UAnimationFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    OnNotified.Broadcast(MeshComp);

    Super::Notify(MeshComp, Animation, EventReference);
}

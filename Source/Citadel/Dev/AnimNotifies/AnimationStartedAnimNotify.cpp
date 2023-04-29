// Fill out your copyright notice in the Description page of Project Settings.

#include "Dev/AnimNotifies/AnimationStartedAnimNotify.h"

void UAnimationStartedAnimNotify::Notify(USkeletalMeshComponent* MeshComp,
    UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    OnNotified.Broadcast(MeshComp);

    Super::Notify(MeshComp, Animation, EventReference);
}

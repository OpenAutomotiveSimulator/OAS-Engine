// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "PaperSpriteThumbnailRenderer.h"
#include "PaperFlipbookThumbnailRenderer.generated.h"

class FCanvas;
class FRenderTarget;

UCLASS()
class UPaperFlipbookThumbnailRenderer : public UPaperSpriteThumbnailRenderer
{
	GENERATED_UCLASS_BODY()

	// UThumbnailRenderer interface
	virtual void Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget*, FCanvas* Canvas, bool bAdditionalViewFamily) override;
	// End of UThumbnailRenderer interface
};

#if UE_ENABLE_INCLUDE_ORDER_DEPRECATED_IN_5_2
#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#endif
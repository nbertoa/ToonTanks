#include "PlayerControllerBase.h"

void 
APlayerControllerBase::SetPlayerEnabledState(const bool aEnable) {
    check(GetPawn() != nullptr);

    if (aEnable) {
        GetPawn()->EnableInput(this);
    } else {
        GetPawn()->DisableInput(this);
    }

    bShowMouseCursor = aEnable;
}

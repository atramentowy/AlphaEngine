#ifndef DEBUG_H
#define DEBUG_H

#include <raylib.h>
#include <rlgl.h>

bool wireframeEnabled = false;
bool backfaceCullingEnabled = true;

void UpdateDebug() {
    if (IsKeyPressed(KEY_F1)) { 
        if (wireframeEnabled) {
            rlDisableWireMode();
        } else {
            rlEnableWireMode();
        }
        wireframeEnabled = !wireframeEnabled;
    }

    if(IsKeyPressed(KEY_F2)) {
        if (backfaceCullingEnabled) {
            rlDisableBackfaceCulling();
        } else {
            rlEnableBackfaceCulling();
        }
        backfaceCullingEnabled = !backfaceCullingEnabled;
    }
}

#endif
/*!
 * \headerfile handler.h "game/handler.h"
 */
#include "game/handler.h"

string Handler::keyManager() {
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        return "<Escape>";
    }
    if (Keyboard::isKeyPressed(Keyboard::Space)) {
        return "<Space>";
    }
    if (Keyboard::isKeyPressed(Keyboard::Enter)) {
        return "<Enter>";
    }
    if (Keyboard::isKeyPressed(Keyboard::P)) {
        return "<P>";
    }
    if (Keyboard::isKeyPressed(Keyboard::Up)) {
        return "<Up>";
    }
    if (Keyboard::isKeyPressed(Keyboard::Left)) {
        return "<Left>";
    }
    if (Keyboard::isKeyPressed(Keyboard::Right)) {
        return "<Right>";
    }
    if (Keyboard::isKeyPressed(Keyboard::A)) {
        return "<A>";
    }
    if (Keyboard::isKeyPressed(Keyboard::D)) {
        return "<D>";
    }
    else {
        return "<>";
    }
}
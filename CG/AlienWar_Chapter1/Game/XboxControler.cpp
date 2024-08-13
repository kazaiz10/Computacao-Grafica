#include "XboxControler.h"

XboxControler::XboxControler() {
	this->Connect = 0;
	this->Sensitivity = 0;
}

XboxControler::XboxControler(int Sensitivity) {
	this->Connect = 0;
	this->Sensitivity = Sensitivity;
}

short XboxControler::getConnectState() { return this->Connect; }
int XboxControler::getSensitivity() { return this->Sensitivity; }

void XboxControler::setSensitivity(int Sensitivity) { this->Sensitivity = Sensitivity; }

void XboxControler::init() {
    ZeroMemory(&this->state, sizeof(XINPUT_STATE));
    this->dwResult = XInputGetState(0, &this->state);
    ZeroMemory(&this->state, sizeof(XINPUT_STATE));
    XInputGetState(0, &this->state);
}

void XboxControler::start(float* varX, float* varZ, float* Zoom, float* DeltaTime, Player* player, Projectile* Pro, float deltatime, float* FireTime) {
    if (dwResult == ERROR_SUCCESS) {
        if (state.Gamepad.sThumbLX > this->Sensitivity || state.Gamepad.sThumbLX < -this->Sensitivity)
            *varX = *varX + ((player->getVelocity() * state.Gamepad.sThumbLX) / 32000.0f) * *DeltaTime;
        if (state.Gamepad.sThumbLY > this->Sensitivity || state.Gamepad.sThumbLY < -this->Sensitivity)
            *varZ = *varZ + ((player->getVelocity() * state.Gamepad.sThumbLY) / 32000.0f) * *DeltaTime;
        if (state.Gamepad.sThumbRY > this->Sensitivity || state.Gamepad.sThumbRY < -this->Sensitivity)
            *Zoom = *Zoom + state.Gamepad.sThumbRY / 1000 * *DeltaTime;
        if (this->state.Gamepad.wButtons == XINPUT_GAMEPAD_A) {
            if (deltatime >= 1.0f / player->getFireRate()) {
                Pro->newProjectile(15.0f, glm::vec3(player->getPosition().x, 0.3f, player->getPosition().z), player->getAngle(), glm::vec3(0.0f, 1.0f, 0.0f));
                *FireTime = glfwGetTime();
            }
        }
        if (this->state.Gamepad.wButtons == XINPUT_GAMEPAD_RIGHT_SHOULDER) player->rotate(0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
        if (this->state.Gamepad.wButtons == XINPUT_GAMEPAD_LEFT_SHOULDER) player->rotate(-0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
        ZeroMemory(&this->state, sizeof(XINPUT_STATE));
        XInputGetState(0, &this->state);
    }
}
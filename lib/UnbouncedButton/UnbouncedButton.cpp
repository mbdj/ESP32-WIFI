#include <UnbouncedButton.h>

UnbouncedButton::UnbouncedButton(uint8_t pin, uint8_t mode, unsigned long debounceDelay, Edge edge)
    : pin{pin}, mode{mode}, debounceDelay{debounceDelay}, edge{edge}
{
    pinMode(pin, mode);
};


UnbouncedButton::ButtonState UnbouncedButton::buttonState(void)
{
    ButtonState buttonState = ButtonState::UNCHANGED;

    // une seconde lecture est effectuée après le délai de rebond
    int buttonPreviousState = digitalRead(pin);
    delay(debounceDelay);
    int buttonPinRead = digitalRead(pin);

    // On détecte le front montant ou descendant selon  PULLUP ou PULLDOWN
    if (edge == Edge::PRESSED)
    {
        if ((mode == INPUT_PULLDOWN && buttonPinRead == HIGH && buttonPreviousState == LOW)|| (mode == INPUT_PULLUP && buttonPinRead == LOW && buttonPreviousState == HIGH))
            buttonState = ButtonState::PRESSED;
    }
    else if ((mode == INPUT_PULLDOWN && buttonPinRead == LOW && buttonPreviousState == HIGH) || (mode == INPUT_PULLUP && buttonPinRead == HIGH && buttonPreviousState == LOW))
        buttonState = ButtonState::PRESSED;

    return buttonState;
};

bool UnbouncedButton::buttonPressed(void) { return buttonState() == ButtonState::PRESSED; };
bool UnbouncedButton::buttonUnchanged(void) { return buttonState() == ButtonState::UNCHANGED; };
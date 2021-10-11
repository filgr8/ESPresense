#include "GUI.h"

GUI Display;

void GUI::seenStart()
{
#ifdef M5ATOM
    M5.dis.drawpix(0, CRGB(15, 15, 15));
#else
    digitalWrite(LED_BUILTIN, LED_BUILTIN_ON);
#endif
}

void GUI::seenEnd()
{
#ifdef M5ATOM
    M5.dis.drawpix(0, CRGB(0, 0, 0));
#else
    digitalWrite(LED_BUILTIN, !LED_BUILTIN_ON);
#endif
}

void GUI::erasing()
{
    status("Erasing...");
    Serial.println(F("Resetting back to defaults..."));
}

void GUI::erased()
{
}

void GUI::connecting()
{
    status("Connecting...");
    connected(false, false);
#ifdef LED_BUILTIN
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
#endif
}

void GUI::connected(bool wifi = false, bool mqtt = false)
{
#ifdef M5ATOM
    if (!wifi)
        M5.dis.drawpix(0, CRGB(0, 128, 0));
    else if (!mqtt)
        M5.dis.drawpix(0, CRGB(0, 0, 128));
    else
        M5.dis.drawpix(0, CRGB(0, 0, 0));
#else
#ifdef LED_BUILTIN
    digitalWrite(LED_BUILTIN, !LED_BUILTIN_ON);
#endif
    status("Wifi: %s Mqtt: %s", (wifi ? "yes" : "no"), (mqtt ? "yes" : "no"));
#endif
}

void GUI::added(BleFingerprint *f)
{
    if (f->getIgnore()) return;
    Serial.printf("%d New   | MAC: %s, ID: %-60s %s\n", xPortGetCoreID(), f->getMac().c_str(), f->getId().c_str(), f->getDiscriminator().c_str());
}

void GUI::removed(BleFingerprint *f, long age)
{
    if (f->getIgnore()) return;
    Serial.printf("\u001b[31m%d Del   | MAC: %s, ID: %-60s (%lus)\u001b[0m\n", xPortGetCoreID(), f->getMac().c_str(), f->getId().c_str(), age / 1000);
}

void GUI::close(BleFingerprint *f)
{
    if (f->getIgnore()) return;
    Serial.printf("\u001b[32m%d Close | MAC: %s, ID: %-60s (%.2fm) %ddBm\u001b[0m\n", xPortGetCoreID(), f->getMac().c_str(), f->getId().c_str(), f->getDistance(), f->getNewestRssi());
    status("C: %s", f->getId().c_str());
}

void GUI::left(BleFingerprint *f)
{
    if (f->getIgnore()) return;
    Serial.printf("\u001b[33m%d Left  | MAC: %s, ID: %-60s (%.2fm) %ddBm\u001b[0m\n", xPortGetCoreID(), f->getMac().c_str(), f->getId().c_str(), f->getDistance(), f->getNewestRssi());
    status("L: %s", f->getId().c_str());
}

void GUI::status(const char *format, ...)
{
#ifdef M5STICK
    sprite.fillSprite(TFT_BLACK);
    sprite.setTextDatum(MC_DATUM);

    char *message;
    va_list args;
    va_start(args, format);
    vasprintf(&message, format, args);
    va_end(args);
#ifdef PLUS
    sprite.drawString(message, sprite.width() / 2, sprite.height() / 2, 4);
#else
    sprite.drawString(message, sprite.width() / 2, sprite.height() / 2, 1);
#endif
    free(message);
    dirty = true;
#endif
}

void GUI::update()
{
    if (!init)
    {
#ifdef M5STICK
        M5.begin(true, true, false);
        M5.Lcd.setRotation(3);
        sprite.createSprite(M5.Lcd.width(), M5.Lcd.height());
        sprite.setSwapBytes(true);
#elif defined M5ATOM
        M5.begin(false, false, true);
        M5.dis.drawpix(0, CRGB(64, 0, 0));
#endif
        init = true;
    }
#ifdef M5STICK
    if (dirty)
    {
        sprite.pushSprite(0, 0);
        M5.Axp.ScreenBreath(12);
        dirty = false;
    }
#endif
}

void GUI::updateProgress(unsigned int percent)
{
#ifdef LED_BUILTIN
    digitalWrite(LED_BUILTIN, percent % 2);
#endif
}

void GUI::updateEnd()
{
#ifdef LED_BUILTIN
    digitalWrite(LED_BUILTIN, !LED_BUILTIN_ON);
#endif
}

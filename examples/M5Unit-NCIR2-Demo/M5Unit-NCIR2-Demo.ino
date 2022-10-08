/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more
*information：https://docs.m5stack.com/en/unit/UNIT%20NCIR2
* 获取更多资料请访问：https://docs.m5stack.com/en/unit/UNIT%20NCIR2
*
* Product: M5Unit-NCIR2.
* Date：2022/10/08
*******************************************************************************
*/

#include <M5Stack.h>
#include <Wire.h>
#include "M5UNIT_NCIR2.h"
#include <M5GFX.h>

M5GFX display;
M5Canvas canvas(&display);
M5UNIT_NCIR2 ncir2;

int16_t temperature;
int16_t temperature_soc;
float emissivity;
uint8_t button;

uint16_t low_alarm_freq, high_alarm_freq, low_alarm_interval,
    high_alarm_interval, buzz_freq;
uint8_t low_alarm_duty, high_alarm_duty, duty;
int16_t low_alarm_temp, high_alarm_temp;

enum {
    D_WRITE_TEST = 0,
    D_READ_TEST,
    PWM_TEST,
    ADC_TEST,
    RGB_TEST,
    SERVO_TEST,
    SERVO_TEST_AUTO,
    MULTI_TEST
};

void setup() {
    M5.begin(true, false, true);
    M5.Power.begin();
    display.begin();
    ncir2.begin(&Wire, 21, 22, M5UNIT_NCIR2_DEFAULT_ADDR);
    canvas.setColorDepth(8);  // mono color
    canvas.setFont(&fonts::efontCN_12);
    canvas.createSprite(display.width(), display.height());
    // ncir2.setEmissivity(0.95);
    emissivity = 0.95;
    emissivity = ncir2.getEmissivity();

    ncir2.setLEDColor(0x001100);

    ncir2.setAlarmBuzzFreq(M5_NCIR_ALARM_LOW, 4000);
    ncir2.getAlarmBuzzFreq(M5_NCIR_ALARM_LOW, &low_alarm_freq);
    ncir2.setAlarmBuzzDuty(M5_NCIR_ALARM_LOW, 204);
    ncir2.getAlarmBuzzDuty(M5_NCIR_ALARM_LOW, &low_alarm_duty);
    ncir2.setAlarmBuzzInter(M5_NCIR_ALARM_LOW, 200);
    ncir2.getAlarmBuzzInter(M5_NCIR_ALARM_LOW, &low_alarm_interval);
    ncir2.setAlarmLED(M5_NCIR_ALARM_LOW, 0x000011);

    ncir2.setAlarmBuzzFreq(M5_NCIR_ALARM_HIGH, 4000);
    ncir2.getAlarmBuzzFreq(M5_NCIR_ALARM_HIGH, &high_alarm_freq);
    ncir2.setAlarmBuzzDuty(M5_NCIR_ALARM_HIGH, 204);
    ncir2.getAlarmBuzzDuty(M5_NCIR_ALARM_HIGH, &high_alarm_duty);
    ncir2.setAlarmBuzzInter(M5_NCIR_ALARM_HIGH, 100);
    ncir2.getAlarmBuzzInter(M5_NCIR_ALARM_HIGH, &high_alarm_interval);
    ncir2.setAlarmLED(M5_NCIR_ALARM_HIGH, 0x110000);

    ncir2.setAlarmTemp(M5_NCIR_ALARM_LOW, 1000);
    ncir2.getAlarmTemp(M5_NCIR_ALARM_LOW, &low_alarm_temp);
    ncir2.setAlarmTemp(M5_NCIR_ALARM_HIGH, 3700);
    ncir2.getAlarmTemp(M5_NCIR_ALARM_HIGH, &high_alarm_temp);
}

void loop() {
    temperature     = ncir2.getTempValue();
    temperature_soc = ncir2.getSocTempValue();
    emissivity      = ncir2.getEmissivity();
    button          = ncir2.getButtonStatus();
    canvas.fillSprite(BLACK);
    canvas.setTextSize(2);
    canvas.setCursor(100, 0);
    canvas.setTextColor(YELLOW);
    canvas.printf("NCIR2 Test");
    canvas.setTextColor(ORANGE);
    canvas.drawLine(25, 25, 310, 25);
    canvas.setCursor(0, 30);
    canvas.setTextColor(WHITE);
    canvas.printf("温度:");
    canvas.setTextColor(GREEN);
    canvas.printf("%d", temperature);
    canvas.setCursor(0, 60);
    canvas.setTextColor(RED);
    canvas.printf("高温:%d", high_alarm_temp);
    canvas.setTextColor(BLUE);
    canvas.printf("低温:%d", low_alarm_temp);
    canvas.setCursor(0, 90);
    canvas.setTextColor(WHITE);
    canvas.printf("发射率:");
    canvas.setTextColor(GREEN);
    canvas.printf("%.2f", emissivity);
    canvas.setCursor(0, 120);
    canvas.setTextColor(WHITE);
    canvas.printf("芯片温度:");
    canvas.setTextColor(GREEN);
    canvas.printf("%d", temperature_soc);
    canvas.setCursor(0, 150);
    canvas.setTextColor(WHITE);
    canvas.printf("按钮");
    canvas.setTextColor(GREEN);
    canvas.printf("%d", button);
    canvas.setTextColor(WHITE);
    canvas.fillRect(5, 200, 150, 40, BLACK);
    canvas.setCursor(45, 190);
    canvas.printf("save");
    canvas.setCursor(25, 210);
    canvas.printf("config");
    canvas.setCursor(125, 190);
    canvas.printf("发射率");
    canvas.setCursor(150, 210);
    canvas.printf("+");
    canvas.setCursor(210, 190);
    canvas.printf("发射率");
    canvas.setCursor(245, 210);
    canvas.printf("-");
    M5.update();
    if (M5.BtnA.wasPressed()) {
        ncir2.setConfig();
        canvas.setCursor(20, 140);
        canvas.printf("config saved");
    } else if (M5.BtnB.wasPressed()) {
        if (emissivity < 1.0) {
            emissivity += 0.01;
            ncir2.setEmissivity(emissivity);
        }
    } else if (M5.BtnC.wasPressed()) {
        if (emissivity > 0.1) {
            emissivity -= 0.01;
            ncir2.setEmissivity(emissivity);
        }
    }

    canvas.pushSprite(0, 0);
}

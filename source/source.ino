#include "lcd.h"
#include "clock.h"
#include "remote.h"

#include "Arduino.h"

static constexpr auto IR_PIN = 7;
static constexpr auto BUZZER_PIN = 10;

enum class mode_t
{
	SHOW_TIME,
	SET_ALARM,
	ALARM_RINGING
};
static mode_t curr_mode = mode_t::SHOW_TIME;

// temporary storage for storing data while setting the alarm
static clock::time_t new_alarm;
static uint8_t digit_idx = 0;
static int8_t input_digits[2] = {-1, -1};  // this is so that we can hold up to 2 digits of input

static uint8_t problem_a = 0;
static uint8_t problem_b = 0;
static int     answer    = -1;

static unsigned long last_beep_ms = 0;
static int beep_phase = 0;


// sorry, my code was much cleaner until i started jamming everything in here
void alarm_three_beeps()
{
	for (int i = 0; i < 3; i++)
	{
		digitalWrite(BUZZER_PIN, HIGH);
		delay(100);
		digitalWrite(BUZZER_PIN, LOW);
		delay(50);
	}
	delay(1500);
}

void setup()
{
	pinMode(BUZZER_PIN, OUTPUT);
	digitalWrite(BUZZER_PIN, LOW);

	lcd::init();
	clock::init();
	remote::init(IR_PIN);

	randomSeed(analogRead(millis())); // seed the random for later
}

void loop()
{
    clock::update_clock_time();
    int8_t btn = remote::get_button_idx();

    // did the alarm just go off?
    if (clock::check_alarm() && curr_mode != mode_t::ALARM_RINGING)
	{
        curr_mode = mode_t::ALARM_RINGING;
        lcd::clear();
        answer = -1;
        input_digits[0] = input_digits[1] = -1;
        problem_a = random(1, 10);
        problem_b = random(1, 10);
    }

    // handle user input
    if (curr_mode == mode_t::ALARM_RINGING)
	{
        int8_t digit = remote::idx_to_digit(btn);
        if (digit >= 0)
		{
            if (input_digits[0] < 0)
			{
                input_digits[0] = digit;
            }
            else if (input_digits[1] < 0)
			{
                input_digits[1] = digit;
                answer = input_digits[0]*10 + input_digits[1];

                // stop or reset
                if (answer == (problem_a + problem_b))
				{
                    curr_mode = mode_t::SHOW_TIME;
                    lcd::clear();
                }
				else
				{
                    answer = -1;
                    input_digits[0] = input_digits[1] = -1;
                }
            }
        }
    }
    else if (curr_mode == mode_t::SHOW_TIME)
	{
        if (btn == 2) {
            curr_mode = mode_t::SET_ALARM;
            lcd::clear();
            new_alarm = clock::get_alarm_time();
            digit_idx = 0;
        }
    }
    else if (curr_mode == mode_t::SET_ALARM)
	{
        int8_t digit = remote::idx_to_digit(btn);
        if (digit >= 0 && digit_idx < 4) {
            switch (digit_idx) {
                case 0: new_alarm.hours   = digit * 10; break;
                case 1: new_alarm.hours  += digit;      break;
                case 2: new_alarm.minutes = digit * 10; break;
                case 3: new_alarm.minutes+= digit;      break;
            }
            digit_idx++;
            if (digit_idx == 4) {
                new_alarm.seconds = 0;
                clock::set_alarm_time(new_alarm);
                curr_mode = mode_t::SHOW_TIME;
                lcd::clear();
            }
        }
    }

	// display
    switch (curr_mode)
	{
    case mode_t::SHOW_TIME:
        lcd::print1(" Time: %s", clock::get_clock_time_string());
        lcd::print2("Alarm: %s", clock::get_alarm_time_string());
        break;

    case mode_t::SET_ALARM:
	{
        char buf[5] = {'_','_','_','_','\0'};
        if (digit_idx > 0) buf[0] = '0' + new_alarm.hours / 10;
        if (digit_idx > 1) buf[1] = '0' + new_alarm.hours % 10;
        if (digit_idx > 2) buf[2] = '0' + new_alarm.minutes / 10;
        if (digit_idx > 3) buf[3] = '0' + new_alarm.minutes % 10;
        lcd::print1("Set Alarm:");
        lcd::print2(" %c%c:%c%c", buf[0], buf[1], buf[2], buf[3]);
        break;
    }

    case mode_t::ALARM_RINGING:
	{
        char typed[4] = {'_', '_', '\0'};
        if (input_digits[0] >= 0) typed[0] = '0' + input_digits[0];
        if (input_digits[1] >= 0) typed[1] = '0' + input_digits[1];

        lcd::print1(" %d + %d = ?", problem_a, problem_b);
        lcd::print2("  Ans: %s", typed);

		if (answer == (problem_a + problem_b))
        {
            curr_mode = mode_t::SHOW_TIME;
            digitalWrite(BUZZER_PIN, LOW);
            beep_phase = 0;
            last_beep_ms = millis();
            digitalWrite(BUZZER_PIN, LOW);

            lcd::clear();

            return; // stop running the buzzer logic this loop so it doesn't just stay on
        }

        unsigned long now = millis();
        if (beep_phase % 2 == 0) {
            // even phases: buzz on
            if (now - last_beep_ms > 100) {
                digitalWrite(BUZZER_PIN, LOW);
                beep_phase++;
                last_beep_ms = now;
            }
        } else {
            // odd phases: buzz off
            if (now - last_beep_ms > 50) {
                if (beep_phase < 5) {
                    digitalWrite(BUZZER_PIN, HIGH);
                } else {
                    digitalWrite(BUZZER_PIN, LOW);
                }
                beep_phase++;
                last_beep_ms = now;
                if (beep_phase > 6) {
                    beep_phase = 0;
                    last_beep_ms = now + 1500; // wait 2s before looping again
                }
            }
        }

        break;
    }
	}

    delay(100);
}

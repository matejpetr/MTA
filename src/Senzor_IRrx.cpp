#include "Senzor_IRrx.hpp"
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>



static IRrecv* s_irrecv = nullptr;
static decode_results s_res;
static int s_currentPin = -1;
static bool s_enabled = false;

static TaskHandle_t s_taskHandle  = nullptr;

// Poslední zachycená hodnota pro update()
static volatile uint32_t      s_latestVal    = 0;
static volatile bool          s_haveAny      = false;
static volatile unsigned long s_lastStoreMs  = 0;

// Dedup (na úrovni příjmu)
static uint32_t       s_lastValRx   = 0;
static unsigned long  s_lastMsRx    = 0;
static unsigned long  s_dedupMsCfg  = 150; // nastavuje IRrx_update()

// Krátký spinlock pro sdílená data
static portMUX_TYPE   s_spin = portMUX_INITIALIZER_UNLOCKED;

static String toHex10(uint32_t v) {
  char out[11]; // "0x" + 8 + '\0'
  snprintf(out, sizeof(out), "0x%08lX", (unsigned long)v);
  return String(out);
}

static void ensureIrRecv(int pin) {
  if (s_irrecv == nullptr || s_currentPin != pin) {
    if (s_irrecv) { delete s_irrecv; s_irrecv = nullptr; }
    s_irrecv = new IRrecv(pin);
    s_currentPin = pin;
    s_enabled = false;
  }
  if (!s_enabled) {
    s_irrecv->setTolerance(40);        // benevolentnější časování
    s_irrecv->setUnknownThreshold(12); // odfiltruj krátký šum
    s_irrecv->enableIRIn();
    s_enabled = true;
  }
}

static void IRReaderTask(void* pv) {
  const int pin = (int)(intptr_t)pv;
  ensureIrRecv(pin);

  for (;;) {
    if (s_irrecv && s_irrecv->decode(&s_res)) {
      uint32_t v = (uint32_t)s_res.value;

      // ignoruj repeat a příliš rychlé duplikáty stejné hodnoty
      bool isRepeat    = s_res.repeat;
      bool tooSoonSame = (millis() - s_lastMsRx) < s_dedupMsCfg && v == s_lastValRx;

      if (!isRepeat && !tooSoonSame) {
        portENTER_CRITICAL(&s_spin);
        s_latestVal   = v;
        s_haveAny     = true;
        s_lastStoreMs = millis();
        portEXIT_CRITICAL(&s_spin);

        s_lastValRx = v;
        s_lastMsRx  = millis();
      }

      s_irrecv->resume();
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

static void startIrTaskIfNeeded(int pin) {
  ensureIrRecv(pin);
  if (!s_taskHandle) {
    // stack 4096, priorita 1, core 0
    xTaskCreatePinnedToCore(IRReaderTask, "IRReader",
                            4096, (void*)(intptr_t)pin, 1, &s_taskHandle, 0);
  }
}

void IRrx_init() {}

void IRrx_reset() {
  // vypni task a uvolni přijímač; další update vše znovu nastartuje
  if (s_taskHandle) {
    vTaskDelete(s_taskHandle);
    s_taskHandle = nullptr;
  }
  if (s_irrecv) {
    delete s_irrecv;
    s_irrecv = nullptr;
  }
  s_enabled    = false;
  s_currentPin = -1;

  portENTER_CRITICAL(&s_spin);
  s_haveAny     = false;
  s_latestVal   = 0;
  s_lastStoreMs = 0;
  portEXIT_CRITICAL(&s_spin);

  s_lastValRx  = 0;
  s_lastMsRx   = 0;
}

void IRrx_update(int pin, unsigned long dedupMs) {
  s_dedupMsCfg = dedupMs;   // aktualizuj dedup okno
  startIrTaskIfNeeded(pin); // zajisti běžící příjem

  // vezmi POSLEDNÍ známou hodnotu
  uint32_t v; bool have;
  portENTER_CRITICAL(&s_spin);
  v    = s_latestVal;
  have = s_haveAny;
  portEXIT_CRITICAL(&s_spin);


  String out = "?type=IRrx&id=12&code=" + toHex10(v);
  if (ResponseAll) globalBuffer += out;
  else Serial.println(out);
  
}
